#include "eggy.h"
#include "furn2/furn2.h"
#include "yiffurc/yiffurc.h"
#include "resource.h"
#include "tinyxml/tinyxml.h"
#include "xml_charset.h"

#include <fstream>

namespace eggy {

//////////////////////////////////////////////////////////////////////////

enum { IMGL_GROUP = 1, IMGL_GROUPEX, IMGL_BUDDY, IMGL_BUDDYOFF, IMGL_BUDDYNEARBY };
enum { WINDOW_MIN_WIDTH = 96, WINDOW_MIN_HEIGHT = 48 };
enum { CLIENT_MARGIN = 7 };

//////////////////////////////////////////////////////////////////////////

static HMENU      s_popup_menu  = NULL;
static WNDPROC    s_tvproc      = NULL;
static HIMAGELIST s_tv_imglist  = NULL;
static HFONT      s_font_group        = NULL;
static HFONT      s_font_buddy        = NULL;
static HFONT      s_font_buddy_frozen = NULL;
static HFONT      s_font_group_frozen = NULL;

//////////////////////////////////////////////////////////////////////////

void BuddyList::class_init( )
{
   /* Init common controls. */
   {
      INITCOMMONCONTROLSEX iccex;
      iccex.dwSize = sizeof( iccex );
      iccex.dwICC = ICC_TREEVIEW_CLASSES;
      InitCommonControlsEx( &iccex );
   }

   /* Register the buddy list window class. */
   {
      WNDCLASSEX wc;
      memset( &wc, 0, sizeof( wc ) );
      wc.cbSize        = sizeof( wc );
      // wc.hIcon         = LoadIcon( s_inst, MAKEINTRESOURCE( IDI_ICON1 ) );
      wc.hbrBackground = GetSysColorBrush( COLOR_3DFACE );
      wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
      wc.hInstance     = s_inst;
      wc.lpfnWndProc   = BuddyList::wnd_proc;
      wc.lpszClassName = "EggyBuddyList";
      wc.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
      wc.lpszMenuName  = MAKEINTRESOURCE( IDR_BUDDYLIST );
      RegisterClassEx( &wc );
   }

   /* Create the popup menu. */
   s_popup_menu = LoadMenu( s_inst, MAKEINTRESOURCE( IDR_LISTPOPUP ) );

   /* Create the image list for the treeview. */
   s_tv_imglist = ImageList_LoadImage( s_inst, MAKEINTRESOURCE( IDB_LIST ), 
                                       32, 0, CLR_DEFAULT, 
                                       IMAGE_BITMAP, LR_CREATEDIBSECTION );

   /* Get the treeview proc. */
   s_tvproc = get_sys_class_proc( WC_TREEVIEW ); 

   /* Create fonts. */
   {
      HFONT font_sys = GetStockFont( DEFAULT_GUI_FONT );
      LOGFONT lf;
      GetObject( font_sys, sizeof( LOGFONT ), &lf );

      lf.lfQuality = 5; // CLEARTYPE_QUALITY;      
      s_font_buddy = CreateFontIndirect( &lf );
      
      lf.lfItalic = TRUE;
      s_font_buddy_frozen = CreateFontIndirect( &lf );
      
      lf.lfWeight = FW_BOLD;
      lf.lfItalic = FALSE;
      s_font_group = CreateFontIndirect( &lf );

      lf.lfItalic = TRUE;
      s_font_group_frozen = CreateFontIndirect( &lf );
   }      
}

void BuddyList::class_uninit( )
{
   DeleteFont( s_font_buddy ); s_font_buddy = NULL;
   DeleteFont( s_font_buddy_frozen ); s_font_buddy_frozen = NULL;
   DeleteFont( s_font_group ); s_font_group = NULL;
   DeleteFont( s_font_group_frozen ); s_font_group_frozen = NULL;

   s_tvproc = NULL;
   
   ImageList_Destroy( s_tv_imglist ); s_tv_imglist = NULL;

   DestroyMenu( s_popup_menu );
   s_popup_menu = NULL;

   UnregisterClass( "EggyBuddyList", s_inst );
}

BuddyList::BuddyList( )
{
   assert( Eggy::singleton( ) );

   m_always_on_top = true;

   /* Create the buddy list window. */
   m_wnd.wnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_TOPMOST, "EggyBuddyList", "Eggy",
                               WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX,
                               0, 0, 200, 300,
                               NULL, NULL, s_inst, 
                               this );

   /* Create the controls. */
   m_wnd.tree = CreateWindowEx( 0, WC_TREEVIEW, NULL,
                                WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | 
                                TVS_LINESATROOT | TVS_EDITLABELS |
                                TVS_INFOTIP | TVS_FULLROWSELECT | TVS_NOHSCROLL ,
                                0, 0, 1, 1,
                                m_wnd.wnd, NULL, s_inst, NULL );
   // Set the common control version to 5 so text doesn't get clipped.
   SendMessage( m_wnd.tree, CCM_SETVERSION, 5, 0 );
   // Set the icon.
   SetClassLongPtr( m_wnd.wnd, GCLP_HICON, (LONG_PTR) Eggy::singleton( )->getIcon( ) );
   // Set the image list.
   TreeView_SetImageList( m_wnd.tree, s_tv_imglist, TVSIL_STATE );
   // Give tooltips a really long autopop.
   SendMessage( TreeView_GetToolTips( m_wnd.tree ), TTM_SETDELAYTIME, TTDT_AUTOPOP, 30000 );
   // And pretty margins.
   {
      RECT r = { 3, 3, 3, 3 };
      SendMessage( TreeView_GetToolTips( m_wnd.tree ), TTM_SETMARGIN, 0, (LPARAM) &r );
   }
   // Subclass.
   SetWindowLongPtr( m_wnd.tree, GWLP_WNDPROC, (LONG_PTR) treeview_proc );

   // readList( ); Not here because readList() depends on Eggy having a ptr to us.

   fitWindowElements( );

   setAlwaysOnTop( m_always_on_top );
}

BuddyList::~BuddyList( )
{
   DestroyWindow( m_wnd.wnd );

   while (!m_groups.empty( ))
   {
      GROUP* group = m_groups.front( );

      while (!group->buddies.empty( ))
      {
         BUDDY* buddy = group->buddies.front( );

         buddy->contact->setBuddy( false );

         if (group->frozen)
            buddy->contact->thaw( );

         buddy->contact->release( );
         delete buddy;

         group->buddies.pop_front( );
      }

      delete group;
      m_groups.pop_front( );
   }
}

void BuddyList::show( bool onoff )
{
   if (onoff)
   {
      if (!IsWindowVisible( m_wnd.wnd )) 
      {
         // Center inside the furcadia client area.
         HWND parent = Eggy::singleton( )->getParentWnd( );
         RECT parent_rect;
         GetWindowRect( parent, &parent_rect );
         parent_rect.right = parent_rect.right - parent_rect.left;
         parent_rect.bottom = parent_rect.bottom - parent_rect.top;
         RECT dlg_rect;
         GetWindowRect( m_wnd.wnd, &dlg_rect );
         dlg_rect.right = dlg_rect.right - dlg_rect.left;
         dlg_rect.bottom = dlg_rect.bottom - dlg_rect.top;
         POINT pt = { 
                      parent_rect.left + (parent_rect.right - dlg_rect.right) / 2,
                      parent_rect.top + (parent_rect.bottom - dlg_rect.bottom) / 2
                    };
         SetWindowPos( m_wnd.wnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

         // Show.
         ShowWindow( m_wnd.wnd, SW_SHOWNA );
      }
      else
      {
         if (IsIconic( m_wnd.wnd ))
            OpenIcon( m_wnd.wnd );
      }
   }
   else
   {
      // Hide.
      ShowWindow( m_wnd.wnd, SW_HIDE );
      writeList( );
   }
}

void BuddyList::setAlwaysOnTop( bool onoff )
{
   if (m_always_on_top = onoff)
      SetWindowPos( m_wnd.wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
   else
      SetWindowPos( m_wnd.wnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
}

bool BuddyList::isBuddy( const string& name ) const {
   return findBuddy( name );
}

bool BuddyList::findBuddy( const string& name, 
                           const BuddyList::BUDDY** buddy_out /* = NULL */, 
                           const BuddyList::GROUP** group_out /* = NULL  */ ) const
{
   string shortname = make_shortname( name );

   for (list<GROUP*>::const_iterator i = m_groups.begin( ); 
        i != m_groups.end( ); ++i)
   {
      for (list<BUDDY*>::const_iterator j = (*i)->buddies.begin( );
           j != (*i)->buddies.end( ); ++j)
      {
         if ((*j)->contact->getShortName( ) == shortname)
         {
            if (buddy_out) *buddy_out = (*j);
            if (group_out) *group_out = (*i);
            return true;
         }
      }
   }

   if (buddy_out) *buddy_out = NULL;
   if (group_out) *group_out = NULL;

   return false;
}

bool BuddyList::findBuddy( const string& name, 
                          BuddyList::BUDDY** buddy_out /* = NULL */, 
                          BuddyList::GROUP** group_out /* = NULL  */ )
{
   string shortname = make_shortname( name );

   for (list<GROUP*>::iterator i = m_groups.begin( ); 
        i != m_groups.end( ); ++i)
   {
      for (list<BUDDY*>::iterator j = (*i)->buddies.begin( );
           j != (*i)->buddies.end( ); ++j)
      {
         if ((*j)->contact->getShortName( ) == shortname)
         {
            if (buddy_out) *buddy_out = (*j);
            if (group_out) *group_out = (*i);
            return true;
         }
      }
   }

   if (buddy_out) *buddy_out = NULL;
   if (group_out) *group_out = NULL;

   return false;
}

BuddyList::GROUP* BuddyList::addGroup( const string& group_name )
{
   /* Check if the group already exists. */
   for (list<GROUP*>::iterator i = m_groups.begin( );
        i != m_groups.end( ); ++i)
   {
      if (!_stricmp( (*i)->name.c_str( ), group_name.c_str( ) ))
         // Already added.
         return (*i);
   }

   /* Create the group. */
   GROUP* group  = new GROUP( group_name );
   m_groups.push_back( group );

   /* Update the window. */
   {
      if (!group_name.length( )) // Unsorted group.
      {
         // Unsorted group is just the root level.
         group->tree_handle = TVI_ROOT;
      }
      else
      {
         /* Add the group to the root of the tree. */
         TVINSERTSTRUCT ins_item;
         ins_item.hParent        = TVI_ROOT;
         ins_item.hInsertAfter   = TVI_LAST;
         makeTreeItem( group, &ins_item.itemex );

         group->tree_handle = TreeView_InsertItem( m_wnd.tree, &ins_item );

         delete [] ins_item.itemex.pszText;

         // Sort.
         sortTreeItems( );
      }
   }

   return group;
}

bool BuddyList::removeGroup( const string& name )
{
   for (list<GROUP*>::iterator i = m_groups.begin( );
        i != m_groups.end( ); ++i)
   {
      if (!_stricmp( (*i)->name.c_str( ), name.c_str( ) ))
      {
         /* Here it is. */
         GROUP* group = (*i);

         // Thaw and release all children buddies.
         while (!group->buddies.empty( ))
         {
            BUDDY* buddy = group->buddies.front( );

            buddy->contact->setBuddy( false );

            // Remove from UI.
            TreeView_DeleteItem( m_wnd.tree, buddy->tree_handle );
            
            if (group->frozen)
               buddy->contact->thaw( );
            
            buddy->contact->release( );
            delete buddy;

            group->buddies.pop_front( );
         }

         // Remove from UI.
         if (group->name.length( ))
            TreeView_DeleteItem( m_wnd.tree, group->tree_handle );

         // Remove and delete the group.
         delete group;
         m_groups.erase( i );

         // Sort.
         sortTreeItems( );

         return true;
      }
   }

   return false;
}

list<string> BuddyList::getGroups( ) const
{
   list<string> r;
   for (list<GROUP*>::const_iterator i = m_groups.begin( ); i != m_groups.end( ); ++i)
      r.push_back( (*i)->name );
   return r;
}

BuddyList::BUDDY* BuddyList::addBuddy( const string& name, 
                                       const string& group_name /* = "" */ )
{
   // Sanity check.
   if (!name.length( ))
      return NULL;
   
   /* See if the buddy already exists. */
   BUDDY* buddy = NULL;
   GROUP* group = NULL;
   if (findBuddy( name, &buddy, &group ))
   {
      if (!_stricmp( group->name.c_str( ), group_name.c_str( ) ))
      {
         // Already in this group.
         buddy->contact->adjustName( name );
         return buddy;
      }
   }
   
   if (buddy) // Exists, but in another group.
   {
      /* Remove the buddy from the other group. */
      TreeView_DeleteItem( m_wnd.tree, buddy->tree_handle );

      for (list<BUDDY*>::iterator i = group->buddies.begin( );
           i != group->buddies.end( ); ++i)
      {
         if ((*i) == buddy)
         {
            if (group->frozen)
               buddy->contact->thaw( );
            group->buddies.erase( i );
            break;
         }
      }

      // Update the group's tree item.
      if (group->name.length( ))
      {
         TVITEMEX item;
         makeTreeItem( group, &item );
         TreeView_SetItem( m_wnd.tree, &item );
         delete [] item.pszText;
      }

      // Adjust the name.
      buddy->contact->adjustName( name );
   }
   else // Doesn't exist.
   {
      // Create a new buddy.
      buddy = new BUDDY( Eggy::singleton( )->contactCreate( name ) );
      if (!buddy->contact)
      {
         delete buddy;
         return NULL;
      }
   }
   
   // Get/create the destination group.
   if (!(group = addGroup( group_name )))
   {
      // This is bad.
      throw runtime_error( "Zombie buddy created!" );
   }

   // Insert.
   group->buddies.push_back( buddy );

   // Freeze if the group is frozen.
   if (group->frozen)
      buddy->contact->freeze( );

   /* Update the window. */
   {
      {
         // Add to the end of the group.
         TVINSERTSTRUCT ins_item;
         ins_item.hParent        = group->tree_handle;
         ins_item.hInsertAfter   = TVI_LAST;
         makeTreeItem( buddy, &ins_item.itemex );

         buddy->tree_handle = TreeView_InsertItem( m_wnd.tree, 
                                                   &ins_item );

         delete [] ins_item.itemex.pszText;
      }

      // Update the group's tree item.
      if (group->name.length( ))
      {
         TVITEMEX item;
         makeTreeItem( group, &item );
         TreeView_SetItem( m_wnd.tree, &item );
         delete [] item.pszText;
      }

      // Sort.
      sortTreeItems( );
   }

   buddy->contact->setBuddy( true );

   return buddy;
}

bool BuddyList::removeBuddy( const string& name )
{
   string shortname = make_shortname( name );

   // Look through the groups for the buddy.
   for (list<GROUP*>::iterator i = m_groups.begin( );
        i != m_groups.end( ); ++i)
   {
      for (list<BUDDY*>::iterator j = (*i)->buddies.begin( );
           j != (*i)->buddies.end( ); ++j)
      {
         if ((*j)->contact->getShortName( ) == shortname)
         {
            // Here it is.
            BUDDY* buddy = (*j);

            buddy->contact->setBuddy( false );

            // Remove from UI.
            TreeView_DeleteItem( m_wnd.tree, buddy->tree_handle );

            // Unfreeze if the group is frozen.
            if ((*i)->frozen)
               buddy->contact->thaw( );

            // Release.
            buddy->contact->release( );
            delete buddy;

            (*i)->buddies.erase( j );

            // Update the group's tree item.
            if ((*i)->name.length( ))
            {
               TVITEMEX item;
               makeTreeItem( (*i), &item );
               TreeView_SetItem( m_wnd.tree, &item );
               delete [] item.pszText;
            }

            // Sort.
            sortTreeItems( );

            return true;
         }
      }
   }

   return false;
}

bool BuddyList::freezeGroup( const string& name, bool freeze )
{
   for (list<GROUP*>::iterator i = m_groups.begin( );
        i != m_groups.end( ); ++i)
   {
      if (!_stricmp( (*i)->name.c_str( ), name.c_str( ) ))
      {
         /* Here it is. */
         GROUP* group = (*i);
         
         if (freeze != group->frozen)
         {
            group->frozen = freeze;

            // Freeze all its contacts.
            for (list<BUDDY*>::iterator j = group->buddies.begin( );
                 j != group->buddies.end( ); ++j)
            {
               if (freeze)
                  (*j)->contact->freeze( );
               else
                  (*j)->contact->thaw( );
            }
         }

         return true;
      }
   }

   return false;
}

void BuddyList::onContact( const string& name, On_Contact how )
{
   /* Find the contact. */
   BUDDY* buddy;
   GROUP* group;
   if (!findBuddy( name, &buddy, &group ))
      return;

   // Mention it.
   if (how == CONTACT_ONLINE)
   {      
      ostringstream ss;
      if (buddy->contact->getOnline( ))
      {
         ss << "<b><name shortname='" << buddy->contact->getShortName( ) << "'>"
            << pipes_to_spaces( buddy->contact->getName( ) ) << "</name></b> is online.";
         f2_speak( ss.str( ).c_str( ) );
      }
      else if (Eggy::singleton( )->getOptFlags( ) & OPT_ANNOUNCELOGOFFS)
      {
         ss << "<b><name shortname='" << buddy->contact->getShortName( ) << "'>"
            << pipes_to_spaces( buddy->contact->getName( ) ) << "</name></b> is offline.";
         f2_speak( ss.str( ).c_str( ) );
      }         
   }
   else if (how == CONTACT_NEARBY)
   {
      if (buddy->contact->isBuddy( ))
      {
         if (Eggy::singleton( )->isMapReady( ) && Eggy::singleton( )->getOptFlags( ) & OPT_ANNOUNCENEARBY)
         {
            // Announce it.
            ostringstream ss;
            if (buddy->contact->getNearby( ))
            {
               ss << "<b><name shortname='" << buddy->contact->getShortName( ) << "'>"
                  << pipes_to_spaces( buddy->contact->getName( ) ) << "</name></b> is on the map.";
               f2_speak( ss.str( ).c_str( ) );
            }
         }
      }
   }
   
   if (how == CONTACT_ONLINE)
   {
      /* Update the UI. */

      // Buddy.
      TVITEMEX item;
      makeTreeItem( buddy, &item );
      TreeView_SetItem( m_wnd.tree, &item );
      delete [] item.pszText;

      // Group.
      if (group->name.length( ))
      {
         makeTreeItem( group, &item );
         TreeView_SetItem( m_wnd.tree, &item );
         delete [] item.pszText;
      }

      // Sort.
      sortTreeItems( );
   }
   else if (how == CONTACT_NAME || how == CONTACT_NEARBY)
   {
      // Update the buddy's name and image in the UI.
      TVITEMEX item;
      makeTreeItem( buddy, &item );
      TreeView_SetItem( m_wnd.tree, &item );
      delete [] item.pszText;

      // Sort.
      sortTreeItems( );
   }
}

void BuddyList::onTick( unsigned int dt )
{
   /* Scroll the treeview if dragging past the client area. */
   if (m_drag.isDragging( ) && (m_drag.scroll_ms += dt) >= 180)
   {
      m_drag.scroll_ms = 0;

      TVHITTESTINFO hit;
      GetCursorPos( &hit.pt );
      ScreenToClient( m_wnd.tree, &hit.pt );
      TreeView_HitTest( m_wnd.tree, &hit );
      
      if (!hit.hItem)
      {
         if (hit.flags & TVHT_ABOVE)
         {
            TreeView_SelectDropTarget( m_wnd.tree, TreeView_GetFirstVisible( m_wnd.tree ) );
            SendMessage( m_wnd.tree, WM_KEYDOWN, VK_UP, 0 );
            SendMessage( m_wnd.tree, WM_KEYUP, VK_UP, 0 );
            TreeView_SelectDropTarget( m_wnd.tree, TreeView_GetFirstVisible( m_wnd.tree ) );
         }
         else if (hit.flags & TVHT_BELOW)
         {
            TreeView_SelectDropTarget( m_wnd.tree, TreeView_GetLastVisible( m_wnd.tree ) );
            SendMessage( m_wnd.tree, WM_KEYDOWN, VK_DOWN, 0 );
            SendMessage( m_wnd.tree, WM_KEYUP, VK_DOWN, 0 );
            TreeView_SelectDropTarget( m_wnd.tree, TreeView_GetLastVisible( m_wnd.tree ) );
         }
         else
            return;

         /* Remember the target group. */
         TVITEMEX item;
         item.mask = TVIF_PARAM;
         if (item.hItem = TreeView_GetSelection( m_wnd.tree ))
         {
            TreeView_GetItem( m_wnd.tree, &item );
            if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
               m_drag.drop_group = ((GROUP*) item.lParam)->name;
            else if (*((Tree_Item_Type*) item.lParam) == TREEITEM_BUDDY)
            {
               GROUP* group;
               findBuddy( ((BUDDY*) item.lParam)->contact->getName( ), NULL, &group );
               assert( group );
               m_drag.drop_group = group->name;
            }
         }
         else
            m_drag.drop_group = "";
      }
   }
}

void BuddyList::onMapReady( )
{
   if (Eggy::singleton( )->getOptFlags( ) & OPT_ANNOUNCENEARBY)
   {
      /* Buffer all nearby buddies. */
      vector<BUDDY*> nearby_buddies;

      for (list<GROUP*>::const_iterator i = m_groups.begin( );
           i != m_groups.end( ); ++i)
      {
         for (list<BUDDY*>::const_iterator j = (*i)->buddies.begin( );
             j != (*i)->buddies.end( ); ++j)
         {
            if ((*j)->contact->getNearby( ))
               nearby_buddies.push_back( *j );
         }
      }

      if (nearby_buddies.size( ))
      {
         /* Sort all nearby buddies. */
         struct BuddyPredicate
         {
            bool operator()( const BUDDY* a, const BUDDY* b ) const
            {
               return a->contact->getShortName( ) < b->contact->getShortName( );
            }
         };
         
         sort( nearby_buddies.begin( ), nearby_buddies.end( ), BuddyPredicate( ) );
         
         /* Spit it out. */
         ostringstream ss;
         ss << "You have buddies on this map: ";
         for (vector<BUDDY*>::const_iterator i = nearby_buddies.begin( ); i != nearby_buddies.end( ); ++i)
            if (i != nearby_buddies.begin( ))
               ss << ", <b><name shortname='" << (*i)->contact->getShortName( ) << "'>" << (*i)->contact->getName( ) << "</name></b>";
            else
               ss << "<b><name shortname='" << (*i)->contact->getShortName( ) << "'>" << (*i)->contact->getName( ) << "</name></b>";
         ss << ".";

         f2_speak( ss.str( ).c_str( ) );
      }
   }
}

void BuddyList::fitWindowElements( )
{
   /* Tree view should fit entire client area with 7 pixel padding. */
   RECT r;
   GetClientRect( m_wnd.wnd, &r );
   SetWindowPos( m_wnd.tree, 0, 
                 CLIENT_MARGIN, CLIENT_MARGIN, 
                 max( r.right - CLIENT_MARGIN * 2, (LONG) 0 ), 
                 max( r.bottom - CLIENT_MARGIN * 2, (LONG) 0 ), 
                 SWP_NOZORDER );
}

LRESULT CALLBACK BuddyList::wnd_proc( HWND wnd, UINT msg, 
                                      WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((BuddyList*) GetWindowLongPtr( wnd, GWLP_USERDATA ))

   static const Tree_Item_Type LPARAM_ADDBUDDY = TREEITEM_ADDBUDDY;
   static const Tree_Item_Type LPARAM_ADDGROUP = TREEITEM_ADDGROUP;

   switch (msg)
   {
   case WM_CREATE:
      {
         CREATESTRUCT* cs = (CREATESTRUCT*) lparam;
         // Store the instance ptr.
         SetWindowLongPtr( wnd, GWLP_USERDATA, (LONG_PTR) cs->lpCreateParams );
      }
      return 0;

   case WM_GETMINMAXINFO:
      ((MINMAXINFO*) lparam)->ptMinTrackSize.x = WINDOW_MIN_WIDTH;
      ((MINMAXINFO*) lparam)->ptMinTrackSize.y = WINDOW_MIN_HEIGHT;
      return 0;
   
   case WM_SIZE:
      if (wparam == SIZE_MAXIMIZED || wparam == SIZE_RESTORED)
         _INST_PTR( )->fitWindowElements( );
      else if (wparam == SIZE_MINIMIZED)
         // Or else Furc doesn't seem to get keyboard focus.
         SetFocus( GetParent( wnd ) );      
      break;

   case furn2::F2M_ISNOTDLGWND:
      return TRUE;
   
   case WM_CLOSE:
      _INST_PTR( )->show( false );
      return 0;

   case WM_COMMAND:
      if (!HIWORD( wparam )) // Menu Item.
      {
         BuddyList* inst = _INST_PTR( );

         switch (LOWORD( wparam ))
         {
         case ID_FILE_EXIT:
            SendMessage( wnd, WM_CLOSE, 0, 0 );
            return 0;

         case ID_EDIT_OPTIONS:
            Eggy::singleton( )->showOptions( );
            return 0;

         case ID_EDIT_VIEWLOG:
            ShellExecute( NULL, "open", Eggy::singleton( )->getLogFile( ).c_str( ), 
                          NULL, NULL, SW_SHOWNORMAL );
            return 0;

         case ID_EDIT_BROWSELOGFOLDER:
            ShellExecute( NULL, "explore", Eggy::singleton( )->getLogFolder( ).c_str( ),
                          NULL, NULL, SW_SHOWNORMAL );
            return 0;

         case ID_EDIT_OFFLINEMESSAGES:
            Eggy::singleton( )->getOfflineMessages( )->show( true );
            return 0;

         case ID_HELP_REPORTBUGS:
            Eggy::singleton( )->pmCreate( "Lothus|Marque", true );
            return 0;

         case ID_HELP_ABOUT:
            Eggy::singleton( )->showAbout( );
            return 0;
         
         case ID_LIST_ADDBUDDY:
         case ID_LIST_ADDGROUP:
            {
               // Determine where in the tree to insert it.
               HTREEITEM parent = TVI_ROOT;

               if (LOWORD( wparam ) == ID_LIST_ADDBUDDY)
               {
                  HTREEITEM sel = TreeView_GetSelection( inst->m_wnd.tree );
                  if (sel)
                  {
                     TVITEMEX item;
                     memset( &item, 0, sizeof( item ) );
                     item.mask = TVIF_HANDLE | TVIF_PARAM;
                     item.hItem = sel;
                     TreeView_GetItem( inst->m_wnd.tree, &item );

                     if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
                        parent = sel;
                     else if (TreeView_GetParent( inst->m_wnd.tree, sel ))
                        parent = TreeView_GetParent( inst->m_wnd.tree, sel );
                  }
               }

               // Insert a dummy item and edit it.
               TVINSERTSTRUCT tvi;
               memset( &tvi, 0, sizeof( tvi ) );
               tvi.hParent = parent;
               tvi.hInsertAfter  = TVI_FIRST;
               tvi.itemex.mask   = TVIF_PARAM | TVIF_STATE | TVIF_TEXT;
               tvi.itemex.lParam = LOWORD( wparam ) == ID_LIST_ADDBUDDY ? 
                                   (LPARAM) &LPARAM_ADDBUDDY : (LPARAM) &LPARAM_ADDGROUP;
               tvi.itemex.state  = LOWORD( wparam ) == ID_LIST_ADDBUDDY ? 
                  INDEXTOSTATEIMAGEMASK( IMGL_BUDDY ) : INDEXTOSTATEIMAGEMASK( IMGL_GROUP );
               tvi.itemex.stateMask = TVIS_STATEIMAGEMASK;
               tvi.itemex.pszText = "";

               HTREEITEM dummy = TreeView_InsertItem( inst->m_wnd.tree, &tvi );
               // Make sure the parent is expanded.
               TreeView_Expand( inst->m_wnd.tree, parent, TVE_EXPAND );
               TreeView_EditLabel( inst->m_wnd.tree, dummy );              
            }
            return 0;

         case ID_LIST_DELETE:
            {
               TVITEMEX item;
               if (item.hItem = TreeView_GetSelection( inst->m_wnd.tree ))
               {
                  item.mask = TVIF_PARAM;
                  TreeView_GetItem( inst->m_wnd.tree, &item );

                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
                  {
                     inst->removeGroup( ((GROUP*) item.lParam)->name );
                     inst->writeList( );
                  }
                  else if (*((Tree_Item_Type*) item.lParam) == TREEITEM_BUDDY)
                  {
                     inst->removeBuddy( ((BUDDY*) item.lParam)->contact->getName( ) );
                     inst->writeList( );
                  }
               }
            }
            return 0;

         case ID_LIST_FREEZE:
            {
               TVITEMEX item;
               if (item.hItem = TreeView_GetSelection( inst->m_wnd.tree ))
               {
                  item.mask = TVIF_PARAM;
                  TreeView_GetItem( inst->m_wnd.tree, &item );

                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
                  {
                     inst->freezeGroup( ((GROUP*) item.lParam)->name,
                                        !((GROUP*) item.lParam)->frozen );
                     inst->writeList( );

                     InvalidateRect( inst->m_wnd.tree, NULL, FALSE );
                  }
               }
            }
            return 0;

         case ID_LIST_MESSAGE:
            {
               TVITEMEX item;
               if (item.hItem = TreeView_GetSelection( inst->m_wnd.tree ))
               {
                  item.mask = TVIF_PARAM;
                  TreeView_GetItem( inst->m_wnd.tree, &item );

                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_BUDDY)
                     Eggy::singleton( )->pmCreate( ((BUDDY*) item.lParam)->contact->getName( ), true );
               }
            }
            return 0;
            
         case ID_LIST_MESSAGEALL:
            {
               TVITEMEX item;
               if (item.hItem = TreeView_GetSelection( inst->m_wnd.tree ))
               {
                  item.mask = TVIF_PARAM;
                  TreeView_GetItem( inst->m_wnd.tree, &item );

                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
                     inst->messageGroup( ((GROUP*) item.lParam)->name, false );
               }
            }
            return 0;

         case ID_LIST_MESSAGEONLINE:
            {
               TVITEMEX item;
               if (item.hItem = TreeView_GetSelection( inst->m_wnd.tree ))
               {
                  item.mask = TVIF_PARAM;
                  TreeView_GetItem( inst->m_wnd.tree, &item );

                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
                     inst->messageGroup( ((GROUP*) item.lParam)->name, true );
               }
            }
            return 0;

         case ID_LIST_SUMMON:
            {
               TVITEMEX item;
               if (item.hItem = TreeView_GetSelection( inst->m_wnd.tree ))
               {
                  item.mask = TVIF_PARAM;
                  TreeView_GetItem( inst->m_wnd.tree, &item );

                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
                     inst->summonGroup( ((GROUP*) item.lParam)->name );
                  else if (*((Tree_Item_Type*) item.lParam) == TREEITEM_BUDDY)
                     inst->summonBuddy( ((BUDDY*) item.lParam)->contact->getName( ) );
               }
            }
            return 0;

         case ID_LIST_JOIN:
            {
               TVITEMEX item;
               if (item.hItem = TreeView_GetSelection( inst->m_wnd.tree ))
               {
                  item.mask = TVIF_PARAM;
                  TreeView_GetItem( inst->m_wnd.tree, &item );

                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_BUDDY)
                     inst->joinBuddy( ((BUDDY*) item.lParam)->contact->getName( ) );
               }
            }
            return 0;
         }
      }
      break;

   case WM_NOTIFY:
      if (((NMHDR*) lparam)->hwndFrom == _INST_PTR( )->m_wnd.tree)
      {
         BuddyList* inst = _INST_PTR( );

         switch (((NMHDR*) lparam)->code)
         {
         case NM_RETURN:
         case NM_DBLCLK:
            {
               TVITEMEX item;
               if (item.hItem = TreeView_GetSelection( inst->m_wnd.tree ))
               {
                  item.mask = TVIF_PARAM;
                  TreeView_GetItem( inst->m_wnd.tree, &item );
                  // Only allow message-on-doubleclick/enter for buddies, not groups.
                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_BUDDY)
                  {
                     // Post a command, otherwise the double click will steal focus back
                     // right away, even if you return TRUE.
                     PostMessage( wnd, WM_COMMAND, ID_LIST_MESSAGE, 0 );
                  }
               }
            }
            return TRUE;

         case NM_RCLICK:
            {
               /* The mix of selected items determine what kind of popup gets
               displayed. */
               HMENU menu = NULL;
               HTREEITEM sel = TreeView_GetSelection( inst->m_wnd.tree );

               if (sel)
               {
                  TVITEMEX item;
                  memset( &item, 0, sizeof( item ) );
                  item.mask = TVIF_HANDLE | TVIF_PARAM;
                  item.hItem = sel;
                  TreeView_GetItem( inst->m_wnd.tree, &item );

                  if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
                  {
                     menu = GetSubMenu( s_popup_menu, 2 );
                     // Check the freeze option if frozen.
                     if (((GROUP*) item.lParam)->frozen)
                        CheckMenuItem( menu, ID_LIST_FREEZE, MF_BYCOMMAND | MF_CHECKED );
                     else
                        CheckMenuItem( menu, ID_LIST_FREEZE, MF_BYCOMMAND | MF_UNCHECKED );
                  }
                  else
                     menu = GetSubMenu( s_popup_menu, 1 );
               }
               else
                  menu = GetSubMenu( s_popup_menu, 0 );

               POINT cursor;
               GetCursorPos( &cursor );
               TrackPopupMenu( menu, TPM_RIGHTBUTTON, cursor.x, cursor.y, 0,
                               wnd, NULL );
            }
            return FALSE;

         case TVN_BEGINLABELEDIT:
            {
               // Don't allow in-place editing of non-dummy items.
               NMTVDISPINFO* nmtv = (NMTVDISPINFO*) lparam;
               if (*((Tree_Item_Type*) nmtv->item.lParam) != TREEITEM_ADDBUDDY &&
                   *((Tree_Item_Type*) nmtv->item.lParam) != TREEITEM_ADDGROUP )
                  return TRUE;
            }
            return FALSE;

         case TVN_ENDLABELEDIT:
            {
               NMTVDISPINFO* nmtv = (NMTVDISPINFO*) lparam;
               if (nmtv->item.pszText && *nmtv->item.pszText) // Edit successful.
               {
                  if (*((Tree_Item_Type*) nmtv->item.lParam) == TREEITEM_ADDBUDDY)
                  {
                     // Adding a buddy.
                     HTREEITEM parent = TreeView_GetParent( nmtv->hdr.hwndFrom, nmtv->item.hItem );
                     if (!parent)   // Unsorted/No group.
                     {
                        BUDDY* buddy = inst->addBuddy( nmtv->item.pszText, "" );
                        // Select the newly added item.
                        if (buddy)
                        {
                           TreeView_SelectItem( nmtv->hdr.hwndFrom, buddy->tree_handle );
                           inst->writeList( );
                        }
                     }
                     else  // Add under a group.
                     {
                        TVITEMEX item;
                        memset( &item, 0, sizeof( item ) );
                        item.mask  = TVIF_PARAM;
                        item.hItem = parent;
                        TreeView_GetItem( nmtv->hdr.hwndFrom, &item );
                        BUDDY* buddy = inst->addBuddy( nmtv->item.pszText, 
                                                       ((GROUP*) item.lParam)->name );
                        // Select the newly added item.
                        if (buddy)
                        {
                           TreeView_SelectItem( nmtv->hdr.hwndFrom, buddy->tree_handle );
                           inst->writeList( );
                        }
                     }
                  }
                  else
                  {
                     // Adding a group.
                     GROUP* group = inst->addGroup( nmtv->item.pszText );
                     // Select the newly added item.
                     if (group)
                     {
                        TreeView_SelectItem( nmtv->hdr.hwndFrom, group->tree_handle );
                        inst->writeList( );
                     }
                  }
               }

               // Delete the dummy edit label. Hope this is safe within this event.
               TreeView_DeleteItem( nmtv->hdr.hwndFrom, nmtv->item.hItem );
            }
            return FALSE;

         case TVN_BEGINDRAG:
            {
               NMTREEVIEW* nmtv = (NMTREEVIEW*) lparam;

               /* Only drag buddies. */
               if (*((Tree_Item_Type*) nmtv->itemNew.lParam) == TREEITEM_BUDDY)
               {
                  inst->m_drag.drag_buddy = 
                     ((BUDDY*) nmtv->itemNew.lParam)->contact->getName( );
                  SetCapture( wnd );
                  inst->m_drag.scroll_ms = 0;
               }
            }
            return 0;

         case TVN_ITEMEXPANDED:
            {
               NMTREEVIEW* nmtv = (NMTREEVIEW*) lparam;
               if (*((Tree_Item_Type*) nmtv->itemNew.lParam) == TREEITEM_GROUP)
               {
                  UINT state = (nmtv->itemNew.state & 0xFF) & TVIS_EXPANDED ? 
                     INDEXTOSTATEIMAGEMASK( IMGL_GROUPEX ) : INDEXTOSTATEIMAGEMASK( IMGL_GROUP );

                  TreeView_SetItemState( nmtv->hdr.hwndFrom, nmtv->itemNew.hItem,
                                         state, TVIS_STATEIMAGEMASK );
               }
            }
            return 0;

         case NM_CUSTOMDRAW:
            {
               NMTVCUSTOMDRAW* cd = (NMTVCUSTOMDRAW*) lparam;
               if (cd->nmcd.dwDrawStage == CDDS_PREPAINT)
                  return CDRF_NOTIFYITEMDRAW;

               // Change the font and text color per item.
               if (cd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
               {
                  if (*((Tree_Item_Type*) cd->nmcd.lItemlParam) == TREEITEM_BUDDY)
                  {
                     BUDDY* buddy = (BUDDY*) cd->nmcd.lItemlParam;
                     SelectFont( cd->nmcd.hdc, 
                                 buddy->contact->isFrozen( ) ? s_font_buddy_frozen : s_font_buddy );
                     if (!buddy->contact->getOnline( ))
                        SetTextColor( cd->nmcd.hdc, RGB( 128, 128, 128 ) );

                     return CDRF_NEWFONT;
                  }
                  else if (*((Tree_Item_Type*) cd->nmcd.lItemlParam) == TREEITEM_GROUP)
                  {
                     GROUP* group = (GROUP*) cd->nmcd.lItemlParam;
                     SelectFont( cd->nmcd.hdc, 
                                 group->frozen ? s_font_group_frozen : s_font_group );
                     return CDRF_NEWFONT;
                  }
               }
            }
            return CDRF_DODEFAULT;

         case TVN_GETINFOTIP:
            {
               NMTVGETINFOTIP* nmtv = (NMTVGETINFOTIP*) lparam;
               if (*((Tree_Item_Type*) nmtv->lParam) == TREEITEM_BUDDY)
               {
                  BUDDY* buddy = (BUDDY*) nmtv->lParam;

                  string tip = buddy->contact->makeTip( );
                  if (tip.length( ) > (size_t) nmtv->cchTextMax)
                  {
                     assert( nmtv->cchTextMax >= 3 );
                     tip.resize( nmtv->cchTextMax - 3 );
                     tip += "...";
                  }
                  strcpy( nmtv->pszText, tip.c_str( ) );
               }
            }
            return 0;

         default:
            break;
         }
      }
      break;

   case WM_MOUSEMOVE:
      {
         BuddyList* inst = _INST_PTR( );

         if (inst->m_drag.isDragging( ))
         {
            /* Highlight and remember drop targets. */
            TVHITTESTINFO hit;
            hit.pt.x = GET_X_LPARAM( lparam );
            hit.pt.y = GET_Y_LPARAM( lparam );
            ClientToScreen( wnd, &hit.pt );
            ScreenToClient( inst->m_wnd.tree, &hit.pt );
            TreeView_HitTest( inst->m_wnd.tree, &hit );

            if (hit.hItem)
            {
               TVITEMEX item;
               item.mask  = TVIF_PARAM;
               item.hItem = hit.hItem;
               TreeView_GetItem( inst->m_wnd.tree, &item );
               
               if (*((Tree_Item_Type*) item.lParam) == TREEITEM_GROUP)
                  inst->m_drag.drop_group = ((GROUP*) item.lParam)->name;
               else if (*((Tree_Item_Type*) item.lParam) == TREEITEM_BUDDY)
               {
                  GROUP* group;
                  inst->findBuddy( ((BUDDY*) item.lParam)->contact->getName( ), NULL, &group );
                  assert( group );
                  inst->m_drag.drop_group = group->name;
               }

               TreeView_SelectDropTarget( inst->m_wnd.tree, hit.hItem );
            }
            else if (hit.flags & TVHT_NOWHERE) // In the client area but not on an item.
               inst->m_drag.drop_group = "";
         }
      }
      return 0;

   case WM_LBUTTONUP:
      {
         BuddyList* inst = _INST_PTR( );

         if (inst->m_drag.isDragging( ))
         {
            TreeView_SelectDropTarget( inst->m_wnd.tree, NULL );

            /* Move the buddy. */
            inst->addBuddy( inst->m_drag.drag_buddy, inst->m_drag.drop_group );
            inst->writeList( );

            ReleaseCapture( );   // Will trigger a WM_CAPTURECHANGED
            inst->m_drag.drag_buddy.clear( );
         }
      }
      return 0;

   case WM_CAPTURECHANGED:
      {
         BuddyList* inst = _INST_PTR( );

         if (inst->m_drag.isDragging( ))
         {
            inst->m_drag.drag_buddy.clear( );
            TreeView_SelectDropTarget( inst->m_wnd.tree, NULL );
         }
      }
      return 0;
   }

   return DefWindowProc( wnd, msg, wparam, lparam );

#undef _INST_PTR
}

void BuddyList::makeTreeItem( BuddyList::BUDDY* in, TVITEMEX* out ) const
{
   memset( out, 0, sizeof( TVITEMEX ) );
   out->mask = TVIF_PARAM | TVIF_TEXT | TVIF_STATE;
   out->hItem   = in->tree_handle;
   
   out->lParam  = (LPARAM) in;
   string display_name = xml_charset::ascii_to_latin( pipes_to_spaces( in->contact->getName( ) ) );
   strcpy( out->pszText = new char[ display_name.length( ) + 1 ], display_name.c_str( ) );

   out->stateMask = TVIS_STATEIMAGEMASK;
   if (in->contact->getOnline( ))
   {
      if (in->contact->getNearby( ))
         out->state = INDEXTOSTATEIMAGEMASK( IMGL_BUDDYNEARBY );
      else
         out->state = INDEXTOSTATEIMAGEMASK( IMGL_BUDDY );
   }
   else
      out->state = INDEXTOSTATEIMAGEMASK( IMGL_BUDDYOFF );
}

void BuddyList::makeTreeItem( BuddyList::GROUP* in, TVITEMEX* out ) const
{
   string text;
   {
      unsigned int n_total  = (unsigned int) in->buddies.size( );
      unsigned int n_online = 0;
      for (list<BUDDY*>::const_iterator i = in->buddies.begin( );
           i != in->buddies.end( ); ++i)
           n_online += (*i)->contact->getOnline( ) ? 1 : 0;

      ostringstream ss;
      ss << " " << in->name << " ( " << n_online << " / " << n_total << " )";
      text = ss.str( );
   }   
   
   memset( out, 0, sizeof( TVITEMEX ) );
   out->mask = TVIF_PARAM | TVIF_TEXT;
   out->hItem   = in->tree_handle;
   out->lParam  = (LPARAM) in;
   strcpy( out->pszText = new char[ text.length( ) + 1 ], text.c_str( ) );

   /* If the group doesn't exist yet, set the state image. */
   if (!in->tree_handle)
   {
      out->mask |= TVIF_STATE;
      out->stateMask = TVIS_STATEIMAGEMASK;
      out->state = INDEXTOSTATEIMAGEMASK( IMGL_GROUP );
   }
}

void BuddyList::sortTreeItems( )
{
   TVSORTCB tvsort;
   tvsort.lParam      = 0;
   tvsort.lpfnCompare = treeview_cmp;

   /* Sort the root. */
   tvsort.hParent     = TVI_ROOT;
   TreeView_SortChildrenCB( m_wnd.tree, &tvsort, 0 );

   /* Sort each named group. */
   for (list<GROUP*>::const_iterator i = m_groups.begin( );
        i != m_groups.end( ); ++i)
   {
      if ((*i)->name.length( ))
      {
         tvsort.hParent = (*i)->tree_handle;
         TreeView_SortChildrenCB( m_wnd.tree, &tvsort, 0 );
      }
   }
}

void BuddyList::messageGroup( const string& name, bool online_only )
{
   // Find the group.
   for (list<GROUP*>::iterator i = m_groups.begin( ); 
        i != m_groups.end( ); ++i)
   {
      if (!_stricmp( (*i)->name.c_str( ), name.c_str( ) ))
      {
         if (!(*i)->buddies.empty( ))
         {
            // Create a NEW pmSession for all of the buddies.
            PmSession* session = Eggy::singleton( )->pmCreate( );
            
            for (list<BUDDY*>::iterator j = (*i)->buddies.begin( );
                 j != (*i)->buddies.end( ); ++j)
            {
               if (!online_only || (*j)->contact->getOnline( ))
                  session->addRecipient( (*j)->contact->getName( ) );
            }

            if (session->numRecipients( ) > 1)
               session->showRecipientsPane( true );
            session->raise( );
         }
         break;
      }
   }
}

void BuddyList::summonGroup( const string& name )
{
   // Find the group.
   for (list<GROUP*>::iterator i = m_groups.begin( ); 
      i != m_groups.end( ); ++i)
   {
      if (!_stricmp( (*i)->name.c_str( ), name.c_str( ) ))
      {
         if (!(*i)->buddies.empty( ))
         {
            // Autosummon everyone in the group.
            for (list<BUDDY*>::iterator j = (*i)->buddies.begin( );
               j != (*i)->buddies.end( ); ++j)
            {
               Eggy::singleton( )->autoSummon( (*j)->contact->getName( ) );
            }
         }
         break;
      }
   }
}

void BuddyList::summonBuddy( const string& name )
{
   BUDDY* buddy = NULL;
   if (findBuddy( name, &buddy ))
   {
      // Autosummon.
      Eggy::singleton( )->autoSummon( buddy->contact->getName( ) );
   }
}

void BuddyList::joinBuddy( const string& name )
{
   BUDDY* buddy = NULL;
   if (findBuddy( name, &buddy ))
   {
      const char* cmd = c2s_enc_join( ("%" + buddy->contact->getShortName( )).c_str( ) );
      f2_netout( cmd, strlen( cmd ) );
      yiffurc_release( cmd );
   }
}

void BuddyList::writeList( )
{
   HANDLE hf = CreateFile( (string( f2_moddocroot( s_mod ) ) + "\\list.xml").c_str( ), GENERIC_WRITE,
                           0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
   if (hf != INVALID_HANDLE_VALUE)
   {
      ostringstream ss;
      ss << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\r\n"
         << "<buddyList>\r\n";

      for (list<GROUP*>::iterator i = m_groups.begin( ); 
           i != m_groups.end( ); ++i)
      {
         if ((*i)->name.length( )) // proper group
         {
            ss << "\t<group>\r\n"
               << "\t\t<name>" << to_xml_friendly( (*i)->name ) << "</name>\r\n";
            if ((*i)->frozen)
               ss << "\t\t<frozen/>\r\n";
            if (TreeView_GetItemState( m_wnd.tree, (*i)->tree_handle, TVIS_EXPANDED ) & TVIS_EXPANDED)
               ss << "\t\t<expanded/>\r\n";
         }

         for (list<BUDDY*>::iterator j = (*i)->buddies.begin( );
              j != (*i)->buddies.end( ); ++j)
         {
            ss << "\t\t<buddy>\r\n"
               << "\t\t\t<name>" << to_xml_friendly( (*j)->contact->getName( ) )
               << "</name>\r\n"
               << "\t\t\t<desc>" << to_xml_friendly( (*j)->contact->getDesc( ) )
               << "</desc>\r\n"
               << "\t\t\t<lastSeen>" << (*j)->contact->getLastSeen( )
               << "</lastSeen>\r\n"
               << "\t\t</buddy>\r\n";
         }

         if ((*i)->name.length( )) // proper group
         {
            ss << "\t</group>\r\n";
         }
      }

      ss << "</buddyList>\r\n";

      {
         string str = ss.str( );
         DWORD written;
         WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
      }
      CloseHandle( hf );
      Eggy::singleton( )->broadcastSync( );
      return;
   }

   return;   
}

void BuddyList::readList( )
{
   // Prefer the one in the document folder if it exists.
   string filename = string( f2_moddocroot( s_mod ) ) + "\\list.xml";
   if (INVALID_FILE_ATTRIBUTES == GetFileAttributes( filename.c_str( ) ))
      filename = string( f2_modroot( s_mod ) ) + "\\list.xml";

   TiXmlDocument doc( filename );
   if (!doc.LoadFile( ))
   {
      // ErrorId 2 is "Failed to open file."
      if (doc.ErrorId( ) == 2 && INVALID_FILE_ATTRIBUTES != GetFileAttributes( filename.c_str( ) ))
      {
         // File exists but is locked? Try again.
         Eggy::singleton( )->resync( );
      }
      else
         f2_errlog( "Eggy: list.xml file is damaged." );

      return;
   }

   const TiXmlElement* root = doc.RootElement( );
   if (!root || root->ValueStr( ) != "buddyList")
   {
      f2_errspeak( "Eggy: list.xml file is malformed." );
      return;
   }

   // Have to be stealthy, because this can happen during use.
   list<string> expanded_groups;
   for (list<GROUP*>::iterator i = m_groups.begin( );
        i != m_groups.end( ); ++i)
   {
      if ((*i)->name.length( ))
         if (TreeView_GetItemState( m_wnd.tree, (*i)->tree_handle, TVIS_EXPANDED ) & TVIS_EXPANDED)
            expanded_groups.push_back( (*i)->name );
   }

   // Save groups so contacts don't expire.
   list<GROUP*> old_list = m_groups;
   TreeView_DeleteAllItems( m_wnd.tree );
   m_groups.clear( );

   // (Re)populate the list.
   for (const TiXmlElement* e = root->FirstChildElement( ); e; e = e->NextSiblingElement( ))
   {
      if (e->ValueStr( ) == "group")
      {
         string group_name;
         {
            const TiXmlElement* _e = e->FirstChildElement( "name" );
            if (_e && _e->GetText( ))
               group_name = _e->GetText( );
         }

         addGroup( group_name );

         if (group_name.length( )) // Proper group.
         {
            const TiXmlElement* _e = e->FirstChildElement( "frozen" );
            if (_e)
               freezeGroup( group_name, true );
            _e = e->FirstChildElement( "expanded" );
            if (_e)
               expanded_groups.push_back( group_name );
         }

         for (const TiXmlElement* buddy_e = e->FirstChildElement( "buddy" ); buddy_e;
              buddy_e = buddy_e->NextSiblingElement( "buddy" ))
         {
            string buddy_name;
            {
               const TiXmlElement* name = buddy_e->FirstChildElement( "name" );
               if (name && name->GetText( ))
                  buddy_name = name->GetText( );
            }

            if (buddy_name.length( ))
            {
               BUDDY* buddy = addBuddy( buddy_name, group_name );

               if (buddy)
               {
                  const TiXmlElement* _e = buddy_e->FirstChildElement( "desc" );
                  if (_e && _e->GetText( ))
                     buddy->contact->setDesc( _e->GetText( ) );

                  _e = buddy_e->FirstChildElement( "lastSeen" );
                  if (_e && _e->GetText( ))
                     buddy->contact->setLastSeen( atol( _e->GetText( ) ) );
               }
            }
         }
      } // if (e->ValueStr( ) == "group")
      else if (e->ValueStr( ) == "buddy")
      {
         string buddy_name;
         {
            const TiXmlElement* name = e->FirstChildElement( "name" );
            if (name && name->GetText( ))
               buddy_name = name->GetText( );
         }

         if (buddy_name.length( ))
         {
            BUDDY* buddy = addBuddy( buddy_name, "" );

            if (buddy)
            {
               const TiXmlElement* _e = e->FirstChildElement( "desc" );
               if (_e && _e->GetText( ))
                  buddy->contact->setDesc( _e->GetText( ) );

               _e = e->FirstChildElement( "lastSeen" );
               if (_e && _e->GetText( ))
                  buddy->contact->setLastSeen( atol( _e->GetText( ) ) );
            }
         }
      }
   }

   // Now release old contacts.
   while (!old_list.empty( ))
   {
      GROUP* group = old_list.front( );
      old_list.pop_front( );

      while (!group->buddies.empty( ))
      {
         BUDDY* buddy = group->buddies.front( );
         group->buddies.pop_front( );
      
         if (group->frozen)
            buddy->contact->thaw( );
         buddy->contact->release( );
         delete buddy;
      }

      delete group;
   }

   // Expand groups that used to be expanded.
   while (!expanded_groups.empty( ))
   {
      for (list<GROUP*>::iterator i = m_groups.begin( );
           i != m_groups.end( ); ++i)
      {
         if (!_stricmp( (*i)->name.c_str( ), expanded_groups.front( ).c_str( ) ))
            TreeView_Expand( m_wnd.tree, (*i)->tree_handle, TVE_EXPAND );
      }
      expanded_groups.pop_front( );
   }
}

LRESULT CALLBACK BuddyList::treeview_proc( HWND wnd, UINT msg, 
                                           WPARAM wparam, LPARAM lparam )
{
   switch (msg)
   {
   case WM_LBUTTONDBLCLK:
   case WM_LBUTTONDOWN:
   case WM_RBUTTONDOWN:
      TVHITTESTINFO hit;
      hit.pt.x = GET_X_LPARAM( lparam );
      hit.pt.y = GET_Y_LPARAM( lparam );
      TreeView_HitTest( wnd, &hit );

      /* If a left click occurred on the state image of a group,
      expand/collapse it. */
      if ((msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) && 
          hit.hItem && hit.flags == TVHT_ONITEMSTATEICON)
      {
         TreeView_Expand( wnd, hit.hItem, TVE_TOGGLE );
         NMTREEVIEW nmtv;
         memset( &nmtv, 0, sizeof( nmtv ) );
         nmtv.hdr.code = TVN_ITEMEXPANDED;
         nmtv.hdr.hwndFrom = wnd;
         nmtv.action = TVE_TOGGLE;

         nmtv.itemNew.mask = TVIF_PARAM | TVIF_STATE | TVIF_CHILDREN;
         nmtv.itemNew.hItem = hit.hItem;
         TreeView_GetItem( wnd, &nmtv.itemNew );

         if (*((Tree_Item_Type*) nmtv.itemNew.lParam) == TREEITEM_GROUP)
         {
            /* Send TVN_ITEMEXPANDED manually because manual expands don't
            seem to trigger them. */
            SendMessage( GetParent( wnd ), WM_NOTIFY, 0, (LPARAM) &nmtv );
            // Avoid selecting groups if only the state icon was clicked.
            return 0;
         }
      }

      /* Select whatever was clicked right away. */
      TreeView_SelectItem( wnd, hit.hItem );

      break;   // Need to let it through so wm_notify comes up.

   default:
      break;
   }

   return CallWindowProc( s_tvproc, wnd, msg, wparam, lparam );
}

int CALLBACK BuddyList::treeview_cmp( LPARAM a, LPARAM b, LPARAM sort_param )
{
   if (*((Tree_Item_Type*) a) == TREEITEM_GROUP)
   {
      if (*((Tree_Item_Type*) b) == TREEITEM_GROUP)
      {
         // group vs. group
         return _stricmp( ((GROUP*) a)->name.c_str( ), ((GROUP*) b)->name.c_str( ) );
      }
      if (*((Tree_Item_Type*) b) == TREEITEM_BUDDY)
      {
         // group vs. buddy
         return -1;
      }
      
      // group vs. adds.
      return 1;
   }
   if (*((Tree_Item_Type*) a) == TREEITEM_BUDDY)
   {
      if (*((Tree_Item_Type*) b) == TREEITEM_GROUP)
      {
         // buddy vs. group
         return 1;
      }
      if (*((Tree_Item_Type*) b) == TREEITEM_BUDDY)
      {
         // buddy vs. buddy
         if (((BUDDY*) a)->contact->getOnline( ) == ((BUDDY*) b)->contact->getOnline( ))
            return _stricmp( ((BUDDY*) a)->contact->getName( ).c_str( ),
                             ((BUDDY*) b)->contact->getName( ).c_str( ) );
         
         return ((BUDDY*) a)->contact->getOnline( ) ? -1 : 1;
      }

      // buddy vs. adds.
      return 1;
   }
   
   // add vs. anything.
   return -1;
}

//////////////////////////////////////////////////////////////////////////
} // namespace eggy