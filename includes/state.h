#ifndef KERNEL_STATE_H
#define KERNEL_STATE_H

#define KERNEL_STATE_NORMAL 0
#define KERNEL_STATE_UNINTERRUPTABLE 1
#define KERNEL_STATE_GURUMEDITATION 2

namespace Kernel {
	class State {
		public:
			static int getState();
	};
}
#endif