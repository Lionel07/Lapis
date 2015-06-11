#include <state.h>
#include <log/printk.h>
#include <panic.h>
int kernel_state_id = KERNEL_STATE_NORMAL;
bool kernel_state_criticalsection = 0;

int Kernel::State::getState() {
	return kernel_state_id;
}

int Kernel::State::setState(int state) {
	if(kernel_state_id == KERNEL_STATE_GURUMEDITATION) {
		printk(LOG_PANIC,"state: Attempted to switch out of Guru Meditation!\n");
		return 0;
	}
	kernel_state_id = state;
	return 1;
}
void GuruMeditation(int component, int device, int resource, int data) {
	printk(LOG_PANIC,"guru: code %d:%d:%d:%d\n", component,device,resource,data);
	Kernel::State::setState(KERNEL_STATE_GURUMEDITATION);
	panic("Guru Meditation");
}