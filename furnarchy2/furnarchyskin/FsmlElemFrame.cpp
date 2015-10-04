#include "FsmlElem.h"
#include "FsmlCanvas.h"
#include "FsmlScript.h"

enum { MOUSE_LDOWN = 0x1, MOUSE_RDOWN = 0x2 };

#define CLAMP_INT( i ) (((int) i) < 0 ? 0 : (i))

const char* const FsmlElemFrame::s_symbol = "frame";

void FsmlElemFrame::setParent( FsmlElem* parent )
{
   m_parent = parent;
}

FsmlElem* FsmlElemFrame::clone( ) const
{
   FsmlElemFrame* e = new FsmlElemFrame( );

   e->m_script   = m_script;
   e->m_visible  = m_visible;
   e->m_rect     = m_rect;
   e->m_origin   = m_origin;
   e->m_attrs    = m_attrs;
   e->m_mouse    = m_mouse;

   ElemVector::const_iterator i = m_children.begin( );

   for (; i != m_children.end( ); ++i)
   {
      e->m_children.push_back( (*i)->clone( ) );
      e->m_children.back( )->setParent( e );
   }
   
   return e;
}

bool FsmlElemFrame::isVisible( ) const
{
   return m_visible;
}

void FsmlElemFrame::getRect( FSML_RECT* out ) const
{
   *out = m_rect;
}

const FsmlAttr* FsmlElemFrame::getAttr( const string& name ) const
{
   return m_attrs.getAttr( name.c_str( ) );
}

void FsmlElemFrame::setAttr( const FsmlAttr& attr )
{
   m_attrs.setAttr( attr );

   if (attr.getName( ) == "visible")
   {
      if (attr.getVal( ) == "no")
         m_visible = false;
      else
         m_visible = true;
   }

   calcRect( &m_rect, &m_origin );
   if (m_parent) m_parent->onInvalidate( this );
}

FsmlElem* FsmlElemFrame::find( const string& id, 
                               FsmlElemType t /* = FSMLELEM_ANY */ )
{
   /* Check ourself. */
   if (fsml_elem_same( FSMLELEM_FRAME, t ))
      if (m_attrs.getAttrSz( "id", "" ) == id)
         return this;
   
   /* Check children. */
   ElemVector::iterator i = m_children.begin( );
   FsmlElem* f = 0;

   for (; i != m_children.end( ); ++i)
   {
      if (f = (*i)->find( id, t ))
         return f;
   }

   return 0;
}

const FsmlElem* FsmlElemFrame::find( const string& id, 
                                     FsmlElemType t /* = FSMLELEM_ANY */ ) const
{
   /* Check ourself. */
   if (fsml_elem_same( FSMLELEM_FRAME, t ))
      if (m_attrs.getAttrSz( "id", "" ) == id)
         return this;

   /* Check children. */
   ElemVector::const_iterator i = m_children.begin( );
   const FsmlElem* f = 0;

   for (; i != m_children.end( ); ++i)
   {
      if (f = (*i)->find( id, t ))
         return f;
   }

   return 0;
}

bool FsmlElemFrame::addChild( FsmlElem* child, FsmlElem* before /* = 0  */ )
{
   /* Only can have certain child types. */
   switch (child->getType( ))
   {
   default:
      return false;
   case FSMLELEM_BUTTON:
   case FSMLELEM_FRAME:
   case FSMLELEM_IMAGE:
   case FSMLELEM_TEMPLATE:
      break;
   }

   /* Insert before an element? */
   if (before)
   {
      // Find the element.
      ElemVector::iterator i = m_children.begin( );
      for (; i != m_children.end( ); ++i)
         if ((*i) == before)
            break;

      if (i != m_children.end( ))
      {
         m_children.insert( i, child );
         child->setParent( this );
         calcRect( &m_rect, &m_origin );
         if (m_parent) m_parent->onInvalidate( this );
         return true;
      }
   }

   /* Otherwise just push to the back. */
   m_children.push_back( child );
   child->setParent( this );
   calcRect( &m_rect, &m_origin );
   if (m_parent) m_parent->onInvalidate( this );
   return true;
}

unsigned int FsmlElemFrame::getNumChildren( ) const
{
   return (unsigned int) m_children.size( );
}

bool FsmlElemFrame::construct( const string& id )
{
   ElemVector::iterator i = m_children.begin( );

   for (; i != m_children.end( ); ++i)
   {
      if ((*i)->construct( id ))
         return true;
   }

   return false;
}

void FsmlElemFrame::draw( FsmlCanvas* canvas )
{
   if (m_visible)
   {
      ElemVector::const_iterator i = m_children.begin( );

      for (; i != m_children.end( ); ++i)
      {
         FSML_RECT child_rect;
         (*i)->getRect( &child_rect );

         child_rect.x += m_origin.x;
         child_rect.y += m_origin.y;
         canvas->pushViewport( &child_rect );

         (*i)->draw( canvas );

         canvas->popViewport( );
      }
   }
}

void FsmlElemFrame::onSelectScript( FsmlScript* script )
{
   m_script = script;

   ElemVector::iterator i = m_children.begin( );
   for (; i != m_children.end( ); ++i)
      (*i)->onSelectScript( script );
}

void FsmlElemFrame::onTick( unsigned int dt )
{
   if (m_script)
      if (m_script->call( this, m_attrs.getAttrSz( "onTick" ), "i", dt ))
         return;

   for (ElemVector::iterator i = m_children.begin( );
        i != m_children.end( ); ++i)
      (*i)->onTick( dt );
}

void FsmlElemFrame::onModState( unsigned int mod, long state )
{
   if (m_script)
      if (m_script->call( this, m_attrs.getAttrSz( "onModState" ), "il", mod, state ))
         return;

   for (ElemVector::iterator i = m_children.begin( );
      i != m_children.end( ); ++i)
      (*i)->onModState( mod, state );
}

void FsmlElemFrame::onMouseOver( const FSML_POS& pos )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseOver" ), "2", &pos );
}

void FsmlElemFrame::onMouseMove( const FSML_POS&from, const FSML_POS&to )
{
   FsmlElemPtr target = childFromPoint( to );

   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseMove" ), 
                                     "22", &from, &to ))
   {
      if (target != m_mouse.target)
      {
         if (m_mouse.target)
            m_mouse.target->onMouseOut( toChildPos( m_mouse.target, to ) );

         if (target)
            target->onMouseOver( toChildPos( target, to ) );
      }

      if (target)
         target->onMouseMove( toChildPos( target, from ), toChildPos( target, to ) );
   }

   m_mouse.target = target;
}

void FsmlElemFrame::onMouseHover( const FSML_POS& pos )
{
   const FsmlAttr* attr;
   if (attr = m_attrs.getAttr( "tip" ))
      if (m_parent) m_parent->onShowTip( attr->getVal( ) );

   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseHover" ), 
                                     "2", &pos ))
   {
      if (m_mouse.target)
         m_mouse.target->onMouseHover( toChildPos( m_mouse.target, pos ) );
   }
}

void FsmlElemFrame::onMouseOut( const FSML_POS& pos )
{
   if (m_parent) m_parent->onHideTip( );

   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseOut" ), 
                                     "2", &pos ))
   {
      if (m_mouse.target)
         m_mouse.target->onMouseOut( toChildPos( m_mouse.target, pos ) );
   } 

   m_mouse.target = 0;
}

void FsmlElemFrame::onMouseDown( const FSML_POS& pos, unsigned int repeat )
{
   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseDown" ), 
                                     "2i", &pos, repeat ))
   {
      if (m_mouse.target)
         m_mouse.target->onMouseDown( toChildPos( m_mouse.target, pos ), repeat );
   }

   m_mouse.flags |= MOUSE_LDOWN;
}

void FsmlElemFrame::onMouseUp( const FSML_POS& pos )
{
   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseUp" ), 
                                     "2", &pos ))
   {
      if (m_mouse.flags & MOUSE_LDOWN && m_script)
         m_script->call( this, m_attrs.getAttrSz( "onMouseClick" ), "2", &pos );

      if (m_mouse.target)
         m_mouse.target->onMouseUp( toChildPos( m_mouse.target, pos ) );
   }
   else if (m_mouse.flags & MOUSE_LDOWN && m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseClick" ), "2", &pos );

   m_mouse.flags &= (~MOUSE_LDOWN);
}


void FsmlElemFrame::onMouseRDown( const FSML_POS& pos, unsigned int repeat )
{
   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseRDown" ), 
                                     "2i", &pos, repeat ))
   {
      if (m_mouse.target)
         m_mouse.target->onMouseRDown( toChildPos( m_mouse.target, pos ), repeat );
   }

   m_mouse.flags |= MOUSE_RDOWN;
}

void FsmlElemFrame::onMouseRUp( const FSML_POS& pos )
{
   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseUp" ), 
                                     "2", &pos ))
   {
      if (m_mouse.flags & MOUSE_RDOWN && m_script)
         m_script->call( this, m_attrs.getAttrSz( "onMouseRClick" ), "2", &pos );

      if (m_mouse.target)
         m_mouse.target->onMouseRUp( toChildPos( m_mouse.target, pos ) );      
   }
   else if (m_mouse.flags & MOUSE_RDOWN && m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseRClick" ), "2", &pos );

   m_mouse.flags &= (~MOUSE_RDOWN);
}

void FsmlElemFrame::onMouseDblClick( const FSML_POS& pos )
{
   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseDblClick" ), 
                                     "2", &pos ))
   {
      if (m_mouse.target)
         m_mouse.target->onMouseDblClick( toChildPos( m_mouse.target, pos ) );
   }

   m_mouse.flags &= (~MOUSE_LDOWN);
}

void FsmlElemFrame::onMouseWheel( const FSML_POS& pos, int direction )
{
   if (!m_script || !m_script->call( this, m_attrs.getAttrSz( "onMouseWheel" ),
                                     "2i", &pos, direction ))
   {
      if (m_mouse.target)
         m_mouse.target->onMouseWheel( pos, direction );
   }
}

void FsmlElemFrame::onInvalidate( FsmlElem* from )
{
   calcRect( &m_rect, &m_origin );
   if (m_parent) m_parent->onInvalidate( this );
}

void FsmlElemFrame::onShowTip( const string& tip )
{
   if (m_parent) m_parent->onShowTip( tip );
}

void FsmlElemFrame::onHideTip( )
{
   if (m_parent) m_parent->onHideTip( );
}

void FsmlElemFrame::calcRect( FSML_RECT* r, FSML_POS* o ) const
{
   FSML_RECT full_rect;
   if (m_children.size( ))
   {
      ElemVector::const_iterator i = m_children.begin( );
      ElemVector::const_iterator end = m_children.end( );

      (*i)->getRect( &full_rect );
      ++i;

      FSML_RECT ch_rect;
      for (; i != end; ++i)
      {
         (*i)->getRect( &ch_rect );
         fsml_rect_union( &full_rect, ch_rect, full_rect );
      }
   }

   /* If the width and height are set, the frame is of a fixed size,
   otherwise the dimensions are fitted around its contents. */
   const FsmlAttr* w_attr = m_attrs.getAttr( "width" );
   const FsmlAttr* h_attr = m_attrs.getAttr( "height" );

   r->x = m_attrs.getAttrInt( "x", 0 );
   r->y = m_attrs.getAttrInt( "y", 0 );

   if (w_attr)
      r->w = CLAMP_INT( w_attr->getValI( ) );
   else
      r->w = full_rect.w;

   if (h_attr)
      r->h = CLAMP_INT( h_attr->getValI( ) );
   else
      r->h = full_rect.h;

   o->x = -full_rect.x;
   o->y = -full_rect.y;

   /* Add the scroll pos to the origin. */
   o->x -= m_attrs.getAttrInt( "scrollX", 0 );
   o->y -= m_attrs.getAttrInt( "scrollY", 0 );
}

FsmlElem* FsmlElemFrame::childFromPoint( const FSML_POS& pos )
{
   ElemVector::reverse_iterator i = m_children.rbegin( );
   const FSML_POS _pos( pos.x - m_origin.x, pos.y - m_origin.y );
   FSML_RECT cr;

   for (; i != m_children.rend( ); ++i)
   {
      (*i)->getRect( &cr );

      if (fsml_pt_in_rect( _pos, cr ))
         if ((*i)->isVisible( ))
            return (*i);
   }

   return 0;
}

inline FSML_POS FsmlElemFrame::toChildPos( const FsmlElem* child, const FSML_POS& pos ) const
{
   FSML_RECT r;
   child->getRect( &r );
   return FSML_POS( (pos.x - m_origin.x)- r.x, (pos.y - m_origin.y) - r.y );
}