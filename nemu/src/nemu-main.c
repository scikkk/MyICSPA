#include <common.h>
//#include <nemu/src/monitor/sdb/sdb.h>


word_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {
  bool *s = (bool*)malloc(sizeof(bool));
  *s = true;
  char e[65536];
  int num = scanf("%s", e);
  assert(num == 1);
  uint32_t res =  expr(e,s);
  printf("%u\n", res);
  free(s);
  return 0;
}
