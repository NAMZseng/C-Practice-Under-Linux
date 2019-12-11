#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

// 进程通信，有名管道中信息的写入
int main(int argc, char *argv[])
{
	// 创建一个有名管道
	mkfifo("fifo_znr", 0777);

	// 以只写的方式打开创建的有名管道
	int fd = open("fifo_znr", O_WRONLY);
	
	// 发送消息
	write(fd, "hello fifo", strlen("hello fifo"));
	
	// 关闭文件描述符fd
	close(fd);
	
	return 0;
}