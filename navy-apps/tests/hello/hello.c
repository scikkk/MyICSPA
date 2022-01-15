#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv, char **envp) {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  while (1) {
    j ++;
    if (j == 999999) {
		for(int k = 0; k <= argc; k++)
		printf("argc=%d\targv[%d]=%s\n", argc, k, argv[k]);
		for(int k = 0; envp[k]; k++)
		printf("envp[%d]=%s\n", k, envp[k]);
      printf("Hello World from Navy-apps for the %dth time!\n", i ++);
      j = 0;
    }
  }

  /* printf("wk\nwk\n"); */
  return 0;
}
