#include "ScriptInst.h"
#include "scripts.h"
#include "net.h"
#include "world.h"
#include "client.h"
#include "window.h"
#include "modules.h"
#include "ScriptTerm.h"
#include "common/RegexOp.h"
#include "gm/gmThread.h"
#include "core.h"
#include "gm/gmCall.h"
#include "net_scan.h"
#include "common/xml_charset.h"
#include "shortname.h"
#include <iostream>
#include <algorithm>

using std::min;
using std::max;

//////////////////////////////////////////////////////////////////////////

enum
{ 
   KEY_F1 = 0x0100, 
   KEY_F2,
   KEY_F3,
   KEY_F4,
   KEY_F5,
   KEY_F6,
   KEY_F7,
   KEY_F8,
   KEY_F9,
   KEY_F10,
   KEY_F11,
   KEY_F12,
   KEY_LEFT,
   KEY_RIGHT,
   KEY_UP,
   KEY_DOWN,
   KEY_ESC,
   KEY_SHIFT,
   KEY_PAUSE,
};

//////////////////////////////////////////////////////////////////////////

class ScriptInst::Library
{
   static int GM_CDECL lib_toint( gmThread* );
   static int GM_CDECL lib_tofloat( gmThread* );
   static int GM_CDECL lib_tostring( gmThread* );
   static int GM_CDECL lib_uinttostring( gmThread* );
   static int GM_CDECL lib_stringtouint( gmThread* );
   static int GM_CDECL lib_tochar( gmThread* );
   static int GM_CDECL lib_totypename( gmThread* );
   static int GM_CDECL lib_join( gmThread* );
   static int GM_CDECL lib_map( gmThread* );
   static int GM_CDECL lib_slice( gmThread* );
   static int GM_CDECL lib_split( gmThread* );
   static int GM_CDECL lib_gettime( gmThread* );
   static int GM_CDECL lib_tolocaltime( gmThread* );
   static int GM_CDECL lib_length( gmThread* );
   static int GM_CDECL lib_strcat( gmThread* );
   static int GM_CDECL lib_substr( gmThread* );
   static int GM_CDECL lib_strcmp( gmThread* );
   static int GM_CDECL lib_stricmp( gmThread* );
   static int GM_CDECL lib_strchr( gmThread* );
   static int GM_CDECL lib_strrchr( gmThread* );
   static int GM_CDECL lib_strrev( gmThread* );
   static int GM_CDECL lib_strstr( gmThread* );
   static int GM_CDECL lib_stresc( gmThread* );
   static int GM_CDECL lib_strlwr( gmThread* );
   static int GM_CDECL lib_strupr( gmThread* );
   static int GM_CDECL lib_strtok( gmThread* );
   static int GM_CDECL lib_strtokargs( gmThread* );
   static int GM_CDECL lib_rematch( gmThread* );
   static int GM_CDECL lib_rematch2( gmThread* );
   static int GM_CDECL lib_resubst( gmThread* );
   static int GM_CDECL lib_resubst2( gmThread* );
   static int GM_CDECL lib_netscan( gmThread* );
   static int GM_CDECL lib_parsechat( gmThread* );
   static int GM_CDECL lib_recap( gmThread* );
   static int GM_CDECL lib_base95toint( gmThread* );
   static int GM_CDECL lib_inttobase95( gmThread* );
   static int GM_CDECL lib_base220toint( gmThread* );
   static int GM_CDECL lib_inttobase220( gmThread* );
   static int GM_CDECL lib_getcolorlen( gmThread* );
   static int GM_CDECL lib_dbopen( gmThread* );
   static int GM_CDECL lib_dbclose( gmThread* );
   static int GM_CDECL lib_dbread( gmThread* );
   static int GM_CDECL lib_dbreadsectionname ( gmThread* );
   static int GM_CDECL lib_dbreadat( gmThread* );
   static int GM_CDECL lib_dbwrite( gmThread* );
   static int GM_CDECL lib_dbpurge( gmThread* );
   static int GM_CDECL lib_dbcount( gmThread* );
   static int GM_CDECL lib_dbcountsections( gmThread* );
   static int GM_CDECL lib_dberase( gmThread* );
   static int GM_CDECL lib_dberasesection( gmThread* );
   static int GM_CDECL lib_log( gmThread* );
   static int GM_CDECL lib_rand( gmThread* );
   static int GM_CDECL lib_srand( gmThread* );
   static int GM_CDECL lib_uptime( gmThread* );
   static int GM_CDECL lib_getname( gmThread* );
   static int GM_CDECL lib_getdesc( gmThread* );
   static int GM_CDECL lib_getcolors( gmThread* );
   static int GM_CDECL lib_getpos( gmThread* );
   static int GM_CDECL lib_getmonsters( gmThread* );
   static int GM_CDECL lib_getmonsterinfo( gmThread* );
   static int GM_CDECL lib_desctag( gmThread* );
   static int GM_CDECL lib_badgecancel( gmThread* );
   static int GM_CDECL lib_flashwindow( gmThread* );
   static int GM_CDECL lib_getwindowtitle( gmThread* );
   static int GM_CDECL lib_setwindowtitle( gmThread* );
   static int GM_CDECL lib_ctrlstate( gmThread* );
   static int GM_CDECL lib_altstate( gmThread* );
   static int GM_CDECL lib_shiftstate( gmThread* );
   static int GM_CDECL lib_inetcount( gmThread* );
   static int GM_CDECL lib_onetcount( gmThread* );
   static int GM_CDECL lib_speak( gmThread* );
   static int GM_CDECL lib_errspeak( gmThread* );
   static int GM_CDECL lib_onet( gmThread* );
   static int GM_CDECL lib_inet( gmThread* );
   static int GM_CDECL lib_onetline( gmThread* );
   static int GM_CDECL lib_inetline( gmThread* );
   static int GM_CDECL lib_hideline( gmThread* );
   static int GM_CDECL lib_cmd( gmThread* );
   static int GM_CDECL lib_exit( gmThread* );
   static int GM_CDECL lib_getargs( gmThread* );
   static int GM_CDECL lib_print( gmThread* );
   static int GM_CDECL lib_import( gmThread* );
   static int GM_CDECL lib_modmsg( gmThread* );
   static int GM_CDECL lib_modmsgsz( gmThread* );
   static int GM_CDECL lib_modfind( gmThread* );
   static int GM_CDECL lib_setclipboard( gmThread* );
   static int GM_CDECL lib_getclipboard( gmThread* );
   static int GM_CDECL lib_collectgarbage( gmThread* );
   static int GM_CDECL lib_rgb( gmThread* );
   static int GM_CDECL lib_rgbunpack( gmThread* );
   static int GM_CDECL lib_termcreate( gmThread* );
   static int GM_CDECL lib_termset( gmThread* );
   static int GM_CDECL lib_termget( gmThread* );
   static int GM_CDECL lib_termdestroy( gmThread* );
   static int GM_CDECL lib_termwrite( gmThread* );
   static int GM_CDECL lib_termprint( gmThread* );
   static int GM_CDECL lib_termflush( gmThread* );
   static int GM_CDECL lib_asciitolatin( gmThread* );
   static int GM_CDECL lib_ignore( gmThread* );
   static int GM_CDECL lib_unignore( gmThread* );
   static int GM_CDECL lib_toshortname( gmThread* );
   static void GM_CDECL str_get_ind( gmThread*, gmVariable* );
   static int GM_CDECL lib_sqrt( gmThread* );
   static int GM_CDECL lib_cos( gmThread* );
   static int GM_CDECL lib_sin( gmThread* );
   static int GM_CDECL lib_tan( gmThread* );
   static int GM_CDECL lib_acos( gmThread* );
   static int GM_CDECL lib_asin( gmThread* );
   static int GM_CDECL lib_atan( gmThread* );
   static int GM_CDECL lib_abs( gmThread* );
   static int GM_CDECL lib_get_script_id( gmThread* );
public:
   static bool bind( gmMachine* machine );
   static string var2str( const gmVariable& var );
   static void term_set( TERMINST* term, gmTableObject* atts, gmMachine* machine );
};

//////////////////////////////////////////////////////////////////////////

template <class T_IterA, class T_IterB>
inline T_IterA find_term_inst( const ScriptTerm* term,
                               T_IterA beg, 
                               const T_IterB& end )
{
   for (; beg != end && beg->term != term; ++beg) ;
   return beg;
}

template <class T_IterA, class T_IterB>
inline T_IterA find_term_inst_by_id( const int id,
                                     T_IterA beg, 
                                     const T_IterB& end )
{
   for (; beg != end && beg->id != id; ++beg) ;
   return beg;
}

//////////////////////////////////////////////////////////////////////////

ScriptInst::ScriptInst( unsigned int sid, const ArgList& args )
 : m_sid( sid ), m_runlevel( RL_SUSPENDED ), m_started( false ), m_retval( 0 ), m_args( args ),
   m_gc_countdown( GC_RATE )
{   
   assert( m_args.size( ) );

   ScriptTerm::init( core::get_core_inst( ) );

   // Use the per-user file if it exists.
   m_script_file = core::get_furn_doc_root( ) + "\\scripts\\" + m_args[ 0 ] + ".gm";
   if (GetFileAttributes( m_script_file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
   {
      m_script_file = core::get_furn_doc_root( ) + "\\scripts\\" + m_args[ 0 ] + ".gm.txt";
      if (GetFileAttributes( m_script_file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
      {
         // Fall back to program file.
         m_script_file = core::get_furn_root( ) + "\\scripts\\" + m_args[ 0 ] + ".gm";
         if (GetFileAttributes( m_script_file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
         {
            m_script_file = core::get_furn_root( ) + "\\scripts\\" + m_args[ 0 ] + ".gm.txt";
         }
      }
   }

   // Always log to the per-user location.
   m_log_file = core::get_furn_doc_root( ) + "\\scripts\\logs\\" + m_args[ 0 ] + ".gm";

   m_rand_state = GetTickCount( );

   /* Register for a desctag. */
   m_desctag_id = world::desctag_reg( );
   
   /* Initialize the db. */
   m_db = new IniFile( core::get_furn_doc_root( ) + "\\scripts\\dbs\\" + m_args[ 0 ] + ".ini" );

   /* Clear handlers. */
   for (size_t i = 0; i < FN_MAX; ++i) m_handlers[ i ] = 0;
   
   /* Set up the machine. */
   m_machine.m_userParam = this;
   m_machine.SetDebugMode( true );
   Library::bind( &m_machine );
}

ScriptInst::~ScriptInst( )
{      
   kill( );

   // Free terms.
   while (!m_terms.empty( ))
   {
      // kill() should have destroyed them all.
      assert( m_terms.front( ).term->isDestroyed( ) );
      delete m_terms.front( ).term;
      m_terms.pop_front( );
   }

   /* Clear out reop cache. */
   RegexOp* reop;
   while (m_reops.cache.size( ))
   {
      m_reops.cache.oldest( &reop );
      m_reops.cache.pop( );
      delete reop;
   }

   delete m_db;

   world::desctag_unreg( m_desctag_id );  // Could result in a `desc

   ScriptTerm::uninit( );
}

size_t ScriptInst::findHandlers( )
{
   /* We store the function object, not the variable holding the function object,
   so after a global function is called, all subsequent calls are bound to
   the function object, even if the variable is reassigned in the script. */

   gmVariable global_var;

   global_var = 
      m_machine.GetGlobals( )->Get( gmVariable( m_machine.AllocStringObject( "onRun" ) ) );

   if (GM_FUNCTION == global_var.m_type)
      m_handlers[ FN_ONRUN ] = (gmFunctionObject*) global_var.m_value.m_ref;
   else
      m_handlers[ FN_ONRUN ] = 0;
   
   global_var = 
      m_machine.GetGlobals( )->Get( gmVariable( m_machine.AllocStringObject( "onKill" ) ) );

   if (GM_FUNCTION == global_var.m_type)
      m_handlers[ FN_ONKILL ] = (gmFunctionObject*) global_var.m_value.m_ref;
   else
      m_handlers[ FN_ONKILL ] = 0;

   global_var = 
      m_machine.GetGlobals( )->Get( gmVariable( m_machine.AllocStringObject( "onTick" ) ) );

   if (GM_FUNCTION == global_var.m_type)
      m_handlers[ FN_ONTICK ] = (gmFunctionObject*) global_var.m_value.m_ref;
   else
      m_handlers[ FN_ONTICK ] = 0;

   global_var = 
      m_machine.GetGlobals( )->Get( gmVariable( m_machine.AllocStringObject( "onCommand" ) ) );

   if (GM_FUNCTION == global_var.m_type)
      m_handlers[ FN_ONCOMMAND ] = (gmFunctionObject*) global_var.m_value.m_ref;
   else
      m_handlers[ FN_ONCOMMAND ] = 0;

   global_var = 
      m_machine.GetGlobals( )->Get( gmVariable( m_machine.AllocStringObject( "onInbound" ) ) );

   if (GM_FUNCTION == global_var.m_type)
      m_handlers[ FN_ONINBOUND ] = (gmFunctionObject*) global_var.m_value.m_ref;
   else
      m_handlers[ FN_ONINBOUND ] = 0;
   
   global_var = 
      m_machine.GetGlobals( )->Get( gmVariable( m_machine.AllocStringObject( "onOutbound" ) ) );

   if (GM_FUNCTION == global_var.m_type)
      m_handlers[ FN_ONOUTBOUND ] = (gmFunctionObject*) global_var.m_value.m_ref;
   else
      m_handlers[ FN_ONOUTBOUND ] = 0;

   global_var = 
      m_machine.GetGlobals( )->Get( gmVariable( m_machine.AllocStringObject( "onChar" ) ) );

   if (GM_FUNCTION == global_var.m_type)
      m_handlers[ FN_ONCHAR ] = (gmFunctionObject*) global_var.m_value.m_ref;
   else
      m_handlers[ FN_ONCHAR ] = 0;

   /* Make all functions persistent and return the number of handlers found. */
   size_t count = 0;
   for (size_t i = 0; i < FN_MAX; ++i)
   {
      if (m_handlers[ i ])
      {
         m_handlers[ i ]->Mark( &m_machine, GM_MARK_PERSIST );
         // Not needed in mark & sweep.
         // m_machine.m_gc->MakeObjectPersistant( m_handlers[ i ] );
         ++count;
      }
   }

   return count;
}

void ScriptInst::collectGarbage( )
{
   /* Clean up destroyed terms. */
   while (true) // Keep looping until there are no more destroyed terms.
   {
      list<TERMINST>::iterator i = m_terms.begin( );
      for (; i != m_terms.end( ); ++i)
      {
         if (i->term->isDestroyed( ))
            break;
      }
      if (i != m_terms.end( ))
      {
         delete i->term;
         m_terms.erase( i );
      }
      else
         break;
   }

   // Collect garbage from the script engine.
   m_machine.CollectGarbage( );
}

void ScriptInst::speakErrors( )
{
   gmLog& log = m_machine.GetLog( );
   const char* sz_err;
   string err;
   bool b_first = true;

   while ((sz_err = log.GetEntry( b_first )))
   {
      /* Watch out, gm likes to spit out newlines. */
      while (*sz_err && (*sz_err == '\r' || *sz_err == '\n')) ++sz_err;

      while (*sz_err)
      {
         const char* c = sz_err;
         while (*c && (*c != '\r' && *c != '\n')) ++c;

         if (c != sz_err && *c)
         {
            err.assign( sz_err, (size_t) c - (size_t) sz_err );
            core::system_errspeak( err );
         }

         while (*c && (*c == '\r' || *c == '\n')) ++c;
         sz_err = c;
      }
   }
}

RegexOp* ScriptInst::createRegexOp( const string& definition, string* error /* = NULL  */ )
{
   /* Check if it's been cached. */
   RegexOp* reop = NULL;
   if (!m_reops.cache.findAndTouch( definition, &reop ))
   {
      /* Not cached, so compile it. */
      const string* p_err = NULL;
      reop = RegexOp::from_str( definition, &p_err );

      if (!reop)
      {
         assert( p_err );
         ostringstream ss;
         ss << "Couldn't parse regex operation definition "
            << '"' << definition << "\": " << p_err->c_str( );
         *error = ss.str( );
         return NULL;
      }

      if (!reop->ok( ))
      {
         ostringstream ss;
         ss << "Couldn't compile regex in definition "
            << '"' << definition << "\": " << reop->getLastError( );
         delete reop;
         return NULL;
      }

      /* All good, so add it to the cache. */
      RegexOp* old;
      if (m_reops.cache.push( definition, reop, &old ))
      {
         // Something old got bumped out of cache, so delete it.
         delete old;
      }
   }
   return reop;
}

void ScriptInst::start( )
{
   if (m_runlevel != RL_SUSPENDED)
      return;

   m_runlevel = RL_RUNNING;

   if (m_started) // Already been started, just re-suspended.
      return;

   m_started = true;

   /* Load and run the global part of the script. */
   ifstream fs( m_script_file.c_str( ) );

   if (fs.fail( ))
   {
      m_retval = EXIT_FAILURE;
      m_runlevel = RL_KILLED;
      core::system_errspeak( "Failed to open script '" + m_args[ 0 ] + "'." );
      return;
   }

   string str_script = string( std::istreambuf_iterator<char>( fs ) , 
                               std::istreambuf_iterator<char>( ) );
   fs.close( );
   
   core::system_speak( "Running script '" + m_args[ 0 ] + "'." );

   int thread_id = GM_INVALID_THREAD;
   if (m_machine.ExecuteString( str_script.c_str( ), &thread_id ))
   {
      m_runlevel = RL_KILLED;
      m_retval = EXIT_FAILURE;
      core::system_errspeak( "The Script had compilation errors." );
      speakErrors( );
   }
   else if (thread_id != GM_INVALID_THREAD && m_machine.GetThread( thread_id ))
   {
      m_runlevel = RL_KILLED;
      m_retval = EXIT_FAILURE;
      core::system_errspeak( "The script '" + m_args[ 0 ] + 
                             "' encountered an exception." );
      speakErrors( );
   }
   else // All good.
   {
      if (m_runlevel == RL_KILLED) // Exited.
         return;

      // If no handlers are found and no terms are created, might as well terminate.
      if (!findHandlers( ) && m_terms.empty( )) 
      {
         kill( );
         return;
      }

      // Call onRun.
      if (m_handlers[ FN_ONRUN ])
      {
         gmCall call;
         call.BeginFunction( &m_machine, m_handlers[ FN_ONRUN ] );
         /* Put the args into table form. */
         gmTableObject* table = m_machine.AllocTableObject( );

         int index = 0;
         for (ArgList::iterator i = m_args.begin( );
              i != m_args.end( ); ++i)
         {
            gmStringObject* str_obj = 
               m_machine.AllocStringObject( (*i).c_str( ), (*i).length( ) );
            table->Set( &m_machine, index++, gmVariable( str_obj ) );
         }

         call.AddParamTable( table );

         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
         }
      } // if (m_handlers[ FN_ONRUN ])
   }
}

void ScriptInst::kill( )
{
   // Destroy all terms.
   for (list<TERMINST>::iterator i = m_terms.begin( ); i != m_terms.end( ); ++i)
      i->term->destroy( );

   if (m_runlevel == RL_RUNNING || m_runlevel == RL_SUSPENDED)
   {
      m_runlevel = RL_KILLED;

      if (m_handlers[ FN_ONKILL ])
      {
         gmCall call;
         call.BeginFunction( &m_machine, m_handlers[ FN_ONKILL ] );
         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
            return;
         }
      }

      core::system_speak( "The script '" + m_args[ 0 ] + "' terminated." );
   }
}

void ScriptInst::onTick( unsigned int dt )
{
   if (m_runlevel == RL_RUNNING)
   {
      if (m_handlers[ FN_ONTICK ])
      {
         gmCall call;
         call.BeginFunction( &m_machine, m_handlers[ FN_ONTICK ] );
         call.AddParamInt( dt );
         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
         }
      }
   }

   // Do garbage collection.
   if (m_gc_countdown <= dt)
   {
      m_gc_countdown = GC_RATE;
      collectGarbage( );
   }
   else
      m_gc_countdown -= dt;
}

void ScriptInst::onInbound( string* line, int from, size_t num )
{
   if (m_runlevel == RL_RUNNING)
   {
      if (m_handlers[ FN_ONINBOUND ])
      {
         gmCall call;
         call.BeginFunction( &m_machine, m_handlers[ FN_ONINBOUND ] );

         gmStringObject* line_obj = m_machine.AllocStringObject( line->c_str( ), 
                                                                 line->length( ) );
         call.AddParamString( line_obj );
         call.AddParamInt( from );
         call.AddParamInt( num );
         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
         }
         else  // Completed successfully.
         {
            /* Modified? */
            const char* rv;
            if (call.GetReturnedString( rv ))
               line->assign( rv );
         }
      }
   }
}

void ScriptInst::onOutbound( string* line, int from, size_t num )
{
   if (m_runlevel == RL_RUNNING)
   {
      if (m_handlers[ FN_ONOUTBOUND ])
      {
         gmCall call;
         call.BeginFunction( &m_machine, m_handlers[ FN_ONOUTBOUND ] );

         gmStringObject* line_obj = m_machine.AllocStringObject( line->c_str( ), 
                                                                 line->length( ) );
         call.AddParamString( line_obj );
         call.AddParamInt( from );
         call.AddParamInt( num );
         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
         }
         else  // Completed successfully.
         {
            /* Modified? */
            const char* rv;
            if (call.GetReturnedString( rv ))
               line->assign( rv );
         }
      }
   }
}

bool ScriptInst::onCommand( const ArgList& args )
{
   if (m_runlevel == RL_RUNNING)
   {
      if (m_handlers[ FN_ONCOMMAND ])
      {
         gmCall call;
         call.BeginFunction( &m_machine, m_handlers[ FN_ONCOMMAND ] );

         /* Need to put the args into table form. */
         gmTableObject* table_obj = m_machine.AllocTableObject( );
         
         for (size_t i = 0; i < args.size( ); i++)
         {
            table_obj->Set( &m_machine, i, 
                            gmVariable( m_machine.AllocStringObject( args[ i ].c_str( ), 
                                                                     args[ i ].length( ) ) ) );
         }
         
         call.AddParamTable( table_obj );

         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
         }
         else  // Completed successfully.
         {
            /* Check if it was handled. */
            int rv;
            if (call.GetReturnedInt( rv ) && rv)
               return true;
         }
      }
   }

   return false;
}

bool ScriptInst::onWndProc( WNDPROCPARAMS* params )
{
   // Pass on WM_CHARs and certain WM_KEYDOWNs
   int ch, repeat;

   switch (params->msg)
   {
   default:
      return false;

   case WM_CHAR:
      ch     = params->wparam;
      repeat = LOWORD( params->lparam );
      break;

   case WM_KEYDOWN:
   case WM_SYSKEYDOWN:
      repeat = LOWORD( params->lparam );
      switch (params->wparam)
      {
      default:
         return false;
      case VK_SHIFT:
         ch = KEY_SHIFT;   break;
      case VK_ESCAPE:
         ch = KEY_ESC;     break;
      case VK_LEFT:
         ch = KEY_LEFT;    break;
      case VK_RIGHT:
         ch = KEY_RIGHT;   break;
      case VK_UP:
         ch = KEY_UP;      break;
      case VK_DOWN:
         ch = KEY_DOWN;    break;
      case VK_F1:
         ch = KEY_F1;      break;
      case VK_F2:
         ch = KEY_F2;      break;
      case VK_F3:
         ch = KEY_F3;      break;
      case VK_F4:
         ch = KEY_F4;      break;
      case VK_F5:
         ch = KEY_F5;      break;
      case VK_F6:
         ch = KEY_F6;      break;
      case VK_F7:
         ch = KEY_F7;      break;
      case VK_F8:
         ch = KEY_F8;      break;
      case VK_F9:
         ch = KEY_F9;      break;
      case VK_F10:
         ch = KEY_F10;     break;
      case VK_F11:
         ch = KEY_F11;     break;
      case VK_F12:
         ch = KEY_F12;     break;
      case VK_PAUSE:
         ch = KEY_PAUSE;   break;
      }
      break;
   }

   if (m_runlevel == RL_RUNNING)
   {
      if (m_handlers[ FN_ONCHAR ])
      {
         gmCall call;
         call.BeginFunction( &m_machine, m_handlers[ FN_ONCHAR ] );

         call.AddParamInt( ch );
         call.AddParamInt( repeat );

         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
         }
         else  // Completed successfully.
         {
            /* Check if it was handled. */
            int rv;
            if (call.GetReturnedInt( rv ) && rv)
            {
               params->retval = 0;
               return true;
            }
         }
      }
   }

   return false;
}

void ScriptInst::onTermDestroy( ScriptTerm* term )
{   
   list<TERMINST>::iterator i = find_term_inst( term, m_terms.begin( ), m_terms.end( ) );

   assert( i != m_terms.end( ) );
   assert( i->term == term );

   if (m_runlevel == RL_RUNNING)
   {
      if (i->on_destroy)
      {
         gmCall call;
         call.BeginFunction( &m_machine, i->on_destroy );
         call.AddParamInt( i->id );

         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
         }
      }
   }
   // The destroyed term will get pruned during CollectGarbage().
}

void ScriptInst::onTermInput( ScriptTerm* term, string& input )
{
   list<TERMINST>::iterator i = find_term_inst( term, m_terms.begin( ), m_terms.end( ) );

   assert( i != m_terms.end( ) );
   assert( i->term == term );

   if (m_runlevel == RL_RUNNING)
   {
      if (i->on_input)
      {
         gmCall call;
         call.BeginFunction( &m_machine, i->on_input );
         call.AddParamInt( i->id );
         call.AddParamString( input.c_str( ) );

         int thread_id = GM_INVALID_THREAD;
         call.End( &thread_id );

         if (thread_id != GM_INVALID_THREAD)
         {
            // An exception occurred.
            m_runlevel = RL_KILLED;
            m_retval = EXIT_FAILURE;
            core::system_errspeak( "The script '" + m_args[ 0 ] + 
                                   "' encountered an exception." );
            speakErrors( );
         }
      }
   }
}

/************************************************************************/
/*   Library Functions                                                  */
/************************************************************************/

bool ScriptInst::Library::bind( gmMachine* machine )
{
   /* Bind library functions. */
   static gmFunctionEntry lib_entries[] = 
   {
      { "toInt", lib_toint },
      { "toFloat", lib_tofloat },
      { "toString", lib_tostring },
      { "uintToString", lib_uinttostring },
      { "stringToUint", lib_stringtouint },
      { "toChar", lib_tochar },
      { "toTypename", lib_totypename },
      { "join", lib_join },
      { "map", lib_map },
      { "slice", lib_slice },
      { "split", lib_split },
      { "getTime", lib_gettime },
      { "toLocalTime", lib_tolocaltime },
      { "length", lib_length },
      { "strCat", lib_strcat },
      { "strSub", lib_substr },
      { "strCmp", lib_strcmp },
      { "strCaseCmp", lib_stricmp },
      { "strFirst", lib_strchr },
      { "strLast", lib_strrchr },
      { "strRev", lib_strrev },
      { "strStr", lib_strstr },
      { "strEsc", lib_stresc },
      { "strLwr", lib_strlwr },
      { "strUpr", lib_strupr },
      { "strTok", lib_strtok },
      { "strTokArgs", lib_strtokargs },
      { "reMatch", lib_rematch },
      { "reMatch2", lib_rematch2 },
      { "reSubst", lib_resubst },
      { "reSubst2", lib_resubst2 },
      { "netScan", lib_netscan },
      { "parseChat", lib_parsechat },
      { "reCap", lib_recap },
      { "furcToInt", lib_base95toint }, // For legacy compatibility.
      { "intToFurc", lib_inttobase95 }, // For legacy compatibility.      
      { "base95ToInt", lib_base95toint },
      { "intToBase95", lib_inttobase95 },
      { "base220ToInt", lib_base220toint },
      { "intToBase220", lib_inttobase220 },
      { "getColorLen", lib_getcolorlen },
      { "dbOpen", lib_dbopen },
      { "dbClose", lib_dbclose },
      { "dbRead", lib_dbread },
	  { "dbReadSectionName", lib_dbreadsectionname},
      { "dbWrite", lib_dbwrite },
      { "dbPurge", lib_dbpurge },
      { "dbErase", lib_dberase },
      { "dbReadAt", lib_dbreadat },
      { "dbCount", lib_dbcount },
	  { "dbCountSections", lib_dbcountsections},
      { "dbEraseSection", lib_dberasesection },
      { "log", lib_log },
      { "rand", lib_rand },
      { "randSeed", lib_srand },
      { "uptime", lib_uptime },
      { "getName", lib_getname },
      { "getDesc", lib_getdesc },
      { "getColors", lib_getcolors },
      { "getPos", lib_getpos },
      { "getMonsters", lib_getmonsters },
      { "getMonsterInfo", lib_getmonsterinfo },
      { "desctag", lib_desctag },
      { "badgeCancel", lib_badgecancel },
      { "flashWindow", lib_flashwindow },
      { "getWindowTitle", lib_getwindowtitle },
      { "setWindowTitle", lib_setwindowtitle },
      { "ctrlState", lib_ctrlstate },
	  { "altState", lib_altstate },
	  { "shiftState", lib_shiftstate },
      { "countInbound", lib_inetcount },
      { "countOutbound", lib_onetcount },
      { "speak", lib_speak },
      { "errSpeak", lib_errspeak },
      { "netOut", lib_onet },
      { "netOutLine", lib_onetline },
      { "netIn", lib_inet },
      { "netInLine", lib_inetline },
      { "netHideLine", lib_hideline },
      { "command", lib_cmd },
      { "exit", lib_exit },
      { "getArgs", lib_getargs },
      { "print", lib_print },
      { "import", lib_import },
      { "modMsg", lib_modmsg },
      { "modFind", lib_modfind },
      { "modMsgSz", lib_modmsgsz },
      { "getClipboard", lib_getclipboard },
      { "setClipboard", lib_setclipboard },
      { "collectGarbage", lib_collectgarbage },
      { "rgb", lib_rgb },
      { "rgbUnpack", lib_rgbunpack },
      { "termCreate", lib_termcreate },
      { "termDestroy", lib_termdestroy },
      { "termSet", lib_termset },
      { "termGet", lib_termget },
      { "termWrite", lib_termwrite },
      { "termPrint", lib_termprint },
      { "termFlush", lib_termflush },
      { "asciiToLatin", lib_asciitolatin },
      { "ignore", lib_ignore },
      { "unignore", lib_unignore },
      { "toShortname", lib_toshortname },
	  { "sqrt", lib_sqrt },
	  { "cos", lib_cos },
	  { "sin", lib_sin },
	  { "tan", lib_tan },
	  { "acos", lib_acos },
	  { "asin", lib_asin },
	  { "atan", lib_atan },
	  { "abs", lib_abs },
	  { "getScriptID", lib_get_script_id }
   };

   static const int num_entries = 
      sizeof( lib_entries ) / sizeof( gmFunctionEntry );

   machine->RegisterLibrary( lib_entries, num_entries );

   /* Bind overloaded operators. */
   machine->RegisterTypeOperator( GM_STRING, O_GETIND, 0, str_get_ind );

   return true;
}

void GM_CDECL ScriptInst::
              Library::str_get_ind( gmThread* a_thread, gmVariable* a_operands )
{
   gmStringObject* obj = (gmStringObject*) a_operands[ 0 ].m_value.m_ref;
   int i = a_operands[ 1 ].m_value.m_int;

   if ((unsigned) i < (unsigned) obj->GetLength( ))
   {
      a_operands[ 0 ].SetInt( (unsigned char) obj->GetString( )[ i ] );
   }
   else
   {
      a_operands[ 0 ].Nullify( );
   }
}

string GM_CDECL ScriptInst::Library::var2str( const gmVariable& var )
{
   ostringstream ss;

   switch (var.m_type)
   {
   case GM_INT:
      ss << var.m_value.m_int;
      break;
   case GM_FLOAT:
      ss << var.m_value.m_float;
      break;
   case GM_STRING:
      ss << ((gmStringObject*) var.m_value.m_ref)->GetString( );
      break;
   case GM_NULL:
      ss << "null";
      break;
   case GM_TABLE:
      {
         gmTableObject* table = ((gmTableObject*) var.m_value.m_ref);
         gmTableIterator iter;
         ss << "{";
         bool is_first = true;
         for (gmTableNode* node = table->GetFirst( iter ); node;
              node = table->GetNext( iter ))
         {
            if (is_first)
            {
               ss << " ";
               is_first = false;
            }
            else
               ss << ", ";

            // Print the key for associative elements.
            if (node->m_key.m_type != GM_INT)
               ss << var2str( node->m_key ) << "=";

            // Print the value; surround in quotes if a string.
            if (node->m_value.m_type == GM_STRING)
               ss << "\"" << var2str( node->m_value ) << "\"";
            else
               ss << var2str( node->m_value );
         }
         ss << " }";
      }
      break;
   case GM_FUNCTION:
      {
         gmFunctionObject* fn = (gmFunctionObject*) var.m_value.m_ref;
         ss << fn->GetDebugName( );
      }
      break;
   default:
      ss << "USER@" << std::hex << (long) var.m_value.m_ref;
      break;
   }

   return ss.str( );
}

void ScriptInst::Library::term_set( TERMINST* term, gmTableObject* atts, gmMachine* machine )
{
   gmVariable entry;
   
   entry = atts->GetLinearSearch( "id" );
   if (entry.m_type == GM_INT)
      term->id = entry.m_value.m_int;

   entry = atts->GetLinearSearch( "title" );
   if (entry.m_type == GM_STRING)
      term->term->setTitle( ((gmStringObject*) entry.m_value.m_ref)->GetString( ) );

   entry = atts->GetLinearSearch( "onDestroy" );
   if (entry.m_type == GM_FUNCTION)
   {
      term->on_destroy = (gmFunctionObject*) entry.m_value.m_ref;
      // Make it persistent.
      term->on_destroy->Mark( machine, GM_MARK_PERSIST );
   }

   entry = atts->GetLinearSearch( "onInput" );
   if (entry.m_type == GM_FUNCTION)
   {
      term->on_input = (gmFunctionObject*) entry.m_value.m_ref;
      // Make it persistent.
      term->on_input->Mark( machine, GM_MARK_PERSIST );
   }

   entry = atts->GetLinearSearch( "font" );
   if (entry.m_type == GM_TABLE)
   {
      gmTableObject* font_table = (gmTableObject*) entry.m_value.m_ref;

      // Name and height are optional fields.
      unsigned font_height = 0;
      string font_name = term->term->getFont( &font_height );      

      entry = font_table->GetLinearSearch( "name" );
      if (entry.m_type == GM_STRING)
         font_name = ((gmStringObject*) entry.m_value.m_ref)->GetString( );

      entry = font_table->GetLinearSearch( "height" );
      if (entry.m_type == GM_INT)
         font_height = entry.m_value.m_int;

      term->term->setFont( font_name, font_height );
   }
   entry = atts->GetLinearSearch( "types" );
   if (entry.m_type == GM_TABLE)
   {
      gmTableObject* types_table = (gmTableObject*) entry.m_value.m_ref;

#define _GETTYPE( type, key ) { \
                                 gmTableObject* t = types_table->GetLinearSearch( key ).GetTableObjectSafe( ); \
                                 if (t) \
                                 { \
                                    ScriptTerm::StyleColor sc = term->term->getTextType( type );  \
                                    gmVariable v = t->GetLinearSearch( "style" ); \
                                    if (v.m_type == GM_INT) \
                                       sc.style = v.m_value.m_int; \
                                    v = t->GetLinearSearch( "color" ); \
                                    if (v.m_type == GM_INT) \
                                       sc.color = (COLORREF) v.m_value.m_int; \
                                    term->term->setTextType( type, sc ); \
                                 } \
                              }

      _GETTYPE( ScriptTerm::TEXT_OUTPUT, "output" );
      _GETTYPE( ScriptTerm::TEXT_LINK, "link" );
      _GETTYPE( ScriptTerm::TEXT_SUCCESS, "success" );
      _GETTYPE( ScriptTerm::TEXT_WARNING, "warning" );
      _GETTYPE( ScriptTerm::TEXT_ERROR, "error" );
      _GETTYPE( ScriptTerm::TEXT_CHANNEL, "channel" );
      _GETTYPE( ScriptTerm::TEXT_WHISPER, "whisper" );
      _GETTYPE( ScriptTerm::TEXT_EMOTE, "emote" );
      _GETTYPE( ScriptTerm::TEXT_EMIT, "emit" );
      _GETTYPE( ScriptTerm::TEXT_MYSPEECH, "myspeech" );
      _GETTYPE( ScriptTerm::TEXT_BCAST, "bcast" );
      _GETTYPE( ScriptTerm::TEXT_ROLL, "roll" );
      _GETTYPE( ScriptTerm::TEXT_SHOUT, "shout" );
      _GETTYPE( ScriptTerm::TEXT_TRADE, "trade" );
      _GETTYPE( ScriptTerm::TEXT_DRAGONSPEAK, "dragonspeak" );
      _GETTYPE( ScriptTerm::TEXT_NOTIFY, "notify" );
      _GETTYPE( ScriptTerm::TEXT_QUERY, "query" );
      _GETTYPE( ScriptTerm::TEXT_HIGHLIGHT, "highlight" );
      _GETTYPE( ScriptTerm::TEXT_LOG, "log" );
      _GETTYPE( ScriptTerm::TEXT_HISTORY, "history" );
      _GETTYPE( ScriptTerm::TEXT_FURCLINK, "furclink" );
      _GETTYPE( ScriptTerm::TEXT_CMDLINK, "cmdlink" );
      _GETTYPE( ScriptTerm::TEXT_NAME, "name" );
      _GETTYPE( ScriptTerm::TEXT_INPUT, "input" );
      _GETTYPE( ScriptTerm::TEXT_F2CMDLINK, "f2cmdlink" );

#undef _GETTYPE

   }
   entry = atts->GetLinearSearch( "bgColors" );
   if (entry.m_type == GM_TABLE)
   {
      gmTableObject* bgcolor_table = (gmTableObject*) entry.m_value.m_ref;

      // Preserve any missing colors.
      COLORREF color_input, color_output;
      term->term->getBgColors( &color_input, &color_output );

      entry = bgcolor_table->GetLinearSearch( "input" );
      if (entry.m_type == GM_INT)
         color_input = entry.m_value.m_int;

      entry = bgcolor_table->GetLinearSearch( "output" );
      if (entry.m_type == GM_INT)
         color_output = entry.m_value.m_int;

      term->term->setBgColors( color_input, color_output );
   }
}

//////////////////////////////////////////////////////////////////////////

#define LIBDEF( fn ) int GM_CDECL ScriptInst::Library::fn( gmThread* a_thread )

LIBDEF( lib_toint )
{
   GM_CHECK_NUM_PARAMS( 1 );
   const gmVariable& var = a_thread->Param( 0 );

   int ret = 0;
   switch (var.m_type)
   {
   case GM_INT:
      ret = var.m_value.m_int;
      break;
   case GM_FLOAT:
      ret = (int) var.m_value.m_float;
      break;
   case GM_STRING:
      ret = (int) strtol( ((gmStringObject*) var.m_value.m_ref)->GetString( ), NULL, 10 );
      break;
   default:
      break;
   }

   a_thread->PushInt( ret );
   return GM_OK;
}

LIBDEF( lib_tofloat )
{
   GM_CHECK_NUM_PARAMS( 1 );
   const gmVariable& var = a_thread->Param( 0 );

   float ret = 0.0f;
   switch (var.m_type)
   {
   case GM_INT:
      ret = (float) var.m_value.m_int;
      break;
   case GM_FLOAT:
      ret = var.m_value.m_float;
      break;
   case GM_STRING:
      ret = (float) atof( ((gmStringObject*) var.m_value.m_ref)->GetString( ) );
      break;
   default:
      break;
   }

   a_thread->PushFloat( ret );
   return GM_OK;
}

LIBDEF( lib_sqrt )
{
   GM_CHECK_NUM_PARAMS( 1 );
   a_thread->PushFloat(sqrt(a_thread->ParamFloatOrInt(0)));
   return GM_OK;
}

LIBDEF( lib_cos )
{
   GM_CHECK_NUM_PARAMS( 1 );
   a_thread->PushFloat(cos(a_thread->ParamFloatOrInt(0)));
   return GM_OK;
}

LIBDEF( lib_sin )
{
   GM_CHECK_NUM_PARAMS( 1 );
   a_thread->PushFloat(sin(a_thread->ParamFloatOrInt(0)));
   return GM_OK;
}

LIBDEF( lib_tan )
{
   GM_CHECK_NUM_PARAMS( 1 );
   a_thread->PushFloat(tan(a_thread->ParamFloatOrInt(0)));
   return GM_OK;
}

LIBDEF (lib_acos)
{
	GM_CHECK_NUM_PARAMS( 1 );
	a_thread->PushFloat(acos(a_thread->ParamFloatOrInt(0)));
	return GM_OK;
}

LIBDEF( lib_asin )
{
   GM_CHECK_NUM_PARAMS( 1 );
   a_thread->PushFloat(asin(a_thread->ParamFloatOrInt(0)));
   return GM_OK;
}

LIBDEF (lib_atan)
{
	GM_CHECK_NUM_PARAMS( 1 );
	a_thread->PushFloat(atan(a_thread->ParamFloatOrInt(0)));
	return GM_OK;
}

LIBDEF (lib_abs)
{
	GM_CHECK_NUM_PARAMS(1);
	a_thread->PushInt(abs(a_thread->ParamFloatOrInt(0)));
	return GM_OK;
}

LIBDEF ( lib_get_script_id )
{
	ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
	a_thread->PushInt(inst->m_sid);
	return GM_OK;
//	inst->m_sid;
}

LIBDEF( lib_tostring )
{
   GM_CHECK_NUM_PARAMS( 1 );
   a_thread->PushNewString( var2str( a_thread->Param( 0 ) ).c_str( ) );
   return GM_OK;
}

/* stringToUint( str, [radix] ) */
LIBDEF( lib_stringtouint )
{
   if (a_thread->GetNumParams( ) < 1) 
   {
      GM_EXCEPTION_MSG( "Expecting at least 1 parameter." );
      return GM_EXCEPTION;
   }

   GM_CHECK_STRING_PARAM( sz, 0 );

   unsigned radix = 10;

   if (a_thread->GetNumParams( ) > 1)
   {
      GM_CHECK_INT_PARAM( _radix, 1 );
      radix = min( (unsigned) _radix, (unsigned) 36 );
      if (radix < 2)
      {
         GM_EXCEPTION_MSG( "Radix must be 2 or greater." );
         return GM_EXCEPTION;
      }
   }

   /* Precomputed LUT radix digit values ('0' - 'Z'). */
   static const unsigned char radix_values[ 256 ] = 
   {  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      0 ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  
      25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  -1,  -1,  -1,  -1,  -1,  
      -1,  10 ,  11 ,  12 ,  13 ,  14 ,  15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,  22 ,  23 ,  24 ,  
      25 ,  26 ,  27 ,  28 ,  29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
      -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  
   };

   unsigned i = 0;
   while (radix_values[ (unsigned char) *sz ] != (unsigned char) -1)
      i = (i * radix) + radix_values[ (unsigned char) *(sz++) ];

   a_thread->PushInt( i );
   return GM_OK;
}

/* uintToString( uint, [radix], [width] ) */
LIBDEF( lib_uinttostring )
{
   if (a_thread->GetNumParams( ) < 1) 
   {
      GM_EXCEPTION_MSG( "Expecting at least 1 parameter." );
      return GM_EXCEPTION;
   }

   unsigned n;
   {
      GM_CHECK_INT_PARAM( _n, 0 );
      n = _n;
   }

   unsigned radix = 10;
   
   if (a_thread->GetNumParams( ) > 1)
   {
      GM_CHECK_INT_PARAM( _radix, 1 );
      radix = min( (unsigned) _radix, (unsigned) 36 );
      if (radix < 2)
      {
         GM_EXCEPTION_MSG( "Radix must be 2 or greater." );
         return GM_EXCEPTION;
      }
   }

   unsigned width = 0;

   if (a_thread->GetNumParams( ) > 2)
   {
      GM_CHECK_INT_PARAM( _width, 2 );
      width = min( (unsigned) _width, (unsigned) 32 );
   }

   static const char digits[36] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
                                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                                    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                                    'U', 'V', 'W', 'X', 'Y', 'Z' };
   // Encode backwards.
   string str;
   for ( ; n; n /= radix)
      str += digits[ n % radix ];

   if (width)
   {
      if (str.length( ) > width)
         // Clip to width.
         str.resize( width );
      else
         // Pad with zeroes.
         str.append( width - str.length( ), digits[ 0 ] );
   }

   // Reverse.
   reverse( str.begin( ), str.end( ) );

   a_thread->PushNewString( str.c_str( ) );
   return GM_OK;
}

LIBDEF( lib_tochar )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( ch, 0 );

   char sz[] = { ch, '\0' };
   a_thread->PushNewString( sz, 2 );
   return GM_OK;
}

LIBDEF( lib_totypename )
{
   GM_CHECK_NUM_PARAMS( 1 );

   const char* tn = a_thread->GetMachine( )->GetTypeName( a_thread->Param( 0 ).m_type );
   a_thread->PushNewString( tn );
   
   return GM_OK;
}

LIBDEF( lib_join )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_TABLE_PARAM( table, 0 );
   const char* separator = a_thread->ParamString( 1, " " );

   // Create a string with all the table elements separated by 'separator'.
   ostringstream ss;
   gmTableIterator iter;
   for (gmTableNode* node = table->GetFirst( iter ); node;
        node = table->GetNext( iter ))
   {
      if (iter != 1)
         ss << separator;
      ss << var2str( node->m_value );
   }

   a_thread->PushNewString( ss.str( ).c_str( ) );
   return GM_OK;
}

LIBDEF( lib_map )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_TABLE_PARAM( table, 0 );
   GM_CHECK_FUNCTION_PARAM( fn, 1 );

   gmMachine* machine = a_thread->GetMachine( );
   table = table->Duplicate( machine );
   gmTableIterator iter;
   gmTableNode* node = table->GetFirst( iter );

   /* Pass each table element to the map function and store the return
      value in its place. */
   while (node)
   {
      gmCall call;
      call.BeginFunction( machine, fn );
      call.AddParam( node->m_value );
      
      int thread_id = GM_INVALID_THREAD;
      call.End( &thread_id );

      if (thread_id != GM_INVALID_THREAD)
      {
         // An exception occurred.
         GM_EXCEPTION_MSG( "map() function encountered an exception." );
         return GM_EXCEPTION;
      }
      
      node->m_value = call.GetReturnedVariable( );

      node = table->GetNext( iter );
   }

   a_thread->PushTable( table );
   return GM_OK;
}

LIBDEF( lib_slice )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_TABLE_PARAM( table, 0 );

   gmMachine* machine = a_thread->GetMachine( );

   /* If no other parameters, just duplicate the table. */
   if (a_thread->GetNumParams( ) == 1)
   {
      a_thread->PushTable( table->Duplicate( machine ) );
      return GM_OK;
   }
   /* Otherwise, do some slicing. */
   gmTableObject* result = machine->AllocTableObject( );
   int result_len = 0;

   const int num_args = a_thread->GetNumParams( );
   for (int arg = 1; arg < num_args; ++arg)
   {
      int off, len;
      {
         GM_CHECK_TABLE_PARAM( table_slice, arg );
         gmVariable t = table_slice->Get( gmVariable( 0 ) );
         off = std::max( (t.m_type == GM_INT) ? t.m_value.m_int : 0, 0 );
         t = table_slice->Get( gmVariable( 1 ) );
         len = (t.m_type == GM_INT) ? t.m_value.m_int : -1;
      }

      // Append the slice to the resultant table.
      gmTableIterator iter;
      gmTableNode* node = table->GetFirst( iter );
      for (int i = 0; i < off && node; node = table->GetNext( iter ), ++i) ;
      for (int i = 0; i != len && node; node = table->GetNext( iter ), ++i)
         result->Set( machine, result_len++, node->m_value );
   }

   a_thread->PushTable( result );
   return GM_OK;
}

LIBDEF( lib_split )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz, 0 );
   const char* separator = a_thread->ParamString( 1, " " );
   gmMachine* machine = a_thread->GetMachine( );

   gmTableObject* table = machine->AllocTableObject( );
   const size_t sep_len = strlen( separator );

   string buf;
   const char* f = NULL;
   while (*sz)
   {
      if (f = strstr( sz, separator ))
      {
         size_t len = (size_t) f - (size_t) sz;
         if (len)
         {
            buf.assign( sz, len );
            table->Set( machine, table->Count( ), 
                        gmVariable( machine->AllocStringObject( buf.c_str( ), len ) ) );
         }
         sz += len + sep_len;
      }
      else
      {
         buf.assign( sz );
         table->Set( machine, table->Count( ), 
                     gmVariable( machine->AllocStringObject( buf.c_str( ), buf.length( ) ) ) );
         sz += buf.length( );
      }
   }

   a_thread->PushTable( table );
   return GM_OK;
}

LIBDEF( lib_gettime )
{
   a_thread->PushInt( (int) time( 0 ) );
   return GM_OK;
}

LIBDEF( lib_tolocaltime )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( t, 0 );
   time_t _t = t; // Problem because sizeof( t ) < sizeof( time_t )
   tm* lt = localtime( &_t );

   gmMachine* machine = a_thread->GetMachine( );
   gmTableObject* table = machine->AllocTableObject( );

   table->Set( machine, "hour", gmVariable( lt->tm_hour ) );
   table->Set( machine, "isdst", gmVariable( lt->tm_isdst ) );
   table->Set( machine, "mday", gmVariable( lt->tm_mday ) );
   table->Set( machine, "min", gmVariable( lt->tm_min ) );
   table->Set( machine, "mon", gmVariable( lt->tm_mon ) );
   table->Set( machine, "sec", gmVariable( lt->tm_sec ) );
   table->Set( machine, "wday", gmVariable( lt->tm_wday ) );
   table->Set( machine, "yday", gmVariable( lt->tm_yday ) );
   table->Set( machine, "year", gmVariable( lt->tm_year + 1900 ) );

   a_thread->PushTable( table );
   return GM_OK;
}

LIBDEF( lib_length )
{
   GM_CHECK_NUM_PARAMS( 1 );

   const gmVariable& var = a_thread->Param( 0 );

   int rv;

   switch (var.m_type)
   {
   case GM_STRING:
      rv = ((gmStringObject*) var.m_value.m_ref)->GetLength( );
      break;
   case GM_TABLE:
      rv = ((gmTableObject*) var.m_value.m_ref)->Count( );
      break;
   default:
      rv = 0;
      break;
   }

   a_thread->PushInt( rv );

   return GM_OK;
}

LIBDEF( lib_strcat )
{
   ostringstream ss;

   for (int i = 0; i < a_thread->GetNumParams( ); i++)
      ss << var2str( a_thread->Param( i ) );

   a_thread->PushNewString( ss.str( ).c_str( ) );
   return GM_OK;
}  

LIBDEF( lib_substr )
{
   GM_CHECK_NUM_PARAMS( 2 );
   gmStringObject* obj = a_thread->ParamStringObject( 0 );

   if (!obj)
   {
      GM_EXCEPTION_MSG( "Expected a string." );
      return GM_EXCEPTION;
   }

   GM_CHECK_INT_PARAM( off, 1 );
   off = std::max( off, 0 );

   if (off < obj->GetLength( ))
   {
      int len = a_thread->ParamInt( 2, obj->GetLength( ) - off );

      if (len > 0)
      {
         len = std::min( len, obj->GetLength( ) - off );

         /* PushNewString and consequently AllocStringObject expects the string
            to be null-terminated. >( */
         static string substr;
         substr.assign( obj->GetString( ) + off, len );
         a_thread->PushNewString( substr.c_str( ), len );

         return GM_OK;
      }
   }

   a_thread->PushNewString( "" );
   return GM_OK;
}

LIBDEF( lib_strcmp )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( a, 0 );
   GM_CHECK_STRING_PARAM( b, 1 );
   a_thread->PushInt( strcmp( a, b ) );
   return GM_OK;
}

LIBDEF( lib_stricmp )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( a, 0 );
   GM_CHECK_STRING_PARAM( b, 1 );
   a_thread->PushInt( stricmp( a, b ) );
   return GM_OK;
}

LIBDEF( lib_strchr )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( sz, 0 );
   GM_CHECK_STRING_PARAM( chr, 1 );

   const char* r = std::strpbrk( sz, chr );

   if (r)
   {
      a_thread->PushInt( (size_t) r - (size_t) sz );
      return GM_OK;
   }

   a_thread->PushNull( );
   return GM_OK;
}

LIBDEF( lib_strrchr )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( sz, 0 );
   GM_CHECK_STRING_PARAM( chr, 1 );

   char c;
   const char *_sz, *_chr;
   size_t len = strlen( sz );

   for (_sz = sz + (len - 1); len; --len,--_sz)
   {
      c = *_sz;

      for (_chr = chr; *_chr; ++_chr)
      {
         if (*_chr == c)
         {
            a_thread->PushInt( (size_t) _sz - (size_t) sz );
            return GM_OK;
         }
      }
   }

   a_thread->PushNull( );
   return GM_OK;
}

LIBDEF( lib_strrev )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( _sz, 0 );

   char* sz = strdup( _sz );
   a_thread->PushNewString( strrev( sz ) );
   free( sz );

   return GM_OK;
}

LIBDEF( lib_strstr )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( src, 0 );
   GM_CHECK_STRING_PARAM( substr, 1 );

   const char* f = strstr( src, substr );
   if (f)
      a_thread->PushInt( (size_t) f - (size_t) src );
   else
      a_thread->PushNull( );

   return GM_OK;
}

LIBDEF( lib_strlwr )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( _sz, 0 );
   
   char* sz = strdup( _sz );
   a_thread->PushNewString( strlwr( sz ) );
   free( sz );

   return GM_OK;
}

LIBDEF( lib_strupr )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( _sz, 0 );

   char* sz = strdup( _sz );
   a_thread->PushNewString( strupr( sz ) );
   free( sz );

   return GM_OK;
}

LIBDEF( lib_strtok )
{
   GM_CHECK_NUM_PARAMS( 2 );
   gmMachine* machine = a_thread->GetMachine( ); 
   gmTableObject* table = machine->AllocTableObject( );
   int table_len = 0;

   GM_CHECK_STRING_PARAM( _sz, 0 );
   GM_CHECK_STRING_PARAM( delims, 1 );
   char* sz = strdup( _sz );
   const char* tok = strtok( sz, delims );

   while (tok)
   {
      table->Set( machine, table_len++, 
                  gmVariable( machine->AllocStringObject( tok ) ) );
      tok = strtok( 0, delims );
   }

   free( sz );
   
   a_thread->PushTable( table );
   return GM_OK;
}

LIBDEF( lib_strtokargs )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz, 0 );

   gmMachine* machine = a_thread->GetMachine( ); 
   gmTableObject* table = machine->AllocTableObject( );
   int table_len = 0;
   char c;
   static string arg;

   while ((c = *sz) && c == ' ') ++sz;

   while ((c = *sz))
   {
      arg.clear( );
      bool esc = false;

      if (c == '\'' || c == '\"' || c == '`')   // quote-delimited
      {
         const char delim = *(sz++);

         while ((c = *sz))
         {
            ++sz;
            if (!esc)
            {
               if (c == delim)
                  break;
               else if (c == '\\')
                  esc = true;
               else
                  arg.push_back( c );
            }
            else
            {
               esc = false;

               if (c != delim)
                  arg.push_back( '\\' );

               arg.push_back( c );
            }
         }
      }
      else
      {
         do
         {
            ++sz;
            if (!esc)
            {
               if (c == ' ')
                  break;
               else if (c == '\\')
                  esc = true;
               else
                  arg.push_back( c );
            }
            else
            {
               esc = false;
               if (c != ' ')
                  arg.push_back( '\\' );
               arg.push_back( c );
            }
         } while ((c = *sz));
      }
      
      table->Set( machine, table_len++, 
                  gmVariable( machine->AllocStringObject( arg.c_str( ),
                                                          arg.length( ) ) ) );
   }

   a_thread->PushTable( table );
   return GM_OK;
}

/* strEsc( str, [char] ) */
LIBDEF( lib_stresc )
{
   if (a_thread->GetNumParams( ) < 1)
   {
      GM_EXCEPTION_MSG( "Expecting at least 1 parameter." );
      return GM_EXCEPTION;
   }
   
   GM_CHECK_STRING_PARAM( sz, 0 );
   
   char ch = ' '; // Escape spaces by default.

   if (a_thread->GetNumParams( ) > 1)
   {
      GM_CHECK_INT_PARAM( _ch, 1 );
      ch = _ch;
   }
   
   string str = sz;
   a_thread->PushNewString( escape_char( &str, ch ).c_str( ) );

   return GM_OK;
}

/* reMatch( subject, reop ) */
LIBDEF( lib_rematch )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( subject, 0 );
   GM_CHECK_STRING_PARAM( _def, 1 );

   // Form the complete definition.
   string def( "m" );
   def += _def;

   /* Create it. */
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   string err;
   RegexOp* reop = inst->createRegexOp( def, &err );

   if (!reop)
   {
      GM_EXCEPTION_MSG( err.c_str( ) );
      return GM_EXCEPTION;
   }

   if (reop->execute( subject, strlen( subject ), 0 )) // Matched successfully.
   {
      /* Save the captured substrings. */
      string* cap = inst->m_reops.caps;
      for (size_t i = 0; i < MAX_CAPTURES; i++, cap++)
      {
         cap->clear( );
         reop->getSubstr( subject, i, cap );
      }

      a_thread->PushInt( true );
   }
   else
      a_thread->PushInt( false );
   
   return GM_OK;
}

/* reMatch2( subject, regex, [manipulators] ) */
LIBDEF( lib_rematch2 )
{
   if (a_thread->GetNumParams( ) < 2)
   {
      GM_EXCEPTION_MSG( "Expected at least 2 parameters." );
      return GM_EXCEPTION;
   }

   string subject;
   {
      GM_CHECK_STRING_PARAM( _subject, 0 );
      subject = _subject;
   }
   string regex;
   {
      GM_CHECK_STRING_PARAM( _regex, 1 );
      regex = _regex;
   }

   const char* manips = "";

   if (a_thread->GetNumParams( ) > 2)
   {
      GM_CHECK_STRING_PARAM( _manips, 2 );
      manips = _manips;
   }

   // Form the complete definition.
   string def( "m~" );
   def += escape_char( &regex, '~' );
   def += '~';
   def += manips;

   /* Create it. */
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   string err;
   RegexOp* reop = inst->createRegexOp( def, &err );

   if (!reop)
   {
      GM_EXCEPTION_MSG( err.c_str( ) );
      return GM_EXCEPTION;
   }

   if (reop->execute( subject.c_str( ), subject.length( ), 0 ))  // Matched succesfully.
   {
      /* Save the captured substrings. */
      string* cap = inst->m_reops.caps;
      for (size_t i = 0; i < MAX_CAPTURES; i++, cap++)
      {
         cap->clear( );
         reop->getSubstr( subject.c_str( ), i, cap );
      }

      a_thread->PushInt( true );
   }
   else
      a_thread->PushInt( false );
   
   return GM_OK;
}

/* reSubst( subject, definition ) */
LIBDEF( lib_resubst )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( subject, 0 );
   GM_CHECK_STRING_PARAM( _def, 1 );

   // Form the complete definition.
   string def( "s" );
   def += _def;

   /* Create it. */
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   string err;
   RegexOp* reop = inst->createRegexOp( def, &err );

   if (!reop)
   {
      GM_EXCEPTION_MSG( err.c_str( ) );
      return GM_EXCEPTION;
   }

   static string result;
   result.clear( );
   if (reop->execute( subject, strlen( subject ), &result ))
   {
      /* Save the captured substrings. */
      string* cap = inst->m_reops.caps;
      for (size_t i = 0; i < MAX_CAPTURES; i++, cap++)
      {
         cap->clear( );
         reop->getSubstr( subject, i, cap );
      }
   }
   else
      result = subject;

   a_thread->PushNewString( result.c_str( ), result.length( ) );
   return GM_OK;
}

/* reSubst2( subject, regex, replacement, [manipulators] ) */
LIBDEF( lib_resubst2 )
{
   if (a_thread->GetNumParams( ) < 3)
   {
      GM_EXCEPTION_MSG( "Expected at least 3 parameters." );
      return GM_EXCEPTION;
   }

   string subject;
   {
      GM_CHECK_STRING_PARAM( _subject, 0 );
      subject = _subject;
   }
   string regex;
   {
      GM_CHECK_STRING_PARAM( _regex, 1 );
      regex = _regex;
   }
   string replacement;
   {
      GM_CHECK_STRING_PARAM( _replacement, 2 );
      replacement = _replacement;
   }

   const char* manips = "";

   if (a_thread->GetNumParams( ) > 3)
   {
      GM_CHECK_STRING_PARAM( _manips, 3 );
      manips = _manips;
   }

   // Form the complete definition.
   string def( "s~" );
   def += escape_char( &regex, '~' );
   def += '~';
   def += escape_char( &replacement, '~' );
   def += '~';
   def += manips;

   /* Create it. */
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   string err;
   RegexOp* reop = inst->createRegexOp( def, &err );

   if (!reop)
   {
      GM_EXCEPTION_MSG( err.c_str( ) );
      return GM_EXCEPTION;
   }

   static string result;
   result.clear( );
   if (reop->execute( subject.c_str( ), subject.length( ), &result ))
   {
      /* Save the captured substrings. */
      string* cap = inst->m_reops.caps;
      for (size_t i = 0; i < MAX_CAPTURES; i++, cap++)
      {
         cap->clear( );
         reop->getSubstr( subject.c_str( ), i, cap );
      }
   }
   else
      result = subject;

   a_thread->PushNewString( result.c_str( ), result.length( ) );
   return GM_OK;
}

/* reCap( num ) */
LIBDEF( lib_recap )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( index, 0 );
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   if ((unsigned) index < MAX_CAPTURES)
   {
      const string& cap = inst->m_reops.caps[ index ];
      a_thread->PushNewString( cap.c_str( ), cap.length( ) );
      return GM_OK;
   }

   a_thread->PushNull( );
   return GM_OK;
}

LIBDEF( lib_netscan )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( sz_pattern, 0 );
   GM_CHECK_STRING_PARAM( sz_subject, 1 );

   list<net_scan::FIELD> fields;
   if (net_scan::scan( sz_pattern, sz_subject, -1, fields ))
   {
      // Return each field in an array.
      gmTableObject* fields_tbl = a_thread->PushNewTable( );
      for (list<net_scan::FIELD>::const_iterator i = fields.begin( ); i != fields.end( ); ++i)
      {
         if (i->type == net_scan::FIELD::FIELD_INT)
         {
            fields_tbl->Set( a_thread->GetMachine( ), (int) fields_tbl->Count( ), 
                             gmVariable( i->value_int ) );
         }
         else if (i->type == net_scan::FIELD::FIELD_STR)
         {
            fields_tbl->Set( a_thread->GetMachine( ), 
                             (int) fields_tbl->Count( ),
                             gmVariable( a_thread->GetMachine( )->AllocStringObject( i->value_str.c_str( ) ) ) );
         }
         else
         {
            // wtf?
            assert( false );
         }
      }

      return GM_OK;
   }

   a_thread->PushNull( );
   return GM_OK;
}

LIBDEF( lib_parsechat )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz_chat, 0 );

   // Synchronized with parsechat.gms
   enum { CHAT_WHISPER = 0, CHAT_SHOUT, CHAT_SPEECH, CHAT_EMOTE, CHAT_DESCBODY,
          CHAT_EMIT, CHAT_COOKIE, CHAT_SUMMON, CHAT_JOIN, CHAT_SUMMONED,
          CHAT_JOINED, CHAT_DECLINED, CHAT_DESCHEAD, };

   using namespace yiffurc;
   NET_SESSION session;
   memset( &session, 0, sizeof( session ) );
   session.level = NET_LEVEL_LOGGEDIN;
   const S2C* cmd = s2c_parse( sz_chat, strlen( sz_chat ), &session );
   if (cmd)
   {
      gmMachine* machine = a_thread->GetMachine( );

      switch (cmd->type)
      {
      default:
      case S2C_TYPE_CHAT:
         a_thread->PushNull( );
         break;
      case S2C_TYPE_WHISPER:
         {
            const S2C_WHISPER* whisper = (const S2C_WHISPER*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( whisper->from ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( whisper->from_short ) ) );
            tbl->Set( machine, "message", gmVariable( machine->AllocStringObject( whisper->message ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_WHISPER ) );
         }
         break;
      case S2C_TYPE_SHOUT:
         {
            const S2C_WHISPER* shout = (const S2C_WHISPER*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( shout->from ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( shout->from_short ) ) );
            tbl->Set( machine, "message", gmVariable( machine->AllocStringObject( shout->message ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_SHOUT ) );
         }
         break;
      case S2C_TYPE_SPEECH:
         {
            const S2C_SPEECH* speech = (const S2C_SPEECH*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( speech->from ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( speech->from_short ) ) );
            tbl->Set( machine, "message", gmVariable( machine->AllocStringObject( speech->message ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_SPEECH ) );
         }
         break;
      case S2C_TYPE_EMOTE:
         {
            const S2C_EMOTE* emote = (const S2C_EMOTE*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( emote->from ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( emote->from_short ) ) );
            tbl->Set( machine, "message", gmVariable( machine->AllocStringObject( emote->message ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_EMOTE ) );
         }
         break;
      case S2C_TYPE_EMIT:
         {
            const S2C_EMIT* emit = (const S2C_EMIT*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "message", gmVariable( machine->AllocStringObject( emit->message ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_EMIT ) );
         }
         break;
      case S2C_TYPE_COOKIE:
         {
            const S2C_COOKIE* cookie = (const S2C_COOKIE*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( cookie->from ) ) );
            tbl->Set( machine, "note", gmVariable( machine->AllocStringObject( cookie->note ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_COOKIE ) );
         }
         break;
      case S2C_TYPE_DESCHEAD:
         {
            const S2C_DESCHEAD* deschead = (const S2C_DESCHEAD*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "player", gmVariable( machine->AllocStringObject( deschead->player ) ) );
            tbl->Set( machine, "player_short", gmVariable( machine->AllocStringObject( deschead->player_short ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_DESCHEAD ) );
         }
         break;
      case S2C_TYPE_DESCBODY:
         {
            const S2C_DESCBODY* descbody = (const S2C_DESCBODY*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "player_short", gmVariable( machine->AllocStringObject( descbody->player_short ) ) );
            tbl->Set( machine, "body", gmVariable( machine->AllocStringObject( descbody->body ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_DESCBODY ) );
         }
         break;
      case S2C_TYPE_DESCBODY2:
         {
            const S2C_DESCBODY2* descbody = (const S2C_DESCBODY2*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "player_short", gmVariable( machine->AllocStringObject( descbody->player_short ) ) );
            tbl->Set( machine, "body", gmVariable( machine->AllocStringObject( descbody->body ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_DESCBODY ) );
         }
         break;
      case S2C_TYPE_SUMMON:
         {
            const S2C_SUMMON* summon = (const S2C_SUMMON*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( summon->from ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( summon->from_short ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_SUMMON ) );
         }
         break;
      case S2C_TYPE_JOIN:
         {
            const S2C_JOIN* join = (const S2C_JOIN*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( join->from ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( join->from_short ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_JOIN ) );
         }
         break;
      case S2C_TYPE_SUMMONED:
         {
            const S2C_SUMMONED* summoned = (const S2C_SUMMONED*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( summoned->summoner ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( summoned->summoner_short ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_SUMMONED ) );
         }
         break;
      case S2C_TYPE_JOINED:
         {
            const S2C_JOINED* joined = (const S2C_JOINED*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( joined->joiner ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( joined->joiner_short ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_JOINED ) );
         }
         break;
      case S2C_TYPE_DECLINED:
         {
            const S2C_DECLINED* declined = (const S2C_DECLINED*) cmd;
            gmTableObject* tbl = a_thread->PushNewTable( );
            tbl->Set( machine, "from", gmVariable( machine->AllocStringObject( declined->player ) ) );
            tbl->Set( machine, "from_short", gmVariable( machine->AllocStringObject( declined->player_short ) ) );
            tbl->Set( machine, "type", gmVariable( CHAT_DECLINED ) );
         }
         break;
      } // switch (cmd->type)
   } // if (cmd)

   return GM_OK;
}

LIBDEF( lib_base95toint )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz, 0 );

   unsigned i = 0;
   while (*sz)
      i = (i * 95) + ((unsigned) *(sz++) - (unsigned) ' ');

   a_thread->PushInt( i );
   return GM_OK;
}

/* intToBase95( int, [width] ) */
LIBDEF( lib_inttobase95 )
{
   if (a_thread->GetNumParams( ) < 1) 
   {
      GM_EXCEPTION_MSG( "Expecting at least 1 parameter." );
      return GM_EXCEPTION;
   }

   unsigned n;
   {
      GM_CHECK_INT_PARAM( _n, 0 );
      n = _n;
   }

   unsigned width = 0;

   if (a_thread->GetNumParams( ) > 1)
   {
      GM_CHECK_INT_PARAM( _width, 1 );
      width = min( (unsigned) _width, (unsigned) 32 );
   }

   // Encode backwards.
   string str;
   if (n)
   {
      for ( ; n; n /= 95)
         str += (unsigned char) ' ' + n % 95;
   }
   else
      str = " ";

   if (width)
   {
      if (str.length( ) > width)
         // Clip to width.
         str.resize( width );
      else
         // Pad with zeroes.
         str.append( width - str.length( ), ' ' );
   }

   // Reverse.
   reverse( str.begin( ), str.end( ) );

   a_thread->PushNewString( str.c_str( ) );
   return GM_OK;
}

LIBDEF( lib_base220toint )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz, 0 );

   unsigned i = 0;
   for (unsigned mul = 1; *sz; mul *= 220, ++sz)
      i += (*(unsigned char*) sz - (unsigned char) '#') * mul;

   a_thread->PushInt( i );
   return GM_OK;
}

/* intToFurc( int, [width] ) */
LIBDEF( lib_inttobase220 )
{
   if (a_thread->GetNumParams( ) < 1) 
   {
      GM_EXCEPTION_MSG( "Expecting at least 1 parameter." );
      return GM_EXCEPTION;
   }

   unsigned n;
   {
      GM_CHECK_INT_PARAM( _n, 0 );
      n = _n;
   }

   unsigned width = 0;

   if (a_thread->GetNumParams( ) > 1)
   {
      GM_CHECK_INT_PARAM( _width, 1 );
      width = min( (unsigned) _width, (unsigned) 32 );
   }

   string str;
   if (n)
   {
      for ( ; n; n /= 220)
         str += (unsigned char) '#' + n % 220;
   }
   else
      str = "#";

   if (width)
   {
      // Clip to width.
      if (str.length( ) > width)
         str.resize( width );
      else
         // Pad with zeroes.
         str.append( width - str.length( ), '#' );
   }

   a_thread->PushNewString( str.c_str( ) );
   return GM_OK;
}

LIBDEF( lib_asciitolatin)
{
   GM_CHECK_STRING_PARAM( sz_ascii, 0 );

   string latin = xml_charset::ascii_to_latin( sz_ascii );
   a_thread->PushNewString( latin.c_str( ), latin.length( ) );

   return GM_OK;
}

LIBDEF( lib_getcolorlen )
{
   GM_CHECK_STRING_PARAM( sz_cc, 0 );

   a_thread->PushInt( (int) yiffurc::get_color_len( sz_cc, strlen( sz_cc ) ) );

   return GM_OK;
}

LIBDEF( lib_dbopen )
{
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   inst->m_db->open( true );

   return GM_OK;
}

LIBDEF( lib_dbclose )
{
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   inst->m_db->close( );

   return GM_OK;
}

LIBDEF( lib_dbreadsectionname )
{
	GM_CHECK_NUM_PARAMS( 1 );
    GM_CHECK_INT_PARAM( field, 0 );

	ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
      
    const string& value = inst->m_db->readSectionName( field );

    a_thread->PushNewString( value.c_str( ) );

	return GM_OK;
}

LIBDEF( lib_dbread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( field, 0 );
   const char* section = a_thread->ParamString( 1, "" );

   if (*field)
   {
      ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
      
      const string& value = inst->m_db->read( field, section );

      a_thread->PushNewString( value.c_str( ) );
   }
   else
      a_thread->PushNewString( "" );

   return GM_OK;
}

LIBDEF( lib_dbreadat )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( index, 0 );
   const char* section = a_thread->ParamString( 1, "" );
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   
   string key;
   const string& value = inst->m_db->read( (size_t) index, &key, section );

   if (!key.length( ))
      a_thread->PushNull( );
   else
   {
      gmTableObject* table = inst->m_machine.AllocTableObject( );
      gmStringObject* str = inst->m_machine.AllocStringObject( key.c_str( ) );
      table->Set( &inst->m_machine, "key", gmVariable( str ) );
      str = inst->m_machine.AllocStringObject( value.c_str( ) );
      table->Set( &inst->m_machine, "value", gmVariable( str ) );
      a_thread->PushTable( table );
   }

   return GM_OK;
}

LIBDEF( lib_dbwrite )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( field, 0 );
   GM_CHECK_STRING_PARAM( value, 1 );
   const char* section = a_thread->ParamString( 2, "" );

   if (*field)
   {
      ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

      inst->m_db->write( field, value, section );
   }

   return GM_OK;
}

LIBDEF( lib_dbpurge )
{
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   inst->m_db->purge( );

   return GM_OK;
}

LIBDEF( lib_dberase )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( field, 0 );
   const char* section = a_thread->ParamString( 1, "" );

   if (*field)
   {
      ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

      inst->m_db->erase( field, section );
   }

   return GM_OK;
}

LIBDEF( lib_dberasesection )
{
   const char* section = a_thread->ParamString( 0, "" );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   inst->m_db->eraseSection( section );

   return GM_OK;
}

LIBDEF ( lib_dbcountsections )
{
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   a_thread->PushInt( inst->m_db->countSections() );
   
   return GM_OK;
}

LIBDEF( lib_dbcount )
{
   const char* section = a_thread->ParamString( 0, "" );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   a_thread->PushInt( inst->m_db->count( section ) );
   
   return GM_OK;
}   

LIBDEF( lib_log )
{
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   /* Append to the log file. */
   FILE* f = std::fopen( inst->m_log_file.c_str( ), "a" );

   if (f)
   {
      for (int i = 0; i < a_thread->GetNumParams( ); i++)
         fprintf( f, "%s", var2str( a_thread->Param( i ) ).c_str( ) );
   }

   std::fclose( f );

   return GM_OK;
}

LIBDEF( lib_rand )
{
   int max = a_thread->ParamInt( 0, 0x7FFFFFFF );

   if (max <= 0)
      max = 1; // No divide by zero or negs.

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   unsigned int& s = inst->m_rand_state;
   s = 16807 * (s - (s/127773)  * 127773) - 2836 * (s/127773);
   a_thread->PushInt( s % max );

   return GM_OK;
}

LIBDEF( lib_srand )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( seed, 0 );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   inst->m_rand_state = seed;

   return GM_OK;
}

LIBDEF( lib_uptime )
{
   a_thread->PushInt( (int) (time( 0 ) - core::get_start_time( )) );

   return GM_OK;
}

LIBDEF( lib_getname )
{
   a_thread->PushNewString( world::get_player( ).name.c_str( ) );

   return GM_OK;
}

LIBDEF( lib_getdesc )
{
   a_thread->PushNewString( world::get_player( ).desc.c_str( ) );

   return GM_OK;
}

LIBDEF( lib_getcolors )
{
   a_thread->PushNewString( world::get_player( ).colors.c_str( ) );
   return GM_OK;
}

LIBDEF( lib_getmonsters )
{
   vector<unsigned> vguids;
   world::monster_enum( &vguids );

   gmTableObject* table = a_thread->PushNewTable( );

   for (vector<unsigned>::size_type i = 0; i < vguids.size( ); ++i)
      table->Set( a_thread->GetMachine( ), (int) i, gmVariable( (int) vguids[ i ] ) );

   return GM_OK;
}

// getMonsterInfo( guid ) or getMonsterInfo( name )
LIBDEF( lib_getmonsterinfo )
{
   GM_CHECK_NUM_PARAMS( 1 );
   gmVariable param = a_thread->Param( 0 );
   const MONSTERINFO* monster = NULL;

   if (param.m_type == GM_INT)
   {
      // By GUID.
      monster = world::monster_find( (unsigned) param.m_value.m_int );
   }
   else if (param.m_type == GM_STRING)
   {
      // By name.
      monster = world::monster_find( shortname::make_shortname( ((gmStringObject*) param.m_value.m_ref)->GetString( ) ), true );
   }
   else
   {
      GM_EXCEPTION_MSG( "Expected parameter 0 as an integer or a string." );
      return GM_EXCEPTION;
   }

   if (!monster)
   {
      a_thread->PushNull( );
      return GM_OK;
   }

   gmTableObject* table = a_thread->PushNewTable( );
   gmMachine* machine   = a_thread->GetMachine( );

   table->Set( machine, "guid", gmVariable( (int) monster->guid ) );
   table->Set( machine, "name", gmVariable( machine->AllocStringObject( monster->name.c_str( ) ) ) );
   table->Set( machine, "color", gmVariable( machine->AllocStringObject( monster->color.c_str( ) ) ) );
   table->Set( machine, "frame", gmVariable( (int) monster->frame ) );
   table->Set( machine, "visible", gmVariable( (int) monster->visible ) );
   table->Set( machine, "has_profile", gmVariable( (int) (monster->flags & MONSTER_HASPROFILE) ) );
   {
      gmTableObject* pos_table = machine->AllocTableObject( );
      pos_table->Set( machine, "x", gmVariable( (int) monster->pos.x ) );
      pos_table->Set( machine, "y", gmVariable( (int) monster->pos.y ) );
      table->Set( machine, "pos", gmVariable( pos_table ) );
   }

   return GM_OK;
}

LIBDEF( lib_getpos )
{
   const POINT2& pos = world::get_player( ).cam;

   // Return as an associative table.
   gmTableObject* table = a_thread->PushNewTable( );
   table->Set( a_thread->GetMachine( ), "x", gmVariable( (int) pos.x ) );
   table->Set( a_thread->GetMachine( ), "y", gmVariable( (int) pos.y ) );

   return GM_OK;
}

LIBDEF( lib_desctag )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz, 0 );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   world::desctag_set( inst->m_desctag_id, sz );
   return GM_OK;
}

LIBDEF( lib_badgecancel )
{
   GM_CHECK_NUM_PARAMS( 0 );
   world::badge_cancel( );
   return GM_OK;
}

LIBDEF( lib_flashwindow )
{
   // Only flash if minimized or not the foreground window and the foreground
   // window isn't in the same thread.
   HWND mywnd = window::get_handle( );
   if (IsIconic( mywnd ) || 
         (GetForegroundWindow( ) != mywnd && 
          GetCurrentThreadId( ) != GetWindowThreadProcessId( GetForegroundWindow( ), NULL )))
   {
      FlashWindow( mywnd, TRUE );
   }

   return GM_OK;
}

LIBDEF( lib_getwindowtitle )
{
   static char sz[ 256 ];
   GetWindowText( window::get_handle( ), sz, 256 );
   sz[ 255 ] = '\0';

   a_thread->PushNewString( sz );
   return GM_OK;
}

LIBDEF( lib_setwindowtitle )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( title, 0 );

   SetWindowText( window::get_handle( ), title );
   
   return GM_OK;
}

LIBDEF( lib_ctrlstate )
{
   if (GetKeyState( VK_CONTROL ) & 0x80000000)
      a_thread->PushInt( 1 );
   else
      a_thread->PushInt( 0 );
   return GM_OK;
}

LIBDEF( lib_altstate )
{
   if (GetKeyState( VK_MENU ) & 0x80000000)
      a_thread->PushInt( 1 );
   else
      a_thread->PushInt( 0 );
   return GM_OK;
}

LIBDEF( lib_shiftstate )
{
   if (GetKeyState( VK_SHIFT ) & 0x80000000)
      a_thread->PushInt( 1 );
   else
      a_thread->PushInt( 0 );
   return GM_OK;
}

LIBDEF( lib_inetcount )
{
   a_thread->PushInt( (int) net::get_inbound_linecount( ) );

   return GM_OK;
}


LIBDEF( lib_onetcount )
{
   a_thread->PushInt( (int) net::get_outbound_linecount( ) );

   return GM_OK;
}

LIBDEF( lib_speak )
{
   ostringstream ss;
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
      ss << var2str( a_thread->Param( i ) );

   core::system_speak( ss.str( ) );
   return GM_OK;
}

LIBDEF( lib_errspeak )
{
   ostringstream ss;
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
      ss << var2str( a_thread->Param( i ) );

   core::system_errspeak( ss.str( ) );
   return GM_OK;
}

LIBDEF( lib_onet )
{
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
   {
      GM_CHECK_STRING_PARAM( sz, i );
      net::push_outbound( sz, strlen( sz ) );
   }

   return GM_OK;
}

LIBDEF( lib_onetline )
{
   string line;
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
   {
      GM_CHECK_STRING_PARAM( sz, i );
      line += sz;
   }
   a_thread->PushInt( net::push_outbound_line( line.c_str( ), line.length( ) ) );

   return GM_OK;
}

LIBDEF( lib_inet )
{
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
   {
      GM_CHECK_STRING_PARAM( sz, i );
      net::push_inbound( sz, strlen( sz ) );
   }

   return GM_OK;
}

LIBDEF( lib_inetline )
{
   string line;
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
   {
      GM_CHECK_STRING_PARAM( sz, i );
      line += sz;
   }
   a_thread->PushInt( net::push_inbound_line( line.c_str( ), line.length( ) ) );

   return GM_OK;
}

LIBDEF( lib_hideline )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( flags, 0 );

   scripts::hide_line( flags );
   return GM_OK;
}

LIBDEF( lib_cmd )
{
   GM_CHECK_NUM_PARAMS( 1 );   
   GM_CHECK_STRING_PARAM( sz, 0 );
   int noecho = a_thread->ParamInt( 1, 0 );

   core::push_command( sz, noecho ? true : false );
   return GM_OK;
}

LIBDEF( lib_exit )
{
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   inst->m_retval = a_thread->ParamInt( 0, 0 );
   inst->kill( );

   return GM_SYS_KILL;
}

LIBDEF( lib_getargs )
{
   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   /* Put the args into table form. */
   gmTableObject* table = a_thread->PushNewTable( );
   
   int index = 0;
   for (ArgList::iterator i = inst->m_args.begin( );
        i != inst->m_args.end( ); ++i)
   {
      gmStringObject* str_obj = 
         inst->m_machine.AllocStringObject( (*i).c_str( ), (*i).length( ) );
      table->Set( &inst->m_machine, index++, gmVariable( str_obj ) );
   }

   return GM_OK;
}

LIBDEF( lib_print )
{
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
      std::cout << var2str( a_thread->Param( i ) );
   
   return GM_OK;
}

LIBDEF( lib_import )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( name, 0 );

   // Use the per-user file if it exists.
   string file = core::get_furn_doc_root( ) + "\\scripts\\imports\\" + name + ".gm";
   if (GetFileAttributes( file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
      // Fall back to program file.
      file = core::get_furn_root( ) + "\\scripts\\imports\\" + name + ".gm";

   /* Load and run the script. */
   ifstream fs( file.c_str( ) );

   if (fs.fail( ))
   {
      GM_EXCEPTION_MSG( "Couldn't load import script '%s'.", file.c_str( ) );
      return GM_EXCEPTION;
   }

   string str_script = string( std::istreambuf_iterator<char>( fs ) , 
                               std::istreambuf_iterator<char>( ) );
   fs.close( );

   gmMachine* machine = a_thread->GetMachine( );
   int thread_id = GM_INVALID_THREAD;
   if (machine->ExecuteString( str_script.c_str( ), &thread_id ))
   {
      GM_EXCEPTION_MSG( "Import script '%s' had compilation errors.", file.c_str( ) );
      ((ScriptInst*) machine->m_userParam)->speakErrors( );
      return GM_EXCEPTION;
   }
   else if (thread_id != GM_INVALID_THREAD 
            && machine->GetThread( thread_id ))
   {
      GM_EXCEPTION_MSG( "Import script '%s' encountered an exception.", file.c_str( ) );
      ((ScriptInst*) machine->m_userParam)->speakErrors( );
      return GM_EXCEPTION;
   }
   
   return GM_OK;
}

LIBDEF( lib_modmsg )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( mod, 0 );

   string msg;
   for (int i = 1; i < a_thread->GetNumParams( ); i++)
   {
      GM_CHECK_STRING_PARAM( sz, i );
      msg += sz;
   }

   a_thread->PushInt( modules::msg_module( (unsigned int) mod, 
                                           const_cast<char*>( msg.c_str( ) ) ) );
   return GM_OK;
}

LIBDEF( lib_modmsgsz )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( mod, 0 );

   string msg;
   for (int i = 1; i < a_thread->GetNumParams( ); i++)
   {
      GM_CHECK_STRING_PARAM( sz, i );
      msg += sz;
   }

   // Interpret the return value as a c-string.
   const char* sz = (const char*) modules::msg_module( (unsigned int) mod, 
                                                       const_cast<char*>( msg.c_str( ) ) );
   if (sz)
      a_thread->PushNewString( sz );
   else
      a_thread->PushNull( );

   return GM_OK;
}

LIBDEF( lib_modfind )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz_id, 0 );
   a_thread->PushInt( modules::find_module( UUID128( sz_id ) ) );
   return GM_OK;
}

LIBDEF( lib_getclipboard )
{
   GM_CHECK_NUM_PARAMS( 0 );

   if (OpenClipboard( window::get_handle( ) ))
   {
      HANDLE hmem = GetClipboardData( CF_TEXT );
      if (hmem)
      {
         a_thread->PushNewString( (const char*) GlobalLock( hmem ) );
         GlobalUnlock( hmem );
      }
      CloseClipboard( );
   }

   return GM_OK;
}

LIBDEF( lib_setclipboard )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz_text, 0 );

   if (OpenClipboard( window::get_handle( ) ))
   {
      EmptyClipboard( );

      HANDLE hmem = GlobalAlloc( GMEM_MOVEABLE, strlen( sz_text ) + 1 );
      char* buf = (char*) GlobalLock( hmem );
      strcpy( buf, sz_text );
      GlobalUnlock( hmem );

      if (!SetClipboardData( CF_TEXT, hmem ))
         GlobalFree( hmem );

      CloseClipboard( );
   }

   return GM_OK;
}

LIBDEF( lib_collectgarbage )
{
   bool full = false;
   if (a_thread->GetNumParams( ) > 0)
   {
      GM_CHECK_NUM_PARAMS( 1 );
      GM_CHECK_INT_PARAM( _full, 0 );
      full = _full != 0;
   }

   a_thread->PushInt( a_thread->GetMachine( )->CollectGarbage( full ) );
   return GM_OK;
}

/* rgb( red, green, blue ) */
LIBDEF( lib_rgb )
{
   GM_CHECK_NUM_PARAMS( 3 );
   GM_CHECK_INT_PARAM( r, 0 );
   GM_CHECK_INT_PARAM( g, 1 );
   GM_CHECK_INT_PARAM( b, 2 );

   a_thread->PushInt( RGB( r, g, b ) );
   return GM_OK;
}

/* rgbUnpack( color ) */
LIBDEF( lib_rgbunpack )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( color, 0 );

   // Return a table with r, g, b keys.
   gmTableObject* rgb = a_thread->PushNewTable( );
   rgb->Set( a_thread->GetMachine( ), "r", gmVariable( GetRValue( color ) ) );
   rgb->Set( a_thread->GetMachine( ), "g", gmVariable( GetGValue( color ) ) );
   rgb->Set( a_thread->GetMachine( ), "b", gmVariable( GetBValue( color ) ) );
   return GM_OK;
}

/* termCreate( id, atts ) */
LIBDEF( lib_termcreate )
{
   if (a_thread->GetNumParams( ) > 1)
   {
      GM_EXCEPTION_MSG( "Too many parameters." );
      return GM_EXCEPTION;
   }

   gmTableObject* atts = NULL;

   if (a_thread->GetNumParams( ) == 1)
   {
      if (a_thread->ParamType( 0 ) != GM_TABLE)
      {
         GM_EXCEPTION_MSG( "Expecting parameter as a table." );
         return GM_EXCEPTION;
      }

      atts = a_thread->ParamTable( 0 );
   }

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   // Generate a unique term ID.
   TERMINST term = { NULL, 0, NULL, NULL };
   for (; ; ++term.id)
   {
      list<TERMINST>::const_iterator i = inst->m_terms.begin( );
      for (; i != inst->m_terms.end( ); ++i)
      {
         if (i->id == term.id)
            break;
      }
      if (i == inst->m_terms.end( ))
         break;
   }

   // Create the term.
   term.term = new ScriptTerm( inst, window::get_handle( ) );

   assert( !term.term->isDestroyed( ) );

   // Add it to the list.
   inst->m_terms.push_back( term );

   // Set attributes.
   if (atts)
      term_set( &inst->m_terms.back( ), atts, &inst->m_machine );

   a_thread->PushInt( term.id );

   return GM_OK;
}

/* termDestroy( id ) */
LIBDEF( lib_termdestroy )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( id, 0 );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   // Destroy any terms with a matching id.
   while (true)
   {
      list<TERMINST>::iterator i = find_term_inst_by_id( id, inst->m_terms.begin( ), inst->m_terms.end( ) );
      if (i != inst->m_terms.end( ))
         i->term->destroy( );
      else
         break;
   }

   return GM_OK;
}

/* termSet( id, atts ) */
LIBDEF( lib_termset )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_INT_PARAM( id, 0 );
   GM_CHECK_TABLE_PARAM( atts, 1 );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   // Set atts on any terms with a matching id.
   for (list<TERMINST>::iterator beg = inst->m_terms.begin( );
        beg != inst->m_terms.end( ); )
   {
      list<TERMINST>::iterator i = find_term_inst_by_id( id, beg, inst->m_terms.end( ) );
      if (i != inst->m_terms.end( ))
      {
         term_set( &*i, atts, &inst->m_machine );
         beg = ++i;
      }
      else
         break;
   }

   return GM_OK;
}

/* termGet( id ) */
LIBDEF( lib_termget )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( id, 0 );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;

   // Retrieve attributes on the first term with a matching id.
   list<TERMINST>::iterator i = find_term_inst_by_id( id, inst->m_terms.begin( ), inst->m_terms.end( ) );
   if (i != inst->m_terms.end( ))
   {
      // Fetch atts and put them into a table.
      gmTableObject* table = a_thread->PushNewTable( );
      gmMachine* machine = a_thread->GetMachine( );

      table->Set( machine, "id", gmVariable( i->id ) );
      table->Set( machine, "title", gmVariable( machine->AllocStringObject( i->term->getTitle( ).c_str( ) ) ) );
      
      if (i->on_destroy)
         table->Set( machine, "onDestroy", gmVariable( i->on_destroy ) );
      else
      {
         gmVariable null_var; null_var.Nullify( );
         table->Set( machine, "onDestroy", null_var );
      }

      if (i->on_input)
         table->Set( machine, "onInput", gmVariable( i->on_input ) );
      else
      {
         gmVariable null_var; null_var.Nullify( );
         table->Set( machine, "onInput", null_var );
      }

      {
         gmTableObject* font_table = machine->AllocTableObject( );
         
         unsigned font_height = 0;
         string font_name = i->term->getFont( &font_height );

         font_table->Set( machine, "name", gmVariable( machine->AllocStringObject( font_name.c_str( ) ) ) );
         font_table->Set( machine, "height", gmVariable( (int) font_height ) );

         table->Set( machine, "font", gmVariable( font_table ) );
      }
      {
         gmTableObject* types_table = machine->AllocTableObject( );

#define _ADDTYPE( type, key ) { \
                                 gmTableObject* t = machine->AllocTableObject( ); \
                                 t->Set( machine, "style", gmVariable( (int) i->term->getTextType( type ).style ) ); \
                                 t->Set( machine, "color", gmVariable( (int) i->term->getTextType( type ).color ) ); \
                                 types_table->Set( machine, key, gmVariable( t ) ); \
                              }

         _ADDTYPE( ScriptTerm::TEXT_OUTPUT, "output" );
         _ADDTYPE( ScriptTerm::TEXT_LINK, "link" );
         _ADDTYPE( ScriptTerm::TEXT_SUCCESS, "success" );
         _ADDTYPE( ScriptTerm::TEXT_WARNING, "warning" );
         _ADDTYPE( ScriptTerm::TEXT_ERROR, "error" );
         _ADDTYPE( ScriptTerm::TEXT_CHANNEL, "channel" );
         _ADDTYPE( ScriptTerm::TEXT_WHISPER, "whisper" );
         _ADDTYPE( ScriptTerm::TEXT_EMOTE, "emote" );
         _ADDTYPE( ScriptTerm::TEXT_EMIT, "emit" );
         _ADDTYPE( ScriptTerm::TEXT_MYSPEECH, "myspeech" );
         _ADDTYPE( ScriptTerm::TEXT_BCAST, "bcast" );
         _ADDTYPE( ScriptTerm::TEXT_ROLL, "roll" );
         _ADDTYPE( ScriptTerm::TEXT_SHOUT, "shout" );
         _ADDTYPE( ScriptTerm::TEXT_TRADE, "trade" );
         _ADDTYPE( ScriptTerm::TEXT_DRAGONSPEAK, "dragonspeak" );
         _ADDTYPE( ScriptTerm::TEXT_NOTIFY, "notify" );
         _ADDTYPE( ScriptTerm::TEXT_QUERY, "query" );
         _ADDTYPE( ScriptTerm::TEXT_HIGHLIGHT, "highlight" );
         _ADDTYPE( ScriptTerm::TEXT_LOG, "log" );
         _ADDTYPE( ScriptTerm::TEXT_HISTORY, "history" );
         _ADDTYPE( ScriptTerm::TEXT_FURCLINK, "furclink" );
         _ADDTYPE( ScriptTerm::TEXT_CMDLINK, "cmdlink" );
         _ADDTYPE( ScriptTerm::TEXT_NAME, "name" );
         _ADDTYPE( ScriptTerm::TEXT_INPUT, "input" );
         _ADDTYPE( ScriptTerm::TEXT_F2CMDLINK, "f2cmdlink" );

#undef _ADDTYPE

         table->Set( machine, "types", gmVariable( types_table ) );
      }
      {
         gmTableObject* bgcolor_table = machine->AllocTableObject( );

         COLORREF color_input, color_output;
         i->term->getBgColors( &color_input, &color_output );

         bgcolor_table->Set( machine, "input", gmVariable( (int) color_input ) );
         bgcolor_table->Set( machine, "output", gmVariable( (int) color_output ) );

         table->Set( machine, "bgColors", gmVariable( bgcolor_table ) );
      }
   }
   else
      a_thread->PushNull( );

   return GM_OK;
}

/* termWrite( id, line, [style, [color]] ) */
LIBDEF( lib_termwrite )
{
   if (a_thread->GetNumParams( ) < 2)
   {
      GM_EXCEPTION_MSG( "Expecting at least 2 parameters." );
      return GM_EXCEPTION;
   }

   GM_CHECK_INT_PARAM( id, 0 );
   GM_CHECK_STRING_PARAM( sz_line, 1 );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   list<TERMINST>::iterator i = find_term_inst_by_id( id, inst->m_terms.begin( ), inst->m_terms.end( ) );
   if (i != inst->m_terms.end( ))
   {
      if (a_thread->GetNumParams( ) > 2)
      {
         gmVariable v_style = a_thread->Param( 2 );
         if (v_style.m_type != GM_INT && v_style.m_type != GM_NULL)
         {
            GM_EXCEPTION_MSG( "Parameter 3 must be an int or null." );
            return GM_EXCEPTION;
         }

         if (a_thread->GetNumParams( ) > 3)
         {
            GM_CHECK_INT_PARAM( color, 3 );
            i->term->write( sz_line, 
                            v_style.m_type == GM_NULL ? NULL : &v_style.m_value.m_int, 
                            (COLORREF*) &color );
         }
         else
            i->term->write( sz_line, 
                            v_style.m_type == GM_NULL ? NULL : &v_style.m_value.m_int );
      }
      else
         i->term->write( sz_line );
   }

   return GM_OK;
}

/* termPrint( id, line, [style, [color]] ) */
LIBDEF( lib_termprint )
{
   if (a_thread->GetNumParams( ) < 2)
   {
      GM_EXCEPTION_MSG( "Expecting at least 2 parameters." );
      return GM_EXCEPTION;
   }
   if (a_thread->GetNumParams( ) > 4)
   {
      GM_EXCEPTION_MSG( "Too many parameters." );
      return GM_EXCEPTION;
   }
   
   GM_CHECK_INT_PARAM( id, 0 );
   GM_CHECK_STRING_PARAM( sz_line, 1 );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   list<TERMINST>::iterator i = find_term_inst_by_id( id, inst->m_terms.begin( ), inst->m_terms.end( ) );
   if (i != inst->m_terms.end( ))
   {
      if (a_thread->GetNumParams( ) > 2)
      {
         gmVariable v_style = a_thread->Param( 2 );
         if (v_style.m_type != GM_INT && v_style.m_type != GM_NULL)
         {
            GM_EXCEPTION_MSG( "Parameter 3 must be an int or null." );
            return GM_EXCEPTION;
         }

         if (a_thread->GetNumParams( ) > 3)
         {
            GM_CHECK_INT_PARAM( color, 3 );
            i->term->print( sz_line, 
                            v_style.m_type == GM_NULL ? NULL : &v_style.m_value.m_int, 
                            (COLORREF*) &color );
         }
         else
            i->term->print( sz_line, 
                            v_style.m_type == GM_NULL ? NULL : &v_style.m_value.m_int );
      }
      else
         i->term->print( sz_line );
   }

   return GM_OK;
}

/* termFlush( id ) */
LIBDEF( lib_termflush )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( id, 0 );

   ScriptInst* inst = (ScriptInst*) a_thread->GetMachine( )->m_userParam;
   list<TERMINST>::iterator i = find_term_inst_by_id( id, inst->m_terms.begin( ), inst->m_terms.end( ) );
   if (i != inst->m_terms.end( ))
      i->term->clear( );

   return GM_OK;
}

LIBDEF( lib_ignore )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz_name, 0 );

   if (*sz_name)
      client::ignore_add( sz_name );
   return GM_OK;
}

LIBDEF( lib_unignore )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz_name, 0 );

   if (*sz_name)
      client::ignore_rm( sz_name );
   return GM_OK;
}

LIBDEF( lib_toshortname )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( sz_name, 0 );

   a_thread->PushNewString( shortname::make_shortname( sz_name ).c_str( ) );

   return GM_OK;
}