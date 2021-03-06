#ifndef _IMAGE_LOCATE_H__
#define _IMAGE_LOCATE_H__

#include <cstddef>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/** Looks for unused bytes typically at the end the PE header. */
char* image_locate_header_surplus( void* base, std::size_t* max_size_out );

/** Looks for unused bytes typically at the end of the code section. */
char* image_locate_code_surplus( void* base, std::size_t* max_size_out );

/** Looks for an import thunk (the real one). */
char* image_locate_import_thunk( void* base, const char* lib,
                                 const char* symbol, bool symbol_is_ordinal );

/** Looks for the PE entry point. */
char* image_locate_entry_point( void* base );

/** Sets the PE entry point. */
bool image_set_entry_point( void* base, const void* new_entry );

#endif
