#include "Furnarchy.h"

namespace Furnarchy
{

ModuleHost::ModuleHost( UInt32 id )
: m_my_id( id )
{
   furn2::F2VERSION ver;
   furn2::f2_version( &ver );
   m_version.major    = ver.major;
   m_version.minor    = ver.minor;
   m_version.revision = ver.revision;

   m_single_user_mode       = furn2::f2_singleusermode( );
   m_my_state               = furn2::f2_modstate( id ) & furn2::MOD_ENABLED ? Module_State::Enabled : Module_State::Disabled;
   m_my_name                = gcnew String( furn2::f2_modname( id ) );
   m_my_fancy_name          = gcnew String( furn2::f2_modfancyname( id ) );   
   m_client_data_folder     = gcnew String( furn2::f2_cldataroot( ) );
   m_client_document_folder = gcnew String( furn2::f2_cldocroot( ) );
   m_client_folder          = gcnew String( furn2::f2_clroot( ) );
   m_f2_data_folder         = gcnew String( furn2::f2_dataroot( ) );
   m_f2_document_folder     = gcnew String( furn2::f2_docroot( ) );
   m_f2_program_folder      = gcnew String( furn2::f2_root( ) );
   m_my_data_folder         = gcnew String( furn2::f2_moddataroot( id ) );
   m_my_document_folder     = gcnew String( furn2::f2_moddocroot( id ) );
   m_my_program_folder      = gcnew String( furn2::f2_modroot( id ) );
   m_my_thumbnail           = gcnew String( furn2::f2_modthumb( id ) );

   Client     = gcnew ClientSubsystem( this );
   Net        = gcnew NetSubsystem( this );
   Scripts    = gcnew ScriptsSubsystem( this );
   World      = gcnew WorldSubsystem( this );

   m_broadcast_msg = RegisterWindowMessageW( L"F2AdapterCLIBroadcastValueMsg" );
}


bool ModuleHost::onCommand( int argc, const char* argv[] )
{
   array<String^>^ args = gcnew array<String^>( argc );
   for (int i = 0; i < argc; ++i)
      args[ i ] = gcnew String( argv[ i ] );

   CommandEventArgs e;
   e.args = args;
   e.handled = false;
   CommandEvent( this, %e );
   return e.handled;
}


void ModuleHost::onInbound( furn2::Revision rev, const char* line, int from, size_t num )
{
   if (rev == furn2::REV_REVISING)
   {
      InboundEventArgs e;
      e.line = CliUtil::multiByteStrToCliBytes( line );
      e.count = (UInt32) num;
      int hf = furn2::f2_hideflags( );
      if (hf & furn2::HIDE_CLIENT)
         e.hide_flags = e.hide_flags | Hide_Flags::Client;
      if (hf & furn2::HIDE_FILTERS)
         e.hide_flags = e.hide_flags | Hide_Flags::Filters;
      if (hf & furn2::HIDE_MODULES)
         e.hide_flags = e.hide_flags | Hide_Flags::Modules;
      if (hf & furn2::HIDE_SCRIPTS)
         e.hide_flags = e.hide_flags | Hide_Flags::Scripts;
      if (hf & furn2::HIDE_WORLD)
         e.hide_flags = e.hide_flags | Hide_Flags::World;

      Net->onInbound( %e );

      hf = 0;
      if ((e.hide_flags & Hide_Flags::Client) == Hide_Flags::Client)
         hf |= furn2::HIDE_CLIENT;
      if ((e.hide_flags & Hide_Flags::Filters) == Hide_Flags::Filters)
         hf |= furn2::HIDE_FILTERS;
      if ((e.hide_flags & Hide_Flags::Modules) == Hide_Flags::Modules)
         hf |= furn2::HIDE_MODULES;
      if ((e.hide_flags & Hide_Flags::Scripts) == Hide_Flags::Scripts)
         hf |= furn2::HIDE_SCRIPTS;
      if ((e.hide_flags & Hide_Flags::World) == Hide_Flags::World)
         hf |= furn2::HIDE_WORLD;
      furn2::f2_hideline( hf );

      furn2::f2_reviseline( CliUtil::CliBytesToMultiByteStr( e.line ).c_str( ) );
   }
}

intptr_t ModuleHost::onModule( furn2::Module_Event e, intptr_t param )
{
   switch (e)
   {
   case furn2::MOD_EVENT_ENABLE:
      m_my_state = m_my_state | Module_State::Enabled;
      EnabledEvent( this, gcnew EventArgs );
      return 0;
   case furn2::MOD_EVENT_DISABLE:
      m_my_state = m_my_state & (~Module_State::Enabled);
      DisabledEvent( this, gcnew EventArgs );
      return 0;
/*
   case furn2::MOD_EVENT_START:
      StartEvent( this, gcnew EventArgs );
      return 0;
   case furn2::MOD_EVENT_STOP:
      StopEvent( this, gcnew EventArgs );
      return 0;*/

   case furn2::MOD_EVENT_TOUCH:
      TouchedEvent( this, gcnew EventArgs );
      return 0;
   }
   return 0;
}

intptr_t ModuleHost::onClient( furn2::Client_Event e, intptr_t param )
{
   switch (e)
   {
   case furn2::CL_EVENT_PLAYERMENUSHOW:
      Client->onShowingPlayerMenu( );
      return 0;
   case furn2::CL_EVENT_WNDCREATED:
      Client->onWindowCreated( (IntPtr) (void*) param );
      return 0;
   case furn2::CL_EVENT_SCRIPT:
      Scripts->onScript( );
      return 0;
   case furn2::CL_EVENT_RENDER:
      Client->onDrawClient( (IntPtr) ((furn2::RENDERPARAMS*) param)->dc, ((furn2::RENDERPARAMS*) param)->width, ((furn2::RENDERPARAMS*) param)->height );
      return 0;
   case furn2::CL_EVENT_TICK:
      {
         TickEventArgs e;
         e.dt = (UInt32) param;
         TickEvent( this, %e );
      }
      return 0;
   }
   return 0;
}

void ModuleHost::onOutbound( furn2::Revision rev, const char* line, int from, size_t num )
{
   if (rev == furn2::REV_REVISING)
   {
      OutboundEventArgs e;
      e.line = CliUtil::multiByteStrToCliBytes( line );
      e.count = (UInt32) num;
      int hf = furn2::f2_hideflags( );
      if (hf & furn2::HIDE_CLIENT)
         e.hide_flags = e.hide_flags | Hide_Flags::Client;
      if (hf & furn2::HIDE_FILTERS)
         e.hide_flags = e.hide_flags | Hide_Flags::Filters;
      if (hf & furn2::HIDE_MODULES)
         e.hide_flags = e.hide_flags | Hide_Flags::Modules;
      if (hf & furn2::HIDE_SCRIPTS)
         e.hide_flags = e.hide_flags | Hide_Flags::Scripts;
      if (hf & furn2::HIDE_WORLD)
         e.hide_flags = e.hide_flags | Hide_Flags::World;
      
      Net->onOutbound( %e );

      hf = 0;
      if ((e.hide_flags & Hide_Flags::Client) == Hide_Flags::Client)
         hf |= furn2::HIDE_CLIENT;
      if ((e.hide_flags & Hide_Flags::Filters) == Hide_Flags::Filters)
         hf |= furn2::HIDE_FILTERS;
      if ((e.hide_flags & Hide_Flags::Modules) == Hide_Flags::Modules)
         hf |= furn2::HIDE_MODULES;
      if ((e.hide_flags & Hide_Flags::Scripts) == Hide_Flags::Scripts)
         hf |= furn2::HIDE_SCRIPTS;
      if ((e.hide_flags & Hide_Flags::World) == Hide_Flags::World)
         hf |= furn2::HIDE_WORLD;
      furn2::f2_hideline( hf );

      furn2::f2_reviseline( CliUtil::CliBytesToMultiByteStr( e.line ).c_str( ) );
   }
}

bool ModuleHost::onWndProc( furn2::WNDPROCPARAMS* params )
{
   if (params->msg == m_broadcast_msg && params->wparam == m_my_id)
   {
      BroadcastValueEventArgs e;
      e.value = (IntPtr) params->lparam;
      BroadcastValueEvent( this, %e );
      params->retval = 0;
      return true;
   }

   WndProcEventArgs e;
   e.hwnd = (IntPtr) (void*) params->wnd;
   e.msg = (UInt32) params->msg;
   e.wparam = (IntPtr) (void*) params->wparam;
   e.lparam = (IntPtr) (void*) params->lparam;
   e.retval = (IntPtr) (void*) params->retval;
   if (Client->onWndProc( %e ))
   {
      params->wnd = (HWND) e.hwnd.ToPointer( );
      params->msg = e.msg;
      params->wparam = (WPARAM) e.wparam.ToPointer( );
      params->lparam = (WPARAM) e.lparam.ToPointer( );
      params->retval = (LRESULT) e.retval.ToPointer( );
      return true;
   }
   return false;
}

intptr_t ModuleHost::onWorld( furn2::World_Event e, intptr_t param )
{
   switch (e)
   {
   case furn2::WO_EVENT_CONNECTED:
      Net->onConnected( );
      return 0;
   case furn2::WO_EVENT_DISCONNECTED:
      Net->onDisconnected( );
      return 0;
   case furn2::WO_EVENT_LOGGEDIN:
      Net->onLoggedIn( );
      return 0;
   case furn2::WO_EVENT_MONSTER:
      World->onMonster( (UInt32) param );
      return 0;
   case furn2::WO_EVENT_PLAYERINFO:
      World->onPlayerInfo( );
      return 0;
   }
   return 0;
}

} // namespace Furnarchy