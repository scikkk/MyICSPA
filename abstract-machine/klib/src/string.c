#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
	/* panic("Not implemented"); */
	assert(s != NULL);
	int len = 0;
	while(*s++ != '\0'){
		len++;
	}
	return len;
}

char *strcpy(char *dst, const char *src) {
	/* panic("Not implemented"); */
	assert(dst != NULL && src != NULL);
	char *ret = dst;
	while((*dst++ = *src++) != '\0');
	return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
	/* panic("Not implemented"); */
	char *temp = dst;
	while(n && ((*temp++ = *src++) != '\0')){
		n--;
	}
	if (n){
		while(--n){
			*temp++ = '\0';
		}
	}
	return dst;
}
char *strcat(char *dst, const char *src) {
	/* panic("Not implemented"); */
	assert((dst != NULL) && (src != NULL));
	char *temp = dst;
	while(*temp){
		temp++;
	}
	while((*temp++ = *src++) != '\0');
	return dst;
}

int strcmp(const char *s1, const char *s2) {
	/* panic("Not implemented"); */
	assert((s1 != NULL) && (s2 != NULL));
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
	assert((s1 != NULL) && (s2 != NULL));
	if(n == 0) return 0;
	while(--n && *s1 && (*s1 == *s2)){
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

void *memset(void *s, int c, size_t n) {
	/* panic("Not implemented"); */
	if (s == NULL || n < 0){
		return NULL;
	}
	char *tmps = (char*)s;
	while(n-- > 0){
		*tmps++ = c;
	}
	return s;
}

void *memmove(void *dst, const void *src, size_t n) {
	/* panic("Not implemented"); */
	void *ret = dst;
	while(n--){
		*(char*)dst = *(char*)src;
		dst = (char*)dst + 1;
		src = (char*)src + 1;
	}
	return ret;
}

void *memcpy(void *out, const void *in, size_t n) {
	/* panic("Not implemented"); */
	if (n < 0 || out == NULL || in == NULL){
		return NULL;
	}
	char *tempout = (char*)out;
	char *tempin = (char*)in;
	while(n-- > 0){
		*tempout++ = *tempin++;
	}
	return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	/* panic("Not implemented"); */
	assert(s1 && s2);
	if(n == 0) return 0;
	while(--n && (*(char*)s1 == *(char*)s2)){
			s1 = (char*)s1 + 1;
			s2 = (char*)s2 + 1;
		}
	
	return (*((unsigned char *)s1) - *((unsigned char *)s2));
}

#endif
