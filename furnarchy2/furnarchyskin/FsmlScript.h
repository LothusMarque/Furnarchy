#ifndef _FSML_SKIN_SCRIPT_H__
#define _FSML_SKIN_SCRIPT_H__

#include "FsmlElem.h"
#include "gm/gmMachine.h"
#include "common/IniFile.h"
#include "common/SzMap.h"
#include <cstdarg>

class FsmlScript
{
   class UserT_Element;
   class Library;
   typedef SzMap<gmFunctionObject*> FuncMap;
   typedef std::vector<FsmlElem*> ElemVector;
   enum { USERTYPE_ELEMENT, NUM_USERTYPES };
   enum { GC_RATE = 10000 };
private:
   FsmlScript( ) { }
public:
   FsmlScript( FsmlElemSkin* skin, const string& script_file, 
               const string& db_file );
   ~FsmlScript( );

   void tick( unsigned int dt );

   int  call( FsmlElem* from, const char* name, const char* fmt = "", ... );
   int  call( const char* name, const char* fmt = "", ... );
   void pushConstructedElem( FsmlElem* elem );

private:
   int call( FsmlElem* from, const char* name, const char* fmt, 
             std::va_list args );
   void speakErrors( );

private:
   FsmlElemSkin* m_skin;
   gmMachine     m_machine;
   FuncMap       m_funcs;
   int           m_usertypes[ NUM_USERTYPES ];
   IniFile*      m_db;
   ElemVector    m_constructed;
   unsigned int  m_gc_countdown;
};

inline int FsmlScript::call( FsmlElem* from, const char* name, 
                             const char* fmt /* = "" */, ... )
{
   if (!name)
      return 0;

   std::va_list args;
   va_start( args, fmt );
   return call( from, name, fmt, args );
}

inline int FsmlScript::call( const char* name, 
                             const char* fmt /* = "" */, ... )
{
   if (!name)
      return 0;

   std::va_list args;
   va_start( args, fmt );
   return call( 0, name, fmt, args );
}

inline void FsmlScript::pushConstructedElem( FsmlElem* elem )
{
   m_constructed.push_back( elem );
}

#endif
