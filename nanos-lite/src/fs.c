#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
	char *name;
	size_t size;
	size_t disk_offset;
	ReadFn read;
	WriteFn write;
	size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENTS, FD_DISPINFO, FD_FB, FD_NORMAL};

size_t invalid_read(void *buf, size_t offset, size_t len) {
	panic("should not reach here");
	return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
	panic("should not reach here");
	return 0;
}


// wk 3.3
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
// wk 3.3


/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
	[FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write, 0},
	[FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write, 0},
	[FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write, 0},
	[FD_EVENTS] = {"/dev/events", 0, 0, events_read, invalid_write, 0},
	[FD_DISPINFO] = {"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write, 0},
	[FD_FB] = {"/dev/fb", 0, 0, invalid_read, fb_write, 0},
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

int fs_open(const char *pathname, int flags, int mode){
	/* if(strcmp("/dev/events", pathname)&&strcmp("/dev/fb", pathname)) printf("fs.c:open %s\n", pathname); */
	int ret = -1;
	while(ret < FILE_NUM && strcmp(file_table[++ret].name, pathname) != 0){
		if(file_table[ret + 1].name==NULL){
		return -2;
		}
	};
//		if(strcmp("/dev/events", pathname)&&strcmp("/dev/fb", pathname)) printf("%s == %s\n", pathname, file_table[ret].name);
	file_table[ret].open_offset = 0;
	if(ret >= FD_NORMAL){
		file_table[ret].read = ramdisk_read;
		file_table[ret].write = ramdisk_write;
	}
	assert(ret < FILE_NUM);
	/* Log("fd: %d\n", ret); */
	return ret;
}

size_t fs_read(int fd, void *buf, size_t len){
	assert(fd > 2);
	if (fd == FD_EVENTS){
		return file_table[fd].read(buf, 0, len);
	}
	if(fd >= FD_NORMAL && (file_table[fd].open_offset >= file_table[fd].size || len == 0))
		return 0;
	if((fd >= FD_NORMAL) && file_table[fd].open_offset + len > file_table[fd].size)
		len = file_table[fd].size - file_table[fd].open_offset;
	file_table[fd].read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
	file_table[fd].open_offset += len;
	/* printf("%d: offset: %d\n", __LINE__, file_table[fd].open_offset); */
	return len;
}
size_t fs_write(int fd, const void *buf, size_t len){
	/* assert(fd > 2); */
	/* printf("%d: offset: %d\n", __LINE__, file_table[fd].open_offset); */
	if(fd >= FD_NORMAL && file_table[fd].open_offset >= file_table[fd].size)
		return 0;
	if(fd >= FD_NORMAL && file_table[fd].open_offset + len > file_table[fd].size)
		len = file_table[fd].size - file_table[fd].open_offset;
	file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
	file_table[fd].open_offset += len;
	/* printf("%d: offset: %d\n", __LINE__, file_table[fd].open_offset); */
	return len; 
}

size_t fs_lseek(int fd, size_t offset, int whence){
	/* printf("%d: offset: %d\n", __LINE__, file_table[fd].open_offset); */
	switch (whence){
		case SEEK_SET:  
			file_table[fd].open_offset = offset;
			break; 
		case SEEK_CUR:
			file_table[fd].open_offset += offset;
			break; 
		case SEEK_END:
			file_table[fd].open_offset = file_table[fd].size + offset;
			break;
		default: assert(0);
	}
	/* printf("%d\t%d\t%d\n",fd, offset, whence); */
	if(offset > file_table[fd].size){
		file_table[fd].open_offset = file_table[fd].size;
	}
	if(offset < 0){
		file_table[fd].open_offset = 0;
	}
	/* printf("%d: offset: %d\n", __LINE__, file_table[fd].open_offset); */

	return file_table[fd].open_offset;
}
int fs_close(int fd){
	/* assert(fd > 2); */
	return 0;
}
// wk 3.3--------------------------------------------------





void init_fs() {
	// TODO: initialize the size of /dev/fb
	file_table[FD_FB].size = 4*io_read(AM_GPU_CONFIG).width*io_read(AM_GPU_CONFIG).height;
	/* printf("size:%d\n", file_table[FD_FB].size); */
	/* assert(0); */
}
