#ifndef __INIT_H__
#define __INIT_H__

#if defined(__GNUC__)

#define __used          __attribute__ ((used))
#define __unused        __attribute__ ((used))
#define __packed        __attirbute__ ((packed))
#define __section(_x_)  __attribute__ ((__section__(_x_)))

#define __likely(_x_)   __builtin_expect (!!(_x_), 1)
#define __unlikely(_x_) __builtin_expect (!!(_x_), 0)

typedef int (*initcall_t)(void);

#define __init          __section (".init.text")
#define __initdata      __section (".init.data")

#define __define_initcall(_cat_, _fn_) \
        static initcall_t __initcall_ ## _fn_ __used \
        __section(".init_" _cat_ ".init") = _fn_

#define core_initcall(fn)   __define_initcall("core", fn)
#define arch_initcall(fn)   __define_initcall("arch", fn)
#define drv_initcall(fn)    __define_initcall("drv", fn)
#define api_initcall(fn)    __define_initcall("api", fn)
#define mw_initcall(fn)     __define_initcall("mw", fn)
#define app_initcall(fn)    __define_initcall("app", fn)

#else   /* !__GNUC__ */

#define __used
#define __unused
#define __packed
#define likely(_x_)     _x_
#define unlikely(_x_)   _x_

#endif

#endif /* __INIT_H__ */
