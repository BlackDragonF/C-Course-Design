/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSQueryCaseUI.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly includes callback functions when query
buttons about case clicked.
Function List: 
1.on_case_number_query_clicked   called back when query-case-
by-number-button clicked. Run a dialog containing a list view
to display the information.
2.on_case_name_and_level_query_clicked    called back when
query-case-by-name-and-level-button clicked. Run a dialog
containing a list view to display the information.
************************************************************/

#include "ACSHeader.h"

extern GtkWidget * main_window;
extern ProvinceList head;

enum {NUMBER_COLUMN, DATE_COLUMN, CASE_PROVINCE_COLUMN, DEFENDANT_COLUMN, LEVEL_COLUMN, CHARGE_COLUMN, AMOUNT_COLUMN, PUNISHMENT_COLUMN, CASE_COLUMNS};

/*************************************************
Function: on_case_number_query_clicked   
Description: called back when query-case-by-number-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_case_number_query_clicked(GtkWidget * widget)
{
	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("按案件编号查询", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * numberlabel = gtk_label_new("案件编号：");
    GtkWidget * numberentry = gtk_entry_new();

    GtkWidget * hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), numberlabel, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), numberentry, FALSE, FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    char number[12];
    strcpy(number, gtk_entry_get_text(GTK_ENTRY(numberentry)));

    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
    {
    	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
        GtkWidget * dialog = gtk_dialog_new_with_buttons("查询结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
        gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
        g_object_unref(pixbuf), pixbuf = NULL;

        GtkListStore * store = gtk_list_store_new(CASE_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
	    GtkWidget * case_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	    GtkTreeIter iter;
	    CaseList temp_head = seek_case_number(head, number);
	    CaseNode case_node = temp_head;
        while (case_node != NULL)
        {
            char level[25], charge[17];

            if(case_node->content.level == 'A')
                strcpy(level, "副科级");
            else if (case_node->content.level == 'B')
                strcpy(level, "正科级");
            else if (case_node->content.level == 'C')
                strcpy(level, "副处级");
            else if (case_node->content.level == 'D')
                strcpy(level, "正处级");
            else if (case_node->content.level == 'E')
                strcpy(level, "副厅级");
            else if (case_node->content.level == 'F')
                strcpy(level, "正厅级");
            else if (case_node->content.level == 'G')
                strcpy(level, "副处级");
            else if (case_node->content.level == 'H')
                strcpy(level, "正处级及以上");

            if(case_node->content.charge == '1')
                strcpy(charge, "受贿");
            else if (case_node->content.charge == '2')
                strcpy(charge, "贪污");
            else if (case_node->content.charge == '3')
                strcpy(charge, "挪用公款");
            else if (case_node->content.charge == '4')
                strcpy(charge, "渎职");
            else if (case_node->content.charge == '5')
                strcpy(charge, "滥用职权");
            else if (case_node->content.charge == '6')
                strcpy(charge, "其他");

            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, NUMBER_COLUMN, case_node->content.number, DATE_COLUMN, case_node->content.date, CASE_PROVINCE_COLUMN, case_node->content.province, DEFENDANT_COLUMN, case_node->content.defendant, LEVEL_COLUMN, level, CHARGE_COLUMN, charge, AMOUNT_COLUMN, case_node->content.amount, PUNISHMENT_COLUMN, case_node->content.punishment, -1);
            
            case_node = case_node->next;
        }
        free_case_list(temp_head);
	    g_object_unref(store);

	    GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
	    GtkTreeViewColumn * number_column = gtk_tree_view_column_new_with_attributes("案件编号", renderer, "text", NUMBER_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(number_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), number_column);
	    GtkTreeViewColumn * date_column = gtk_tree_view_column_new_with_attributes("立案日期", renderer, "text", DATE_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(date_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(date_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(date_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), date_column);
	    GtkTreeViewColumn * province_column = gtk_tree_view_column_new_with_attributes("所属省份", renderer, "text", CASE_PROVINCE_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(province_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(province_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(province_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), province_column);
	    GtkTreeViewColumn * defendant_column = gtk_tree_view_column_new_with_attributes("被告人姓名", renderer, "text", DEFENDANT_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(defendant_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(defendant_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(defendant_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), defendant_column);
	    GtkTreeViewColumn * level_column = gtk_tree_view_column_new_with_attributes("行政级别", renderer, "text", LEVEL_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(level_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(level_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(level_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), level_column);
	    GtkTreeViewColumn * charge_column = gtk_tree_view_column_new_with_attributes("主要罪名", renderer, "text", CHARGE_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(charge_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(charge_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(charge_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), charge_column);
	    GtkTreeViewColumn * amount_column = gtk_tree_view_column_new_with_attributes("涉案金额", renderer, "text", AMOUNT_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(amount_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), amount_column);
	    GtkTreeViewColumn * punishment_column = gtk_tree_view_column_new_with_attributes("刑事处罚", renderer, "text", PUNISHMENT_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(punishment_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(punishment_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(punishment_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), punishment_column);

	    GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), case_list);

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
Function: on_case_name_and_level_query_clicked    
Description: called back when query-case-by-name-and-level-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_case_name_and_level_query_clicked(GtkWidget * widget)
{
	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("按被告姓名和行政级别查询", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * namelabel = gtk_label_new("被告姓名：");
    GtkWidget * levellabel = gtk_label_new("行政级别：");
    GtkWidget * nameentry = gtk_entry_new();
    GtkWidget * levelentry = gtk_entry_new();

    GtkWidget * table = gtk_table_new(2, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), namelabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), levellabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), nameentry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), levelentry, 1, 2, 1, 2);

    gtk_table_set_row_spacings(GTK_TABLE(table), 5);
    gtk_table_set_col_spacings(GTK_TABLE(table), 5);
    gtk_container_set_border_width(GTK_CONTAINER(table), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), table);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    char name[15], level;
    strcpy(name, gtk_entry_get_text(GTK_ENTRY(nameentry)));
    level = *gtk_entry_get_text(GTK_ENTRY(levelentry));

    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
    {
    	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
        GtkWidget * dialog = gtk_dialog_new_with_buttons("查询结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
        gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
        g_object_unref(pixbuf), pixbuf = NULL;

        GtkListStore * store = gtk_list_store_new(CASE_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
	    GtkWidget * case_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	    GtkTreeIter iter;
	    CaseList temp_head = seek_case_name_level(head, name, level);
	    CaseNode case_node = temp_head;
        while (case_node != NULL)
        {
            char level[25], charge[17];

            if(case_node->content.level == 'A')
                strcpy(level, "副科级");
            else if (case_node->content.level == 'B')
                strcpy(level, "正科级");
            else if (case_node->content.level == 'C')
                strcpy(level, "副处级");
            else if (case_node->content.level == 'D')
                strcpy(level, "正处级");
            else if (case_node->content.level == 'E')
                strcpy(level, "副厅级");
            else if (case_node->content.level == 'F')
                strcpy(level, "正厅级");
            else if (case_node->content.level == 'G')
                strcpy(level, "副处级");
            else if (case_node->content.level == 'H')
                strcpy(level, "正处级及以上");

            if(case_node->content.charge == '1')
                strcpy(charge, "受贿");
            else if (case_node->content.charge == '2')
                strcpy(charge, "贪污");
            else if (case_node->content.charge == '3')
                strcpy(charge, "挪用公款");
            else if (case_node->content.charge == '4')
                strcpy(charge, "渎职");
            else if (case_node->content.charge == '5')
                strcpy(charge, "滥用职权");
            else if (case_node->content.charge == '6')
                strcpy(charge, "其他");

            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, NUMBER_COLUMN, case_node->content.number, DATE_COLUMN, case_node->content.date, CASE_PROVINCE_COLUMN, case_node->content.province, DEFENDANT_COLUMN, case_node->content.defendant, LEVEL_COLUMN, level, CHARGE_COLUMN, charge, AMOUNT_COLUMN, case_node->content.amount, PUNISHMENT_COLUMN, case_node->content.punishment, -1);
            
            case_node = case_node->next;
        }
        free_case_list(temp_head);
	    g_object_unref(store);

	    GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
	    GtkTreeViewColumn * number_column = gtk_tree_view_column_new_with_attributes("案件编号", renderer, "text", NUMBER_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(number_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), number_column);
	    GtkTreeViewColumn * date_column = gtk_tree_view_column_new_with_attributes("立案日期", renderer, "text", DATE_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(date_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(date_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(date_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), date_column);
	    GtkTreeViewColumn * province_column = gtk_tree_view_column_new_with_attributes("所属省份", renderer, "text", CASE_PROVINCE_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(province_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(province_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(province_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), province_column);
	    GtkTreeViewColumn * defendant_column = gtk_tree_view_column_new_with_attributes("被告人姓名", renderer, "text", DEFENDANT_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(defendant_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(defendant_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(defendant_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), defendant_column);
	    GtkTreeViewColumn * level_column = gtk_tree_view_column_new_with_attributes("行政级别", renderer, "text", LEVEL_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(level_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(level_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(level_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), level_column);
	    GtkTreeViewColumn * charge_column = gtk_tree_view_column_new_with_attributes("主要罪名", renderer, "text", CHARGE_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(charge_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(charge_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(charge_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), charge_column);
	    GtkTreeViewColumn * amount_column = gtk_tree_view_column_new_with_attributes("涉案金额", renderer, "text", AMOUNT_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(amount_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(amount_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), amount_column);
	    GtkTreeViewColumn * punishment_column = gtk_tree_view_column_new_with_attributes("刑事处罚", renderer, "text", PUNISHMENT_COLUMN, NULL);
	    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(punishment_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
	    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(punishment_column), TRUE);
	    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(punishment_column), TRUE);
	    gtk_tree_view_append_column(GTK_TREE_VIEW(case_list), punishment_column);

	    GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), case_list);

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
