#include <common.h>
//#include <nemu/src/monitor/sdb/sdb.h>

word_t expr(char *e, bool *success);
void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {   

	/* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
	am_init_monitor();
#else
	init_monitor(argc, argv);
#endif  


	char e[65536] = "";
	int num = scanf("%s", e);
	assert(num == 1);
	printf("%s\n", e);

	bool success = true; 
	uint32_t res = expr(e, &success);
	if (success) {
		printf("%u\n", res);
	}
	else {
		printf("表达式不合法！\n");
	}

	return 0;
}
