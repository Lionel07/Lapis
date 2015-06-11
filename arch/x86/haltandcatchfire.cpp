void halt_and_catch_fire() {
	for(;;) {
		asm("cli");
		asm("pause");
		asm("hlt");
	}
}