#ifndef _FURNARCHYCORE_SCRIPTS_SCRIPTINST_H__
#define _FURNARCHYCORE_SCRIPTS_SCRIPTINST_H__

#include "common.h"
#include "common/CacheMap.h"
#include "common/IniFile.h"
#include "gm/gmMachine.h"
#include "ScriptTerm.h"

class RegexOp;

/** A running script. */
class ScriptInst
{
private:
   class Library;

   struct TERMINST
   {
      ScriptTerm*       term;
      int               id;
      gmFunctionObject* on_destroy;
      gmFunctionObject* on_input;
   };

public:
   // For CacheMap.
   struct STRHASH
   {
      inline size_t operator( )( const string& key )
      {
         const char* c = key.c_str( );
         size_t r = 0;

         while (*c) 
            r = *(c++) + (r << 6) + (r << 16) - r;

         return r;
      }
   };

public:
   enum RunLevel { RL_SUSPENDED, RL_RUNNING, RL_KILLED };
   enum { GC_RATE = 10000 };

private:
   ScriptInst( ) { }
   ScriptInst( const ScriptInst& ) { }
public:
   ScriptInst( unsigned int sid, const ArgList& args );
   ~ScriptInst( );

   RunLevel       getRunLevel( ) const;
   const string&  getName( ) const;
   int            getRetVal( ) const;
   const ArgList& getArgs( ) const;
   unsigned int   getSid( ) const;
   void           start( );
   void           suspend( );
   void           kill( );

   void           onTick( unsigned int dt );
   bool           onCommand( const ArgList& args );
   void           onInbound( string* line, int from, size_t num );
   void           onOutbound( string* line, int from, size_t num );
   bool           onWndProc( WNDPROCPARAMS* params );

public:
   /* For ScriptTerm. */
   void onTermDestroy( ScriptTerm* term );
   void onTermInput( ScriptTerm* term, string& input );

private:
   void   speakErrors( );
   size_t findHandlers( );
   void   collectGarbage( );
   RegexOp* createRegexOp( const string& definition, string* error = NULL );

private:
   typedef CacheMap< string, RegexOp*, STRHASH > ReopCache;
   enum { FN_ONRUN, FN_ONKILL, FN_ONINBOUND, FN_ONOUTBOUND, 
          FN_ONCOMMAND, FN_ONTICK, FN_ONCHAR, FN_MAX };
   enum { REOP_CACHE_LIMIT = 64, MAX_CAPTURES = 9 };

   unsigned int      m_sid;
   string            m_script_file;
   string            m_log_file;
   RunLevel          m_runlevel;
   bool              m_started;
   int               m_retval;
   ArgList           m_args;
   gmMachine         m_machine;
   unsigned int      m_rand_state;
   int               m_desctag_id;
   IniFile*          m_db;
   gmFunctionObject* m_handlers[ FN_MAX ];
   unsigned int      m_gc_countdown;

   struct _REOPS
   {
      ReopCache cache;
      string    caps[ MAX_CAPTURES + 1 ];

      _REOPS( ) : cache( REOP_CACHE_LIMIT ) { }
   }                 m_reops;

   list<TERMINST>    m_terms;
};

inline unsigned int ScriptInst::getSid( ) const
{
   return m_sid;
}

inline ScriptInst::RunLevel ScriptInst::getRunLevel( ) const
{
   return m_runlevel;
}

inline const string& ScriptInst::getName( ) const
{
   return m_args[ 0 ];
}

inline int ScriptInst::getRetVal( ) const
{
   return m_retval;
}

inline const ArgList& ScriptInst::getArgs( ) const
{
   return m_args;
}

inline void ScriptInst::suspend( )
{
   m_runlevel = RL_SUSPENDED;
}

#endif
