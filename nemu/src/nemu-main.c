
#include <common.h>
//#include <nemu/src/monitor/sdb/sdb.h>

word_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {   
	char e[65536] = "";
	int num = scanf("%s", e);
	assert(num == 1);
	printf("%s\n", e);
	
	bool* success = (bool*)malloc(sizeof(bool));
	*success = true;
	uint32_t res = expr(e,success);
	if (*success) {
		printf("%u\n", res);
	}
	else {
		printf("表达式不合法！\n");
	}
	free(success);
	
	return 0;
}
