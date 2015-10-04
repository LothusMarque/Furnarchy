using System;
using System.Collections.Generic;
using System.Text;

/************************************************************************/
/* EmptyModule.cs                                                       */
/* Just an (almost) empty C# module.                                    */
/* It subscribes to a couple commonly used events and giggles when you  */
/* touch it.                                                            */
/*                                                                      */
/* Compile as a .NET 2.0 class library with                             */
/* C:\Program Files\Furnarchy2\adapters\cli.dll as a reference.         */
/* But make sure the cli.dll reference is not set to "Copy Local".      */
/* Your module class will fail to be constructed if you ship with       */
/* a copy of cli.dll in your module's folder!                           */
/* Finally, ensure you have a properly formed module.ini file           */
/* with 'adapter=cli' in your module's folder to get furnarchy to load  */
/* it as a CLI module.                                                  */
/************************************************************************/

namespace EmptyModule
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

         // Subscribe to some commonly used events. (optional)
         m_host.EnabledEvent += new Furnarchy.EnabledDelegate( onEnabled );
         m_host.DisabledEvent += new Furnarchy.DisabledDelegate( onDisabled );
         m_host.TouchedEvent += new Furnarchy.TouchedDelegate( onTouched );
         m_host.TickEvent += new Furnarchy.TickDelegate( onTick );
      }

      void onEnabled( object sender, EventArgs e )
      {
         m_host.speak( String.Format( "\"{0}\" enabled!", m_host.FancyName ) );
      }

      void onDisabled( object sender, EventArgs e )
      {
         m_host.speak( String.Format( "\"{0}\" disabled!", m_host.FancyName ) );
      }

      // You know, when someone right-clicks on the module icon.
      void onTouched( object sender, EventArgs e )
      {
         // Look, the World subsystem allows us to call the current player by name!
         m_host.speak( String.Format( "That tickles, {0}!", m_host.World.CurrentPlayer.name ) );
      }

      // This event is raised roughly every 75ms.
      void onTick( object sender, Furnarchy.TickEventArgs e )
      {
         // e.dt is the number of milliseconds that have passed since the last tick event.
      }

      //////////////////////////////////////////////////////////////////////////

      // Instance of the ModuleHost class, which exposes the full Furnarchy API to us.
      private readonly Furnarchy.ModuleHost m_host;
   }
}
