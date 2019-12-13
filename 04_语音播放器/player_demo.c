#include <gtk/gtk.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MUSIC_PATH "./music/"

typedef struct
{
    GtkWidget *label_music;
    GtkWidget *progress_bar;
    GtkWidget *btn_pre_song;
    GtkWidget *btn_play_stop;
    GtkWidget *btn_next_song;

} MusicButton;

char music_list[100][100]; // 音乐名称列表
int playing_index = -1; // 正在播放的音乐的索引
int music_num = 0; // 音乐总数
int fifo_fd = -1; // 有名管道的文件描述符
int pip_fd[2]; // 无名管道文件描述符
int mplayer_pid = -1; // 子进程pid
int flag_ask_progress = 0; // 是否开启播放进程查询

void get_music_list();
void deal_play(GtkWidget *btn, gpointer data);
void deal_play_pre(GtkWidget *btn, gpointer data);
void deal_play_next(GtkWidget *btn, gpointer data);
void my_main_quit(gpointer data);
void *ask_progress(void *arg);
void *receive_progress(void *arg);
void *receive_serial(void *arg);

// gtk播放器demo
int main(int argc, char *argv[])
{
    pipe(pip_fd);

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork");
        _exit(-1);
    }
    if(pid == 0) // 子进程
    {
        mkfifo("fifo_znr_player", 0777);

        // 重定义mplayer输出到无名管道中
        dup2(pip_fd[1], 1);

        // 以slave模式启动播放器，idle：完整播放文件后不自动退出
        // input: 重定向读取管道路径
        execlp("mplayer", "mplayer", "-slave", "-quiet", "-idle", "-input",
               "file=./fifo_znr_player", NULL);

    }
    else if(pid > 0) // 父进程
    {
        mplayer_pid = pid;

        mkfifo("fifo_znr_player", 0777);
        fifo_fd = open("fifo_znr_player", O_WRONLY);

        // 获取最新播放列表
        get_music_list();

        // gtk界面
        gtk_init(&argc, &argv);

        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "语音播放器");
        gtk_window_set_default_size(GTK_WINDOW(window), 400, 240);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

        // 创建表格布局
        GtkWidget *table = gtk_table_new(4, 3, TRUE);
        gtk_container_add(GTK_CONTAINER(window), table);

        MusicButton music_button;

        music_button.label_music = gtk_label_new("语音播放器");
        gtk_table_attach_defaults(GTK_TABLE(table), music_button.label_music, 0, 3, 0, 1);

        music_button.progress_bar = gtk_progress_bar_new();
        gtk_progress_bar_set_text(GTK_PROGRESS_BAR(music_button.progress_bar), "播放进度");
        gtk_table_attach_defaults(GTK_TABLE(table), music_button.progress_bar, 0, 3, 1, 2);

        music_button.btn_pre_song = gtk_button_new_with_label("上一首");
        gtk_table_attach_defaults(GTK_TABLE(table),  music_button.btn_pre_song, 0, 1, 2, 3);
        g_signal_connect(music_button.btn_pre_song, "clicked", G_CALLBACK(deal_play_pre), &music_button);

        music_button.btn_play_stop = gtk_button_new_with_label("播放");
        gtk_table_attach_defaults(GTK_TABLE(table),  music_button.btn_play_stop, 1, 2, 2, 3);
        g_signal_connect(music_button.btn_play_stop, "clicked", G_CALLBACK(deal_play), &music_button);

        music_button.btn_next_song = gtk_button_new_with_label("下一首");
        gtk_table_attach_defaults(GTK_TABLE(table),  music_button.btn_next_song, 2, 3, 2, 3);
        g_signal_connect(music_button.btn_next_song, "clicked", G_CALLBACK(deal_play_next), &music_button);


        // 创建线程发送进度条
        pthread_t ask_progress_tid;
        pthread_create(&ask_progress_tid, NULL, ask_progress, NULL);

        // 主线程与子线程分离，且相互独立运行，子线程结束后，资源自动回收
        pthread_detach(ask_progress_tid);

        // 创建线程接收进度
        pthread_t receive_progress_tid;
        pthread_create(&receive_progress_tid, NULL, receive_progress, &music_button);
        pthread_detach(receive_progress_tid);

        // 创建串口读取线程，接收语音识别信息
        // pthread_t receive_serial_tid;
        // pthread_create(&receive_serial_tid, NULL, receive_serial, &music_button);
        // pthread_detach(receive_serial_tid);

        // 设置关闭退出以及窗口显示
        g_signal_connect(window, "destroy", G_CALLBACK(my_main_quit), NULL);
        gtk_widget_show_all(window);
        gtk_main();
    }

    return 0;
}

// 处理串口语音识别信息
void *receive_serial(void *arg)
{
    MusicButton *p = (MusicButton *)arg;

    // 初始化语音识别串口
    int fd1 = uart_init("/dev/ttyUSB0");

    while(1)
    {
        char text[128] = "";
        uart_readline(fd1, text, sizeof(text), 1000);

        //判断text中语句 并发送响应的mplayer指令给管道
        switch(text[0])
        {
        case 'b': // text:"bofang\r\n" 播放口令
        case 'z': // text:"zanting\r\n" 暂停口令
            deal_play(p->btn_play_stop, p);
            break;
        case 's': // text:"shangyishou\r\n" 上一首口令
            deal_play_pre(NULL, p);
            break;
        case 'x': // text:"xiayishou\r\n" 下一首口令
            deal_play_next(NULL, p);
            break;
        case 't': // text:"tuichu\r\n" 退出口令
            // 杀死子进程
            system("kill -9  $(ps -A | grep mplayer | awk '{printf $1}')");

            // 杀死父进程
            system("kill -9  $(ps -A | grep demo | awk '{printf $1}')");
            break;
        default:
            break;
        }
    }
}

// 请求音乐播放进度
void *ask_progress(void *arg)
{
    while(1)
    {
        if(flag_ask_progress == 1) // 仅在播放的时候查询
        {
            write(fifo_fd, "get_percent_pos\n", strlen("get_percent_pos\n"));
        }
        sleep(1);
    }
}

// 获取音乐播放进度，更新进度条
void *receive_progress(void *arg)
{
    MusicButton *p = (MusicButton *)arg;

    while(1)
    {
        if (flag_ask_progress == 1)
        {
            char buf[50] = "";

            int flage = read(pip_fd[0], buf, sizeof(buf));

            if(flage > 0) // 当读到字符
            {
                char *temp;

                // 当mplayer返回进度请求时，输出ANS_PERCENT_POSITION=n
                // 以“=”号分割temp
                char *str_precent = strtok_r(buf, "=", &temp);

                // mplayer运行时会输出一整行====作为显示分离
                // 此时strtok_r函数会返回NULL
                if(str_precent != NULL && strcmp(str_precent, "ANS_PERCENT_POSITION") == 0)
                {
                    // 得到n
                    str_precent = strtok_r(NULL, "=", &temp);

                    // 将字符n转为浮点数
                    double n = atof(str_precent);
                    double precent = n / 100;
                    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(p->progress_bar),  precent);
                }
            }
        }
    }
}

// 播放上一首
void deal_play_pre(GtkWidget *btn, gpointer data)
{
    MusicButton *p = (MusicButton *)data;

    const gchar *label = gtk_button_get_label(GTK_BUTTON(p->btn_play_stop));
    if(strcmp((char *)label, "播放") == 0) // 当未播放歌曲时，点击将播放
    {
        // 更新播放按钮
        gtk_button_set_label(GTK_BUTTON(p->btn_play_stop), "暂停");
    }

    // 当已经是第一首时或未播放歌曲时，直接播放到最后一首
    if(playing_index == 0 || playing_index == -1)
    {
        playing_index = music_num - 1;
    }
    else
    {
        playing_index--;
    }

    // 显示歌曲名
    gtk_label_set_text(GTK_LABEL(p->label_music), music_list[playing_index]);

    char str_cmd[120] = {0};
    sprintf(str_cmd, "loadfile %s%s\n", MUSIC_PATH, music_list[playing_index]);
    write(fifo_fd, str_cmd, strlen(str_cmd));

    // 开启查询播放进度
    flag_ask_progress = 1;
}


// 播放下一首
void deal_play_next(GtkWidget *btn, gpointer data)
{
    MusicButton *p = (MusicButton *)data;

    const gchar *label = gtk_button_get_label(GTK_BUTTON(p->btn_play_stop));
    if(strcmp((char *)label, "播放") == 0) // 当未播放歌曲时，点击将播放
    {
        // 更新播放按钮
        gtk_button_set_label(GTK_BUTTON(p->btn_play_stop), "暂停");
    }

    // 当已经是最后一首时，直接播放到第一首
    if(playing_index == music_num - 1)
    {
        playing_index = 0;
    }
    else
    {
        playing_index++;
    }

    // 显示歌曲名
    gtk_label_set_text(GTK_LABEL(p->label_music), music_list[playing_index]);

    char str_cmd[120] = {0};
    sprintf(str_cmd, "loadfile %s%s\n", MUSIC_PATH, music_list[playing_index]);
    write(fifo_fd, str_cmd, strlen(str_cmd));

    // 开启查询播放进度
    flag_ask_progress = 1;
}

// 控制播放/暂停
void deal_play(GtkWidget *btn, gpointer data)
{
    MusicButton *p = (MusicButton *)data;

    const gchar *label = gtk_button_get_label(GTK_BUTTON(btn));

    if(strcmp((char *)label, "播放") == 0) // 此时为暂停，点击将播放
    {
        if(playing_index == -1) // 首次播放
        {
            playing_index = 0;

            // 显示歌曲名
            gtk_label_set_text(GTK_LABEL(p->label_music), music_list[playing_index]);

            char str_cmd[120] = {0};
            sprintf(str_cmd, "loadfile %s%s\n", MUSIC_PATH, music_list[playing_index]);

            // 播放第一首
            write(fifo_fd, str_cmd, strlen(str_cmd));
        }
        else
        {
            // 取消暂停，开始播放
            write(fifo_fd, "pause\n", strlen("pause\n"));
        }

        // 开启查询播放进度
        flag_ask_progress = 1;

        // 更新按钮
        gtk_button_set_label(GTK_BUTTON(btn), "暂停");
    }
    if(strcmp((char *)label,  "暂停") == 0) // 此时为播放中，点击将暂停
    {
        // 停止查询播放进度
        flag_ask_progress = 0;

        // 停止播放
        write(fifo_fd, "pause\n", strlen("pause\n"));

        // 更新按钮
        gtk_button_set_label(GTK_BUTTON(btn), "播放");
    }
}

// 获取播放列表的音乐名
void get_music_list()
{
    DIR *dir;
    struct dirent *ptr;

    //打开一个目录
    dir = opendir(MUSIC_PATH);
    while((ptr = readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
        {
            continue;
        }

        strcat(music_list[music_num], ptr->d_name);
        music_num++;

        if (music_num >= 100)
        {
            break;
        }
    }

    closedir(dir);
}

// 处理退出
void my_main_quit(gpointer data)
{
    // 关闭管道文件
    close(fifo_fd);
    close(pip_fd[0]);
    close(pip_fd[1]);

    char str_cmd[40] = {0};

    // 杀死音乐播放的子进程
    sprintf(str_cmd, "kill -9 %d", mplayer_pid);
    system(str_cmd);

    gtk_main_quit();
}
