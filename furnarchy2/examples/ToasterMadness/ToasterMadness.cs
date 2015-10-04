using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;

/************************************************************************/
/* ToasterMadness.cs                                                    */
/* Attaches a submenu to the right-click player context menu to turn    */
/* people into toasters!                                                */
/*                                                                      */
/* Compile as a .NET 2.0 class library with                             */
/* C:\Program Files\Furnarchy2\adapters\cli.dll                         */
/* and System.Windows.Forms as a reference.                             */
/* But make sure the cli.dll reference is not set to "Copy Local".      */
/* Your module class will fail to be constructed if you ship with       */
/* a copy of cli.dll in your module's folder!                           */
/* Finally, ensure you have a properly formed module.ini file           */
/* with 'adapter=cli' in your module's folder to get furnarchy to load  */
/* it as a CLI module.                                                  */
/************************************************************************/

namespace ToasterMadness
{
   // Your module's main class must implement the IFurnarchyModule interface.
   public class EmptyModule : Furnarchy.IFurnarchyModule
   {
      //////////////////////////////////////////////////////////////////////////

      // Your module's main class must have a constructor that takes
      // ONLY a ModuleHost object.
      public EmptyModule( Furnarchy.ModuleHost host )
      {
         m_host = host;

         // Our list of toasters.
         m_toasters = new LinkedList<Toaster>( );

         // Create our player context submenu you see when you right-click 
         // on an avatar.
         m_player_menu = new MenuItem( );
         m_player_menu.MenuItems.Add( "Toaster!" ).Click += new EventHandler( onMenuToasterClick );
        
         // Attach it to Furc's player context menu.
         m_host.Client.attachPlayerMenu( m_player_menu );

         /* Subscribe to events. */

         // We need to intercept avatar creation, movement, and update commands to make
         // sure people are created as and remain toasters. The Net subsystem
         // exposes these events.
         m_host.Net.InboundAvatarCreateEvent += new Furnarchy.InboundAvatarCreateDelegate( onInboundAvatarCreate );
         m_host.Net.InboundAvatarMoveEvent += new Furnarchy.InboundAvatarMoveDelegate( onInboundAvatarMove );
         m_host.Net.InboundAvatarUpdateEvent += new Furnarchy.InboundAvatarUpdateDelegate( onInboundAvatarUpdate );
         m_host.Net.InboundAvatarShowEvent += new Furnarchy.InboundAvatarShowDelegate( onInboundAvatarShow );
         m_host.Net.InboundAvatarShow2Event += new Furnarchy.InboundAvatarShow2Delegate( onInboundAvatarShow2 );

         // The Client subsystem's PlayerMenuShowingEvent event will tell us when the player 
         // context menu is being shown and the player's name. This allows us to customize the 
         // menu per-player.
         m_host.Client.PlayerMenuShowingEvent += new Furnarchy.PlayerMenuShowingDelegate( onPlayerMenuShowing );

         // Some other common events we'll handle.
         m_host.EnabledEvent += new Furnarchy.EnabledDelegate( onEnabled );
         m_host.DisabledEvent += new Furnarchy.DisabledDelegate( onDisabled );
         m_host.TouchedEvent += new Furnarchy.TouchedDelegate( onTouched );
         m_host.CommandEvent += new Furnarchy.CommandDelegate( onCommand );

         // Load up the toasters file.
         loadToasters( );

         // Enable ourselves right away.
         m_host.Enabled = true;
      }

      void onEnabled( object sender, EventArgs e )
      {
         m_host.speak( String.Format( "\"{0}\" enabled! {1} people are toasters!", m_host.FancyName, m_toasters.Count ) );

         // Update all toasters' avatars.
         for (LinkedListNode<Toaster> i = m_toasters.First; i != null; i = i.Next)
         {
            Furnarchy.MonsterInfo info = m_host.World.getMonsterInfo( i.Value.shortname );
            if (info.guid != 0)
            {
               // Remember the avatar's real frame number.
               i.Value = new Toaster( i.Value.shortname, info.frame );
               // Change the avatar's frame number to a toaster's.
               if (info.visible)
                  updateAvatar( info.guid, makeToasterFrame( info.frame ), info.colors );
            }
         }
      }

      void onDisabled( object sender, EventArgs e )
      {
         m_host.speak( String.Format( "\"{0}\" disabled!", m_host.FancyName ) );

         // Revert all toasters' avatars.
         for (LinkedListNode<Toaster> i = m_toasters.First; i != null; i = i.Next)
         {
            Furnarchy.MonsterInfo info = m_host.World.getMonsterInfo( i.Value.shortname );
            if (info.guid != 0)
            {
               // Change the avatar's frame number to a toaster's.
               if (info.visible)
                  updateAvatar( info.guid, i.Value.frame, info.colors );
            }
         }
      }

      // You know, when someone right-clicks on the module icon.
      void onTouched( object sender, EventArgs e )
      {
         // Tell the player how to use this module.
         m_host.speak( String.Format( "<b>{0} version 1.0-r0<b>", m_host.FancyName ) );
         m_host.speak( 
            String.Format( "Right-click on people and navigate to the {0} submenu to" +
                           " toaster-fy or untoaster-fy them!", m_host.FancyName ) 
                     );
         m_host.speak( "Say <i>@toaster clear</i> to clear all toasters." );
         m_host.speak( "Say <i>@toaster list</i> to list all toasters." );
         m_host.speak( "This module must be enabled to see the toasters." );
      }

      void onCommand( object sender, Furnarchy.CommandEventArgs e )
      {
         if (e.args[ 0 ].ToLower( ) == "toaster")
         {
            if (e.args.Length > 1)
            {
               if (e.args[ 1 ].ToLower( ) == "clear")
               {
                  // User said "@toaster clear", so flush the toaster list!
                  clearToasters( false );
                  saveToasters( );
                  // Set 'handled' to true to indicate we handled the command.
                  e.handled = true;
                  return;
               }
               else if (e.args[ 1 ].ToLower( ) == "list")
               {
                  // User said "@toaster list", so list the toasters!
                  StringBuilder sb = new StringBuilder( String.Format( "{0} toasters: ", m_toasters.Count ) );
                  for (LinkedListNode<Toaster> i = m_toasters.First; i != null; i = i.Next)
                  {
                     sb.Append( "<i>" );
                     sb.Append( i.Value.shortname );
                     sb.Append( "</i>" );
                     if (i.Next != null)
                        sb.Append( ", " );
                     else
                        sb.Append( "." );
                  }

                  m_host.speak( sb.ToString( ) );
                  // Set 'handled' to true to indicate we handled the command.
                  e.handled = true;
                  return;
               }
            }

            // Touch this module so we raise an onTouched event and spit out the instructions.
            m_host.touch( );
            // Set 'handled' to true to indicate we handled the command.
            e.handled = true;
            return;
         }
      }

      // An avatar is being created.
      void onInboundAvatarCreate( object sender, Furnarchy.InboundAvatarCreateEventArgs e )
      {
         if (m_host.Enabled)
         {
            LinkedListNode<Toaster> t = findToaster( e.name );
            if (t != null)
            {
               // This avatar is in our toaster list!
               // Rebuild the command so that the avatar frame is a toaster.
               // The basic format of an avatar creation command is:
               // <{guid}{x}{y}{frame}{name}{colors}{flags}
               // format() can easily build this line for us.
               e.line = m_host.Net.format( "<%4N%2N%2N%2N%s%c%1N",
                                           e.guid, e.x, e.y,
                                           makeToasterFrame( e.frame ),
                                           e.name, e.colors, e.flags );

               // Remember the avatar's real frame number in case we have to un-toaster him later.
               t.Value = new Toaster( t.Value.shortname, e.frame );
            }
         }
      }

      // An avatar is moving.
      void onInboundAvatarMove( object sender, Furnarchy.InboundAvatarMoveEventArgs e )
      {
         if (m_host.Enabled)
         {
            // Use the World subsystem to find the avatar's name.
            Furnarchy.MonsterInfo info = m_host.World.getMonsterInfo( e.guid );
            if (info.guid != 0)
            {
               LinkedListNode<Toaster> t = findToaster( info.name );
               if (t != null)
               {
                  // This avatar is in our toaster list!
                  // Rebuild the command so that the avatar frame is a toaster.
                  // The basic format of an avatar move command is:
                  // /{guid}{x}{y}{end frame}
                  // format() can easily build this line for us.
                  e.line = m_host.Net.format( "/%4N%2N%2N%2N",
                                              e.guid, e.x, e.y,
                                              makeToasterFrame( e.frame ) );

                  // Remember the avatar's real frame number in case we have to un-toaster him later.
                  t.Value = new Toaster( t.Value.shortname, e.frame );
               }
            }
         }
      }

      // An avatar's appearance is changing.
      void onInboundAvatarUpdate( object sender, Furnarchy.InboundAvatarUpdateEventArgs e )
      {
         if (m_host.Enabled)
         {
            // Use the World subsystem to find the avatar's name.
            Furnarchy.MonsterInfo info = m_host.World.getMonsterInfo( e.guid );
            if (info.guid != 0)
            {
               LinkedListNode<Toaster> t = findToaster( info.name );
               if (t != null)
               {
                  // This avatar is in our toaster list!
                  // Rebuild the command so that the avatar frame is a toaster.
                  // The basic format of an avatar update command is:
                  // B{guid}{frame}{colors}
                  // format() can easily build this line for us.
                  e.line = m_host.Net.format( "B%4N%2N%c",
                                              e.guid, makeToasterFrame( e.frame ), e.colors );

                  // Remember the avatar's real frame number in case we have to un-toaster him later.
                  t.Value = new Toaster( t.Value.shortname, e.frame );
               }
            }
         }
      }

      // An avatar should be displayed with the specified frame and position.
      void onInboundAvatarShow( object sender, Furnarchy.InboundAvatarShowEventArgs e )
      {
         if (m_host.Enabled)
         {
            // Use the World subsystem to find the avatar's name.
            Furnarchy.MonsterInfo info = m_host.World.getMonsterInfo( e.guid );
            if (info.guid != 0)
            {
               LinkedListNode<Toaster> t = findToaster( info.name );
               if (t != null)
               {
                  // This avatar is in our toaster list!
                  // Rebuild the command so that the avatar frame is a toaster.
                  // The basic format of an avatar show command is:
                  // A{guid}{x}{y}{frame}
                  // format() can easily build this line for us.
                  e.line = m_host.Net.format( "A%4N%2N%2N%2N",
                                              e.guid, e.x, e.y, makeToasterFrame( e.frame ) );

                  // Remember the avatar's real frame number in case we have to un-toaster him later.
                  t.Value = new Toaster( t.Value.shortname, e.frame );
               }
            }
         }
      }

      // An avatar should be displayed with the specified frame and position.
      // This variant does not move your camera if the avatar happens to be you.
      void onInboundAvatarShow2( object sender, Furnarchy.InboundAvatarShow2EventArgs e )
      {
         if (m_host.Enabled)
         {
            // Use the World subsystem to find the avatar's name.
            Furnarchy.MonsterInfo info = m_host.World.getMonsterInfo( e.guid );
            if (info.guid != 0)
            {
               LinkedListNode<Toaster> t = findToaster( info.name );
               if (t != null)
               {
                  // This avatar is in our toaster list!
                  // Rebuild the command so that the avatar frame is a toaster.
                  // The basic format of an avatar show (2) command is:
                  // D{guid}{x}{y}{frame}########
                  // format() can easily build this line for us.
                  e.line = m_host.Net.format( "D%4N%2N%2N%2N########",
                                              e.guid, e.x, e.y, makeToasterFrame( e.frame ) );

                  // Remember the avatar's real frame number in case we have to un-toaster him later.
                  t.Value = new Toaster( t.Value.shortname, e.frame );
               }
            }
         }
      }

      // Player context menu is about to be shown.
      void onPlayerMenuShowing( object sender, Furnarchy.PlayerMenuShowingEventArgs e )
      {
         // Check the "Toaster" menu item if the player is already a toaster.
         if (findToaster( e.player_name ) != null)
            m_player_menu.MenuItems[ 0 ].Checked = true;
         else
            // Not a toaster, so uncheck it.
            m_player_menu.MenuItems[ 0 ].Checked = false;
      }

      // The "Toaster!" menu item on our submenu has been clicked.
      void onMenuToasterClick( object sender, EventArgs e )
      {
         // Toggle the toaster status of the player.
         // World.PlayerMenuName tells us the player name for the last displayed player context menu.
         toggleToaster( m_host.Client.PlayermenuName );
         saveToasters( );
      }

      void toggleToaster( string player )
      {
         // If the the player is a toaster, un-toaster him, and vice-versa.
         if (findToaster( player ) != null)
            removeToaster( player, false );
         else
            addToaster( player, false );
      }

      void addToaster( string player, bool quiet )
      {
         // Avoid duplicates.
         if (findToaster( player ) == null)
         {
            // Create a new Toaster.
            Toaster t;
            t.shortname = m_host.makeShortname( player );
            t.frame = 0;

            // Get information on the player.
            Furnarchy.MonsterInfo info = m_host.World.getMonsterInfo( player );
            if (info.guid != 0)
            {
               t.frame = info.frame;
               // Change his frame number to a toaster's.
               if (m_host.Enabled && info.visible)
                  updateAvatar( info.guid, makeToasterFrame( info.frame ), info.colors );
            }

            // Add him to the list of toasters.
            m_toasters.AddFirst( t );

            if (!quiet)
            {
               if (m_host.Enabled)
                  m_host.speak( String.Format( "{0} is now a toaster!", player ) );
               else
                  m_host.speak( String.Format( "{0} will be a toaster once {1} is enabled!", player, m_host.FancyName ) );
            }
         }
      }

      void removeToaster( string player, bool quiet )
      {
         LinkedListNode<Toaster> t = findToaster( player );
         if (t != null)
         {
            // Remove the player from the toasters list.
            m_toasters.Remove( t );

            // Get information on the player.
            Furnarchy.MonsterInfo info = m_host.World.getMonsterInfo( player );
            if (info.guid != 0)
            {
               // Revert his frame number to its real value.
               if (m_host.Enabled && info.visible)
                  updateAvatar( info.guid, t.Value.frame, info.colors );
            }

            if (!quiet)
            {
               if (m_host.Enabled)
                  m_host.speak( String.Format( "{0} is no longer a toaster!", player ) );
               else
                  m_host.speak( String.Format( "{0} will no longer be a toaster!", player ) );
            }
         }
      }

      void clearToasters( bool quiet )
      {
         int num_toasters = m_toasters.Count;
         
         while (m_toasters.Count > 0)
            removeToaster( m_toasters.First.Value.shortname, true );
         
         if (!quiet)
            m_host.speak( String.Format( "{0} toasters cleared!", num_toasters ) );
      }

      uint makeToasterFrame( uint frame )
      {
         // Decode the current frame.
         Furnarchy.Avatar_Class av_class;
         Furnarchy.Avatar_Pose av_pose;
         Furnarchy.Move_Direction av_dir;
         m_host.Net.decodeAvatarFrameNumber( frame, out av_class, out av_pose, out av_dir );
         // Encode a toaster frame using the same pose and direction.
         return m_host.Net.encodeAvatarFrameNumber( Furnarchy.Avatar_Class.Toaster, av_pose, av_dir );
      }

      LinkedListNode<Toaster> findToaster( string player )
      {
         // Compare with shortnames because people can 
         // (technically) alter their names' capitalization and spacing.
         string player_short = m_host.makeShortname( player );
         
         for (LinkedListNode<Toaster> toaster = m_toasters.First; toaster != null; toaster = toaster.Next)
         {
            if (toaster.Value.shortname == player_short)
            {
               return toaster;
            }
         }
         return null;
      }

      void updateAvatar( uint guid, uint frame, byte[] colors )
      {
         // Inject an avatar update command to change its frame number.
         // The basic format of a server avatar update command is:
         // B{guid}{frame}{colors}
         // Net subsystem's format() method can easily build this line for us.
         byte[] cmd = m_host.Net.format( "B%4N%2N%c", guid, frame, colors );

         m_host.Net.inject( cmd );
         // Remember, because we injected the line, we won't receive the consequent
         // InboundAvatarUpdateEvent, but everyone else will.
      }

      void saveToasters( )
      {
         FileStream fs = null;
         try
         {
            // The DocumentFolder is where all per-user configuration files should be stored.
            fs = new FileStream( m_host.DocumentFolder + "\\toasters.txt",
                                 FileMode.Create, FileAccess.Write, FileShare.None );
         }
         catch (Exception e)
         {
            m_host.speakError( String.Format( "{0} (saveToaters) {1} ", m_host.FancyName, e.Message ) );
            return;
         }

         StreamWriter sw = new StreamWriter( fs );

         for (LinkedListNode<Toaster> i = m_toasters.First; i != null; i = i.Next)
            sw.WriteLine( i.Value.shortname );

         sw.Close( );
      }

      void loadToasters()
      {
         FileStream fs = null;

         try
         {
            // The DocumentFolder is where all per-user configuration files should be stored.
            fs = new FileStream( m_host.DocumentFolder + "\\toasters.txt",
                                 FileMode.Open, FileAccess.Read, FileShare.Read );
         }
         catch (Exception e)
         {
            m_host.speakError( String.Format( "{0} (loadToaters) {1} ", m_host.FancyName, e.Message ) );
            return;
         }

         StreamReader sr = new StreamReader( fs );

         m_toasters.Clear( );
         while (!sr.EndOfStream)
         {
            Toaster t = new Toaster( m_host.makeShortname( sr.ReadLine( ) ), 0 );
            if (t.shortname.Length > 0)
               m_toasters.AddLast( t );
         }

         sr.Close( );
      }

      //////////////////////////////////////////////////////////////////////////

      // Instance of the ModuleHost class, which exposes the full Furnarchy API to us.
      private readonly Furnarchy.ModuleHost m_host;
      // Our list of toaster'd player names.
      private LinkedList<Toaster> m_toasters;
      // Our player context (sub)menu.
      private MenuItem m_player_menu;
   }
   
   // Represents a toaster'd player.
   struct Toaster
   {
      public Toaster( string shortname, uint frame )
      {
         this.shortname = shortname;
         this.frame = frame;
      }
      // Player's shortname.
      public string shortname;
      // Player's real avatar frame.
      public uint frame;
   }
}
