#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


int vsprintf(char *out, const char *fmt, va_list ap) {
	/* panic("Not implemented"); */

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
							 assert(*out=='\0');
							 break;
						 }
				case 'd':{	 int valint = va_arg(ap, int);
							 if (valint==-2147483648){
								 strcat(out,"-2147483648");
								 out += 11;
								 ret += 11;
								 fmt++;
								 break;
							 }
						 if(valint < 0){
								*out++ = '-';
								ret++;
								valint = -valint;
							 }
							 char *p = out;
							 char *q = out;
							 do{ret++;
								*q++ = (char)(valint%10 + '0');
								valint /= 10;
							 }while(valint>0);
							 *q = '\0';
							 out = q;
							 q--;
							 while(q > p){
								char temp = *p;
								*p++ = *q;
								*q-- = temp;
							 }
							 fmt++;
							 break;
						 }
				default: {	 ret++;
							 *out++ = *fmt++;
							 *out = '\0';
						 }
			}
			assert(*out == '\0');
		}
    
	}
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
	putch('w');
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
