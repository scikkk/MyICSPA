#include <stdio.h>
#include <assert.h>

int main() {
  FILE *fp = fopen("/share/files/num", "r+");
  assert(fp);

  /* fseek(fp, 0, SEEK_END); */
  /* long size = ftell(fp); */
  /* printf("%d\n", size); */
  /* assert(size == 5000); */

  /* fseek(fp, 500 * 5, SEEK_SET); */
  int i, n;
  /* for (i = 500; i < 1000; i ++) { */
  /*   fscanf(fp, "%d", &n); */
	/* printf("i=%d\tn=%d\n", i, n); */
    /* assert(n == i + 1); */
  /* } */

  fseek(fp, 0, SEEK_SET);
  /* size = ftell(fp); */
  /* printf("size: %d\n", size); */
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }

  
   int size = ftell(fp); 
   printf("size: %d\n", size); 
  for (i = 500; i < 502; i ++) { // 1000
    fscanf(fp, "%d", &n);
	printf("i=%d\tn=%d\n", i, n);
    assert(n == i + 1);
  }
/*
  fseek(fp, 0, SEEK_SET);
   size = ftell(fp); 
   printf("size: %d\n", size); 
  for (i = 0; i < 500; i ++) {
    fscanf(fp, "%d", &n);
	 printf("i=%d\tn=%d\n", i, n); 
    assert(n == i + 1 + 1000);
  }
*/
  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
