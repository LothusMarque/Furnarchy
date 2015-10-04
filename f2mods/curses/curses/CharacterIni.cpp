#include "CharacterIni.h"
#include "yiffurc/util.hpp"
#include <fstream>
#include <sstream>

using namespace std;

static string spec_from_colors( const string& colors )
{
   string spec = "000";

   if (!colors.empty( ))
   {
      switch (colors[ 0 ])
      {
      case 't':
         {
            yiffurc::ColorCodePal code( colors.substr( 1 ) );
            spec[ 0 ] += code.getGsd( yiffurc::ColorCodePal::GSD_GENDER );
            spec[ 1 ] += code.getGsd( yiffurc::ColorCodePal::GSD_SPECIES );
            spec[ 2 ] += code.getGsd( yiffurc::ColorCodePal::GSD_DIGO );
         }
         break;

      default: // Old-skool.
         if (colors.length( ) > 10)
            spec[ 0 ] += colors[ 10 ] - ' ';
         if (colors.length( ) > 11)
            spec[ 1 ] += colors[ 11 ] - ' ';
         if (colors.length( ) > 12)
            spec[ 2 ] += colors[ 12 ] - ' ';
      }
   }
   return spec;
}

static string update_color_format( const string& colors )
{
   if (colors[ 0 ] != 't' && colors[ 0 ] != 'u' && colors[ 0 ] != 'v'
       && colors.length( ) <= 13)
   {
      string updated = "t";
      for (size_t i = 0; i < 13 && i < colors.length( ); ++i)
         updated += yiffurc::itof1_220( (unsigned) colors[ i ] - ' '  );
      if (updated.length( ) < 13)
         updated.append( 13 - updated.length( ), yiffurc::itof1_220( 0 ) );
      return updated;
   }
   return colors;
}

bool CharacterIni::read( )
{
   ifstream fs( m_file.c_str( ) );

   if (!fs.good( ))
      return false;

   // First line should be "V1.6 character".
   string line;
   getline( fs, line );
   if (line != "V1.6 character" && line != "V2.0 character" && line != "V3.0 character")
      return false;

   while (fs.good( ))
   {
      getline( fs, line );

      if (line.length( ))
      {
         if (string::npos != line.find_first_of( "=" ))
         {
            string field = line.substr( 0, line.find_first_of( "=" ) );
            field        = field.substr( field.find_first_not_of( " \t" ) );         
            string value = line.substr( line.find_first_of( "=" ) + 1 );

            if (field == "Name")
               m_name = value;
            else if (field == "Password")
               m_password = value;
            else if (field == "Desc")
               m_description = value;
            else if (field == "Colors")
               m_colorcode = value;
            else if (field == "Spec")
               m_spec = value;
            else if (field == "Logins")
               m_num_logins = atol( value.c_str( ) );
            else if (field == "LastLogin")
               m_last_login = atol( value.c_str( ) );
			//Client V027
			else if (field == "AutoResponse")
               m_autoresponse = value;
			else if (field == "AutoResponseMessage")
               m_autoresmessage = value;
			else if (field == "AFKTime")
               m_afktime = value;
			else if (field == "AFKMessage")
               m_afkmessage = value;
			else if (field == "AFKDescription")
               m_afkdesc = value;
			else if (field == "AFKPortrait")
               m_afkport = value;
			else if (field == "DefaultPortrait")
               m_defaultport = value;
			else if (field == "AFKDisconnectTime")
               m_afkdisconnecttime = value;
			//V027
         }
      }
   }

   m_colorcode = update_color_format( m_colorcode );

   return true;
}

string CharacterIni::toStr( ) const
{
   ostringstream ss;
   ss << "V3.0 character\n";
   ss << "Colors=" << m_colorcode << '\n'
      << "Name=" << m_name << '\n'
      << "Password=" << m_password << '\n'
      << "Desc=" << m_description << '\n'
      //<< "Spec=" << (m_spec.length( ) == 3 ? spec_from_colors( m_colorcode ) : m_spec) << '\n'
      << "Logins=" << m_num_logins << '\n'
	  //V027
	  << "AutoResponse=" << m_autoresponse << '\n'
	  << "AutoResponseMessage=" << m_autoresmessage << '\n'
	  << "AFKTime=" << m_afktime << '\n'
	  << "AFKMessage=" << m_afkmessage << '\n'
	  << "AFKDescription=" << m_afkdesc << '\n'
	  << "AFKPortrait=" << m_afkport << '\n'
	  << "DefaultPortrait=" << m_defaultport << '\n'
	  << "AFKDisconnectTime=" << m_afkdisconnecttime
	  //V027
	  ;
   if (m_last_login)
      ss << "LastLogin=" << m_last_login << '\n';
   return ss.str( );
}

bool CharacterIni::write( )
{
   ofstream fs( m_file.c_str( ) );
   if (fs.good( ))
   {
      fs << toStr( );
      fs.close( );
      return true;
   }
   return false;
}