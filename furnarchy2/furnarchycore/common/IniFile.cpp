#include "IniFile.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

IniFile::IniFile( const string& file )
{
   m_file = file;
   m_open = false;
   m_dirty = false;
   m_first_line    = 0;
   m_last_field    = 0;
   m_last_section  = 0;
}

IniFile::~IniFile( )
{
   reset( );
}

bool IniFile::open( bool create )
{
   if (isOpen( ))
      return true;

   ifstream f( m_file.c_str( ) );

   if (f.fail( ))
   {
      if (create)
      {
         m_open = true;
         m_dirty = true;
         return true;
      }
      else
      {
         cerr << "IniFile: Couldn't open file " << m_file
              << " for reading!\n";
      }
      return false;
   }

   string str_line;
   LINE* curr_line = 0;
   
   while (!f.fail( ) && !f.eof( ))
   {
      str_line.clear( );
      getline( f, str_line );

      LINE* line = parse_line( str_line.c_str( ) );

      if (line->type == INI_SECTION)
      {
         // Sections are stored backwards.
         ((LINE_SECTION*) line)->prev_section = m_last_section;
         m_last_section = (LINE_SECTION*) line;
      }
      else if (line->type == INI_FIELD)
      {
         // Fields are stored backwards.
         if (m_last_section)
         {
            // Push to top.
            ((LINE_FIELD*) line)->prev_field = m_last_section->last_field;
            m_last_section->last_field = (LINE_FIELD*) line;
         }
         else  // Global.
         {
            // Push to top.
            ((LINE_FIELD*) line)->prev_field = m_last_field;
            m_last_field = (LINE_FIELD*) line;
         }
      }

      if (curr_line)
         curr_line->next_line = line;
      else // First line.
         m_first_line = line;
      curr_line = line;
   }

   f.close( );

   m_open = true;
   m_dirty = false;
   return true;
}

bool IniFile::close( bool nowrite )
{
   if (!isOpen( ))
      return true;

   if (nowrite)
   {
      reset( );
      return true;
   }

   if (m_dirty) // Dirty, so need to sync to the file.
   {
      ofstream f( m_file.c_str( ) );

      if (f.fail( ))
      {
         cerr << "IniFile: Couldn't open file " << m_file
              << " for writing!\n";
         reset( );
         return false;
      }

      f << toStr( );

      f.close( );
   }

   reset( );
   return true;
}

void IniFile::reset( )
{
   for (LINE *line = m_first_line, *next = 0; line; )
   {
      next = line->next_line;
      delete line;
      line = next;
   }
   
   m_dirty = false;
   m_open = false;
   m_first_line    = 0;
   m_last_field    = 0;
   m_last_section = 0;
}

const string& IniFile::read( const string& name, const string& section ) const
{
   if (!isOpen( ))
      return m_emptyval;

   if (!section.length( )) // Global?
   {
      for (const LINE_FIELD* j = m_last_field; j; j = j->prev_field)
      {
         if (j->name == name)
            return j->value;
      }
   }
   else
   {
      for (const LINE_SECTION* i = m_last_section; i; i = i->prev_section)
      {
         if (i->name == section)
         {
            for (const LINE_FIELD* j = i->last_field; j; j = j->prev_field)
            {
               if (j->name == name)
                  return j->value;
            }
         }
      }
   }

   return m_emptyval;
}

const string& IniFile::read( size_t index, std::string* name_out , 
                             const std::string& section ) const
{
   if (!isOpen( ))
      return m_emptyval;

   if (!section.length( )) // Global?
   {
      for (const LINE_FIELD* j = m_last_field; j; j = j->prev_field)
      {
         if (index == 0)
         {
            if (name_out) name_out->assign( j->name );
            return j->value;
         }
         --index;
      }

      if (name_out) name_out->clear( );
      return m_emptyval;
   }

   for (const LINE_SECTION* i = m_last_section; i; i = i->prev_section)
   {
      if (i->name == section)
      {
         for (const LINE_FIELD* j = i->last_field; j; j = j->prev_field)
         {
            if (index == 0)
            {
               if (name_out) name_out->assign( j->name );
               return j->value;
            }
            --index;
         }
      }
   }

   if (name_out) name_out->clear( );
   return m_emptyval;
}

const string& IniFile::readSectionName(size_t index) const
{
	 if (!isOpen( ))
      return m_emptyval;

   for (const LINE_SECTION* i = m_last_section; i; i = i->prev_section)
   {
      if (index == 0)
      {
		  return i->name;
      }
	  --index;
   }

   return m_emptyval;
}

size_t IniFile::count( const std::string& section ) const
{
   if (!isOpen( ))
      return 0;

   size_t count = 0;
   if (!section.length( )) // Global?
   {
      for (const LINE_FIELD* j = m_last_field; j; j = j->prev_field)
         ++count;
   }
   else
   {
      for (const LINE_SECTION* i = m_last_section; i; i = i->prev_section)
      {
         if (i->name == section)
         {
            for (const LINE_FIELD* j = i->last_field; j; j = j->prev_field)
               ++count;
         }
      }
   }

   return count;
}

size_t IniFile::countSections( ) const
{
   if (!isOpen( ))
      return 0;

   size_t count = 0;
      for (const LINE_SECTION* i = m_last_section; i; i = i->prev_section)
      {
			 ++count;
      }

   return count;
}

string IniFile::toStr( ) const
{
   std::ostringstream s;
   LINE* i = m_first_line;
   if (i)
   {
      s << i->line;

      for (i = i->next_line; i; i = i->next_line)
         s << '\n' << i->line;
   }
   return s.str( );
}

void IniFile::write( const string& name, const string& value, 
                     const string& section )
{
   if (!isOpen( ))
      return;

   m_dirty = true;

   if (!section.length( )) // Global?
   {
      for (LINE_FIELD* j = m_last_field; j; j = j->prev_field)
      {
         if (j->name == name)
         {
            // Update it in place.
            j->value = value;
            j->line = j->name + "=" + j->value;
            return;
         }
      }
   }
   else
   {
      // See if it's in the ini already.
      for (LINE_SECTION* i = m_last_section; i; i = i->prev_section)
      {
         if (i->name == section)
         {
            for (LINE_FIELD* j = i->last_field; j; j = j->prev_field)
            {
               if (j->name == name)
               {
                  // Update it in place.
                  j->value = value;
                  j->line = j->name + "=" + j->value;
                  return;
               }      
            }
         }
      }
   }
   
   // Create a new field.
   LINE_FIELD* new_field = new LINE_FIELD;
   new_field->name  = name;
   new_field->value = value;
   new_field->line  = name + "=" + value;
   
   // Look for the section to add it in.
   if (!section.length( )) // Global?
   {
      if (m_last_field)
      {
         new_field->next_line = m_last_field->next_line;
         m_last_field->next_line = new_field;
      }
      else // Totally empty INI or no global fields.
      {
         // Just stick at the top of the INI.
         new_field->next_line = m_first_line;
         m_first_line = new_field;
      }
      
      new_field->prev_field = m_last_field;
      m_last_field = new_field;
      return;
   }

   for (LINE_SECTION* i = m_last_section; i; i = i->prev_section)
   {
      if (i->name == section)
      {
         if (i->last_field)
         {
            new_field->next_line = i->last_field->next_line;
            i->last_field->next_line = new_field;
         }
         else  // Empty section.
         {
            // Just stick it at the top of the section.
            new_field->next_line = i->next_line;
            i->next_line = new_field;
         }
         
         new_field->prev_field = i->last_field;
         i->last_field = new_field;
         return;
      }
   }

   // Need to create both the section and field.
   LINE_SECTION* new_section = new LINE_SECTION;
   new_section->name = section;
   new_section->line = "[" + section + "]";
   new_section->last_field = new_field;
   new_section->next_line = new_field;

   if (!m_first_line) // Empty INI
   {
      m_first_line = new_section;
      m_last_section = new_section;
   }
   else
   {
      // Stick at the end of the INI.

      LINE* last_line = m_first_line;
      for (; last_line->next_line; last_line = last_line->next_line) ;

      last_line->next_line = new_section;
      new_section->prev_section = m_last_section;
      m_last_section = new_section;
   }
}

void IniFile::erase( const std::string& name, const std::string& section )
{
   if (!isOpen( ))
      return;

   if (!section.length( )) // Global?
   {
      LINE_FIELD *j = m_last_field, *j_next = 0;
      while (j)
      {
         if (j->name == name)
         {
            m_dirty = true;

            if (!j_next)   // Last field.
               m_last_field = j->prev_field;
            else
               j_next->prev_field = j->prev_field;

            // Get the line before this.
            LINE* prev_line = m_first_line;
            for (; prev_line && prev_line->next_line != j; 
                 prev_line = prev_line->next_line) ;
            
            if (!prev_line) // First line.
               m_first_line = j->next_line;
            else
               prev_line->next_line = j->next_line;
            
            LINE* line = j;
            j = j->prev_field;
            delete line;
         }
         else
         {
            j_next = j;
            j = j->prev_field;
         }
      }
   }
   else
   {
      for (LINE_SECTION* i = m_last_section; i; i = i->prev_section)
      {
         if (i->name == section)
         {
            LINE_FIELD *j = i->last_field, *j_next = 0;
            while (j)
            {
               if (j->name == name)
               {
                  m_dirty = true;

                  if (!j_next)   // Last field.
                     i->last_field = j->prev_field;
                  else
                     j_next->prev_field = j->prev_field;

                  // Get the line before this.
                  LINE* prev_line = i;
                  for (; prev_line && prev_line->next_line != j; 
                       prev_line = prev_line->next_line) ;

                  if (!prev_line) // First line in the section.
                     i->next_line = j->next_line;
                  else
                     prev_line->next_line = j->next_line;

                  LINE* line = j;
                  j = j->prev_field;
                  delete line;
               }
               else
               {
                  j_next = j;
                  j = j->prev_field;
               }
            }
         }
      }
   }
}

void IniFile::eraseSection( const std::string& section )
{
   if (!isOpen( ))
      return;

   if (!section.length( )) // Global?
   {
      LINE_FIELD* first_field = m_last_field;
      for (; first_field && first_field->prev_field; 
           first_field = first_field->prev_field) ;

      if (!first_field) // Nothing to do.
         return;

      // Get the line before the first field.
      LINE* prev_line = m_first_line;
      for (; prev_line && prev_line->next_line != first_field; 
           prev_line = prev_line->next_line) ;

      if (!prev_line) // First line.
         m_first_line = m_last_field->next_line;
      else
         prev_line->next_line = m_last_field->next_line;

      const LINE* end = m_last_field->next_line;
      LINE* line = first_field;
      while (line != end)
      {
         LINE* next = line->next_line;
         delete line;
         line = next;
      }

      m_last_field = 0;

      m_dirty = true;
   }
   else
   {

      LINE_SECTION *i = m_last_section, *i_next = 0;
      while (i)
      {
         if (i->name == section)
         {
            m_dirty = true;

            // Get the line before the section.
            LINE* prev_line = m_first_line;
            for (; prev_line && prev_line->next_line != i; 
                 prev_line = prev_line->next_line) ;

            LINE* section_end = i->last_field ? i->last_field->next_line : i->next_line;

            if (!prev_line) // First line.
               m_first_line = section_end;
            else
               prev_line->next_line = section_end;

            if (!i_next) // last section.
               m_last_section = i->prev_section;
            else
               i_next->prev_section = i->prev_section;

            LINE* line = i;
            i = i->prev_section;

            while (line != section_end)
            {
               LINE* next = line->next_line;
               delete line;
               line = next;
            }
         }
         else
         {
            i_next = i;
            i = i->prev_section;
         }
      }
   }
}

void IniFile::purge( )
{
   reset( );
   m_open = true;  // keep or make open.
   m_dirty = true; // mark file dirty.
}

IniFile::LINE* IniFile::parse_line( const char* sz_line )
{
   const char* p = sz_line;

   for (; *p; ++p)
   {
      if ((unsigned) *p > (unsigned) ' ')
         break;
   }

   if (*p == '[') // Section?
   {
      std::size_t n = 0;
      p = section_name( sz_line, &n );

      if (p) // Allow [] sections.
      {
         LINE_SECTION* section = new LINE_SECTION;
         section->line = sz_line;
         section->name.assign( p, n );
         return section;
      }
   }
   else if (*p == '#' || *p == ';') // Comment.
   {
      LINE_COMMENT* comment = new LINE_COMMENT;
      comment->line = sz_line;
      return comment;
   }
   else // Field?
   {
      std::size_t n = 0;
      p = field_name( sz_line, &n );

      if (p && n)
      {
         LINE_FIELD* field = new LINE_FIELD;
         field->line = sz_line;
         field->name.assign( p, n );

         n = 0;
         p = field_value( sz_line, &n );
         if (p)
            field->value.assign( p, n );

         return field;
      }
   }

   LINE* unk = new LINE;
   unk->line = sz_line;

   return unk;
}

const char* IniFile::section_name( const char* sz_line, std::size_t* len)
{
   *len = 0;

   const char* p = sz_line;

   for (; *p; ++p)
   {
      if (*p == '[')
         break;
   }

   if (!*p)
      return 0;

   const char* p2 = p + 1;

   for (; *p2; ++p2)
   {
      if (*p2 == ']')
         break;
   }

   if (!*p2)
      return 0;

   --p2;
   for (; (unsigned) *p2 <= (unsigned) ' '; --p2);

   if (p == p2)
      return p;

   ++p;
   for (; *p && (unsigned) *p <= (unsigned) ' '; ++p);

   ++p2;
   *len = (std::size_t) p2 - (std::size_t) p;

   return p;
}

const char* IniFile::field_name( const char* sz_line, std::size_t* len )
{
   *len = 0;

   const char* p = sz_line;

   for (; *p; ++p)
   {
      if ((unsigned) *p > (unsigned) ' ')
         break;
   }

   if (!*p)
      return 0;

   const char* p2 = p;

   for (; *p2; ++p2)
   {
      if (*p2 == '=')
         break;
   }

   if (!*p2)
      return 0;

   --p2;
   for (; (unsigned) *p2 <= (unsigned) ' '; --p2);
   ++p2;

   *len = (std::size_t) p2 - (std::size_t) p;
   return p;
}

const char* IniFile::field_value( const char* sz_line, std::size_t* len )
{
   *len = 0;

   const char* p = sz_line;

   for (; *p; ++p)
   {
      if (*p == '=')
         break;
   }

   if (!*p)
      return 0;

   ++p;
   for (; *p  && (unsigned) *p <= (unsigned) ' '; ++p);

   if (!*p)
      return 0;

   const char* p2 = p;

   for (; *p2; ++p2);

   for (; (unsigned) *p2 <= (unsigned) ' '; --p2);

   ++p2;
   *len = (std::size_t) p2 - (std::size_t) p;
   return p;
}