#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

// 随机字符串长度
#define N 50
// 空格作为游戏继续标志
#define CONTINUE 32 
// Esc作死游戏退出标志
#define END 27


void game_help(void);
void create_str(char src_str[N + 1]);
char mygetch(void);
void play_game(char *src_str);

// 一般主函数在文件开头
// 便于通过主函数里的对其他函数的调用了解文件结构
int main(int argc, char *argv[])
{
	while(1)
	{
		// 显示游戏帮助信息
		game_help();

		// 初始化测试的随机字符串
		char src_str[N+1] = {0};

		// 获取生成的随机字符串
		create_str(src_str);
		
		// 按任意键开始游戏
		mygetch();

		// 显示需要随机字符串
		printf("%s\n", src_str);

		// 开始游戏
		play_game(src_str);
		
		char play_flag = 0;
		while(1)
		{
			// 判断游戏是否继续
			play_flag = mygetch();
			if ( play_flag == END)
			{
				return;
			}
			if (play_flag == CONTINUE)
			{
				//清屏
				system("clear");
				break;
			}
		}
	}
	
	return 0;
}

void play_game(char *src_str)
{
	int i = 0;
	int start_time = 0;
	int end_time = 0;
	int currect_type = 0;
	
	for(i = 0; i < N; i++)
	{
		char ch = mygetch();
		if( i == 0) 
		{
			start_time = time(NULL);
		}
		if (ch == src_str[i])
		{
			currect_type++;
			putchar(ch);
		}
		else
		{
			putchar('_');
		}
	}
	end_time = time(NULL);
	
	printf("\n正确率：%.2f %%\n", (float)currect_type / N * 100);
	printf("所用时间为：%ds\n", end_time - start_time);
	printf("按Esc退出，按空格继续\n");
}
// 打印打字游戏的帮助信息
void game_help(void)
{
	printf("*********************************************\n");
	printf("*输入过程中无法退出                         *\n");
	printf("*按所给的字母敲击键盘                       *\n");
	printf("*按任意键开始，按下首字母开始计时           *\n");
	printf("*输入出错以_表示                            *\n");
	printf("*********************************************\n");
	
	return;
}

// 生成打字测试的随机字符串
void create_str(char src_str[N + 1])
{
	srand(time(NULL));
	int i = 0;
	for(i = 0; i<N; i++)
	{
		// 设置当前时间作为随机种子
		src_str[i] = 'a' + rand() % 26;
	}
}

// 获取从键盘输入的字符，不需按回车
char mygetch(void)
{
    struct termios oldt, newt;
    char ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}

