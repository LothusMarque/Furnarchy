#include "FsmlElem.h"
#include "FsmlScript.h"

const char* const FsmlElemTemplate::s_symbol = "template";

void FsmlElemTemplate::setParent( FsmlElem* parent )
{
   m_parent = parent;
}

FsmlElem* FsmlElemTemplate::clone( ) const
{
   FsmlElemTemplate* e = new FsmlElemTemplate( );

   e->m_parent = m_parent;
   e->m_script = m_script;
   e->m_attrs  = m_attrs;

   ElemVector::const_iterator i = m_children.begin( );

   for (; i != m_children.end( ); ++i)
      e->m_children.push_back( (*i)->clone( ) );

   return e;
}

void FsmlElemTemplate::setAttr( const FsmlAttr& attr )
{
   /* Necessary for external creation. */
   m_attrs.setAttr( attr );
}

/* Necessary for external creation. */
bool FsmlElemTemplate::addChild(FsmlElem* child, FsmlElem* before /* = 0  */ )
{
   switch (child->getType( ))
   {
   default:
      return false;
   case FSMLELEM_BUTTON:
   case FSMLELEM_FRAME:
   case FSMLELEM_IMAGE:
   case FSMLELEM_TEMPLATE:  // Would it really hurt?
      break;
   }

   /* Ignore the before parameter. */
   m_children.push_back( child );
   return true;
}

bool FsmlElemTemplate::construct( const string& id )
{
   if (!(m_attrs.getAttrSz( "id", "" ) == id))
      return false;

   /* I'm up to bat. */
   if (m_parent)
   {
      ElemVector::const_iterator i = m_children.begin( );
      FsmlElemPtr cloned;  // To avoid memory leaks if the parent ignores it.
      for (; i != m_children.end( ); ++i)
      {
         cloned = (*i)->clone( );
         if (m_parent->addChild( cloned, this ))
         {
            if (m_script) m_script->pushConstructedElem( cloned );
         }
      }

      return true;
   }

   return false;
}

void FsmlElemTemplate::onSelectScript( FsmlScript* script )
{
   m_script = script;
   
   ElemVector::iterator i = m_children.begin( );
   for (; i != m_children.end( ); ++i)
      (*i)->onSelectScript( script );
}
