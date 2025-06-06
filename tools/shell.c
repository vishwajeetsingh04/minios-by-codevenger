#include <string.h>
#include <time.h>
#include <asm/io.h>
#include <asm/system.h>
#include <minios/tty.h>
#include <minios/kernel.h>

extern void timer_wait(size_t ticks);

void prompt(void)
{
	printk("\nminios $ ");
}

void shell(void)
{
	prompt();
}

static void reboot(void)
{
	unsigned char good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);
	hlt();
}

static void help(void)
{
	printk("\nMINI shell, version 1.1.12-stable (minios-%s)\n", VERSION);
	printk("These commands are defined internally.\n\n");
	printk(" clear		clear the screen\n");
	printk(" echo		print the given text\n");
	printk(" help		print this help list\n");
	printk(" halt		halt the system\n");
	printk(" reboot		reboot the system\n");
}

void proccmd(char * cmd)
{
	if (strcmp(cmd, "clear") == 0) {
		clrscr();
		gotoxy(0, -1);	/* csr_y = -1 because of "\n" in prompt */
	} else if (strcmp(cmd, "halt") == 0) {
		printk("\nhalting system...\n");
		timer_wait(10);
		printk("\n[NOW]");
		timer_wait(5);
		hlt();
	} else if (strcmp(cmd, "reboot") == 0) {
		printk("\nrebooting...\n");
		timer_wait(10);
		printk("\n[NOW]");
		timer_wait(5);
		reboot();
	} else if (strcmp(cmd, "help") == 0)
		help();
	else if (strcmp(cmd, "echo") == 0) {
		printk("\n");
	} else if (strlen(cmd) > 5 && cmd[0] == 'e' && cmd[1] == 'c' && cmd[2] == 'h' && cmd[3] == 'o' && cmd[4] == ' ') {
		printk("\n%s\n", cmd + 5);
	} else
		printk("\nMINI shell : %s : command not found\n", cmd);
}
