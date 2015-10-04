/************************************************************************/
/* Copyright (c) 2006 Cluracan (strangelove@heroinpuppy.com)            */
/*                                                                      */
/* The "MIT" License                                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* - The above copyright notice and this permission notice shall be     */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          */
/* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         */
/* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                            */
/************************************************************************/

#ifndef _YIFFURC_NET_PFX_HEAP_H__
#define _YIFFURC_NET_PFX_HEAP_H__

#include <cstddef>
#include <new>

namespace yiffurc {

struct _align_T
{
   union _align_U
   {
      double      d;
      long double ld;
      long        l;
      long long   ll;
      void*       v;
   }    u;
   char c;
};

const size_t MAX_ALIGN = (sizeof( _align_T ) - sizeof(_align_T::_align_U));

struct PFX
{
   enum { STATIC = 0x1, DESTRUCT = 0x2 };

   struct
   {
      short flags;
      short clsid;
   }           info;
   mutable int ref_count;

   static inline size_t padded_size( )
   {
      size_t r = sizeof( PFX ) % MAX_ALIGN;
      if (!r) return sizeof( PFX );
      return sizeof( PFX ) + (MAX_ALIGN - r);
   }
};

extern size_t pfx_count;

/* For simple data types or objects with trivial destructors. */
template <typename T>
inline T* pfx_new( size_t size = 1, bool is_static = false )
{
   pfx_count++;
   PFX* p = (PFX*) ::operator new( PFX::padded_size( ) + sizeof( T ) * size );
   p->info.flags = is_static ? PFX::STATIC : 0;
   p->info.clsid = 0;
   p->ref_count  = 1;
   return new( (char*) p + PFX::padded_size( ) ) T;
}

/** For objects with non-trivial destructors. */
template <typename T>
inline T* pfx_new_obj( bool is_static = false )
{
   pfx_count++;
   PFX* p = (PFX*) ::operator new( PFX::padded_size( ) + sizeof( T ) );
   p->info.flags = PFX::DESTRUCT | (is_static ? PFX::STATIC : 0);
   p->info.clsid = T::CLSID;  // Must have unique CLSID member.
   p->ref_count  = 1;
   return new( (char*) p + PFX::padded_size( ) ) T;
}

inline int pfx_add_ref( const void* p )
{
   if (p)
   {
      const PFX* pfx = (PFX*) ( (const char*) p - PFX::padded_size( ) );
      if (!( pfx->info.flags & PFX::STATIC ))
         return ++pfx->ref_count;
   }
   return 0;
}

extern void pfx_free( const void* p );

inline void pfx_release( const void* p )
{
   if (p)
   {
      const PFX* pfx = (PFX*) ( (const char*) p - PFX::padded_size( ) );
      if (!( pfx->info.flags & PFX::STATIC ))
         if (--pfx->ref_count <= 0)
            return pfx_free( p );
   }
}

} // namespace yiffurc

#endif

