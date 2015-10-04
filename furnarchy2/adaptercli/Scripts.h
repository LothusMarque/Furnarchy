#ifndef _SCRIPTS_H__
#define _SCRIPTS_H__

#include "Furnarchy.h"

namespace Furnarchy
{
   /// <summary> Script run flags. </summary>
   [Flags]
   public enum class Script_Flags
   {
      None    = 0x0,
      Unique  = 0x1,
      Replace = 0x2,
      Late    = 0x4
   };
   /// <summary> Script run errors. </summary>
   public enum class Script_Error
   {
      Success,
      BadArgument,
      Failed,
      BadSid,
      NotUnique,
   };
   /// <summary> Information on a script instance. </summary>
   public value class ScriptInstance
   {
   public:
      UInt32          id;
      Script_Flags    flags;
      array<String^>^ arguments;
   };
   
   /// <summary> Arguments for <see cref='ScriptRanDelegate' />. </summary>
   public ref class ScriptRanEventArgs : EventArgs
   {
   public:
      ScriptInstance script;
   };
   /// <summary> Arguments for <see cref='ScriptKilledDelegate' />. </summary>
   public ref class ScriptKilledEventArgs : EventArgs
   {
   public:
      ScriptInstance script;
   };

   //////////////////////////////////////////////////////////////////////////

   /// <summary> A script event delegate. </summary>
   /// <seealso cref='ScriptsSubsystem::ScriptRanEvent' />
   public delegate void ScriptRanDelegate( Object^ sender, ScriptRanEventArgs^ e );
   /// <summary> A script event delegate. </summary>
   /// <seealso cref='ScriptsSubsystem::ScriptKilledEvent' />
   public delegate void ScriptKilledDelegate( Object^ sender, ScriptKilledEventArgs^ e );

   //////////////////////////////////////////////////////////////////////////

   /// <summary> Exposes control over the scripting subsystem. </summary>
   public ref class ScriptsSubsystem sealed
   {
   private:
      ScriptsSubsystem( ) { }
   internal:
      ScriptsSubsystem( ModuleHost^ host );
      ~ScriptsSubsystem( );

      void onScript( );

   public:

      /// <summary> Retrieve an array of all running scripts. </summary>
      /// <remarks> 
      ///  To maintain an up-to-date list of running scripts, one should
      ///  make a call to <c>getRunningScripts</c> whenever running scripts have been updated.
      /// </remarks>
      /// <returns>
      ///  An array of <see cref='ScriptInstance' /> structures with information on each running script.
      /// </returns>
      /// <seealso cref='ScriptRanEvent' />
      /// <seealso cref='ScriptKilledEvent' />
      array<ScriptInstance>^ getRunningScripts( );
      /// <summary> Run a script. </summary>
      /// <param name='instance'> A <see cref='ScriptInstance' /> structure that contains the script arguments to run. </param>
      /// <remarks>
      ///   If successful, the <c>id</c> member of <c>instance</c> will be filled with the ID of the running script.
      /// </remarks>
      /// <returns> A <see cref='Script_Error' /> enumeration indicating success or failure. </returns>
      Script_Error run( ScriptInstance% instance );
      /// <summary> Kill a running script. </summary>
      /// <param name='id'> The ID of the running script. </param>
      /// <returns> A <see cref='Script_Error' /> enumeration indicating success or failure. </returns>
      Script_Error kill( UInt32 id );

      /// <summary> A script has been run. </summary>
      event ScriptRanDelegate^    ScriptRanEvent;
      /// <summary> A script has been killed. </summary>
      event ScriptKilledDelegate^ ScriptKilledEvent;

   private:
      ModuleHost^                 m_host;
      LinkedList<ScriptInstance>^ m_scripts;
   };
}

#endif