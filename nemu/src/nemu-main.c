#include <common.h>
//#include <nemu/src/monitor/sdb/sdb.h>
word_t expr(char *e, bool *success);
int main(int argc, char *argv[]) {
  


  bool *s = (bool*)malloc(sizeof(bool));
  return expr("1+1",s);
}
