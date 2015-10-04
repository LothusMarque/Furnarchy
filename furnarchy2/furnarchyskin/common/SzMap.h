#ifndef _SZ_MAP_H__
#define _SZ_MAP_H__

/* Standards are for the weak! */
#ifdef __GNUC__

#include <ext/hash_map>

struct SZMAP_HASH
{
   size_t operator ()( const char* sz ) const
   {
      std::size_t h= 0;
      std::size_t c;

      while (c = *sz++)
         h = c + (h << 6) + (h << 16) - h;

      return h;
   }
};

struct SZMAP_EQ
{
   int operator ()( const sz* a, const sz* b ) const {
      return std::strcmp( a, b );
   }
};

template <typename T>
class SzMap : public __gnu_cxx::hash_map< const char*, T,
                                                 SZMAP_HASH,
                                                 SZMAP_EQ > { };

#else /* MSVC */

#include <hash_map>

struct SZMAP_TRAITS
{
   enum
   {
      bucket_size = 4,
      min_buckets = 8,
   };

   size_t operator ()( const char* sz ) const
   {
      std::size_t h = 0;
      std::size_t c;

      while (c = *sz++)
         h = c + (h << 6) + (h << 16) - h;

      return h;
   }

   bool operator ()( const char* a, const char* b ) const {
      return (std::strcmp( a, b ) < 0);
   }
};

template <typename T>
class SzMap : public stdext::hash_map< const char*, T,
                                       SZMAP_TRAITS > { };

#endif

#endif // #ifndef _SZMAP_H__