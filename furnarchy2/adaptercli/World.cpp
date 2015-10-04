#include "Furnarchy.h"
#include "shortname.h"

namespace Furnarchy 
{

using namespace std;

WorldSubsystem::WorldSubsystem( ModuleHost^ host )
   : m_host( host ), m_monsters( gcnew LinkedList<MonsterInfo>( ) )
{
   m_player_info.name        = "";
   m_player_info.shortname   = "";
   m_player_info.password    = "";
   m_player_info.colors      = gcnew array<byte>( 0 );
   m_player_info.description = "";
   m_player_info.mac_id      = "";
   m_player_info.guid        = 0;
   m_player_info.x           = 0;
   m_player_info.y           = 0;

   m_host->Net->DisconnectedEvent += gcnew DisconnectedDelegate( this, &WorldSubsystem::onDisconnected );
}

WorldSubsystem::~WorldSubsystem( )
{

}

DescTag^ WorldSubsystem::createDescTag( )
{
   return gcnew DescTag( );   
}

String^ WorldSubsystem::createShortName( String^ name )
{
   string shortname = shortname::make_shortname( CliUtil::cliStrToAscii( name ) );
   return gcnew String( shortname.c_str( ) );
}

MonsterInfo WorldSubsystem::getMonsterInfo( String^ name )
{
   String^ shortname = createShortName( name );
   MonsterInfo mi;
   for each (MonsterInfo i in m_monsters)
   {
      if (i.shortname == shortname)
      {
         mi = i;
         break;
      }
   }
   return mi;
}

MonsterInfo WorldSubsystem::getMonsterInfo( UInt32 guid )
{
   MonsterInfo mi;
   for each (MonsterInfo i in m_monsters)
   {
      if (i.guid == guid)
      {
         mi = i;
         break;
      }
   }
   return mi;
}

array<MonsterInfo>^ WorldSubsystem::getMonsters( )
{
   array<MonsterInfo>^ monsters = gcnew array<MonsterInfo>( m_monsters->Count );
   m_monsters->CopyTo( monsters, 0 );
   return monsters;
}

void WorldSubsystem::onDisconnected( Object^ sender, EventArgs^ e )
{
   /* Clear the current player info. */
   m_player_info.name        = "";
   m_player_info.shortname   = "";
   m_player_info.password    = "";
   m_player_info.colors      = gcnew array<byte>( 0 );
   m_player_info.description = "";
   m_player_info.mac_id      = "";
   m_player_info.guid        = 0;
   m_player_info.x           = 0;
   m_player_info.y           = 0;

   {
      CurrentPlayerUpdatedEventArgs e;
      e.info = m_player_info;
      CurrentPlayerUpdatedEvent( this, %e );
   }

   /* Clear all monsters. */
   {
      MonsterDestroyedEventArgs e;
      while (m_monsters->Count)
      {
         e.monster = m_monsters->First->Value;
         m_monsters->RemoveFirst( );
         MonsterDestroyedEvent( this, %e );
      }
   }
}

void WorldSubsystem::onMonster( UInt32 guid )
{
   /* Try to figure out which monster was modified and how. */

   furn2::MONSTERINFO _mi;
   furn2::f2_monsterinfo( guid, &_mi );

   if (_mi.guid)
   {
      MonsterInfo mi;
      mi.name      = gcnew String( _mi.name );
      mi.colors    = CliUtil::multiByteStrToCliBytes( _mi.color );
      mi.shortname = createShortName( mi.name );
      mi.guid      = _mi.guid;
      mi.frame     = _mi.frame;
      mi.x         = _mi.pos.x;
      mi.y         = _mi.pos.y;
      mi.flags     = Monster_Flags::None;
      if (_mi.flags & furn2::MONSTER_HASPROFILE)
         mi.flags = mi.flags | Monster_Flags::HasProfile;
      if (_mi.flags & furn2::MONSTER_UNK1)
         mi.flags = mi.flags | Monster_Flags::Unk1;
      if (_mi.flags & furn2::MONSTER_UNK2)
         mi.flags = mi.flags | Monster_Flags::Unk2;
      mi.visible   = furn2::f2_monstervisible( _mi.guid );

      for (LinkedListNode<MonsterInfo>^ i = m_monsters->First; i != nullptr; i = i->Next)
      {
         if (i->Value.guid == guid)
         {
            i->Value = mi;
            MonsterUpdatedEventArgs e;
            e.monster = mi;
            MonsterUpdatedEvent( this, %e );
            return;
         }
      }

      // Monster wasn't found. Add it.
      m_monsters->AddLast( mi );
      MonsterCreatedEventArgs e;
      e.monster = mi;
      MonsterCreatedEvent( this, %e );
      return;
   }

   // Monster got destroyed.
   for (LinkedListNode<MonsterInfo>^ i = m_monsters->First; i != nullptr; i = i->Next)
   {
      if (i->Value.guid == guid)
      {
         MonsterDestroyedEventArgs e;
         e.monster = i->Value;
         m_monsters->Remove( i );
         MonsterDestroyedEvent( this, %e );
         return;
      }
   }
}

void WorldSubsystem::onPlayerInfo( )
{
   furn2::PLAYERINFO pi;
   furn2::f2_myplayerinfo( &pi );

   m_player_info.name = gcnew String( pi.name );
   m_player_info.shortname = createShortName( m_player_info.name );
   m_player_info.password = gcnew String( pi.pw );
   m_player_info.colors      = CliUtil::multiByteStrToCliBytes( pi.colors );
   m_player_info.description = gcnew String( pi.desc );
   m_player_info.mac_id      = gcnew String( pi.mac_id );
   m_player_info.guid        = pi.guid;
   m_player_info.x           = pi.cam.x;
   m_player_info.y           = pi.cam.y;


   CurrentPlayerUpdatedEventArgs e;
   e.info = m_player_info;
   CurrentPlayerUpdatedEvent( this, %e );
}

} // namespace Furnarchy