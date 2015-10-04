#ifndef _CACHE_MAP_H__
#define _CACHE_MAP_H__

#include <cstddef>

template <typename KeyT, typename ValueT, typename HashT>
class CacheMap
{
private:
   struct MAP_NODE;
   CacheMap( ) { }
   CacheMap( CacheMap& ) { }
public:
   CacheMap( std::size_t capacity );
   ~CacheMap( );

   inline void        touch( const KeyT& key );
   inline bool        find( const KeyT& key, ValueT* out );
   inline bool        findAndTouch( const KeyT& key, ValueT* out );
   inline void        clear( );
   inline std::size_t size( ) const;
   inline std::size_t capacity( ) const;
   inline bool        oldest( ValueT* out );
   inline void        pop( );
   /** Pushes a new value into the cache map.
   * If the map is full, the oldest entry will be popped and
   * returned in \c out.  &value == out is OK.
   * \return true if an old entry was popped out. */
   bool               push( const KeyT& key, ValueT& value, ValueT* out );

private:
   inline void      _touch( MAP_NODE* node );
   inline MAP_NODE* _find( const KeyT& key );
   void             _erase( MAP_NODE* node );
   void             _insert( MAP_NODE* node );

private:
   HashT       m_hashfn;
   std::size_t m_capacity;
   std::size_t m_size;
   MAP_NODE**  m_map;
   MAP_NODE*   m_oldest;
   MAP_NODE*   m_youngest;

private:
   struct MAP_NODE
   {
      std::size_t hash;
      KeyT        key;
      ValueT      value;
      MAP_NODE*   prev_bucket;
      MAP_NODE*   next_bucket;
      MAP_NODE*   next_oldest;
      MAP_NODE*   prev_oldest;
   };
};

template <typename KeyT, typename ValueT, typename HashT>
CacheMap<KeyT,ValueT,HashT>::CacheMap( std::size_t capacity )
{
   /* A lot of stuff assumes capacity is at least 1. */
   m_capacity = capacity > 0 ? capacity : 1;
   m_size     = 0;
   m_oldest   = 0;
   m_youngest = 0;
   m_map      = new MAP_NODE*[ capacity ];

   for (std::size_t i = 0; i < capacity; i++)
      m_map[ i ] = 0;
}

template <typename KeyT, typename ValueT, typename HashT>
CacheMap<KeyT,ValueT,HashT>::~CacheMap( )
{
   clear( );
   delete [] m_map;
}

template <typename KeyT, typename ValueT, typename HashT>
std::size_t CacheMap<KeyT,ValueT,HashT>::size( ) const
{
   return m_size;
}

template <typename KeyT, typename ValueT, typename HashT>
std::size_t CacheMap<KeyT,ValueT,HashT>::capacity( ) const
{
   return m_capacity;
}

template <typename KeyT, typename ValueT, typename HashT>
void CacheMap<KeyT,ValueT,HashT>::clear( )
{
   MAP_NODE* node = m_oldest;
   MAP_NODE* t;
   while (node)
   {
      t = node->prev_oldest;
      delete node;
      node = t;
   }

   for (std::size_t i = 0; i < m_capacity; i++)
      m_map[ i ] = 0;
   
   m_oldest   = 0;
   m_youngest = 0;
   m_size     = 0;
}

template <typename KeyT, typename ValueT, typename HashT>
void CacheMap<KeyT,ValueT,HashT>::touch( const KeyT& key )
{
   MAP_NODE* node = _find( key );

   if (node)
      _touch( node );
}

template <typename KeyT, typename ValueT, typename HashT>
bool CacheMap<KeyT,ValueT,HashT>::find( const KeyT& key, ValueT* out )
{
   MAP_NODE* node = _find( key );

   if (node)
   {
      *out = node->value;
      return true;
   }

   return false;
}

template <typename KeyT, typename ValueT, typename HashT>
bool CacheMap<KeyT,ValueT,HashT>::findAndTouch( const KeyT& key, ValueT* out )
{
   MAP_NODE* node = _find( key );

   if (node)
   {
      _touch( node );
      *out = node->value;
      return true;
   }

   return 0;
}

template <typename KeyT, typename ValueT, typename HashT>
bool CacheMap<KeyT,ValueT,HashT>::oldest( ValueT* out )
{
   if (m_oldest)
   {
      *out = m_oldest->value;
      return true;
   }

   return false;
}

template <typename KeyT, typename ValueT, typename HashT>
void CacheMap<KeyT,ValueT,HashT>::pop( )
{
   if (m_oldest)
   {
      MAP_NODE* node = m_oldest;
      _erase( m_oldest );
      delete node;
   }      
}

template <typename KeyT, typename ValueT, typename HashT>
bool CacheMap<KeyT,ValueT,HashT>::push( const KeyT& key, ValueT& value, ValueT* out )
{
   /* Make sure it isn't already in the map. */
   MAP_NODE* node = _find( key );
   
   if (node)
      return false;
   
   if (m_capacity == m_size) // Out of room?
   {
      /* Pop off the oldest and insert at the top. */
      node = m_oldest;
      _erase( m_oldest );

      ValueT old_value = node->value;

      node->hash  = m_hashfn( key );
      node->key   = key;
      node->value = value;

      *out = old_value;

      _insert( node );

      return true;
   }

   node        = new MAP_NODE;
   node->hash  = m_hashfn( key );
   node->key   = key;
   node->value = value;

   /* Just insert at the top. */
   _insert( node );

   return false;
}

template <typename KeyT, typename ValueT, typename HashT>
void CacheMap<KeyT,ValueT,HashT>::_touch( MAP_NODE* node )
{
   if (node == m_youngest)
      return;  // Nothing to do.

   _erase( node );
   _insert( node );   
}

template <typename KeyT, typename ValueT, typename HashT>
typename CacheMap<KeyT,ValueT,HashT>::MAP_NODE* CacheMap<KeyT,ValueT,HashT>::_find( const KeyT& key )
{
   std::size_t hash = m_hashfn( key );
   MAP_NODE*   node = m_map[ hash % m_capacity ];

   while (node)
   {
      if (hash == node->hash && key == node->key)
         return node;

      node = node->next_bucket;
   }

   return 0;
}

template <typename KeyT, typename ValueT, typename HashT>
void CacheMap<KeyT,ValueT,HashT>::_erase( MAP_NODE* node )
{
   /* Fixup its linked list nature. */
   MAP_NODE* prev = node->prev_oldest;
   MAP_NODE* next = node->next_oldest;

   if (prev)
      prev->next_oldest = next;
   else
      m_youngest = next;

   if (next)
      next->prev_oldest = prev;
   else
      m_oldest = prev;

   /* Fixup its hash map nature. */
   prev = node->prev_bucket;
   next = node->next_bucket;

   if (prev)
      prev->next_bucket = next;
   else
      m_map[ node->hash % m_capacity ] = next;

   if (next)
      next->prev_bucket = prev;

   --m_size;
}

template <typename KeyT, typename ValueT, typename HashT>
void CacheMap<KeyT,ValueT,HashT>::_insert( MAP_NODE* node )
{
   /* Insert as the youngest. */
   MAP_NODE* next = m_youngest;
   m_youngest = node;

   if (next)
      next->prev_oldest = node;
   else
      m_oldest = node;

   node->next_oldest = next;
   node->prev_oldest = 0;

   /* Insert as first map bucket. */
   std::size_t index = node->hash % m_capacity;
   next = m_map[ index ];
   m_map[ index ] = node;

   if (next)
      next->prev_bucket = node;

   node->next_bucket = next;
   node->prev_bucket = 0;

   ++m_size;
}

#endif
