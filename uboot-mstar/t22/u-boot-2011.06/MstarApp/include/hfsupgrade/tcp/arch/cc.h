/* Revise this base on lwIP official site (http://lwip.wikia.com/wiki/Porting_For_Bare_Metal) */
#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

#define BYTE_ORDER LITTLE_ENDIAN

typedef unsigned char     u8_t;
typedef signed char       s8_t;
typedef unsigned short    u16_t;
typedef signed short      s16_t;
typedef unsigned int      u32_t;
typedef signed int        s32_t;

typedef u32_t             mem_ptr_t;

#define LWIP_ERR_T  int

/* Define (sn)printf formatters for these lwIP types */
#define U16_F "hu"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"

/* Compiler hints for packing structures */
#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT  __attribute__((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

#define LWIP_PLATFORM_DIAG(x) //printf x

#define LWIP_PLATFORM_ASSERT(x) //printf("[assert]:"x"\n");

#endif /* __ARCH_CC_H__ */
