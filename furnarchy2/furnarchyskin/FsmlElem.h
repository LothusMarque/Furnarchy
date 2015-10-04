#ifndef _FSML_ELEM_H__
#define _FSML_ELEM_H__

enum FsmlElemType
{
   FSMLELEM_ANY,
   FSMLELEM_SKIN,
   FSMLELEM_STATE,
   FSMLELEM_FRAME,
   FSMLELEM_BUTTON,
   FSMLELEM_IMAGE,
   FSMLELEM_TEMPLATE,
};

inline bool fsml_elem_same( FsmlElemType a, FsmlElemType b )
{
   if (a == FSMLELEM_ANY || b == FSMLELEM_ANY)
      return true;
   return (a == b);
}

#include "fsml_common.h"
#include "FsmlAttr.h"

class FsmlScript;
class FsmlElem;
class FsmlZoo;
class FsmlCanvas;

typedef ref_ptr<FsmlElem> FsmlElemPtr;
typedef c_ref_ptr<FsmlElem> FsmlElemCPtr;

/************************************************************************/
/* FsmlElem                                                             */
/************************************************************************/

/** Abstract Base FSML Element type */
class FsmlElem : public RefCount
{
   friend class FsmlElemImage;
   friend class FsmlElemButton;
   friend class FsmlElemFrame;
   friend class FsmlElemTemplate;
   friend class FsmlElemSkin;

protected:
   FsmlElem( ) { };
   FsmlElem( const FsmlElem& r ) { };
   virtual  ~FsmlElem( ) { };

public:
   virtual FsmlElemType    getType(  ) const = 0;
   virtual void            getRect( FSML_RECT* out ) const 
                           { *out = FSML_RECT( ); }
   virtual const FsmlAttr* getAttr( const string& name ) const { return 0; }
   virtual void            setAttr( const FsmlAttr& attr ) { }
   virtual FsmlElem*       find( const string& id, 
                                 FsmlElemType t = FSMLELEM_ANY ) 
                           { return 0; }
   virtual const FsmlElem* find( const string& id,
                                 FsmlElemType t = FSMLELEM_ANY ) const
                           { return 0; }
   virtual bool            construct( const string& id )
                           { return false; }
   virtual bool            addChild( FsmlElem* child, FsmlElem* before = 0 )
                           { return false; }
   virtual FsmlElem*       clone( ) const { return 0; }
   virtual bool            isVisible( ) const { return false; }

protected:
   virtual void            setParent( FsmlElem* parent ) { };
   
   /* Events that bubble down. */
   virtual void            draw( FsmlCanvas* canvas ) { }
   virtual void            onSelectScript( FsmlScript* script ) { }
   virtual void            onTick( unsigned int dt ) { }
   virtual void            onModState( unsigned int mod, long state ) { }
   virtual void            onMouseOver( const FSML_POS& pos ) { }
   virtual void            onMouseMove( const FSML_POS& from, const FSML_POS& to  ) { }
   virtual void            onMouseHover( const FSML_POS& pos ) { }
   virtual void            onMouseOut( const FSML_POS& pos ) { }
   virtual void            onMouseDown( const FSML_POS& pos, unsigned int repeat ) { }
   virtual void            onMouseUp( const FSML_POS& pos ) { }
   virtual void            onMouseRDown( const FSML_POS& pos, unsigned int repeat ) { }
   virtual void            onMouseRUp( const FSML_POS& pos ) { }
   virtual void            onMouseDblClick( const FSML_POS& pos ) { }
   virtual void            onMouseWheel( const FSML_POS& pos, int direction ) { }

   /* Events that bubble up. */
   virtual void            onInvalidate( FsmlElem* from ) { }
   virtual void            onShowTip( const string& tip ) { }
   virtual void            onHideTip( ) { }
};

/************************************************************************/
/* FsmlElemImage                                                        */
/************************************************************************/

class FsmlElemImage : public FsmlElem
{
public:
   static const char* const s_symbol;
public:
   FsmlElemImage( FsmlZoo* zoo );
private:
   FsmlElemImage( ) { }
   FsmlElemImage( const FsmlElemImage& r ) { }
   virtual ~FsmlElemImage( ) { }

public:
   virtual FsmlElemType    getType( ) const
                           { return FSMLELEM_IMAGE; }
   virtual void            getRect( FSML_RECT* out ) const;
   virtual const FsmlAttr* getAttr( const string& name ) const;
   virtual void            setAttr( const FsmlAttr& attr );
   virtual FsmlElem*       find( const string& id, 
                                 FsmlElemType t = FSMLELEM_ANY );
   virtual const FsmlElem* find( const string& id,
                                 FsmlElemType t = FSMLELEM_ANY ) const;
   virtual FsmlElem*       clone( ) const;
   virtual bool            isVisible( ) const;

private:
   virtual void            setParent( FsmlElem* parent );
   virtual void            draw( FsmlCanvas* canvas );

   virtual void            onSelectScript( FsmlScript* script );
   virtual void            onTick( unsigned int dt );
   virtual void            onModState( unsigned int mod, long state );
   virtual void            onMouseOver( const FSML_POS& pos );
   virtual void            onMouseMove( const FSML_POS& from, const FSML_POS& to );
   virtual void            onMouseHover( const FSML_POS& pos );
   virtual void            onMouseOut( const FSML_POS& pos );
   virtual void            onMouseDown( const FSML_POS& pos, unsigned int repeat );
   virtual void            onMouseUp( const FSML_POS& pos );
   virtual void            onMouseRDown( const FSML_POS& pos, unsigned int repeat );
   virtual void            onMouseRUp( const FSML_POS& pos );
   virtual void            onMouseDblClick( const FSML_POS& pos );
   virtual void            onMouseWheel( const FSML_POS& pos, int direction );
   
   void calcRect( FSML_RECT* r, FSML_POS* o ) const;

private:
   FsmlElem*   m_parent;
   FsmlZoo*    m_zoo;
   FsmlScript* m_script;
   bool        m_visible;
   FSML_RECT   m_rect;
   FSML_POS    m_origin;
   FsmlAttrMap m_attrs;

   struct _MOUSE
   {
      unsigned long flags;
      _MOUSE( ) : flags( 0 ) { }
   }           m_mouse;
};

inline FsmlElemImage::FsmlElemImage( FsmlZoo* zoo )
   : m_zoo( zoo ), m_script( NULL ), m_visible( true ), m_parent( NULL )
{
   
}

/************************************************************************/
/* FsmlElemFrame                                                        */
/************************************************************************/

class FsmlElemFrame : public FsmlElem
{
public:
   static const char* const s_symbol;
private:
   typedef std::vector<FsmlElemPtr> ElemVector;
public:
   FsmlElemFrame( );
private:
   FsmlElemFrame( const FsmlElemFrame& r ) { }
   virtual ~FsmlElemFrame( ) { }

public:
   virtual FsmlElemType    getType( ) const
                           { return FSMLELEM_FRAME; }
   virtual void            getRect( FSML_RECT* out ) const;
   virtual const FsmlAttr* getAttr( const string& name ) const;
   virtual void            setAttr( const FsmlAttr& attr );
   virtual FsmlElem*       find( const string& id,
                                 FsmlElemType t = FSMLELEM_ANY );
   virtual const FsmlElem* find( const string& id,
                                 FsmlElemType t = FSMLELEM_ANY ) const;
   virtual unsigned int    getNumChildren( ) const;
   virtual bool            construct( const string& id );
   virtual bool            addChild( FsmlElem* child, FsmlElem* before = 0 );
   virtual FsmlElem*       clone( ) const;
   virtual bool            isVisible( ) const;

private:
   virtual void            setParent( FsmlElem* parent );
   virtual void            draw( FsmlCanvas* canvas );

   virtual void            onSelectScript( FsmlScript* script );
   virtual void            onTick( unsigned int dt );
   virtual void            onModState( unsigned int mod, long state );
   virtual void            onMouseOver( const FSML_POS& pos );
   virtual void            onMouseMove( const FSML_POS& from, const FSML_POS& to );
   virtual void            onMouseHover( const FSML_POS& pos );
   virtual void            onMouseOut( const FSML_POS& pos );
   virtual void            onMouseDown( const FSML_POS& pos, unsigned int repeat );
   virtual void            onMouseUp( const FSML_POS& pos );
   virtual void            onMouseRDown( const FSML_POS& pos, unsigned int repeat );
   virtual void            onMouseRUp( const FSML_POS& pos );
   virtual void            onMouseDblClick( const FSML_POS& pos );
   virtual void            onMouseWheel( const FSML_POS& pos, int direction );

   virtual void            onInvalidate( FsmlElem* from );
   virtual void            onShowTip( const string& tip );
   virtual void            onHideTip( );
   
   FsmlElem* childFromPoint( const FSML_POS& pt );
   FSML_POS  toChildPos( const FsmlElem* child, const FSML_POS& pos ) const;
   void      calcRect( FSML_RECT* r, FSML_POS* o ) const;

private:
   FsmlElem*   m_parent;
   FsmlScript* m_script;
   bool        m_visible;
   FSML_RECT   m_rect;
   FSML_POS    m_origin;
   FsmlAttrMap m_attrs;
   ElemVector  m_children;

   struct _MOUSE
   {
      unsigned long flags;
      FsmlElemPtr   target;
      _MOUSE( ) : flags( 0 ) { }
   }           m_mouse;
};

inline FsmlElemFrame::FsmlElemFrame( )
               : m_script( NULL ), m_visible( true ), m_parent( NULL )
{

}

/************************************************************************/
/* FsmlElemButton                                                       */
/************************************************************************/

class FsmlElemButton : public FsmlElem
{
public:
   static const char* const s_symbol;

   enum Button_State
   {
      BUTTON_UP = 0, BUTTON_DOWN = 1, BUTTON_OVER = 2,
   };

public:
   FsmlElemButton( FsmlZoo* zoo );
private:
   FsmlElemButton( ) { }
   FsmlElemButton( const FsmlElemButton& r ) { }
   virtual ~FsmlElemButton( ) { }

public:
   virtual FsmlElemType    getType( ) const
                           { return FSMLELEM_BUTTON; }
   virtual void            getRect( FSML_RECT* out ) const;
   virtual const FsmlAttr* getAttr( const string& name ) const;
   virtual void            setAttr( const FsmlAttr& attr );
   virtual FsmlElem*       find( const string& id,
                                 FsmlElemType t = FSMLELEM_ANY );
   virtual const FsmlElem* find( const string& id,
                                 FsmlElemType t = FSMLELEM_ANY ) const;
   virtual FsmlElem*       clone( ) const;
   virtual bool            isVisible( ) const;

private:
   virtual void            setParent( FsmlElem* parent );
   virtual void            draw( FsmlCanvas* canvas );

   virtual void            onSelectScript( FsmlScript* script );
   virtual void            onTick( unsigned int dt );
   virtual void            onModState( unsigned int mod, long state );
   virtual void            onMouseOver( const FSML_POS& pos );
   virtual void            onMouseMove( const FSML_POS& from, const FSML_POS& to );
   virtual void            onMouseHover( const FSML_POS& pos );
   virtual void            onMouseOut( const FSML_POS& pos );
   virtual void            onMouseDown( const FSML_POS& pos, unsigned int repeat );
   virtual void            onMouseUp( const FSML_POS& pos );
   virtual void            onMouseRDown( const FSML_POS& pos, unsigned int repeat );
   virtual void            onMouseRUp( const FSML_POS& pos );
   virtual void            onMouseDblClick( const FSML_POS& pos );
   virtual void            onMouseWheel( const FSML_POS& pos, int direction );

   /** Just sets the 'state' attribute. */
   void         setButtonState( Button_State state );
   /** Just queries the 'state' attribute. */
   Button_State getButtonState( ) const;
   void calcRect( FSML_RECT* r, FSML_POS* o ) const;

private:
   FsmlElem*   m_parent;
   FsmlZoo*    m_zoo;
   FsmlScript* m_script;
   bool        m_visible;
   FSML_RECT   m_rect;
   FSML_POS    m_origin;
   FsmlAttrMap m_attrs;

   struct _MOUSE
   {
      unsigned long flags;
      _MOUSE( ) : flags( 0 ) { }
   }           m_mouse;
};

inline FsmlElemButton::FsmlElemButton( FsmlZoo* zoo )
   : m_zoo( zoo ), m_script( NULL ), m_visible( true ), m_parent( NULL )
{
   
}

/************************************************************************/
/* FsmlElemTemplate                                                     */
/************************************************************************/

class FsmlElemTemplate : public FsmlElem
{
public:
   static const char* const s_symbol;
private:
   typedef std::vector<FsmlElemPtr> ElemVector;
public:
   FsmlElemTemplate( );
private:
   FsmlElemTemplate( const FsmlElemTemplate& r ) { }
   virtual ~FsmlElemTemplate( ) { }

public:
   virtual FsmlElemType    getType( ) const
                           { return FSMLELEM_TEMPLATE; }
   virtual void            setAttr( const FsmlAttr& attr );
   virtual bool            addChild( FsmlElem* child, FsmlElem* before = 0 );
   virtual bool            construct( const string& id );
   virtual FsmlElem*       clone( ) const;
private:
   virtual void            setParent( FsmlElem* parent );
   
   virtual void            onSelectScript( FsmlScript* script );

private:
   FsmlElem*   m_parent;
   FsmlScript* m_script;
   FsmlAttrMap m_attrs;
   ElemVector  m_children;   
};

inline FsmlElemTemplate::FsmlElemTemplate( )
   : m_script( NULL ), m_parent( NULL )
{

}

/************************************************************************/
/* FsmlElemSkin                                                         */
/************************************************************************/

class FsmlElemSkin : public FsmlElem
{
   struct COMPONENT;
   typedef vector<COMPONENT*> ComponentVector;

   FsmlElemSkin( ) { }
   ~FsmlElemSkin( );
public:
   FsmlElemSkin( HINSTANCE inst, HWND wnd, const string& root ) /* throw( runtime_error ) */;

   virtual FsmlElemType    getType( ) const
                           { return FSMLELEM_SKIN; }
   virtual void            getRect( FSML_RECT* out ) const;
   virtual const FsmlAttr* getAttr( const string& name ) const;
   virtual void            setAttr( const FsmlAttr& attr );
   virtual bool            addChild( FsmlElem* child, FsmlElem* before = 0 );
   virtual FsmlElem*       find( const string& id,
                                 FsmlElemType t = FSMLELEM_ANY );
   virtual const FsmlElem* find( const string& id,
                                 FsmlElemType t = FSMLELEM_ANY ) const;
   virtual bool            construct( const string& id );
   virtual bool            isVisible( ) const;
   
   virtual void            onTick( unsigned int dt );
   virtual void            onModState( unsigned int mod, long state );
   bool                    onWndProc( UINT msg, WPARAM wparam, LPARAM lparam );
   void                    draw( HDC dc );

private:
   virtual FsmlElem*       clone( ) const 
                           { return 0; }
   virtual void            onInvalidate( FsmlElem* from );
   virtual void            onShowTip( const string& tip );
   virtual void            onHideTip( );

   void      normalize( FSML_POS* pt ) const;
   FsmlElem* childFromPoint( const FSML_POS& pt );
   FSML_POS  toChildPos( const FsmlElem* child, const FSML_POS& pos ) const;
   void      getScale( float* x, float* y ) const;

private:
   HINSTANCE       m_inst;
   HWND            m_wnd;
   FsmlZoo*        m_zoo;
   FsmlScript*     m_script;
   FsmlAttrMap     m_attrs;
   ComponentVector m_children;
   HWND            m_tipctrl;

   struct _MOUSE
   {
      FSML_POS      pos;
      unsigned long flags;
      FsmlElemPtr   target;
      unsigned int  hover_time;
      unsigned int  repeat_unwind;
      unsigned int  repcount_lbutton;
      unsigned int  repcount_rbutton;
      int           wheel_delta;

      _MOUSE( ) : flags( 0 ), hover_time( 0 ), repeat_unwind( 0 ),
                  repcount_lbutton( 0 ), repcount_rbutton( 0 ),
                  wheel_delta( 0 ) { }
   }               m_mouse;
};

struct FsmlElemSkin::COMPONENT
{
   FsmlElemPtr  elem;
   FSML_RECT    rect;
};

#endif