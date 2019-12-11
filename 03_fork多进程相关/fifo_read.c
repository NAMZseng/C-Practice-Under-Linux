#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>


// 进程通信，有名管道中信息的读取
int main(int argc, char *argv[])
{
	// 以自读的方式打开创建的有名管道
	int fd = open("fifo_znr", O_RDONLY);
	
	char buf[128] = "";
	read(fd, buf, sizeof(buf));
	printf("读取到的数据为：%s\n", buf);
	
	// 关闭文件描述符fd
	close(fd);
	
	return 0;
}