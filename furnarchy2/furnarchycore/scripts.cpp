#include "scripts.h"
#include "core.h"
#include "net.h"
#include "ScriptInst.h"
#include <shellapi.h>

namespace scripts {

//////////////////////////////////////////////////////////////////////////

enum { OPT_EDIT_FORCE = 0x1, PRUNE_DELAY = 3000, };

struct SCRIPT
{
   ScriptInst* inst;
   long        flags;
   SCRIPT( ScriptInst* inst, long flags ) : inst( inst ), flags( flags ) { }
};

typedef list<SCRIPT> ScriptList;

//////////////////////////////////////////////////////////////////////////

bool         g_initialized     = false;
unsigned int g_last_sid        = 1;
ScriptList   g_scripts;
bool         g_raise_event     = false;
long         g_hide_flags      = 0;

void cmd_scrun( const ArgList& args );
void cmd_sckill( const ArgList& args );
void cmd_scnuke( const ArgList& args );
void cmd_scls( const ArgList& args );
void cmd_scedit( const ArgList& args );
void kill_all( );
void prune_scripts( );
ScriptList::iterator find_script( unsigned int sid );
ScriptList::iterator find_script( const string& name );

bool initialize( )
{
   if (!g_initialized)
   {
      g_initialized = true;
   }

   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {
      g_initialized = false;
      kill_all( );
      prune_scripts( );
   }

   return true;
}

long on_inbound_line( string* line, int from, size_t num )
{
   g_hide_flags = 0;
   for (ScriptList::iterator i = g_scripts.begin( ); !(g_hide_flags & HIDE_SCRIPTS) && i != g_scripts.end( ); ++i)
   {
      if (!(i->flags & SCRIPT_RUN_LATE))
         i->inst->onInbound( line, from, num );
   }
   return g_hide_flags;
}

long on_inbound_line_late( string* line, int from, size_t num )
{
   g_hide_flags = 0;
   for (ScriptList::iterator i = g_scripts.begin( ); !(g_hide_flags & HIDE_SCRIPTS) && i != g_scripts.end( ); ++i)
   {
      if (i->flags & SCRIPT_RUN_LATE)
         i->inst->onInbound( line, from, num );
   }
   return g_hide_flags;
}

long on_outbound_line( string* line, int from, size_t num )
{
   g_hide_flags = 0;
   for (ScriptList::iterator i = g_scripts.begin( ); !(g_hide_flags & HIDE_SCRIPTS) && i != g_scripts.end( ); ++i)
   {
      if (!(i->flags & SCRIPT_RUN_LATE))
         i->inst->onOutbound( line, from, num );
   }
   return g_hide_flags;
}

long on_outbound_line_late( string* line, int from, size_t num )
{
   g_hide_flags = 0;
   for (ScriptList::iterator i = g_scripts.begin( ); !(g_hide_flags & HIDE_SCRIPTS) && i != g_scripts.end( ); ++i)
   {
      if (i->flags & SCRIPT_RUN_LATE)
         i->inst->onOutbound( line, from, num );
   }
   return g_hide_flags;
}

void on_tick( unsigned int dt )
{
   for (ScriptList::iterator i = g_scripts.begin( ); i != g_scripts.end( ); ++i)
   {
      i->inst->onTick( dt );
   }   

   // Prune.
   static unsigned int prune_cooldown = PRUNE_DELAY;
   if (prune_cooldown <= dt)
   {
      prune_scripts( );
      prune_cooldown = PRUNE_DELAY;
   }
   else
      prune_cooldown -= dt;

   if (g_raise_event)
   {
      g_raise_event = false;
      core::event_script( );
   }
}

inline void kill_all( )
{
   // size can fluctuate during this loop.
   for (ScriptList::iterator i = g_scripts.begin( ); i != g_scripts.end( ); ++i)
   {
      i->inst->kill( );
   }
}

void prune_scripts( )
{
   for (ScriptList::iterator i = g_scripts.begin( );
        i != g_scripts.end( ); )
   {
      if (i->inst->getRunLevel( ) != ScriptInst::RL_RUNNING && 
          i->inst->getRunLevel( ) != ScriptInst::RL_SUSPENDED)
      {
         delete i->inst;
         g_scripts.erase( i++ );
         g_raise_event = true;
      }
      else
         ++i;
   }
}

bool on_command( const ArgList& args )
{
   // Careful, this can recurse like a bitch.
   if (!args.size( ))
      return false;

   bool handled = false;

   string arg = args[ 0 ];
   to_lower( &arg );

   if (arg == "scrun")
   {
      cmd_scrun( args );
      handled = true;
   }
   else if (arg == "sckill")
   {
      cmd_sckill( args );
      handled = true;
   }
   else if (arg == "scls")
   {
      cmd_scls( args );
      handled = true;
   }
   else if (arg == "scnuke")
   {
      cmd_scnuke( args );
      handled = true;
   }
   else if (arg == "scedit")
   {
      cmd_scedit( args );
      handled = true;
   }

   /* Send to scripts. */
   for (ScriptList::iterator i = g_scripts.begin( ); i != g_scripts.end( ); ++i)
   {
      handled |= i->inst->onCommand( args );
   }

   return handled;
}

void cmd_scrun( const ArgList& args )
{
   size_t i_arg = 1;
   string arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";

   /* Collect options. */
   long flags = 0;
   while (!arg.compare( 0, 2, "--" ))
   {
      to_lower( &arg );

      if (!arg.compare( 2, arg.npos, "unique" ))
         flags |= SCRIPT_RUN_UNIQUE;
      else if (!arg.compare( 2, arg.npos, "replace" ))
         flags |= SCRIPT_RUN_REPLACE;
      else if (!arg.compare( 2, arg.npos, "late" ))
         flags |= SCRIPT_RUN_LATE;
      else
      {
         core::system_errspeak( "Error: Invalid option." );
         return;
      }

      arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";
   }
   
   if (!arg.length( ))
   {
      core::system_errspeak( "Error: Expected a script name." );
      return;
   }

   /* Collect script arguments. */
   ArgList script_args;
   do 
   {
      script_args.push_back( arg );
      arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";
   } while (arg.length( ));

   if (script_run( script_args, flags ) == SCRIPT_NOTUNIQUE)
      core::system_speak( "The script '" + script_args[ 0 ] + 
                          "' is already running." );
}

void cmd_sckill( const ArgList& args )
{
   if (args.size( ) < 2)
   {
      core::system_errspeak( "No scripts specified to kill." );
      return;
   }

   size_t i_arg = 1;
   string arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";

   bool killed = false;
   while (arg.length( ))
   {
      ScriptList::iterator i = find_script( arg );
      if (i != g_scripts.end( ) && script_kill( i->inst->getSid( ) ) == SCRIPT_OK)
         killed = true;

      arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";
   }

   if (!killed)
      core::system_errspeak( "No scripts killed." );
}

void cmd_scls( const ArgList& args )
{
   if (args.size( ) > 1)
   {
      core::system_errspeak( "Error: Too many arguments." );
      return;
   }

   core::system_speak( "Showing running scripts." );
   string arg;
   ostringstream ss;
   bool found_scripts = false;
   for (ScriptList::iterator i = g_scripts.begin( );
        i != g_scripts.end( ); ++i)
   {
      ScriptInst* script = i->inst;

      if (script->getRunLevel( ) == ScriptInst::RL_RUNNING)
      {
         found_scripts = true;
         ss.str( "" );
         const ArgList& script_args = script->getArgs( );

         if (i->flags & SCRIPT_RUN_LATE)
            ss << "<i>";

         for (size_t j = 0; j < script_args.size( ); j++)
            ss << break_html( escape_char( &(arg = script_args[ j ]), ' ' ) ) << ' ';

         if (i->flags & SCRIPT_RUN_LATE)
            ss << "</i>";

         core::system_speak( ss.str( ) );
      }
   }

   if (!found_scripts)
      core::system_speak( "No scripts are running." );
}

void cmd_scedit( const ArgList& args )
{
   size_t i_arg = 1;
   string arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";

   /* Collect options. */
   long flags = 0;
   while (!arg.compare( 0, 2, "--" ))
   {
      to_lower( &arg );

      if (!arg.compare( 2, arg.npos, "force" ))
         flags |= OPT_EDIT_FORCE;
      else
      {
         core::system_errspeak( "Error: Invalid option." );
         return;
      }

      arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";
   }

   if (!arg.length( ))
   {
      core::system_errspeak( "Error: Expected script name." );
      return;
   }

   // Use the per-user file if it exists.
   string dir  = core::get_furn_doc_root( ) + "\\scripts" ;
   string file = dir + "\\" + arg + ".gm";
   if (GetFileAttributes( file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
   {
      // Fall back to program file.
      dir  = core::get_furn_root( ) + "\\scripts" ;
      file = dir + "\\" + arg + ".gm";
   }

   if (GetFileAttributes( file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
   {
      if (flags & OPT_EDIT_FORCE)
      {
         // Create at the per-user location.
         dir  = core::get_furn_doc_root( ) + "\\scripts" ;
         file = dir + "\\" + arg + ".gm";

         HANDLE hf = CreateFile( file.c_str( ), GENERIC_WRITE, 0, 0, 
                                 CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0 );
         if (hf == INVALID_HANDLE_VALUE)
         {
            core::system_errspeak( "Unable to create script file " + file );
            return;
         }
         CloseHandle( hf );
      }
      else
      {
         core::system_errspeak( "The script file " + file + " doesn't exist." );
         return;
      }
   }

   ShellExecute( 0, "open", file.c_str( ), 0, dir.c_str( ), SW_SHOWNORMAL );
}

void cmd_scnuke( const ArgList& args )
{
   if (args.size( ) > 1)
   {
      core::system_errspeak( "Error: Too many arguments." );
      return;
   }

   kill_all( );
}

void on_connlvl( Connection_Level lvl )
{
   /* If not logged in, suspend scripts. */
   if (lvl != CONNLVL_LOGGEDIN)
   {
      for (ScriptList::iterator i = g_scripts.begin( );
           i != g_scripts.end( ); ++i)
      {
         i->inst->suspend( );
      }
   }
   /* If logged in, run scripts. */
   else
   {
      for (ScriptList::iterator i = g_scripts.begin( ); i != g_scripts.end( ); ++i)
      {
         i->inst->start( );
      }
   }
}

bool on_wndproc( WNDPROCPARAMS* params )
{
   for (ScriptList::iterator i = g_scripts.begin( ); i != g_scripts.end( ); ++i)
   {
      if (i->inst->onWndProc( params ))
         return true;
   }
   return false;
}

Script_Error script_enum( vector<unsigned int>* sids )
{
   sids->clear( );

   for (ScriptList::const_iterator i = g_scripts.begin( );
        i != g_scripts.end( ); ++i)
   {
      sids->push_back( i->inst->getSid( ) );
   }
   return SCRIPT_OK;
}

Script_Error script_argv( unsigned int sid, const ArgList** args, long* flags /* = NULL  */ )
{
   ScriptList::iterator i = find_script( sid );
   if (i == g_scripts.end( ))
      return SCRIPT_BADSID;

   *args = &i->inst->getArgs( );
   if (flags)
      *flags = i->flags;

   return SCRIPT_OK;
}

Script_Error script_run( const ArgList& argv, long flags, unsigned int* sid )
{
   if (!argv.size( ) || !argv[ 0 ].length( ))
      return SCRIPT_BADARG;

   /* Check for duplicates or replace. */
   if (flags & (SCRIPT_RUN_UNIQUE | SCRIPT_RUN_REPLACE))
   {
      for (ScriptList::iterator i = g_scripts.begin( );
           i != g_scripts.end( ); ++i)
      {
         // All arguments must be identical.
         if (argv.size( ) == i->inst->getArgs( ).size( ) &&
             !stricmp( argv[ 0 ], i->inst->getName( ) )) // Names are case-insensitive.
         {
            bool matched = true;
            for (ArgList::const_iterator j = i->inst->getArgs( ).begin( ), k = argv.begin( );
                 j != i->inst->getArgs( ).end( ); ++j, ++k)
            {
               if (*j != *k)
               {
                  matched = false;
                  break;
               }
            }

            if (matched)
            {
               if (flags & SCRIPT_RUN_UNIQUE)
                  return SCRIPT_NOTUNIQUE;
               else  // Replace.
                  i->inst->kill( );
            }
         }
      }
   }

   /* Create the script. */
   ScriptInst* inst = new ScriptInst( g_last_sid++, argv );
   g_scripts.push_back( SCRIPT( inst, flags ) );

   g_raise_event = true;

   if (sid)
      *sid = inst->getSid( );

   /* If logged in, start the script right away. */
   if (core::get_connlvl( ) >= CONNLVL_LOGGEDIN)
      inst->start( );
   // Otherwise, leave it suspended until we log in.
   if (inst->getRunLevel( ) == ScriptInst::RL_KILLED && inst->getRetVal( ) == EXIT_FAILURE)
      return SCRIPT_FAILED;
   return SCRIPT_OK;
}

Script_Error script_kill( unsigned int sid )
{
   ScriptList::iterator i = find_script( sid );
   if (i == g_scripts.end( ))
      return SCRIPT_BADSID;

   i->inst->kill( );
   return SCRIPT_OK;
}

ScriptList::iterator find_script( const string& name )
{
   for (ScriptList::iterator i = g_scripts.begin( );
        i != g_scripts.end( ); ++i)
   {
      if (!stricmp( i->inst->getName( ), name ))
         return i;
   }
   return g_scripts.end( );
}

ScriptList::iterator find_script( unsigned int sid )
{
   for (ScriptList::iterator i = g_scripts.begin( );
        i != g_scripts.end( ); ++i)
   {
      if (i->inst->getSid( ) == sid)
         return i;
   }
   return g_scripts.end( );
}

void hide_line( long flags )
{
   // Meaningless unless in the middle of an outbound/inbound event.
   g_hide_flags |= flags;
}

} // namespace scripts