#define WIN32_LEAN_AND_MEAN
#include <string>
#include <stdexcept>
#include <sstream>
#include <msclr/gcroot.h>
#include "Furnarchy.h"
#include "common/DirStack.h"

using namespace System;
using namespace System::Reflection;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace std;
using namespace furn2;

/** Defines a module entry. */
private ref class FurnarchyModule
{
public:
   UInt32                 id;
   Assembly^              assembly;
   Type^                  module_class;
   Furnarchy::ModuleHost^ host;
   Furnarchy::IFurnarchyModule^ module;
};

//////////////////////////////////////////////////////////////////////////

msclr::gcroot<List<FurnarchyModule^>^> g_modules;

//////////////////////////////////////////////////////////////////////////

Assembly^ onAssemblyResolve( Object^ sender, ResolveEventArgs^ e );

/************************************************************************/
/* Managed entry points.                                                */
/************************************************************************/

bool _create( unsigned int me, const char* root, long flags )
{
   if (!g_modules)   // First time creating a module?
   {
      // Create the modules list.
      g_modules = gcnew List<FurnarchyModule^>;
      /* 
         Gotta set up an assembly resolve handler to return this cli assembly otherwise
         CLI modules won't be able to load it as a dependency because we're not in
         the appbase, or in the probe paths (which have to be within the appbase anyway),
         or in the module's directory, or in the GAC. 
         The assembly fusion system pretty much considers this instance of
         cli.dll a completely different assembly because we aren't where we should be
         (even though we have the same strong fullname) and will load and bind to a copy 
         (independent of furnarchy) if we copied ourselves to any of those locations.
      */
      AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler( onAssemblyResolve );
   }


   FurnarchyModule^ m = gcnew FurnarchyModule;
   m->id = me;

   /* Load the dll bearing the same name as the folder. */
   string lib_path = root;

   string lib_name;
   {
      size_t f = lib_path.find_last_of( '\\' );
      if (f != lib_path.npos)
         lib_name.assign( lib_path, f + 1, lib_path.npos );
      else
         lib_name = lib_path;
   }

   lib_path += '\\';
   lib_path += lib_name;
   lib_path += ".dll";
   
   /* Load it. */
   {
      DirStack cd;
      cd.push( root );
      m->assembly = Assembly::LoadFrom( gcnew String( lib_path.c_str( ) ) );
   }

   if (m->assembly == nullptr)
   {
      ostringstream ss;
      ss << "Failed to load CLI module '" << lib_name << "' (ErrCode: "
         << GetLastError( ) << ")";
      f2_errspeak( ss.str( ).c_str( ) );
      return false;
   }

   /* Find the module class. */
   try
   {
      for each (Type^ t in m->assembly->GetTypes( ))
      {
         if (t->IsClass && t->IsVisible)
         {
            Type^ iface = t->GetInterface( "IFurnarchyModule" );
            if (iface != nullptr)
            {
               if (iface->Assembly->FullName ==  Assembly::GetExecutingAssembly( )->FullName)
                  m->module_class = t;
                  break;
            }
         }
      }
   }
   catch (ReflectionTypeLoadException^ e)
   {
      array<Exception^>^ a = e->LoaderExceptions;
      System::Windows::Forms::MessageBox::Show( e->Message );
      return false;
   }

   if (m->module_class == nullptr)
   {
      ostringstream ss;
      ss << "Couldn't find IFurnarchyModule class in CLI module '" << lib_name << "'.";
      f2_errspeak( ss.str( ).c_str( ) );
      return false;
   }
   
   g_modules->Add( m );

   return true;
}

bool _destroy( unsigned int me )
{  
   List<FurnarchyModule^>^ modules = g_modules;
   for (Int32 i = 0; i < modules->Count; ++i)
   {
      if (modules[ i ]->id == me)
      {
         // Dispose.
         delete modules[ i ]->module;
         modules->RemoveAt( me );
         return true;
      }
   }

   return false;
}

void _on_inet( unsigned int me, Revision rev, const char* line,
               int from, size_t num )
{
   List<FurnarchyModule^>^ modules = g_modules;
   for (Int32 i = 0; i < modules->Count; ++i)
   {
      if (modules[ i ]->id == me)
      {
         modules[ i ]->host->onInbound( rev, line, from, num );
         return;
      }
   }
}

void _on_onet( unsigned int me, Revision rev, const char* line,
               int from, size_t num )
{
   List<FurnarchyModule^>^ modules = g_modules;
   for (Int32 i = 0; i < modules->Count; ++i)
   {
      if (modules[ i ]->id == me)
      {
         modules[ i ]->host->onOutbound( rev, line, from, num );
         return;
      }
   }
}

intptr_t _on_module( unsigned int me, Module_Event e, intptr_t param )
{
   List<FurnarchyModule^>^ modules = g_modules;
   for (Int32 i = 0; i < modules->Count; ++i)
   {
      if (modules[ i ]->id == me)
      {
         // If starting, create the module host and class.
         if (e == furn2::MOD_EVENT_START && modules[ i ]->host == nullptr)
         {
            FurnarchyModule^ m = modules[ i ];

            // Create the host.
            m->host = gcnew Furnarchy::ModuleHost( m->id );
            // Construct with IFurnarchyModule( ModuleHost^ )
            array<Object^>^ construct_args = { m->host };
            Object^ inst = nullptr;
            try {
               inst = Activator::CreateInstance( m->module_class, construct_args );
            }
            catch (Exception^ e) {
               System::Windows::Forms::MessageBox::Show( "Activator::CreateInstance threw " + e->GetType( )->Name + ": " + e->Message, 
                                                         m->assembly->GetName( )->Name );
            }

            m->module = safe_cast<Furnarchy::IFurnarchyModule^>( inst );
            if (m->module == nullptr) {
               System::Windows::Forms::MessageBox::Show( "Failed to instantiate CLI module class (" + m->module_class->AssemblyQualifiedName + ").", m->assembly->GetName( )->Name );
               throw gcnew ApplicationException( "Failed to instantiate CLI module class (" + m->module_class->AssemblyQualifiedName + ")." );
            }
         }

         return modules[ i ]->host->onModule( e, param );
      }
   }

   return 0;
}

intptr_t _on_client( unsigned int me, Client_Event e, intptr_t param )
{
   List<FurnarchyModule^>^ modules = g_modules;
   for (Int32 i = 0; i < modules->Count; ++i)
   {
      if (modules[ i ]->id == me)
      {
         return modules[ i ]->host->onClient( e, param );
      }
   }

   return 0;
}

bool _on_wndproc( unsigned int me, WNDPROCPARAMS* params )
{
   List<FurnarchyModule^>^ modules = g_modules;
   for (Int32 i = 0; i < modules->Count; ++i)
   {
      if (modules[ i ]->id == me)
      {
         return modules[ i ]->host->onWndProc( params );
      }
   }

   return false;
}   

intptr_t _on_world( unsigned int me, World_Event e, intptr_t param )
{
   List<FurnarchyModule^>^ modules = g_modules;
   for (Int32 i = 0; i < modules->Count; ++i)
   {
      if (modules[ i ]->id == me)
      {
         return modules[ i ]->host->onWorld( e, param );
      }
   }

   return 0;
}

bool _on_command( unsigned int me, int argc, const char* argv[] )
{
   List<FurnarchyModule^>^ modules = g_modules;
   for (Int32 i = 0; i < modules->Count; ++i)
   {
      if (modules[ i ]->id == me)
      {
         return modules[ i ]->host->onCommand( argc, argv );
      }
   }

   return false;
}

//////////////////////////////////////////////////////////////////////////

Assembly^ onAssemblyResolve( Object^ sender, ResolveEventArgs^ e )
{
   Assembly^ my_assembly = Assembly::GetExecutingAssembly( );
   if (e->Name == my_assembly->FullName)
      return my_assembly;
   return nullptr;
}