#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

// wk: generate a random int
uint32_t choose(uint32_t up_bound) {
	return (uint32_t)rand()%up_bound;
}
// wk: generate a random int

// wk: add 
void add_space() {
	int len = choose(10);
	for (int k = 0; k < len; k++ ) strcat(buf, " \0");
}
void add_num() {
	char num[5];
	sprintf(num,"%u",choose(1000));
	strcat(buf, num);
}
void add_op() {
	switch (choose(4)) {
		case 0:strcat(buf,"+\0"); break;
		case 1:strcat(buf,"-\0"); break;
		case 2:strcat(buf,"*\0"); break;
		case 3:strcat(buf,"/\0"); break;
	}
}
// wk:add 

static void gen_rand_expr() {
	//  buf[0] = '\0';
	//  wk
	// switch (choose(3)) {
	// case 0: gen_num(); break;
	// case 1: gen('('); gen_rand_expr(); gen(')'); break;
	// default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
	// }

	if (strlen(buf) > 60000) {
		add_num();
		return;
	}
	switch (choose(3)) { 
		case 0: add_num(); break;
		case 1:strcat(buf,"(\0");
			   add_space();
			   gen_rand_expr();
			   add_space();
			   strcat(buf,")\0");
			   break;
		case 2: gen_rand_expr();
				add_space();add_op();add_space();
				gen_rand_expr();
				break;
	}
	//  wk
}

int main(int argc, char *argv[]) {
	int seed = time(0);
	srand(seed);
	int loop = 1;
	if (argc > 1) {
		sscanf(argv[1], "%d", &loop);
	}
	int i;
	for (i = 0; i < loop; i ++) {
		// wk add
		buf[0] = '\0';
		// wk add
		gen_rand_expr();
		// 格式化输入到code_buf
		sprintf(code_buf, code_format, buf);
		// 将code_buf写入文件
		FILE *fp = fopen("/tmp/.code.c", "w");
		assert(fp != NULL);
		fputs(code_buf, fp);
		fclose(fp);
		// 编译刚刚创建的C文件
		int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
		if (ret != 0) continue;
		// 运行编译后的可执行文件，获取输出
		fp = popen("/tmp/.expr", "r");
		assert(fp != NULL);
		// 将输出转为整数，存入result
		int result;
		// edited by wk
		// old: fscanf(fp, "%d", &result);
		int nums = fscanf(fp, "%d", &result);
		assert(nums != 0);
		// deited by wk
		pclose(fp);
		// 打印结果
		printf("%u %s\n", result, buf);
	}
	return 0;
}
