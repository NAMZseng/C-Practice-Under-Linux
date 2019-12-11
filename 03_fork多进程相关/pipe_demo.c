#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

// 进程的创建，进程的无名管道通信
int main(int argc, char *argv[])
{
/* 	printf("进程号：%u\n", getpid());
	printf("父进程号：%u\n", getppid());
	
	// 当进程未加入任何组时，组号即为进程号
	// %u十进制无符号数
	printf("进程组号：%u\n", getpgid(0)); 
 */	
	
/* 	// 进程的创建，父子进程分别从fork()的下一语句开始执行
	 int num = 10;
	 
	 pid_t pid = fork();
	 if(pid < 0)
	 {
		perror("fork");
		_exit(-1);
	 }
	 if(pid == 0) // 子进程
	 {
		// 父子进程中的资源相互独立
		num = 100;
		printf("child pid=%u，num=%d\n",getpid(), num);
		
	 }
	 else if (pid > 0) // 父进程,返回的pid是子进程的id
	 {
		// 阻塞5s
		sleep(5);
		printf("parent pid=%u，num=%d\n",getpid(), num);
	 }
 */	 
 
 	// 创建无名管道
	int fd[2];
	pipe(fd);
	
	pid_t pid = fork();
	if(pid < 0)
	 {
		perror("fork");
		_exit(-1);
	 }
	 if(pid == 0) // 子进程
	 {
		char buf[128] = "";
		// 从管道读信息
		read(fd[0], buf, sizeof(buf));
		printf("child pid=%u，message:%s\n",getpid(), buf);
	 }
	 else if (pid > 0) // 父进程,返回的pid是子进程的id
	 {
		// 阻塞5s
		sleep(5);
		// 向管道中写信息
		write(fd[1], "hello pipe", strlen("hello pipe"));
		printf("parent pid=%u sent message\n", getpid());
	 }

	return 0;
}