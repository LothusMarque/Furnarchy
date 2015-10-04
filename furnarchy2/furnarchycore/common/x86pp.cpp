#include "x86pp.h"

using namespace x86pp::Registers;

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4309 )
#endif

namespace x86pp
{
   using std::size_t;

   size_t asm_mov( char* o, reg32 dst, reg32 src )
   {
      *(o++) = 0x8B;
      *(o++) = (0x03 << 6) | (dst << 3) | (src);
      return 2;
   }

   size_t asm_mov( char* o, reg32 dst, imm32 src )
   {
      *(o++) = (0xB8 + dst);
      *( ((imm32*&) o)++ ) = src;
      return 1 + sizeof( imm32 );
   }

   size_t asm_mov( char* o, mem32 dst, reg32 src )
   {
      *(o++) = 0x89;
      *(o++) = (0x00 << 6) | (src << 3) | (0x05);
      *( ((mem32*&) o)++ ) = dst;
      return 2 + sizeof( mem32 );
   }

   size_t asm_mov( char* o, mem32 dst, imm32 src )
   {
      *(o++) = 0xC7;
      *(o++) = (0x00 << 6) | (0x00 << 3) | (0x05);
      *( ((mem32*&) o)++ ) = dst;
      *( ((imm32*&) o)++ ) = src;
      return 2 + sizeof( mem32 ) + sizeof( imm32 );
   }

   size_t asm_mov( char* o, reg32 dst, int off, reg32 src )
   {
      *(o++) = 0x89;

      if (off)
      {
         *(o++) = (0x02 << 6) | (src << 3) | (dst);
         if (dst == ESP)
         {
            *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
            *( ((int*&) o)++ ) = off;
            return 3 + sizeof( int );
         }
         *( ((int*&) o)++ ) = off;
         return 2 + sizeof( int );
      }

      *(o++) = (0x00 << 6) | (src << 3) | (dst);
      if (dst == ESP)
      {
         *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
         return 3;
      }
      return 2;
   }

   size_t asm_mov( char* o, reg32 dst, int off, imm32 src )
   {
      *(o++) = 0xC7;

      if (off)
      {
         *(o++) = (0x02 << 6) | (0x00 << 3) | (dst);
         if (dst == ESP)
         {
            *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
            *( ((int*&) o)++ ) = off;
            *( ((imm32*&) o)++ ) = src;
            return 3 + sizeof( int ) + sizeof( imm32 );
         }
         *( ((int*&) o)++ ) = off;
         *( ((imm32*&) o)++ ) = src;
         return 2 + sizeof( int ) + sizeof( imm32 );
      }

      *(o++) = (0x00 << 6) | (0x00 << 3) | (dst);
      if (dst == ESP)
      {
         *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
         *( ((imm32*&) o)++ ) = src;
         return 3 + sizeof( imm32 );
      }
      *( ((imm32*&) o)++ ) = src;
      return 2 + sizeof( imm32 );
   }

   size_t asm_mov( char* o, reg32 dst, reg32 src, int off )
   {
      *(o++) = 0x8B;

      if (off)
      {
         *(o++) = (0x02 << 6) | (dst << 3) | (src);
         if (src == ESP)
         {
            *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
            *( ((int*&) o)++ ) = off;
            return 3 + sizeof( int );
         }
         *( ((int*&) o)++ ) = off;
         return 2 + sizeof( int );
      }

      *(o++) = (0x00 << 6) | (dst << 3) | (src);
      if (src == ESP)
      {
         *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
         return 3;
      }
      return 2;
   }

   size_t asm_mov( char* o, reg32 dst, mem32 src, int off )
   {
      *(o++) = 0x8B;
      *(o++) = (0x00 << 6) | (dst << 3) | (0x05);
      *( ((mem32*&) o)++ ) = (mem32)( (size_t) src + off );
      return 2 + sizeof( mem32 );
   }

   size_t asm_push( char* o, reg32 src )
   {
      *(o++) = (0x50 + src);
      return 1;
   }

   size_t asm_push( char* o, imm32 src )
   {
      *(o++) = 0x68;
      *( ((imm32*&) o)++ ) = src;
      return 1 + sizeof( imm32 );
   }

   size_t asm_push( char* o, reg32 src, int off )
   {
      *(o++) = 0xFF;

      if (off)
      {
         *(o++) = (0x02 << 6) | (0x06 << 3) | (src);
         if (src == ESP)
         {
            *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
            *( ((int*&) o)++ ) = off;
            return 3 + sizeof( int );
         }
         *( ((int*&) o)++ ) = off;
         return 2 + sizeof( int );
      }

      *(o++) = (0x00 << 6) | (0x06 << 3) | (src);
      if (src == ESP)
      {
         *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
         return 3;
      }
      return 2;
   }

   size_t asm_push( char* o, mem32 src, int off )
   {
      *(o++) = 0xFF;
      *(o++) = (0x00 << 6) | (0x06 << 3) | (0x05);
      *( ((mem32*&) o)++ ) = (mem32)( (size_t) src + off );
      return 2 + sizeof( mem32 );
   }

   size_t asm_pop( char* o, reg32 dst )
   {
      *(o++) = (0x58 + dst);
      return 1;
   }

   size_t asm_pop( char* o, mem32 dst )
   {
      *(o++) = 0x8F;
      *(o++) = (0x00 << 6) | (0x00 << 3) | (0x05);
      *( ((mem32*&) o)++ ) = dst;
      return 2 + sizeof( mem32 );
   }

   size_t asm_jmp( char* o, reg32 src )
   {
      *(o++) = 0xFF;
      *(o++) = (0x03 << 6) | (0x04 << 3) | (src);
      return 2;
   }

   size_t asm_jmp( char* o, rel32 src )
   {
      *(o++) = 0xE9;
      *( ((rel32*&) o)++ ) = src - (1 + sizeof( rel32 ));
      return 1 + sizeof( rel32 );
   }

   size_t asm_jmp( char* o, reg32 src, int off )
   {
      *(o++) = 0xFF;

      if (off)
      {
         *(o++) = (0x02 << 6) | (0x04 << 3) | (src);
         if (src == ESP)
         {
            *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
            *( ((int*&) o)++ ) = off;
            return 3 + sizeof( int );
         }
         *( ((int*&) o)++ ) = off;
         return 2 + sizeof( int );
      }

      *(o++) = (0x00 << 6) | (0x04 << 3) | (src);
      if (src == ESP)
      {
         *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
         return 3;
      }
      return 2;
   }

   size_t asm_jmp( char* o, mem32 src, int off )
   {
      *(o++) = 0xFF;
      *(o++) = (0x00 << 6) | (0x04 << 3) | (0x05);
      *( ((mem32*&) o)++ ) = (mem32)( (size_t) src + off );
      return 2 + sizeof( mem32 );
   }

   size_t asm_jz( char* o, rel32 src )
   {
      *(o++) = 0x0F;
      *(o++) = 0x84;
      *( ((rel32*&) o)++ ) = src - 6;
      return 2 + sizeof( rel32 );   
   }

   size_t asm_jnz( char* o, rel32 src )
   {
      *(o++) = 0x0F;
      *(o++) = 0x85;
      *( ((rel32*&) o)++ ) = src - 6;
      return 2 + sizeof( rel32 );   
   }

   size_t asm_add( char* o, reg32 dst, reg32 src )
   {
      *(o++) = 0x03;
      *(o++) = (0x03 << 6) | (dst << 3) | (src);
      return 2;
   }

   size_t asm_add( char* o, reg32 dst, imm32 src )
   {
      *(o++) = 0x81;
      *(o++) = (0x03 << 6) | (0x00 << 3) | (dst);
      *( ((imm32*&) o)++ ) = src;
      return 2 + sizeof( imm32 );
   }

   size_t asm_add( char* o, reg32 dst, reg32 src, int off )
   {
      *(o++) = 0x03;

      if (off)
      {
         *(o++) = (0x02 << 6) | (dst << 3) | (src);
         if (src == ESP)
         {
            *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
            *( ((int*&) o)++ ) = off;
            return 3 + sizeof( int );
         }
         *( ((int*&) o)++ ) = off;
         return 2 + sizeof( int );
      }

      *(o++) = (0x00 << 6) | (dst << 3) | (src);
      if (src == ESP)
      {
         *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
         return 3;
      }
      return 2;
   }

   size_t asm_add( char* o, reg32 dst, mem32 src, int off )
   {
      *(o++) = 0x03;
      *(o++) = (0x00 << 6) | (dst << 3) | (0x05);
      *( ((mem32*&) o)++ ) = (mem32)( (size_t) src + off );
      return 2 + sizeof( mem32 );
   }

   size_t asm_sub( char* o, reg32 dst, reg32 src )
   {
      *(o++) = 0x2B;
      *(o++) = (0x03 << 6) | (dst << 3) | (src);
      return 2;
   }

   size_t asm_sub( char* o, reg32 dst, imm32 src )
   {
      *(o++) = 0x81;
      *(o++) = (0x03 << 6) | (0x05 << 3) | (dst);
      *( ((imm32*&) o)++ ) = src;
      return 2 + sizeof( imm32 );
   }

   size_t asm_sub( char* o, reg32 dst, reg32 src, int off )
   {
      *(o++) = 0x2B;

      if (off)
      {
         *(o++) = (0x02 << 6) | (dst << 3) | (src);
         if (src == ESP)
         {
            *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
            *( ((int*&) o)++ ) = off;
            return 3 + sizeof( int );
         }
         *( ((int*&) o)++ ) = off;
         return 2 + sizeof( int );
      }

      *(o++) = (0x00 << 6) | (dst << 3) | (src);
      if (src == ESP)
      {
         *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
         return 3;
      }
      return 2;
   }

   size_t asm_sub( char* o, reg32 dst, mem32 src, int off )
   {
      *(o++) = 0x2B;
      *(o++) = (0x00 << 6) | (dst << 3) | (0x05);
      *( ((mem32*&) o)++ ) = (mem32)( (size_t) src + off );
      return 2 + sizeof( mem32 );
   }

   size_t asm_xor( char* o, reg32 dst, reg32 src )
   {
      *(o++) = 0x33;
      *(o++) = (0x03 << 6) | (dst << 3) | (src);
      return 2;
   }

   size_t asm_test( char* o, reg32 a, reg32 b )
   {
      *(o++) = 0x85;
      *(o++) = (0x03 << 6) | (b << 3) | (a);
      return 2;
   }

   size_t asm_test( char* o, reg32 a, imm32 b )
   {
      *(o++) = 0xF7;
      *(o++) = (0x03 << 6) | (0x00 << 3) | (a);
      *( ((imm32*&) o)++ ) = b;
      return 2 + sizeof( imm32 );
   }

   size_t asm_call( char* o, reg32 src )
   {
      *(o++) = 0xFF;
      *(o++) = (0x03 << 6) | (0x02 << 3) | (src);
      return 2;
   }

   size_t asm_call( char* o, rel32 src )
   {
      *(o++) = 0xE8;
      *( ((rel32*&) o)++ ) = src - (1 + sizeof( rel32 ));
      return 1 + sizeof( rel32 );
   }

   size_t asm_call( char* o, reg32 src, int off )
   {
      *(o++) = 0xFF;

      if (off)
      {
         *(o++) = (0x02 << 6) | (0x02 << 3) | (src);
         if (src == ESP)
         {
            *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
            *( ((int*&) o)++ ) = off;
            return 3 + sizeof( int );
         }
         *( ((int*&) o)++ ) = off;
         return 2 + sizeof( int );
      }

      *(o++) = (0x00 << 6) | (0x02 << 3) | (src);
      if (src == ESP)
      {
         *(o++) = (0x00 << 6) | (ESP << 3) | (ESP);
         return 3;
      }
      return 2;
   }

   size_t asm_call( char* o, mem32 src, int off )
   {
      *(o++) = 0xFF;
      *(o++) = (0x00 << 6) | (0x02 << 3) | (0x05);
      *( ((mem32*&) o)++ ) = (mem32)( (size_t) src + off );
      return 2 + sizeof( mem32 );
   }

   size_t asm_nop( char* o )
   {
      *(o++) = 0x90;
      return 1;
   }

   size_t asm_ret( char* o, imm16 pop /* = 0  */ )
   {
      if (pop)
      {
         *(o++) = 0xC2;
         *( ((imm16*&) o)++ ) = pop;
         return 1 + sizeof( imm16 );
      }

      *(o++) = 0xC3;
      return 1;
   }

   size_t asm_pushd( char* o )
   {
      *o = 0x60;
      return 1;
   }

   size_t asm_popd( char* o )
   {
      *o = 0x61;
      return 1;
   }

   size_t asm_rep_movsb( char* o )
   {
      *(o++) = 0xF3;
      *o     = 0xA4;
      return 2;
   }

   size_t asm_rep_movsw( char* o )
   {
      *(o++) = 0x66; // ??
      *(o++) = 0xF3;
      *o     = 0xA5;
      return 3;
   }

   size_t asm_rep_movsd( char* o )
   {
      *(o++) = 0xF3;
      *o     = 0xA5;
      return 2;
   }

   size_t asm_cmp( char* o, reg32 a, reg32 b )
   {
      *(o++) = 0x3B;
      *(o++) = (0x03 << 6) | (b << 3) | (a);
      return 2;
   }

   size_t asm_cmp( char* o, reg16 a, reg16 b )
   {
      *(o++) = 0x66; // 16-bit prefix
      *(o++) = 0x3B;
      *(o++) = (0x03 << 6) | (b << 3) | (a);
      return 3;
   }

   size_t asm_cmp( char* o, reg8 a, reg8 b )
   {
      *(o++) = 0x3A;
      *(o++) = (0x03 << 6) | (b << 3) | (a);
      return 2;
   }

   size_t asm_cmp( char* o, reg32 a, imm32 b )
   {
      if (a == EAX)
      {
         *(o++) = 0x3D;   
         *( ((imm32*&) o)++ ) = b;
         return 5;
      }

      *(o++) = 0x81;
      *(o++) = (0x03 << 6) | (a << 3) | (0x7);
      *( ((imm32*&) o)++ ) = b;
      return 6;
   }

   size_t asm_cmp( char* o, reg16 a, imm16 b )
   {
      *(o++) = 0x66; // 16-bit prefix
      *(o++) = 0x81;
      *(o++) = (0x03 << 6) | (a << 3) | (0x7);
      *( ((imm16*&) o)++ ) = b;
      return 5;
   }

   size_t asm_cmp( char* o, reg8 a, imm8 b )
   {
      *(o++) = 0x80;
      *(o++) = (0x03 << 6) | (a << 3) | (0x7);
      *( ((imm8*&) o)++ ) = b;
      return 3;
   }
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif
