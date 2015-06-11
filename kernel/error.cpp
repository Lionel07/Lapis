void GuruMeditation(int component, int device, int resource, int data) {
	printk(LOG_PANIC,"guru: code %d:%d:%d:%d\n", component,device,resource,data);
	Kernel::State::setState(KERNEL_STATE_GURUMEDITATION);
	panic("Guru Meditation");
}