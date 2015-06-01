#ifndef KERNEL_MEMORY_PMM_H
#define KERNEL_MEMORY_PMM_H
#include <stddef.h>
#include <stdint.h>

#define BUDDY_BITMAPS 5
#define BUDDY_STARTSIZE 12
namespace Kernel {
	class PMM {
	public:
		static uintptr_t memsize;
		static uintptr_t buddy_usedpages;
		static uintptr_t * buddy_startpage;
	public:
		static void refreshCache();
		static void init();
		static int allocate(int pages);
		static int free(uintptr_t * address);
	};
}


#endif