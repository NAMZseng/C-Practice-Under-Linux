#include <gtk/gtk.h>
#include <stdio.h>

void deal_button(GtkButton *button, gpointer data);

int main(int argc, char *argv[])
{
	// gtk环境初始化
	gtk_init(&argc, &argv);
	
	// 1 创建主窗口
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// 若不规定窗口名称，则默认以程序的可执行文件命名
	// gtk_window_set_title((Gtkwindow *)window, "GTK Demo");
	// 使用Gtk的宏GTK_WINDOW()进行类型转换
	gtk_window_set_title(GTK_WINDOW(window), "GTK Demo");
	gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	// 绑定系统提供的退出函数，设置关闭窗口退出程序
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// 2 创建按钮及其相关监听事件
	GtkWidget *button = gtk_button_new_with_label("点击");
	gtk_container_add(GTK_CONTAINER(window), button);
	// 绑定控件与事件信号，以及信号发生时的动作,即回调函数
	g_signal_connect(button, "clicked", G_CALLBACK(deal_button), "button_data");
	
	
	
	// 显示窗口，默认是不显示
	gtk_widget_show_all(window);

	// 3 主事件循环，等待用户操作界面
	gtk_main();
	
	return 0;
}

void deal_button(GtkButton *btn, gpointer data)
{
	printf("data=%s\n", (char *)data);
	// 获取按钮文本，使用const，确保只读，不改变其内容
	const gchar *text = gtk_button_get_label(btn);
	printf("button=%s\n", text);
	
	// 设置按钮文本
	gtk_button_set_label(btn, "新点击");
	
}


