#include "avatar.h"

namespace yiffurc {

// Formula for av_class is g_frame2cls_map[ (frame - 1) / 20 ],
// iff frame : AVCLASS_MIN <= frame <= AVCLASS_MAX
static const Av_Class g_frame2cls_map[] =
{
   AVCLASS_RODENT,
   AVCLASS_EQUINE,
   AVCLASS_FELINE,
   AVCLASS_CANINE,
   AVCLASS_MUSTELINE,
   AVCLASS_LAPINE,
   AVCLASS_RODENTCW,
   AVCLASS_EQUINECW,
   AVCLASS_FELINECW,
   AVCLASS_CANINECW,
   AVCLASS_MUSTELINECW,
   AVCLASS_KIWI,
   AVCLASS_LAPINECW,
   AVCLASS_DRAGON,
   AVCLASS_PHOENIX,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_RODENTTW,
   AVCLASS_EQUINETW,
   AVCLASS_FELINETW,
   AVCLASS_CANINETW,
   AVCLASS_MUSTELINETW,
   AVCLASS_LAPINETW,
   AVCLASS_MIN,
   AVCLASS_MIN,
   AVCLASS_RODENTFW,
   AVCLASS_EQUINEFW,
   AVCLASS_FELINEFW,
   AVCLASS_CANINEFW,
   AVCLASS_MUSTELINEFW,
   AVCLASS_LAPINEFW,
   AVCLASS_GRYFFE,
   AVCLASS_EAGLE,
   AVCLASS_RODENTBW,
   AVCLASS_EQUINEBW,
   AVCLASS_FELINEBW,
   AVCLASS_CANINEBW,
   AVCLASS_MUSTELINEBW,
   AVCLASS_LAPINEBW,
   AVCLASS_SCIURINE,
   AVCLASS_SCIURINECW,
   AVCLASS_SCIURINETW,
   AVCLASS_SCIURINEFW,
   AVCLASS_SCIURINEBW,
   AVCLASS_BOVINE,
   AVCLASS_BOVINECW,
   AVCLASS_BOVINETW,
   AVCLASS_BOVINEFW,
   AVCLASS_BOVINEBW,
   AVCLASS_RODENTPW,
   AVCLASS_EQUINEPW,
   AVCLASS_FELINEPW,
   AVCLASS_CANINEPW,
   AVCLASS_MUSTELINEPW,
   AVCLASS_LAPINEPW,
   AVCLASS_SCIURINEPW,
   AVCLASS_BOVINEPW,
   AVCLASS_PIXIE,
   AVCLASS_KITTER,
   AVCLASS_WOLVEN,
   AVCLASS_URSINE,
   AVCLASS_URSINECW,
   AVCLASS_URSINETW,
   AVCLASS_URSINEFW,
   AVCLASS_URSINEBW,
   AVCLASS_URSINEPW,
   AVCLASS_TYGARD,
   AVCLASS_LEONEN,
   AVCLASS_WEREWOLF,
   AVCLASS_PENGUIN,
   AVCLASS_BAT,
   AVCLASS_FOXEN,
   AVCLASS_FLOXEN,
   AVCLASS_UNICORN,
   AVCLASS_FURLINGM,
   AVCLASS_FURLINGF,
   AVCLASS_CHINCHILLA,
   AVCLASS_TOASTER,
   AVCLASS_BUGGE,
   AVCLASS_KIRIN,
   AVCLASS_KIRINCLOUD,
   AVCLASS_NOBLECANINEM,
   AVCLASS_NOBLECANINEF,
   AVCLASS_NOBLEFELINEM,
   AVCLASS_NOBLEFELINEF,
   AVCLASS_REINDEER,
   AVCLASS_CATTEN,
   AVCLASS_OWLEN,
   AVCLASS_LOVEBIRD
};

// [ (frame - Av_Class) / 4 ] iff frame : 0 <= frame - Av_Class <= 15 && !AVCLASS_KIWI
static const Av_Dir g_frame2dir_map[] =
{
   AVDIR_SW,
   AVDIR_SE,
   AVDIR_NW,
   AVDIR_NE,
};

// [ (frame - Av_Class) / 4 ] iff AVCLASS_KIWI
// [ (frame - Av_Class) - 16 ] iff frame : 16 <= frame - Av_Class <= 19 &&
static const Av_Dir g_frame2dir2_map[] =
{
   AVDIR_SW,
   AVDIR_SE,
   AVDIR_NE,
   AVDIR_NW,
};

// [ (frame - Av_Class) % 4 ] iff frame : 0 <= frame - Av_Class <= 15
// [ 4 ] iff frame : 16 <= frame - Av_Class <= 19
static const Av_Pose g_frame2pose_map[] =
{
   AVPOSE_SIT,
   AVPOSE_WALK0,
   AVPOSE_STAND,
   AVPOSE_WALK1,
   AVPOSE_LIE,
};

bool YIFFURC_CALL frame2av( unsigned int frame, AVFRAME* out )
{
   if (frame >= AVCLASS_MIN && frame <= AVCLASS_MAX)
   {
      out->cls = g_frame2cls_map[ (frame - 1) / 20 ];

      frame -= out->cls;

      if (frame <= 15)
      {
         out->dir  = g_frame2dir_map[ frame / 4 ];
         if (out->cls == AVCLASS_KIWI)
            out->dir = g_frame2dir2_map[ out->dir ];
         out->pose = g_frame2pose_map[ frame % 4 ];
      }
      else
      {
         out->dir  = g_frame2dir_map[ frame - 16 ];
         out->pose = g_frame2pose_map[ 4 ];
      }

      return true;
   }

   out->cls  = AVCLASS_MIN;
   out->dir  = AVDIR_SW;
   out->pose = AVPOSE_STAND;

   return false;
}

unsigned int YIFFURC_CALL av2frame( const AVFRAME* av )
{
   unsigned int frame = av->cls;

   if (av->pose < AVPOSE_LIE)
   {
      if (av->cls != AVCLASS_KIWI)
         frame += av->dir * 4 + av->pose;
      else
         frame += g_frame2dir2_map[ av->dir ] * 4 + av->pose;
   }
   else
      frame += 16 + g_frame2dir2_map[ av->dir ];

   if (frame >= AVCLASS_MIN && frame <= AVCLASS_MAX)
      return frame;

   return AVCLASS_MIN;
}

} // namespace yiffurc