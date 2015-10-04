/************************************************************************/
/* Copyright (c) 2006 Cluracan (strangelove@heroinpuppy.com)            */
/*                                                                      */
/* The "MIT" License                                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* - The above copyright notice and this permission notice shall be     */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          */
/* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         */
/* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                            */
/************************************************************************/

#ifndef _YIFFURC_AVATAR_H__
#define _YIFFURC_AVATAR_H__

#include "common.h"

/** \addtogroup avatar Avatar Frames
* \{ */

#if defined( __cplusplus )
namespace yiffurc {
#endif

/** Avatar classes.
* These are also the frame numbers of the first frame in
* their series.
* Note that 'BW' = bat wings and 'FW' = butterfly wings. */
enum Av_Class
{
   AVCLASS_MIN           = 1,
   AVCLASS_RODENT        = 1,
   AVCLASS_EQUINE        = 21,
   AVCLASS_FELINE        = 41,
   AVCLASS_CANINE        = 61,
   AVCLASS_MUSTELINE     = 81,
   AVCLASS_LAPINE        = 101,
   AVCLASS_RODENTCW      = 121,
   AVCLASS_EQUINECW      = 141,
   AVCLASS_FELINECW      = 161,
   AVCLASS_CANINECW      = 181,
   AVCLASS_MUSTELINECW   = 201,
   AVCLASS_KIWI          = 221,
   AVCLASS_LAPINECW      = 241,
   AVCLASS_DRAGON        = 261,
   AVCLASS_PHOENIX       = 281,
   AVCLASS_RODENTTW      = 501,
   AVCLASS_EQUINETW      = 521,
   AVCLASS_FELINETW      = 541,
   AVCLASS_CANINETW      = 561,
   AVCLASS_MUSTELINETW   = 581,
   AVCLASS_LAPINETW      = 601,
   AVCLASS_RODENTFW      = 661,
   AVCLASS_EQUINEFW      = 681,
   AVCLASS_FELINEFW      = 701,
   AVCLASS_CANINEFW      = 721,
   AVCLASS_MUSTELINEFW   = 741,
   AVCLASS_LAPINEFW      = 761,
   AVCLASS_GRYFFE        = 781,
   AVCLASS_EAGLE         = 801,
   AVCLASS_RODENTBW      = 821,
   AVCLASS_EQUINEBW      = 841,
   AVCLASS_FELINEBW      = 861,
   AVCLASS_CANINEBW      = 881,
   AVCLASS_MUSTELINEBW   = 901,
   AVCLASS_LAPINEBW      = 921,
   AVCLASS_SCIURINE      = 941,
   AVCLASS_SCIURINECW    = 961,
   AVCLASS_SCIURINETW    = 981,
   AVCLASS_SCIURINEFW    = 1001,
   AVCLASS_SCIURINEBW    = 1021,
   AVCLASS_BOVINE        = 1041,
   AVCLASS_BOVINECW      = 1061,
   AVCLASS_BOVINETW      = 1081,
   AVCLASS_BOVINEFW      = 1101,
   AVCLASS_BOVINEBW      = 1121,
   AVCLASS_RODENTPW      = 1141,
   AVCLASS_EQUINEPW      = 1161,
   AVCLASS_FELINEPW      = 1181,
   AVCLASS_CANINEPW      = 1201,
   AVCLASS_MUSTELINEPW   = 1221,
   AVCLASS_LAPINEPW      = 1241,
   AVCLASS_SCIURINEPW    = 1261,
   AVCLASS_BOVINEPW      = 1281,
   AVCLASS_PIXIE         = 1301,
   AVCLASS_KITTER        = 1321,
   AVCLASS_WOLVEN        = 1341,
   AVCLASS_URSINE        = 1361,
   AVCLASS_URSINECW      = 1381,
   AVCLASS_URSINETW      = 1401,
   AVCLASS_URSINEFW      = 1421,
   AVCLASS_URSINEBW      = 1441,
   AVCLASS_URSINEPW      = 1461,
   AVCLASS_TYGARD        = 1481,
   AVCLASS_LEONEN        = 1501,
   AVCLASS_WEREWOLF      = 1521,
   AVCLASS_PENGUIN       = 1541,
   AVCLASS_BAT           = 1561,
   AVCLASS_FOXEN         = 1581,
   AVCLASS_FLOXEN        = 1601,
   AVCLASS_UNICORN       = 1621,
   AVCLASS_FURLINGM      = 1641,
   AVCLASS_FURLINGF      = 1661,
   AVCLASS_CHINCHILLA    = 1681,
   AVCLASS_TOASTER       = 1701,
   AVCLASS_MAX           = 1721,

};

/** Avatar poses. */
enum Av_Pose
{
   AVPOSE_SIT,
   AVPOSE_WALK0,
   AVPOSE_STAND,
   AVPOSE_WALK1,
   AVPOSE_LIE,
};

/** Avatar directions (direction they face). */
enum Av_Dir
{
   AVDIR_SW,
   AVDIR_SE,
   AVDIR_NW,
   AVDIR_NE,
};

/** Describes an avatar frame.
* \sa av2frame, frame2av */
struct AVFRAME
{
   /** The class of avatar (e.g., equine, dragon, lapine with classic wings). */
   enum Av_Class cls;
   /** The pose (e.g., standing, walking, sitting). */
   enum Av_Pose  pose;
   /** The direction it faces (e.g., SW, SE, NW). */
   enum Av_Dir   dir;
};

/** Extracts avatar information from a frame number.
* \param[in]    frame   Avatar frame number.
* \param[out]    out    Extracted avatar information.
* \return \c false if the frame was out of range.
* \sa AVFRAME, S2C_TYPE_AVCREATE, S2C_TYPE_AVTWEEN */
YIFFURC_LINK bool YIFFURC_CALL frame2av( unsigned int frame, struct AVFRAME* out );

/** Converts avatar information into a frame number.
* \param[in]    av      Avatar information.
* \return The frame number.
* \sa AVFRAME, s2c_enc_avset, s2c_enc_avtween */
YIFFURC_LINK unsigned int YIFFURC_CALL av2frame( const struct AVFRAME* av );

#if defined( __cplusplus )
} // namespace yiffurc
#endif

/** \} avatar */

#endif
