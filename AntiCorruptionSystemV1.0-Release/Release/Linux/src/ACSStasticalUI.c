/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSStasticalUI.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly includes callback functions when stastical
buttons clicked.
Function List: 
1.on_charge_stastical_clicked			called back when 
charge-stastical-button clicked. Run a dialog containing a 
list view to display the information.
2.on_punishment_stastical_clicked		called back when
punishment-stastical-button clicked. Run a dialog containing a 
list view to display the information.
3.on_place_stastical_clicked			called back when
place-stastical-button clicked. Run a dialog containing a list 
view to display the information.
4.on_report_stastical_clicked			called back when
report-stastical-button clicked. Run a dialog containing a 
list view to display the information.
5.on_amount_stastical_clicked			called back when
amount-stastical-button clicked. Run a dialog containing a
list view to display the information.
*************************************************************/

#include "ACSHeader.h"

extern GtkWidget * main_window;
extern ProvinceList head;

enum {CHARGE_NAME, CHARGE_COUNT, CHARGE_AMOUNT, CHARGE_COLUMNS};
enum {LEVEL_NAME, PUNISHMENT_TOTAL, PUNISHMENT_DEATH, PUNISHMENT_LIFE, PUNISHMENT_LOW, PUNISHMENT_MEDIUM, PUNISHMENT_HIGH, PUNISHMENT_COLUMNS};
enum {PLACE_NAME, PLACE_COUNT, PLACE_AMOUNT, PLACE_COLUMNS};
enum {REPORT_NUMBER, REPORT_TIMES, REPORT_LEVEL, REPORT_CHARGE, REPORT_AMOUNT, REPORT_PUNISHMENT, REPORT_COLUMNS};
enum {AMOUNT_NUMBER, AMOUNT_NAME, AMOUNT_LEVEL, AMOUNT_CHARGE, AMOUNT_AMOUNT, AMOUNT_PUNISHMENT, AMOUNT_COLUMNS};

/*************************************************
Function: on_charge_stastical_clicked
Description: called back when charge-stastical-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_charge_stastical_clicked(GtkWidget * widget)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("统计结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkListStore * store = gtk_list_store_new(CHARGE_COLUMNS, G_TYPE_STRING, G_TYPE_INT, G_TYPE_DOUBLE);
    GtkWidget * charge_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    GtkTreeIter iter;
	Charge * result = charge_stastical(head);
	int count;
	for (count = 0 ; count < CHARGENUM ; count++)
	{
		char charge[17];

	    if(result[count].name == '1')
            strcpy(charge, "受贿");
        else if (result[count].name == '2')
            strcpy(charge, "贪污");
        else if (result[count].name == '3')
            strcpy(charge, "挪用公款");
        else if (result[count].name == '4')
            strcpy(charge, "渎职");
        else if (result[count].name == '5')
            strcpy(charge, "滥用职权");
        else if (result[count].name == '6')
            strcpy(charge, "其他");

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, CHARGE_NAME, charge, CHARGE_COUNT, result[count].times, CHARGE_AMOUNT, result[count].amount, -1);
	}
    free(result);
    g_object_unref(store);

    GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn * name_column = gtk_tree_view_column_new_with_attributes("罪名名称", renderer, "text", CHARGE_NAME, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(name_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(charge_list), name_column);
    GtkTreeViewColumn * count_column = gtk_tree_view_column_new_with_attributes("发生次数", renderer, "text", CHARGE_COUNT, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(count_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(count_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(count_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(charge_list), count_column);
    GtkTreeViewColumn * amount_column = gtk_tree_view_column_new_with_attributes("涉案金额", renderer, "text", CHARGE_AMOUNT, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(amount_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(charge_list), amount_column);

    GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), charge_list);

    GtkWidget * vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);

}

/*************************************************
Function: on_punishment_stastical_clicked
Description: called back when punishment-stastical-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_punishment_stastical_clicked(GtkWidget * widget)
{
	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("按行政级别查询：", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * yearlabel = gtk_label_new("请输入年份：");
    GtkWidget * yearentry = gtk_entry_new();

    GtkWidget * hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), yearlabel, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), yearentry, FALSE, FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    char year[5];
    strcpy(year, gtk_entry_get_text(GTK_ENTRY(yearentry)));

    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
    {
		GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
	    GtkWidget * dialog = gtk_dialog_new_with_buttons("统计结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
	    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
	    gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
	    g_object_unref(pixbuf), pixbuf = NULL;

	    GtkListStore * store = gtk_list_store_new(PUNISHMENT_COLUMNS, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	    GtkWidget * punishment_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	    GtkTreeIter iter;
		Punishment * result = punishment_stastical(head, year);
		int count;
		for (count = 0 ; count < LEVELNUM ; count++)
		{
			char level[25];

            if(result[count].level == 'A')
                strcpy(level, "副科级");
            else if (result[count].level == 'B')
                strcpy(level, "正科级");
            else if (result[count].level == 'C')
                strcpy(level, "副处级");
            else if (result[count].level == 'D')
                strcpy(level, "正处级");
            else if (result[count].level == 'E')
                strcpy(level, "副厅级");
            else if (result[count].level == 'F')
                strcpy(level, "正厅级");
            else if (result[count].level == 'G')
                strcpy(level, "副处级");
            else if (result[count].level == 'H')
                strcpy(level, "正处级及以上");

	        gtk_list_store_append(store, &iter);
	        gtk_list_store_set(store, &iter, LEVEL_NAME, level, PUNISHMENT_TOTAL, result[count].times, PUNISHMENT_DEATH, result[count].death, PUNISHMENT_LIFE, result[count].life, PUNISHMENT_LOW, result[count].low, PUNISHMENT_MEDIUM, result[count].medium, PUNISHMENT_HIGH, result[count].high, -1);
		}
	    free(result);
	    g_object_unref(store);

	    GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
	    GtkTreeViewColumn * name_column = gtk_tree_view_column_new_with_attributes("行政级别", renderer, "text", LEVEL_NAME, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(name_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(punishment_list), name_column);
	    GtkTreeViewColumn * count_column = gtk_tree_view_column_new_with_attributes("发生次数", renderer, "text", PUNISHMENT_TOTAL, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(count_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(count_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(count_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(punishment_list), count_column);
	    GtkTreeViewColumn * death_column = gtk_tree_view_column_new_with_attributes("死刑", renderer, "text", PUNISHMENT_DEATH, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(death_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(death_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(death_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(punishment_list), death_column);
	    GtkTreeViewColumn * life_column = gtk_tree_view_column_new_with_attributes("无期徒刑", renderer, "text", PUNISHMENT_LIFE, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(life_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(life_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(life_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(punishment_list), life_column);
	    GtkTreeViewColumn * low_column = gtk_tree_view_column_new_with_attributes("有期徒刑1-5年", renderer, "text", PUNISHMENT_LOW, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(low_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(low_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(low_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(punishment_list), low_column);
	    GtkTreeViewColumn * medium_column = gtk_tree_view_column_new_with_attributes("有期徒刑6-10年", renderer, "text", PUNISHMENT_MEDIUM, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(medium_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(medium_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(medium_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(punishment_list), medium_column);
	    GtkTreeViewColumn * high_column = gtk_tree_view_column_new_with_attributes("有期徒刑11年以上", renderer, "text", PUNISHMENT_HIGH, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(high_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(high_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(high_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(punishment_list), high_column);

	    GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), punishment_list);

	    GtkWidget * vbox = gtk_vbox_new(FALSE, 5);
	    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 5);
	    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
	    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);

	    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

	    gtk_widget_show_all(dialog);
	    gtk_dialog_run(GTK_DIALOG(dialog));

	    gtk_widget_destroy(dialog);
	}
}

/*************************************************
Function: on_place_stastical_clicked
Description: called back when place-stastical-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_place_stastical_clicked(GtkWidget * widget)
{
	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("按行政级别统计", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * startlabel = gtk_label_new("开始日期：");
    GtkWidget * endlabel = gtk_label_new("截止日期：");
    GtkWidget * startentry = gtk_entry_new();
    GtkWidget * endentry = gtk_entry_new();

    GtkWidget * table = gtk_table_new(2, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), startlabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), endlabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), startentry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), endentry, 1, 2, 1, 2);

    gtk_table_set_row_spacings(GTK_TABLE(table), 5);
    gtk_table_set_col_spacings(GTK_TABLE(table), 5);
    gtk_container_set_border_width(GTK_CONTAINER(table), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), table);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    char startdate[12], enddate[12];
    strcpy(startdate, gtk_entry_get_text(GTK_ENTRY(startentry)));
    strcpy(enddate, gtk_entry_get_text(GTK_ENTRY(endentry)));

    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
    {
		GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
	    GtkWidget * dialog = gtk_dialog_new_with_buttons("统计结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
	    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
	    gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
	    g_object_unref(pixbuf), pixbuf = NULL;

	    GtkListStore * store = gtk_list_store_new(PLACE_COLUMNS, G_TYPE_STRING, G_TYPE_INT, G_TYPE_DOUBLE);
	    GtkWidget * place_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	    GtkTreeIter iter;
		Place * result = place_stastical(head, startdate, enddate);
		int count;
		for (count = 0 ; count < province_number(head)-1 ; count++)
		{
	        gtk_list_store_append(store, &iter);
	        gtk_list_store_set(store, &iter, PLACE_NAME, result[count].province, PLACE_COUNT, result[count].times, PLACE_AMOUNT, result[count].amount, -1);
		}
	    free(result);
	    g_object_unref(store);

	    GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
	    GtkTreeViewColumn * name_column = gtk_tree_view_column_new_with_attributes("所属省份", renderer, "text", PLACE_NAME, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(name_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(place_list), name_column);
	    GtkTreeViewColumn * count_column = gtk_tree_view_column_new_with_attributes("发生次数", renderer, "text", PLACE_COUNT, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(count_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(count_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(count_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(place_list), count_column);
	    GtkTreeViewColumn * amount_column = gtk_tree_view_column_new_with_attributes("涉案金额", renderer, "text", PLACE_AMOUNT, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(amount_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(place_list), amount_column);
	    GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), place_list);
	    GtkWidget * vbox = gtk_vbox_new(FALSE, 5);
	    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 5);
	    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
	    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);

	    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

	    gtk_widget_show_all(dialog);
	    gtk_dialog_run(GTK_DIALOG(dialog));

	    gtk_widget_destroy(dialog);
    }
}

/*************************************************
Function: on_report_stastical_clicked
Description: called back when report-stastical-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_report_stastical_clicked(GtkWidget * widget)
{
	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("统计结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkListStore * store = gtk_list_store_new(REPORT_COLUMNS, G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT);
    GtkWidget * report_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    GtkTreeIter iter;
	Report * result =  report_stastical(head);
	int count;
	for (count = 0 ; count < MAXREPORTNUM ; count++)
	{
		if (result[count].times != -1)
		{
			char level[25], charge[17];

            if(result[count].level == 'A')
                strcpy(level, "副科级");
            else if (result[count].level == 'B')
                strcpy(level, "正科级");
            else if (result[count].level == 'C')
                strcpy(level, "副处级");
            else if (result[count].level == 'D')
                strcpy(level, "正处级");
            else if (result[count].level == 'E')
                strcpy(level, "副厅级");
            else if (result[count].level == 'F')
                strcpy(level, "正厅级");
            else if (result[count].level == 'G')
                strcpy(level, "副处级");
            else if (result[count].level == 'H')
                strcpy(level, "正处级及以上");

            if(result[count].charge == '1')
                strcpy(charge, "受贿");
            else if (result[count].charge == '2')
                strcpy(charge, "贪污");
            else if (result[count].charge == '3')
                strcpy(charge, "挪用公款");
            else if (result[count].charge == '4')
                strcpy(charge, "渎职");
            else if (result[count].charge == '5')
                strcpy(charge, "滥用职权");
            else if (result[count].charge == '6')
                strcpy(charge, "其他");

        	gtk_list_store_append(store, &iter);
        	gtk_list_store_set(store, &iter, REPORT_NUMBER, result[count].number, REPORT_TIMES, result[count].times, REPORT_LEVEL, level, REPORT_CHARGE, charge, REPORT_AMOUNT, result[count].amount, REPORT_PUNISHMENT, result[count].punishment, -1);
		}
	}
    free(result);
    g_object_unref(store);

    GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn * number_column = gtk_tree_view_column_new_with_attributes("案件编号", renderer, "text", REPORT_NUMBER, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(number_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(report_list), number_column);
    GtkTreeViewColumn * count_column = gtk_tree_view_column_new_with_attributes("媒体报道次数", renderer, "text", REPORT_TIMES, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(count_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(count_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(count_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(report_list), count_column);
    GtkTreeViewColumn * level_column = gtk_tree_view_column_new_with_attributes("被告行政级别", renderer, "text", REPORT_LEVEL, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(level_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(level_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(level_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(report_list), level_column);
    GtkTreeViewColumn * charge_column = gtk_tree_view_column_new_with_attributes("主要罪名", renderer, "text", REPORT_CHARGE, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(charge_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(charge_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(charge_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(report_list), charge_column);
    GtkTreeViewColumn * amount_column = gtk_tree_view_column_new_with_attributes("涉案金额", renderer, "text", REPORT_AMOUNT, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(amount_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(report_list), amount_column);
    GtkTreeViewColumn * punishment_column = gtk_tree_view_column_new_with_attributes("刑事处罚", renderer, "text", REPORT_PUNISHMENT, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(punishment_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(punishment_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(punishment_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(report_list), punishment_column);

    GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), report_list);

    GtkWidget * vbox = gtk_vbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);
}

/*************************************************
Function: on_amount_stastical_clicked
Description: called back when amount-stastical-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_amount_stastical_clicked(GtkWidget * widget)
{
	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("统计结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkListStore * store = gtk_list_store_new(AMOUNT_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT);
    GtkWidget * amount_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    GtkTreeIter iter;
	Amount * result = amount_stastical(head);
	int count;
	for (count = 0 ; count < MAXAMOUNTNUM ; count++)
	{
		if (result[count].amount != -1)
		{
			char level[25], charge[17];

            if(result[count].level == 'A')
                strcpy(level, "副科级");
            else if (result[count].level == 'B')
                strcpy(level, "正科级");
            else if (result[count].level == 'C')
                strcpy(level, "副处级");
            else if (result[count].level == 'D')
                strcpy(level, "正处级");
            else if (result[count].level == 'E')
                strcpy(level, "副厅级");
            else if (result[count].level == 'F')
                strcpy(level, "正厅级");
            else if (result[count].level == 'G')
                strcpy(level, "副处级");
            else if (result[count].level == 'H')
                strcpy(level, "正处级及以上");

            if(result[count].charge == '1')
                strcpy(charge, "受贿");
            else if (result[count].charge == '2')
                strcpy(charge, "贪污");
            else if (result[count].charge == '3')
                strcpy(charge, "挪用公款");
            else if (result[count].charge == '4')
                strcpy(charge, "渎职");
            else if (result[count].charge == '5')
                strcpy(charge, "滥用职权");
            else if (result[count].charge == '6')
                strcpy(charge, "其他");

        	gtk_list_store_append(store, &iter);
        	gtk_list_store_set(store, &iter, AMOUNT_NUMBER, result[count].number, AMOUNT_NAME, result[count].name, AMOUNT_LEVEL, level, AMOUNT_CHARGE, charge, AMOUNT_AMOUNT, result[count].amount, AMOUNT_PUNISHMENT, result[count].punishment, -1);
		}
	}
    free(result);
    g_object_unref(store);

    GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn * number_column = gtk_tree_view_column_new_with_attributes("案件编号", renderer, "text", AMOUNT_NUMBER, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(number_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(amount_list), number_column);
    GtkTreeViewColumn * name_column = gtk_tree_view_column_new_with_attributes("被告姓名", renderer, "text", AMOUNT_NAME, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(name_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(amount_list), name_column);
    GtkTreeViewColumn * level_column = gtk_tree_view_column_new_with_attributes("被告行政级别", renderer, "text", AMOUNT_LEVEL, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(level_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(level_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(level_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(amount_list), level_column);
    GtkTreeViewColumn * charge_column = gtk_tree_view_column_new_with_attributes("主要罪名", renderer, "text", AMOUNT_CHARGE, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(charge_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(charge_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(charge_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(amount_list), charge_column);
    GtkTreeViewColumn * amount_column = gtk_tree_view_column_new_with_attributes("涉案金额", renderer, "text", AMOUNT_AMOUNT, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(amount_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(amount_list), amount_column);
    GtkTreeViewColumn * punishment_column = gtk_tree_view_column_new_with_attributes("刑事处罚", renderer, "text", AMOUNT_PUNISHMENT, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(punishment_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(punishment_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(punishment_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(amount_list), punishment_column);

    GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), amount_list);
    GtkWidget * vbox = gtk_vbox_new(FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));

    gtk_widget_destroy(dialog);
}
