#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINDOWS 0x0410
#define WINVER 0x0410

#include "FsmlElem.h"
#include "FsmlCanvas.h"
#include "FsmlZoo.h"
#include "FsmlScript.h"
#include "tinyxml/tinyxml.h"
#include "../furnarchycore/furn2.h"
#include <commctrl.h>

#ifndef GET_X_LPARAM
   #define GET_X_LPARAM( lparam ) ((int) (short) LOWORD( (lparam) ))
#endif
#ifndef GET_Y_LPARAM
   #define GET_Y_LPARAM( lparam ) ((int) (short) HIWORD( (lparam) ))
#endif
#ifndef WHEEL_DELTA
   #define WHEEL_DELTA 120
#endif

using namespace furn2;

enum { MOUSE_RPS = 8, MOUSE_HOVERTIME = 500,
       MOUSE_LDOWN = 0x1, MOUSE_RDOWN = 0x2 };

//////////////////////////////////////////////////////////////////////////

void xml_build_children( FsmlElem* parent, const TiXmlElement* node,
                         FsmlZoo* zoo );

//////////////////////////////////////////////////////////////////////////

FsmlElemSkin::FsmlElemSkin( HINSTANCE inst, HWND wnd, const string& name )
   : m_inst( inst ), m_wnd( wnd )
{ 
   const string prog_root = f2_root( ) + ("\\skins\\" + name);
   const string doc_root  = f2_docroot( ) + ("\\skins\\" + name);

   /* Create a folder hierarchy in the document root. */
   f2_createdirs( doc_root.c_str( ) );

   /* Find the skin.xml file. */
   // Use the per-user folder if it exists.
   string xml_file = doc_root + "\\skin.xml";
   {
      const DWORD atts = GetFileAttributes( xml_file.c_str( ) );
      if (atts == INVALID_FILE_ATTRIBUTES)
         // Fall back to program folder.
         xml_file = prog_root + "\\skin.xml";
   }

   /* Find the skin.gm file. */
   string script_file = doc_root + "\\skin.gm";
   {
      const DWORD atts = GetFileAttributes( script_file.c_str( ) );
      if (atts == INVALID_FILE_ATTRIBUTES)
         // Fall back to program folder.
         script_file = prog_root + "\\skin.gm";
   }

   /* Find the skin.db file. */
   string db_file  = doc_root + "\\skin.db"; // skin.db is always in the doc root.
   // If the db file doesn't exist, copy it from the program folder.
   CopyFile( (prog_root + "\\skin.db").c_str( ), db_file.c_str( ), TRUE );
   
   {
      vector<string> paths;
      paths.push_back( doc_root );
      paths.push_back( prog_root );
      m_zoo = new FsmlZoo( paths.begin( ), paths.end( ) );
   }
   m_tipctrl = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, 0,
                               WS_OVERLAPPED | TTS_NOPREFIX | TTS_ALWAYSTIP,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               0, // Putting an hParent crashes my win98 vm?
                               0, 0, 0 );
   
   SetWindowPos( m_tipctrl, HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
   SendMessage( m_tipctrl, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
   SendMessage( m_tipctrl, TTM_SETDELAYTIME, TTDT_AUTOPOP, 1000000 );
   SendMessage( m_tipctrl, TTM_SETDELAYTIME, TTDT_RESHOW, 0 );
   // Hold off on creating the script until all the children have been created.
   m_script  = NULL;

   /* Read in the skin.xml file. */
   TiXmlDocument doc( xml_file.c_str( ) );

   if (!doc.LoadFile( ))
   {
      delete m_zoo;
      DestroyWindow( m_tipctrl );
      throw runtime_error( "Couldn't load " + xml_file );
   }

   if (doc.Error( ))
      f2_errspeak( "Skin xml was loaded but with errors." );

   const TiXmlElement* xml_root_elem = doc.FirstChildElement( "skin" );

   if (!xml_root_elem)
   {
      delete m_zoo;
      DestroyWindow( m_tipctrl );
      throw runtime_error( "No root skin element in skin.xml file." );
      return;
   }

   /* Start building the skin. */
   /* Attributes. */
   for (const TiXmlAttribute* att = xml_root_elem->FirstAttribute( ); 
        att; att = att->Next( ))
   {
      m_attrs.setAttr( FsmlAttr( att->Name( ), att->Value( ) ) );
   }

   /* Children. */
   for (const TiXmlElement* node = xml_root_elem->FirstChildElement( ); 
        node; node = node->NextSiblingElement( ))
   {
      xml_build_children( this, node, m_zoo );
   }

   // Now create the script.
   m_script = new FsmlScript( this, script_file, db_file );

   for (ComponentVector::iterator child = m_children.begin( );
        child != m_children.end( ); ++child)
   {
      (*child)->elem->onSelectScript( m_script );
   }

   // Call the onLoad handler.
   m_script->call( this, m_attrs.getAttrSz( "onLoad", 0 ) );
}

FsmlElemSkin::~FsmlElemSkin( )
{
   for (ComponentVector::iterator child = m_children.begin( );
        child != m_children.end( ); ++child)
   {
      delete (*child);
   }

   delete m_zoo;
   delete m_script;
   DestroyWindow( m_tipctrl );
}

void FsmlElemSkin::getRect( FSML_RECT* out ) const
{
   if (!m_children.size( ))
   {
      *out = FSML_RECT( );
      return;
   }
   
   ComponentVector::const_iterator child = m_children.begin( );
   (*child)->elem->getRect( out );
   ++child;

   for (; child != m_children.end( ); ++child)
   {
      fsml_rect_union( out, (*child)->rect, *out );
   }
}

bool FsmlElemSkin::isVisible( ) const
{
   if (!strcmp( m_attrs.getAttrSz( "visible", "yes" ), "no" ))
      return false;
   return true;
}

const FsmlAttr* FsmlElemSkin::getAttr( const string& name ) const
{
   return m_attrs.getAttr( name.c_str( ) );
}

void FsmlElemSkin::setAttr( const FsmlAttr& attr )
{
   m_attrs.setAttr( attr );
}

bool FsmlElemSkin::addChild( FsmlElem* child, FsmlElem* before /* = 0  */ )
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

   FsmlElemSkin::COMPONENT* comp = new FsmlElemSkin::COMPONENT;
   comp->elem = child;
   child->getRect( &comp->rect );

   /* Insert before an element? */
   if (before)
   {
      // Find the element.
      ComponentVector::iterator i = m_children.begin( );
      for (; i != m_children.end( ); ++i)
      {
         if ((*i)->elem == before)
            break;
      }

      if (i != m_children.end( ))
      {
         m_children.insert( i, comp );
         child->setParent( this );
         return true;
      }
   }

   /* Otherwise just push to the back. */
   m_children.push_back( comp );
   child->setParent( this );
   return true;
}

FsmlElem* FsmlElemSkin::find( const string& id, 
                              FsmlElemType t /* = FSMLELEM_ANY */ )
{
   /* Check ourself. */
   if (fsml_elem_same( FSMLELEM_SKIN, t ))
      if (m_attrs.getAttrSz( "id", "" ) == id)
         return this;
   
   /* Check children. */
   FsmlElem* f = 0;
   for (ComponentVector::iterator child = m_children.begin( ); 
        child != m_children.end( ); ++child)
   {
      if (f = (*child)->elem->find( id, t ))
         return f;
   }

   return 0;
}

const FsmlElem* FsmlElemSkin::find( const string& id, 
                                    FsmlElemType t /* = FSMLELEM_ANY */ ) const
{
   /* Check ourself. */
   if (fsml_elem_same( FSMLELEM_SKIN, t ))
      if (m_attrs.getAttrSz( "id", "" ) == id)
         return this;

   /* Check children. */
   const FsmlElem* f = 0;
   for (ComponentVector::const_iterator child = m_children.begin( ); 
        child != m_children.end( ); ++child)
   {
      if (f = (*child)->elem->find( id, t ))
         return f;
   }

   return 0;
}

bool FsmlElemSkin::construct( const string& id )
{
   for (ComponentVector::const_iterator child = m_children.begin( ); 
        child != m_children.end( ); ++child)
   {
      if ((*child)->elem->construct( id ))
         return true;
   }

   return false;
}

void FsmlElemSkin::draw( HDC dc )
{
   if (!strcmp( m_attrs.getAttrSz( "visible", "yes" ), "no" ))
      return;

   DIBSECTION dib_info;
   if (!GetObjectA( GetCurrentObject( dc, OBJ_BITMAP ), sizeof( DIBSECTION ), &dib_info ))
      return;

   if (m_children.size( ))
      GdiFlush( );

   
   FsmlImagePtr canvas_img = new FsmlImage( dib_info.dsBm.bmBits, 
                                            dib_info.dsBm.bmWidth, dib_info.dsBm.bmHeight,
                                            dib_info.dsBm.bmBitsPixel,
                                            dib_info.dsBm.bmWidthBytes );
   FsmlCanvas canvas( canvas_img );

   for (ComponentVector::const_iterator child = m_children.begin( ); 
        child != m_children.end( ); ++child)
   {
      FsmlElemSkin::COMPONENT* comp = (*child);

      if (comp->elem->isVisible( ))
      {
         canvas.pushViewport( &comp->rect );
         comp->elem->draw( &canvas );
         canvas.popViewport( );
      }
   }
}

void FsmlElemSkin::onModState( unsigned int mod, long state )
{
   if (!m_script->call( this, m_attrs.getAttrSz( "onModState" ), 
                        "il", mod, state ))
   {
      for (ComponentVector::iterator child = m_children.begin( );
           child != m_children.end( ); ++child)
         (*child)->elem->onModState( mod, state );
   }
}

void FsmlElemSkin::onInvalidate( FsmlElem* from )
{
   /* Find the kid. */
   for (ComponentVector::const_iterator child = m_children.begin( ); 
        child != m_children.end( ); ++child)
   {
      FsmlElemSkin::COMPONENT* comp = (*child);
      if (comp->elem == from)
      {
         float x_scale, y_scale;
         getScale( &x_scale, &y_scale );

         /* Invalidate the old rect. */
         RECT r;
         r.left   = (LONG) (comp->rect.x * x_scale);
         r.top    = (LONG) (comp->rect.y * y_scale);
         r.right  = (LONG) ((comp->rect.x + comp->rect.w) * x_scale);
         r.bottom = (LONG) ((comp->rect.y + comp->rect.h) * y_scale);
         InvalidateRect( m_wnd, &r, FALSE );

         /* Recalculate the rect. */
         comp->elem->getRect( &comp->rect );

         /* Invalidate the new rect. */
         r.left   = (LONG) (comp->rect.x * x_scale);
         r.top    = (LONG) (comp->rect.y * y_scale);
         r.right  = (LONG) ((comp->rect.x + comp->rect.w) * x_scale);
         r.bottom = (LONG) ((comp->rect.y + comp->rect.h) * y_scale);
         InvalidateRect( m_wnd, &r, FALSE );

         return;
      }
   }
}

void FsmlElemSkin::onShowTip( const string& tip )
{
   TOOLINFO ti;
   ti.cbSize      = sizeof( ti );
   ti.uFlags      = 0;
   ti.hinst       = 0;
   ti.hwnd        = m_wnd;
   ti.lParam      = 0;
   ti.uId         = 0xB2A391FD;
   ti.lpszText    = strdup( tip.c_str( ) );
   ti.rect.left   = m_mouse.pos.x;
   ti.rect.right  = m_mouse.pos.x + 1;
   ti.rect.top    = m_mouse.pos.y;
   ti.rect.bottom = m_mouse.pos.y + 1;

   if (!SendMessage( m_tipctrl, TTM_GETTOOLCOUNT, 0, 0 ))
      SendMessage( m_tipctrl, TTM_ADDTOOL, 0, (LPARAM) &ti );
   else
      SendMessage( m_tipctrl, TTM_SETTOOLINFO, 0, (LPARAM) &ti );

   free( ti.lpszText );

   SendMessage( m_tipctrl, TTM_ACTIVATE, TRUE, 0 );

   MSG mousemove;
   mousemove.hwnd    = m_wnd;
   mousemove.message = WM_MOUSEMOVE;
   mousemove.time    = GetMessageTime( );
   mousemove.wParam  = 0;
   mousemove.pt.x = m_mouse.pos.x;
   mousemove.pt.y = m_mouse.pos.y;
   mousemove.lParam  = ((short) mousemove.pt.x) | (((short) mousemove.pt.y) << 16);
   ClientToScreen( m_wnd, &mousemove.pt );

   SendMessage( m_tipctrl, TTM_RELAYEVENT, 0, (LPARAM) &mousemove );
}

void FsmlElemSkin::onHideTip( )
{
   SendMessage( m_tipctrl, TTM_POP, 0, 0 );
   SendMessage( m_tipctrl, TTM_ACTIVATE, FALSE, 0 );
}

void FsmlElemSkin::onTick( unsigned int dt )
{
   m_script->tick( dt );

   if (!m_script->call( this, m_attrs.getAttrSz( "onTick" ), "i", dt ))
   {
      /* Bubble down the event. */
      for (ComponentVector::iterator i = m_children.begin( );
           i != m_children.end( ); ++i)
      {
         (*i)->elem->onTick( dt );
      }
   }

   /* Generate mouse repeats. */
   if (m_mouse.flags & (MOUSE_LDOWN | MOUSE_RDOWN))
   {
      if (dt >= m_mouse.repeat_unwind)
      {
         m_mouse.repeat_unwind = 1000 / MOUSE_RPS;

         if (m_mouse.flags & MOUSE_LDOWN)
         {
            ++m_mouse.repcount_lbutton;
            if (!m_script->call( this, m_attrs.getAttrSz( "onMouseDown" ), 
                                 "2i", &m_mouse.pos, m_mouse.repcount_lbutton ))
            {
               if (m_mouse.target)
                  m_mouse.target->onMouseDown( toChildPos( m_mouse.target, m_mouse.pos ),
                                               m_mouse.repcount_lbutton );
            }
         }

         if (m_mouse.flags & MOUSE_RDOWN)
         {
            ++m_mouse.repcount_rbutton;
            if (!m_script->call( this, m_attrs.getAttrSz( "onMouseRDown" ),
                                 "2i", &m_mouse.pos, m_mouse.repcount_rbutton ))
            {
               if (m_mouse.target)
               m_mouse.target->onMouseRDown( toChildPos( m_mouse.target, m_mouse.pos), 
                                             m_mouse.repcount_rbutton );
            }
         }
      }
      else
         m_mouse.repeat_unwind -= dt;
   }
   /* Generate hover events. */
   else if (!m_mouse.flags && m_mouse.hover_time) 
   {
      if (dt >= m_mouse.hover_time)
      {
         m_mouse.hover_time = 0;

         if (!m_script->call( this, m_attrs.getAttrSz( "onMouseHover" ),
                              "2", &m_mouse.pos ))
         {
            if (m_mouse.target)
               m_mouse.target->onMouseHover( toChildPos( m_mouse.target, m_mouse.pos ) );
         }
      }
      else
         m_mouse.hover_time -= dt;
   }
}

bool FsmlElemSkin::onWndProc( UINT msg, WPARAM wparam, LPARAM lparam )
{   
   switch (msg)
   {
   default:
      return false;

   case WM_MOUSEMOVE:
      {         
         FSML_POS pt( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) );
         normalize( &pt );

         FsmlElemPtr target = childFromPoint( pt );

         if (!m_script->call( this, m_attrs.getAttrSz( "onMouseMove" ), 
                              "22", &m_mouse.pos, &pt ))
         {
            if (m_mouse.target != target)
            {
               if (m_mouse.target)
                     m_mouse.target->onMouseOut( toChildPos( m_mouse.target, pt ) );

               if (target)
                  target->onMouseOver( toChildPos( target, pt ) );
            }

            if (target)
               target->onMouseMove( toChildPos( target, m_mouse.pos ), 
                                    toChildPos( target, pt ) );
         }

         m_mouse.pos = pt;
         m_mouse.target = target;
         m_mouse.hover_time = MOUSE_HOVERTIME;

         if (m_mouse.target)
         {
            SetCursor( LoadCursor( 0, IDC_ARROW ) );
            return true;
         }
         return false;
      }

   case WM_LBUTTONDOWN:
      {
         FSML_POS pt( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) );
         normalize( &pt );

         if (pt != m_mouse.pos)
            onWndProc( WM_MOUSEMOVE, wparam, lparam );

         if (!m_script->call( this, m_attrs.getAttrSz( "onMouseDown" ), 
                              "2", &pt ))
         {
            if (m_mouse.target)
               m_mouse.target->onMouseDown( toChildPos( m_mouse.target, pt ), 0 );
         }

         m_mouse.flags |= MOUSE_LDOWN;
         m_mouse.hover_time = MOUSE_HOVERTIME;
         m_mouse.repcount_lbutton = 0;
         return (!!m_mouse.target);
      }

   case WM_LBUTTONUP:
      {
         FSML_POS pt( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) );
         normalize( &pt );

         if (pt != m_mouse.pos)
            onWndProc( WM_MOUSEMOVE, wparam, lparam );

         if (!m_script->call( this, m_attrs.getAttrSz( "onMouseUp" ), "2", &pt ))
         {
            if (m_mouse.flags & MOUSE_LDOWN)
               m_script->call( this, m_attrs.getAttrSz( "onMouseClick" ), 
                               "2", &pt );

            if (m_mouse.target)
               m_mouse.target->onMouseUp( toChildPos( m_mouse.target, pt ) );
         }
         else
            if (m_mouse.flags & MOUSE_LDOWN)
               m_script->call( this, m_attrs.getAttrSz( "onMouseClick" ), 
                               "2", &pt );

         m_mouse.flags &= (~MOUSE_LDOWN);
         m_mouse.hover_time = MOUSE_HOVERTIME;
         return (!!m_mouse.target);
      }

   case WM_LBUTTONDBLCLK:
      {
         FSML_POS pt( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) );
         normalize( &pt );

         if (pt != m_mouse.pos)
            onWndProc( WM_MOUSEMOVE, 0, lparam );

         if (!m_script->call( this, m_attrs.getAttrSz( "onMouseDblClick" ), 
                              "2", &pt ))
         {
            if (m_mouse.target)
               m_mouse.target->onMouseDblClick( toChildPos( m_mouse.target, pt ) );
         }
         
         m_mouse.flags &= (~MOUSE_LDOWN);
         m_mouse.hover_time = MOUSE_HOVERTIME;
         return (!!m_mouse.target);
      }

   case WM_RBUTTONDOWN:
      {
         FSML_POS pt( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) );
         normalize( &pt );

         if (pt != m_mouse.pos)
            onWndProc( WM_MOUSEMOVE, wparam, lparam );

         if (!m_script->call( this, m_attrs.getAttrSz( "onMouseRDown" ), 
                              "2", &pt ))
         {
            if (m_mouse.target)
               m_mouse.target->onMouseRDown( toChildPos( m_mouse.target, pt ), 0 );
         }

         m_mouse.flags |= MOUSE_RDOWN;
         m_mouse.hover_time = MOUSE_HOVERTIME;
         m_mouse.repcount_rbutton = 0;
         return (!!m_mouse.target);
      }

   case WM_RBUTTONUP:
      {
         FSML_POS pt( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) );
         normalize( &pt );

         if (pt != m_mouse.pos)
            onWndProc( WM_MOUSEMOVE, wparam, lparam );

         if (!m_script->call( this, m_attrs.getAttrSz( "onMouseRUp" ), "2", &pt ))
         {
            if (m_mouse.flags & MOUSE_RDOWN)
               m_script->call( this, m_attrs.getAttrSz( "onMouseRClick" ), 
                               "2", &pt );

            if (m_mouse.target)
               m_mouse.target->onMouseRUp( toChildPos( m_mouse.target, pt ) );
         }
         else
            if (m_mouse.flags & MOUSE_RDOWN)
               m_script->call( this, m_attrs.getAttrSz( "onMouseRClick" ), 
                               "2", &pt );

         m_mouse.flags &= (~MOUSE_RDOWN);
         m_mouse.hover_time = MOUSE_HOVERTIME;
         return (!!m_mouse.target);
      }

   case WM_MOUSEWHEEL:
      {
         FSML_POS pt( GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) );
         ScreenToClient( m_wnd, (POINT*) &pt );
         normalize( &pt );

         if (pt != m_mouse.pos)
            onWndProc( WM_MOUSEMOVE, wparam, lparam );

         m_mouse.wheel_delta += (int) (signed short) HIWORD( wparam );
         const int dir = m_mouse.wheel_delta > 0 ? 1 : -1;

         while (m_mouse.wheel_delta / WHEEL_DELTA)
         {
            if (!m_script->call( this, m_attrs.getAttrSz( "onMouseWheel" ),
                                 "2i", &pt, dir ))
            {
               if (m_mouse.target)
                  m_mouse.target->onMouseWheel( toChildPos( m_mouse.target, pt ), dir );
            }

            m_mouse.wheel_delta -= dir > 0 ? WHEEL_DELTA : -WHEEL_DELTA;
         }
         
         m_mouse.hover_time = MOUSE_HOVERTIME;
         return (!!m_mouse.target);
      }
   }

   return false;
}

FsmlElem* FsmlElemSkin::childFromPoint( const FSML_POS& pt )
{
   for (ComponentVector::reverse_iterator i = m_children.rbegin( );
        i != m_children.rend( ); ++i)
   {
      if (fsml_pt_in_rect( pt, (*i)->rect ))
      {
         if ((*i)->elem->isVisible( ))
            return (*i)->elem;
      }
   }
   return 0;
}

inline FSML_POS FsmlElemSkin::toChildPos( const FsmlElem* child, const FSML_POS& pos ) const
{
   FSML_RECT r;
   child->getRect( &r );
   return FSML_POS( pos.x - r.x, pos.y - r.y );
}

void FsmlElemSkin::normalize( FSML_POS* pt ) const
{
   if (m_wnd)
   {
      RECT r;
      GetClientRect( m_wnd, &r );
      pt->x = (int) (pt->x * ( 640.0f / r.right ));
      pt->y = (int) (pt->y * ( 480.0f / r.bottom ));
   }
}

void FsmlElemSkin::getScale( float* x, float* y ) const
{
   if (m_wnd)
   {
      RECT r;
      GetClientRect( m_wnd, &r );
      *x = r.right / 640.0f;
      *y = r.bottom / 480.0f;
   }
   else
   {
      *x = 1.0f;
      *y = 1.0f;
   }
}

//////////////////////////////////////////////////////////////////////////

void xml_build_children( FsmlElem* parent, const TiXmlElement* node,
                         FsmlZoo* zoo )
{
   if (node)
   {
      FsmlElemPtr child;
      {
         const char* name = node->Value( );

         if (!strcmp( FsmlElemImage::s_symbol, name ))
            child = new FsmlElemImage( zoo );
         else if (!strcmp( FsmlElemButton::s_symbol, name ))
            child = new FsmlElemButton( zoo );
         else if (!strcmp( FsmlElemFrame::s_symbol, name ))
            child = new FsmlElemFrame( );
         else if (!strcmp( FsmlElemTemplate::s_symbol, name ))
            child = new FsmlElemTemplate( );
      }

      if (child)
      {
         /* Attributes. */
         for (const TiXmlAttribute* att = node->FirstAttribute( ); 
              att; att = att->Next( ))
         {
            child->setAttr( FsmlAttr( att->Name( ), att->Value( ) ) );
         }

         /* Recurse into children. */
         for (node = node->FirstChildElement( ); 
              node; node = node->NextSiblingElement( ))
         {
            xml_build_children( child, node, zoo );
         }

         parent->addChild( child );
      }
   }
}