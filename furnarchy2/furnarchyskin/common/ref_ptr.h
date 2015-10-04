#ifndef _REF_PTR_H__
#define _REF_PTR_H__

#include "ref_count.h"

// Forward declare.
template <class T_> class ref_ptr;
template <class T_> class c_ref_ptr;

/** Complements the RefCount class. T_ must be derived from RefCount. */
template <class T_>
class ref_ptr
{
   friend c_ref_ptr<T_>;
public:

   inline                  ref_ptr( );
   inline                  ref_ptr( const ref_ptr<T_>& copy );
   inline                  ref_ptr( RefCount* target );
   inline                  ~ref_ptr( );

public:

   inline bool             ok( ) const;
   inline void             release( );

//   inline                  operator bool( ) const;
   inline                  operator T_*( ) const;

   inline bool             operator !( ) const;
//   inline bool             operator ==( const RefCount* o ) const;
//   inline bool             operator ==( const ref_ptr<T_>& o ) const;
//   inline bool             operator ==( const c_ref_ptr<T_>& o ) const;
//   inline bool             operator ==( long o ) const;

   inline const ref_ptr<T_>& operator =( RefCount* target );
   inline const ref_ptr<T_>& operator =( const ref_ptr<T_>& target );

   inline T_*              operator ->( ) const;
   inline T_&              operator *( ) const;

private:

   RefCount*               m_target;
};


/** Same as ref_ptr, but handles const pointers. */
template <class T_>
class c_ref_ptr
{
   friend class ref_ptr<T_>;
public:
   
   inline                  c_ref_ptr( );
//   inline                  c_ref_ptr( const ref_ptr<T_>& copy );
   inline                  c_ref_ptr( const c_ref_ptr<T_>& copy );
   inline                  c_ref_ptr( const RefCount* target );
   inline                  ~c_ref_ptr( );

public:
   
   inline bool             ok( ) const;
   inline void             release( );

//   inline                  operator bool( ) const;
   inline                  operator const T_*( ) const;

   inline bool             operator !( ) const;
//   inline bool             operator ==( const RefCount* o ) const;
//   inline bool             operator ==( const ref_ptr<T_>& o ) const;
//   inline bool             operator ==( const c_ref_ptr<T_>& o ) const;
//   inline bool             operator ==( long o ) const;

   inline const c_ref_ptr<T_>& operator =( const RefCount* target );
   inline const c_ref_ptr<T_>& operator =( const c_ref_ptr<T_>& target );
//   inline const c_ref_ptr<T_>& operator =( const ref_ptr<T_>& target );

   inline const T_*        operator ->( ) const;
   inline const T_&        operator *( ) const;

private:

   const RefCount*         m_target;
};


/*-------------ref_ptr definitions---------------*/

template <class T_>
ref_ptr<T_>::ref_ptr( )
{
   m_target = 0;
}

template <class T_>
ref_ptr<T_>::ref_ptr( const ref_ptr<T_>& copy )
{
   m_target = copy.m_target;

   if (m_target)
      m_target->addRef( );
}

template <class T_>
ref_ptr<T_>::ref_ptr( RefCount* target )
{
   m_target = target;

   if (m_target)
      m_target->addRef( );
}

template <class T_>
ref_ptr<T_>::~ref_ptr( )
{
   if (m_target)
      m_target->release( );
}

template <class T_>
bool ref_ptr<T_>::ok( ) const
{
   return (m_target != 0);
}

template <class T_>
void ref_ptr<T_>::release( )
{
   if (m_target)
   {
      m_target->release( );
      m_target = 0;
   }
}
/*
template <class T_>
ref_ptr<T_>::operator bool( ) const
{
   return (m_target != 0);
}
*/
template <class T_>
ref_ptr<T_>::operator T_*( ) const
{
   return (T_*) m_target;
}

template <class T_>
bool ref_ptr<T_>::operator !( ) const
{
   return (m_target == 0);
}
/*
template <class T_>
bool ref_ptr<T_>::operator ==( const RefCount* o ) const
{
   return (m_target == o);
}

template <class T_>
bool ref_ptr<T_>::operator ==( const c_ref_ptr<T_>& o ) const
{
   return (m_target == o.m_target);
}

template <class T_>
bool ref_ptr<T_>::operator ==( const ref_ptr<T_>& o ) const
{
   return (m_target == o.m_target);
}

template <class T_>
bool ref_ptr<T_>::operator ==( long o ) const
{
   return (m_target == o);
}
*/
template <class T_>
const ref_ptr<T_>& ref_ptr<T_>::operator =( RefCount* target )
{
   if (m_target != target)
   {
      if (m_target)
         m_target->release( );

      m_target = target;

      if (m_target)
         m_target->addRef( );
   }
   
   return (*this);
}

template <class T_>
const ref_ptr<T_>& ref_ptr<T_>::operator =( const ref_ptr<T_>& target )
{
   if (m_target != target.m_target)
   {
      if (m_target)
         m_target->release( );

      m_target = target.m_target;

      if (m_target)
         m_target->addRef( );
   }

   return (*this);
}

template <class T_>
T_* ref_ptr<T_>::operator ->( ) const
{
   return (T_*) m_target;
}

template <class T_>
T_& ref_ptr<T_>::operator *( ) const
{
   return (*(T_*) m_target);
}

/*------------c_ref_ptr definitions--------------*/

template <class T_>
c_ref_ptr<T_>::c_ref_ptr( )
{
   m_target = 0;
}
/*
template <class T_>
c_ref_ptr<T_>::c_ref_ptr( const ref_ptr<T_>& copy )
{
   m_target = copy.m_target;

   if (m_target)
      m_target->addRef( );
}
*/
template <class T_>
c_ref_ptr<T_>::c_ref_ptr( const c_ref_ptr<T_>& copy )
{
   m_target = copy.m_target;

   if (m_target)
      m_target->addRef( );
}

template <class T_>
c_ref_ptr<T_>::c_ref_ptr( const RefCount* target )
{
   m_target = target;

   if (m_target)
      m_target->addRef( );
}

template <class T_>
c_ref_ptr<T_>::~c_ref_ptr( )
{
   if (m_target)
      m_target->release( );
}

template <class T_>
void c_ref_ptr<T_>::release( )
{
   if (m_target)
   {
      m_target->release( );
      m_target = 0;
   }
}

template <class T_>
bool c_ref_ptr<T_>::ok( ) const
{
   return (m_target != 0);
}
/*
template <class T_>
c_ref_ptr<T_>::operator bool( ) const
{
   return (m_target != 0);
}
*/
template <class T_>
c_ref_ptr<T_>::operator const T_*( ) const
{
   return (const T_*) m_target;
}

template <class T_>
bool c_ref_ptr<T_>::operator !( ) const
{
   return (m_target == 0);
}
/*
template <class T_>
bool c_ref_ptr<T_>::operator ==( const RefCount* o ) const
{
   return (m_target == o);
}

template <class T_>
bool c_ref_ptr<T_>::operator ==( const ref_ptr<T_>& o ) const
{
   return (m_target == o.m_target);
}

template <class T_>
bool c_ref_ptr<T_>::operator ==( const c_ref_ptr<T_>& o ) const
{
   return (m_target == o.m_target);
}

template <class T_>
bool c_ref_ptr<T_>::operator ==( long o ) const
{
   return (m_target == o);
}
*/
template <class T_>
const c_ref_ptr<T_>& c_ref_ptr<T_>::operator =( const RefCount* target )
{
   if (m_target != target)
   {
      if (m_target)
         m_target->release( );

      m_target = target;

      if (m_target)
         m_target->addRef( );
   }

   return (*this);
}

template <class T_>
const c_ref_ptr<T_>& c_ref_ptr<T_>::operator =( const c_ref_ptr<T_>& target )
{
   if (m_target != target.m_target)
   {
      if (m_target)
         m_target->release( );

      m_target = target.m_target;

      if (m_target)
         m_target->addRef( );
   }

   return (*this);
}
/*
template <class T_>
const c_ref_ptr<T_>& c_ref_ptr<T_>::operator =( const ref_ptr<T_>& target )
{
    if (m_target != (const RefCount*) target)
   {
      if (m_target)
         m_target->release( );

      m_target = target.m_target;

      if (m_target)
         m_target->addRef( );
   }

   return (*this);
}
*/
template <class T_>
const T_* c_ref_ptr<T_>::operator ->( ) const
{
   return (T_*) m_target;
}

template <class T_>
const T_& c_ref_ptr<T_>::operator *( ) const
{
   return (*(T_*) m_target);
}

#endif