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
	//int num = scanf("%s", e);
	//assert(num == 1);
	FILE *fpread = fopen("input.txt", "r");
	FILE *fpwrite = fopen("wkres.txt", "w");
	assert(fpread != NULL || fpwrite == NULL);
	int k = 0;
	while (fgets(e, sizeof(e), fpread) != NULL && k < 10) {
		k++;
		char right[100];
		strtok(e, " ");
		strcpy(right, e);
		int len = strlen(e);
		for (int k = 0; k < 65535-len;k++) e[k] = e[k+len+1];
		e[strlen(e)-1] = '\0'; //remove '\n'
		printf("%s\n%s\n", right, e);
		bool success = true; 
		uint32_t oneres = expr(e, &success);
		if (success) {
			printf("%u\n", oneres);
			fprintf(fpwrite, "my: %u\tright: %s\n", oneres, right);
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
