#include <gtk/gtk.h>

typedef struct
{
	GtkWidget *label_pc_result;
	GtkWidget *label_result;
	GtkWidget *label_person_result;
	GtkWidget *btn0;
	GtkWidget *btn1;
	GtkWidget *btn2;
}MYWID;

void deal_button(GtkWidget *btn, gpointer data);

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Finger Guessing Game");
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 180);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
/* 	// 创建水平布局容器horizontal box (垂直 vertical )
	GtkWidget *hbox = gtk_hbox_new(TRUE, 15);
	// 添加布局容器到主窗口中
	gtk_container_add(GTK_CONTAINER(window), hbox);
	
	//创建其他控件
	GtkWidget *btn1 = gtk_button_new_with_label("button_1");
	GtkWidget *btn2 = gtk_button_new_with_label("button_2");
	GtkWidget *btn3 = gtk_button_new_with_label("button_3");
	
	// 添加控件到布局容器中
	gtk_container_add(GTK_CONTAINER(hbox), btn1);
	gtk_container_add(GTK_CONTAINER(hbox), btn2);
	gtk_container_add(GTK_CONTAINER(hbox), btn3);
	 */
	
	// 创建表格布局4行3列
	GtkWidget *table = gtk_table_new(4, 3, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);
	 
	// 创建控件，并放入表格中
	GtkWidget *label_title = gtk_label_new("猜拳游戏（Finger Guessing Game）");
	// 加入控件到表格中，左右上下
	gtk_table_attach_defaults(GTK_TABLE(table), label_title, 0, 3, 0, 1);
	
	// 第一行
	GtkWidget *label_pc = gtk_label_new("电脑");
	gtk_table_attach_defaults(GTK_TABLE(table), label_pc, 0, 1, 1, 2);
	
	GtkWidget *label_vs = gtk_label_new("VS");
	gtk_table_attach_defaults(GTK_TABLE(table), label_vs, 1, 2, 1, 2);
	
	GtkWidget *label_person = gtk_label_new("人");
	gtk_table_attach_defaults(GTK_TABLE(table), label_person, 2, 3, 1, 2);
	
	// 创建自定义结构体
	MYWID wid;
	// 第二行
	wid.label_pc_result = gtk_label_new("电脑出招");
	gtk_table_attach_defaults(GTK_TABLE(table), wid.label_pc_result, 0, 1, 2, 3);
	
	wid.label_result = gtk_label_new("Result");
	gtk_table_attach_defaults(GTK_TABLE(table), wid.label_result, 1, 2, 2, 3);
	
	wid.label_person_result = gtk_label_new("人出招");
	gtk_table_attach_defaults(GTK_TABLE(table), wid.label_person_result, 2, 3, 2, 3);
	
	// 第三行
	wid.btn0 = gtk_button_new_with_label("剪刀");
	gtk_table_attach_defaults(GTK_TABLE(table), wid.btn0, 0, 1, 3, 4);
	
	wid.btn1 = gtk_button_new_with_label("石头");
	gtk_table_attach_defaults(GTK_TABLE(table), wid.btn1, 1, 2, 3, 4);
	
	wid.btn2 = gtk_button_new_with_label("布");
	gtk_table_attach_defaults(GTK_TABLE(table), wid.btn2, 2, 3, 3, 4);
	
	g_signal_connect(wid.btn0, "clicked", G_CALLBACK(deal_button), &wid);
	g_signal_connect(wid.btn1, "clicked", G_CALLBACK(deal_button), &wid);
	g_signal_connect(wid.btn2, "clicked", G_CALLBACK(deal_button), &wid);
	
	srand(time(NULL));
	
	// 设置关闭退出以及窗口显示
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(window);
	gtk_main();
	
	return 0;
}

// 回调函数
void deal_button(GtkWidget *btn, gpointer data)
{
	MYWID *p = (MYWID *)data;
	int person_result = 0;
	int pc_result = 0;

	// 剪刀
	if(btn == p->btn0)
	{
		gtk_label_set_text(GTK_LABEL(p->label_person_result), "剪刀");
		person_result = 0;
	}
	// 石头
	else if(btn == p->btn1)
	{
		gtk_label_set_text(GTK_LABEL(p->label_person_result), "石头");
		person_result = 1;
	}
	// 布
	else if(btn == p->btn2)
	{
		gtk_label_set_text(GTK_LABEL(p->label_person_result), "布");
		person_result = 2;
	}
	
	// 电脑产生
	 pc_result = rand() % 3;
	 
	 switch(pc_result)
	 {
		case 0:
			gtk_label_set_text(GTK_LABEL(p->label_pc_result), "剪刀");
			break;
		case 1:
			gtk_label_set_text(GTK_LABEL(p->label_pc_result), "石头");
			break;
		case 2:
			gtk_label_set_text(GTK_LABEL(p->label_pc_result), "布");
			break;
		default:
			break;
	 }
	 
	 if(person_result == pc_result)
	 {
		gtk_label_set_text(GTK_LABEL(p->label_result), "平局");
	 }
	 /* else if(person_result > pc_result && person_result - pc_result == 1)
	 {
		gtk_label_set_text(GTK_LABEL(p->label_result), "<");
	 }
	  else if(person_result < pc_result && pc_result - person_result == 1)
	 {
		gtk_label_set_text(GTK_LABEL(p->label_result), ">");
	 }
	 else if(person_result > pc_result && person_result - pc_result == 2)
	 {
		gtk_label_set_text(GTK_LABEL(p->label_result), ">");
	 }
	 else if(person_result < pc_result && pc_result - person_result == 2)
	 {
		gtk_label_set_text(GTK_LABEL(p->label_result), "<");
	 } */
	 
	 else if((pc_result + 1) %3 == person_result)
	 {
		gtk_label_set_text(GTK_LABEL(p->label_result), "<");
	 }
	 else
	 {
		gtk_label_set_text(GTK_LABEL(p->label_result), ">");
	 }
}