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
	q--;
	while(q > p){
		char temp = *p;
		*p++ = *q;
		*q-- = temp;
	}
	return ret;
}

int x2str(char *buf, int num){
	char xtab[] = "0123456789abcdef";
	int ret = 0;
	char *p = buf;
	char *q = buf;
do{
		*q++ = xtab[num%16];
		ret++;
		num /= 16;
	}	while(num > 16);
		*q++ = xtab[num%16];
		ret++;
	*q = '\0';
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

int double2str(char *buf, double flt, int len){
	int ret = 0;
	if ((int)flt < 0){
		ret++;
		*buf++ = '-';
	}
	int flt_int = (int)flt;
	double flt_frac = flt - flt_int;
	int tmp_ret = int2str(buf, flt_int);
	ret += tmp_ret;
	buf += tmp_ret;
	ret ++;
	*buf++ = '.';
	while(len--){
		flt_frac *= 10;
	}
	int flt_frac_int = (int)flt_frac;
	ret += int2str(buf, flt_frac_int);
	return ret;

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
							 int sub_ret = str2str(out, valstr);
							 ret += sub_ret;
							 out += sub_ret;
							 fmt++;
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
						 /* case 'f':{ */	 
						 /* 			 double valflt  = va_arg(ap ,double); */
						 /* 			 int sub_ret = double2str(out, valflt, 6); */
						 /* 			 ret += sub_ret; */
						 /* 			 out += sub_ret; */
						 /* 			 fmt++; */
						 /* 			 /1* *out = '\0'; *1/ */
						 /* 			 break; */
						 /* 		 } */
				case 'x':{	 
							 int val  = va_arg(ap , int);
							 int sub_ret = x2str(out, val);
							 ret += sub_ret;
							 out += sub_ret;
							 fmt++;
							 /* *out = '\0'; */
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
