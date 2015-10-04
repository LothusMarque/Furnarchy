#ifndef _WORLD_H__
#define _WORLD_H__

namespace Furnarchy
{
   /// <summary> Information on the current player. </summary>
   public value class PlayerInfo
   {
   public:
      String^ name;
      String^ shortname;
      String^ password;
      String^ mac_id;
      String^ description;
      array<Byte>^ colors;
      UInt32  guid;
      UInt32  x, y;
   };
   /// <summary> Information on a monster (avatar). </summary>
   public value class MonsterInfo
   {
   public:
      UInt32        guid;
      String^       name;
      String^       shortname;
      array<Byte>^  colors;
      UInt32        x, y;
      UInt32        frame;
      Monster_Flags flags;
      Boolean       visible;
   };

   /// <summary> Arguments for <see cref='MonsterCreatedDelegate' />. </summary>
   public ref class MonsterCreatedEventArgs : EventArgs
   {
   public:
      MonsterInfo monster;
   };
   /// <summary> Arguments for <see cref='MonsterDestroyedDelegate' />. </summary>
   public ref class MonsterDestroyedEventArgs : EventArgs
   {
   public:
      MonsterInfo monster;
   };
   /// <summary> Arguments for <see cref='MonsterUpdatedDelegate' />. </summary>
   public ref class MonsterUpdatedEventArgs : EventArgs
   {
   public:
      MonsterInfo monster;
   };
   /// <summary> Arguments for <see cref='CurrentPlayerUpdatedDelegate' />. </summary>
   public ref class CurrentPlayerUpdatedEventArgs : EventArgs
   {
   public:
      PlayerInfo info;
   };

   //////////////////////////////////////////////////////////////////////////
   
   /// <summary> A monster event delegate. </summary>
   /// <seealso cref='WorldSubsystem::MonsterCreatedEvent' />
   public delegate void MonsterCreatedDelegate( Object^ sender, MonsterCreatedEventArgs^ e );
   /// <summary> A monster event delegate. </summary>
   /// <seealso cref='WorldSubsystem::MonsterDestroyedEvent' />
   public delegate void MonsterDestroyedDelegate( Object^ sender, MonsterDestroyedEventArgs^ e );
   /// <summary> A monster event delegate. </summary>
   /// <seealso cref='WorldSubsystem::MonsterUpdatedEvent' />
   public delegate void MonsterUpdatedDelegate( Object^ sender, MonsterUpdatedEventArgs^ e );
   /// <summary> A world event delegate. </summary>
   /// <seealso cref='WorldSubsystem::CurrentPlayerUpdatedEvent' />
   public delegate void CurrentPlayerUpdatedDelegate( Object^ sender, CurrentPlayerUpdatedEventArgs^ e );

   //////////////////////////////////////////////////////////////////////////
   
   /// <summary> An instance of a desctag. </summary>
   /// <seealso cref='WorldSubsystem::createDescTag' />
   public ref class DescTag sealed
   {
   internal:
      DescTag( ) : m_id( furn2::f2_desctag_reg( ) ), m_body( gcnew String( "" ) ) { }
   public:
      ~DescTag( ) { furn2::f2_desctag_unreg( m_id ); }
      /// <summary> The body of the desctag. </summary>
      /// <remarks>
      ///   Writing to this property will automatically
      ///   update to the player's description with the server.
      /// </remarks>
      property String^ body
      {
         String^ get( ) { return m_body; }
         void set( String^ body ) 
         { 
            if (furn2::f2_desctag_set( m_id, CliUtil::cliStrToAscii( body ).c_str( ) ))
            {
               m_body = body; 
            }
         }
      }
   protected:
      initonly Int32 m_id;
      String^ m_body;
   };

   //////////////////////////////////////////////////////////////////////////

   /// <summary> Encapsulates knowledge of and interactions with the game world. </summary>
   public ref class WorldSubsystem sealed
   {
   private:
      WorldSubsystem( ) { }
   internal:
      WorldSubsystem( ModuleHost^ host );
      ~WorldSubsystem( );

      void onMonster( UInt32 guid );
      void onPlayerInfo( );
      void onDisconnected( Object^ sender, EventArgs^ e );

   public:

      /// <summary> Create a shortname. </summary>
      /// <param name='name'> A longname </param>
      /// <remarks>
      ///   To create a shortname, all spaces and non-ascii characters are stripped.
      ///   Some latin-1 characters that resemble regular ASCII characters are converted to their
      ///   ASCII counterparts (e.g., <c>ë</c> to <c>e</c>).
      /// </remarks>
      /// <returns> The shortname equivalent of <c>name</c>. </returns>
      String^ createShortName( String^ name );
      /// <summary> Create a desctag. </summary>
      /// <remarks>
      ///   Desctags are logical suffixes to the player's description.
      ///   They help keep modules and scripts that wish to append text to
      ///   the player's description from stepping on each other's toes.
      /// </remarks>
      /// <returns> A <see cref='DescTag' /> instance. </returns>
      /// <seealso cref='DescTag' />
      DescTag^ createDescTag( );
      /// <summary> Retrieve information on a monster by its GUID. </summary>
      /// <param name='guid'> 32-bit GUID of the monster. </param>
      /// <remarks> 
      ///  The <c>guid</c> member of the returned <see cref='MonsterInfo' />
      ///  will be zero if the monster could not be found.
      /// </remarks>
      /// <returns>
      /// A <see cref='MonsterInfo' /> structure with information on the monster.
      /// </returns>
      MonsterInfo getMonsterInfo( UInt32 guid );
      /// <summary> Retrieve information on a monster by its name. </summary>
      /// <param name='name'> Name of the monster (this will be converted to a shortname automatically). </param>
      /// <remarks> 
      ///  The <c>guid</c> member of the returned <see cref='MonsterInfo' />
      ///  will be zero if the monster could not be found.
      /// </remarks>
      /// <returns>
      ///  A <see cref='MonsterInfo' /> structure with information on the monster.
      /// </returns>
      MonsterInfo getMonsterInfo( String^ name );
      /// <summary> Retrieve an array of all existing monsters. </summary>
      /// <remarks> 
      ///  To maintain an up-to-date list of existing monsters, one should
      ///  make a call to <c>getMonsters</c> whenever monsters have been updated.
      /// </remarks>
      /// <returns>
      ///  An array of <see cref='MonsterInfo' /> structures with information on each existing monster.
      /// </returns>
      /// <seealso cref='MonsterCreatedEvent' />
      /// <seealso cref='MonsterDestroyedEvent' />
      /// <seealso cref='MonsterUpdatedEvent' />
      array<MonsterInfo>^ getMonsters( );

      /// <summary> Information on the current player. </summary>
      /// <remarks>
      ///  <para>
      ///     You may assign a new <see cref='PlayerInfo' /> value to
      ///     update Furnarchy and the client if you've made logical
      ///     changes to their values (by, say for example, swapping the connection).
      ///  </para>
      ///  <para>
      ///     The client update relies entirely on memlocs.
      ///  </para>
      /// </remarks>
      property PlayerInfo CurrentPlayer
      {
         PlayerInfo get( )
         {
            return m_player_info;
         }
         void set( PlayerInfo info )
         {
            {
               std::string name  = CliUtil::cliStrToAscii( info.name );
               std::string pw    = CliUtil::cliStrToAscii( info.password );
               std::string color = CliUtil::CliBytesToMultiByteStr( info.colors );
               std::string desc  = CliUtil::cliStrToAscii( info.description );
               unsigned int guid = info.guid;
               furn2::f2__setplayerinfo( name.c_str( ), pw.c_str( ), color.c_str( ), desc.c_str( ), &guid );
            }
            furn2::PLAYERINFO pi;
            furn2::f2_myplayerinfo( &pi );
            m_player_info.name        = gcnew String( pi.name );
            m_player_info.shortname   = createShortName( m_player_info.name );
            m_player_info.password    = gcnew String( pi.pw );
            m_player_info.colors      = CliUtil::multiByteStrToCliBytes( pi.colors );
            m_player_info.description = gcnew String( pi.desc );
            m_player_info.mac_id      = gcnew String( pi.mac_id );
            m_player_info.guid        = pi.guid;
            m_player_info.x           = pi.cam.x;
            m_player_info.y           = pi.cam.y;
         }
      }

      /// <summary> A monster was created. </summary>
      event MonsterCreatedDelegate^ MonsterCreatedEvent;
      /// <summary> A monster was destroyed. </summary>
      event MonsterDestroyedDelegate^ MonsterDestroyedEvent;
      /// <summary> A monster was updated. </summary>
      event MonsterUpdatedDelegate^ MonsterUpdatedEvent;
      /// <summary> The player's <see cref='PlayerInfo' /> has changed. </summary>
      event CurrentPlayerUpdatedDelegate^ CurrentPlayerUpdatedEvent;

   private:
      ModuleHost^              m_host;
      PlayerInfo               m_player_info;
      LinkedList<MonsterInfo>^ m_monsters;
   };

}

#endif