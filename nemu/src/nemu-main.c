#include <common.h>
//#include <nemu/src/monitor/sdb/sdb.h>


word_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {
  bool *s = (bool*)malloc(sizeof(bool));
  *s = true;
  uint32_t res =  expr("1+1\0",s);
  printf("%u\n", res);
  free(s);
  return 0;
}
