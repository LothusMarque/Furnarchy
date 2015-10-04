#include "Furnarchy.h"

using namespace std;

namespace Furnarchy
{
ScriptsSubsystem::ScriptsSubsystem( ModuleHost^ host )
   : m_host( host )
{
   m_scripts = gcnew LinkedList<ScriptInstance>;
   onScript( );
}

ScriptsSubsystem::~ScriptsSubsystem( )
{

}

array<ScriptInstance>^ ScriptsSubsystem::getRunningScripts( )
{
   array<ScriptInstance>^ r = gcnew array<ScriptInstance>( m_scripts->Count );
   m_scripts->CopyTo( r, 0 );
   return r;
}

Script_Error ScriptsSubsystem::kill( UInt32 sid )
{
   furn2::Script_Error err = furn2::f2_script_kill( sid );
   switch (err)
   {
   case furn2::SCRIPT_BADARG:
      return Script_Error::BadArgument; break;
   case furn2::SCRIPT_BADSID:
      return Script_Error::BadSid; break;
   case furn2::SCRIPT_NOTUNIQUE:
      return Script_Error::NotUnique; break;
   case furn2::SCRIPT_OK:
      return Script_Error::Success; break;
   }
   return Script_Error::Failed;
}

Script_Error ScriptsSubsystem::run( ScriptInstance% script )
{
   script.id = 0;

   if (script.arguments == nullptr || script.arguments->Length == 0)
      return Script_Error::BadArgument;

   char** argv = new char*[ script.arguments->Length ];
   for (int i = 0; i < script.arguments->Length; ++i)
   {
      string str = CliUtil::cliStrToAscii( script.arguments[ i ] );
      argv[ i ] = new char[ str.length( ) + 1 ];
      strncpy_s( argv[ i ], str.length( ) + 1, str.c_str( ), str.length( ) + 1 );
   }

   int flags = 0;
   if ((script.flags & Script_Flags::Late) == Script_Flags::Late)
      flags |= furn2::SCRIPT_RUN_LATE;
   if ((script.flags & Script_Flags::Replace) == Script_Flags::Replace)
      flags |= furn2::SCRIPT_RUN_REPLACE;
   if ((script.flags & Script_Flags::Unique) == Script_Flags::Unique)
      flags |= furn2::SCRIPT_RUN_UNIQUE;

   unsigned int sid = 0;
   furn2::Script_Error err = 
      furn2::f2_script_run( (const char**) argv, script.arguments->Length, flags, &sid );
   
   for (int i = 0; i < script.arguments->Length; ++i)
      delete argv[ i ];
   delete [] argv;

   switch (err)
   {
   case furn2::SCRIPT_BADARG:
      return Script_Error::BadArgument; break;
   case furn2::SCRIPT_BADSID:
      return Script_Error::BadSid; break;
   case furn2::SCRIPT_NOTUNIQUE:
      return Script_Error::NotUnique; break;
   case furn2::SCRIPT_OK:
      script.id = sid;
      return Script_Error::Success; break;
   }
   return Script_Error::Failed;
}

void ScriptsSubsystem::onScript( )
{
   unsigned num_sids = furn2::f2_script_enum( NULL, 0 );
   unsigned* sids = new unsigned[ num_sids ];
   num_sids = furn2::f2_script_enum( sids, num_sids );

   array<ScriptInstance>^ scripts = gcnew array<ScriptInstance>( num_sids );
   const char* argv[ 16 ];
   int flags;
   for (unsigned i = 0; i < num_sids; ++i)
   {
      unsigned num_args = furn2::f2_script_argv( sids[ i ], argv, 16, &flags );
      scripts[ i ].id = sids[ i ];
      scripts[ i ].arguments = gcnew array<String^>( num_args );
      for (unsigned j = 0; j < num_args; ++j)
         scripts[ i ].arguments[ j ] = gcnew String( argv[ j ] );
      if (flags & furn2::SCRIPT_RUN_LATE)
         scripts[ i ].flags = scripts[ i ].flags | Script_Flags::Late;
      if (flags & furn2::SCRIPT_RUN_REPLACE)
         scripts[ i ].flags = scripts[ i ].flags | Script_Flags::Replace;
      if (flags & furn2::SCRIPT_RUN_UNIQUE)
         scripts[ i ].flags = scripts[ i ].flags | Script_Flags::Unique;
   }
   delete [] sids;

   /* Look for new scripts. */
   for (unsigned i = 0; i < num_sids; ++i)
   {
      bool found = false;
      for (LinkedListNode<ScriptInstance>^ j = m_scripts->First; j != nullptr; j = j->Next)
      {
         if (j->Value.id == scripts[ i ].id)
         {
            found = true;
            break;
         }
      }
      if (!found)
      {
         m_scripts->AddLast( scripts[ i ] );
         ScriptRanEventArgs e;
         e.script = scripts[ i ];
         ScriptRanEvent( this, %e );
      }
   }
   /* Look for killed scripts. */
   if (m_scripts->Count > (int) num_sids)
   {
      for (LinkedListNode<ScriptInstance>^ i = m_scripts->First; i != nullptr; )
      {
         bool found = false;
         for (unsigned j = 0; j < num_sids; ++j)
         {
            if (scripts[ j ].id == i->Value.id)
            {
               found = true;
               break;
            }
         }
         LinkedListNode<ScriptInstance>^ prev = i;
         i = i->Next;
         if (!found)
         {
            ScriptKilledEventArgs e;
            e.script = prev->Value;
            m_scripts->Remove( prev );
            ScriptKilledEvent( this, %e );
         }
      }
   }
}


} // namespace Furnarchy