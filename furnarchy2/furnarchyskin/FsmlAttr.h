#ifndef _FSML_ATTR_H__
#define _FSML_ATTR_H__

#include "fsml_common.h"

/************************************************************************/
/* FsmlAttr                                                             */
/************************************************************************/

class FsmlAttr
{
public:
   FsmlAttr( const string& name,
      const string& value = "" );
   FsmlAttr( const string& name,
      int value );
   FsmlAttr( const string& name,
      float value );
   FsmlAttr( const FsmlAttr& r );
   ~FsmlAttr( );

protected:
   FsmlAttr( ) { };

public:
   const FsmlAttr&    operator =( const FsmlAttr& r );
   const std::string& getName( ) const;
   const std::string& getVal( ) const;
   int                getValI( ) const;
   float              getValF( ) const;

   void               setVal( const string& s );
   void               setValI( int i );
   void               setValF( float f );

private:
   string      m_name;
   string      m_svalue;
   int         m_ivalue;
   float       m_fvalue;
};

inline FsmlAttr::FsmlAttr( const string& name, const string& value )
{
   m_name   = name;
   m_svalue = value;
   m_ivalue = fsml_strtoi( value );
   m_fvalue = fsml_strtof( value );
}

inline FsmlAttr::FsmlAttr( const string& name, int value )
{
   m_name   = name;
   fsml_itostr( &m_svalue, value );
   m_ivalue = value;
   m_fvalue = (float) value;
}

inline FsmlAttr::FsmlAttr( const string& name, float value )
{
   m_name   = name;
   fsml_ftostr( &m_svalue, value );
   m_ivalue = (int) value;
   m_fvalue = value;
}

inline FsmlAttr::FsmlAttr( const FsmlAttr& r )
{
   m_name   = r.m_name;
   m_svalue = r.m_svalue;
   m_ivalue = r.m_ivalue;
   m_fvalue = r.m_fvalue;
}

inline FsmlAttr::~FsmlAttr( )
{

}

inline const FsmlAttr& FsmlAttr::operator =( const FsmlAttr& r )
{
   m_name   = r.m_name;
   m_svalue = r.m_svalue;
   m_ivalue = r.m_ivalue;
   m_fvalue = r.m_fvalue;

   return (*this);
}

inline const string& FsmlAttr::getName( ) const
{
   return m_name;
}

inline const string& FsmlAttr::getVal( ) const
{
   return m_svalue;
}

inline int FsmlAttr::getValI( ) const
{
   return m_ivalue;
}

inline float FsmlAttr::getValF( ) const
{
   return m_fvalue;
}

inline void FsmlAttr::setVal( const string& value )
{
   m_svalue = value;
   m_ivalue = fsml_strtoi( value );
   m_fvalue = fsml_strtof( value );
}

inline void FsmlAttr::setValI( int i )
{
   fsml_itostr( &m_svalue, i );
   m_ivalue = i;
   m_fvalue = (float) i;
}

inline void FsmlAttr::setValF( float f )
{
   fsml_ftostr( &m_svalue, f );
   m_ivalue = (int) f;
   m_fvalue = f;
}

/************************************************************************/
/* FsmlAttrMap                                                          */
/************************************************************************/

#include "common/SzMap.h"

class FsmlAttrMap
{
   typedef SzMap<FsmlAttr*> AttrMap;

public:
   class const_iterator
   {
   private:
      AttrMap::const_iterator i;
   public:
      const_iterator( ) { };
      const_iterator( const const_iterator& _i ) { 
         i = _i.i; 
      }
      const_iterator( const AttrMap::const_iterator& _i ) { 
         i = _i; 
      }
      const_iterator& operator ++( ) { 
         ++i; 
         return *this; 
      }
      const FsmlAttr* operator ->( ) { 
         return i->second; 
      }
      const FsmlAttr& operator *( ) { 
         return *i->second; 
      }
      bool operator == ( const const_iterator& _i ) { 
         return (i == _i.i); 
      }
      bool operator != ( const const_iterator& _i ) { 
         return (i != _i.i); 
      }
      const_iterator& operator =( const const_iterator& _i ) { 
         i = _i.i; return (*this); 
      }
   };

public:
   FsmlAttrMap( ) { }
   FsmlAttrMap( const FsmlAttrMap& r );
   ~FsmlAttrMap( );

public:
   const FsmlAttrMap& operator =( const FsmlAttrMap& r );
   const FsmlAttr*    getAttr( const char* name ) const;
   const FsmlAttr*    getAttr( const string& name ) const;
   const string&      getAttrVal( const char* name,
                                  const string& _default ) const;
   const char*        getAttrSz( const char* name, 
                                 const char* _default = 0 ) const;
   int                getAttrInt( const char* name,
                                  int _default = 0 ) const;
   float              getAttrFloat( const char* name,
                                    float _default = 0.0f ) const;
   /** Setting an attribute string to "" will remove it. */
   void               setAttr( const FsmlAttr& attr );
   const_iterator     end( ) const;
   const_iterator     begin( ) const;

private:
   AttrMap m_map;
};

inline FsmlAttrMap::FsmlAttrMap( const FsmlAttrMap &r )
{
   for (AttrMap::const_iterator i = r.m_map.begin( ); 
        i != r.m_map.end( ); ++i)
   {
      FsmlAttr* my_attr = new FsmlAttr( *i->second );
      m_map.insert( 
         AttrMap::value_type( my_attr->getName( ).c_str( ), my_attr ) );
   }
}

inline FsmlAttrMap::~FsmlAttrMap( )
{
   for (AttrMap::const_iterator i = m_map.begin( ); 
        i != m_map.end( ); ++i)
   {
      delete i->second;
   }

   m_map.clear( );
}

inline const FsmlAttr* FsmlAttrMap::getAttr( const string& name ) const
{
   AttrMap::const_iterator f = m_map.find( name.c_str( ) );

   if (f != m_map.end( ))
      return f->second;

   return 0;
}

inline const FsmlAttr* FsmlAttrMap::getAttr( const char* name ) const
{
   AttrMap::const_iterator f = m_map.find( name );

   if (f != m_map.end( ))
      return f->second;

   return 0;
}

inline void FsmlAttrMap::setAttr( const FsmlAttr& attr )
{
   AttrMap::iterator f = m_map.find( attr.getName( ).c_str( ) );

   if (f != m_map.end( ))
   {
      // Empty attribute values should be removed.
      if (attr.getVal( ).length( ))
         f->second->setVal( attr.getVal( ) );
      else
      {
         // You need to do it like this, apparently, or you get crashes.
         FsmlAttr* p = f->second;
         m_map.erase( f );
         delete p;
      }
   }
   else
   {
      if (attr.getVal( ).length( ))
      {
         FsmlAttr* my_attr = new FsmlAttr( attr );
         m_map.insert( 
            AttrMap::value_type( my_attr->getName( ).c_str( ), my_attr ) );
      }
   }
}

inline const FsmlAttrMap& FsmlAttrMap::operator =( const FsmlAttrMap &r )
{
   for (AttrMap::const_iterator i = m_map.begin( ); 
        i != m_map.end( ); ++i)
   {
      delete i->second;
   }

   m_map.clear( );

   for (AttrMap::const_iterator i = r.m_map.begin( );
        i != r.m_map.end( ); ++i)
   {
      FsmlAttr* my_attr = new FsmlAttr( *i->second );
      m_map.insert( 
         AttrMap::value_type( my_attr->getName( ).c_str( ), my_attr ) );
   }

   return (*this);
}

inline FsmlAttrMap::const_iterator FsmlAttrMap::begin( ) const
{
   return const_iterator( m_map.begin( ) );
}

inline FsmlAttrMap::const_iterator FsmlAttrMap::end( ) const
{
   return const_iterator( m_map.end( ) );
}

inline const string& FsmlAttrMap::getAttrVal( const char* name,
                                              const string& _default ) const
{
   AttrMap::const_iterator f = m_map.find( name );

   if (f != m_map.end( ))
      return f->second->getVal( );

   return _default;
}

inline const char* FsmlAttrMap::getAttrSz( const char* name, 
                                           const char* _default ) const
{
   AttrMap::const_iterator f = m_map.find( name );

   if (f != m_map.end( ))
      return f->second->getVal( ).c_str( );

   return _default;
}

inline int FsmlAttrMap::getAttrInt( const char* name,
                                    int _default ) const
{
   AttrMap::const_iterator f = m_map.find( name );

   if (f != m_map.end( ))
      return f->second->getValI( );

   return _default;
}

inline float FsmlAttrMap::getAttrFloat( const char* name,
                                        float _default ) const
{
   AttrMap::const_iterator f = m_map.find( name );

   if (f != m_map.end( ))
      return f->second->getValF( );

   return _default;
}

#endif