#ifndef _FURNARCHY_H__
#define _FURNARCHY_H__

#include "../furnarchycore/furn2.h"
#include "cli_util.h"
#include <vcclr.h>

namespace Furnarchy
{
   using namespace System;
   using namespace System::Collections::Generic;
   using namespace System::Runtime::InteropServices;

   /// <summary>Empty interface a module's primary class should implement.</summary>
   /// <remarks>
   ///   <para>
   ///      The module host uses reflection to find the first public class that implements
   ///      this interface then constructs it, passing the <see cref='ModuleHost' /> instance.
   ///   </para>
   /// </remarks>
   /// <example>
   ///   Defining a CLI module's primary class (C#).
   ///   <code>
   ///      public class MyModule : Furnarchy.IFurnarchyModule
   ///      {
   ///         ModuleHost m_host;
   ///         // Constructor must have this signature.
   ///         public MyModule( ModuleHost host )
   ///         {
   ///            m_host = host;
   ///         }
   ///      }
   ///   </code>
   /// </example>
   public interface class IFurnarchyModule { };

   /// <summary> Map movement directions. </summary>
   public enum class Move_Direction
   {
      SouthWest = 1,
      SouthEast = 3,
      NorthEast = 7,
      NorthWest = 9
   };
   /// <summary> Avatar types. </summary>
   public enum class Avatar_Class
   {
      Rodent,
      Equine,
      Feline,
      Canine,
      Musteline,
      Lapine,
      RodentClassicWings,
      EquineClassicWings,
      FelineClassicWings,
      CanineClassicWings,
      MustelineClassicWings,
      Kiwi,
      LapineClassicWings,
      Dragon,
      Phoenix,
      RodentTriWings,
      EquineTriWings,
      FelineTriWings,
      CanineTriWings,
      MustelineTriWings,
      LapineTriWings,
      RodentButterflyWings,
      EquineButterflyWings,
      FelineButterflyWings,
      CanineButterflyWings,
      MustelineButterflyWings,
      LapineButterflyWings,
      Gryffe,
      Eagle,
      RodentBatWings,
      EquineBatWings,
      FelineBatWings,
      CanineBatWings,
      MustelineBatWings,
      LapineBatWings,
      Sciurine,
      SciurineClassicWings,
      SciurineTriWings,
      SciurineButterflyWings,
      SciurineBatWings,
      Bovine,
      BovineClassicWings,
      BovineTriWings,
      BovineButterflyWings,
      BovineBatWings,
      RodentPrimeWings,
      EquinePrimeWings,
      FelinePrimeWings,
      CaninePrimeWings,
      MustelinePrimeWings,
      LapinePrimeWings,
      SciurinePrimeWings,
      BovinePrimeWings,
      Pixie,
      Kitter,
      Wolven,
      Ursine,
      UrsineClassicWings,
      UrsineTriWings,
      UrsineButterflyWings,
      UrsineBatWings,
      UrsinePrimeWings,
      Tygard,
      Leonen,
      Werewolf,
      Penguin,
      Bat,
      Foxen,
      Floxen,
      Unicorn,
      FurlingMale,
      FurlingFemale,
      Chinchilla,
      Toaster
   };
   /// <summary> Avatar poses. </summary>
   public enum class Avatar_Pose
   {
      Sitting,
      Walking1,
      Standing,
      Walking2,
      Laying
   };
   
   /// <summary> Monster (avatar) creation flags. </summary>
   [Flags]
   public enum class Monster_Flags
   {
      None = 0x0,
      HasProfile = 0x1,
      Unk1 = 0x2,
      Unk2 = 0x4
   };

   /// <summary> Furnarchy version structure. </summary>
   public value class F2Version
   {
   public:
      UInt32 major;
      UInt32 minor;
      UInt32 revision;
   };

   //////////////////////////////////////////////////////////////////////////

   public ref class FurnarchyException : public Exception
   {
   public:
      FurnarchyException( String^ msg ) : Exception( msg ) { }
   };

   ref class ModuleHost;
   ref class ClientSubsystem;
   ref class NetSubsystem;
   ref class ScriptsSubsystem;
   ref class WorldSubsystem;
}

#include "ModuleHost.h"
#include "Client.h"
#include "Net.h"
#include "Scripts.h"
#include "World.h"

#endif