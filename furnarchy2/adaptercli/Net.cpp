#include "Furnarchy.h"
#include "net_scan.h"
#include "yiffurc/yiffurc.h"
#include <list>

namespace Furnarchy
{
   using namespace std;

NetSubsystem::NetSubsystem( ModuleHost^ host ) 
   : m_host( host ), 
     m_connection_level( Connection_Level::Disconnected )
{
   m_session = new yiffurc::NET_SESSION;
   memset( m_session, 0, sizeof( yiffurc::NET_SESSION ) );
   m_injects = gcnew LinkedList<UInt32>( );
   m_sends = gcnew LinkedList<UInt32>( );
}

NetSubsystem::~NetSubsystem( )
{
   delete m_session;
}

void NetSubsystem::onInbound( InboundEventArgs^ e )
{
   // Ignore lines this module has injected.
   if (m_injects->Count > 0)
   {
      LinkedListNode<UInt32>^ node = m_injects->FindLast( e->count );
      if (node != nullptr)
      {
         // Remove everything before.
         node = node->Next;
         while (m_injects->First != node)
            m_injects->RemoveFirst( );
         // Ignore this line.
         return;
      }
      if (m_injects->Last->Value < e->count)
         // Everything in the list is stale.
         m_injects->Clear( );
   }
      
   // Raise a raw event.
   {
      InboundRawEventArgs args;
      args.line = e->line;
      args.line_id = e->count;
      args.line_hide_flags = e->hide_flags;
      InboundRawEvent( this, %args );

      e->line = args.line;
      e->hide_flags = args.line_hide_flags;
   }

   // Parse.
   string line = CliUtil::CliBytesToMultiByteStr( e->line );
   if (line.length( ))
   {      
      /* Send a raw chat event before more specific chat messages. */
      if (line[ 0 ] == '(')
      {
         InboundRawChatEventArgs args;
         args.line            = e->line;
         args.line_id         = e->count;
         args.line_hide_flags = e->hide_flags;
         args.body            = gcnew String( line.c_str( ) + 1 );
         InboundRawChatEvent( this, %args );   

         e->line = args.line;
         e->hide_flags = args.line_hide_flags;
      }

      using namespace yiffurc;

      m_session->s2c_state.line_count = e->count;
      const S2C* s2c = s2c_parse( line.c_str( ), line.length( ), m_session );
      if (s2c)
      {
         switch (s2c->type)
         {
         case S2C_TYPE_AVTWEEN:
            {
               const S2C_AVTWEEN* parsed_s2c = (const S2C_AVTWEEN*) s2c;
               InboundAvatarMoveEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.guid  = parsed_s2c->uid;
               args.x     = parsed_s2c->x;
               args.y     = parsed_s2c->y;
               args.frame = parsed_s2c->frame;
               InboundAvatarMoveEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_AVCREATE:
            {
               const S2C_AVCREATE* parsed_s2c = (const S2C_AVCREATE*) s2c;
               InboundAvatarCreateEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.guid   = parsed_s2c->uid;
               args.x      = parsed_s2c->x;
               args.y      = parsed_s2c->y;
               args.frame  = parsed_s2c->frame;
               args.flags  = Monster_Flags::None;
               if (parsed_s2c->flags & furn2::MONSTER_HASPROFILE)
                  args.flags = args.flags | Monster_Flags::HasProfile;
               if (parsed_s2c->flags & furn2::MONSTER_UNK1)
                  args.flags = args.flags | Monster_Flags::Unk1;
               if (parsed_s2c->flags & furn2::MONSTER_UNK2)
                  args.flags = args.flags | Monster_Flags::Unk2;
               args.name   = gcnew String( parsed_s2c->name );
               args.colors = CliUtil::multiByteStrToCliBytes( string( parsed_s2c->colors ) );
               InboundAvatarCreateEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_AVDESTROY:
            {
               const S2C_AVDESTROY* parsed_s2c = (const S2C_AVDESTROY*) s2c;
               InboundAvatarDestroyEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.guid   = parsed_s2c->uid;
               InboundAvatarDestroyEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_AVHIDE:
            {
               const S2C_AVHIDE* parsed_s2c = (const S2C_AVHIDE*) s2c;
               InboundAvatarHideEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.guid   = parsed_s2c->uid;
               args.x      = parsed_s2c->x;
               args.y      = parsed_s2c->y;
               InboundAvatarHideEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_AVSHOW:
            {
               const S2C_AVSHOW* parsed_s2c = (const S2C_AVSHOW*) s2c;
               InboundAvatarShowEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.guid   = parsed_s2c->uid;
               args.x      = parsed_s2c->x;
               args.y      = parsed_s2c->y;
               args.frame  = parsed_s2c->frame;
               InboundAvatarShowEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_AVSHOW2:
            {
               const S2C_AVSHOW2* parsed_s2c = (const S2C_AVSHOW2*) s2c;
               InboundAvatarShow2EventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.guid   = parsed_s2c->uid;
               args.x      = parsed_s2c->x;
               args.y      = parsed_s2c->y;
               args.frame  = parsed_s2c->frame;
               InboundAvatarShow2Event( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_AVUPDATECOLORS:
            {
               const S2C_AVUPDATECOLORS* parsed_s2c = (const S2C_AVUPDATECOLORS*) s2c;
               InboundAvatarUpdateEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.guid   = parsed_s2c->uid;
               args.frame  = parsed_s2c->frame;
               args.colors = CliUtil::multiByteStrToCliBytes( parsed_s2c->colors );
               InboundAvatarUpdateEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_CAMTWEEN:
            {
               const S2C_CAMTWEEN* parsed_s2c = (const S2C_CAMTWEEN*) s2c;
               InboundCameraMoveEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.from_x      = parsed_s2c->x_old;
               args.from_y      = parsed_s2c->y_old;
               args.to_x        = parsed_s2c->x;
               args.to_y        = parsed_s2c->y;
               InboundCameraMoveEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_CAMSET:
            {
               const S2C_CAMSET* parsed_s2c = (const S2C_CAMSET*) s2c;
               InboundCameraSetEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.x      = parsed_s2c->x;
               args.y      = parsed_s2c->y;
               InboundCameraSetEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_SPEECH:
            {
               const S2C_SPEECH* parsed_s2c = (const S2C_SPEECH*) s2c;
               InboundChatEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->from_short );
               args.name       = gcnew String( parsed_s2c->from );
               args.body       = gcnew String( parsed_s2c->message );
               InboundChatEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_BADGE:
            {
               const S2C_BADGE* parsed_s2c = (const S2C_BADGE*) s2c;
               InboundChatBadgeEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.badge_id = parsed_s2c->badge;
               InboundChatBadgeEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_YOUSAY:
            {
               const S2C_YOUSAY* parsed_s2c = (const S2C_YOUSAY*) s2c;
               InboundChatEchoEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.body = gcnew String( parsed_s2c->message );
               InboundChatEchoEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_SPECITAG:
            {
               const S2C_SPECITAG* parsed_s2c = (const S2C_SPECITAG*) s2c;
               InboundChatSpeciTagEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.colors = CliUtil::multiByteStrToCliBytes( string( parsed_s2c->colors ).append( parsed_s2c->gsd, parsed_s2c->gsd + 3 ) );
               InboundChatSpeciTagEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DECLINED:
            {
               const S2C_DECLINED* parsed_s2c = (const S2C_DECLINED*) s2c;
               InboundDeclinedEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->player_short );
               args.name       = gcnew String( parsed_s2c->player );
               InboundDeclinedEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DECLINEECHO:
            {
               const S2C_DECLINEECHO* parsed_s2c = (const S2C_DECLINEECHO*) s2c;
               InboundDeclineEchoEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->player_short );
               args.name       = gcnew String( parsed_s2c->player );
               InboundDeclineEchoEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DECLINEFAIL:
            {
               const S2C_DECLINEFAIL* parsed_s2c = (const S2C_DECLINEFAIL*) s2c;
               InboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               InboundDeclineFailEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DESCBODY:
            {
               const S2C_DESCBODY* parsed_s2c = (const S2C_DESCBODY*) s2c;
               InboundDescBodyEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->player_short );
               args.body       = gcnew String( parsed_s2c->body );
               InboundDescBodyEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DESCBODY2:
            {
               S2C_DESCBODY2* parsed_s2c = (S2C_DESCBODY2*) s2c;
               InboundDescBodyEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->player_short );
               args.body       = gcnew String( parsed_s2c->body );
               InboundDescBodyEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DESCHEAD:
            {
               const S2C_DESCHEAD* parsed_s2c = (const S2C_DESCHEAD*) s2c;
               InboundDescHeadEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->player_short );
               args.name       = gcnew String( parsed_s2c->player );
               InboundDescHeadEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DRAGONROAR:
            {
               const S2C_DRAGONROAR* parsed_s2c = (const S2C_DRAGONROAR*) s2c;
               InboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               InboundDragonRoarEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DREAMPOS2:
            {
               S2C_DREAMPOS2* parsed_s2c = (S2C_DREAMPOS2*) s2c;
               InboundDreamPadCreateEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.name    = gcnew String( parsed_s2c->name );
               args.caption = gcnew String( parsed_s2c->caption );
               args.x       = parsed_s2c->x;
               args.y       = parsed_s2c->y;
               InboundDreamPadCreateEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DREAMCLEAR:
            {
               const S2C_DREAMCLEAR* parsed_s2c = (const S2C_DREAMCLEAR*) s2c;
               InboundDreamPadDestroyEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.x       = parsed_s2c->x;
               args.y       = parsed_s2c->y;
               InboundDreamPadDestroyEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DSEMIT:
            {
               const S2C_DSEMIT* parsed_s2c = (const S2C_DSEMIT*) s2c;
               InboundDsEmitEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.body = gcnew String( parsed_s2c->message );
               InboundDsEmitEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_TRIGGER:
            {
               const S2C_TRIGGER* parsed_s2c = (const S2C_TRIGGER*) s2c;
               InboundDsTriggerEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.x1      = parsed_s2c->x1;
               args.y1      = parsed_s2c->y1;
               args.x2      = parsed_s2c->x2;
               args.y2      = parsed_s2c->y2;
               args.triggers = gcnew array<DsTriggerLine>( parsed_s2c->count );
               for (unsigned int i = 0; i < parsed_s2c->count; ++i)
               {
                  args.triggers[ i ].line   = parsed_s2c->triggers[ i ].line;
                  args.triggers[ i ].param1 = parsed_s2c->triggers[ i ].x;
                  args.triggers[ i ].param2 = parsed_s2c->triggers[ i ].y;
               }
               InboundDsTriggerEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_TRIGGERSELF:
            {
               const S2C_TRIGGERSELF* parsed_s2c = (const S2C_TRIGGERSELF*) s2c;
               InboundDsTriggerSelfEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.x1      = parsed_s2c->x1;
               args.y1      = parsed_s2c->y1;
               args.x2      = parsed_s2c->x2;
               args.y2      = parsed_s2c->y2;
               args.triggers = gcnew array<DsTriggerLine>( parsed_s2c->count );
               for (unsigned int i = 0; i < parsed_s2c->count; ++i)
               {
                  args.triggers[ i ].line   = parsed_s2c->triggers[ i ].line;
                  args.triggers[ i ].param1 = parsed_s2c->triggers[ i ].x;
                  args.triggers[ i ].param2 = parsed_s2c->triggers[ i ].y;
               }
               InboundDsTriggerSelfEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_VARSET:
            {
               const S2C_VARSET* parsed_s2c = (const S2C_VARSET*) s2c;
               InboundDsVarSetEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.vars = gcnew array<DsVarSet>( parsed_s2c->count );
               for (unsigned int i = 0; i < parsed_s2c->count; ++i)
               {
                  args.vars[ i ].var_first = parsed_s2c->set[ i ].first;
                  args.vars[ i ].var_count = parsed_s2c->set[ i ].count;
                  args.vars[ i ].var_value = parsed_s2c->set[ i ].value;
               }
               InboundDsVarSetEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_EMIT:
            {
               const S2C_EMIT* parsed_s2c = (const S2C_EMIT*) s2c;
               InboundEmitEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.body = gcnew String( parsed_s2c->message );
               InboundEmitEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_EMOTE:
            {
               const S2C_EMOTE* parsed_s2c = (const S2C_EMOTE*) s2c;
               InboundEmoteEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->from_short );
               args.name       = gcnew String( parsed_s2c->from );
               args.body       = gcnew String( parsed_s2c->message );
               InboundEmoteEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_FOOTOBJ:
            {
               const S2C_FOOTOBJ* parsed_s2c = (const S2C_FOOTOBJ*) s2c;
               InboundFeetObjectSetEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.obj = parsed_s2c->obj;
               InboundFeetObjectSetEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_FETCHDREAM:
            {
               const S2C_FETCHDREAM* parsed_s2c = (const S2C_FETCHDREAM*) s2c;
               InboundFetchDreamEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.name = gcnew String( parsed_s2c->name );
               args.id   = parsed_s2c->did;
               InboundFetchDreamEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_FETCHPID:
            {
               const S2C_FETCHPID* parsed_s2c = (const S2C_FETCHPID*) s2c;
               InboundFetchPortraitEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.pid = parsed_s2c->pid;
               InboundFetchPortraitEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_FLOORSET:
            {
               const S2C_FLOORSET* parsed_s2c = (const S2C_FLOORSET*) s2c;
               InboundFloorSetEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.floors = gcnew array<FloorSet>( parsed_s2c->count );
               for (unsigned int i = 0; i < parsed_s2c->count; ++i)
               {
                  args.floors[ i ].floor_id = parsed_s2c->floors[ i ].id;
                  args.floors[ i ].x = parsed_s2c->floors[ i ].x;
                  args.floors[ i ].y = parsed_s2c->floors[ i ].y;
               }
               InboundFloorSetEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_INVOBJ:
            {
               const S2C_INVOBJ* parsed_s2c = (const S2C_INVOBJ*) s2c;
               InboundInventoryObjectSetEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.obj = parsed_s2c->obj;
               InboundInventoryObjectSetEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_JOINED:
            {
               const S2C_JOINED* parsed_s2c = (const S2C_JOINED*) s2c;
               InboundJoinedEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->joiner_short );
               args.name       = gcnew String( parsed_s2c->joiner );
               InboundJoinedEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_JOIN:
            {
               const S2C_JOIN* parsed_s2c = (const S2C_JOIN*) s2c;
               InboundJoinRequestEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->from_short );
               args.name       = gcnew String( parsed_s2c->from );
               InboundJoinRequestEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_LOGINFAIL:
            {
               const S2C_LOGINFAIL* parsed_s2c = (const S2C_LOGINFAIL*) s2c;
               InboundLoginFailEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.reason = gcnew String( parsed_s2c->reason );
               InboundLoginFailEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_WHISPERFAIL:
            {
               const S2C_WHISPERFAIL* parsed_s2c = (const S2C_WHISPERFAIL*) s2c;
               InboundNameLookupFailEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.name = gcnew String( parsed_s2c->player );
               InboundNameLookupFailEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_OBJSET:
            {
               const S2C_OBJSET* parsed_s2c = (const S2C_OBJSET*) s2c;
               InboundObjectSetEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.objects = gcnew array<ObjectSet>( parsed_s2c->count );
               for (unsigned int i = 0; i < parsed_s2c->count; ++i)
               {
                  args.objects[ i ].obj_id = parsed_s2c->objects[ i ].id;
                  args.objects[ i ].x = parsed_s2c->objects[ i ].x;
                  args.objects[ i ].y = parsed_s2c->objects[ i ].y;
               }
               InboundObjectSetEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_ONLINERESULT:
            {
               const S2C_ONLINERESULT* parsed_s2c = (const S2C_ONLINERESULT*) s2c;
               InboundOnlineCheckEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.name   = gcnew String( parsed_s2c->player );
               args.online = parsed_s2c->online;
               InboundOnlineCheckEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_PLAYSONG:
            {
               const S2C_PLAYSONG* parsed_s2c = (const S2C_PLAYSONG*) s2c;
               InboundPlaySongEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.song_id = parsed_s2c->song;
               InboundPlaySongEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_PLAYSOUND:
            {
               const S2C_PLAYSOUND* parsed_s2c = (const S2C_PLAYSOUND*) s2c;
               InboundPlaySoundEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.sound_id = parsed_s2c->sound;
               InboundPlaySoundEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_PORTRAIT:
            {
               const S2C_PORTRAIT* parsed_s2c = (const S2C_PORTRAIT*) s2c;
               InboundPortraitSetEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.name = gcnew String( parsed_s2c->player );
               args.colors = CliUtil::multiByteStrToCliBytes( string( parsed_s2c->colors ).append( parsed_s2c->gsd, parsed_s2c->gsd + 3 ) );
               InboundPortraitSetEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_CREATEFAIL:
            {
               const S2C_CREATEFAIL* parsed_s2c = (const S2C_CREATEFAIL*) s2c;
               InboundRegisterFailEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.reason = gcnew String( parsed_s2c->reason );
               InboundRegisterFailEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_SUMMONJOINECHO:
            {
               const S2C_SUMMONJOINECHO* parsed_s2c = (const S2C_SUMMONJOINECHO*) s2c;
               InboundRequestEchoEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->player_short );
               args.name = gcnew String( parsed_s2c->player );
               InboundRequestEchoEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_ROLL:
            {
               const S2C_ROLL* parsed_s2c = (const S2C_ROLL*) s2c;
               InboundRollEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->player_short );
               args.name       = gcnew String( parsed_s2c->player );
               args.num_dice   = parsed_s2c->dice;
               args.num_sides  = parsed_s2c->sides;
               args.sum        = parsed_s2c->sum;
               args.body       = gcnew String( parsed_s2c->message );
               InboundRollEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_SHOUT:
            {
               const S2C_SHOUT* parsed_s2c = (const S2C_SHOUT*) s2c;
               InboundShoutEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->from_short );
               args.name       = gcnew String( parsed_s2c->from );
               args.body       = gcnew String( parsed_s2c->message );
               InboundShoutEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_YOUSHOUT:
            {
               const S2C_YOUSHOUT* parsed_s2c = (const S2C_YOUSHOUT*) s2c;
               InboundShoutEchoEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.body       = gcnew String( parsed_s2c->message );
               InboundShoutEchoEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_PASS:
            {
               const S2C_PASS* parsed_s2c = (const S2C_PASS*) s2c;
               InboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               InboundSuccessEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_SUMMONED:
            {
               const S2C_SUMMONED* parsed_s2c = (const S2C_SUMMONED*) s2c;
               InboundSummonedEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->summoner_short );
               args.name       = gcnew String( parsed_s2c->summoner );
               InboundSummonedEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_SUMMON:
            {
               const S2C_SUMMON* parsed_s2c = (const S2C_SUMMON*) s2c;
               InboundSummonRequestEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name  = gcnew String( parsed_s2c->from_short );
               args.name        = gcnew String( parsed_s2c->from );
               args.destination = gcnew String( parsed_s2c->dest );
               InboundSummonRequestEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DISPLISTOPEN:
            {
               const S2C_DISPLISTOPEN* parsed_s2c = (const S2C_DISPLISTOPEN*) s2c;
               InboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               InboundSuppressRenderingEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_SYNCCOLORCODE:
            {
               const S2C_SYNCCOLORCODE* parsed_s2c = (const S2C_SYNCCOLORCODE*) s2c;
               InboundSyncColorCodeEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.colors = CliUtil::multiByteStrToCliBytes( string( parsed_s2c->colors ).append( parsed_s2c->gsd, parsed_s2c->gsd + 3 ) );
               InboundSyncColorCodeEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_DISPLISTCLOSE:
            {
               const S2C_DISPLISTCLOSE* parsed_s2c = (const S2C_DISPLISTCLOSE*) s2c;
               InboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               InboundUnsuppressRenderingEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_USEBGFILE:
            {
               const S2C_USEBGFILE* parsed_s2c = (const S2C_USEBGFILE*) s2c;
               InboundUseBgFileEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.filename = gcnew String( parsed_s2c->file );
               InboundUseBgFileEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_WHISPER:
            {
               const S2C_WHISPER* parsed_s2c = (const S2C_WHISPER*) s2c;
               InboundWhisperEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->from_short );
               args.name       = gcnew String( parsed_s2c->from );
               args.body       = gcnew String( parsed_s2c->message );
               InboundWhisperEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_WHISPERDENIED:
            {
               const S2C_WHISPERDENIED* parsed_s2c = (const S2C_WHISPERDENIED*) s2c;
               InboundWhisperDeniedEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->player_short );
               args.name       = gcnew String( parsed_s2c->player );
               InboundWhisperDeniedEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case S2C_TYPE_YOUWHISPER:
            {
               const S2C_YOUWHISPER* parsed_s2c = (const S2C_YOUWHISPER*) s2c;
               InboundWhisperEchoEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.short_name = gcnew String( parsed_s2c->to_short );
               args.name       = gcnew String( parsed_s2c->to );
               args.body       = gcnew String( parsed_s2c->message );
               InboundWhisperEchoEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         } // switch (s2c->type)
         yiffurc_release( s2c );
      } // if (s2c)
   } // if (line.length( ))
}

void NetSubsystem::onLoggedIn( )
{
   m_connection_level = Connection_Level::LoggedIn;
   m_session->level = yiffurc::NET_LEVEL_LOGGEDIN;
   LoggedInEvent( this, gcnew EventArgs );
}

Boolean NetSubsystem::decodeAvatarFrameNumber( UInt32 frame, 
                                               [Out] Avatar_Class% av, 
                                               [Out] Avatar_Pose% pose, 
                                               [Out] Move_Direction% direction )
{
   using namespace yiffurc;
   AVFRAME avframe;
   if (frame2av( frame, &avframe ))
   {
      switch (avframe.cls)
      {
      default:
      case AVCLASS_RODENT:
         av = Avatar_Class::Rodent; break;
      case AVCLASS_EQUINE:
         av = Avatar_Class::Equine; break;
      case AVCLASS_FELINE:
         av = Avatar_Class::Feline; break;
      case AVCLASS_CANINE:
         av = Avatar_Class::Canine; break;
      case AVCLASS_MUSTELINE:
         av = Avatar_Class::Musteline; break;
      case AVCLASS_LAPINE:
         av = Avatar_Class::Lapine; break;
      case AVCLASS_RODENTCW:
         av = Avatar_Class::RodentClassicWings; break;
      case AVCLASS_EQUINECW:
         av = Avatar_Class::EquineClassicWings; break;
      case AVCLASS_FELINECW:
         av = Avatar_Class::FelineClassicWings; break;
      case AVCLASS_CANINECW:
         av = Avatar_Class::CanineClassicWings; break;
      case AVCLASS_MUSTELINECW:
         av = Avatar_Class::MustelineClassicWings; break;
      case AVCLASS_KIWI:
         av = Avatar_Class::Kiwi; break;
      case AVCLASS_LAPINECW:
         av = Avatar_Class::LapineClassicWings; break;
      case AVCLASS_DRAGON:
         av = Avatar_Class::Dragon; break;
      case AVCLASS_PHOENIX:
         av = Avatar_Class::Phoenix; break;
      case AVCLASS_RODENTTW:
         av = Avatar_Class::RodentTriWings; break;
      case AVCLASS_EQUINETW:
         av = Avatar_Class::EquineTriWings; break;
      case AVCLASS_FELINETW:
         av = Avatar_Class::FelineTriWings; break;
      case AVCLASS_CANINETW:
         av = Avatar_Class::CanineTriWings; break;
      case AVCLASS_MUSTELINETW:
         av = Avatar_Class::MustelineTriWings; break;
      case AVCLASS_LAPINETW:
         av = Avatar_Class::LapineTriWings; break;
      case AVCLASS_RODENTFW:
         av = Avatar_Class::RodentButterflyWings; break;
      case AVCLASS_EQUINEFW:
         av = Avatar_Class::EquineButterflyWings; break;
      case AVCLASS_FELINEFW:
         av = Avatar_Class::FelineButterflyWings; break;
      case AVCLASS_CANINEFW:
         av = Avatar_Class::CanineButterflyWings; break;
      case AVCLASS_MUSTELINEFW:
         av = Avatar_Class::MustelineButterflyWings; break;
      case AVCLASS_LAPINEFW:
         av = Avatar_Class::LapineButterflyWings; break;
      case AVCLASS_GRYFFE:
         av = Avatar_Class::Gryffe; break;
      case AVCLASS_EAGLE:
         av = Avatar_Class::Eagle; break;
      case AVCLASS_RODENTBW:
         av = Avatar_Class::RodentBatWings; break;
      case AVCLASS_EQUINEBW:
         av = Avatar_Class::EquineBatWings; break;
      case AVCLASS_FELINEBW:
         av = Avatar_Class::FelineBatWings; break;
      case AVCLASS_CANINEBW:
         av = Avatar_Class::CanineBatWings; break;
      case AVCLASS_MUSTELINEBW:
         av = Avatar_Class::MustelineBatWings; break;
      case AVCLASS_LAPINEBW:
         av = Avatar_Class::LapineBatWings; break;
      case AVCLASS_SCIURINE:
         av = Avatar_Class::Sciurine; break;
      case AVCLASS_SCIURINECW:
         av = Avatar_Class::SciurineClassicWings; break;
      case AVCLASS_SCIURINETW:
         av = Avatar_Class::SciurineTriWings; break;
      case AVCLASS_SCIURINEFW:
         av = Avatar_Class::SciurineButterflyWings; break;
      case AVCLASS_SCIURINEBW:
         av = Avatar_Class::SciurineBatWings; break;
      case AVCLASS_BOVINE:
         av = Avatar_Class::Bovine; break;
      case AVCLASS_BOVINECW:
         av = Avatar_Class::BovineClassicWings; break;
      case AVCLASS_BOVINETW:
         av = Avatar_Class::BovineTriWings; break;
      case AVCLASS_BOVINEFW:
         av = Avatar_Class::BovineButterflyWings; break;
      case AVCLASS_BOVINEBW:
         av = Avatar_Class::BovineBatWings; break;
      case AVCLASS_RODENTPW:
         av = Avatar_Class::BovinePrimeWings; break;
      case AVCLASS_EQUINEPW:
         av = Avatar_Class::EquinePrimeWings; break;
      case AVCLASS_FELINEPW:
         av = Avatar_Class::FelinePrimeWings; break;
      case AVCLASS_CANINEPW:
         av = Avatar_Class::CaninePrimeWings; break;
      case AVCLASS_MUSTELINEPW:
         av = Avatar_Class::MustelinePrimeWings; break;
      case AVCLASS_LAPINEPW:
         av = Avatar_Class::LapinePrimeWings; break;
      case AVCLASS_SCIURINEPW:
         av = Avatar_Class::SciurinePrimeWings; break;
      case AVCLASS_BOVINEPW:
         av = Avatar_Class::BovinePrimeWings; break;
      case AVCLASS_PIXIE:
         av = Avatar_Class::Pixie; break;
      case AVCLASS_KITTER:
         av = Avatar_Class::Kitter; break;
      case AVCLASS_WOLVEN:
         av = Avatar_Class::Wolven; break;
      case AVCLASS_URSINE:
         av = Avatar_Class::Ursine; break;
      case AVCLASS_URSINECW:
         av = Avatar_Class::UrsineClassicWings; break;
      case AVCLASS_URSINETW:
         av = Avatar_Class::UrsineTriWings; break;
      case AVCLASS_URSINEFW:
         av = Avatar_Class::UrsineButterflyWings; break;
      case AVCLASS_URSINEBW:
         av = Avatar_Class::UrsineBatWings; break;
      case AVCLASS_URSINEPW:
         av = Avatar_Class::UrsinePrimeWings; break;
      case AVCLASS_TYGARD:
         av = Avatar_Class::Tygard; break;
      case AVCLASS_LEONEN:
         av = Avatar_Class::Leonen; break;
      case AVCLASS_WEREWOLF:
         av = Avatar_Class::Werewolf; break;
      case AVCLASS_PENGUIN:
         av = Avatar_Class::Penguin; break;
      case AVCLASS_BAT:
         av = Avatar_Class::Bat; break;
      case AVCLASS_FOXEN:
         av = Avatar_Class::Foxen; break;
      case AVCLASS_FLOXEN:
         av = Avatar_Class::Floxen; break;
      case AVCLASS_UNICORN:
         av = Avatar_Class::Unicorn; break;
      case AVCLASS_FURLINGM:
         av = Avatar_Class::FurlingMale; break;
      case AVCLASS_FURLINGF:
         av = Avatar_Class::FurlingFemale; break;
      case AVCLASS_CHINCHILLA:
         av = Avatar_Class::Chinchilla; break;
      case AVCLASS_TOASTER:
         av = Avatar_Class::Toaster; break;
      }

      switch (avframe.pose)
      {
      case AVPOSE_SIT:
         pose = Avatar_Pose::Sitting; break;
      case AVPOSE_WALK0:
      default:
         pose = Avatar_Pose::Walking1; break;
      case AVPOSE_STAND:
         pose = Avatar_Pose::Standing; break;
      case AVPOSE_WALK1:
         pose = Avatar_Pose::Walking2; break;
      case AVPOSE_LIE:
         pose = Avatar_Pose::Laying; break;
      }

      switch (avframe.dir)
      {
      case AVDIR_NE:
         direction = Move_Direction::NorthEast; break;
      case AVDIR_NW:
         direction = Move_Direction::NorthWest; break;
      case AVDIR_SE:
         direction = Move_Direction::SouthEast; break;
      default:
      case AVDIR_SW:
         direction = Move_Direction::SouthWest; break;
      }

      return true;
   }
   return false;
}

UInt32 NetSubsystem::decodeBase220Number( array<Byte>^ base220 )
{
   UInt32 n   = 0;
   UInt32 mul = 1;
   for each (Byte c in base220)
   {
      n += ((unsigned char) c - (unsigned char) '#') * mul;
      mul *= 220;
   }
   return n;
}

UInt32 NetSubsystem::decodeBase220Number( array<Byte>^ base220, UInt32 offset, UInt32 len )
{
   UInt32 n   = 0;
   UInt32 mul = 1;
   if (len > (UInt32) base220->Length)
      len = base220->Length;
   for (UInt32 i = offset; i < len; ++i)
   {
      n += ((unsigned char) base220[ i ] - (unsigned char) '#') * mul;
      mul *= 220;
   }
   return n;
}

UInt32 NetSubsystem::decodeBase95Number( array<Byte>^ base95 )
{
   UInt32 n = 0;
   for each (Byte c in base95)
   {
      n = (n * 95) + ((unsigned char) c - (unsigned char) ' ');
   }
   return n;
}

UInt32 NetSubsystem::decodeBase95Number( array<Byte>^ base95, UInt32 offset, UInt32 len )
{
   UInt32 n = 0;
   if (len > (UInt32) base95->Length)
      len = base95->Length;
   for (UInt32 i = offset; i < len; ++i)
   {
      n = (n * 95) + ((unsigned char) base95[ i ] - (unsigned char) ' ');
   }
   return n;
}

UInt32 NetSubsystem::encodeAvatarFrameNumber( Avatar_Class av, 
                                              Avatar_Pose pose,
                                              Move_Direction direction )
{
   using namespace yiffurc;
   AVFRAME af;
   switch (av)
   {
   default:
   case Avatar_Class::Rodent:
      af.cls = AVCLASS_RODENT; break;
   case Avatar_Class::Equine:
      af.cls = AVCLASS_EQUINE; break;
   case Avatar_Class::Feline:
      af.cls = AVCLASS_FELINE; break;
   case Avatar_Class::Canine:
      af.cls = AVCLASS_CANINE; break;
   case Avatar_Class::Musteline:
      af.cls = AVCLASS_MUSTELINE; break;
   case Avatar_Class::Lapine:
      af.cls = AVCLASS_LAPINE; break;
   case Avatar_Class::RodentClassicWings:
      af.cls = AVCLASS_RODENTCW; break;
   case Avatar_Class::EquineClassicWings:
      af.cls = AVCLASS_EQUINECW; break;
   case Avatar_Class::FelineClassicWings:
      af.cls = AVCLASS_FELINECW; break;
   case Avatar_Class::CanineClassicWings:
      af.cls = AVCLASS_CANINECW; break;
   case Avatar_Class::MustelineClassicWings:
      af.cls = AVCLASS_MUSTELINECW; break;
   case Avatar_Class::Kiwi:
      af.cls = AVCLASS_KIWI; break;
   case Avatar_Class::LapineClassicWings:
      af.cls = AVCLASS_LAPINECW; break;
   case Avatar_Class::Dragon:
      af.cls = AVCLASS_DRAGON; break;
   case Avatar_Class::Phoenix:
      af.cls = AVCLASS_PHOENIX; break;
   case Avatar_Class::RodentTriWings:
      af.cls = AVCLASS_RODENTTW; break;
   case Avatar_Class::EquineTriWings:
      af.cls = AVCLASS_EQUINETW; break;
   case Avatar_Class::FelineTriWings:
      af.cls = AVCLASS_FELINETW; break;
   case Avatar_Class::CanineTriWings:
      af.cls = AVCLASS_CANINETW; break;
   case Avatar_Class::MustelineTriWings:
      af.cls = AVCLASS_MUSTELINETW; break;
   case Avatar_Class::LapineTriWings:
      af.cls = AVCLASS_LAPINETW; break;
   case Avatar_Class::RodentButterflyWings:
      af.cls = AVCLASS_RODENTFW; break;
   case Avatar_Class::EquineButterflyWings:
      af.cls = AVCLASS_EQUINEFW; break;
   case Avatar_Class::FelineButterflyWings:
      af.cls = AVCLASS_FELINEFW; break;
   case Avatar_Class::CanineButterflyWings:
      af.cls = AVCLASS_CANINEFW; break;
   case Avatar_Class::MustelineButterflyWings:
      af.cls = AVCLASS_MUSTELINEFW; break;
   case Avatar_Class::LapineButterflyWings:
      af.cls = AVCLASS_LAPINEFW; break;
   case Avatar_Class::Gryffe:
      af.cls = AVCLASS_GRYFFE; break;
   case Avatar_Class::Eagle:
      af.cls = AVCLASS_EAGLE; break;
   case Avatar_Class::RodentBatWings:
      af.cls = AVCLASS_RODENTBW; break;
   case Avatar_Class::EquineBatWings:
      af.cls = AVCLASS_EQUINEBW; break;
   case Avatar_Class::FelineBatWings:
      af.cls = AVCLASS_FELINEBW; break;
   case Avatar_Class::CanineBatWings:
      af.cls = AVCLASS_CANINEBW; break;
   case Avatar_Class::MustelineBatWings:
      af.cls = AVCLASS_MUSTELINEBW; break;
   case Avatar_Class::LapineBatWings:
      af.cls = AVCLASS_LAPINEBW; break;
   case Avatar_Class::Sciurine:
      af.cls = AVCLASS_SCIURINE; break;
   case Avatar_Class::SciurineClassicWings:
      af.cls = AVCLASS_SCIURINECW; break;
   case Avatar_Class::SciurineTriWings:
      af.cls = AVCLASS_SCIURINETW; break;
   case Avatar_Class::SciurineButterflyWings:
      af.cls = AVCLASS_SCIURINEFW; break;
   case Avatar_Class::SciurineBatWings:
      af.cls = AVCLASS_SCIURINEBW; break;
   case Avatar_Class::Bovine:
      af.cls = AVCLASS_BOVINE; break;
   case Avatar_Class::BovineClassicWings:
      af.cls = AVCLASS_BOVINECW; break;
   case Avatar_Class::BovineTriWings:
      af.cls = AVCLASS_BOVINETW; break;
   case Avatar_Class::BovineButterflyWings:
      af.cls = AVCLASS_BOVINEFW; break;
   case Avatar_Class::BovineBatWings:
      af.cls = AVCLASS_BOVINEBW; break;
   case Avatar_Class::RodentPrimeWings:
      af.cls = AVCLASS_RODENTPW; break;
   case Avatar_Class::EquinePrimeWings:
      af.cls = AVCLASS_EQUINEPW; break;
   case Avatar_Class::FelinePrimeWings:
      af.cls = AVCLASS_FELINEPW; break;
   case Avatar_Class::CaninePrimeWings:
      af.cls = AVCLASS_CANINEPW; break;
   case Avatar_Class::MustelinePrimeWings:
      af.cls = AVCLASS_MUSTELINEPW; break;
   case Avatar_Class::LapinePrimeWings:
      af.cls = AVCLASS_LAPINEPW; break;
   case Avatar_Class::SciurinePrimeWings:
      af.cls = AVCLASS_SCIURINEPW; break;
   case Avatar_Class::BovinePrimeWings:
      af.cls = AVCLASS_BOVINEPW; break;
   case Avatar_Class::Pixie:
      af.cls = AVCLASS_PIXIE; break;
   case Avatar_Class::Kitter:
      af.cls = AVCLASS_KITTER; break;
   case Avatar_Class::Wolven:
      af.cls = AVCLASS_WOLVEN; break;
   case Avatar_Class::Ursine:
      af.cls = AVCLASS_URSINE; break;
   case Avatar_Class::UrsineClassicWings:
      af.cls = AVCLASS_URSINECW; break;
   case Avatar_Class::UrsineTriWings:
      af.cls = AVCLASS_URSINETW; break;
   case Avatar_Class::UrsineButterflyWings:
      af.cls = AVCLASS_URSINEFW; break;
   case Avatar_Class::UrsineBatWings:
      af.cls = AVCLASS_URSINEBW; break;
   case Avatar_Class::UrsinePrimeWings:
      af.cls = AVCLASS_URSINEPW; break;
   case Avatar_Class::Tygard:
      af.cls = AVCLASS_TYGARD; break;
   case Avatar_Class::Leonen:
      af.cls = AVCLASS_LEONEN; break;
   case Avatar_Class::Werewolf:
      af.cls = AVCLASS_WEREWOLF; break;
   case Avatar_Class::Penguin:
      af.cls = AVCLASS_PENGUIN; break;
   case Avatar_Class::Bat:
      af.cls = AVCLASS_BAT; break;
   case Avatar_Class::Foxen:
      af.cls = AVCLASS_FOXEN; break;
   case Avatar_Class::Floxen:
      af.cls = AVCLASS_FLOXEN; break;
   case Avatar_Class::Unicorn:
      af.cls = AVCLASS_UNICORN; break;
   case Avatar_Class::FurlingMale:
      af.cls = AVCLASS_FURLINGM; break;
   case Avatar_Class::FurlingFemale:
      af.cls = AVCLASS_FURLINGF; break;
   case Avatar_Class::Chinchilla:
      af.cls = AVCLASS_CHINCHILLA; break;
   case Avatar_Class::Toaster:
      af.cls = AVCLASS_TOASTER; break;
   }
   
   switch (pose)
   {
   default:
   case Avatar_Pose::Standing:
      af.pose = AVPOSE_STAND; break;
   case Avatar_Pose::Sitting:
      af.pose = AVPOSE_SIT; break;
   case Avatar_Pose::Walking1:
      af.pose = AVPOSE_WALK0; break;
   case Avatar_Pose::Walking2:
      af.pose = AVPOSE_WALK1; break;
   case Avatar_Pose::Laying:
      af.pose = AVPOSE_LIE; break;
   }

   switch (direction)
   {
   default:
   case Move_Direction::SouthWest:
      af.dir = AVDIR_SW; break;
   case Move_Direction::SouthEast:
      af.dir = AVDIR_SE; break;
   case Move_Direction::NorthWest:
      af.dir = AVDIR_NW; break;
   case Move_Direction::NorthEast:
      af.dir = AVDIR_NE; break;
   }

   return av2frame( &af );
}

array<Byte>^ NetSubsystem::encodeBase220Number( UInt32 n, UInt32 len )
{
   std::string str( '#', len );
   for (UInt32 i = 0; n && i < len; ++i)
   {
      str[ i ] = (unsigned short) ((unsigned char) '#' + (n % 220));
      n /= 220;
   }
   return CliUtil::charsToCliBytes( str.c_str( ), str.length( ) );
}

array<Byte>^ NetSubsystem::encodeBase95Number( UInt32 n, UInt32 len )
{
   std::string str( ' ', len );
   UInt32 modulo = 1;
   for (UInt32 i = 0; i < len; ++i)
      modulo *= 95;
   for (UInt32 i = 0; i < len; ++i)
   {
      str[ i ] = (unsigned short) ((unsigned char) ' ' + (unsigned char) ((n / modulo) % 95));
      modulo /= 95;
   }
   return CliUtil::charsToCliBytes( str.c_str( ), str.length( ) );
}

array<Byte>^ NetSubsystem::encodeOutboundChangeColor( array<Byte>^ colors )
{
   using namespace yiffurc;
   string str_colors = CliUtil::CliBytesToMultiByteStr( colors );
   string str_gsd;
   if (str_colors.length( ) >= 3)
   {
      str_gsd = str_colors.substr( str_colors.length( ) - 3 );
      str_colors.resize( str_colors.length( ) - 3 );
   }
   const char* sz_cmd = c2s_enc_changecolor( str_colors.c_str( ), str_gsd.c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundChangeDesc( String^ desc )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_changedesc( CliUtil::cliStrToAscii( desc ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundChat( String^ body )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_speech( CliUtil::cliStrToAscii( body ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundDecline( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_decline( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundDreamReady( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_downloadfinished( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundEmote( String^ body )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_emote( CliUtil::cliStrToAscii( body ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundFollowDreamLink( String^ furl )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_fdl( CliUtil::cliStrToAscii( furl ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundGoBack( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_goback( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundGoMap( UInt32 map_id )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_gomap( map_id );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundJoin( String^ name )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_join( CliUtil::cliStrToAscii( name ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundLay( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_liedown( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundLogin( String^ name, String^ password, String^ mac_id )
{
   using namespace yiffurc;
   const char* sz_cmd;
   if (mac_id != nullptr)
      sz_cmd = c2s_enc_login( CliUtil::cliStrToAscii( name ).c_str( ), 
                              CliUtil::cliStrToAscii( password ).c_str( ), 
                              CliUtil::cliStrToAscii( mac_id ).c_str( ) );
   else
      sz_cmd = c2s_enc_login( CliUtil::cliStrToAscii( name ).c_str( ), 
                              CliUtil::cliStrToAscii( password ).c_str( ), 
                              NULL );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundLookPos( UInt32 x, UInt32 y )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_lookpos( x, y );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundMove( Move_Direction move_dir )
{
   using namespace yiffurc;
   yiffurc::Move_Dir md;
   switch (move_dir)
   {
   case Move_Direction::NorthEast:
      md = yiffurc::MOVEDIR_NE; break;
   case Move_Direction::NorthWest:
      md = yiffurc::MOVEDIR_NW; break;
   case Move_Direction::SouthEast:
      md = yiffurc::MOVEDIR_SE; break;
   case Move_Direction::SouthWest:
   default:
      md = yiffurc::MOVEDIR_SW; break;
   }
   const char* sz_cmd = c2s_enc_move( md );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundOnlineCheck( String^ name )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_onlinecheck( CliUtil::cliStrToAscii( name ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundPickUp( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_pickup( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundPortraitChange( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_portraitchange( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundQuit( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_quit( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundSetColor( array<Byte>^ colors )
{
   using namespace yiffurc;
   string str_colors = CliUtil::CliBytesToMultiByteStr( colors );
   string str_gsd;
   if (str_colors.length( ) >= 3)
   {
      str_gsd = str_colors.substr( str_colors.length( ) - 3 );
      str_colors.resize( str_colors.length( ) - 3 );
   }
   const char* sz_cmd = c2s_enc_setcolor( str_colors.c_str( ), str_gsd.c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundSetDesc( String^ desc )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_setdesc( CliUtil::cliStrToAscii( desc ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundShout( String^ body )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_shout( CliUtil::cliStrToAscii( body ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundShoutToggle( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_shouttoggle( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundSit( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_sitdown( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundSitStandLay( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_sitstandlie( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundStand( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_standup( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundSummon( String^ name )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_summon( CliUtil::cliStrToAscii( name ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundTurnLeft( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_turnleft( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundTurnRight( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_turnright( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundUseItem( )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_use( );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}

array<Byte>^ NetSubsystem::encodeOutboundWhisper( String^ name, String^ body )
{
   using namespace yiffurc;
   const char* sz_cmd = c2s_enc_whisper( CliUtil::cliStrToAscii( name ).c_str( ),
                                         CliUtil::cliStrToAscii( body ).c_str( ) );
   array<Byte>^ cmd = CliUtil::multiByteStrToCliBytes( sz_cmd );
   yiffurc_release( sz_cmd );
   return cmd;
}


array<Byte>^ NetSubsystem::format( String^ pattern, ... array<Object^>^ _fields )
{
   list<net_scan::FIELD> fields;
   for each (Object^ o in _fields)
   {
      net_scan::FIELD f;
      switch (Type::GetTypeCode( o->GetType( ) ))
      {
      case TypeCode::Int16:
      case TypeCode::Int32:
      case TypeCode::UInt16:
      case TypeCode::UInt32:
      case TypeCode::Byte:
      case TypeCode::SByte:
         f.type = net_scan::FIELD::FIELD_INT;
         f.value_int = Convert::ToUInt32( o );
         break;
      default:
         f.type = net_scan::FIELD::FIELD_INT;
         if (o->GetType( ) == array<byte>::typeid)
            f.value_str = CliUtil::CliBytesToMultiByteStr( (array<byte>^) o );
         else
            f.value_str = CliUtil::cliStrToMultiByteStr( o->ToString( ) );
      }

      fields.push_back( f );
   }

   return CliUtil::multiByteStrToCliBytes( 
      net_scan::format( CliUtil::cliStrToMultiByteStr( pattern ), fields ) );   
}

UInt32 NetSubsystem::inject( array<Byte>^ line )
{
   vector<char> l = CliUtil::cliBytesToChars( line );
   UInt32 id = furn2::f2_netinline( &l.front( ), l.size( ) );
   m_injects->AddLast( id );
   return id;
}

void NetSubsystem::onConnected( )
{
   m_connection_level = Connection_Level::Connected;
   m_session->level = yiffurc::NET_LEVEL_CONNECTED;
   ConnectedEvent( this, gcnew EventArgs );
}

void NetSubsystem::onDisconnected( )
{
   m_connection_level = Connection_Level::Disconnected;
   m_session->level = yiffurc::NET_LEVEL_CONNECTED; // Is no disconnected state in yiffurc.
   DisconnectedEvent( this, gcnew EventArgs );
}
   
void NetSubsystem::onOutbound( OutboundEventArgs^ e )
{
   // Ignore lines this module has injected.
   if (m_sends->Count > 0)
   {
      LinkedListNode<UInt32>^ node = m_sends->FindLast( e->count );
      if (node != nullptr)
      {
         // Remove everything before.
         node = node->Next;
         while (m_sends->First != node)
            m_sends->RemoveFirst( );
         // Ignore this line.
         return;
      }
      if (m_sends->Last->Value < e->count)
         // Everything in the list is stale.
         m_sends->Clear( );
   }

   // Raise a raw event.
   {
      OutboundRawEventArgs args;
      args.line = e->line;
      args.line_id = e->count;
      args.line_hide_flags = e->hide_flags;
      OutboundRawEvent( this, %args );

      e->line = args.line;
      e->hide_flags = args.line_hide_flags;
   }

   // Parse.
   string line = CliUtil::CliBytesToMultiByteStr( e->line );
   if (line.length( ))
   {      
      using namespace yiffurc;

      m_session->c2s_state.line_count = e->count;
      const C2S* c2s = c2s_parse( line.c_str( ), line.length( ), m_session );
      if (c2s)
      {
         switch (c2s->type)
         {
         case C2S_TYPE_CHANGECOLOR:
            {
               C2S_CHANGECOLOR* parsed_c2s = (C2S_CHANGECOLOR*) c2s;
               OutboundChangeColorEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.colors = CliUtil::multiByteStrToCliBytes( string( parsed_c2s->colors ).append( parsed_c2s->gsd, parsed_c2s->gsd + 3 ) );
               OutboundChangeColorEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_CHANGEDESC:
            {
               C2S_CHANGEDESC* parsed_c2s = (C2S_CHANGEDESC*) c2s;
               OutboundChangeDescEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.desc = gcnew String( parsed_c2s->desc );
               OutboundChangeDescEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_SPEECH:
            {
               C2S_SPEECH* parsed_c2s = (C2S_SPEECH*) c2s;
               OutboundChatEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.body = gcnew String( parsed_c2s->message );
               OutboundChatEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_DECLINE:
            {
               C2S_DECLINE* parsed_c2s = (C2S_DECLINE*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundDeclineEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_DOWNLOADFINISHED:
            {
               C2S_DOWNLOADFINISHED* parsed_c2s = (C2S_DOWNLOADFINISHED*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundDreamReadyEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_EMOTE:
            {
               C2S_EMOTE* parsed_c2s = (C2S_EMOTE*) c2s;
               OutboundEmoteEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.body = gcnew String( parsed_c2s->message );
               OutboundEmoteEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_FDL:
            {
               C2S_FDL* parsed_c2s = (C2S_FDL*) c2s;
               OutboundFollowDreamLinkEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.furl = gcnew String( parsed_c2s->url );
               OutboundFollowDreamLinkEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_GOBACK:
            {
               C2S_GOBACK* parsed_c2s = (C2S_GOBACK*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundGoBackEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_GOMAP:
            {
               C2S_GOMAP* parsed_c2s = (C2S_GOMAP*) c2s;
               OutboundGoMapEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.map_id = parsed_c2s->map_num;
               OutboundGoMapEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_JOIN:
            {
               C2S_JOIN* parsed_c2s = (C2S_JOIN*) c2s;
               OutboundJoinEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               if (parsed_c2s->exact)
                  args.name = gcnew String( ("%" + string( parsed_c2s->player )).c_str( ) );
               else
                  args.name = gcnew String( parsed_c2s->player );
               OutboundJoinEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_LOGIN:
            {
               C2S_LOGIN* parsed_c2s = (C2S_LOGIN*) c2s;
               OutboundLoginEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.name     = gcnew String( parsed_c2s->player );
               args.password = gcnew String( parsed_c2s->password );
               args.mac_id   = gcnew String( parsed_c2s->mac_id );
               OutboundLoginEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_LOOKPOS:
            {
               C2S_LOOKPOS* parsed_c2s = (C2S_LOOKPOS*) c2s;
               OutboundLookPosEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.x = parsed_c2s->x;
               args.y = parsed_c2s->y;
               OutboundLookPosEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_MOVE:
            {
               C2S_MOVE* parsed_c2s = (C2S_MOVE*) c2s;
               OutboundMoveEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               switch (parsed_c2s->direction)
               {
               case MOVEDIR_NE:
                  args.move_dir = Move_Direction::NorthEast; break;
               case MOVEDIR_NW:
                  args.move_dir = Move_Direction::NorthWest; break;
               case MOVEDIR_SE:
                  args.move_dir = Move_Direction::SouthEast; break;
               case MOVEDIR_SW:
               default:
                  args.move_dir = Move_Direction::SouthWest; break;
               }
               OutboundMoveEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_ONLINECHECK:
            {
               C2S_ONLINECHECK* parsed_c2s = (C2S_ONLINECHECK*) c2s;
               OutboundOnlineCheckEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.name = gcnew String( parsed_c2s->player );
               OutboundOnlineCheckEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_SETCOLOR:
            {
               C2S_SETCOLOR* parsed_c2s = (C2S_SETCOLOR*) c2s;
               OutboundSetColorEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.colors = CliUtil::multiByteStrToCliBytes( string( parsed_c2s->colors ).append( parsed_c2s->gsd, parsed_c2s->gsd + 3 ) );
               OutboundSetColorEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_SETDESC:
            {
               C2S_SETDESC* parsed_c2s = (C2S_SETDESC*) c2s;
               OutboundSetDescEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.desc = gcnew String( parsed_c2s->desc );
               OutboundSetDescEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_SHOUT:
            {
               C2S_SHOUT* parsed_c2s = (C2S_SHOUT*) c2s;
               OutboundShoutEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               args.body = gcnew String( parsed_c2s->message );
               OutboundShoutEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_SHOUTTOGGLE:
            {
               C2S_SHOUTTOGGLE* parsed_c2s = (C2S_SHOUTTOGGLE*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundShoutToggleEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_SITDOWN:
            {
               C2S_SITDOWN* parsed_c2s = (C2S_SITDOWN*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundSitEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_SITSTANDLIE:
            {
               C2S_SITSTANDLIE* parsed_c2s = (C2S_SITSTANDLIE*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundSitStandLayEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_STANDUP:
            {
               C2S_STANDUP* parsed_c2s = (C2S_STANDUP*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundStandEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_SUMMON:
            {
               C2S_SUMMON* parsed_c2s = (C2S_SUMMON*) c2s;
               OutboundSummonEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               if (parsed_c2s->exact)
                  args.name = gcnew String( ("%" + string( parsed_c2s->player )).c_str( ) );
               else
                  args.name = gcnew String( parsed_c2s->player );
               OutboundSummonEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
         case C2S_TYPE_TURNLEFT:
            {
               C2S_TURNLEFT* parsed_c2s = (C2S_TURNLEFT*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundTurnLeftEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_TURNRIGHT:
            {
               C2S_TURNRIGHT* parsed_c2s = (C2S_TURNRIGHT*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundTurnRightEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_USE:
            {
               C2S_USE* parsed_c2s = (C2S_USE*) c2s;
               OutboundRawEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               OutboundUseItemEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         case C2S_TYPE_WHISPER:
            {
               C2S_WHISPER* parsed_c2s = (C2S_WHISPER*) c2s;
               OutboundWhisperEventArgs args;
               args.line            = e->line;
               args.line_id         = e->count;
               args.line_hide_flags = e->hide_flags;
               if (parsed_c2s->exact)
                  args.name = gcnew String( ("%" + string( parsed_c2s->player )).c_str( ) );
               else
                  args.name = gcnew String( parsed_c2s->player );
               args.body = gcnew String( parsed_c2s->message );
               OutboundWhisperEvent( this, %args );

               e->line = args.line;
               e->hide_flags = args.line_hide_flags;
            }
            break;
         } // switch (c2s->type)
         yiffurc_release( c2s );
      } // if (c2s)
   } // if (line.length( ))
}

array<Object^>^ NetSubsystem::scan( String^ pattern, array<Byte>^ line )
{
   if (line != nullptr && line->Length > 0)
   {
      list<net_scan::FIELD> fields;
      if (net_scan::scan( CliUtil::cliStrToMultiByteStr( pattern ), 
                          CliUtil::CliBytesToMultiByteStr( line ), fields ))
      {
         array<Object^>^ r = gcnew array<Object^>( fields.size( ) );
         UInt32 j = 0;
         for (list<net_scan::FIELD>::const_iterator i = fields.begin( ); i != fields.end( ); ++i, ++j)
         {
            switch (i->type)
            {
            default:
            case net_scan::FIELD::FIELD_INT:
               r[ j ] = gcnew UInt32( (unsigned) i->value_int ); break;
            case net_scan::FIELD::FIELD_STR:
               r[ j ] = gcnew String( i->value_str.c_str( ) ); break;
            case net_scan::FIELD::FIELD_CHAR:
               r[ j ] = CliUtil::charsToCliBytes( i->value_str.c_str( ), i->value_str.length( ) ); break;
            }
         }
         return r;
      }
   }
   return nullptr;
}

UInt32 NetSubsystem::send( array<Byte>^ line )
{
   vector<char> l = CliUtil::cliBytesToChars( line );
   UInt32 id = furn2::f2_netoutline( &l.front( ), l.size( ) );
   m_sends->AddLast( id );
   return id;
}

} // namespace Furnarchy