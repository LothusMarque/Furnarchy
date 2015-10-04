#ifndef _X86PP_H__
#define _X86PP_H__

#include <cstddef>

namespace x86pp
{
   namespace Registers { enum reg32 { EAX = 0, ECX = 1, EDX = 2, EBX = 3, 
      ESP = 4, EBP = 5, ESI = 6, EDI = 7 }; };
   using Registers::reg32;

   typedef void* mem32;
   typedef int   imm32;
   typedef short imm16;
   typedef int   rel32;

   std::size_t asm_mov( char* o, reg32 dst, reg32 src );
   std::size_t asm_mov( char* o, reg32 dst, imm32 src );
   std::size_t asm_mov( char* o, mem32 dst, reg32 src );
   std::size_t asm_mov( char* o, mem32 dst, imm32 src );
   std::size_t asm_mov( char* o, reg32 dst, int off, reg32 src );
   std::size_t asm_mov( char* o, reg32 dst, int off, imm32 src );
   std::size_t asm_mov( char* o, reg32 dst, reg32 src, int off );
   std::size_t asm_mov( char* o, reg32 dst, mem32 src, int off );

   std::size_t asm_push( char* o, reg32 src );
   std::size_t asm_push( char* o, imm32 src );
   std::size_t asm_push( char* o, reg32 src, int off );
   std::size_t asm_push( char* o, mem32 src, int off );

   std::size_t asm_pop( char* o, reg32 dst );
   std::size_t asm_pop( char* o, mem32 dst );

   std::size_t asm_jmp( char* o, reg32 src );
   std::size_t asm_jmp( char* o, rel32 src );
   std::size_t asm_jmp( char* o, reg32 src, int off );
   std::size_t asm_jmp( char* o, mem32 src, int off );

   std::size_t asm_jz( char* o, rel32 src );
   std::size_t asm_jnz( char* o, rel32 src );

   std::size_t asm_add( char* o, reg32 dst, reg32 src );
   std::size_t asm_add( char* o, reg32 dst, imm32 src );
   std::size_t asm_add( char* o, reg32 dst, reg32 src, int off );
   std::size_t asm_add( char* o, reg32 dst, mem32 src, int off );

   std::size_t asm_sub( char* o, reg32 dst, reg32 src );
   std::size_t asm_sub( char* o, reg32 dst, imm32 src );
   std::size_t asm_sub( char* o, reg32 dst, reg32 src, int off );
   std::size_t asm_sub( char* o, reg32 dst, mem32 src, int off );

   std::size_t asm_xor( char* o, reg32 dst, reg32 src );

   std::size_t asm_test( char* o, reg32 a, reg32 b );
   std::size_t asm_test( char* o, reg32 a, imm32 b );

   std::size_t asm_call( char* o, reg32 src );
   std::size_t asm_call( char* o, rel32 src );
   std::size_t asm_call( char* o, reg32 src, int off );
   std::size_t asm_call( char* o, mem32 src, int off );

   std::size_t asm_nop( char* o );
   std::size_t asm_ret( char* o, imm16 pop = 0 );

   std::size_t asm_pushd( char* o );
   std::size_t asm_popd( char* o );

   std::size_t asm_rep_movsb( char* o );
   std::size_t asm_rep_movsw( char* o );
   std::size_t asm_rep_movsd( char* o );

} // namespace x86pp

#endif
