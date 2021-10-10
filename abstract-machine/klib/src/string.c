#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  /* panic("Not implemented"); */
	assert((NULL != s1) && (NULL != s2));
  while((*s1) && (*s1 == *s2)){
	s1++;
	s2++;
  }
  if (*(unsigned char*)s1 > *(unsigned char*)s2){
	return 1;
  }
  else if(*(unsigned char*)s1 > *(unsigned char*)s2){
	return -1;
  }
  else {
	return 0;
  }
}

int strncmp(const char *s1, const char *s2, size_t n) {
  /* panic("Not implemented"); */
	assert((NULL != s1) && (NULL != s2));
	if(n == 0) return 0;
	while(n-- && *s1 && (*s1 == *s2)){
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

void *memset(void *s, int c, size_t n) {
  panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  panic("Not implemented");
}

#endif
