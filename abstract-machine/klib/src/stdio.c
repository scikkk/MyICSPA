#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int int2str(char *buf, int num){
	if (num == -2147483648){
		strcat(buf, "-2147483648");
		return 11;
	}	
	int ret = 0;
	if(num < 0){
		*buf++ = '-';
		ret++;
		num = -num;
	}
	char *p = buf;
	char *q = buf;
	do{ret++;
		*q++ = (char)(num%10 + '0');
		num /= 10;
	}while(num>0);
	*q = '\0';
	buf = q;
	q--;
	while(q > p){
		char temp = *p;
		*p++ = *q;
		*q-- = temp;
	}
	return ret;
}

int str2str(char *buf, char *src){

	strcat(buf, src);
	return strlen(src);
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	/* panic("Not implemented"); */

	*out = '\0';
	int ret = 0;
	while(*fmt != '\0'){
		if(*fmt != '%'){
			ret++;
			*out++ = *fmt++;
			*out = '\0';
		}
		else{
			fmt++;
			switch(*fmt){
				case 's':{	 char *valstr = va_arg(ap, char*);
							 strcat(out, valstr);
							 out += strlen(valstr);
							 ret += strlen(valstr);
							 fmt++;
							 /* putstr(out); */
							 assert(*out=='\0');
							 break;
						 }
				case 'd':{	 int valint = va_arg(ap, int);
							 int sub_ret = int2str(out, valint);
							 ret += sub_ret;
							 out += sub_ret;
							 fmt++;
							 break;
						 }
				default: {	 ret++;
							 *out++ = *fmt++;
							 *out = '\0';
						 }
			}
		}

		assert(*out == '\0');
	}
	assert(*out == '\0');
	return ret;
}


int sprintf(char *out, const char *fmt, ...) {
	/* panic("Not implemented"); */
	va_list ap;
	va_start(ap, fmt);
	int ret = vsprintf(out, fmt, ap);
	va_end(ap);
	return ret;
}


int printf(const char *fmt, ...) {
	/* panic("Not implemented"); */
	char out[999];
	va_list ap;
	va_start(ap, fmt);
	int ret = vsprintf(out, fmt, ap);
	va_end(ap);
	/* putch('A'); */
	/* for(int k = 0; k < 20; k++){ */
	/* putch(out[k]); */
	/* putch('\n'); */
	/* } */
	putstr(out);
	return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
	panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
	panic("Not implemented");
}

#endif
