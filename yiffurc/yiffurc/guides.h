/** \addtogroup guides Guides
*  \{ */

/** \defgroup building Building YIFFurc
* Creating a static or dynamic YIFFurc library for your platform.
*
* \section building_start Getting Started
* To provide a platform-independent way of building the YIFFurc
* library, YIFFurc uses the the SCons build system. \n
* SCons is similar to GNU's autotools 
* (i.e., the <tt>./configure && make && make install</tt> process),
* but uses Python for its configuration and has the advantage of supporting 
* Windows and Windows toolsets.
*
* To use SCons, (in addition to a C++ compiler and linker) you need
* the following installed:
*     - The Python Runtime (http://www.python.org)
*     - The SCons Build System (http://www.scons.org)
*
* Once that's done, you can continue on to creating your own dynamic or static
* library.
*
* \section building_dynamic Building a Dynamic Library / Shared Object
* To build YIFFurc as a dynamic library with SCons, \c cd into the \c yiffurc/ 
* directory and run:
* \code scons yiffurc \endcode
*
* If all goes well, you should find the DLL binaries in \c yiffurc/bin/ (\c yiffurc.so,
* or \c yiffurc.dso, or \c yiffurc.dll + \c yiffurc.lib, depending on your platform).
*
* \section building_static Building a Static Library / Archive
* To build YIFFurc as a static library with SCons, \c cd into the \c yiffurc/
* directory and run:
* \code scons STATIC_LIB=1 yiffurc \endcode
*
* If all goes well, you should find the static binaries in \c yiffurc/bin/ (\c yiffurc.a 
* or \c yiffurc.lib, depending on your platform).
*
* \note On Windows, when compiling against a static version of YIFFurc,
*       make sure to either <tt>##define YIFFURC_STATIC</tt> before including
*       the yiffurc.h header or define it on the command line.  This ensures
*       the symbols are properly found when it comes to linking with the
*       static library.
*
* \section Passing Compiler Flags
* You can also apply various compiler flags, such as optimizations, to the 
* build process with the \c CFLAGS= argument.  These flags depend on your
* compiler.
*
* For example, with the gcc/mingw compiler, I would specify optimization 
* level 2, loop-unrolling, and frame pointer omission during the build with:
* \code scons "CFLAGS=-O2 -funroll-loops -fomit-frame-pointer" yiffurc \endcode
*
* For MSVC, I might do something like:
* \code scons "CFLAGS=/O2 /Oy /GL" yiffurc \endcode
* 
* Refer to your compiler's documentation/man page for a full list of 
* compiler flags. \n
* For MSVC's compiler options, try 
* http://msdn2.microsoft.com/en-us/library/9s7c9wdw.aspx
*/

/** \defgroup offend An Offensive Example
* A C++ console app that delivers a crushing blow to Jag.
*
* \section offend_compiling offend.cpp
*
* Below is the source code for \c offend.cpp.  It should compile and run
* under most POSIX operating systems and Win32.  You'll probably want
* to modify the <tt>##include "yiffurc.h"</tt> line to point to the
* proper location of your yiffurc.h header.  Also, don't forget to
* define \c YIFFURC_STATIC if you're linking against a static version of
* \c yiffurc.lib under Win32 
* (see \ref building_static "the notes on a static YIFFurc library").
*
* The bulk of the YIFFurc code is in the <tt>game_run( )</tt> function.
*
* \include offend.cpp
*/

/** \} */
