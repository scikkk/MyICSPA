#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

// wk
#include <memory/vaddr.h>
// wk

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

// wk add
void new_wp(const char* expr,const char* type);
void free_wp(int wp_no);
void wp_display();
// wk add

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}


static int cmd_q(char *args) {
	// wk add	
	nemu_state.state = NEMU_QUIT;
	// wk add
	return -1;

}

static int cmd_help(char *args);


// wk: execute by step
static int cmd_si(char *args) {
	char* len_of_step_str = strtok(NULL, " ");
	int len_of_step = 1;
	if (len_of_step_str != NULL) {len_of_step = atoi(len_of_step_str);}
	cpu_exec(len_of_step);
	return 0;
}
// wk: execute by step

// wk: print registers
static int cmd_info(char *args){
	char* parameter = strtok(NULL, " ");
	if (parameter == NULL) {printf("Bad command!\nUse \"info r\" or \"info w\".\n");return 0;}
	if (strcmp(parameter, "r") == 0){
		isa_reg_display();
	}
	else if (strcmp(parameter, "w") == 0){
		wp_display();
	}
	else{ printf("Bad command!\nUse \"info r\" or \"info w\".\n");}
	return 0;
}
// wk: print registers

// wk: scan memory
static int cmd_x(char *args){
	int N = atoi(strtok(NULL, " "));
	char *expr = strtok(NULL, " ");
	char* next;
	long value_of_expr = strtol(expr, &next, 16);
	for (int k = 0; k < N; k++) {
		printf("0x%08lx:\t0x%08x\n",value_of_expr+4*k , vaddr_read(value_of_expr+4*k, 4));
	}

	return 0;
}
// wk: scan memory

// wk: evaluate expressions
static int cmd_p(char *args) {
	char *e = args;
	bool* success = (bool*)malloc(sizeof(bool));
	*success = true;
	uint32_t res = expr(e,success);
	if (*success) {
		printf("%u\n", res);
	}
	else {
		printf("Invalid expression！\n");
	}
	free(success);
	return 0;
}
// wk: evaluate expressions


// wk: watchpointers
static int cmd_w(char *args) {
	char *e = args;
	new_wp(e,"Watchpoint");
	return 0;
}
static int cmd_d(char *args) {
	char* num_s = strtok(NULL," ");
	if (num_s == NULL) free_wp(-1);
	while(num_s != NULL)  {	
		int num = atoi(num_s);
		free_wp(num);
		num_s = strtok(NULL, " ");
	}
	return 0;
}

static int cmd_b(char *args) {
	char *e = args;
	new_wp(e,"Breakpoint");
	return 0;

}
// wk: watchpointers

static struct {
	const char *name;
	const char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	/* TODO: Add more commands */
	{ "si", "Let the program step through N instructions and then pause execution\nUsage: si [N] \nWhen N is not given, it defaults to 1\n", cmd_si}, //wk
	{"info", "Print register status", cmd_info}, //wk
	{"x", "Examine memory: x [N] ADDRESS.\n Use the ADDRESS as the starting memory address, and output N consecutive 4 bytes in hexadecimal form\n", cmd_x},
	{"p", "Print value of expression EXPR.\nUsage: p [EXPR]\n", cmd_p},
	{"w", "Set a watchpoint for an expression.\nUsage: w EXPRESSION\nA watchpoint stops execution of your program whenever the value ofan expression changes.\n", cmd_w},
		{"b", "Set a breakpoint for an expression.\nUsage: b EXPRESSION\nA breakpoint stops execution of your program whenever the value ofan expression changes.\n", cmd_b},
{"d", "Delete all or some watchpoints.\nUsage: delete [WATCHPOINTNUM]...\nArguments are watchpoint numbers with spaces in between.\nTo delete all watchpoints, give no argument. \n", cmd_d},
};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if (arg == NULL) {
		/* no argument given */
		for (i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for (i = 0; i < NR_CMD; i ++) {
			if (strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void sdb_set_batch_mode() {
	is_batch_mode = true;
}

void sdb_mainloop() {
	if (is_batch_mode) {
		cmd_c(NULL);
		return;
	}

	for (char *str; (str = rl_gets()) != NULL; ) {
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if (cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if (args >= str_end) {
			args = NULL;
		}

#ifdef CONFIG_DEVICE
		extern void sdl_clear_event_queue();
		sdl_clear_event_queue();
#endif

		int i;
		for (i = 0; i < NR_CMD; i ++) {
			if (strcmp(cmd, cmd_table[i].name) == 0) {
				if (cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}

void init_sdb() {
	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint pool. */
	init_wp_pool();


	// wk add for debug
	/* new_wp("$pc", "Watchpoint"); */
	// wk 3
}
