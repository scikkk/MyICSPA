#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
	char *name;
	size_t size;
	size_t disk_offset;
	ReadFn read;
	WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
	panic("should not reach here");
	return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
	panic("should not reach here");
	return 0;
}





/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
	[FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
	[FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
	[FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};



// wk 3.3--------------------------------------------------

// wk 3.2
// 从ramdisk中`offset`偏移处的`len`字节读入到`buf`中
size_t ramdisk_read(void *buf, size_t offset, size_t len);

// 把`buf`中的`len`字节写入到ramdisk中`offset`偏移处
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

// 返回ramdisk的大小, 单位为字节
size_t get_ramdisk_size();

// wk 3.2
#define FILE_NUM 128
static int open_offset[FILE_NUM] = {0,0,0,0,0,0,0,0,0};

int fs_open(const char *pathname, int flags, int mode){
	int ret = -1;
	while(ret < FILE_NUM && strcmp(file_table[++ret].name, pathname) != 0);
	open_offset[ret] = 0;
	assert(ret < FILE_NUM);
	/* Log("fd: %d\n", ret); */
	return ret;
}

size_t fs_read(int fd, void *buf, size_t len){
assert(fd > 2);
	len = (len > file_table[fd].size-open_offset[fd])? file_table[fd].size-open_offset[fd]:len;
	ramdisk_read(buf, file_table[fd].disk_offset+open_offset[fd], len);

	printf("openoff: %d; size: %d", open_offset[fd], file_table[fd].size);
	open_offset[fd] += len;
	assert(open_offset[fd] <= file_table[fd].size);
	printf("openoff: %d; size: %d", open_offset[fd], file_table[fd].size);
	return len;
}
size_t fs_write(int fd, const void *buf, size_t len){
	assert(fd > 2);
	ramdisk_write(buf, file_table[fd].disk_offset+open_offset[fd], len);
	open_offset[fd] += len;
	assert(open_offset[fd] <= file_table[fd].size);
	return len; 
}


size_t fs_lseek(int fd, size_t offset, int whence){
	/* assert(fd > 2); */	
	switch (whence){
	 	case SEEK_SET:  
			open_offset[fd] = offset;
			break;
		case SEEK_CUR:
			open_offset[fd] += offset;
			break;
		case SEEK_END:
			open_offset[fd] = file_table[fd].size + offset;
			break;
		default: assert(0);
	}
	/* printf("%d\t%d\t%d\n",fd, offset, whence); */
	assert(open_offset[fd] <= file_table[fd].size);
	
	return open_offset[fd];
}
int fs_close(int fd){
	/* assert(fd > 2); */
	return 0;
}
// wk 3.3--------------------------------------------------





void init_fs() {
	// TODO: initialize the size of /dev/fb
}
