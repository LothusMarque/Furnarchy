#ifndef _FURCNETBUFFER_H__
#define _FURCNETBUFFER_H__

#include <cstddef>
#include <string>

class NetBuffer
{
public:
   enum { NB_EOF = -1 };
public:
   inline NetBuffer( std::size_t capacity = 4096 );
   inline ~NetBuffer( );

public:
   inline std::size_t        getSize( ) const;
   inline std::size_t        getCapacity( ) const;
   inline std::size_t        pushBytes( const void* p, std::size_t size );
   inline std::size_t        pushFrontBytes( const void* p, std::size_t size );
   inline bool               popNextLine( );
   inline std::size_t        popBytes( std::size_t size );
   inline const std::string& getLastLine( ) const;
   inline std::size_t        eatBytes( std::size_t size );
   inline void               clear( );
   inline void               resize( std::size_t at_least );
   inline int                peek( ) const;
   inline std::size_t        peekBytes( char* p, std::size_t size );
   
private:
   std::size_t m_size;
   std::size_t m_capacity;
   std::size_t m_front;
   char*       m_buf;
   std::string m_last_line;
};

NetBuffer::NetBuffer( std::size_t capacity )
{
   // I like multiples of two.
   m_size     = 0;
   m_capacity = capacity + (capacity & 0x1);
   m_front    = 0;
   m_buf      = new char[ m_capacity ];
}

NetBuffer::~NetBuffer( )
{
   delete [] m_buf;
}

std::size_t NetBuffer::getSize( ) const
{
   return m_size;
}

std::size_t NetBuffer::getCapacity( ) const
{
   return m_capacity;
}

const std::string& NetBuffer::getLastLine( ) const
{
   return m_last_line;
}

void NetBuffer::clear( )
{
   m_size = 0;
   m_front = 0;
}

void NetBuffer::resize( std::size_t at_least )
{
   if (m_capacity < at_least)
   {
      /* Resize. */
      std::size_t new_capacity = m_capacity;
      do 
      {
         new_capacity <<= 1;
      } while (new_capacity < at_least);

      char* new_buf = new char[ new_capacity ];

      /* Copy contents. */
      char* dst = new_buf;
      const char *src = m_buf + m_front;

      if (m_size > m_capacity - m_front)
      {
         const char* const end = m_buf + m_capacity;
         while (src != end) 
            *(dst++) = *(src++);

         const char* const end2 = m_buf + ( m_size - (m_capacity - m_front) );
         src = m_buf;
         while (src != end2) 
            *(dst++) = *(src++);
      }
      else
      {
         const char* const end = src + m_size;
         while (src != end)
            *(dst++) = *(src++);
      }

      /* Reset. */
      m_capacity = new_capacity;
      m_front    = 0;
      delete [] m_buf;
      m_buf      = new_buf;
   }
}

std::size_t NetBuffer::pushBytes( const void* p, std::size_t size )
{
   if (m_size + size > m_capacity)
      resize( m_size + size );

   std::size_t back;
   if (m_front + m_size <= m_capacity)
      back = m_front + m_size;
   else
      back = m_size - (m_capacity - m_front);

   if (back + size <= m_capacity)
   {
      const char* end = m_buf + (back + size);
      char* c = m_buf + back;
      while (c != end)
         *(c++) = *( ((char*&) p)++ );
   }
   else
   {
      const char* end = m_buf + m_capacity;
      char* c = m_buf + back;
      while (c != end)
         *(c++) = *( ((char*&) p)++ );

      end = m_buf + (size - (m_capacity - back));
      c = m_buf;
      while (c != end)
         *(c++) = *( ((char*&) p)++ );
   }

   m_size += size;
   return size;
}

std::size_t NetBuffer::pushFrontBytes( const void* p, std::size_t size )
{
   if (m_size + size > m_capacity)
      resize( m_size + size );

   if (m_front >= size)
   {
      std::size_t pfx = m_front - size;
      const char* end = m_buf + m_front;
      char* c = m_buf + pfx;
      while (c != end)
         *(c++) = *( ((char*&) p)++ );

      m_front = pfx;
   }
   else
   {
      std::size_t pfx = m_capacity - (size - m_front);
      const char* end = m_buf + m_capacity;
      char* c = m_buf + pfx;
      while (c != end)
         *(c++) = *( ((char*&) p)++ );

      end = m_buf + m_front;
      c = m_buf;
      while (c != end)
         *(c++) = *( ((char*&) p)++ );

      m_front = pfx;
   }

   m_size += size;
   return size;
}

bool NetBuffer::popNextLine( )
{
   m_last_line.clear( );
   const char* const begin = m_buf + m_front;
   const char* end;
   if (m_front + m_size <= m_capacity)
      end = begin + m_size;
   else
      end = m_buf + m_capacity;
   
   const char* c = begin;
   for (; c != end && *c != '\n'; ++c) ;

   if (c != end)
   {
      std::size_t len = (std::size_t) c - (std::size_t) begin;
      m_last_line.assign( begin, len );
      ++len;
      m_size -= len;
      m_front += len;
      if (m_front == m_capacity)
         m_front = 0;
      return true;
   }

   if (m_front + m_size > m_capacity)
   {
      std::size_t len = (std::size_t) end - (std::size_t) begin;
      end = m_buf + (m_size - len);
      c = m_buf;
      for (; c != end && *c != '\n'; ++c) ;

      if (c != end)
      {
         m_last_line.assign( begin, len );
         std::size_t len2 = (std::size_t) c - (std::size_t) m_buf;
         m_last_line.append( m_buf, len2 );
         ++len2;
         m_size -= (len + len2);
         m_front = len2;

         return true;
      }
   }

   return false;
}

std::size_t NetBuffer::eatBytes( std::size_t size )
{
   if (size > m_size)
      size = m_size;

   m_last_line.clear( );

   if (m_front + size < m_capacity)
      m_front += size;
   else
      m_front = size - (m_capacity - m_front);

   m_size -= size;
   return size;
}

std::size_t NetBuffer::popBytes( std::size_t size )
{
   m_last_line.clear( );
   if (size > m_size)
      size = m_size;

   if (m_front + size < m_capacity)
   {
      m_last_line.assign( m_buf + m_front, size );
      m_front += size;
   }
   else
   {
      m_last_line.assign( m_buf + m_front, m_capacity - m_front );
      m_last_line.append( m_buf, m_size - (m_capacity - m_front) );
      m_front = size - (m_capacity - m_front);
   }

   m_size  -= size;
   return size;
}

int NetBuffer::peek( ) const
{
   if (m_size)
      return m_buf[ m_front ];

   return NB_EOF;
}

std::size_t NetBuffer::peekBytes( char* buf, std::size_t size )
{
   if (size > m_size)
      size = m_size;

   if (m_front + size < m_capacity)
   {
      memcpy( buf, m_buf + m_front, size );
   }
   else
   {
      memcpy( buf, m_buf + m_front, m_capacity - m_front );
      memcpy( buf + (m_capacity - m_front), m_buf, size - (m_capacity - m_front) );
   }

   return size;
}


#endif