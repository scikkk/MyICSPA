#include <stdint.h>
#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>

#include<NDL.h>
#define SYS_gettimeofday 19
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);



void hello_world(void)
{
	printf("Hello world!!!!\r\n");
}

int main(void)
{

	struct  timeval   tv_begin,tv_end;
	/* struct timeval	tv_cur; */

	/* //        gettimeofday(&tv_begin,NULL); */
	/* _syscall_(SYS_gettimeofday, (unsigned long)&tv_begin, 0, 0); */

	/* hello_world(); */

	/* //        gettimeofday(&tv_end,NULL); */
	/* _syscall_(SYS_gettimeofday, (unsigned long)&tv_begin, 0, 0); */

	/* printf("tv_begin_sec:%ld\n",tv_begin.tv_sec); */

	/* printf("tv_begin_usec:%ld\n",tv_begin.tv_usec); */

	/* printf("tv_end_sec:%ld\n",tv_end.tv_sec); */

	/* printf("tv_end_usec:%ld\n",tv_end.tv_usec); */





	NDL_Init(0);
	unsigned msec = 0;
	uint32_t ms;
	while (msec<10000) {
		while(1){ 
			ms  = NDL_GetTicks();
			/* _syscall_(SYS_gettimeofday, (unsigned long)&tv_cur, 0, 0); */
			if(ms > msec) break;
		} 

		/* printf("(sec.usec): %lu.%lu\n",tv_cur.tv_sec, tv_cur.tv_usec); */
		/* printf("ms:%u\n",ms); */

		printf("%d mseconds).\n", msec);
		msec += 100;
	}
	return 0;

}
