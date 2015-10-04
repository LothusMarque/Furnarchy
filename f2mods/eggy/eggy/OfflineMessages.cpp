#include "eggy.h"
#include "furn2/furn2.h"
#include "yiffurc/yiffurc.h"
#include "resource.h"
#include "tinyxml/tinyxml.h"
#include "xml_charset.h"
#include "resource.h"

#include <fstream>
#include <ctime>

namespace eggy {

//////////////////////////////////////////////////////////////////////////

struct OFFLINEEDITPARAM
{
   OfflineMessages* inst;
   unsigned         id;
};

//////////////////////////////////////////////////////////////////////////

static string fst_timestamp( time_t t )
{
   char sz_buf[ 64 ];
   time_t fst_date = t - 6*60*60;
   strftime( sz_buf, 64, "%b %d %I:%M %p FST", gmtime( &fst_date ) );
   return sz_buf;
}

//////////////////////////////////////////////////////////////////////////

void OfflineMessages::class_init( )
{

}

void OfflineMessages::class_uninit( )
{

}

//////////////////////////////////////////////////////////////////////////

OfflineMessages::OfflineMessages( ) : m_wnd( NULL ), m_edit_wnd( NULL ), m_list_menu( NULL ), m_sort( SORT_DATE )
{
   assert( Eggy::singleton( ) );

   m_list_menu = LoadMenu( eggy::s_inst, MAKEINTRESOURCE( IDR_OFFLINEMESSAGES ) );
}

OfflineMessages::~OfflineMessages( )
{
   DestroyMenu( m_list_menu );
   m_list_menu = NULL;
   show( false );
}

unsigned OfflineMessages::add( const string& from, const string& to, const string& msg, time_t date /* = 0 */ )
{
   if (!make_shortname( to ).length( ))
      return -1;

   Contact* contact = Eggy::singleton( )->contactCreate( to );

   if (!contact)
      return -1;

   if (!date)
      date = std::time( NULL );

   unsigned id = nextId( );

   m_messages.push_back( OFFLINEMSG( id, from, contact, msg, date ) );
   contact->release( );
   save( );
   syncUi( );
   return id;
}

bool OfflineMessages::remove( unsigned id )
{
   list<OFFLINEMSG>::iterator i = std::find( m_messages.begin( ), m_messages.end( ), id );   
   if (i == m_messages.end( ))
      return false;
   m_messages.erase( i );
   save( );
   syncUi( );
   return true;
}

void OfflineMessages::clear( )
{
   m_messages.clear( );
   save( );
   syncUi( );
}

list<OfflineMessages::OFFLINEMSG>::iterator OfflineMessages::find( unsigned id )
{
   for (list<OFFLINEMSG>::iterator i = m_messages.begin( ); i != m_messages.end( ); ++i)
      if (i->id == id)
         return i;
   return m_messages.end( );
}

unsigned OfflineMessages::nextId( ) const
{
   unsigned id = 0;
   for (list<OFFLINEMSG>::const_iterator i = m_messages.begin( ); i != m_messages.end( ); ++i)
      if (id <= i->id)
         id = i->id + 1;
   assert( std::find( m_messages.begin( ), m_messages.end( ), id ) == m_messages.end( ) );
   return id;
}

void OfflineMessages::onContact( const string& name, On_Contact how )
{
   if (how == CONTACT_ONLINE /* && Eggy::singleton( )->isEnabled( ) */)
   {
      Contact* contact = Eggy::singleton( )->contactCreate( name );
      if (contact->getOnline( ))
      {
         assert( Eggy::singleton( )->isLoggedIn( ) );

         PLAYERINFO pi;
         f2_myplayerinfo( &pi );
         const string sn = make_shortname( pi.name );
         unsigned send_count = 0;

         list<OFFLINEMSG> outbox;
         for (list<OFFLINEMSG>::const_iterator i = m_messages.begin( ); i != m_messages.end( ); )
         {
            assert( i->to );
            if (make_shortname( i->from ) == sn && contact == i->to)
            {
               outbox.push_back( *i );
               m_messages.erase( i++ );
            }
            else
               ++i;
         }
         
         if (outbox.size( ))
         {
            // Notify the user.
            PmSession* pm = Eggy::singleton( )->pmFind( name );
            string note = "Delivering offline messages for <name shortname='";
            note += make_shortname( name );
            note += "'>";
            note += name;
            note += "</name>...";
            f2_speak( note.c_str( ) );
            if (pm)
            {
               note = "Delivering offline messages for ";
               note += name;
               note += "...";
               pm->doSysMsg( note );
            }

            // Deliver them.
            {
               tm* utc;
               char sz_buf[ 64 ];
               for (list<OFFLINEMSG>::const_iterator i = outbox.begin( ); i != outbox.end( ); ++i)
               {
                  time_t fst_date = i->date - 6*60*60;
                  utc = gmtime( &fst_date );
                  strftime( sz_buf, 64, " -- <i>%b %d %I:%M %p FST</i>", utc );
                  const char* cmd = yiffurc::c2s_enc_whisper( ("%" + i->to->getShortName( ) ).c_str( ), (i->msg + sz_buf).c_str( ) );
                  f2_netoutline( cmd, strlen( cmd ) );
                  yiffurc::yiffurc_release( cmd );
               }
            }

            save( );
            syncUi( );
         }
      }
      contact->release( );
   }
}

bool OfflineMessages::save( )
{
   ostringstream ss;

   ss << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\r\n"
      << "<offlineMessages>\r\n";

   for (list<OFFLINEMSG>::const_iterator i = m_messages.begin( ); i != m_messages.end( ); ++i)
   {
      ss << "\t<message>\r\n";
      ss << "\t\t<date>" << (unsigned) i->date << "</date>\r\n";
      ss << "\t\t<from>" << to_xml_friendly( i->from ) << "</from>\r\n";
      ss << "\t\t<to>" << to_xml_friendly( i->to->getName( ) ) << "</to>\r\n";
      ss << "\t\t<msg>" << to_xml_friendly( i->msg ) << "</msg>\r\n";
      ss << "\t</message>\r\n";
   }

   ss << "</offlineMessages>\r\n";

   HANDLE hf = CreateFile( (string( f2_moddocroot( s_mod ) ) + "\\offline.xml").c_str( ), GENERIC_WRITE, 0, NULL, 
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
   if (hf != INVALID_HANDLE_VALUE)
   {
      string str = ss.str( );
      DWORD written;
      WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
      CloseHandle( hf );
      Eggy::singleton( )->broadcastSync( );
      return true;
   }

   return false;
}

bool OfflineMessages::load( )
{
   // Prefer the one in the document folder if it exists.
   string filename = string( f2_moddocroot( s_mod ) ) + "\\offline.xml";
   if (INVALID_FILE_ATTRIBUTES == GetFileAttributes( filename.c_str( ) ))
      filename = string( f2_modroot( s_mod ) ) + "\\offline.xml";

   TiXmlDocument doc( filename );
   if (!doc.LoadFile( ) || doc.Error( ))
   {
      // ErrorId 2 is "Failed to open file."
      if (doc.ErrorId( ) == 2 && INVALID_FILE_ATTRIBUTES != GetFileAttributes( filename.c_str( ) ))
      {
         // File exists but is locked? Try again.
         Eggy::singleton( )->resync( );
      }
      else
         f2_errlog( "Eggy: offline.xml file is damaged." );

      return false;
   }

   const TiXmlElement* root = doc.RootElement( );
   if (!root || root->ValueStr( ) != "offlineMessages")
   {
      f2_errspeak( "Eggy: offline.xml file is malformed." );
      return false;
   }

   // Reset.
   m_messages.clear( );

   /* Recreate. */
   for (const TiXmlElement* e_message = root->FirstChildElement( "message" ); 
        e_message; e_message = e_message->NextSiblingElement( "message" ))
   {
      time_t date = 0;
      string from, to, msg;
      const TiXmlElement* e = e_message->FirstChildElement( "date" );
      if (e && e->GetText( ))
         date = atol( e->GetText( ) );
      e = e_message->FirstChildElement( "from" );
      if (e && e->GetText( ))
         from = e->GetText( );
      e = e_message->FirstChildElement( "to" );
      if (e && e->GetText( ))
         to = e->GetText( );
      e = e_message->FirstChildElement( "msg" );
      if (e && e->GetText( ))
         msg = e->GetText( );

      if (make_shortname( from ).length( ) && make_shortname( to ).length( ) && msg.length( ))
      {
         Contact* contact = Eggy::singleton( )->contactCreate( to );
         if (contact)
         {
            m_messages.push_back( OFFLINEMSG( nextId( ), from, contact, msg, date ) );
            contact->release( );
         }
      }
   }

   /* Update the UI. */
   syncUi( );

   return true;
}

void OfflineMessages::show( bool showhide )
{
   if (showhide)
   {
      if (m_wnd)
      {
         ShowWindow( m_wnd, SW_SHOWNORMAL );
         SetForegroundWindow( m_wnd );
      }
      else
      {
         // m_wnd will be set in WM_INITDIALOG
         CreateDialogParam( eggy::s_inst, MAKEINTRESOURCE( IDD_OFFLINEMESSAGES ), 
                            NULL, dlg_proc, (LPARAM) this );
         if (m_wnd)
         {
            m_sort = SORT_DATE;
            syncUi( );
            ShowWindow( m_wnd, SW_SHOWNORMAL );
            SetForegroundWindow( m_wnd );
         }
      }
   }
   else
   {
      if (m_wnd)
      {
         DestroyWindow( m_wnd );
         assert( m_wnd == NULL );
      }
   }
}

void OfflineMessages::beginEdit( unsigned id )
{
   if (m_edit_wnd)
   {
      DestroyWindow( m_edit_wnd );
      assert( !m_edit_wnd );
   }
   
   OFFLINEEDITPARAM param = { this, id };
   CreateDialogParam( eggy::s_inst, MAKEINTRESOURCE( IDD_OFFLINEMESSAGEEDIT ),
                      m_wnd, edit_dlg_proc, (LPARAM) &param );
   if (m_edit_wnd)
   {
      ShowWindow( m_edit_wnd, SW_SHOWNORMAL );
      SetForegroundWindow( m_edit_wnd );
   }
}

void OfflineMessages::syncUi( )
{
   if (m_wnd)
   {
      HWND lv = GetDlgItem( m_wnd, IDC_OFFLINEMESSAGES );
      assert( lv );
      
      POINT origin;
      ListView_GetOrigin( lv, &origin );
      ListView_DeleteAllItems( lv );
      LVITEM item;
      memset( &item, 0,sizeof( item ) );
      item.mask = LVIF_PARAM;
      
      for (list<OFFLINEMSG>::const_iterator i = m_messages.begin( ); i != m_messages.end( ); ++i)
      {
         item.lParam   = (LPARAM) i->id;
         item.iItem = 0;
         item.iItem    = ListView_InsertItem( lv, &item );

         ListView_SetItemText( lv, item.iItem, 0, const_cast<char*>( i->to->getName( ).c_str( ) ) );
         ListView_SetItemText( lv, item.iItem, 1, const_cast<char*>( i->from.c_str( ) ) );
         ListView_SetItemText( lv, item.iItem, 2, const_cast<char*>( i->msg.c_str( ) ) );
         string timestamp = fst_timestamp( i->date );
         ListView_SetItemText( lv, item.iItem, 3, const_cast<char*>( timestamp.c_str( ) ) );
      }

      ListView_SortItems( lv, list_sort, (LPARAM) this );
      ListView_Scroll( lv, origin.x, origin.y );

      std::ostringstream ss;
      ss << ListView_GetItemCount( lv ) << " total.";
      SetDlgItemText( m_wnd, IDC_OFFLINEMESSAGESTOTAL, ss.str( ).c_str( ) );
   }
}

INT_PTR OfflineMessages::dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((OfflineMessages*) GetWindowLongPtr( wnd, GWLP_USERDATA ))
   switch (msg)
   {
   case WM_INITDIALOG:
      {
         SetWindowLongPtr( wnd, GWLP_USERDATA, (LONG_PTR) lparam );
         OfflineMessages* inst = _INST_PTR( );
         inst->m_wnd = wnd;
         SetClassLongPtr( wnd, GCLP_HICON, (LONG_PTR) Eggy::singleton( )->getIcon( ) );

         /* Center inside the parent's client area. */
         {
            HWND parent = GetParent( wnd );
            if (!parent)
               parent = Eggy::singleton( )->getParentWnd( );
            RECT parent_rect;
            GetWindowRect( parent, &parent_rect );
            parent_rect.right = parent_rect.right - parent_rect.left;
            parent_rect.bottom = parent_rect.bottom - parent_rect.top;
            RECT dlg_rect;
            GetWindowRect( wnd, &dlg_rect );
            dlg_rect.right = dlg_rect.right - dlg_rect.left;
            dlg_rect.bottom = dlg_rect.bottom - dlg_rect.top;
            POINT pt = { 
                          parent_rect.left + (parent_rect.right - dlg_rect.right) / 2,
                          parent_rect.top + (parent_rect.bottom - dlg_rect.bottom) / 2
                       };
            SetWindowPos( wnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
         }

         {
            HWND ctrl = GetDlgItem( wnd, IDC_OFFLINEMESSAGES );
            ListView_SetExtendedListViewStyleEx( ctrl, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT );
            LVCOLUMN col;
            memset( &col, 0, sizeof( col ) );
            col.mask     = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
            
            col.iSubItem = 0;
            col.cx = 60;
            col.pszText  = "To";
            ListView_InsertColumn( ctrl, 0, &col );

            col.iSubItem = 1;
            col.cx = 60;
            col.pszText  = "From";
            ListView_InsertColumn( ctrl, 1, &col );

            col.iSubItem = 2;
            col.cx = 200;
            col.pszText  = "Message";
            ListView_InsertColumn( ctrl, 2, &col );

            col.iSubItem = 3;
            col.cx = 120;
            col.pszText  = "Time";
            ListView_InsertColumn( ctrl, 3, &col );
         }

         SetFocus( GetDlgItem( wnd, IDC_OFFLINEMESSAGES ) );
         return FALSE;
      }
   case WM_COMMAND:
      if (lparam)
      {
         switch (HIWORD( wparam ))
         {
         case BN_CLICKED:
            if (LOWORD( wparam ) == IDOK)
            {
               DestroyWindow( wnd );
               return TRUE;
            }
            break;
         }
      }
      else if (HIWORD( wparam )== 0) // Menu.
      {
         switch (LOWORD( wparam ))
         {
         case ID_OFFLINEMESSAGES_ADD:
            {
               OfflineMessages* inst = _INST_PTR( );
               assert( inst );
               inst->beginEdit( -1 );
               return TRUE;
            }
            break;
         case ID_OFFLINEMESSAGES_EDIT:
            {
               OfflineMessages* inst = _INST_PTR( );
               assert( inst );

               HWND lv = GetDlgItem( wnd, IDC_OFFLINEMESSAGES );
               LVITEM item;
               item.mask     = LVIF_PARAM;
               item.iItem    = ListView_GetSelectionMark( lv );
               item.iSubItem = 0;
               if (ListView_GetItem( lv, &item ))
                  inst->beginEdit( (unsigned) item.lParam );
               return TRUE;
            }
            break;
         case ID_OFFLINEMESSAGES_REMOVE:
            {
               OfflineMessages* inst = _INST_PTR( );
               assert( inst );

               HWND lv = GetDlgItem( wnd, IDC_OFFLINEMESSAGES );
               LVITEM item;
               item.mask     = LVIF_PARAM;
               item.iItem    = ListView_GetSelectionMark( lv );
               item.iSubItem = 0;
               if (ListView_GetItem( lv, &item ))
                  inst->remove( (unsigned) item.lParam );
               return TRUE;
            }
            break;
         case ID_OFFLINEMESSAGES_CLEARALL:
            {
               OfflineMessages* inst = _INST_PTR( );
               assert( inst );
               inst->clear( );
               return TRUE;
            }
         }
      }
      break;
   case WM_NOTIFY:
      if (((NMHDR*)lparam)->idFrom = IDC_OFFLINEMESSAGES)
      {
         switch (((NMHDR*)lparam)->code)
         {
         case LVN_COLUMNCLICK:
            {
               OfflineMessages* inst = _INST_PTR( );
               if (inst)
               {
                  switch (((NMLISTVIEW*)lparam)->iSubItem)
                  {
                  case 0:
                     inst->m_sort = SORT_TO;
                     break;
                  case 1:
                     inst->m_sort = SORT_FROM;
                     break;
                  case 2:
                     inst->m_sort = SORT_MSG;
                     break;
                  case 3:
                     inst->m_sort = SORT_DATE;
                     break;
                  }
                  ListView_SortItems( ((NMLISTVIEW*)lparam)->hdr.hwndFrom, list_sort, (LPARAM) inst );
                  return TRUE;
               }
            }
            break;
         case NM_RCLICK:
            {
               OfflineMessages* inst = _INST_PTR( );
               if (inst)
               {
                  NMITEMACTIVATE* nm = (NMITEMACTIVATE*) lparam;
                  POINT cursor;
                  GetCursorPos( &cursor );
                  if (nm->iItem != -1)
                  {
                     TrackPopupMenu( GetSubMenu( inst->m_list_menu, 0 ), TPM_RIGHTBUTTON, 
                                     cursor.x, cursor.y, 0, wnd, NULL );
                  }
                  else // Not on an item.
                  {
                     TrackPopupMenu( GetSubMenu( inst->m_list_menu, 1 ), TPM_RIGHTBUTTON, 
                                     cursor.x, cursor.y, 0, wnd, NULL );
                  }
                  return TRUE;
               }
            }
            break;
         case NM_DBLCLK:
            {
               OfflineMessages* inst = _INST_PTR( );
               if (inst)
               {
                  NMITEMACTIVATE* nm = (NMITEMACTIVATE*) lparam;
                  if (nm->iItem != -1)
                     inst->beginEdit( nm->iItem );
                  return TRUE;
               }
            }
            break;
         }
      }
      break;
   case WM_CLOSE:
      DestroyWindow( wnd );
      break;
   case WM_DESTROY:
      {
         OfflineMessages* inst = _INST_PTR( );
         if (inst)
         {
            inst->m_wnd = NULL;
         }
      }
      break;
   case WM_ACTIVATE:
      if (wparam != WA_INACTIVE)
         f2_setcurdialog( wnd );
   }
   return FALSE;
#undef _INST_PTR
}

INT_PTR OfflineMessages::edit_dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((OfflineMessages*) GetWindowLongPtr( wnd, GWLP_USERDATA ))
   switch (msg)
   {
   case WM_INITDIALOG:
      {
         SetWindowLongPtr( wnd, GWLP_USERDATA, (LONG_PTR) ((OFFLINEEDITPARAM*)lparam)->inst );
         OfflineMessages* inst = ((OFFLINEEDITPARAM*)lparam)->inst;
         inst->m_edit_wnd = wnd;
         SetClassLongPtr( wnd, GCLP_HICON, (LONG_PTR) Eggy::singleton( )->getIcon( ) );

         /* Center inside the parent's client area. */
         {
            HWND parent = GetParent( wnd );
            if (!parent)
               parent = Eggy::singleton( )->getParentWnd( );
            RECT parent_rect;
            GetWindowRect( parent, &parent_rect );
            parent_rect.right = parent_rect.right - parent_rect.left;
            parent_rect.bottom = parent_rect.bottom - parent_rect.top;
            RECT dlg_rect;
            GetWindowRect( wnd, &dlg_rect );
            dlg_rect.right = dlg_rect.right - dlg_rect.left;
            dlg_rect.bottom = dlg_rect.bottom - dlg_rect.top;
            POINT pt = { 
                          parent_rect.left + (parent_rect.right - dlg_rect.right) / 2,
                          parent_rect.top + (parent_rect.bottom - dlg_rect.bottom) / 2
                       };
            SetWindowPos( wnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
         }

         if (((OFFLINEEDITPARAM*)lparam)->id != -1)
         {
            list<OFFLINEMSG>::const_iterator i = inst->find( ((OFFLINEEDITPARAM*)lparam)->id );
            if (i != inst->m_messages.end( ))
            {
               SetDlgItemText( wnd, IDC_OFFLINEMESSAGEFROM, i->from.c_str( ) );
               SetDlgItemText( wnd, IDC_OFFLINEMESSAGETO, i->to->getName( ).c_str( ) );
               SetDlgItemText( wnd, IDC_OFFLINEMESSAGEMSG, i->msg.c_str( ) );
               SetDlgItemInt( wnd, IDC_OFFLINEMESSAGEID, i->id, TRUE );
               SetFocus( GetDlgItem( wnd, IDC_OFFLINEMESSAGEMSG ) );
               return FALSE;
            }
         }

         PLAYERINFO pi;
         f2_myplayerinfo( &pi );
         SetDlgItemText( wnd, IDC_OFFLINEMESSAGEFROM, pi.name );
         SetDlgItemInt( wnd, IDC_OFFLINEMESSAGEID, -1, TRUE );
         SetFocus( GetDlgItem( wnd, IDC_OFFLINEMESSAGETO ) );
         return FALSE;
      }
   case WM_COMMAND:
      if (lparam)
      {
         switch (HIWORD( wparam ))
         {
         case BN_CLICKED:
            if (LOWORD( wparam ) == IDOK)
            {
               OfflineMessages* inst = _INST_PTR( );
               char* buf = new char[ 4096 ];
               unsigned id = GetDlgItemInt( wnd, IDC_OFFLINEMESSAGEID, NULL, TRUE );
               GetDlgItemText( wnd, IDC_OFFLINEMESSAGEFROM, buf, 4096 );
               string from = buf;
               GetDlgItemText( wnd, IDC_OFFLINEMESSAGETO, buf, 4096 );
               string to = buf;
               GetDlgItemText( wnd, IDC_OFFLINEMESSAGEMSG, buf, 4096 );
               string msg = buf;
               
               if (id != -1)
               {
                  list<OFFLINEMSG>::iterator i = inst->find( id );
                  if (i != inst->m_messages.end( ))
                  {
                     i->from = from;
                     Contact* c = i->to;
                     i->to = Eggy::singleton( )->contactCreate( to );
                     c->release( );
                     i->msg  = msg;
                     i->date = time( NULL );
                     inst->save( );
                     inst->syncUi( );
                     DestroyWindow( wnd );
                     return TRUE;
                  }
               }                  
               inst->add( from, to, msg );
               DestroyWindow( wnd );
               return TRUE;
            }
            else if (LOWORD( wparam ) == IDCANCEL)
            {
               DestroyWindow( wnd );
               return TRUE;
            }
            break;
         }
      }
      break;
   case WM_CLOSE:
      DestroyWindow( wnd );
      break;
   case WM_DESTROY:
      {
         OfflineMessages* inst = _INST_PTR( );
         if (inst)
         {
            inst->m_edit_wnd = NULL;
         }
      }
      break;
   case WM_ACTIVATE:
      if (wparam != WA_INACTIVE)
         f2_setcurdialog( wnd );
   }
   return FALSE;
#undef _INST_PTR
}

int CALLBACK OfflineMessages::list_sort( LPARAM a, LPARAM b, LPARAM sort_param )
{
   OfflineMessages* inst = (OfflineMessages*) sort_param;
   assert( inst );

   list<OFFLINEMSG>::iterator _a = inst->find( a );
   list<OFFLINEMSG>::iterator _b = inst->find( b );

   if (_a != inst->m_messages.end( ) && _b != inst->m_messages.end( ))
   {
      switch (inst->m_sort)
      {
      case SORT_FROM:
         return _stricmp( _a->from.c_str( ), _b->from.c_str( ) );
      case SORT_TO:
         return _stricmp( _a->to->getName( ).c_str( ), _b->to->getName( ).c_str( ) );
      case SORT_MSG:
         return _stricmp( _a->msg.c_str( ), _b->msg.c_str( ) );
      default:
         return (int) (_b->date - _a->date);
      }
   }
   return 0;   
}

//////////////////////////////////////////////////////////////////////////

} // namespace eggy