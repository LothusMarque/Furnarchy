#ifndef _NETSUBSYSTEM_H__
#define _NETSUBSYSTEM_H__

#include "yiffurc/yiffurc.h"

namespace Furnarchy
{
   /// <summary> Various connection states the client can be in. </summary>
   public enum class Connection_Level
   {
      Disconnected,
      Connected,
      LoggedIn
   };
   /// <summary> Network hide flags. </summary>
   /// <remarks>
   ///   Network lines processed during the
   ///   <see cref='NetSubsystem::InboundRawEvent' /> or <see cref='NetSubsystem::OutboundRawEvent' /> events
   ///   may be hidden from subsequent modules or subsystems by setting these flags.
   /// </remarks>
   [Flags]
   public enum class Hide_Flags
   {
      None    = 0x0,
      Filters = 0x1,
      Scripts = 0x2,
      Modules = 0x4,
      World   = 0x8,
      Client  = 0x10,
      All     = 0x1F,
   };
   /// <summary> Describes an object and its map position. </summary>
   public value class ObjectSet
   {
   public:
      UInt32 obj_id;
      UInt32 x;
      UInt32 y;
   };
   /// <summary> Describes a floor tile and its map position. </summary>
   public value class FloorSet
   {
   public:
      UInt32 floor_id;
      UInt32 x;
      UInt32 y;
   };
   /// <summary> Describes a range of DS variables to set to a value. </summary>
   public value class DsVarSet
   {
   public:
      UInt32 var_first;
      UInt32 var_count;
      UInt32 var_value;
   };
   /// <summary> Describes a triggered DS line and two parameters. </summary>
   public value class DsTriggerLine
   {
   public:
      UInt32 line;
      UInt32 param1;
      UInt32 param2;
   };

   //////////////////////////////////////////////////////////////////////////

   /// <summary> For use with <see cref='NetSubsystem::onInbound' /> </summary>.
   private ref class InboundEventArgs : EventArgs
   {
   public:
      array<Byte>^ line;
      UInt32       count;
      Hide_Flags   hide_flags;
   };
   /// <summary> For use with <see cref='NetSubsystem::onOutbound' /> </summary>.
   private ref class OutboundEventArgs : EventArgs
   {
   public:
      array<Byte>^ line;
      UInt32       count;
      Hide_Flags   hide_flags;
   };

   /// <summary> Arguments for <see cref='InboundRawDelegate' />. </summary>
   public ref class InboundRawEventArgs : EventArgs
   {
   public:
      /// <summary> The line in its raw form. </summary>
      /// <remarks>
      ///   Modifying this line will, in turn, modify the line for subsequent modules or subsystems.
      /// </remarks>
      array<Byte>^ line;
      /// <summary> An increasing number that uniquely identifies this inbound line. </summary>
      UInt32       line_id;
      /// <summary> Flags that may be set to hide the line from subsequent modules or subsystems. </summary>
      /// <remarks>
      ///   This is any combination of the <see cref='Hide_Flags' /> enumeration.
      /// </remarks>
      Hide_Flags   line_hide_flags;
   };
   /// <summary> Arguments for <see cref='InboundAvatarMoveDelegate' />. </summary>
   public ref class InboundAvatarMoveEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 guid;
      UInt32 x;
      UInt32 y;
      UInt32 frame;
   };
   /// <summary> Arguments for <see cref='InboundAvatarCreateDelegate' />. </summary>
   public ref class InboundAvatarCreateEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 guid;
      UInt32 x;
      UInt32 y;
      UInt32 frame;
      String^ name;
      array<Byte>^ colors;
      Monster_Flags flags;
   };
   /// <summary> Arguments for <see cref='InboundAvatarDestroyDelegate' />. </summary>
   public ref class InboundAvatarDestroyEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 guid;
   };
   /// <summary> Arguments for <see cref='InboundAvatarHideDelegate' />. </summary>
   public ref class InboundAvatarHideEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 guid;
      UInt32 x;
      UInt32 y;
   };
   /// <summary> Arguments for <see cref='InboundAvatarShowDelegate' />. </summary>
   public ref class InboundAvatarShowEventArgs : InboundRawEventArgs 
   {
   public:
      UInt32 guid; 
      UInt32 x;
      UInt32 y;
      UInt32 frame;
   };
   /// <summary> Arguments for <see cref='InboundAvatarShow2Delegate' />. </summary>
   public ref class InboundAvatarShow2EventArgs : InboundRawEventArgs 
   {
   public:
      UInt32 guid; 
      UInt32 x;
      UInt32 y;
      UInt32 frame;
   };
   /// <summary> Arguments for <see cref='InboundAvatarUpdateDelegate' />. </summary>
   public ref class InboundAvatarUpdateEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 guid; 
      UInt32 frame; 
      array<Byte>^ colors;
   };
   /// <summary> Arguments for <see cref='InboundCameraMoveDelegate' />. </summary>
   public ref class InboundCameraMoveEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 to_x;
      UInt32 to_y;
      UInt32 from_x;
      UInt32 from_y;
   };
   /// <summary> Arguments for <see cref='InboundCameraSetDelegate' />. </summary>
   public ref class InboundCameraSetEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 x;
      UInt32 y;
   };
   /// <summary> Arguments for <see cref='InboundChatDelegate' />. </summary>
   public ref class InboundChatEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundChatBadgeDelegate' />. </summary>
   public ref class InboundChatBadgeEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 badge_id;
   };
   /// <summary> Arguments for <see cref='InboundChatEchoDelegate' />. </summary>
   public ref class InboundChatEchoEventArgs : InboundRawEventArgs
   {
   public:
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundChatSpeciTagDelegate' />. </summary>
   public ref class InboundChatSpeciTagEventArgs : InboundRawEventArgs
   {
   public:
      array<Byte>^ colors;
   };
   /// <summary> Arguments for <see cref='InboundDeclinedDelegate' />. </summary>
   public ref class InboundDeclinedEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundDeclineEchoDelegate' />. </summary>
   public ref class InboundDeclineEchoEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundDescBodyDelegate' />. </summary>
   public ref class InboundDescBodyEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundDescHeadDelegate' />. </summary>
   public ref class InboundDescHeadEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundDreamPadCreateDelegate' />. </summary>
   public ref class InboundDreamPadCreateEventArgs : InboundRawEventArgs
   {
   public:
      String^ name;
      String^ caption;
      UInt32 x;
      UInt32 y;
   };
   /// <summary> Arguments for <see cref='InboundDreamPadDestroyDelegate' />. </summary>
   public ref class InboundDreamPadDestroyEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 x;
      UInt32 y;
   };
   /// <summary> Arguments for <see cref='InboundDsEmitDelegate' />. </summary>
   public ref class InboundDsEmitEventArgs : InboundRawEventArgs
   {
   public:
      String^ body;
   };  
   /// <summary> Arguments for <see cref='InboundDsTriggerDelegate' />. </summary>
   public ref class InboundDsTriggerEventArgs : InboundRawEventArgs 
   {
   public:
      UInt32 x1;
      UInt32 y1;
      UInt32 x2;
      UInt32 y2;
      array<DsTriggerLine>^ triggers;
   };
   /// <summary> Arguments for <see cref='InboundDsTriggerSelfDelegate' />. </summary>
   public ref class InboundDsTriggerSelfEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 x1;
      UInt32 y1;
      UInt32 x2;
      UInt32 y2;
      array<DsTriggerLine>^ triggers;
   };
   /// <summary> Arguments for <see cref='InboundDsVarSetDelegate' />. </summary>
   public ref class InboundDsVarSetEventArgs : InboundRawEventArgs
   {
   public:
      array<DsVarSet>^ vars;
   };
   /// <summary> Arguments for <see cref='InboundEmitDelegate' />. </summary>
   public ref class InboundEmitEventArgs : InboundRawEventArgs
   {
   public:
      String^ body;
   };            
   /// <summary> Arguments for <see cref='InboundEmoteDelegate' />. </summary>
   public ref class InboundEmoteEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundFeetObjectSetDelegate' />. </summary>
   public ref class InboundFeetObjectSetEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 obj;
   };
   /// <summary> Arguments for <see cref='InboundFetchDreamDelegate' />. </summary>
   public ref class InboundFetchDreamEventArgs : InboundRawEventArgs
   {
   public:
      String^ name;
      UInt32 id;
   };
   /// <summary> Arguments for <see cref='InboundFetchPortraitDelegate' />. </summary>
   public ref class InboundFetchPortraitEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 pid;
   };
   /// <summary> Arguments for <see cref='InboundFloorSetDelegate' />. </summary>
   public ref class InboundFloorSetEventArgs : InboundRawEventArgs
   {
   public:
      array<FloorSet>^ floors;
   };
   /// <summary> Arguments for <see cref='InboundInventoryObjectSetDelegate' />. </summary>
   public ref class InboundInventoryObjectSetEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 obj;
   };
   /// <summary> Arguments for <see cref='InboundJoinedDelegate' />. </summary>
   public ref class InboundJoinedEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundJoinRequestDelegate' />. </summary>
   public ref class InboundJoinRequestEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundLoginFailDelegate' />. </summary>
   public ref class InboundLoginFailEventArgs : InboundRawEventArgs
   {
   public:
      String^ reason;
   };
   /// <summary> Arguments for <see cref='InboundNameLookupFailDelegate' />. </summary>
   public ref class InboundNameLookupFailEventArgs : InboundRawEventArgs
   {
   public:
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundObjectSetDelegate' />. </summary>
   public ref class InboundObjectSetEventArgs : InboundRawEventArgs
   {
   public:
      array<ObjectSet>^ objects;
   };
   /// <summary> Arguments for <see cref='InboundOnlineCheckDelegate' />. </summary>
   public ref class InboundOnlineCheckEventArgs : InboundRawEventArgs
   {
   public:
      String^ name;
      Boolean online;
   };
   /// <summary> Arguments for <see cref='InboundPlaySongDelegate' />. </summary>
   public ref class InboundPlaySongEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 song_id;
   };
   /// <summary> Arguments for <see cref='InboundPlaySoundDelegate' />. </summary>
   public ref class InboundPlaySoundEventArgs : InboundRawEventArgs
   {
   public:
      UInt32 sound_id;
   };
   /// <summary> Arguments for <see cref='InboundPortraitSetDelegate' />. </summary>
   public ref class InboundPortraitSetEventArgs : InboundRawEventArgs
   {
   public:
      String^ name;
      array<Byte>^ colors;
   };
   /// <summary> Arguments for <see cref='InboundRawChatDelegate' />. </summary>
   public ref class InboundRawChatEventArgs : InboundRawEventArgs
   {
   public:
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundRegisterFailDelegate' />. </summary>
   public ref class InboundRegisterFailEventArgs : InboundRawEventArgs
   {
   public:
      String^ reason;
   };
   /// <summary> Arguments for <see cref='InboundRequestEchoDelegate' />. </summary>
   public ref class InboundRequestEchoEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundRollDelegate' />. </summary>
   public ref class InboundRollEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
      UInt32 num_dice;
      UInt32 num_sides;
      UInt32 sum;
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundShoutDelegate' />. </summary>
   public ref class InboundShoutEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundShoutEchoDelegate' />. </summary>
   public ref class InboundShoutEchoEventArgs : InboundRawEventArgs
   {
   public:
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundSummonedDelegate' />. </summary>
   public ref class InboundSummonedEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundSummonRequestDelegate' />. </summary>
   public ref class InboundSummonRequestEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
      String^ destination;
   };
   /// <summary> Arguments for <see cref='InboundSyncColorCodeDelegate' />. </summary>
   public ref class InboundSyncColorCodeEventArgs : InboundRawEventArgs
   {
   public:
      array<Byte>^ colors;
   };
   /// <summary> Arguments for <see cref='InboundUseBgFileDelegate' />. </summary>
   public ref class InboundUseBgFileEventArgs : InboundRawEventArgs
   {
   public:
      String^ filename;
   };
   /// <summary> Arguments for <see cref='InboundWhisperDelegate' />. </summary>
   public ref class InboundWhisperEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
      String^ body;
   };
   /// <summary> Arguments for <see cref='InboundWhisperDeniedDelegate' />. </summary>
   public ref class InboundWhisperDeniedEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
   };
   /// <summary> Arguments for <see cref='InboundWhisperEchoDelegate' />. </summary>
   public ref class InboundWhisperEchoEventArgs : InboundRawEventArgs
   {
   public:
      String^ short_name;
      String^ name;
      String^ body;
   };

   /// <summary> Arguments for <see cref='OutboundRawDelegate' />. </summary>
   public ref class OutboundRawEventArgs : EventArgs
   {
   public:
      /// <summary> The line in its raw form. </summary>
      /// <remarks>
      ///   Modifying this line will, in turn, modify the line for subsequent modules or subsystems.
      /// </remarks>
      array<Byte>^ line;
      /// <summary> An increasing number that uniquely identifies this outbound line. </summary>
      UInt32       line_id;
      /// <summary> Flags that may be set to hide the line from subsequent modules or subsystems. </summary>
      /// <remarks>
      ///   This is any combination of the <see cref='Hide_Flags' /> enumeration.
      /// </remarks>
      Hide_Flags   line_hide_flags;
   };
   /// <summary> Arguments for <see cref='OutboundChangeColorDelegate' />. </summary>
   public ref class OutboundChangeColorEventArgs : OutboundRawEventArgs
   {
   public:
      array<Byte>^ colors;
   };
   /// <summary> Arguments for <see cref='OutboundChangeDescDelegate' />. </summary>
   public ref class OutboundChangeDescEventArgs : OutboundRawEventArgs
   {
   public:
      String^ desc;
   };
   /// <summary> Arguments for <see cref='OutboundChatDelegate' />. </summary>
   public ref class OutboundChatEventArgs : OutboundRawEventArgs
   {
   public:
      String^ body;
   };
   /// <summary> Arguments for <see cref='OutboundEmoteDelegate' />. </summary>
   public ref class OutboundEmoteEventArgs : OutboundRawEventArgs
   {
   public:
      String^ body;
   };
   /// <summary> Arguments for <see cref='OutboundFollowDreamLinkDelegate' />. </summary>
   public ref class OutboundFollowDreamLinkEventArgs : OutboundRawEventArgs
   {
   public:
      String^ furl;
   };
   /// <summary> Arguments for <see cref='OutboundGoMapDelegate' />. </summary>
   public ref class OutboundGoMapEventArgs : OutboundRawEventArgs
   {
   public:
      UInt32 map_id;
   };
   /// <summary> Arguments for <see cref='OutboundJoinDelegate' />. </summary>
   public ref class OutboundJoinEventArgs : OutboundRawEventArgs
   {
   public:
      String^ name;
   };
   /// <summary> Arguments for <see cref='OutboundLoginDelegate' />. </summary>
   public ref class OutboundLoginEventArgs : OutboundRawEventArgs
   {
   public:
      String^ name;
      String^ password;
      String^ mac_id;
   };
   /// <summary> Arguments for <see cref='OutboundLookPosDelegate' />. </summary>
   public ref class OutboundLookPosEventArgs: OutboundRawEventArgs
   {
   public:
      UInt32 x, y;
   };
   /// <summary> Arguments for <see cref='OutboundMoveDelegate' />. </summary>
   public ref class OutboundMoveEventArgs : OutboundRawEventArgs
   {
   public:
      Move_Direction move_dir;
   };
   /// <summary> Arguments for <see cref='OutboundOnlineCheckDelegate' />. </summary>
   public ref class OutboundOnlineCheckEventArgs : OutboundRawEventArgs
   {
   public:
      String^ name;
   };
   /// <summary> Arguments for <see cref='OutboundSetColorDelegate' />. </summary>
   public ref class OutboundSetColorEventArgs : OutboundRawEventArgs
   {
   public:
      array<Byte>^ colors;
   }; 
   /// <summary> Arguments for <see cref='OutboundSetDescDelegate' />. </summary>
   public ref class OutboundSetDescEventArgs : OutboundRawEventArgs
   {
   public:
      String^ desc;
   };
   /// <summary> Arguments for <see cref='OutboundShoutDelegate' />. </summary>
   public ref class OutboundShoutEventArgs : OutboundRawEventArgs
   {
   public:
      String^ body;
   };
   /// <summary> Arguments for <see cref='OutboundSummonDelegate' />. </summary>
   public ref class OutboundSummonEventArgs : OutboundRawEventArgs
   {
   public:
      String^ name;
   };
   /// <summary> Arguments for <see cref='OutboundWhisperDelegate' />. </summary>
   public ref class OutboundWhisperEventArgs : OutboundRawEventArgs
   {
   public:
      String^ name;
      String^ body;
   };

   //////////////////////////////////////////////////////////////////////////
   
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundAvatarMoveDelegate( Object^ sender, InboundAvatarMoveEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundAvatarCreateDelegate( Object^ sender, InboundAvatarCreateEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundAvatarDestroyDelegate( Object^ sender, InboundAvatarDestroyEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundAvatarHideDelegate( Object^ sender, InboundAvatarHideEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundAvatarShowDelegate( Object^ sender, InboundAvatarShowEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundAvatarShow2Delegate( Object^ sender, InboundAvatarShow2EventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundAvatarUpdateDelegate( Object^ sender, InboundAvatarUpdateEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundCameraMoveDelegate( Object^ sender, InboundCameraMoveEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundCameraSetDelegate( Object^ sender, InboundCameraSetEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundChatDelegate( Object^ sender, InboundChatEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundChatBadgeDelegate( Object^ sender, InboundChatBadgeEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundChatEchoDelegate( Object^ sender, InboundChatEchoEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundChatSpeciTagDelegate( Object^ sender, InboundChatSpeciTagEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDeclinedDelegate( Object^ sender, InboundDeclinedEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDeclineEchoDelegate( Object^ sender, InboundDeclineEchoEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDeclineFailDelegate( Object^ sender, InboundRawEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDescBodyDelegate( Object^ sender, InboundDescBodyEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDescHeadDelegate( Object^ sender, InboundDescHeadEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDragonRoarDelegate( Object^ sender, InboundRawEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDreamPadCreateDelegate( Object^ sender, InboundDreamPadCreateEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDreamPadDestroyDelegate( Object^ sender, InboundDreamPadDestroyEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDsEmitDelegate( Object^ sender, InboundDsEmitEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDsTriggerDelegate( Object^ sender, InboundDsTriggerEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDsTriggerSelfDelegate( Object^ sender, InboundDsTriggerSelfEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundDsVarSetDelegate( Object^ sender, InboundDsVarSetEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundEmitDelegate( Object^ sender, InboundEmitEventArgs^ e );            
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundEmoteDelegate( Object^ sender, InboundEmoteEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundFeetObjectSetDelegate( Object^ sender, InboundFeetObjectSetEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundFetchDreamDelegate( Object^ sender, InboundFetchDreamEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundFetchPortraitDelegate( Object^ sender, InboundFetchPortraitEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundFloorSetDelegate( Object^ sender, InboundFloorSetEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundInventoryObjectSetDelegate( Object^ sender, InboundInventoryObjectSetEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundJoinedDelegate( Object^ sender, InboundJoinedEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundJoinRequestDelegate( Object^ sender, InboundJoinRequestEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundLoginFailDelegate( Object^ sender, InboundLoginFailEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundNameLookupAmbiguousDelegate( Object^ sender, InboundRawEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundNameLookupFailDelegate( Object^ sender, InboundNameLookupFailEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundObjectSetDelegate( Object^ sender, InboundObjectSetEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundOnlineCheckDelegate( Object^ sender, InboundOnlineCheckEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundPlaySongDelegate( Object^ sender, InboundPlaySongEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundPlaySoundDelegate( Object^ sender, InboundPlaySoundEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundPortraitSetDelegate( Object^ sender, InboundPortraitSetEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundRawChatDelegate( Object^ sender, InboundRawChatEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundRawDelegate( Object^ sender, InboundRawEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundRegisterFailDelegate( Object^ sender, InboundRegisterFailEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundRequestEchoDelegate( Object^ sender, InboundRequestEchoEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundRollDelegate( Object^ sender, InboundRollEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundShoutDelegate( Object^ sender, InboundShoutEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundShoutEchoDelegate( Object^ sender, InboundShoutEchoEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundSuccessDelegate( Object^ sender, InboundRawEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundSummonedDelegate( Object^ sender, InboundSummonedEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundSummonRequestDelegate( Object^ sender, InboundSummonRequestEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundSuppressRenderingDelegate( Object^ sender, InboundRawEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundSyncColorCodeDelegate( Object^ sender, InboundSyncColorCodeEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundUseBgFileDelegate( Object^ sender, InboundUseBgFileEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundUnsuppressRenderingDelegate( Object^ sender, InboundRawEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundWhisperDelegate( Object^ sender, InboundWhisperEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundWhisperDeniedDelegate( Object^ sender, InboundWhisperDeniedEventArgs^ e );
   /// <summary> A parsed inbound network event delegate. See <see cref='NetSubsystem::InboundRawEvent' />. </summary>
   public delegate void InboundWhisperEchoDelegate( Object^ sender, InboundWhisperEchoEventArgs^ e );

   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundChangeColorDelegate( Object^ sender, OutboundChangeColorEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundChangeDescDelegate( Object^ sender, OutboundChangeDescEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundChatDelegate( Object^ sender, OutboundChatEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundDeclineDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundDreamReadyDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundEmoteDelegate( Object^ sender, OutboundEmoteEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundFollowDreamLinkDelegate( Object^ sender, OutboundFollowDreamLinkEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundGoBackDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundGoMapDelegate( Object^ sender, OutboundGoMapEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundJoinDelegate( Object^ sender, OutboundJoinEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundLayDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundLoginDelegate( Object^ sender, OutboundLoginEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundLookPosDelegate( Object^ sender, OutboundLookPosEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundMoveDelegate( Object^ sender, OutboundMoveEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundOnlineCheckDelegate( Object^ sender, OutboundOnlineCheckEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundPickUpDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundPortraitChangeDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundQuitDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundRawDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundSetColorDelegate( Object^ sender, OutboundSetColorEventArgs^ e ); 
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundSetDescDelegate( Object^ sender, OutboundSetDescEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundShoutDelegate( Object^ sender, OutboundShoutEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundShoutToggleDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundSitDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundSitStandLayDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundStandDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundSummonDelegate( Object^ sender, OutboundSummonEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundTurnLeftDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundTurnRightDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundUseItemDelegate( Object^ sender, OutboundRawEventArgs^ e );
   /// <summary> A parsed outbound network event delegate. See <see cref='NetSubsystem::OutboundRawEvent' />. </summary>
   public delegate void OutboundWhisperDelegate( Object^ sender, OutboundWhisperEventArgs^ e );

   /// <summary> A network event delegate. </summary>
   /// <seealso cref='NetSubsystem::ConnectedEvent' />
   public delegate void ConnectedDelegate( Object^ sender, EventArgs^ e );
   /// <summary> A network event delegate. </summary>
   /// <seealso cref='NetSubsystem::DisconnectedEvent' />
   public delegate void DisconnectedDelegate( Object^ sender, EventArgs^ e );
   /// <summary> A network event delegate. </summary>
   /// <seealso cref='NetSubsystem::LoggedInEvent' />
   public delegate void LoggedInDelegate( Object^ sender, EventArgs^ e );

   //////////////////////////////////////////////////////////////////////////
   
   /// <summary> Encapsulates network operations and sDelegate. </summary>
   public ref class NetSubsystem sealed
   {
   private:
      NetSubsystem( ) { }
   internal:
      NetSubsystem( ModuleHost^ host );
      ~NetSubsystem( );

      void onConnected( );
      void onLoggedIn( );
      void onDisconnected( );
      void onInbound( InboundEventArgs^ e );
      void onOutbound( OutboundEventArgs^ e );

   public:

      /// <summary> Generate an avatar's frame number. </summary>
      /// <param name='av'> The type of avatar (see <see cref='Avatar_Class' />). </param>
      /// <param name='pose'> The avatar's pose (see <see cref='Avatar_Pose' />). </param>
      /// <param name='direction'> The direction the avatar is facing. </param>
      /// <remarks>
      ///   Whenever an avatar is spawned, moved, or updated, the server
      ///   sends a frame number (or two!) which maps to an FSH shape for that avatar.
      /// </remarks>
      /// <returns> The frame number. </returns>
      UInt32 encodeAvatarFrameNumber( Avatar_Class av, 
                                      Avatar_Pose pose,
                                      Move_Direction direction );
      /// <summary> Decode an avatar's frame number. </summary>
      /// <param name='frame'> The avatar frame number. </param>
      /// <param name='av'> (out) The type of avatar (see <see cref='Avatar_Class' />). </param>
      /// <param name='pose'> (out) The avatar's pose (see <see cref='Avatar_Pose' />). </param>
      /// <param name='direction'> (out) The direction the avatar is facing. </param>
      /// <remarks>
      ///   Whenever an avatar is spawned, moved, or updated, the server
      ///   sends a frame number (or two!) which maps to an FSH shape for that avatar.
      /// </remarks>
      /// <returns> true if the frame number was successfully decoded. </returns>
      Boolean   decodeAvatarFrameNumber( UInt32 frame, 
                                         [Out] Avatar_Class% av, 
                                         [Out] Avatar_Pose% pose, 
                                         [Out] Move_Direction% direction );
      /// <summary> Encode an integer into base-220. </summary>
      /// <param name='n'> The integer. </param>
      /// <param name='len'> How many digits wide the base-220 number should be. </param>
      /// <remarks>
      ///   Many of the newer sever-to-client messages now use base-220 to encode
      ///   integers, rather than the old base-95.
      /// </remarks>
      /// <returns> The base-220 number as a byte array. </returns>
      array<Byte>^ encodeBase220Number( UInt32 n, UInt32 len );
      /// <summary> Decode a base-220 number into an integer. </summary>
      /// <param name='base220'> The base-220 number string. </param>
      /// <remarks>
      ///   Many of the newer sever-to-client messages now use base-220 to encode
      ///   integers, rather than the old base-95.
      /// </remarks>
      /// <returns> The integer value of the base-220 number. </returns>
      UInt32  decodeBase220Number( array<Byte>^ base220 );
      /// <summary> Decode a base-220 number into an integer. </summary>
      /// <param name='base220'> The base-220 number string. </param>
      /// <param name='offset'> Zero-based index of the start of the base-220 number within the array. </param>
      /// <param name='len'> Length of the base-220 number within the array. </param>
      /// <remarks>
      ///   Many of the newer sever-to-client messages now use base-220 to encode
      ///   integers, rather than the old base-95.
      /// </remarks>
      /// <returns> The integer value of the base-220 number. </returns>
      UInt32  decodeBase220Number( array<Byte>^ base220, UInt32 offset, UInt32 len );
      /// <summary> Encode an integer into base-95. </summary>
      /// <param name='n'> The integer. </param>
      /// <param name='len'> How many digits wide the base-95 number should be. </param>
      /// <remarks>
      ///   Many of the older sever-to-client messages use base-95 to encode
      ///   integers.
      /// </remarks>
      /// <returns> The base-95 number as a byte array. </returns>
      array<Byte>^ encodeBase95Number( UInt32 n, UInt32 len );
      /// <summary> Decode a base-95 number into an integer. </summary>
      /// <param name='base95'> The base-95 number string. </param>
      /// <remarks>
      ///   Many of the older sever-to-client messages use base-95 to encode
      ///   integers.
      /// </remarks>
      /// <returns> The integer value of the base-95 number. </returns>
      UInt32  decodeBase95Number( array<Byte>^ base95 );
      /// <summary> Decode a base-95 number into an integer. </summary>
      /// <param name='base95'> The base-95 number string. </param>
      /// <param name='offset'> Zero-based index of the start of the base-220 number within the array. </param>
      /// <param name='len'> Length of the base-220 number within the array. </param>
      /// <remarks>
      ///   Many of the older sever-to-client messages use base-95 to encode
      ///   integers.
      /// </remarks>
      /// <returns> The integer value of the base-95 number. </returns>
      UInt32  decodeBase95Number( array<Byte>^ base95, UInt32 offset, UInt32 len );
      /// <summary> Match and extract information from a network string. </summary>
      /// <param name='pattern'> String defining format and data types to extract. </param>
      /// <param name='line'> A network line. </param>
      /// <remarks>
      ///   <para>
      ///      This is similar to the scanf family of functions in C.
      ///      <c>pattern</c> will contain a mix of literals and format specifiers.
      ///      If <c>line</c> matches the format defined by <c>pattern</c>,
      ///      an array of boxed Objects are returned containing all the scanned elements
      ///      in order from left-to-right.
      ///   </para>
      ///   <para>
      ///      Format specifiers have the structure <c>%[length]type</c>, where
      ///      <c>length</c> is the number of symbols (or digits) of that type to extract
      ///      and <c>type</c> is the type of information to extract.  The <c>length</c>
      ///      specifier may be omitted in some cases, such as with byte strings and
      ///      colors.
      ///   </para>
      ///   <list type='bullet'>
      ///      <listheader>
      ///         <term>Available format specifiers.</term>
      ///      </listheader>
      ///      <item>
      ///         <term><c>%c</c></term>
      ///         <description>
      ///            Any one (8-bit) character. May be augmented with a length 
      ///            such as <c>%3c</c> to read in 3 any characters or <c>%*c</c> to read in 
      ///            all characters that follow. Returns as a byte array. 
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%s</c></term>
      ///         <description>
      ///            A furcadia byte-string (A base-220 digit followed by a string). 
      ///            Returns as a string.
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%k</c></term>
      ///         <description>
      ///            An old-style furcadia color-code (10-characters long, not including the 
      ///            3 trailing GSD bytes). Returns as a byte array. 
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%K</c></term>
      ///         <description>
      ///            A new-style furcadia color-code (variable-length, not including the 3 
      ///            trailing GSD bytes). 
      ///            Returns as a byte array.
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%n</c></term>
      ///         <description>
      ///            A single-digit base-95 number. May be augmented with a length 
      ///            such as <c>%2n</c> to read in a 2-digit base-95 number. Returns as an integer. 
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%N</c></term>
      ///         <description>
      ///            A single-digit base-220 number. May be augmented with a 
      ///            length such as <c>%2N</c> to read in a 2-digit base-220 number. Returns as an integer. 
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%%</c></term>
      ///         <description>
      ///            A literal <c>%</c> character. Isn't captured 
      ///            so it doesn't count towards the overall length of the returned array. 
      ///         </description>
      ///      </item>
      ///   </list>
      /// </remarks>
      /// <example>Extracting name and color data from a portrait string (C#).
      ///    <code>
      ///      using System;
      ///      
      ///      public class MyModule : Furnarchy.IFurnarchyModule
      ///      {
      ///        private readonly Furnarchy.ModuleHost m_host;
      ///      
      ///        public MyModule( Furnarchy.ModuleHost host )
      ///        {
      ///            m_host = host;
      ///            m_host.Net.InboundRawEvent += new Furnarchy.InboundRawDelegate( onInboundRaw );
      ///        }
      ///            
      ///        void onInboundRaw( Object sender, Furnarchy.InboundRawEventArgs e )
      ///        {
      ///            // Match the characters "]f", then capture a color-code string, 
      ///            // 3 more characters (the gender, species and digo bytes),
      ///            // and all remaining characters (the name).
      ///            object[] scanned = m_host.Net.scan( "]f%K%3c%*c", e.line );
      ///            if (scanned != null)
      ///            {
      ///                // It was a portrait string!
      ///                m_host.speak( String.Format( "Player {2}'s full color code is {0}{1}.",
      ///                                             System.Text.Encoding.ASCII.GetString( (byte[]) scanned[0] ), 
      ///                                             System.Text.Encoding.ASCII.GetString( (byte[]) scanned[1] ),
      ///                                             System.Text.Encoding.ASCII.GetString( (byte[]) scanned[2] ) ) );
      ///            }
      ///         }
      ///      }
      ///    </code>
      /// </example>
      /// <returns> An Object array containing boxed versions of each element scanned. </returns>
      array<Object^>^ scan( String^ pattern, array<Byte>^ line );
      /// <summary> Formats a network string. </summary>
      /// <param name='pattern'> String defining format and data types to encode. </param>
      /// <param name='fields'> Data to be encoded (passed in the same order as defined by <c>pattern</c>). </param>
      /// <remarks>
      ///   <para>
      ///      This is similar to the sprintf family of functions in C.
      ///      <c>pattern</c> will contain a mix of literals and format specifiers.
      ///      The returned string will interpolate <c>fields</c> over <c>pattern</c>
      ///      using format specifiers to determine how to encode each field.
      ///      The returned byte array is suitable for passing into <see cref='inject' /> or <see cref='send' />.
      ///   </para>
      ///   <para>
      ///      Format specifiers have the structure <c>%[length]type</c>, where
      ///      <c>length</c> is the number of symbols (or digits) of that type to extract
      ///      and <c>type</c> is the type of information to extract.  The <c>length</c>
      ///      specifier may be omitted in some cases, such as when strings or byte arrays are
      ///      expected.
      ///      Note that many of the format specifiers are not completely analagous to
      ///      those used by <see cref='scan' />.
      ///   </para>
      ///   <list type='bullet'>
      ///      <listheader>
      ///         <term>Available format specifiers.</term>
      ///      </listheader>
      ///      <item>
      ///         <term><c>%c</c></term>
      ///         <description>
      ///            One or more (8-bit) characters. Expects a byte array or string.
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%s</c></term>
      ///         <description>
      ///            A furcadia byte-string (A base-220 digit followed by a string). 
      ///            Expects a string.
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%n</c></term>
      ///         <description>
      ///            A single-digit base-95 number. May be augmented with a length 
      ///            such as <c>%2n</c> to read in a 2-digit base-95 number. Expects an integer. 
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%N</c></term>
      ///         <description>
      ///            A single-digit base-220 number. May be augmented with a 
      ///            length such as <c>%2N</c> to read in a 2-digit base-220 number. Expects an integer. 
      ///         </description>
      ///      </item>
      ///      <item>
      ///         <term><c>%%</c></term>
      ///         <description>
      ///            A literal <c>%</c> character. Isn't captured 
      ///            so it doesn't count towards the overall length of the returned array. 
      ///         </description>
      ///      </item>
      ///   </list>
      /// </remarks>
      /// <example>Modifying a portrait string (C#).
      ///    <code>
      ///      using System;
      ///      
      ///      public class MyModule : Furnarchy.IFurnarchyModule
      ///      {
      ///        private readonly Furnarchy.ModuleHost m_host;
      ///      
      ///        public MyModule( Furnarchy.ModuleHost host )
      ///        {
      ///            m_host = host;
      ///            m_host.Net.InboundRawEvent += new Furnarchy.InboundPortraitSetDelegate( onInboundPortraitSet );
      ///        }
      ///            
      ///        void onInboundPortraitSet( Object sender, Furnarchy.InboundRawEventArgs e )
      ///        {
      ///            // Modify the line in-place so that all portrait names are set to "Zoidberg".
      ///            e.line = m_host.Net.format( "]f%c%c", e.colors, "Zoidberg" );
      ///         }
      ///      }
      ///    </code>
      /// </example>
      /// <returns> A byte array suitable for passing into <see cref='inject' /> or <see cref='send' />. </returns>
      array<Byte>^ format( String^ pattern, ... array<Object^>^ fields );
      /// <summary> Inject an inbound line. </summary>
      /// <remarks>
      ///   The subsequent inbound line will not raise a network event in
      ///   the calling module.
      /// </remarks>
      /// <returns> The line's id. </returns>
      UInt32 inject( array<Byte>^ line );
      /// <summary> Send an outbound line. </summary>
      /// <remarks>
      ///   The subsequent outbound line will not raise a network event in
      ///   the calling module.
      /// </remarks>
      /// <returns> The line's id. </returns>
      UInt32 send( array<Byte>^ line );

      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundChangeColor( array<Byte>^ colors );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundChangeDesc( String^ desc );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundChat( String^ body );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundDecline( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundDreamReady( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundEmote( String^ body );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundFollowDreamLink( String^ furl );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundGoBack( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundGoMap( UInt32 map_id );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundJoin( String^ name );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundLay( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundLogin( String^ name, String^ password, String^ mac_id );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundLookPos( UInt32 x, UInt32 y );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundMove( Move_Direction move_dir );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundOnlineCheck( String^ name );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundPickUp( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundPortraitChange( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundQuit( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundSetColor( array<Byte>^ colors );            
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundSetDesc( String^ desc );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundShout( String^ body );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundShoutToggle( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundSit( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundSitStandLay( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundStand( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundSummon( String^ name );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundTurnLeft( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundTurnRight( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundUseItem( );
      /// <summary> Encode an outbound (client-to-server) network line. </summary>
      array<Byte>^ encodeOutboundWhisper( String^ name, String^ body );

      /// <summary> The state of the game's primary connection. </summary>
      /// <remarks>
      ///   The connection level can be one of <see cref='Connection_Level' />.
      /// </remarks>
      property Connection_Level ConnectionLevel
      {
         Connection_Level get( ) { return m_connection_level; }
      }

      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundAvatarMoveDelegate^           InboundAvatarMoveEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundAvatarCreateDelegate^         InboundAvatarCreateEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundAvatarDestroyDelegate^        InboundAvatarDestroyEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundAvatarHideDelegate^           InboundAvatarHideEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundAvatarShowDelegate^           InboundAvatarShowEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundAvatarShow2Delegate^          InboundAvatarShow2Event;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundAvatarUpdateDelegate^         InboundAvatarUpdateEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundCameraMoveDelegate^           InboundCameraMoveEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundCameraSetDelegate^            InboundCameraSetEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundChatDelegate^                 InboundChatEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundChatBadgeDelegate^            InboundChatBadgeEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundChatEchoDelegate^             InboundChatEchoEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundChatSpeciTagDelegate^         InboundChatSpeciTagEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDeclinedDelegate^             InboundDeclinedEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDeclineEchoDelegate^          InboundDeclineEchoEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDeclineFailDelegate^          InboundDeclineFailEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDescBodyDelegate^             InboundDescBodyEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDescHeadDelegate^             InboundDescHeadEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDragonRoarDelegate^           InboundDragonRoarEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDreamPadCreateDelegate^       InboundDreamPadCreateEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDreamPadDestroyDelegate^      InboundDreamPadDestroyEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDsEmitDelegate^               InboundDsEmitEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDsTriggerDelegate^            InboundDsTriggerEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDsTriggerSelfDelegate^        InboundDsTriggerSelfEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundDsVarSetDelegate^             InboundDsVarSetEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundEmitDelegate^                 InboundEmitEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundEmoteDelegate^                InboundEmoteEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundFeetObjectSetDelegate^        InboundFeetObjectSetEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundFetchDreamDelegate^           InboundFetchDreamEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundFetchPortraitDelegate^        InboundFetchPortraitEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundFloorSetDelegate^             InboundFloorSetEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundInventoryObjectSetDelegate^   InboundInventoryObjectSetEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundJoinedDelegate^               InboundJoinedEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundJoinRequestDelegate^          InboundJoinRequestEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundLoginFailDelegate^            InboundLoginFailEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundNameLookupAmbiguousDelegate^  InboundNameLookupAmbiguousEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundNameLookupFailDelegate^       InboundNameLookupFailEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundObjectSetDelegate^            InboundObjectSetEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundOnlineCheckDelegate^          InboundOnlineCheckEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundPlaySongDelegate^             InboundPlaySongEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundPlaySoundDelegate^            InboundPlaySoundEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundPortraitSetDelegate^          InboundPortraitSetEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundRawChatDelegate^              InboundRawChatEvent;
      /// <summary> A raw, unparsed inbound network event. </summary>
      /// <remarks>
      ///   <para>
      ///      Before a line received from the server is given to the client,
      ///      Furnarchy sends it to filters, scripts, then modules.
      ///      When the module host receives the line, it triggers an <c>InboundRawEvent</c> event
      ///      then a more specific event, if one exists.
      ///   </para>
      ///   <para>
      ///      For example, if the client receives the line <c>"@!4 C"</c>, an <c>InboundRawEvent</c>
      ///      event is raised first, then (assuming the <c>line</c> field of the event object wasn't modified)
      ///      an <see cref='InboundCameraSetEvent' /> event.
      ///   </para>
      ///   <para>
      ///      At any time during the handling of these events, you may cancel/hide the line
      ///      by setting the <c>line_hide_flags</c> field of the passed <see cref='InboundRawEventArgs' /> structure
      ///      and you may edit it (in its raw form) by assigning a new String to the <c>line</c> field.
      ///   </para>
      /// </remarks>
      event InboundRawDelegate^                  InboundRawEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundRegisterFailDelegate^         InboundRegisterFailEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundRequestEchoDelegate^          InboundRequestEchoEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundRollDelegate^                 InboundRollEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundShoutDelegate^                InboundShoutEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundShoutEchoDelegate^            InboundShoutEchoEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundSuccessDelegate^              InboundSuccessEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundSummonedDelegate^             InboundSummonedEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundSummonRequestDelegate^        InboundSummonRequestEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundSuppressRenderingDelegate^    InboundSuppressRenderingEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundSyncColorCodeDelegate^        InboundSyncColorCodeEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundUseBgFileDelegate^            InboundUseBgFileEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundUnsuppressRenderingDelegate^  InboundUnsuppressRenderingEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundWhisperDelegate^              InboundWhisperEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundWhisperDeniedDelegate^        InboundWhisperDeniedEvent;
      /// <summary> A parsed inbound network event. See <see cref='InboundRawEvent' />. </summary>
      event InboundWhisperEchoDelegate^          InboundWhisperEchoEvent;

      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundChangeColorDelegate^     OutboundChangeColorEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundChangeDescDelegate^      OutboundChangeDescEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundChatDelegate^            OutboundChatEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundDeclineDelegate^         OutboundDeclineEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundDreamReadyDelegate^      OutboundDreamReadyEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundEmoteDelegate^           OutboundEmoteEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundFollowDreamLinkDelegate^ OutboundFollowDreamLinkEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundGoBackDelegate^          OutboundGoBackEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundGoMapDelegate^           OutboundGoMapEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundJoinDelegate^            OutboundJoinEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundLayDelegate^             OutboundLayEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundLoginDelegate^           OutboundLoginEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundLookPosDelegate^         OutboundLookPosEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundMoveDelegate^            OutboundMoveEvent;        
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundOnlineCheckDelegate^     OutboundOnlineCheckEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundPickUpDelegate^          OutboundPickUpEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundPortraitChangeDelegate^  OutboundPortraitChangeEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundQuitDelegate^            OutboundQuitEvent;
      /// <summary> A raw, unparsed outbound network event. </summary>
      /// <remarks>
      ///   <para>
      ///      Before a line sent from the client to the server,
      ///      Furnarchy sends it to filters, scripts, then modules.
      ///      When the module host receives the line, it triggers an <c>OutboundRawEvent</c> event
      ///      then a more specific event, if one exists.
      ///   </para>
      ///   <para>
      ///      For example, if the client receives the line <c>"m 3"</c>, an <c>OutboundRawEvent</c>
      ///      event is raised first, then (assuming the <c>line</c> field of the event object wasn't modified) 
      ///      an <see cref='OutboundMoveEvent' /> event.
      ///   </para>
      ///   <para>
      ///      At any time during the handling of these events, you may cancel/hide the line
      ///      by setting the <c>line_hide_flags</c> field of the passed <see cref='InboundRawEventArgs' /> structure
      ///      and you may edit it (in its raw form) by assigning a new String to the <c>line</c> field.
      ///   </para>
      /// </remarks>
      event OutboundRawDelegate^             OutboundRawEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundSetColorDelegate^        OutboundSetColorEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundSetDescDelegate^         OutboundSetDescEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundShoutDelegate^           OutboundShoutEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundShoutToggleDelegate^     OutboundShoutToggleEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundSitDelegate^             OutboundSitEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundSitStandLayDelegate^     OutboundSitStandLayEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundStandDelegate^           OutboundStandEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundSummonDelegate^          OutboundSummonEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundTurnLeftDelegate^        OutboundTurnLeftEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundTurnRightDelegate^       OutboundTurnRightEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundUseItemDelegate^         OutboundUseItemEvent;
      /// <summary> A parsed outbound network event. See <see cref='OutboundRawEvent' />. </summary>
      event OutboundWhisperDelegate^         OutboundWhisperEvent;

      /// <summary> The primary game server connection has been established. </summary>
      event ConnectedDelegate^    ConnectedEvent;
      /// <summary> The player has logged in. </summary>
      event LoggedInDelegate^     LoggedInEvent;
      /// <summary> The primary game server connection was lost. </summary>
      event DisconnectedDelegate^ DisconnectedEvent;

   private:

      ModuleHost^           m_host;
      Connection_Level      m_connection_level;
      yiffurc::NET_SESSION* m_session;
      LinkedList<UInt32>^   m_injects;
      LinkedList<UInt32>^   m_sends;
   };

} // namespace Furnarchy

#endif