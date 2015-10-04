#ifndef _INI_FILE_H__
#define _INI_FILE_H__

#include <string>
#include <list>

/** Simple INI file reader/writer. */
class IniFile
{
   enum Ini_Line_Type
   {
      INI_LINE,
      INI_FIELD,
      INI_SECTION,
      INI_COMMENT,
   };
   
   struct LINE;
   struct LINE_COMMENT;
   struct LINE_FIELD;
   struct LINE_SECTION;

public:
   class field_iterator;
   class unk_iterator;
   class section_iterator;

public:
   IniFile( const std::string& file );
   ~IniFile( );
private:
   IniFile( const IniFile& r ) { };

public:
   bool open( bool create = false );
   bool close( bool nowrite = false );
   void invalidate( );
   bool isOpen( ) const;
   /** Writes value to field in section. */
   void write( const std::string& field, const std::string& value,
               const std::string& section = "" );
   /** Reads a field by name and section. */
   const std::string& read( const std::string& name, 
                            const std::string& section = "" ) const;
   /** Reads a field by reverse index within the section.
   * name_out may optionally retrieve the field name. */
   const std::string& read( size_t index, std::string* name_out = 0, 
                            const std::string& section = "" ) const;
   /** Counts how many fields are in a section. */
   size_t count( const std::string& section = "" ) const;
   /** Erases a field by name and section.
   * Duplicate fields will also be erased. */
   void erase( const std::string& name, const std::string& section = "" );
   /** Erases a section and all its fields.
   * Duplicate sections will also be erased. */
   void eraseSection( const std::string& section );

   void purge( );
   void reset( );

   std::string toStr( ) const;

private:
   static LINE*       parse_line( const char* sz_line );
   static const char* section_name( const char* sz_line, std::size_t* len );
   static const char* field_name( const char* sz_line, std::size_t* len );
   static const char* field_value( const char* sz_line, std::size_t* len );

private:
   std::string   m_file;
   bool          m_open;
   bool          m_dirty;
   // First line in the INI.
   LINE*         m_first_line;
   // Last global field in the INI.
   LINE_FIELD*   m_last_field;
   // Last section in the INI.
   LINE_SECTION* m_last_section;
   std::string   m_emptyval;
};

inline bool IniFile::isOpen( ) const
{
   return m_open;
}

inline void IniFile::invalidate( )
{
   m_dirty = true;
}

/*--------------------------------------*/

struct IniFile::LINE
{ 
   Ini_Line_Type type;
   LINE*         next_line;
   std::string   line;
   LINE( ) : type( INI_LINE ), next_line( 0 ) { }
   virtual ~LINE( ) { }
};

struct IniFile::LINE_COMMENT : public IniFile::LINE
{
   LINE_COMMENT* next_comment;
   LINE_COMMENT( ) : next_comment( 0 ) { type = INI_COMMENT; }
};

struct IniFile::LINE_FIELD : public IniFile::LINE
{
   std::string name;
   LINE_FIELD* prev_field;
   std::string value;
   LINE_FIELD( )  : prev_field( 0 ) { type = INI_FIELD; }
};

struct IniFile::LINE_SECTION : public IniFile::LINE
{
   std::string   name;
   LINE_SECTION* prev_section;
   LINE_FIELD*   last_field;
   LINE_SECTION( )  : prev_section( 0 ), 
                      last_field( 0 ) { type = INI_SECTION; }
};

/*
class IniFile::field_iterator
{ 
   friend class IniFile;
private:
   IniFile*        m_ini;
   Lines::iterator m_pos;

public:
   field_iterator( ) : m_ini( 0 ) { }
   field_iterator( const field_iterator& r )
   {
      m_ini = r.m_ini;
      m_pos = r.m_pos;
   }
   const std::string& name( ) const 
   { 
      return ((FIELD*)*m_pos)->name; 
   }
   const std::string& value( ) const 
   { 
      return ((FIELD*)*m_pos)->value; 
   }
   field_iterator& operator ++()
   {
      if (m_pos != m_ini->m_lines.end( ))
      {
         do { ++m_pos; } 
         while (m_pos != m_ini->m_lines.end( ) 
                && (*m_pos)->type != INI_SECTION
                && (*m_pos)->type != INI_FIELD);
      }

      return *this;
   }
   bool at_end( ) const
   {
      return (m_pos == m_ini->m_lines.end( ) || (*m_pos)->type == INI_SECTION);
   }
};

class IniFile::unk_iterator
{ 
   friend class IniFile;
private:
   IniFile*        m_ini;
   Lines::iterator m_pos;

public:
   unk_iterator( ) : m_ini( 0 ) { }
   unk_iterator( const unk_iterator& r )
   {
      m_ini = r.m_ini;
      m_pos = r.m_pos;
   }
   const std::string& line( ) const 
   { 
      return (*m_pos)->line; 
   }
   unk_iterator& operator ++()
   {
      if (m_pos != m_ini->m_lines.end( ))
      {
         do { ++m_pos; } 
         while (m_pos != m_ini->m_lines.end( ) 
            && (*m_pos)->type != INI_SECTION
            && (*m_pos)->type != INI_UNKNOWN);
      }

      return *this;
   }
   bool at_end( ) const
   {
      return (m_pos == m_ini->m_lines.end( ) || (*m_pos)->type == INI_SECTION);
   }
};
*/
#endif
