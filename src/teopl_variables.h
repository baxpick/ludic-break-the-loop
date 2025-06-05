#ifndef _TEOPL_VARIABLES_H_
#define _TEOPL_VARIABLES_H_

#include "cpctelera_wrap.h"

/// Use to declare/define global variables so that addresses are not "moved around" by the compiler
/// this means that array of bytes is defined inside a wrapper function with exposed labels(variables)
/// since functions are put in memory in a sequence, the variables wrapped by the function will have predictable addresses
/// Main usage is inside "apps" since apps must have globals inside [0x4000-0x7FFF]

// util
#define STRINGIFY(s) #s
#define _Args(...) __VA_ARGS__
#define STRIP_PARENTHESES(X) X
#define PASS_PARAMETERS(X) STRIP_PARENTHESES( _Args X )

// declare
#define DCL(TYPE,NAME) extern TYPE NAME;
#define DCL_array(TYPE,NAME) extern TYPE NAME[];

#define DCL_u8(NAME) DCL(u8, NAME)
#define DCL_i8(NAME) DCL(i8, NAME)
#define DCL_u16(NAME) DCL(u16, NAME)
#define DCL_i16(NAME) DCL(i16, NAME)
#define DCL_u32(NAME) DCL(u32, NAME)
#define DCL_i32(NAME) DCL(i32, NAME)
#define DCL_u8_ptr(NAME) DCL(u8*, NAME)
#define DCL_i8_ptr(NAME) DCL(i8*, NAME)
#define DCL_u16_ptr(NAME) DCL(u16*, NAME)
#define DCL_i16_ptr(NAME) DCL(i16*, NAME)
#define DCL_u32_ptr(NAME) DCL(u32*, NAME)
#define DCL_i32_ptr(NAME) DCL(i32*, NAME)

#define DCL_u8_array(NAME) DCL_array(u8, NAME)
#define DCL_i8_array(NAME) DCL_array(i8, NAME)
#define DCL_u16_array(NAME) DCL_array(u16, NAME)
#define DCL_i16_array(NAME) DCL_array(i16, NAME)
#define DCL_u32_array(NAME) DCL_array(u32, NAME)
#define DCL_i32_array(NAME) DCL_array(i32, NAME)

#define MYCONCAT(BASE,MODULE) BASE##MODULE

// define
#ifndef WINCPCTELERA
#define DEF_start(MODULE) void MYCONCAT(variables_space,MODULE)() __naked {
#define DEF_end }
#else
#define DEF_start(MODULE)
#define DEF_end
#endif

#ifndef WINCPCTELERA
#define Z80_DEF(NAME,SIZE) __asm__("_" NAME "::"); __asm__(".ds " SIZE);

// variables and non-const arrays
#define DEF_u8(NAME) Z80_DEF(STRINGIFY(NAME),"1")
#define DEF_i8(NAME) Z80_DEF(STRINGIFY(NAME),"1")
#define DEF_u16(NAME) Z80_DEF(STRINGIFY(NAME),"2")
#define DEF_i16(NAME) Z80_DEF(STRINGIFY(NAME),"2")
#define DEF_u32(NAME) Z80_DEF(STRINGIFY(NAME),"4")
#define DEF_i32(NAME) Z80_DEF(STRINGIFY(NAME),"4")
#define DEF_u8_ptr(NAME) Z80_DEF(STRINGIFY(NAME),"2")
#define DEF_i8_ptr(NAME) Z80_DEF(STRINGIFY(NAME),"2")
#define DEF_u16_ptr(NAME) Z80_DEF(STRINGIFY(NAME),"2")
#define DEF_i16_ptr(NAME) Z80_DEF(STRINGIFY(NAME),"2")
#define DEF_u32_ptr(NAME) Z80_DEF(STRINGIFY(NAME),"2")
#define DEF_i32_ptr(NAME) Z80_DEF(STRINGIFY(NAME),"2")
#define DEF_u8_array(NAME,SIZE) Z80_DEF(STRINGIFY(NAME),STRINGIFY(SIZE))
#define DEF_i8_array(NAME,SIZE) Z80_DEF(STRINGIFY(NAME),STRINGIFY(SIZE))
#define DEF_u16_array(NAME,SIZE) Z80_DEF(STRINGIFY(NAME),STRINGIFY(SIZE*2))
#define DEF_i16_array(NAME,SIZE) Z80_DEF(STRINGIFY(NAME),STRINGIFY(SIZE*2))
#define DEF_u32_array(NAME,SIZE) Z80_DEF(STRINGIFY(NAME),STRINGIFY(SIZE*4))
#define DEF_i32_array(NAME,SIZE) Z80_DEF(STRINGIFY(NAME),STRINGIFY(SIZE*4))
#else
#define DEF_u8(NAME) u8 NAME;
#define DEF_i8(NAME) i8 NAME;
#define DEF_u16(NAME) u16 NAME;
#define DEF_i16(NAME) i16 NAME;
#define DEF_u32(NAME) u32 NAME;
#define DEF_i32(NAME) i32 NAME;
#define DEF_u8_ptr(NAME) u8* NAME;
#define DEF_i8_ptr(NAME) i8* NAME;
#define DEF_u16_ptr(NAME) u16* NAME;
#define DEF_i16_ptr(NAME) i16* NAME;
#define DEF_u32_ptr(NAME) u32* NAME;
#define DEF_i32_ptr(NAME) i32* NAME;
#define DEF_u8_array(NAME,SIZE) u8 NAME[SIZE];
#define DEF_i8_array(NAME,SIZE) i8 NAME[SIZE];
#define DEF_u16_array(NAME,SIZE) u16 NAME[SIZE];
#define DEF_i16_array(NAME,SIZE) i16 NAME[SIZE];
#define DEF_u32_array(NAME,SIZE) u32 NAME[SIZE];
#define DEF_i32_array(NAME,SIZE) i32 NAME[SIZE];
#endif

// const arrays
#ifdef WINCPCTELERA
#define DEF_u8_array_CONST(NAME,NUMBERS) u8 NAME[] = { NUMBERS };
#define DEF_i8_array_CONST(NAME,NUMBERS) i8 NAME[] = { NUMBERS };
#define DEF_u16_array_CONST(NAME,NUMBERS) u16 NAME[] = { NUMBERS };
#define DEF_i16_array_CONST(NAME,NUMBERS) i16 NAME[] = { NUMBERS };
#else
#define DEF_u8_array_CONST(NAME,NUMBERS) __asm__("_" #NAME "::"); __asm .db NUMBERS __endasm;
#define DEF_i8_array_CONST(NAME,NUMBERS) __asm__("_" #NAME "::"); __asm .db NUMBERS __endasm;
#define DEF_u16_array_CONST(NAME,NUMBERS) __asm__("_" #NAME "::"); __asm .dw NUMBERS __endasm;
#define DEF_i16_array_CONST(NAME,NUMBERS) __asm__("_" #NAME "::"); __asm .dw NUMBERS __endasm;
#endif

#endif // _TEOPL_VARIABLES_H_
