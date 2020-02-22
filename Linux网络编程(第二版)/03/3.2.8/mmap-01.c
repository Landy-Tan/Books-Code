/*文件mmap-01.c,使用mmap对文件进行操作*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>/*mmap*/
#include <string.h>/*memset warning*/
#include <stdio.h>
#define FILELENGTH 80
int main(void)
{
	int fd = -1;	
	char buf[]="quick brown fox jumps over the lazy dog";																	/*将要写入文件的字符串*/
	char *ptr = NULL;
	
	/*打开文件mmap.txt，并将文件长度缩小为0，
	如果文件不存在则创建它，权限为可读写执行*/
	fd = open("mmap.txt", O_RDWR/*可读写*/|O_CREAT/*不存在，创建*/|O_TRUNC/*缩小为0*/, S_IRWXU);	
	if( -1 == fd){ 								/*打开文件失败，退出*/
		return -1;
	}	
	/*下面的代码将文件的长度扩大为80*/
	/*向后偏移文件的偏移量到79*/
	lseek(fd, FILELENGTH-1, SEEK_SET);
	
	write(fd, "a", 1); 							/*随意写入一个字符，此时文件的													 长度为80*/
	
	/*将文件mmap.txt中的数据段从开头到1M的数据映射到内存中，对文件的操作立刻显示在文
		件上，可读写*/
	ptr = (char*)mmap(NULL, FILELENGTH, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	if( (char*)-1 == ptr){ 						/*如果映射失败，则退出*/	
		printf("mmap failure\n");	
		close(fd);
		return -1;
	} 
	
	memcpy(ptr+16, buf, strlen(buf)); 			/*将buf中的字符串复制到映射区															 域中，起始地址为ptr偏移16*/
	munmap(ptr, FILELENGTH); 						/*取消文件映射关系*/ 
	close(fd); 									/*关闭文件*/
 
	return 0;
}
