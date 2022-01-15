#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv) {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  while (1) {
    j ++;
    if (j == 999999) {
		printf("argc=%d\targv[0]=%s\targv[1]=%s\n", argc, argv[0], argv[1]);
		printf("argc=%d\t&argv[0]=%p\t&argv[1]=%p\n", argc, &argv[0], &argv[1]);
      printf("Hello World from Navy-apps for the %dth time!\n", i ++);
      j = 0;
    }
  }

  /* printf("wk\nwk\n"); */
  return 0;
}
