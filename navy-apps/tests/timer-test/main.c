#include <stdint.h>
#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>


#define SYS_gettimeofday 19
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);



void hello_world(void)
{
    printf("Hello world!!!!\r\n");
}

int main(void)
{

        struct  timeval   tv_begin,tv_end;
        
//        gettimeofday(&tv_begin,NULL);
		_syscall_(SYS_gettimeofday, (unsigned long)&tv_begin, 0, 0);
        
        hello_world();

//        gettimeofday(&tv_end,NULL);
		_syscall_(SYS_gettimeofday, (unsigned long)&tv_begin, 0, 0);
    
        printf("tv_begin_sec:%ld\n",tv_begin.tv_sec);

        printf("tv_begin_usec:%ld\n",tv_begin.tv_usec);

        printf("tv_end_sec:%ld\n",tv_end.tv_sec);

        printf("tv_end_usec:%ld\n",tv_end.tv_usec);

        return 0;

}
