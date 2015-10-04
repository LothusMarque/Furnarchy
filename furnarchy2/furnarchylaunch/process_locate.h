#ifndef _PROCESS_LOCATE_H__
#define _PROCESS_LOCATE_H__

#include <cstddef>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/** Copies a process' PE headers. */
bool process_copy_pe_header( HANDLE process, void* base, IMAGE_DOS_HEADER* dh, 
                             IMAGE_NT_HEADERS* nth );

/** Looks for unused bytes typically at the end the PE header. */
char* process_locate_header_surplus( HANDLE process, std::size_t* max_size_out );

/** Looks for unused bytes typically at the end of the code section. */
char* process_locate_code_surplus( HANDLE process, std::size_t* max_size_out );

/** Looks for an import thunk (the real one). */
char* process_locate_import_thunk( HANDLE process, const char* lib,
                                          const char* symbol, bool symbol_is_ordinal );

/** Looks for the process entry point. */
char* process_locate_entry_point( HANDLE process );

#endif
