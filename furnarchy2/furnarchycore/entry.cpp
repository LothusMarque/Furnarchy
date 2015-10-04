#include "core.h"

HMODULE g_inst;

BOOL APIENTRY DllMain( HMODULE inst, DWORD reason, LPVOID reserved )
{
   if (reason == DLL_PROCESS_ATTACH)
   {
      DisableThreadLibraryCalls( (HMODULE) inst );
      g_inst = inst;
   }
   else if (reason == DLL_PROCESS_DETACH)
   {
      core::uninitialize( );
   }

   return TRUE;
}

/* The loader routine. */
extern "C" __declspec( dllexport ) void CDECL go( unsigned int crc )
{
   core::initialize( g_inst, crc );
}
