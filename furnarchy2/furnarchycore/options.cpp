#include "options.h"
#include "core.h"
#include "tinyxml/tinyxml.h"

namespace options
{
//////////////////////////////////////////////////////////////////////////

bool load_f2_options( );

//////////////////////////////////////////////////////////////////////////

bool         g_initialized = false;
list<string> g_modules_masked;
list<string> g_modules_accepted;
bool         g_single_user_mode = false;
string       g_command_prefix;

//////////////////////////////////////////////////////////////////////////

bool initialize( )
{
   g_initialized = true;
   load_f2_options( );
   return true;
}

bool uninitialize( )
{
   g_initialized = false;
   return true;
}

const list<string>& get_modules_masked( )
{
   return g_modules_masked;
}

const list<string>& get_modules_accepted( )
{
   return g_modules_accepted;
}

bool get_single_user_mode( )
{
   return g_single_user_mode;
}

const string& get_command_prefix( )
{
   return g_command_prefix;
}

//////////////////////////////////////////////////////////////////////////

bool load_f2_options( )
{
   // Reset.
   g_modules_accepted.clear( );
   g_modules_masked.clear( );
   g_command_prefix = "@";
   g_single_user_mode = false;

   /* Load the single-user options file. */
   string xml_file = core::get_furn_root( ) + "\\options.xml"; 
   TiXmlDocument xml_doc;

   if (xml_doc.LoadFile( xml_file ))
   {
      if (xml_doc.Error( ))
      {
         core::log_error( "Furnarchy options file (%s) couldn't be loaded.",
                          xml_file.c_str( ) );
      }
      else
      {
         // Get the root element.
         const TiXmlElement* const xml_root = xml_doc.RootElement( );
         if (!xml_root || xml_root->ValueStr( ) != "furnarchyOpts")
         {
            core::log_error( "Furnarchy options file (%s) couldn't be loaded.",
                             xml_file.c_str( ) );
         }

         // Read single user mode option.
         {
            const TiXmlElement* const xml_sum =  xml_root->FirstChildElement( "singleUserMode" );
            if (xml_sum && xml_sum->GetText( ))
            {
               g_single_user_mode = strcmp( xml_sum->GetText( ), "true" ) == 0;
            }
         }

         // Read command prefix option.
         {
            const TiXmlElement* const xml_prefix =  xml_root->FirstChildElement( "commandPrefix" );
            if (xml_prefix && xml_prefix->GetText( ))
            {
               g_command_prefix =  xml_prefix->GetText( );
               if (g_command_prefix.empty( ))
                  g_command_prefix = "@";
            }
         }

         // Read the module loading options.
         {
            const TiXmlElement* xml_modules =  xml_root->FirstChildElement( "modules" );
            for (; xml_modules; xml_modules = xml_modules->NextSiblingElement( "modules" ))
            {
               // Iterate children.
               for (const TiXmlElement* xml_elem = xml_modules->FirstChildElement( );
                  xml_elem; xml_elem = xml_elem->NextSiblingElement( ))
               {
                  if (xml_elem->ValueStr( ) == "mask" && xml_elem->GetText( ))
                     g_modules_masked.push_back( xml_elem->GetText( ) );
                  else if (xml_elem->ValueStr( ) == "accept" && xml_elem->GetText( ))
                     g_modules_accepted.push_back( xml_elem->GetText( ) );
               }
            }
         }
      }
   }
   
   /* Load the per-user options file. */
   if (!g_single_user_mode)
   {
      xml_file = core::get_furn_doc_root( ) + "\\options.xml"; 

      if (xml_doc.LoadFile( xml_file ))
      {
         if (xml_doc.Error( ))
         {
            core::log_error( "Furnarchy options file (%s) couldn't be loaded.",
                             xml_file.c_str( ) );
         }
         else
         {
            // Overwrite masked and accepted modules.
            g_modules_accepted.clear( );
            g_modules_masked.clear( );

            // Get the root element.
            const TiXmlElement* const xml_root = xml_doc.RootElement( );
            if (!xml_root || xml_root->ValueStr( ) != "furnarchyOpts")
            {
               core::log_error( "Furnarchy options file (%s) couldn't be loaded.",
                                xml_file.c_str( ) );
            }

            // Read single user mode option.
            {
               const TiXmlElement* const xml_sum =  xml_root->FirstChildElement( "singleUserMode" );
               if (xml_sum && xml_sum->GetText( ))
               {
                  g_single_user_mode = strcmp( xml_sum->GetText( ), "true" ) == 0;
               }
            }

            // Read command prefix option.
            {
               const TiXmlElement* const xml_prefix =  xml_root->FirstChildElement( "commandPrefix" );
               if (xml_prefix && xml_prefix->GetText( ))
               {
                  g_command_prefix =  xml_prefix->GetText( );
                  if (g_command_prefix.empty( ))
                     g_command_prefix = "@";
               }
            }

            // Read the module loading options.
            {
               const TiXmlElement* xml_modules =  xml_root->FirstChildElement( "modules" );
               for (; xml_modules; xml_modules = xml_modules->NextSiblingElement( "modules" ))
               {
                  // Iterate children.
                  for (const TiXmlElement* xml_elem = xml_modules->FirstChildElement( );
                     xml_elem; xml_elem = xml_elem->NextSiblingElement( ))
                  {
                     if (xml_elem->ValueStr( ) == "mask" && xml_elem->GetText( ))
                        g_modules_masked.push_back( xml_elem->GetText( ) );
                     else if (xml_elem->ValueStr( ) == "accept" && xml_elem->GetText( ))
                        g_modules_accepted.push_back( xml_elem->GetText( ) );
                  }
               }
            }
         }
      }
   }

   return true;
}


//////////////////////////////////////////////////////////////////////////
} // namespace options