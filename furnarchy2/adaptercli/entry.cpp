#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../furnarchycore/furn2.h"

using namespace furn2;

//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain( HMODULE inst, DWORD reason, LPVOID reserved )
{
   if (reason == DLL_PROCESS_ATTACH)
      DisableThreadLibraryCalls( (HMODULE) inst );
   return TRUE;
}

/************************************************************************/
/* Managed entry points.                                                */
/************************************************************************/

extern bool _create( unsigned int me, const char* root, long flags );
extern bool _destroy( unsigned int me );
extern void _on_inet( unsigned int me, Revision rev, const char* line, int from, size_t num );
extern void _on_onet( unsigned int me, Revision rev, const char* line, int from, size_t num );
extern intptr_t _on_module( unsigned int me, Module_Event e, intptr_t param );
extern intptr_t _on_client( unsigned int me, Client_Event e, intptr_t param );
extern bool _on_wndproc( unsigned int me, WNDPROCPARAMS* params );
extern intptr_t _on_world( unsigned int me, World_Event e, intptr_t param );
extern bool _on_command( unsigned int me, int argc, const char* argv[] );

/************************************************************************/
/* Native stubs for managed entry points.                               */
/************************************************************************/

F2EXPORT bool F2CALL create( unsigned int me, const char* root, long flags )
{
   return _create( me, root, flags );
}

F2EXPORT bool F2CALL destroy( unsigned int me )
{  
   return _destroy( me );
}

F2EXPORT void F2CALL on_inet( unsigned int me, Revision rev, const char* line,
                              int from, size_t num )
{
   _on_inet( me, rev, line, from, num );
}

F2EXPORT void F2CALL on_onet( unsigned int me, Revision rev, const char* line,
                              int from, size_t num )
{
   _on_onet( me, rev, line, from, num );
}

F2EXPORT intptr_t F2CALL on_module( unsigned int me, Module_Event e, intptr_t param )
{
   return _on_module( me, e, param );
}

F2EXPORT intptr_t F2CALL on_client( unsigned int me, Client_Event e, intptr_t param )
{
   return _on_client( me, e, param );
}

F2EXPORT bool F2CALL on_wndproc( unsigned int me, WNDPROCPARAMS* params )
{
   return _on_wndproc( me, params );
}   

F2EXPORT intptr_t F2CALL on_world( unsigned int me, World_Event e, intptr_t param )
{
   return _on_world( me, e, param );
}

F2EXPORT bool F2CALL on_command( unsigned int me, int argc, const char* argv[] )
{
   return _on_command( me, argc, argv );
}
