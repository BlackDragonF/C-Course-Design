/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSQueryProvinceUI.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly includes callback functions when query
buttons about province clicked.
Function List: 
1.on_province_name_query_clicked    called back when query-
province-by-name-button clicked. Run a dialog containing a 
list view to display the information.
2.on_province_head_query_clicked    called back when query-
province-by-head-button clicked. Run a dialog containing a 
list view to display the information.
************************************************************/

#include "ACSHeader.h"

extern GtkWidget * main_window;
extern ProvinceList head;
enum {PROVINCE_COLUMN, HEAD_COLUMN, TEL_COLUMN, PROVINCE_COLUMNS};

/*************************************************
Function: on_province_name_query_clicked
Description: called back when query-province-by-name-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_province_name_query_clicked(GtkWidget * widget)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("按省份名称查询", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * provincelabel = gtk_label_new("所属省份：");
    GtkWidget * provinceentry = gtk_entry_new();

    GtkWidget * hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), provincelabel, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), provinceentry, FALSE, FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    char province[8];
    strcpy(province, gtk_entry_get_text(GTK_ENTRY(provinceentry)));

    gtk_widget_destroy(dialog);
    if (result == GTK_RESPONSE_OK)
    {
        GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
        GtkWidget * resultdialog = gtk_dialog_new_with_buttons("查询结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
        gtk_window_set_icon(GTK_WINDOW(resultdialog), pixbuf);
        g_object_unref(pixbuf), pixbuf = NULL;

        GtkWidget * provincelabel = gtk_label_new("所属省份：");
        GtkWidget * headlabel = gtk_label_new("纪检负责人：");
        GtkWidget * tellabel = gtk_label_new("联系电话：");
        GtkWidget * provinceresult = gtk_label_new("未找到对应省份");
        GtkWidget * headresult = gtk_label_new("未找到对应省份");
        GtkWidget * telresult = gtk_label_new("未找到对应省份");

        GtkWidget * table = gtk_table_new(3, 2, FALSE);

        ProvinceNode result_node = seek_province_name(head, province);
        if (result_node != NULL)
        {
            gtk_label_set_text(GTK_LABEL(provinceresult), result_node->content.province);
            gtk_label_set_text(GTK_LABEL(headresult), result_node->content.head);
            gtk_label_set_text(GTK_LABEL(telresult), result_node->content.tel);
        }

        gtk_table_attach_defaults(GTK_TABLE(table), provincelabel, 0, 1, 0, 1);
        gtk_table_attach_defaults(GTK_TABLE(table), headlabel, 0, 1, 1, 2);
        gtk_table_attach_defaults(GTK_TABLE(table), tellabel, 0, 1, 2, 3);
        gtk_table_attach_defaults(GTK_TABLE(table), provinceresult, 1, 2, 0, 1);
        gtk_table_attach_defaults(GTK_TABLE(table), headresult, 1, 2, 1, 2);
        gtk_table_attach_defaults(GTK_TABLE(table), telresult, 1, 2, 2, 3);
        gtk_table_set_row_spacings(GTK_TABLE(table), 5);
        gtk_table_set_col_spacings(GTK_TABLE(table), 5);
        gtk_container_set_border_width(GTK_CONTAINER(table), 5);
        gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(resultdialog)->vbox), table);

        gtk_window_set_position(GTK_WINDOW(resultdialog), GTK_WIN_POS_CENTER);

        gtk_widget_show_all(resultdialog);
        gtk_dialog_run(GTK_DIALOG(resultdialog));

        gtk_widget_destroy(resultdialog);
    }
}

/*************************************************
Function: on_province_head_query_clicked
Description: called back when query-province-by-head-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_province_head_query_clicked(GtkWidget * widget)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("按纪检负责人查询", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * headlabel = gtk_label_new("纪检负责人：");
    GtkWidget * headentry = gtk_entry_new();

    GtkWidget * hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), headlabel, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), headentry, FALSE, FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    char headname[15];
    strcpy(headname, gtk_entry_get_text(GTK_ENTRY(headentry)));

    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
    {
        GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
        GtkWidget * dialog = gtk_dialog_new_with_buttons("查询结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
        gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
        g_object_unref(pixbuf), pixbuf = NULL;

        GtkListStore * store = gtk_list_store_new(PROVINCE_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);
        GtkWidget * province_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
        GtkTreeIter iter;
        ProvinceList temp_head = seek_province_head(head, headname);
        ProvinceNode node = temp_head;
        while (node != NULL)
        {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, PROVINCE_COLUMN, node->content.province, HEAD_COLUMN, node->content.head, TEL_COLUMN, node->content.tel, -1);
            node = node->next;
        }
        free_province_list(temp_head);
        g_object_unref(store);

        GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn * province_column = gtk_tree_view_column_new_with_attributes("所属省份", renderer, "text", PROVINCE_COLUMN, NULL);
        gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(province_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
        gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(province_column), TRUE);
        gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(province_column), TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(province_list), province_column);
        GtkTreeViewColumn * head_column = gtk_tree_view_column_new_with_attributes("纪检负责人姓名", renderer, "text", HEAD_COLUMN, NULL);
        gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(head_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
        gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(head_column), TRUE);
        gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(head_column), TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(province_list), head_column);
        GtkTreeViewColumn * tel_column = gtk_tree_view_column_new_with_attributes("联系电话", renderer, "text", TEL_COLUMN, NULL);
        gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(tel_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
        gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(tel_column), TRUE);
        gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(tel_column), TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(province_list), tel_column);

        GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), province_list);

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
