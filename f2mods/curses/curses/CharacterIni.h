#ifndef _CHARACTER_INI_H__
#define _CHARACTER_INI_H__

#include <string>
#include <ctime>

class CharacterIni
{
public:
   explicit CharacterIni( const std::string& file );
   CharacterIni( ) { }
   ~CharacterIni( ) { }

   bool read( );
   bool write( );
   void clear( );
   std::string toStr( ) const;
   const std::string& getFile( ) const;
   void          setFile( const std::string& file );

   const std::string&  getName( ) const;
   const std::string&  getPassword( ) const;
   const std::string&  getDescription( ) const;
   const std::string&  getColorCode( ) const;
   const std::string&  getSpecies( ) const;
   std::time_t         getLastLogin( ) const;
   unsigned            getNumLogins( ) const;

   void setName( const std::string& name );
   void setPassword( const std::string& password );
   void setDescription( const std::string& description );
   void setColorCode( const std::string& colors );
   void setSpecies( const std::string& species );
   void setLastLogin( std::time_t login );
   void setNumLogins( unsigned logins );

private:
   std::string   m_file;
   std::string   m_name;
   std::string   m_password;
   std::string   m_description;
   std::string   m_colorcode;
   std::string   m_spec;
   std::time_t   m_last_login;
   unsigned      m_num_logins;
   //V3.0/Client 027
   std::string   m_autoresponse;
   std::string   m_autoresmessage;
   std::string   m_afktime;
   std::string   m_afkmessage;
   std::string   m_afkdesc;
   std::string   m_afkport;
   std::string   m_defaultport;
   std::string   m_afkdisconnecttime;
   //
};

inline CharacterIni::CharacterIni( const std::string& file )
   : m_file( file ), m_num_logins( 0 ), m_last_login( 0 ) { }

inline const std::string& CharacterIni::getFile( ) const {
   return m_file;
}

inline void CharacterIni::setFile( const std::string& file ) {
   m_file = file;
}

inline void CharacterIni::clear( )
{
   m_file.clear( );
   m_name.clear( );
   m_password.clear( );
   m_description.clear( );
   m_colorcode.clear( );
   m_spec.clear( );
   m_last_login = 0;
   m_num_logins = 0;
   
}

inline const std::string& CharacterIni::getName( ) const {
   return m_name;
}

inline const std::string& CharacterIni::getPassword( ) const {
   return m_password;
}

inline const std::string& CharacterIni::getDescription( ) const {
   return m_description;
}

inline const std::string& CharacterIni::getColorCode( ) const {
   return m_colorcode;
}

inline const std::string& CharacterIni::getSpecies( ) const {
   return m_spec;
}

inline std::time_t CharacterIni::getLastLogin( ) const {
   return m_last_login;
}

inline unsigned CharacterIni::getNumLogins( ) const {
   return m_num_logins;
}

inline void CharacterIni::setName( const std::string& name ) {
   m_name = name;
}

inline void CharacterIni::setPassword( const std::string& password ) {
   m_password = password;
}

inline void CharacterIni::setDescription( const std::string& description ) {
   m_description = description;
}

inline void CharacterIni::setColorCode( const std::string& colors ) {
   m_colorcode = colors;
}

inline void CharacterIni::setSpecies( const std::string& species ) {
   m_spec = species;
}

inline void CharacterIni::setLastLogin( std::time_t login ) {
   m_last_login = login;
}

inline void CharacterIni::setNumLogins( unsigned logins ) {
   m_num_logins = logins;
}

#endif