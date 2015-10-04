#define WIN32_LEAN_AND_MEAN
#include "FsmlScript.h"
#include "gm/gmCall.h"
#include "../furnarchycore/furn2.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace furn2;

//////////////////////////////////////////////////////////////////////////

class FsmlScript::UserT_Element
{
public:
   // Must be a weak reference to allow proper destruction.
   FsmlElem* ptr;
   UserT_Element( FsmlElem* ptr ) : ptr( ptr ) { }
private:
   static void GM_CDECL destruct( gmMachine* a_machine, gmUserObject* a_object, gmuint32 a_mark );
   static void GM_CDECL set_dot( gmThread* a_thread, gmVariable* a_operands );
   static void GM_CDECL get_dot( gmThread* a_thread, gmVariable* a_operands );
public:
   static int reg_type( gmMachine* machine );
};

//////////////////////////////////////////////////////////////////////////

class FsmlScript::Library
{
private:
   static int GM_CDECL lib_tofloat( gmThread* );
   static int GM_CDECL lib_toint( gmThread* );
   static int GM_CDECL lib_tostring( gmThread* );
   static int GM_CDECL lib_tochar( gmThread* );
   static int GM_CDECL lib_totypename( gmThread* );
   static int GM_CDECL lib_join( gmThread* );
   static int GM_CDECL lib_map( gmThread* );
   static int GM_CDECL lib_slice( gmThread* );
   static int GM_CDECL lib_split( gmThread* );
   static int GM_CDECL lib_length( gmThread* );
   static int GM_CDECL lib_dbopen( gmThread* );
   static int GM_CDECL lib_dbclose( gmThread* );
   static int GM_CDECL lib_dbread( gmThread* );
   static int GM_CDECL lib_dbreadat( gmThread* );
   static int GM_CDECL lib_dbwrite( gmThread* );
   static int GM_CDECL lib_dbpurge( gmThread* );
   static int GM_CDECL lib_dbcount( gmThread* );
   static int GM_CDECL lib_dberase( gmThread* );
   static int GM_CDECL lib_dberasesection( gmThread* );
   static int GM_CDECL lib_elemroot( gmThread* );
   static int GM_CDECL lib_elemfind( gmThread* );
   static int GM_CDECL lib_elemrect( gmThread* );
   static int GM_CDECL lib_modcount( gmThread* );
   static int GM_CDECL lib_modinfo( gmThread* );
   static int GM_CDECL lib_modenable( gmThread* );
   static int GM_CDECL lib_modtouch( gmThread* );
   static int GM_CDECL lib_construct( gmThread* );
   static int GM_CDECL lib_setcursor( gmThread* );
   static int GM_CDECL lib_print( gmThread* );
   static int GM_CDECL lib_speak( gmThread* );
   static int GM_CDECL lib_errspeak( gmThread* );
   static int GM_CDECL lib_command( gmThread* );
   static int GM_CDECL lib_shiftstate( gmThread* );
public:
   static bool reg_library( gmMachine* machine );
   static string var2str( const gmVariable& var );
};

//////////////////////////////////////////////////////////////////////////

FsmlScript::FsmlScript( FsmlElemSkin* skin, const string& script_file, const string& db_file ) 
   : m_skin( skin ), m_gc_countdown( GC_RATE )
{
   m_db = new IniFile( db_file );
   m_machine.m_userParam = this;
   m_machine.SetDebugMode( true );
   m_usertypes[ USERTYPE_ELEMENT ] = UserT_Element::reg_type( &m_machine );
   Library::reg_library( &m_machine );

   std::ifstream fs( script_file.c_str( ) );

   if (!fs.fail( ))
   {
      string str_script = string( std::istreambuf_iterator<char>( fs ) , 
                                  std::istreambuf_iterator<char>( ) );

      fs.close( );

      int thread_id = GM_INVALID_THREAD;
      if (m_machine.ExecuteString( str_script.c_str( ), &thread_id ))
      {
         f2_errspeak( "The skin script couldn't be compiled." );
         speakErrors( );
      }
      else if (thread_id != GM_INVALID_THREAD && m_machine.GetThread( thread_id ))
      {
         f2_errspeak( "The skin script encountered an exception." );
         speakErrors( );
      }
      else // All good.
      {

      }
   }
}

FsmlScript::~FsmlScript( )
{
   for (FuncMap::iterator i = m_funcs.begin( );
        i != m_funcs.end( ); ++i)
   {
      free( (void*) (*i).first );
   }

   m_funcs.clear( );

   delete m_db;
}

void FsmlScript::tick( unsigned int dt )
{
   // Garbage collect.
   if (m_gc_countdown <= dt)
   {
      m_gc_countdown = GC_RATE;
      m_machine.CollectGarbage( );
   }
   else
   {
      m_gc_countdown -= dt;
   }
}

int FsmlScript::call( FsmlElem* from, const char* name, 
                      const char* fmt, std::va_list args )
{
   if (!name || !*name)
      return 0;

   /* We store the function object, not the variable holding the function object,
      so after a global function is called, all subsequent calls are bound to
      the function object, even if the variable is reassigned in the script. */
   gmFunctionObject* fn_obj = 0;

   {
      FuncMap::iterator f = m_funcs.find( name );

      if (f == m_funcs.end( ))
      {
         gmVariable fn_var =
            m_machine.GetGlobals( )->Get( 
               gmVariable( m_machine.AllocStringObject( name ) ) );

         if (GM_FUNCTION != fn_var.m_type || !fn_var.m_value.m_ref)
            return 0;

         fn_obj = (gmFunctionObject*) fn_var.m_value.m_ref;
         
         m_funcs.insert( FuncMap::value_type( strdup( name ), fn_obj ) );
         fn_obj->Mark( &m_machine, GM_MARK_PERSIST );
         // Not necessary for mark & sweep.
         // m_machine.m_gc->MakeObjectPersistant( fn_obj );
      }
      else
         fn_obj = f->second;
   }

   gmCall call;

   if (from)
   {
      gmVariable var_this( m_machine.AllocUserObject( 
         new UserT_Element( from ), m_usertypes[ USERTYPE_ELEMENT ] ) );
      call.BeginFunction( &m_machine, fn_obj, var_this );
   }
   else
      call.BeginFunction( &m_machine, fn_obj );

   /* Parse param list. */
   char c;
   do
   {
      switch (c = *(fmt++))
      {
      case 'i':
         call.AddParamInt( va_arg( args, int ) ); break;
      case 'l':
         call.AddParamInt( (int) va_arg( args, long ) ); break;
      case 's':
         call.AddParamString( va_arg( args, const char* ) ); break;
      case 'S':
         call.AddParamString( va_arg( args, const string* )->c_str( ) ); break;
      case 'f':
         call.AddParamFloat( va_arg( args, float ) ); break;
      case 'E':
         call.AddParamUser( va_arg( args, void* ), m_usertypes[ USERTYPE_ELEMENT ] ); break;
      case '2':
         {
            gmTableObject* table = m_machine.AllocTableObject( );
            FSML_POS* pos = va_arg( args, FSML_POS* );
            table->Set( &m_machine, "x", gmVariable( pos->x ) );
            table->Set( &m_machine, "y", gmVariable( pos->y ) );
            call.AddParamTable( table );
         }
         break;
      default:
         break;
      }
   } while (c);

   /* Make the call. */
   int thread_id = GM_INVALID_THREAD;
   call.End( &thread_id );

   if (thread_id != GM_INVALID_THREAD)
   {
      // An exception occurred.
      f2_errspeak( "The skin script encountered an exception." );
      speakErrors( );
      return 0;
   }

   int ret;
   if (call.GetReturnedInt( ret ))
      return ret;

   return 0;
}

void FsmlScript::speakErrors( )
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
            f2_errspeak( err.c_str( ) );
         }

         while (*c && (*c == '\r' || *c == '\n')) ++c;
         sz_err = c;
      }
   }
}

/************************************************************************/
/* UserT_Element                                                        */
/************************************************************************/

void GM_CDECL FsmlScript::
              UserT_Element::destruct( gmMachine* a_machine, gmUserObject* a_object, 
                                       gmuint32 a_mark )
{
   delete (UserT_Element*) a_object->m_user;
   a_object->m_user = 0;
}

void GM_CDECL FsmlScript::
              UserT_Element::set_dot( gmThread* a_thread, gmVariable* a_operands )
{
   UserT_Element* e = (UserT_Element*) 
      ((gmUserObject*) a_operands[ 0 ].m_value.m_ref)->m_user;
   const char* attr = 
      ((gmStringObject*) a_operands[ 2 ].m_value.m_ref)->GetString( );
   gmVariable rval = a_operands[ 1 ];

   if (e->ptr)
   {
      switch (rval.m_type)
      {
      case GM_STRING:
         {
            gmStringObject* str = (gmStringObject*) rval.m_value.m_ref;
            e->ptr->setAttr( FsmlAttr( attr, str->GetString( ) ) );
         }
         break;
      case GM_INT:
         e->ptr->setAttr( FsmlAttr( attr, rval.m_value.m_int ) );
         break;
      case GM_FLOAT:
         e->ptr->setAttr( FsmlAttr( attr, rval.m_value.m_float ) );
         break;
      case GM_NULL:
      default:
         e->ptr->setAttr( FsmlAttr( attr, "" ) );
         break;
      }
   }

   a_operands[ 0 ] = rval;
}
void GM_CDECL FsmlScript::
              UserT_Element::get_dot( gmThread* a_thread, gmVariable* a_operands )
{
   UserT_Element* e = (UserT_Element*) 
      ((gmUserObject*) a_operands[ 0 ].m_value.m_ref)->m_user;
   const char* attr = 
      ((gmStringObject*) a_operands[ 1 ].m_value.m_ref)->GetString( );

   if (e->ptr)
   {
      /* Always return a string. */
      const FsmlAttr* a = e->ptr->getAttr( attr );
      if (a)
      {
         gmStringObject* str_obj = 
            a_thread->GetMachine( )->AllocStringObject( a->getVal( ).c_str( ) );
         a_operands[ 0 ].SetString( str_obj );
         return;
      }
   }
   a_operands[ 0 ].Nullify( );
}

int FsmlScript::
    UserT_Element::reg_type( gmMachine* machine )
{
   int type = machine->CreateUserType( "Element" );

   if (type != GM_NULL)
   {
      machine->RegisterUserCallbacks( type, NULL, destruct );
      machine->RegisterTypeOperator( type, O_GETDOT, 0, get_dot );
      machine->RegisterTypeOperator( type, O_SETDOT, 0, set_dot );
   }
   return type;
}

/************************************************************************/
/* Library                                                              */
/************************************************************************/

bool FsmlScript::Library::reg_library( gmMachine* machine )
{
   static gmFunctionEntry lib_entries[] = 
   {
      { "toFloat", lib_tofloat },
      { "toInt", lib_toint },
      { "toString", lib_tostring },
      { "toChar", lib_tochar },
      { "toTypename", lib_totypename },
      { "join", lib_join },
      { "map", lib_map },
      { "slice", lib_slice },
      { "split", lib_split },
      { "length", lib_length },
      { "dbOpen", lib_dbopen },
      { "dbClose", lib_dbclose },
      { "dbRead", lib_dbread },
      { "dbWrite", lib_dbwrite },
      { "dbPurge", lib_dbpurge },
      { "dbErase", lib_dberase },
      { "dbReadAt", lib_dbreadat },
      { "dbCount", lib_dbcount },
      { "elemRoot", lib_elemroot },
      { "elemFind", lib_elemfind },
      { "elemRect", lib_elemrect },
      { "modCount", lib_modcount },
      { "modInfo", lib_modinfo },
      { "modEnable", lib_modenable },
      { "modTouch", lib_modtouch },
      { "command", lib_command },
      { "construct", lib_construct },
      { "print", lib_print },
      { "speak", lib_speak },
      { "errSpeak", lib_errspeak },
      { "shiftState", lib_shiftstate },
   };

   const int num_lib_entries = 
      sizeof( lib_entries ) / sizeof( gmFunctionEntry );
   machine->RegisterLibrary( lib_entries, num_lib_entries );
   return true;
}

string GM_CDECL FsmlScript::Library::var2str( const gmVariable& var )
{
   std::ostringstream ss;

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
         for (gmTableNode* node = table->GetFirst( iter ); node;
              node = table->GetNext( iter ))
         {
            if (iter == 1)
               ss << " ";
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
   default:
      ss << "USER@" << std::hex << (long) var.m_value.m_ref;
      break;
   }

   return ss.str( );
}

int GM_CDECL FsmlScript::Library::lib_tofloat( gmThread* a_thread )
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

int GM_CDECL FsmlScript::Library::lib_toint( gmThread* a_thread )
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
      ret = (int) atol( ((gmStringObject*) var.m_value.m_ref)->GetString( ) );
      break;
   default:
      break;
   }

   a_thread->PushInt( ret );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_tostring( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   a_thread->PushNewString( var2str( a_thread->Param( 0 ) ).c_str( ) );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_tochar( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( ch, 0 );

   char sz[] = { ch, '\0' };
   a_thread->PushNewString( sz, 2 );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_totypename( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );

   const char* tn = a_thread->GetMachine( )->GetTypeName( a_thread->Param( 0 ).m_type );
   a_thread->PushNewString( tn );
   
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_join( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_TABLE_PARAM( table, 0 );
   const char* separator = a_thread->ParamString( 1, " " );

   // Create a string with all the table elements separated by 'separator'.
   std::ostringstream ss;
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

int GM_CDECL FsmlScript::Library::lib_map( gmThread* a_thread )
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

int GM_CDECL FsmlScript::Library::lib_slice( gmThread* a_thread )
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

int GM_CDECL FsmlScript::Library::lib_split( gmThread* a_thread )
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

int GM_CDECL FsmlScript::Library::lib_length( gmThread* a_thread )
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

int GM_CDECL FsmlScript::Library::lib_dbopen( gmThread* a_thread )
{
   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

   script->m_db->open( true );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_dbclose( gmThread* a_thread )
{
   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

   script->m_db->close( );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_dbread( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( field, 0 );
   const char* section = a_thread->ParamString( 1, "" );

   if (*field)
   {
      FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

      const string& value = script->m_db->read( field, section );

      a_thread->PushNewString( value.c_str( ) );
   }
   else
      a_thread->PushNewString( "" );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_dbreadat( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( index, 0 );
   const char* section = a_thread->ParamString( 1, "" );
   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;
   
   string key;
   const string& value = script->m_db->read( (size_t) index, &key, section );

   if (!key.length( ))
      a_thread->PushNull( );
   else
   {
      gmTableObject* table = script->m_machine.AllocTableObject( );
      gmStringObject* str = script->m_machine.AllocStringObject( key.c_str( ) );
      table->Set( &script->m_machine, "key", gmVariable( str ) );
      str = script->m_machine.AllocStringObject( value.c_str( ) );
      table->Set( &script->m_machine, "value", gmVariable( str ) );
      a_thread->PushTable( table );
   }

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_dbwrite( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_STRING_PARAM( field, 0 );
   GM_CHECK_STRING_PARAM( value, 1 );
   const char* section = a_thread->ParamString( 2, "" );

   if (*field)
   {
      FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

      script->m_db->write( field, value, section );
   }

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_dbpurge( gmThread* a_thread )
{
   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

   script->m_db->purge( );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_dberase( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( field, 0 );
   const char* section = a_thread->ParamString( 1, "" );

   if (*field)
   {
      FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

      script->m_db->erase( field, section );
   }

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_dberasesection( gmThread* a_thread )
{
   const char* section = a_thread->ParamString( 0, "" );

   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;
   script->m_db->eraseSection( section );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_dbcount( gmThread* a_thread )
{
   const char* section = a_thread->ParamString( 0, "" );

   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;
   a_thread->PushInt( script->m_db->count( section ) );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_elemroot( gmThread* a_thread )
{
   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;
   if (script->m_skin)
   {
      a_thread->PushNewUser( new UserT_Element( script->m_skin ),
                             script->m_usertypes[ USERTYPE_ELEMENT ] );
   }
   else
      a_thread->PushNull( );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_elemfind( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( id, 0 );

   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

   int param_type = 0;
   UserT_Element* root = (UserT_Element*) a_thread->ParamUser( 1, &param_type );
   FsmlElem* f = NULL;
   if (root && param_type == script->m_usertypes[ USERTYPE_ELEMENT ])
      f = root->ptr->find( id );
   else
      f = script->m_skin->find( id );

   if (f)
      a_thread->PushNewUser( new UserT_Element( f ), 
                             script->m_usertypes[ USERTYPE_ELEMENT ] );
   else
      a_thread->PushNull( );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_elemrect( gmThread* a_thread )
{
   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_USER_PARAM( UserT_Element*, script->m_usertypes[ USERTYPE_ELEMENT ], elem, 0 );

   FSML_RECT rect;
   elem->ptr->getRect( &rect );

   /* Return as a table. */
   gmTableObject* table_obj = a_thread->PushNewTable( );

   table_obj->Set( &script->m_machine, "x", gmVariable( (int) rect.x ) );
   table_obj->Set( &script->m_machine, "y", gmVariable( (int) rect.y ) );
   table_obj->Set( &script->m_machine, "w", gmVariable( (int) rect.w ) );
   table_obj->Set( &script->m_machine, "h", gmVariable( (int) rect.h ) );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_modcount( gmThread* a_thread )
{
   FsmlScript* script = (FsmlScript*) a_thread->GetMachine( )->m_userParam;
   a_thread->PushInt( f2_modcount( ) );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_modinfo( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( mod, 0 );
   
   if ((unsigned) mod < f2_modcount( ))
   {
      /* Return as a table. */
      gmMachine* machine = a_thread->GetMachine( );
      gmTableObject* table_obj = a_thread->PushNewTable( );
      
      const char* sz = f2_modfancyname( mod );

      if (sz)
         table_obj->Set( machine, "name", 
                         gmVariable( machine->AllocStringObject( sz ) ) );

      sz = f2_modroot( mod );

      if (sz)
         table_obj->Set( machine, "root", 
                         gmVariable( machine->AllocStringObject( sz ) ) );
      
      sz = f2_modthumb( mod );

      if (sz)
         table_obj->Set( machine, "thumb", 
                         gmVariable( machine->AllocStringObject( sz ) ) );
      
      table_obj->Set( machine, "state", 
                      gmVariable( (int) f2_modstate( mod ) ) );

      return GM_OK;
   }

   a_thread->PushNull( );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_modenable( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 2 );
   GM_CHECK_INT_PARAM( mod, 0 );
   GM_CHECK_INT_PARAM( onoff, 1 );

   f2_modenable( mod, onoff ? true : false );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_modtouch( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_INT_PARAM( mod, 0 );
   f2_modtouch( mod );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_construct( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( id, 0 );
   FsmlScript* script   = (FsmlScript*) a_thread->GetMachine( )->m_userParam;

   script->m_constructed.clear( );
   if (script->m_skin->construct( id ))
   {
      /* Return the constructed elements as a table. */
      gmMachine* machine = a_thread->GetMachine( );
      gmTableObject* table = a_thread->PushNewTable( );

      for (size_t i = 0; i < script->m_constructed.size( ); ++i)
      {
         gmUserObject* obj = 
            machine->AllocUserObject( new UserT_Element( script->m_constructed[ i ] ), 
                                      script->m_usertypes[ USERTYPE_ELEMENT ] );
         table->Set( machine, (int) i, gmVariable( obj ) );
      }
      script->m_constructed.clear( );

      return GM_OK;
   }

   a_thread->PushNull( );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_setcursor( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( cursor, 0 );

   if (!strcmp( cursor, "HAND" ))
      SetCursor( LoadCursor( 0, IDC_HAND ) );
   else if (!strcmp( cursor, "HELP" ))
      SetCursor( LoadCursor( 0, IDC_HELP ) );
   else if (!strcmp( cursor, "SIZENS" ))
      SetCursor( LoadCursor( 0, IDC_SIZENS ) );
   else if (!strcmp( cursor, "SIZEWE" ))
      SetCursor( LoadCursor( 0, IDC_SIZEWE ) );
   else
      SetCursor( LoadCursor( 0, IDC_ARROW ) );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_speak( gmThread* a_thread )
{
   std::ostringstream ss;
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
      ss << var2str( a_thread->Param( i ) );

   f2_speak( ss.str( ).c_str( ) );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_errspeak( gmThread* a_thread )
{
   std::ostringstream ss;
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
      ss << var2str( a_thread->Param( i ) );

   f2_errspeak( ss.str( ).c_str( ) );
   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_command( gmThread* a_thread )
{
   GM_CHECK_NUM_PARAMS( 1 );
   GM_CHECK_STRING_PARAM( cmd, 0 );

   f2_command( cmd );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_shiftstate( gmThread* a_thread )
{
   if (GetKeyState( VK_SHIFT ) & 0x80000000)
      a_thread->PushInt( 1 );
   else
      a_thread->PushInt( 0 );

   return GM_OK;
}

int GM_CDECL FsmlScript::Library::lib_print( gmThread* a_thread )
{
   for (int i = 0; i < a_thread->GetNumParams( ); i++)
      std::cout << var2str( a_thread->Param( i ) );

   return GM_OK;
}