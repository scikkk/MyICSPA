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
	while (fgets(e, sizeof(e), fpread) != NULL && k < 999) {
		k++;
		char right[100],oneres[100];
		strtok(e, " ");
		strcpy(right, e);
		int len = strlen(e);
		for (int k = 0; k < 65535-len;k++) e[k] = e[k+len+1];
		e[strlen(e)-1] = '\0'; //remove '\n'
		bool success = true; 
		uint32_t oneres_uint = expr(e, &success);
		sprintf(oneres,"%u",oneres_uint);
		if (success) {
			if( strcmp(oneres, right)!=0) 
				fprintf(fpwrite, "Line %d: %-10s\tright: %-10s\tlen: %ld\n",k, oneres, right,strlen(e));
		} 
		else { 
			fprintf(fpwrite, "Line %d: Invalid!\tright: %-10s\tlen: %ld\n",k,right,strlen(e));
		} 
	}
	fclose(fpread);
	fclose(fpwrite);
	return 0;
}
