#include "Furnarchy.h"

namespace Furnarchy
{

ClientSubsystem::ClientSubsystem( ModuleHost^ host )
   : m_host( host )
{
   m_build_ok   = furn2::f2_buildok( );
   m_wnd        = (IntPtr) furn2::f2_getwnd( );
   m_client_crc = furn2::f2_clcrc( );
   m_player_menu = nullptr;
}

ClientSubsystem::~ClientSubsystem( )
{

}

void ClientSubsystem::attachPlayerMenu( System::Windows::Forms::MenuItem^ menu )
{
   m_player_menu = menu;
   furn2::f2_playermenu_set( m_host->Id, (HMENU) m_player_menu->Handle.ToPointer( ) );
}

void ClientSubsystem::cancelBadge( )
{
   furn2::f2_badge_cancel( );
}

void ClientSubsystem::detachPlayerMenu( )
{
   m_player_menu = nullptr;
   furn2::f2_playermenu_unset( m_host->Id );
}

void ClientSubsystem::ignoreAdd( String^ name )
{
   furn2::f2__ignore_add( CliUtil::cliStrToAscii( name ).c_str( ) );
}

void ClientSubsystem::ignoreRemove( String^ name )
{
   furn2::f2__ignore_rm( CliUtil::cliStrToAscii( name ).c_str( ) );
}

Boolean ClientSubsystem::ignoreTest( String^ name )
{
   return furn2::f2__ignore_test( CliUtil::cliStrToAscii( name ).c_str( ) );
}

void ClientSubsystem::onShowingPlayerMenu( )
{
   PlayerMenuShowingEventArgs e;
   const char* sz_name = furn2::f2__playermenu_name( );
   if (sz_name)
   {
      e.player_name = gcnew String( sz_name );
      PlayerMenuShowingEvent( this, %e );
   }

   // In case the module modified its player menu, we need
   // to re-attach it or else it won't display.
   if (m_player_menu != nullptr)
      furn2::f2_playermenu_set( m_host->Id, (HMENU) m_player_menu->Handle.ToPointer( ) );
}

void ClientSubsystem::onWindowCreated( IntPtr wnd )
{
   m_wnd = wnd;
   WindowCreatedEventArgs e;
   e.hwnd = m_wnd;
   WindowCreatedEvent( this, %e );
}

bool ClientSubsystem::onWndProc( WndProcEventArgs^ params )
{
   WindowMessageEventArgs e;
   e.hwnd = params->hwnd;
   e.msg = params->msg;
   e.wparam = params->wparam;
   e.lparam = params->lparam;
   e.retval = params->retval;
   e.handled = false;

   WindowMessageEvent( this, %e );
   if (e.handled)
   {
      params->hwnd = e.hwnd;
      params->msg = e.msg;
      params->wparam = e.wparam;
      params->lparam = e.lparam;
      params->retval = e.retval;
      return true;
   }

   // Gotta handle WM_COMMANDs to reroute player menu events to onClicks.
   if (m_player_menu != nullptr)
   {
      if (params->msg == WM_COMMAND && (LPARAM) params->lparam.ToPointer( ) == 0 && 
          HIWORD( (WPARAM) params->wparam.ToPointer( ) ) == 0)
      {
         System::Windows::Forms::MenuItem^ m = findMenuItem( m_player_menu, LOWORD( (WPARAM) params->wparam.ToPointer( ) ) );
         if (m != nullptr)
         {
            m->PerformClick( );
            params->retval = IntPtr( 0 );
            return true;
         }
      }
   }

   return false;
}

void ClientSubsystem::onDrawClient( IntPtr hdc, Int32 width, Int32 height )
{
   DrawClientEventArgs e;
   e.graphics = Graphics::FromHdc( hdc );
   e.width = width;
   e.height = height;
   DrawClientEvent( this, %e );
   delete e.graphics;
}

void ClientSubsystem::pushChatBox( String^ line )
{
   furn2::f2__pushchatbox( CliUtil::cliStrToAscii( line ).c_str( ) );
}

MenuItem^ ClientSubsystem::findMenuItem( MenuItem^ menu, Int32 id )
{   
   for (Int32 i = 0; i < menu->MenuItems->Count; ++i)
   {
      MenuItem^ item = menu->MenuItems[ i ];
      if (!item->IsParent)
      {
         if (GetMenuItemID( (HMENU) menu->Handle.ToPointer( ), i ) == id)
            return item;
      }
      else
         findMenuItem( item, id );
   }
   return nullptr;
}

} // namespace Furnarchy