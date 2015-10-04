#pragma once

namespace Furnarchy
{
   /// <summary> Module state flags. </summary>
   [Flags]
   public enum class Module_State
   {
      Disabled = 0x0,
      Enabled  = 0x1
   };

   //////////////////////////////////////////////////////////////////////////


   /// <summary> Arguments for <see cref='TickDelegate' />. </summary>
   public ref class TickEventArgs : EventArgs
   {
   public:
      UInt32 dt;
   };
   /// <summary> Arguments for <see cref='CommandDelegate' />. </summary>
   public ref class CommandEventArgs : EventArgs
   {
   public:
      array<String^>^ args;
      Boolean         handled;
   };
   /// <summary> Arguments for <see cref='BroadcastValueDelegate' />. </summary>
   public ref class BroadcastValueEventArgs : EventArgs
   {
   public:
      IntPtr value;
   };

/*
      /// <summary> A module event delegate. </summary>
      /// <seealso cref='ModuleHost::StartEvent' />
      public delegate void StartDelegate( Object^ sender, EventArgs^ e );
      /// <summary> A module event delegate. </summary>
      /// <seealso cref='ModuleHost::StopEvent' />
      public delegate void StopDelegate( Object^ sender, EventArgs^ e );*/
   
   /// <summary> A module event delegate. </summary>
   /// <seealso cref='ModuleHost::EnabledEvent' />
   public delegate void EnabledDelegate( Object^ sender, EventArgs^ e );
   /// <summary> A module event delegate. </summary>
   /// <seealso cref='ModuleHost::DisabledEvent' />
   public delegate void DisabledDelegate( Object^ sender, EventArgs^ e );
   /// <summary> A module event delegate. </summary>
   /// <seealso cref='ModuleHost::TouchedEvent' />
   public delegate void TouchedDelegate( Object^ sender, EventArgs^ e );
   /// <summary> A module event delegate. </summary>
   /// <seealso cref='ModuleHost::TickEvent' />
   public delegate void TickDelegate( Object^ sender, TickEventArgs^ e );
   /// <summary> A module event delegate. </summary>
   /// <seealso cref='ModuleHost::CommandEvent' />
   public delegate void CommandDelegate( Object^ sender, CommandEventArgs^ e );
   /// <summary> A module event delegate. </summary>
   /// <seealso cref='ModuleHost::BroadcastValueEvent' />
   public delegate void BroadcastValueDelegate( Object^ sender, BroadcastValueEventArgs^ e );

   //////////////////////////////////////////////////////////////////////////

   /// <summary>Class providing common services for CLI modules.</summary>
   /// <remarks>
   ///   <para>
   ///      An instance of this class will be created for each module that
   ///      defines a public class that implements the IFurnarchyModule interface.
   ///   </para>
   ///   <para>
   ///      Please be wary of some features that depend on memlocs that will
   ///      operate with limited (if any) functionality if Furnarchy does not
   ///      support the running client build.
   ///   </para>
   /// </remarks>
   public ref class ModuleHost sealed
   {
   private:
      ModuleHost( ) { }
   internal:
      ModuleHost( UInt32 id );

   internal:
      bool     onCommand( int argc, const char* argv[] );
      intptr_t onModule( furn2::Module_Event e, intptr_t param );
      intptr_t onClient( furn2::Client_Event e, intptr_t param );
      intptr_t onWorld( furn2::World_Event e, intptr_t param );
      void     onInbound( furn2::Revision rev, const char* line, int from, size_t num );
      void     onOutbound( furn2::Revision rev, const char* line, int from, size_t num );
      bool     onWndProc( furn2::WNDPROCPARAMS* params );

   public:

      /// <summary> Posts a @-style command to Furnarchy. </summary>
      /// <param name="command">
      ///   Command line. Each argument is space-delimited. To preserve whitespace, you
      ///   must escape it or surround each argument with quotes, apostrophes, or backticks.
      ///   Non-ascii characters will be XML-escaped.
      /// </param>
      /// <param name="no_echo">
      ///   Whether or not to echo the command in the chat buffer.
      /// </param>
      /// <remarks> 
      ///   After the command is processed, a <see cref="CommandEvent" /> will
      ///   be raised.
      /// </remarks>
      void postCommand( String^ command, Boolean no_echo )
      {
         if (command == nullptr)
            throw gcnew ArgumentNullException;
         furn2::f2_command( CliUtil::cliStrToAscii( command ).c_str( ), no_echo );
      }
      /// <summary> Convert a user name into its shortname equivalent. </summary>
      /// <param name="name"> The name to transform. Non-ascii characters will be XML-escaped. </param>
      /// <returns> The shortname version of <c>name</c>. </returns>
      /// <remarks> Shortnames are stripped down versions of names and how the server sees names. </remarks>
      String^ makeShortname( String^ name )
      {
         if (name == nullptr)
            throw gcnew ArgumentNullException;
         char* buf = new char[ name->Length + 1 ];
         furn2::f2_makeshortname( CliUtil::cliStrToAscii( name ).c_str( ), buf );
         String^ r = gcnew String( buf );
         delete [] buf;
         return r;
      }
      /// <summary> Broadcast a value to all (other) running instances of this module. </summary>
      /// <param name='value'> Value to broadcast. </param>
      /// <remarks>
      ///   This will asynchronously raise a <see cref='BroadcastValueEvent' /> in all running
      ///   instances of this module.
      /// </remarks>
      /// <seealso cref='BroadcastValueEvent' />
      void    broadcastValue( IntPtr value ) 
      {
         DWORD recips = BSM_APPLICATIONS;
         BroadcastSystemMessageW( BSF_POSTMESSAGE | BSF_IGNORECURRENTTASK,
                                  &recips, m_broadcast_msg, (WPARAM) m_my_id, (LPARAM) value.ToPointer( ) );
      }
      /// <summary> Echoes a string in the chat buffer. </summary>
      /// <param name="line"> String to echo. Non-ascii characters will be XML-escaped. </param>
      /// <seealso cref="speakError" />
      void speak( String^ line )
      {
         if (line == nullptr)
            throw gcnew ArgumentNullException;
         furn2::f2_speak( CliUtil::cliStrToAscii( line ).c_str( ) );
      }
      /// <summary> Echoes a string in the chat buffer and adds it to the error log. </summary>
      /// <param name="line"> String to echo. Non-ascii characters will be XML-escaped. </param>
      /// <seealso cref="speak" />
      /// <seealso cref="logError" />
      void speakError( String^ line )
      {
         if (line == nullptr)
            throw gcnew ArgumentNullException;
         furn2::f2_errspeak( CliUtil::cliStrToAscii( line ).c_str( ) );
      }
      /// <summary> Logs an error to the error log. </summary>
      /// <param name="line"> String to append to the error log. Non-ascii characters will be XML-escaped. </param>
      /// <seealso cref="speakError" />
      void logError( String^ line )
      {
         if (line == nullptr)
            throw gcnew ArgumentNullException;
         furn2::f2_errlog( CliUtil::cliStrToAscii( line ).c_str( ) );
      }
      /// <summary> "Touches" this module. </summary>
      /// <remarks>
      ///   This is equivalent to when the user right-clicks on your module's icon.
      ///   A <see cref='TouchedEvent' /> will be immediately raised.
      /// </remarks>
      void touch( )
      {
         furn2::f2_modtouch( m_my_id );
      }

      /// <summary> Number of modules loaded. </summary>
      /// <remarks> This is a read-only attribute. </remarks>
      /// <returns> The number of modules loaded/running. </returns>
      property UInt32 ModulesCount
      {
         UInt32 get( ) { return furn2::f2_modcount( ); }
      }
      /// <summary> This module's module ID. </summary>
      /// <remarks> This is a read-only property. </remarks>
      /// <returns> Your module's ID. </returns>
      property UInt32 Id 
      { 
         UInt32 get( ) { return m_my_id; } 
      }
      /// <summary> This module's state. </summary>
      /// <remarks>
      ///   You can assign a new state to this property to update your module's state to Furnarchy.
      ///   Enabling or disabling a module will immediately raise an <see cref="EnabledEvent" /> or <see cref="DisabledEvent" />.
      /// </remarks>
      /// <seealso cref='Module_State' />
      /// <returns> The state of this module (see <see cref='Module_State' />). </returns>
      property Module_State State
      {
         Module_State get( ) 
         { 
            return m_my_state;
         }
         void set( Module_State new_state )
         {
            int _state = 0;
            if ((new_state & Module_State::Enabled) == Module_State::Enabled)
               _state |= furn2::MOD_ENABLED;
            if (_state & furn2::MOD_ENABLED)
               furn2::f2_modenable( m_my_id, true );
            else
               furn2::f2_modenable( m_my_id, false );
         }
      }
      /// <summary> This module's name. </summary>
      /// <remarks> The module name is simply the name of its program folder. </remarks>
      /// <returns> This module's name. </returns>
      property String^ Name
      {
         String^ get( ) { return m_my_name; }
      }
      /// <summary> This module's fancy name. </summary>
      /// <remarks> This is the name specified in the module's module.ini file. </remarks>
      /// <returns> This module's fancy name. </returns>
      property String^ FancyName
      {
         String^ get( ) { return m_my_fancy_name; }
      }
      /// <summary> The running version of Furnarchy. </summary>
      /// <remarks> This is a read-only property. </remarks>
      /// <seealso cref="F2Version" />
      property F2Version ProxyVersion
      {
         F2Version get( )
         {
            return m_version;
         }
      }
      /// <summary> Whether Furnarchy is running in single-user-mode. </summary>
      /// <remarks> 
      ///   <para>
      ///      Single-user-mode means Furnarchy has reverted to the old, pre-Vista folder
      ///      layout, where all files are contained within the Furnarchy Program Files folder.
      ///   </para>
      ///   <para>
      ///      This is a read-only attribute.
      ///   </para>
      /// </remarks>
      property Boolean SingleUserMode
      {
         Boolean get( ) { return m_single_user_mode; }
      }
      /// <summary> Furnarchy's program root folder. </summary>
      property String^ ProxyProgramFolder
      {
         String^ get( ) { return m_f2_program_folder; }
      }
      /// <summary> Furnarchy's application data folder. </summary>
      /// <remarks> In single-user mode, this is equivalent to <see cref='ProxyProgramFolder' />. </remarks>
      property String^ ProxyDataFolder
      {
         String^ get( ) { return m_f2_data_folder; }
      }
      /// <summary> Furnarchy's document folder. </summary>
      /// <remarks> In single-user mode, this is equivalent to <see cref='ProxyProgramFolder' />. </remarks>
      property String^ ProxyDocumentFolder
      {
         String^ get( ) { return m_f2_document_folder; }
      }
      /// <summary> Furcadia's program root folder. </summary>
      property String^ ClientFolder
      {
         String^ get( ) { return m_client_folder; }
      }
      /// <summary> Furcadia's application data folder. </summary>
      property String^ ClientDataFolder
      {
         String^ get( ) { return m_client_data_folder; }
      }
      /// <summary> Furcadia's document folder. </summary>
      property String^ ClientDocumentFolder
      {
         String^ get( ) { return m_client_document_folder; }
      }
      /// <summary> The program folder of this module. </summary>
      /// <remarks> The program folder is the module where the module binary resides. </remarks>
      /// <returns> A folder path. </returns>
      property String^ ProgramFolder
      {
         String^ get( ) { return m_my_program_folder; }
      }
      /// <summary> The application data folder of this module. </summary>
      /// <remarks> 
      ///   <para> 
      ///      The application data folder of the module is where the module should store
      ///      files that the user should not easily access.
      ///   </para>
      ///   <para>
      ///      In single-user mode, this is equivalent to <see cref='ProgramFolder' />.
      ///   </para>
      /// </remarks>
      /// <returns> A folder path. </returns>
      property String^ DataFolder
      {
         String^ get( ) { return m_my_data_folder; }
      }
      /// <summary> The document folder of this module. </summary>
      /// <remarks> 
      ///   <para> 
      ///      The document folder of the module is where the module should store
      ///      files easily accessible to the user.
      ///   </para>
      ///   <para>
      ///      In single-user mode, this is equivalent to <see cref='ProgramFolder' />.
      ///   </para>
      /// </remarks>
      /// <returns> A folder path. </returns>
      property String^ DocumentFolder
      {
         String^ get( ) { return m_my_document_folder; }
      }
      /// <summary> This module's thumbnail file. </summary>
      /// <returns> The path to a module's thumbnail file. </returns>
      property String^ Thumbnail
      {
         String^ get( ) { return m_my_thumbnail; }
      }
      /// <summary> Whether this module is enabled. </summary>
      /// <seealso cref='State' />
      property Boolean Enabled
      {
         Boolean get( ) { return (m_my_state & Module_State::Enabled) == Module_State::Enabled; }
         void set( Boolean enabled )
         {
            if (enabled)
               furn2::f2_modenable( m_my_id, true );
            else
               furn2::f2_modenable( m_my_id, false );
         }
      }

      /// <summary> The module has been enabled. </summary>
      /// <remarks> 
      ///   This can occur if the user left-clicks on your module's icon or the 
      ///   <see cref='State' /> property has been assigned a new value.
      /// </remarks>
      /// <seealso cref='State' />
      event EnabledDelegate^  EnabledEvent;
      /// <summary> The module has been disabled. </summary>
      /// <remarks> 
      ///   This can occur if the user left-clicks on your module's icon or the 
      ///   <see cref='State' /> property has been assigned a new value.
      /// </remarks>
      /// <seealso cref='State' />
      event DisabledDelegate^ DisabledEvent;
      /// <summary> The module has been touched. </summary>
      /// <remarks> 
      ///   This can occur when the user right-clicks on your module's icon
      ///   or a call to <see cref='touch' /> is made. 
      /// </remarks>
      event TouchedDelegate^  TouchedEvent;
      /// <summary> A module heartbeat has occured. </summary>
      /// <param name='dt'> Time (in milliseconds) since last tick event. </param>
      /// <remarks>
      ///  This has a resolution of about 75ms, but will vary slightly
      ///  from machine to machine.
      /// </remarks>
      event TickDelegate^     TickEvent;
      /// <summary> A '@' command needs to be processed. </summary>
      /// <param name='args'> Structure containing the command arguments. </param>
      /// <remarks>
      ///  If you handle the command, you should set the <c>handled</c> field
      ///  in <c>args</c> to <c>true</c>, otherwise the user will get an error
      ///  about an unrecognized command.
      /// </remarks>
      event CommandDelegate^  CommandEvent;
      /// <summary> A value is being broadcasted to all instances of this module. </summary>
      /// <seealso cref='broadcastValue' />
      event BroadcastValueDelegate^    BroadcastValueEvent;

      //////////////////////////////////////////////////////////////////////////
   public:

      /// <summary> The client subsystem. See <see cref='ClientSubsystem' />. </summary>
      initonly ClientSubsystem^     Client;
      /// <summary> The network subsystem. See <see cref='NetSubsystem' />. </summary>
      initonly NetSubsystem^        Net;
      /// <summary> The scripts subsystem. See <see cref='ScriptsSubsystem' />. </summary>
      initonly ScriptsSubsystem^    Scripts;
      /// <summary> The world subsystem. See <see cref='WorldSubsystem' />. </summary>
      initonly WorldSubsystem^      World;

   private:

      initonly F2Version         m_version;
      initonly UInt32            m_my_id;
      Module_State               m_my_state;
      initonly String^           m_my_name;
      initonly String^           m_my_fancy_name;
      initonly Boolean           m_single_user_mode;
      initonly String^           m_client_data_folder;
      initonly String^           m_client_document_folder;
      initonly String^           m_client_folder;
      initonly String^           m_f2_data_folder;
      initonly String^           m_f2_document_folder;
      initonly String^           m_f2_program_folder;
      initonly String^           m_my_data_folder;
      initonly String^           m_my_document_folder;
      initonly String^           m_my_program_folder;
      initonly String^           m_my_thumbnail;
      initonly UInt32            m_broadcast_msg;

   }; // ref class ModuleHost
}