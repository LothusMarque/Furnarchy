#include "ifilters.h"
#include "core.h"
#include "world.h"
#include "common/RegexOp.h"
#include <algorithm>

namespace ifilters {

enum { OPT_TRAP = 0x1, OPT_CHAT = 0x2, OPT_ALL = 0x4, OPT_QUIET = 0x8, OPT_HIDE = 0x10 };

struct FILTER
{
   string   str_filter;
   long     options;
   long     hide_flags;
   RegexOp* reop;

   FILTER( ) : options( 0 ), hide_flags( 0 ), reop( NULL ) { }
   ~FILTER( ) { delete reop; }
};

//////////////////////////////////////////////////////////////////////////

bool          g_initialized = false;
string        g_use;
bool          g_enabled     = true;
list<FILTER*> g_filters;

 bool cmd_ifadd( const ArgList& arg_list );
 bool cmd_ifrm( const ArgList& arg_list );
 void cmd_ifls( const ArgList& arg_list );
 void cmd_ifon( const ArgList& arg_list );
 void cmd_ifoff( const ArgList& arg_list );
 bool cmd_ifmv( const ArgList& arg_list );
 void cmd_ifuse( const ArgList& arg_list );
 bool cmd_ifclr( const ArgList& arg_list );
 bool load_filters( );
 bool write_filters( );
 void clear_filters( );

bool initialize( )
{
   if (!g_initialized)
   {
      g_initialized = true;
      g_use         = "in";
      g_enabled     = true;
      load_filters( );
   }

   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {
      g_initialized = false;
      clear_filters( );
   }

   return true;
}

void clear_filters( )
{
   for (list<FILTER*>::iterator i = g_filters.begin( );
        i != g_filters.end( ); ++i)
      delete *i;

   g_filters.clear( );
}

bool cmd_ifadd( const ArgList& arg_list )
{
   size_t i_arg = 1;
   string arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";

   /* Collect options. */
   long opts = 0;
   long hide_flags = 0;
   while (!arg.compare( 0, 2, "--" ))
   {
      to_lower( &arg );

      if (!arg.compare( 2, arg.npos, "chat" ))
         opts |= OPT_CHAT;
      else if (!arg.compare( 2, arg.npos, "trap" ))
         opts |= OPT_TRAP;
      else if (!arg.compare( 2, arg.npos, "hide" ))
      {
         opts |= OPT_HIDE;
         arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";

         if (arg.empty( ))
         {
            core::system_errspeak( "Error: Expected hide flags string." );
            return false;
         }

         if (arg.find_first_not_of( "acfmsw" ) != arg.npos )
         {
            core::system_errspeak( "Error: Valid flags for --hide option must be one of the letters: 'acfmsw'." );
            return false;
         }

         if (arg.find_first_of( "a" ) != arg.npos)
            hide_flags = HIDE_ALL;
         else
         {
            if (arg.find_first_of( "c" ) != arg.npos)
               hide_flags |= HIDE_CLIENT;
            if (arg.find_first_of( "f" ) != arg.npos)
               hide_flags |= HIDE_FILTERS;
            if (arg.find_first_of( "m" ) != arg.npos)
               hide_flags |= HIDE_MODULES;
            if (arg.find_first_of( "s" ) != arg.npos)
               hide_flags |= HIDE_SCRIPTS;
            if (arg.find_first_of( "w" ) != arg.npos)
               hide_flags |= HIDE_WORLD;
         }
      }
      else
      {
         core::system_errspeak( "Error: Invalid option." );
         return false;
      }

      arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";
   }

   /* Collect filter definition. */
   if (!arg.length( ))
   {
      core::system_errspeak( "Error: Expected filter definition." );
      return false;
   }

   if (arg_list.size( ) > i_arg)
   {
      core::system_errspeak( "Error: Too many arguments." );
      return false;
   }

   const string* p_err;
   RegexOp* reop = RegexOp::from_str( arg, &p_err );

   if (!reop)
   {
      core::system_errspeak( *p_err );
      return false;
   }

   if (!reop->ok( ))
   {
      core::system_errspeak( reop->getLastError( ) );
      delete reop;
      return false;
   }

   FILTER* filter     = new FILTER;
   filter->options    = opts;
   filter->hide_flags = hide_flags;
   filter->str_filter = arg;
   filter->reop       = reop;

   g_filters.push_back( filter );

   ostringstream ss;
   ss << "Filter ( " << g_filters.size( ) << " ) added.";
   core::system_speak( ss.str( ) );

   return true;
}

bool cmd_ifrm( const ArgList& arg_list )
{   
   size_t i_arg = 1;
   string arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";

   /* Collect indices. */
   vector<size_t> indices;
   while (arg.length( ))
   {
      size_t i = 0;
      if (!to_size_t( arg, &i ))
      {
         core::system_errspeak( "Error: Expected an index number." );
         return false;
      }
      if (!i || i > g_filters.size( ))
      {
         core::system_errspeak( "Error: Index out of bounds." );
         return false;
      }
      indices.push_back( --i );

      arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";
   }

   size_t num_removed = 0;
   size_t pos = 0;
   for (list<FILTER*>::iterator i = g_filters.begin( );
        i != g_filters.end( ); ++pos)
   {
      if (find( indices.begin( ), indices.end( ), pos ) != indices.end( ))
      {
         delete (*i);
         g_filters.erase( i++ );
         ++num_removed;
      }
      else
         ++i;
   }

   if (!num_removed)
   {
      core::system_speak( "Nothing was removed." );
      return false;
   }

   ostringstream ss;
   ss << "Removed " << num_removed << " filters.";
   core::system_speak( ss.str( ) );

   return true;
}

void cmd_ifon( const ArgList& arg_list )
{
   if (arg_list.size( ) > 1)
   {
      core::system_errspeak( "Error: Too many arguments." );
      return;
   }

   g_enabled = true;
   core::system_speak( "ifilters enabled." );
}

void cmd_ifoff( const ArgList& arg_list )
{
   if (arg_list.size( ) > 1)
   {
      core::system_errspeak( "Error: Too many arguments." );
      return;
   }

   g_enabled = false;
   core::system_speak( "ifilters disabled." );
}

void cmd_ifls( const ArgList& arg_list )
{
   if (arg_list.size( ) > 1)
   {
      core::system_errspeak( "Error: Too many arguments." );
      return;
   }

   if (!g_filters.size( ))
   {
      core::system_speak( "No filters in " + g_use );
      return;
   }
   else
   {
      core::system_speak( "Listing \"" + g_use + "\" filters." );
      
      ostringstream ss;
      size_t pos = 1;
      for (list<FILTER*>::const_iterator i = g_filters.begin( );
           i != g_filters.end( ); ++i, ++pos)
      {
         ss.str( "" );
         ss << pos << ".  ";

         if ((*i)->options & OPT_CHAT)
            ss << "--chat ";

         if ((*i)->options & OPT_TRAP)
            ss << "--trap ";

         if ((*i)->options & OPT_HIDE)
         {
            ss << "--hide '";
            if (((*i)->hide_flags & HIDE_ALL) == HIDE_ALL)
               ss << 'a';
            else
            {
               if ((*i)->hide_flags & HIDE_FILTERS)
                  ss << 'f';
               if ((*i)->hide_flags & HIDE_SCRIPTS)
                  ss << 's';
               if ((*i)->hide_flags & HIDE_MODULES)
                  ss << 'm';
               if ((*i)->hide_flags & HIDE_WORLD)
                  ss << 'w';
               if ((*i)->hide_flags & HIDE_CLIENT)
                  ss << 'c';
            }
            ss << "' ";
         }

         ss << break_html( break_ent_refs( (*i)->str_filter ) );

         core::system_speak( ss.str( ) );
      }
   }
}

bool cmd_ifmv( const ArgList& arg_list )
{
   size_t i_arg = 1;
   string arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";

   /* Collect index pairs. */
   vector< std::pair<size_t,size_t> > pairs;
   while (arg.length( ))
   {
      std::pair<size_t,size_t> i;
      if (!to_size_t_pair( arg, &i ))
      {
         core::system_errspeak( "Error: Expected an \"a-b\" form index-pair." );
         return false;
      }
      if (!i.first || !i.second || 
         i.first > g_filters.size( ) || i.second > g_filters.size( ))
      {
         core::system_errspeak( "Error: Index out of bounds." );
         return false;
      }

      --i.first; --i.second;
      pairs.push_back( i );

      arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";
   }

   if (!pairs.size( ))
   {
      core::system_speak( "Nothing to move." );
      return false;
   }

   for (size_t i = 0; i < pairs.size( ); i++)
   {
      if (pairs[ i ].first != pairs[ i ].second)
      {
         size_t from = pairs[ i ].first;
         size_t to = pairs[ i ].second;

         list<FILTER*>::iterator pos1 = g_filters.begin( );
         for (size_t j = 0; j != from; ++j, ++pos1) ;
         list<FILTER*>::iterator pos2 = g_filters.begin( );
         for (size_t j = 0; j != to; ++j, ++pos2) ;

         FILTER* f = *pos1;
         g_filters.erase( pos1 );
         g_filters.insert( pos2, f );
      }
   }

   ostringstream ss;
   ss << "Moved " << pairs.size( ) << " filter(s).";
   core::system_speak( ss.str( ) );

   return true;
}

void cmd_ifuse( const ArgList& arg_list )
{
   size_t i_arg = 1;
   string arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";

   /* Collect options. */
   long opts = 0;
   while (!arg.compare( 0, 2, "--" ))
   {
      to_lower( &arg );

      if (!arg.compare( 2, arg.npos, "quiet" ))
         opts |= OPT_QUIET;
      else
      {
         core::system_errspeak( "Error: Invalid option." );
         return;
      }

      arg = (arg_list.size( ) > i_arg++) ? arg_list[ i_arg - 1 ] : "";
   }

   if (!arg.length( ))
   {
      core::system_errspeak( "Error: Expected a filters name." );
      return;
   }

   if (arg_list.size( ) > i_arg)
   {
      core::system_errspeak( "Error: Too many arguments." );
      return;
   }

   if (opts & OPT_QUIET)
      core::silence( true );

   clear_filters( );
   g_use = arg;
   load_filters( );

   if (opts & OPT_QUIET)
      core::silence( false );

   core::system_speak( "Using \"" + g_use + "\" ifilters." );
}

bool cmd_ifclr( const ArgList& arg_list )
{
   if (arg_list.size( ) > 1)
   {
      core::system_errspeak( "Error: Too many arguments." );
      return false;
   }

   clear_filters( );

   core::system_speak( "Removed all filters from \"" + g_use + "\"." );

   return true;
}


bool on_command( const ArgList& arg_list )
{
   if (!arg_list.size( ))
      return false;

   string arg = arg_list[ 0 ];
   to_lower( &arg );

   if (arg == "ifadd")
   {
      if (cmd_ifadd( arg_list ))
         write_filters( );
      return true;
   }
   if (arg == "ifrm")
   {
      if (cmd_ifrm( arg_list ))
         write_filters( );
      return true;
   }
   if (arg == "ifmv")
   {
      if (cmd_ifmv( arg_list ))
         write_filters( );
      return true;
   }
   if (arg == "ifls")
   {
      cmd_ifls( arg_list );
      return true;
   }
   if (arg == "ifon")
   {
      cmd_ifon( arg_list );
      return true;
   }
   if (arg == "ifoff")
   {
      cmd_ifoff( arg_list );
      return true;
   }
   if (arg == "ifuse")
   {
      cmd_ifuse( arg_list );
      return true;
   }
   if (arg == "ifclr")
   {
      if (cmd_ifclr( arg_list ))
         write_filters( );
      return true;
   }

   return false;
}

long on_inbound_line( string* line, int from, size_t num )
{
   static string tmp;

   // Gotta accumulate all the hide flags from each filter.
   long hide_flags = 0;

   if (!g_enabled || !line->length( ) || core::get_connlvl( ) < CONNLVL_LOGGEDIN)
      return hide_flags;

   const bool was_chat = (*line)[ 0 ] == '(';
   bool match = false;

   for (list<FILTER*>::const_iterator i = g_filters.begin( );
        !(hide_flags & HIDE_FILTERS) && i != g_filters.end( ); ++i)
   {
      match = false;

      if ((*i)->options & OPT_CHAT)
      {
         if (line->c_str( )[ 0 ] == '(')
         {
            tmp = "(";
            match = (*i)->reop->execute( line->c_str( ) + 1, line->length( ) - 1, &tmp );
         }
      }
      else
      {
         tmp.clear( );
         match = (*i)->reop->execute( line->c_str( ), line->length( ), &tmp );
      }

      if (match)
      {
         if ((*i)->reop->getType( ) == RegexOp::TYPE_SUBSTITUTE)
            line->assign( tmp );
         if ((*i)->options & OPT_TRAP)
            break;
         if ((*i)->options & OPT_HIDE)
            hide_flags |= (*i)->hide_flags;
      }
   }

   // cancel badges for chat messages.
   if (match && was_chat && (*line)[0] != '(')
      world::badge_cancel( );

   return hide_flags;
}

bool write_filters( )
{
   // Use the per-user location file always.
   string file = core::get_furn_doc_root( ) + "\\filters\\" + g_use + ".txt";

   ofstream fs( file.c_str( ) );

   if (!fs.fail( ))
   {
      string t;
      for (list<FILTER*>::const_iterator i = g_filters.begin( ); 
           i != g_filters.end( ); ++i)
      {
         if ((*i)->options & OPT_CHAT)
            fs << "--chat ";

         if ((*i)->options & OPT_TRAP)
            fs << "--trap ";

         if ((*i)->options & OPT_HIDE)
         {
            fs << "--hide '";
            if (((*i)->hide_flags & HIDE_ALL) == HIDE_ALL)
               fs << 'a';
            else
            {
               if ((*i)->hide_flags & HIDE_FILTERS)
                  fs << 'f';
               if ((*i)->hide_flags & HIDE_SCRIPTS)
                  fs << 's';
               if ((*i)->hide_flags & HIDE_MODULES)
                  fs << 'm';
               if ((*i)->hide_flags & HIDE_WORLD)
                  fs << 'w';
               if ((*i)->hide_flags & HIDE_CLIENT)
                  fs << 'c';
            }
            fs << "' ";
         }

         fs << escape_char( &(t = (*i)->str_filter), ' ' ) << '\n';
      }

      fs.close( );
      return true;
   }
   else
   {
      core::system_errspeak( "Couldn't write filters to file " + file );
      return false;
   }

   return true;
}

bool load_filters( )
{
   // Use the per-user location file if it exists.
   string file = core::get_furn_doc_root( ) + "\\filters\\" + g_use + ".txt";
   if (GetFileAttributes( file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
      // Fall back to program file.
      file = core::get_furn_root( ) + "\\filters\\" + g_use + ".txt";

   ifstream fs( file.c_str( ) );

   if (!fs.fail( ))
   {
      string line;
      ArgList arg_list;

      while (!fs.eof( ) && !fs.fail( ))
      {
         getline( fs, line );
         make_arg_list( "ifadd " + line, &arg_list );
         if (arg_list.size( ) > 1)
            cmd_ifadd( arg_list );
      }

      fs.close( );
   }
   else
   {
      core::system_errspeak( "Couldn't read filters file " + file );
      return false;
   }

   return true;
}

} // namespace ifilters