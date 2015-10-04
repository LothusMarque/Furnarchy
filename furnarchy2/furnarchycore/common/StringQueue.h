#ifndef _STRINGQUEUE_H__
#define _STRINGQUEUE_H__

#include <string>
#include <queue>

class StringQueue
{
public:
   inline StringQueue( );
   inline StringQueue( const StringQueue& r );
   inline ~StringQueue( );

   inline void               push( const std::string& str );
   inline std::string&       front( );
   inline const std::string& front( ) const;
   inline void               pop( );
   inline bool               empty( ) const;
   inline std::size_t        size( ) const;
   inline void               clear( );

private:
   std::queue<std::string*> m_queue;
};

StringQueue::StringQueue( )
{

}

StringQueue::StringQueue( const StringQueue& r )
{
   m_queue = r.m_queue;
}

StringQueue::~StringQueue( )
{
   clear( );
}

void StringQueue::push( const std::string& str )
{
   m_queue.push( new std::string( str ) );
}

std::string& StringQueue::front( )
{
   return *m_queue.front( );
}

const std::string& StringQueue::front( ) const
{
   return *m_queue.front( );
}

void StringQueue::pop( )
{
   m_queue.pop( );
}

bool StringQueue::empty( ) const
{
   return m_queue.empty( );
}

std::size_t StringQueue::size( ) const
{
   return m_queue.size( );
}

void StringQueue::clear( )
{
   while (!m_queue.empty( ))
   {
      delete m_queue.front( );
      m_queue.pop( );
   }
}

#endif
