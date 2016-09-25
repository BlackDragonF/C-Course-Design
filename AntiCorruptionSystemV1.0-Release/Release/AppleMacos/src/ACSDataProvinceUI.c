/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSDataProvinceUI.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly bridges the UI layer and data layer. This
includes building certain UI, providing certain signal-event 
drives and calls data functions.This module mainly manages
province data and UI.
Function List: 
1.province_method               configure a window to display 
information(listview) about province. 
2.on_province_add_clicked       called back when add button
clicked.Run a dialog for information adding.
3.on_province_modify_clicked    called back when modify button
clicked.Run a dialog for information modifying.
4.on_province_delete_clicked    called back when delete button
clicked.Delete certain province node.
5.reload_province_list          reload all data, this is 
usually because of the change of data such as adding, 
modifying and deleting data.
************************************************************/

#include "ACSHeader.h"

GtkWidget * province_list;
enum {PROVINCE_COLUMN, HEAD_COLUMN, TEL_COLUMN, PROVINCE_ADDRESS_COLUMN, PROVINCE_COLUMNS};

extern ProvinceList head;
extern GtkWidget * main_window;

/*************************************************
Function: province_method
Description: configure a window to display information(listview) about province. 
Input: None
Output: None
Return: None
Others: None
*************************************************/
void province_method(void)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_icon(GTK_WINDOW(window), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;
    gtk_window_set_title(GTK_WINDOW(window), "省份监管信息管理");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_window));
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_widget_set_usize(window, 640, 480);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);

    GtkWidget * addbtn = gtk_button_new_with_label("录入");
    GtkWidget * modifybtn = gtk_button_new_with_label("修改");
    GtkWidget * deletebtn = gtk_button_new_with_label("删除");
    GtkWidget * cancelbtn = gtk_button_new_with_label("取消");
    gtk_widget_set_usize(addbtn, 72, 24);
    gtk_widget_set_usize(modifybtn, 72, 24);
    gtk_widget_set_usize(deletebtn, 72, 24);
    gtk_widget_set_usize(cancelbtn, 72, 24);

    GtkListStore * store = gtk_list_store_new(PROVINCE_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);
    province_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
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

    GtkWidget * hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox), cancelbtn, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox), deletebtn, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox), modifybtn, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox), addbtn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(G_OBJECT(addbtn), "clicked", G_CALLBACK(on_province_add_clicked), window);
    g_signal_connect(G_OBJECT(modifybtn), "clicked", G_CALLBACK(on_province_modify_clicked), window);
    g_signal_connect(G_OBJECT(deletebtn), "clicked", G_CALLBACK(on_province_delete_clicked), NULL);
    g_signal_connect(G_OBJECT(cancelbtn), "clicked", G_CALLBACK(on_cancel_clicked), window);

    reload_province_list();

    gtk_widget_show_all(window);
}

/*************************************************
Function: on_province_add_clicked
Description: called back when add button clicked.Run a dialog for information adding.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer data indicating the window that passes
Output: None
Return: None
Others: None
*************************************************/
void on_province_add_clicked(GtkWidget * widget, gpointer data)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("省份监管信息录入", GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * provincelabel = gtk_label_new("所属省份：");
    GtkWidget * headlabel = gtk_label_new("纪检负责人：");
    GtkWidget * tellabel = gtk_label_new("联系电话：");
    GtkWidget * provinceentry = gtk_entry_new();
    GtkWidget * headentry = gtk_entry_new();
    GtkWidget * telentry = gtk_entry_new();

    GtkWidget * table = gtk_table_new(3, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), provincelabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), headlabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), tellabel, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), provinceentry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), headentry, 1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), telentry, 1, 2, 2, 3);
    gtk_table_set_row_spacings(GTK_TABLE(table), 5);
    gtk_table_set_col_spacings(GTK_TABLE(table), 5);
    gtk_container_set_border_width(GTK_CONTAINER(table), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), table);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_widget_show_all(dialog);

    int error;
    do
    {
        error = 1;
        gint result = gtk_dialog_run(GTK_DIALOG(dialog));
        switch (result)
        {
            case GTK_RESPONSE_OK:
            if (*gtk_entry_get_text(GTK_ENTRY(provinceentry)) == '\0')
            {
                error_message_dialog("错误", "所属省份不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(headentry)) == '\0')
            {
                error_message_dialog("错误", "纪检负责人姓名不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(telentry)) == '\0')
            {
                error_message_dialog("错误", "联系电话不能为空！");
            }
            else if (is_unique_province(head, gtk_entry_get_text(GTK_ENTRY(provinceentry))) == 0)
            {
                error_message_dialog("错误","所属省份重复！");
            }
            else
            {
                Province temp;
                strcpy(temp.province, gtk_entry_get_text(GTK_ENTRY(provinceentry)));
                strcpy(temp.head, gtk_entry_get_text(GTK_ENTRY(headentry)));
                strcpy(temp.tel, gtk_entry_get_text(GTK_ENTRY(telentry)));
                insert_province(head, head, temp);
                reload_province_list();

                error = 0;
            }
            break;
            default:
            error = 0;
            break;
        }
    } while (error != 0);

    gtk_widget_destroy(dialog);
}

/*************************************************
Function: on_province_modify_clicked
Description: called back when modify button clicked. Run a dialog for information modifying.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer data indicating the window that passes
Output: None
Return: None
Others: None
*************************************************/
void on_province_modify_clicked(GtkWidget * widget, gpointer data)
{
    GtkTreeIter iter;
    GtkTreeModel * model;
    ProvinceNode node = NULL;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(gtk_tree_view_get_selection(GTK_TREE_VIEW(province_list))), &model, &iter))
    {
        gtk_tree_model_get(model, &iter, PROVINCE_ADDRESS_COLUMN, &node, -1);
    }
    else
    {
        warning_message_dialog("未选中任何条目", "请先选中一个条目");
        return;
    }

    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("省份监管信息维护", GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * provincelabel = gtk_label_new("所属省份：");
    GtkWidget * headlabel = gtk_label_new("纪检负责人：");
    GtkWidget * tellabel = gtk_label_new("联系电话：");

    GtkWidget * provinceentry = gtk_entry_new();
    GtkWidget * headentry = gtk_entry_new();
    GtkWidget * telentry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(provinceentry), node->content.province);
    gtk_entry_set_text(GTK_ENTRY(headentry), node->content.head);
    gtk_entry_set_text(GTK_ENTRY(telentry), node->content.tel);

    GtkWidget * table = gtk_table_new(3, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), provincelabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), headlabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), tellabel, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), provinceentry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), headentry, 1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), telentry, 1, 2, 2, 3);
    gtk_table_set_row_spacings(GTK_TABLE(table), 5);
    gtk_table_set_col_spacings(GTK_TABLE(table), 5);
    gtk_container_set_border_width(GTK_CONTAINER(table), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), table);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_widget_show_all(dialog);

    int error;
    do
    {
        error = 1;
        gint result = gtk_dialog_run(GTK_DIALOG(dialog));
        switch (result)
        {
            case GTK_RESPONSE_OK:
            if (*gtk_entry_get_text(GTK_ENTRY(provinceentry)) == '\0')
            {
                error_message_dialog("错误", "所属省份不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(headentry)) == '\0')
            {
                error_message_dialog("错误", "纪检负责人姓名不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(telentry)) == '\0')
            {
                error_message_dialog("错误", "联系电话不能为空！");
            }
            else if ((is_unique_province(head, gtk_entry_get_text(GTK_ENTRY(provinceentry))) == 0) && (strcmp(node->content.province, gtk_entry_get_text(GTK_ENTRY(provinceentry))) != 0))
            {
                error_message_dialog("错误","所属省份重复！");
            }
            else
            {
                if (strcmp(node->content.province, gtk_entry_get_text(GTK_ENTRY(provinceentry))) != 0)
                {
                    CaseNode case_node = node->case_head;
                    while (case_node != NULL)
                    {
                        strcpy(case_node->content.province, gtk_entry_get_text(GTK_ENTRY(provinceentry)));
                        MediaNode media_node = case_node->media_head;
                        while (media_node != NULL)
                        {
                            strcpy(media_node->content.province, gtk_entry_get_text(GTK_ENTRY(provinceentry)));
                            media_node = media_node->next;
                        }
                        case_node = case_node->next;
                    }
                }
                strcpy(node->content.province, gtk_entry_get_text(GTK_ENTRY(provinceentry)));
                strcpy(node->content.head, gtk_entry_get_text(GTK_ENTRY(headentry)));
                strcpy(node->content.tel, gtk_entry_get_text(GTK_ENTRY(telentry)));
                reload_province_list();

                error = 0;
            }
            break;
            default:
            error = 0;
            break;
        }
    } while (error != 0);

    gtk_widget_destroy(dialog);

}

/*************************************************
Function: on_province_delete_clicked
Description: called back when delete button clicked. Delete certain media node.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_province_delete_clicked(GtkWidget * widget)
{
    GtkTreeIter iter;
    GtkTreeModel * model;
    ProvinceNode node;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(gtk_tree_view_get_selection(GTK_TREE_VIEW(province_list))), &model, &iter))
    {
        gtk_tree_model_get(model, &iter, PROVINCE_ADDRESS_COLUMN, &node, -1);
    }
    else
    {
        warning_message_dialog("未选中任何条目", "请先选中一个条目");
        return;
    }
    delete_province(head, node);
    reload_province_list();
}

/*************************************************
Function: reload_province_list
Description: reload all data, this usually because of the change of data such as adding, modifying and deleting data.
Input: None
Output: None
Return: None
Others: None
*************************************************/
void reload_province_list(void)
{
    GtkListStore * store;
    GtkTreeModel * model;
    GtkTreeIter iter;
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(province_list)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(province_list));
    gtk_list_store_clear(store);
    ProvinceNode node = head->next;
    while (node != NULL)
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, PROVINCE_COLUMN, node->content.province, HEAD_COLUMN, node->content.head, TEL_COLUMN, node->content.tel, PROVINCE_ADDRESS_COLUMN, node, -1);
        node = node->next;
    }
}
