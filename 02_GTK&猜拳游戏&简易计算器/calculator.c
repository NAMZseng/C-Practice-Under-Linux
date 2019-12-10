#include <gtk/gtk.h>
#include <stdio.h>

long pre_num = 0;
long end_num = 0;
// 操作数
char action = 0;

void deal_num(GtkWidget *btn, gpointer data);
void deal_action(GtkWidget *btn, gpointer data);
void deal_result(GtkWidget *btn, gpointer data);
void deal_clear(GtkWidget *btn, gpointer data);

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Calculator");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 240);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
	// 创建表格布局4行6列
	GtkWidget *table = gtk_table_new(4, 6, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);
	
	// 第一行
	GtkWidget *entry  =  gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table), entry, 0, 6, 0, 1);
	
	//第二行
	GtkWidget *btn_9 = gtk_button_new_with_label("9");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_9, 0, 1, 1, 2);
	g_signal_connect(btn_9, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_8 = gtk_button_new_with_label("8");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_8, 1, 2, 1, 2);
	g_signal_connect(btn_8, "clicked", G_CALLBACK(deal_num), entry);
		
	GtkWidget *btn_7 = gtk_button_new_with_label("7");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_7, 2, 3, 1, 2);
	g_signal_connect(btn_7, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_add = gtk_button_new_with_label("+");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_add, 3, 4, 1, 2);
	g_signal_connect(btn_add, "clicked", G_CALLBACK(deal_action), entry);
	
	GtkWidget *btn_sub = gtk_button_new_with_label("-");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_sub, 4, 5, 1, 2);
	g_signal_connect(btn_sub, "clicked", G_CALLBACK(deal_action), entry);
	
	GtkWidget *btn_del = gtk_button_new_with_label("");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_del, 5, 6, 1, 2);
	
	// 第三行
	GtkWidget *btn_6 = gtk_button_new_with_label("6");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_6, 0, 1, 2, 3);
	g_signal_connect(btn_6, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_5 = gtk_button_new_with_label("5");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_5, 1, 2, 2, 3);
	g_signal_connect(btn_5, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_4 = gtk_button_new_with_label("4");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_4, 2, 3, 2, 3);
	g_signal_connect(btn_4, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_mul = gtk_button_new_with_label("*");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_mul, 3, 4, 2, 3);
	g_signal_connect(btn_mul, "clicked", G_CALLBACK(deal_action), entry);
	
	GtkWidget *btn_div = gtk_button_new_with_label("/");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_div, 4, 5, 2, 3);
	g_signal_connect(btn_div, "clicked", G_CALLBACK(deal_action), entry);
	
	GtkWidget *btn_clear = gtk_button_new_with_label("C");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_clear, 5, 6, 2, 3);
	g_signal_connect(btn_clear, "clicked", G_CALLBACK(deal_clear), entry);
	
	// 第四行
	GtkWidget *btn_3 = gtk_button_new_with_label("3");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_3, 0, 1, 3, 4);
	g_signal_connect(btn_3, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_2 = gtk_button_new_with_label("2");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_2, 1, 2, 3, 4);
	g_signal_connect(btn_2, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_1 = gtk_button_new_with_label("1");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_1, 2, 3, 3, 4);
	g_signal_connect(btn_1, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_0 = gtk_button_new_with_label("0");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_0, 3, 4, 3, 4);
	g_signal_connect(btn_0, "clicked", G_CALLBACK(deal_num), entry);
	
	GtkWidget *btn_result = gtk_button_new_with_label("=");
	gtk_table_attach_defaults(GTK_TABLE(table), btn_result, 4, 6, 3, 4);
	g_signal_connect(btn_result, "clicked", G_CALLBACK(deal_result), entry);
	
	
	
	// 设置关闭退出以及窗口显示
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(window);
	gtk_main();
	
	return 0;
}

void deal_clear(GtkWidget *btn, gpointer data)
{
	//清空
	gtk_entry_set_text(GTK_ENTRY(data), "");
}

// 获取结果
void deal_result(GtkWidget *btn, gpointer data)
{
	//清空
	gtk_entry_set_text(GTK_ENTRY(data), "");
	double reslut = 0;
	
	switch(action)
	{
		case '+':
			reslut = end_num + pre_num;
			break;
		case '-':
			reslut = end_num - pre_num;
			break;
		case '*':
			reslut = end_num * pre_num;
			break;
		case '/':
			if(pre_num == 0)
			{
				gtk_entry_set_text(GTK_ENTRY(data), "除零错误!");
				end_num = 0;
				return;
			}
			reslut = (double)end_num / pre_num;
			break;
		default:
			break;
	}
	
	char result_text[100];
	sprintf(result_text, "%lf", reslut);
	gtk_entry_set_text(GTK_ENTRY(data), result_text);
	
	end_num = 0;
	pre_num = reslut;
}

// 处理操作数
void deal_action(GtkWidget *btn, gpointer data)
{
	//清空
	const gchar *action_text = gtk_button_get_label(GTK_BUTTON(btn));
	gtk_entry_append_text(GTK_ENTRY(data), action_text);
	 
	 end_num = pre_num;
	 pre_num = 0;
	 
	 // 获取操作
	 const gchar *text = gtk_button_get_label(GTK_BUTTON(btn));
	 action = text[0];
}

// 处理数字键
void deal_num(GtkWidget *btn, gpointer data)
{
	const gchar *text = gtk_button_get_label(GTK_BUTTON(btn));
	pre_num *= 10;
	// '0' 的ASII 为 48
	pre_num += (int)text[0] - 48;
	
	// 显示
	 gtk_entry_append_text(GTK_ENTRY(data), text);
	
	//printf("num=%ld\n", pre_num);
}