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

	char e[65536];
	for(int k = 0; k < 65535; k++)
	{
		e[k] = '9';e[k+1] = '\0';
	}
	//int num = scanf("%s", e);
	//assert(num == 1);
	FILE *fpread = fopen("/src/input.txt", "r");
	FILE *fpwrite = fopen("/src/wkres.txt", "w");
	assert(fpread != NULL || fpwrite == NULL);
		printf("1:%s\n", e);
	while (fgets(e, sizeof(e), fpread) != NULL) {
		printf("2:%s\n", e);
		bool success = true; 
		uint32_t oneres = expr(e, &success);
		if (success) {
			printf("%u\n", oneres);
			fprintf(fpwrite, "%u\n", oneres);
		} 
		else {
			printf("表达式不合法！\n");
			fprintf(fpwrite, "%s\n", "Invalid!\0");
		} 
	}
	fclose(fpread);
	fclose(fpwrite);
	return 0;
}
