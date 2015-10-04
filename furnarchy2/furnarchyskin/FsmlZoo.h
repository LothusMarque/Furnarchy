#ifndef _FSML_ZOO_H__
#define _FSML_ZOO_H__

#include "fsml_common.h"
#include "FsmlImage.h"
#include "common/SzMap.h"

class FsmlZoo
{
   struct ZOO_IMAGE;
   typedef SzMap<ZOO_IMAGE*> ImageMap;

public:
   template <class Iter, class ConstIter>
   FsmlZoo( Iter paths_first, const ConstIter& paths_last );
   ~FsmlZoo( );
private:
   FsmlZoo( ) { }
   FsmlZoo( const FsmlZoo& r ) { }
public:
   /** Creates the image if it doesn't exist. */
   FsmlImage* getImage( const string& name );
   FsmlImage* findImage( const string& name ) const;
   void       addImage( const string& name,
                        FsmlImage* img );
   void       removeImage( const string& name );
   void       purge( );

private:
   vector<string> m_paths;
   ImageMap       m_images;
};

struct FsmlZoo::ZOO_IMAGE
{
   string       name;
   FsmlImagePtr image;
   ZOO_IMAGE( const string& name = "", FsmlImage* image = 0 )
      : name( name ), image( image )
   {
      
   }
};

template <class Iter, class ConstIter>
inline FsmlZoo::FsmlZoo( Iter paths_first, const ConstIter& paths_last )
{
   for (; paths_first != paths_last; ++paths_first)
      m_paths.push_back( *paths_first );
}

inline FsmlZoo::~FsmlZoo( )
{
   purge( );
}

inline FsmlImage* FsmlZoo::findImage( const string& name ) const
{
   ImageMap::const_iterator f = m_images.find( name.c_str( ) );

   if (f != m_images.end( ))
      return f->second->image;

   return 0;
}

inline void FsmlZoo::addImage( const string& name, FsmlImage* img )
{
   ImageMap::iterator f = m_images.find( name.c_str( ) );

   if (f != m_images.end( ))
      f->second->image = img;
   else
   {
      ZOO_IMAGE* my_img = new ZOO_IMAGE( name, img );
      m_images.insert( ImageMap::value_type( my_img->name.c_str( ), my_img ) );
   }
}

inline void FsmlZoo::removeImage( const string& name )
{
   ImageMap::iterator f = m_images.find( name.c_str( ) );

   if (f != m_images.end( ))
   {
      ZOO_IMAGE* p = f->second;
      m_images.erase( f );
      delete p;
   }
}

inline void FsmlZoo::purge( )
{
   for (ImageMap::const_iterator i = m_images.begin( ); 
        i != m_images.end( ); ++i)
   {
      delete i->second;
   }

   m_images.clear( );
}

#endif