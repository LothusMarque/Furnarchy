#ifndef _CLIENT_H__
#define _CLIENT_H__

#include "Furnarchy.h"

namespace Furnarchy
{
   using namespace System::Windows::Forms;
   using namespace System::Drawing;

   /// <summary> For use with <see cref='Client::onWndProc' />. </summary>
   private ref class WndProcEventArgs : EventArgs
   {
   public:
      IntPtr  hwnd;
      UInt32  msg;
      IntPtr  wparam;
      IntPtr  lparam;
      IntPtr  retval;
   };

   private ref class HwndWrapper : IWin32Window
   {
   public:
      HwndWrapper( IntPtr win32handle )
      {
         m_win32handle = win32handle;
      }

      property IntPtr Handle
      {
         virtual IntPtr get( ) { return m_win32handle; }
      }
   private:
      IntPtr m_win32handle;
   };

   /// <summary> Arguments for <see cref='WindowCreatedDelegate' />. </summary>
   public ref class WindowCreatedEventArgs : EventArgs
   {
   public:
      IntPtr hwnd;
   };
   /// <summary> Arguments for <see cref='PlayerMenuShowingDelegate' />. </summary>
   public ref class PlayerMenuShowingEventArgs : EventArgs
   {
   public:
      String^ player_name;
   };
   /// <summary> Arguments for <see cref='WindowMessageDelegate' />. </summary>
   public ref class WindowMessageEventArgs : EventArgs
   {
   public:
      IntPtr  hwnd;
      UInt32  msg;
      IntPtr  wparam;
      IntPtr  lparam;
      IntPtr  retval;
      Boolean handled;
   };
   /// <summary> Arguments for <see cref='DrawClientDelegate' />. </summary>
   public ref class DrawClientEventArgs : EventArgs
   {
   public:
      Graphics^ graphics;
      Int32     width;
      Int32     height;
   };

   //////////////////////////////////////////////////////////////////////////

   /// <summary> A client event delegate. </summary>
   /// <seealso cref='ClientSubsystem::WindowCreatedEvent' />
   public delegate void WindowCreatedDelegate( Object^ sender, WindowCreatedEventArgs^ e );
   /// <summary> A client event delegate. </summary>
   /// <seealso cref='ClientSubsystem::PlayerMenuShowingEvent' />
   public delegate void PlayerMenuShowingDelegate( Object^ sender, PlayerMenuShowingEventArgs^ e );
   /// <summary> A client event delegate. </summary>
   /// <seealso cref='ClientSubsystem::WindowMessageEvent' />
   public delegate void WindowMessageDelegate( Object^ sender, WindowMessageEventArgs^ e );
   /// <summary> A client event delegate. </summary>
   /// <seealso cref='ClientSubsystem::DrawClientEvent' />
   public delegate void DrawClientDelegate( Object^ sender, DrawClientEventArgs^ e );

   //////////////////////////////////////////////////////////////////////////

   /// <summary> Encapsulates client hacks and properties. </summary>
   public ref class ClientSubsystem sealed
   {
   private:
      ClientSubsystem( ) { }
   internal:
      ClientSubsystem( ModuleHost^ host );
      ~ClientSubsystem( );

      void onShowingPlayerMenu( );
      void onWindowCreated( IntPtr wnd );
      bool onWndProc( WndProcEventArgs^ params );
      void onDrawClient( IntPtr hdc, Int32 width, Int32 height );
      MenuItem^ findMenuItem( MenuItem^ menu, Int32 id );

   public:

      /// <summary> Cancels the buffered chat badge. </summary>
      /// <remarks>
      ///  Cancels the badge to display with the next line of text.
      /// </remarks>
      void cancelBadge( );
      /// <summary> Attaches a menu to the player right-click menu. </summary>
      /// <param name='menu'> MenuItem to attach. </param>
      /// <remarks>
      ///  <para>
      ///   The player menu is the popup menu that appears when a name or avatar is right-clicked.
      ///   A submenu will be created for each module that attaches a valid MenuItem.
      ///  </para>
      /// </remarks>
      void    attachPlayerMenu( System::Windows::Forms::MenuItem^ menu );
      /// <summary> Detaches a module's player right-click menu. </summary>
      /// <remarks>
      ///   The player menu is the popup menu that appears when a name or avatar is right-clicked.
      /// </remarks>
      void    detachPlayerMenu( );
      /// <summary> Pushes a line to the chat buffer. </summary>
      /// <param name='line'> Line to push. </param>
      /// <remarks>
      ///   This relies on client memlocs. If the build is unsupported, the line will be
      ///   pushed by injecting a <c>'('</c> network line.
      /// </remarks>
      void    pushChatBox( String^ line );
      /// <summary> Ignore a name or channel. </summary>
      /// <param name='name'> Name or channel to ignore. </param>
      /// <remarks>
      ///   <para>
      ///      To ignore a channel, prefix the channel name with a '@' character.
      ///   </para>
      ///   <para>
      ///      This relies on client memlocs and will do nothing if the current build is unsupported.
      ///   </para>
      /// </remarks>
      void    ignoreAdd( String^ name );
      /// <summary> Unignore a name or channel. </summary>
      /// <param name='name'> Name or channel to unignore. </param>
      /// <remarks>
      ///   <para>
      ///      To unignore a channel, prefix the channel name with a '@' character.
      ///   </para>
      ///   <para>
      ///      This relies on client memlocs and will do nothing if the current build is unsupported.
      ///   </para>
      /// </remarks>
      void    ignoreRemove( String^ name );
      /// <summary> Tests if a name or channel is ignored. </summary>
      /// <param name='name'> Name or channel to test. </param>
      /// <remarks>
      ///   <para>
      ///      To test a channel, prefix the channel name with a '@' character.
      ///   </para>
      ///   <para>
      ///      This relies on client memlocs and will do nothing if the current build is unsupported.
      ///   </para>
      /// </remarks>
      /// <returns> <c>true</c> if the name or channel is ignored. </returns>
      Boolean ignoreTest( String^ name );

      /// <summary> Throws an exception if <see cref='BuildOk' /> is false. </summary>
      void    throwBuildOk( )
      {
         if (BuildOk)
            return;
         throw gcnew FurnarchyException( "Memlocs do not support the running client build." );
      }

      /// <summary> Whether or not the client build is supported by Furnarchy. </summary>
      /// <remarks>
      ///   When the client build is not supported, many operations that depend on
      ///   memlocs or hacks will be crippled or disabled entirely.
      /// </remarks>
      property Boolean BuildOk
      {
         Boolean get( )
         {
            return m_build_ok;
         }
      }
      /// <summary> The client program's CRC32. </summary>
      property UInt32 ClientCrc
      {
         UInt32 get( )
         {
            return m_client_crc;
         }
      }
      /// <summary> The HWND of the client's main window. </summary>
      property IntPtr ClientHwnd
      {
         IntPtr get( )
         {
            return m_wnd;
         }
      }

      /// <summary> ClientHwnd wrapped in a IWin32Window (for use with <see cref='Form.Show' />). </summary>
      property IWin32Window^ ClientWindow
      {
         IWin32Window^ get( )
         {
            return gcnew HwndWrapper( m_wnd );
         }
      }
      /// <summary> The name of the player whose player menu (right-click menu) is being displayed for. </summary>
      /// <remarks> 
      ///   This depends on memlocs and will return an empty string if the client build is unsupported. 
      /// </remarks>
      property String^ PlayermenuName
      {
         String^ get( )
         {
            return gcnew String( furn2::f2__playermenu_name( ) );
         }
      }

      /// <summary> The player menu popup is about to be displayed. </summary>
      /// <param name='name'> The name of the player being clicked on. </param>
      event PlayerMenuShowingDelegate^ PlayerMenuShowingEvent;
      /// <summary> The main window has been created. </summary>
      /// <param name='hwnd'> The handle (HWND) if the window. </param>
      event WindowCreatedDelegate^ WindowCreatedEvent;
      /// <summary> A window message is coming through the client's message pump. </summary>
      /// <param name='msg'> Properties of the message, which you may modify. </param>
      /// <remarks>
      ///  To indicate you've handled the message and wish to prevent the client or other 
      ///  modules from processing it, set the <c>handled</c> field to <c>true</c>.
      /// </remarks>
      event WindowMessageDelegate^ WindowMessageEvent;
      /// <summary> The client-area is being drawn. </summary>
      /// <param name='graphics'> A Graphics object for the client-area. </param>
      event DrawClientDelegate^ DrawClientEvent;

   private:

      ModuleHost^      m_host;
      initonly Boolean m_build_ok;
      initonly UInt32  m_client_crc;
      IntPtr           m_wnd;
      MenuItem^        m_player_menu;
   };
}

#endif