#ifndef _REFCOUNT_H__
#define _REFCOUNT_H__

class RefCount
{
public:

   inline int           addRef( ) const;
   inline int           countRef( ) const;
   inline int           release( ) const;

protected:
   
   inline               RefCount( );
   virtual              ~RefCount( ) { }

protected:

   mutable int          m_ref_count;

};

RefCount::RefCount( )
{
   m_ref_count = 0;
}

int RefCount::addRef( ) const
{
   return ++m_ref_count;
}

int RefCount::countRef( ) const
{
   return m_ref_count;
}

int RefCount::release( ) const
{
   if (--m_ref_count <= 0)
   {
      delete this;
      return 0;
   }

   return m_ref_count;
}

#endif