#include "FsmlZoo.h"

FsmlImage* FsmlZoo::getImage( const string &name )
{
   /* Check if it's already in the zoo first. */
   FsmlImage* img = findImage( name );
   
   if (!img)
   {
      /* Nah, load it and add it. */
      for (vector<string>::const_iterator i = m_paths.begin( ); i != m_paths.end( ); ++i)
      {
         DirStack cwd;
         cwd.push( *i );
         if (img = FsmlImage::load_from_file( name ))
         {
            addImage( name, img );
            break;
         }
      }
   }

   return img;
}
