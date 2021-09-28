#ifndef _COMPAT_H_
#define _COMPAT_H_

#define __iomem

#define msleep(x)	udelay(1000*x)

#define strlcpy		strncpy
#define dev_err(dev, str, ...) printf(str, ##__VA_ARGS__)
#define dev_dbg(dev, str, ...) printf(str, ##__VA_ARGS__)
#define dev_warn(dev, condition, str, ...) printf(str, ##__VA_ARGS__)
#define dev_WARN_ONCE(dev, condition, str, ...) printf(str, ##__VA_ARGS__)
#define pr_err printf
#define pr_dbg printf

#define KERN_SOH	"\001"		/* ASCII Start Of Header */
#define KERN_ERR	KERN_SOH "3"	/* error conditions */

#define GFP_KERNEL			0

#define kmalloc(size, flags)	malloc(size)
#define kzalloc(size, flags)	calloc(size, 1)
#define kfree(ptr)		free(ptr)
#define devm_kzalloc(dev, size, flags)	calloc(size, 1)

/*
 * ..and if you can't take the strict
 * types, you can specify one yourself.
 *
 * Or not use min/max at all, of course.
 */
#define min_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define max_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

/* Force a compilation error if condition is true */
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#ifndef BUG
#define BUG() do { \
	printf("U-Boot BUG at %s:%d!\n", __FILE__, __LINE__); \
} while (0)

#define BUG_ON(condition) do { if (condition) BUG(); } while(0)
#endif /* BUG */

#ifndef WARN_ON
#define WARN_ON(condition) do { if (condition) printf("U-Boot WARN at %s:%d!\n", __FILE__, __LINE__);  } while(0)
#endif /* WARN_ON */

#define true                        1
#define false                       0

#define PAGE_SIZE	4096

struct regulator { int i; };
typedef unsigned int bool;
typedef u32 unicode_t;
typedef u64 async_cookie_t;

#define BITS_PER_LONG 32

struct device { int i; };
struct platform_device { 	struct device dev; };
typedef int	wait_queue_head_t;
struct completion {
	unsigned int done;
	wait_queue_head_t wait;
};
struct mutex { int i; };
struct clk { int i; };
struct work_struct { int i; };
struct workqueue_struct { int i; };
struct delayed_work {
	struct work_struct work;
	struct workqueue_struct *wq;
};
struct device_attribute { int i; };
union ktime { int i; };
typedef union ktime ktime_t;
struct devfreq { int i; };
enum irqreturn {
	IRQ_NONE		= (0 << 0),
	IRQ_HANDLED		= (1 << 0),
	IRQ_WAKE_THREAD		= (1 << 1),
};
typedef enum irqreturn irqreturn_t;
enum dma_data_direction {
	DMA_BIDIRECTIONAL	= 0,
	DMA_TO_DEVICE		= 1,
	DMA_FROM_DEVICE		= 2,
	DMA_NONE			= 3
};

#define mutex_init(...)
#define mutex_lock(...)
#define mutex_unlock(...)
#define spin_lock_irqsave(lock, flags)		flags=0
#define spin_unlock_irqrestore(lock, flags)	
#define init_completion(...)
#define wake_up(...)
#define pm_runtime_get_sync(...)
#define pm_runtime_put_sync(...)
#define INIT_WORK(...)
#define init_waitqueue_head(...)	do { } while (0)

#define UFS_MASK(mask, offset)		((mask) << (offset))
#define BLK_MAX_CDB	16
struct request {
	int tag;
	int errors;
	void *sense;
};
struct scsi_driver { int i; };
struct scsi_device { u64 lun; };
struct scsi_pointer { int i; };
struct Scsi_Host { int i; };
#define SG_ALL	SCSI_MAX_SG_SEGMENTS

#define module_platform_driver(...)
#define EXPORT_SYMBOL(...)
#define EXPORT_SYMBOL_GPL(...)
#define module_param_call(...)
#define MODULE_PARM_DESC(...)
#define MODULE_VERSION(...)
#define MODULE_DESCRIPTION(...)
#define MODULE_AUTHOR(...)
#define MODULE_LICENSE(...)

#define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((u32)(n))
/*
static __inline__ void __set_bit(int nr, volatile void * addr)
{
	unsigned long * m = ((unsigned long *) addr) + (nr >> 5);

	*m |= 1UL << (nr & 31);
}

static __inline__ void __clear_bit(int nr, volatile unsigned long *addr)
{
	unsigned long * m = ((unsigned long *) addr) + (nr >> 5);

	*m &= ~(1UL << (nr & 31));
}
*/
#define clear_bit					__clear_bit
#define clear_bit_unlock			__clear_bit

/*
static __inline__ int test_and_set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long old;

	old = (*addr);
	(*addr) |= (1<<nr);

	return (old & (1<<nr)) != 0;
}
*/
#define test_and_set_bit_lock(x, y)	test_and_set_bit(x, y)

static __inline__ unsigned long find_last_bit(const unsigned long *addr, unsigned long size)
{
	long idx;

	if(size) {
		for (idx=(size-1) ; idx>0 ; idx--)
			if(((*addr)>>idx)&0x1)
				return idx;
	}
	return size;
}

static __inline__ unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
			    unsigned long offset)
{
	unsigned long idx;

	for (idx=offset ; idx<size ; idx++)
		if(((*addr)>>idx)&0x1)
			break;

	return idx;
}
#define find_first_bit(addr, size) find_next_bit((addr), (size), 0)
#define for_each_set_bit(bit, addr, size) \
	for ((bit) = find_first_bit((addr), (size));		\
	     (bit) < (size);					\
	     (bit) = find_next_bit((addr), (size), (bit) + 1))

#define ENOTSUPP	524	/* Operation is not supported */

#ifndef BIT0
#define BIT0    (1<<0)
#endif
#ifndef BIT1
#define BIT1    (1<<1)
#endif
#ifndef BIT2
#define BIT2    (1<<2)
#endif
#ifndef BIT3
#define BIT3    (1<<3)
#endif
#ifndef BIT4
#define BIT4    (1<<4)
#endif
#ifndef BIT5
#define BIT5    (1<<5)
#endif
#ifndef BIT6
#define BIT6    (1<<6)
#endif
#ifndef BIT7
#define BIT7    (1<<7)
#endif
#ifndef BIT8
#define BIT8    (1<<8)
#endif
#ifndef BIT9
#define BIT9    (1<<9)
#endif
#ifndef BIT10
#define BIT10   (1<<10)
#endif
#ifndef BIT11
#define BIT11   (1<<11)
#endif
#ifndef BIT12
#define BIT12   (1<<12)
#endif
#ifndef BIT13
#define BIT13   (1<<13)
#endif
#ifndef BIT14
#define BIT14   (1<<14)
#endif
#ifndef BIT15
#define BIT15   (1<<15)
#endif
#endif
