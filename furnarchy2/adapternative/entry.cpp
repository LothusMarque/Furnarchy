#define WIN32_LEAN_AND_MEAN
#include "../furnarchycore/furn2.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace furn2;
using namespace std;

BOOL APIENTRY DllMain( HMODULE inst, DWORD reason, LPVOID reserved )
{
   if (reason == DLL_PROCESS_ATTACH)
      DisableThreadLibraryCalls( (HMODULE) inst );
   return TRUE;
}

/** Automatically restores the initial working directory
*   upon destruction. */
class DirStack
{
private:
   std::vector<std::string> dirs;
public:
   void push( const std::string& dir ) {
      dirs.push_back( dir );
      SetCurrentDirectory( dir.c_str( ) );
   }
   void pop( ) {
      if (dirs.size( ) > 1)
      {
         dirs.pop_back( );
         SetCurrentDirectory( dirs.back( ).c_str( ) );
      }
   }
   void reset( ) {
      SetCurrentDirectory( dirs.front( ).c_str( ) );
      dirs.resize( 1 );
   }
   DirStack( ) { 
      char buf[ MAX_PATH ];
      GetCurrentDirectory( MAX_PATH, buf );
      dirs.push_back( buf );
   }
   ~DirStack( ) {
      SetCurrentDirectory( dirs.front( ).c_str( ) );
   }
};

/** Defines a module entry. */
struct MODULE_ENT
{
   typedef void(CDECL *PFN_ON_INET)( Revision rev, 
                                     const char* line, int from, size_t num );
   typedef void(CDECL *PFN_ON_ONET)( Revision rev, 
                                     const char* line, int from, size_t num );
   typedef intptr_t(CDECL *PFN_ON_MODULE)( Module_Event e, intptr_t param );
   typedef intptr_t(CDECL *PFN_ON_CLIENT)( Client_Event e, intptr_t param );
   typedef bool(CDECL *PFN_ON_WNDPROC)( WNDPROCPARAMS* params );
   typedef intptr_t(CDECL *PFN_ON_WORLD)( World_Event e, intptr_t data );
   typedef bool(CDECL *PFN_ON_COMMAND)( int argc, char* argv[] );
   
   unsigned int   me;
   HMODULE        lib;
   PFN_ON_MODULE  on_module;
   PFN_ON_INET    on_inet;
   PFN_ON_ONET    on_onet;
   PFN_ON_CLIENT  on_client;
   PFN_ON_WNDPROC on_wndproc;
   PFN_ON_WORLD   on_world;
   PFN_ON_COMMAND on_command;
};

vector<MODULE_ENT> g_modules;

/** Locate a module entry by module index. */
inline MODULE_ENT* find_module( unsigned int me )
{
   const size_t size = g_modules.size( );
   for (unsigned int i = 0; i < size; ++i)
      if (g_modules[ i ].me == me)
         return &g_modules[ i ];
   return 0;
}

/************************************************************************/
/* Adapter entry points                                                 */
/************************************************************************/

F2EXPORT bool F2CALL create( unsigned int me, const char* root, long flags )
{
   MODULE_ENT ent = { me, 0, 0, 0, 0, 0, 0, 0 };

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
      ent.lib = LoadLibrary( lib_path.c_str( ) );
   }

   if (!ent.lib)
   {
      ostringstream ss;
      ss << "Failed to load native module '" << lib_name << "' (ErrCode: "
         << GetLastError( ) << ")";
      f2_errspeak( ss.str( ).c_str( ) );
      return false;
   }

   /* Find the entry points. */
   ent.on_module = (MODULE_ENT::PFN_ON_MODULE) GetProcAddress( ent.lib, "on_module" );
   ent.on_inet = (MODULE_ENT::PFN_ON_INET) GetProcAddress( ent.lib, "on_inet" );
   ent.on_onet = (MODULE_ENT::PFN_ON_ONET) GetProcAddress( ent.lib, "on_onet" );
   ent.on_client = (MODULE_ENT::PFN_ON_CLIENT) GetProcAddress( ent.lib, "on_client" );
   ent.on_wndproc = (MODULE_ENT::PFN_ON_WNDPROC) GetProcAddress( ent.lib, "on_wndproc" );
   ent.on_world = (MODULE_ENT::PFN_ON_WORLD) GetProcAddress( ent.lib, "on_world" );
   ent.on_command = (MODULE_ENT::PFN_ON_COMMAND) GetProcAddress( ent.lib, "on_command" );
   
   /* Entry complete. */
   g_modules.push_back( ent );

   return true;
}

F2EXPORT bool F2CALL destroy( unsigned int me )
{  
   MODULE_ENT* ent = find_module( me );

   if (ent)
   {
      FreeLibrary( ent->lib );
      
      vector<MODULE_ENT>::iterator _where = g_modules.begin( );
      while (ent != &(*(_where++))) ;
      g_modules.erase( _where );

      return true;
   }

   return false;
}

F2EXPORT void F2CALL on_inet( unsigned int me, Revision rev, const char* line,
                              int from, size_t num )
{
   /* Pass it on to the right module. */
   MODULE_ENT* ent = find_module( me );

   if (ent && ent->on_inet)
      ent->on_inet( rev, line, from, num );
}

F2EXPORT void F2CALL on_onet( unsigned int me, Revision rev, const char* line,
                              int from, size_t num )
{
   /* Pass it on to the right module. */
   MODULE_ENT* ent = find_module( me );

   if (ent && ent->on_onet)
      ent->on_onet( rev, line, from, num );
}

F2EXPORT intptr_t F2CALL on_module( unsigned int me, Module_Event e, intptr_t param )
{
   /* Pass it on to the right module. */
   MODULE_ENT* ent = find_module( me );

   if (ent && ent->on_module)
      return ent->on_module( e, param );

   return 0;
}

F2EXPORT intptr_t F2CALL on_client( unsigned int me, Client_Event e, intptr_t param )
{
   /* Pass it on to the right module. */
   MODULE_ENT* ent = find_module( me );

   if (ent && ent->on_client)
      return ent->on_client( e, param );

   return 0;
}

F2EXPORT bool F2CALL on_wndproc( unsigned int me, WNDPROCPARAMS* params )
{
   /* Pass it on to the right module. */
   MODULE_ENT* ent = find_module( me );

   if (ent && ent->on_wndproc)
      return ent->on_wndproc( params );

   return false;
}   

F2EXPORT intptr_t F2CALL on_world( unsigned int me, World_Event e, intptr_t param )
{
   /* Pass it on to the right module. */
   MODULE_ENT* ent = find_module( me );

   if (ent && ent->on_world)
      return ent->on_world( e, param );

   return 0;
}

F2EXPORT bool F2CALL on_command( unsigned int me, int argc, char* argv[] )
{
   /* Pass it on to the right module. */
   MODULE_ENT* ent = find_module( me );

   if (ent && ent->on_command)
      return ent->on_command( argc, argv );

   return false;
}
