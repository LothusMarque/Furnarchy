#include "FsmlElem.h"
#include "FsmlCanvas.h"
#include "FsmlZoo.h"
#include "FsmlScript.h"

enum { MOUSE_LDOWN = 0x1, MOUSE_RDOWN = 0x2 };

#define CLAMP_INT( i ) (((int) i) < 0 ? 0 : (i))

const char* const FsmlElemButton::s_symbol = "button";

void FsmlElemButton::setParent( FsmlElem* parent )
{
   m_parent = parent;
}

FsmlElem* FsmlElemButton::clone( ) const
{
   FsmlElemButton* e = new FsmlElemButton( );

   e->m_zoo         = m_zoo;
   e->m_script      = m_script;
   e->m_visible     = m_visible;
   e->m_rect        = m_rect;
   e->m_origin      = m_origin;
   e->m_attrs       = m_attrs;
   e->m_mouse       = m_mouse;

   return e;
}

void FsmlElemButton::getRect( FSML_RECT* out ) const
{
   *out = m_rect;
}

bool FsmlElemButton::isVisible( ) const
{
   return m_visible;
}

void FsmlElemButton::calcRect( FSML_RECT* r, FSML_POS* o ) const
{
   r->x = m_attrs.getAttrInt( "x", 0 );
   r->y = m_attrs.getAttrInt( "y", 0 );

   /* If the width or height aren't set, use the image's dimensions. */
   const FsmlAttr* w_attr = m_attrs.getAttr( "width" );
   const FsmlAttr* h_attr = m_attrs.getAttr( "height" );

   r->w = w_attr ? w_attr->getValI( ) : 0;
   r->h = h_attr ? h_attr->getValI( ) : 0;

   if (!w_attr || !h_attr)
   {
      const char* src = m_attrs.getAttrSz( "src", "" );
      if (*src && m_zoo)
      {
         FsmlImage* img = m_zoo->getImage( src );

         if (img)
         {
            /* Button images are of multiple of 3 width. */
            if (!w_attr)
               r->w = img->getSize( ).w / 3;
            if (!h_attr)
               r->h = img->getSize( ).h;
         }
      }
   }

   r->w = CLAMP_INT( r->w );
   r->h = CLAMP_INT( r->h );

   o->x = m_attrs.getAttrInt( "scrollX", 0 );
   o->y = m_attrs.getAttrInt( "scrollY", 0 );
}

const FsmlAttr* FsmlElemButton::getAttr( const string& name ) const
{
   return m_attrs.getAttr( name.c_str( ) );
}

void FsmlElemButton::setAttr( const FsmlAttr& attr )
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

FsmlElem* FsmlElemButton::find( const string& id, 
                                FsmlElemType t /* = FSMLELEM_ANY */ )
{
   /* Check ourself. */
   if (fsml_elem_same( t, FSMLELEM_BUTTON ))
      if (m_attrs.getAttrSz( "id", "" ) == id)
         return this;

   return 0;
}

const FsmlElem* FsmlElemButton::find( const string& id, 
                                      FsmlElemType t /* = FSMLELEM_ANY */ ) const
{
   /* Check ourself. */
   if (fsml_elem_same( t, FSMLELEM_BUTTON ))
      if (m_attrs.getAttrSz( "id", "" ) == id)
         return this;
   
   return 0;
}

void FsmlElemButton::draw( FsmlCanvas* canvas )
{
   if (m_visible)
   {
      FsmlImage* img = 0;
      const char* src = m_attrs.getAttrSz( "src", "" );

      if (*src && m_zoo)
         img = m_zoo->getImage( src );
      if (!img)
         return;

      FSML_POS src_pos = m_origin;
      {
         switch (getButtonState( ))
         {
         default:
         case BUTTON_UP:
            src_pos.x += 0;
            break;
         case BUTTON_OVER:
            src_pos.x += m_rect.w;
            break;
         case BUTTON_DOWN:
            src_pos.x += 2 * m_rect.w;
            break;
         }
      }

      canvas->drawImage( *img, src_pos, FSML_POS( 0, 0 ),
                         &FSML_SIZE( m_rect.w, m_rect.h ) );
   }
}

void FsmlElemButton::onSelectScript( FsmlScript* script )
{
   m_script = script;
}

void FsmlElemButton::onTick( unsigned int dt )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onTick" ), "i", dt );
}

void FsmlElemButton::onModState( unsigned int mod, long state )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onModState" ), "il", mod, state );
}

void FsmlElemButton::onMouseOver( const FSML_POS& pos )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseOver" ), "2", &pos );

   setButtonState( BUTTON_OVER );
}

void FsmlElemButton::onMouseMove( const FSML_POS& from, const FSML_POS& to )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseMove" ), "22", &from, &to );
}

void FsmlElemButton::onMouseHover( const FSML_POS& pos )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseHover" ), "2", &pos );

   const FsmlAttr* attr;
   if (attr = m_attrs.getAttr( "tip" ))
      if (m_parent) m_parent->onShowTip( attr->getVal( ) );
}

void FsmlElemButton::onMouseOut( const FSML_POS& pos )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseOut" ), "2", &pos );

   setButtonState( BUTTON_UP );
   if (m_parent) m_parent->onHideTip( );
}

void FsmlElemButton::onMouseDown( const FSML_POS& pos, unsigned int repeat )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseDown" ), "2i", &pos, repeat );

   setButtonState( BUTTON_DOWN );

   m_mouse.flags |= MOUSE_LDOWN;
}

void FsmlElemButton::onMouseUp( const FSML_POS& pos )
{
   if (m_script)
   {
      m_script->call( this, m_attrs.getAttrSz( "onMouseUp" ), "2", &pos );
      if (m_mouse.flags & MOUSE_LDOWN)
         m_script->call( this, m_attrs.getAttrSz( "onMouseClick" ), "2", &pos );
   }

   setButtonState( BUTTON_OVER );

   m_mouse.flags &= (~MOUSE_LDOWN);
}

void FsmlElemButton::onMouseRDown( const FSML_POS& pos, unsigned int repeat )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseRDown" ), "2i", &pos, repeat );

   m_mouse.flags |= MOUSE_RDOWN;
}

void FsmlElemButton::onMouseRUp( const FSML_POS& pos )
{
   if (m_script)
   {
      m_script->call( this, m_attrs.getAttrSz( "onMouseRUp" ), "2", &pos );
      if (m_mouse.flags & MOUSE_RDOWN)
         m_script->call( this, m_attrs.getAttrSz( "onMouseRClick" ), "2", &pos );
   }

   m_mouse.flags &= (~MOUSE_RDOWN);
}

void FsmlElemButton::onMouseDblClick( const FSML_POS& pos )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseDblClick" ), "2", &pos );
}

void FsmlElemButton::onMouseWheel( const FSML_POS& pos, int direction )
{
   if (m_script)
      m_script->call( this, m_attrs.getAttrSz( "onMouseWheel" ),
                      "2i", &pos, direction );
}

inline void FsmlElemButton::setButtonState( FsmlElemButton::Button_State state )
{
   switch (state)
   {
   default:
      m_attrs.setAttr( FsmlAttr( "state", "up" ) );
      break;
   case BUTTON_DOWN:
      m_attrs.setAttr( FsmlAttr( "state", "down" ) );
      break;
   case BUTTON_OVER:
      m_attrs.setAttr( FsmlAttr( "state", "over" ) );
      break;
   }

   if (m_parent) m_parent->onInvalidate( this );
}

inline FsmlElemButton::Button_State FsmlElemButton::getButtonState( ) const
{
   const FsmlAttr* attr = m_attrs.getAttr( "state" );

   if (attr)
   {
      if (attr->getVal( ) == "down")
         return BUTTON_DOWN;
      else if (attr->getVal( ) == "over")
         return BUTTON_OVER;
   }

   return BUTTON_UP;
}