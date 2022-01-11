#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	printf("main argv=%p\n", argv);
	int n = (argc >= 2 ? atoi(argv[1]) : 1);
	printf("wk%s: argv[1] = %d\n", argv[0], n);

	char buf[16];
	sprintf(buf, "%d", n + 1);
	execl(argv[0], argv[0], buf, NULL);
	return 0;
}
