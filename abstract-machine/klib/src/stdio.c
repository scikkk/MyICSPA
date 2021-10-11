#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
	panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	panic("Not implemented");
}


int sprintf(char *out, const char *fmt, ...) {
	/* panic("Not implemented"); */
	int ret = 0;
	va_list ap;
	va_start(ap, fmt);
	while(*fmt != '\0'){
		if(*fmt != '%'){
			ret++;
			*out++ = *fmt++;
			*out = '\0';
		}
		else{
			fmt++;
			switch(*fmt){
				case 's':{
							 char *valstr = va_arg(ap, char*);
							 strcat(out, valstr);
							 out += strlen(valstr);
							 ret += strlen(valstr);
							 fmt++;
							 break;
						 }
				case 'd':{
							 int valint = va_arg(ap, int);
							 if(valint < 0){
								*out++ = '-';
								valint = -valint;
							 }
							 char *p = out;
							 char *q = out;
							 do{
								ret++;
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

						 }
				default: {
							 ret++;
							 *out++ = *fmt++;
							 *out = '\0';
						 }
			}
			assert(*out == '\0');
		}

	}
	va_end(ap);
	return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
	panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
	panic("Not implemented");
}

#endif
