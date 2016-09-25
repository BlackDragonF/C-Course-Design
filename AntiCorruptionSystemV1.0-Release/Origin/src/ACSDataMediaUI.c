/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSDataMediaUI.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly bridges the UI layer and data layer. This
includes building certain UI, providing certain signal-event 
drives and calls data functions.This module mainly manages
media data and UI.
Function List: 
1.media_method               configure a window to display 
information(listview) about media. 
2.on_media_add_clicked       called back when add button
clicked.Run a dialog for information adding.
3.on_media_modify_clicked    called back when modify button
clicked.Run a dialog for information modifying.
4.on_media_delete_clicked    called back when delete button
clicked.Delete certain media node.
5.reload_media_list          reload all data, this is usually
because of the change of data such as adding, modifying and
deleting data.
6.on_province_combo_changed  called back when province combo
changed and then number combo will be refreshed.
************************************************************/

#include "ACSHeader.h"

GtkWidget * media_list;
enum {MEDIA_NUMBER_COLUMN, MEDIA_DATE_COLUMN, TYPE_COLUMN, NAME_COLUMN, INDEX_COLUMN, MEDIA_PROVINCE_COLUMN, MEDIA_PROVINCE_ADDRESS_COLUMN, MEDIA_CASE_ADDRESS_COLUMN, MEDIA_ADDRESS_COLUMN, MEDIA_COLUMNS};

extern ProvinceList head;
extern GtkWidget * main_window;

/*************************************************
Function: media_method
Description: configure a window to display information(listview) about media. 
Input: None
Output: None
Return: None
Others: None
*************************************************/
void media_method(void)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_icon(GTK_WINDOW(window), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;
    gtk_window_set_title(GTK_WINDOW(window), "媒体报道信息管理");
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

    GtkListStore * store = gtk_list_store_new(MEDIA_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER, G_TYPE_POINTER, G_TYPE_POINTER);
    media_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);
    GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn * number_column = gtk_tree_view_column_new_with_attributes("案件编号", renderer, "text", MEDIA_NUMBER_COLUMN, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(number_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(number_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(media_list), number_column);
    GtkTreeViewColumn * date_column = gtk_tree_view_column_new_with_attributes("报道日期", renderer, "text", MEDIA_DATE_COLUMN, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(date_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(date_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(date_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(media_list), date_column);
    GtkTreeViewColumn * type_column = gtk_tree_view_column_new_with_attributes("媒体类别", renderer, "text", TYPE_COLUMN, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(type_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(type_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(type_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(media_list), type_column);
    GtkTreeViewColumn * name_column = gtk_tree_view_column_new_with_attributes("媒体名称", renderer, "text", NAME_COLUMN, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(name_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(name_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(media_list), name_column);
    GtkTreeViewColumn * index_column = gtk_tree_view_column_new_with_attributes("内容索引", renderer, "text", INDEX_COLUMN, NULL);
    gtk_tree_view_column_set_sizing(GTK_TREE_VIEW_COLUMN(index_column), GTK_TREE_VIEW_COLUMN_GROW_ONLY);
    gtk_tree_view_column_set_resizable(GTK_TREE_VIEW_COLUMN(index_column), TRUE);
    gtk_tree_view_column_set_expand(GTK_TREE_VIEW_COLUMN(index_column), TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(media_list), index_column);

    GtkWidget * scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), media_list);

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

    g_signal_connect(G_OBJECT(addbtn), "clicked", G_CALLBACK(on_media_add_clicked), window);
    g_signal_connect(G_OBJECT(modifybtn), "clicked", G_CALLBACK(on_media_modify_clicked), window);
    g_signal_connect(G_OBJECT(deletebtn), "clicked", G_CALLBACK(on_media_delete_clicked), NULL);
    g_signal_connect(G_OBJECT(cancelbtn), "clicked", G_CALLBACK(on_cancel_clicked), window);

    reload_media_list();

    gtk_widget_show_all(window);
}

/*************************************************
Function: on_media_add_clicked
Description: called back when add button clicked.Run a dialog for information adding.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer data indicating the window that passes
Output: None
Return: None
Others: None
*************************************************/
void on_media_add_clicked(GtkWidget * widget , gpointer data)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("媒体报道信息录入", GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * provincelabel = gtk_label_new("所属省份：");
    GtkWidget * numberlabel = gtk_label_new("案件编号：");
    GtkWidget * datelabel = gtk_label_new("报道日期：");
    GtkWidget * typelabel = gtk_label_new("媒体类别：");
    GtkWidget * namelabel = gtk_label_new("媒体名称：");
    GtkWidget * indexlabel = gtk_label_new("内容索引：");

    GtkWidget * dateentry = gtk_entry_new();
    GtkWidget * nameentry = gtk_entry_new();
    GtkWidget * indexentry = gtk_entry_new();

    GtkListStore * province_store = gtk_list_store_new(1, G_TYPE_STRING);
    GtkTreeIter iter;
    ProvinceNode node = head->next;
    while (node != NULL)
    {
        gtk_list_store_append(province_store, &iter);
        gtk_list_store_set(province_store, &iter, 0, node->content.province, -1);
        node = node->next;
    }
    GtkWidget * provincecombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(province_store));
    g_object_unref(province_store);
    GtkCellRenderer * provincerenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(provincecombo), provincerenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(provincecombo), provincerenderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(provincecombo), 0);

    GtkListStore * number_store = gtk_list_store_new(1, G_TYPE_STRING);
    CaseNode case_node = province_with_name(head, gtk_combo_box_get_active_text(GTK_COMBO_BOX(provincecombo)))->case_head;
    while (case_node != NULL)
    {
        gtk_list_store_append(number_store, &iter);
        gtk_list_store_set(number_store, &iter, 0, case_node->content.number, -1);
        case_node = case_node->next;
    }
    GtkWidget * numbercombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(number_store));
    g_object_unref(number_store);
    GtkCellRenderer * numberrenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(numbercombo), numberrenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(numbercombo), numberrenderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(numbercombo), 0);

    g_signal_connect(G_OBJECT(provincecombo), "changed", G_CALLBACK(on_province_combo_changed), numbercombo);

    GtkListStore * type_store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(type_store, &iter);
    gtk_list_store_set(type_store, &iter, 0, "报纸", -1);
    gtk_list_store_append(type_store, &iter);
    gtk_list_store_set(type_store, &iter, 0, "广播", -1);
    gtk_list_store_append(type_store, &iter);
    gtk_list_store_set(type_store, &iter, 0, "电视", -1);
    gtk_list_store_append(type_store, &iter);
    gtk_list_store_set(type_store, &iter, 0, "互联网", -1);
    GtkWidget * typecombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(type_store));
    g_object_unref(type_store);
    GtkCellRenderer * typerenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(typecombo), typerenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(typecombo), typerenderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(typecombo), 0);

    GtkWidget * table = gtk_table_new(6, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), provincelabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), numberlabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), datelabel, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), typelabel, 0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), namelabel, 0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), indexlabel, 0, 1, 5, 6);

    gtk_table_attach_defaults(GTK_TABLE(table), provincecombo, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), numbercombo, 1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), dateentry, 1, 2, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), typecombo, 1, 2, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), nameentry, 1, 2, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), indexentry, 1, 2, 5, 6);

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
            if (gtk_combo_box_get_active(GTK_COMBO_BOX(provincecombo)) == -1)
            {
                error_message_dialog("错误", "请先录入对应的省份信息！");
            }
            else if (gtk_combo_box_get_active(GTK_COMBO_BOX(numbercombo)) == -1)
            {
                error_message_dialog("错误", "请先录入对应的案件信息！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(dateentry)) == '\0')
            {
                error_message_dialog("错误", "报道日期不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(nameentry)) == '\0')
            {
                error_message_dialog("错误", "媒体名称不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(indexentry)) == '\0')
            {
                error_message_dialog("错误", "内容索引不能为空！");
            }
            else
            {
                Media temp;
                strcpy(temp.province, gtk_combo_box_get_active_text(GTK_COMBO_BOX(provincecombo)));
                strcpy(temp.number, gtk_combo_box_get_active_text(GTK_COMBO_BOX(numbercombo)));
                strcpy(temp.date, gtk_entry_get_text(GTK_ENTRY(dateentry)));

                if (gtk_combo_box_get_active(GTK_COMBO_BOX(typecombo)) == 0)
                    temp.type = '1';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(typecombo)) == 1)
                    temp.type = '2';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(typecombo)) == 2)
                    temp.type = '3';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(typecombo)) == 3)
                    temp.type = '4';

                strcpy(temp.name, gtk_entry_get_text(GTK_ENTRY(nameentry)));
                strcpy(temp.index, gtk_entry_get_text(GTK_ENTRY(indexentry)));

                ProvinceNode province_node = province_with_name(head, temp.province);
                CaseNode case_node = case_with_number(province_node->case_head, temp.number);
                MediaList media_head = case_node->media_head;
                if (media_head == NULL)
                {
                    case_node->media_head = create_media_list(temp);
                }
                else
                {
                    insert_media(media_head, media_head, temp);
                }

                reload_media_list();

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
Function: on_media_modify_clicked
Description: called back when modify button clicked. Run a dialog for information modifying.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer data indicating the window that passes
Output: None
Return: None
Others: None
*************************************************/
void on_media_modify_clicked(GtkWidget * widger, gpointer data)
{
    GtkTreeIter iter;
    GtkTreeModel * model;
    ProvinceNode province_node;
    CaseNode case_node;
    MediaNode media_node;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(gtk_tree_view_get_selection(GTK_TREE_VIEW(media_list))), &model, &iter))
    {
        gtk_tree_model_get(model, &iter, MEDIA_PROVINCE_ADDRESS_COLUMN, &province_node, MEDIA_CASE_ADDRESS_COLUMN, &case_node, MEDIA_ADDRESS_COLUMN, &media_node, -1);
    }
    else
    {
        warning_message_dialog("未选中任何条目", "请先选中一个条目");
        return;
    }

    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("媒体报道信息修改", GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * provincelabel = gtk_label_new("所属省份：");
    GtkWidget * numberlabel = gtk_label_new("案件编号：");
    GtkWidget * datelabel = gtk_label_new("报道日期：");
    GtkWidget * typelabel = gtk_label_new("媒体类别：");
    GtkWidget * namelabel = gtk_label_new("媒体名称：");
    GtkWidget * indexlabel = gtk_label_new("内容索引：");

    GtkWidget * dateentry = gtk_entry_new();
    GtkWidget * nameentry = gtk_entry_new();
    GtkWidget * indexentry = gtk_entry_new();

    GtkListStore * type_store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(type_store, &iter);
    gtk_list_store_set(type_store, &iter, 0, "报纸", -1);
    gtk_list_store_append(type_store, &iter);
    gtk_list_store_set(type_store, &iter, 0, "广播", -1);
    gtk_list_store_append(type_store, &iter);
    gtk_list_store_set(type_store, &iter, 0, "电视", -1);
    gtk_list_store_append(type_store, &iter);
    gtk_list_store_set(type_store, &iter, 0, "互联网", -1);
    GtkWidget * typecombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(type_store));
    g_object_unref(type_store);
    GtkCellRenderer * typerenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(typecombo), typerenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(typecombo), typerenderer, "text", 0, NULL);

    if (media_node->content.type == '1')
        gtk_combo_box_set_active(GTK_COMBO_BOX(typecombo), 0);
    else if (media_node->content.type == '2')
        gtk_combo_box_set_active(GTK_COMBO_BOX(typecombo), 1);
    else if (media_node->content.type == '3')
        gtk_combo_box_set_active(GTK_COMBO_BOX(typecombo), 2);
    else if (media_node->content.type == '4')
        gtk_combo_box_set_active(GTK_COMBO_BOX(typecombo), 3);

    GtkWidget * provincecontent = gtk_label_new(media_node->content.province);
    GtkWidget * numbercontent = gtk_label_new(media_node->content.number);
    gtk_entry_set_text(GTK_ENTRY(dateentry), media_node->content.date);
    gtk_entry_set_text(GTK_ENTRY(nameentry), media_node->content.name);
    gtk_entry_set_text(GTK_ENTRY(indexentry), media_node->content.index);

    GtkWidget * table = gtk_table_new(6, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), provincelabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), numberlabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), datelabel, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), typelabel, 0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), namelabel, 0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), indexlabel, 0, 1, 5, 6);

    gtk_table_attach_defaults(GTK_TABLE(table), provincecontent, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), numbercontent, 1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), dateentry, 1, 2, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), typecombo, 1, 2, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), nameentry, 1, 2, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), indexentry, 1, 2, 5, 6);

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
            if (*gtk_entry_get_text(GTK_ENTRY(dateentry)) == '\0')
            {
                error_message_dialog("错误", "报道日期不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(nameentry)) == '\0')
            {
                error_message_dialog("错误", "媒体名称不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(indexentry)) == '\0')
            {
                error_message_dialog("错误", "内容索引不能为空！");
            }
            else
            {
                strcpy(media_node->content.date, gtk_entry_get_text(GTK_ENTRY(dateentry)));

                if (gtk_combo_box_get_active(GTK_COMBO_BOX(typecombo)) == 0)
                    media_node->content.type = '1';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(typecombo)) == 1)
                    media_node->content.type = '2';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(typecombo)) == 2)
                    media_node->content.type = '3';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(typecombo)) == 3)
                    media_node->content.type = '4';

                strcpy(media_node->content.name, gtk_entry_get_text(GTK_ENTRY(nameentry)));
                strcpy(media_node->content.index, gtk_entry_get_text(GTK_ENTRY(indexentry)));

                reload_media_list();

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
Function: on_media_delete_clicked
Description: called back when delete button clicked. Delete certain media node.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_media_delete_clicked(GtkWidget * widget)
{
    GtkTreeIter iter;
    GtkTreeModel * model;
    ProvinceNode province_node;
    CaseNode case_node;
    MediaNode media_node;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(gtk_tree_view_get_selection(GTK_TREE_VIEW(media_list))), &model, &iter))
    {
        gtk_tree_model_get(model, &iter, MEDIA_PROVINCE_ADDRESS_COLUMN, &province_node, MEDIA_CASE_ADDRESS_COLUMN, &case_node, MEDIA_ADDRESS_COLUMN, &media_node, -1);
    }
    else
    {
        warning_message_dialog("未选中任何条目", "请先选中一个条目");
        return;
    }
    delete_media(&case_node->media_head, media_node);
    reload_media_list();
}

/*************************************************
Function: reload_media_list
Description: reload all data, this usually because of the change of data such as adding, modifying and deleting data.
Input: None
Output: None
Return: None
Others: None
*************************************************/
void reload_media_list(void)
{
    GtkListStore * store;
    GtkTreeModel * model;
    GtkTreeIter iter;
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(media_list)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(media_list));
    gtk_list_store_clear(store);
    ProvinceNode province_node = head;
    CaseNode case_node = NULL;
    MediaNode media_node = NULL;
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            media_node = case_node->media_head;
            while (media_node != NULL)
            {
                char type[13];

                if (media_node->content.type == '1')
                    strcpy(type, "报纸");
                else if (media_node->content.type == '2')
                    strcpy(type, "广播");
                else if (media_node->content.type == '3')
                    strcpy(type, "电视");
                else if (media_node->content.type == '4')
                    strcpy(type, "互联网");

                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, MEDIA_NUMBER_COLUMN, media_node->content.number, MEDIA_DATE_COLUMN, media_node->content.date, TYPE_COLUMN, type, NAME_COLUMN, media_node->content.name, INDEX_COLUMN, media_node->content.index, MEDIA_PROVINCE_COLUMN, media_node->content.province, MEDIA_PROVINCE_ADDRESS_COLUMN, province_node, MEDIA_CASE_ADDRESS_COLUMN, case_node, MEDIA_ADDRESS_COLUMN, media_node, -1);
                
                media_node = media_node->next;
            }
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
}

/*************************************************
Function: on_province_combo_changed
Description: called back when province combo changed and then number combo will be refreshed.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer indicating the number combo box than passes in
Output: None
Return: None
Others: None
*************************************************/
void on_province_combo_changed(GtkWidget * widget, gpointer data)
{
    GtkTreeIter iter;
    GtkListStore * store = GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(data)));
    GtkTreeModel * model = gtk_combo_box_get_model(GTK_COMBO_BOX(data));
    CaseNode case_node = province_with_name(head, gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget)))->case_head;
    gtk_list_store_clear(store);
    while (case_node != NULL)
    {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, case_node->content.number, -1);
        case_node = case_node->next;
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(data), 0);
    g_object_unref(store);
}
