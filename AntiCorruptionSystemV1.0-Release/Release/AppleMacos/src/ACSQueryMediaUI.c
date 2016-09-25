/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSQueryMediaUI.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly includes callback functions when query
buttons about media clicked.
Function List: 
1.on_media_number_and_name_query_clicked    called back when 
query-media-by-number-and-name-button clicked. Run a dialog 
containing a list view to display the information.
2.on_media_date_and_partname_query_clicked    called back 
when query-media-by-date-and-partname-button clicked. Run a 
dialog containing a list view to display the information.
************************************************************/

#include "ACSHeader.h"

extern GtkWidget * main_window;
extern ProvinceList head;

enum {MEDIA_NUMBER_COLUMN, MEDIA_DATE_COLUMN, TYPE_COLUMN, NAME_COLUMN, INDEX_COLUMN, MEDIA_COLUMNS};

/*************************************************
Function: on_media_number_and_name_query_clicked
Description: called back when query-media-by-number-and-name-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_media_number_and_name_query_clicked(GtkWidget * widget)
{
	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("按案件编号和媒体名称查询", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * numberlabel = gtk_label_new("案件编号：");
    GtkWidget * namelabel = gtk_label_new("媒体名称：");
    GtkWidget * numberentry = gtk_entry_new();
    GtkWidget * nameentry = gtk_entry_new();

    GtkWidget * table = gtk_table_new(2, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), numberlabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), namelabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), numberentry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), nameentry, 1, 2, 1, 2);

    gtk_table_set_row_spacings(GTK_TABLE(table), 5);
    gtk_table_set_col_spacings(GTK_TABLE(table), 5);
    gtk_container_set_border_width(GTK_CONTAINER(table), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), table);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    char number[12], name[20];
    strcpy(number, gtk_entry_get_text(GTK_ENTRY(numberentry)));
    strcpy(name, gtk_entry_get_text(GTK_ENTRY(nameentry)));

    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
    {
    	GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
        GtkWidget * dialog = gtk_dialog_new_with_buttons("查询结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
        gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
        g_object_unref(pixbuf), pixbuf = NULL;

    	GtkListStore * store = gtk_list_store_new(MEDIA_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER, G_TYPE_POINTER, G_TYPE_POINTER);
	    GtkWidget * media_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
		GtkTreeIter iter;
	    MediaList temp_head = seek_media_number_name(head, number, name);
	    MediaNode media_node = temp_head;
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
            gtk_list_store_set(store, &iter, MEDIA_NUMBER_COLUMN, media_node->content.number, MEDIA_DATE_COLUMN, media_node->content.date, TYPE_COLUMN, type, NAME_COLUMN, media_node->content.name, INDEX_COLUMN, media_node->content.index, -1);
            
            media_node = media_node->next;
        }
        free_media_list(temp_head);
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
        gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
        gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);
        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

        gtk_widget_show_all(dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));

        gtk_widget_destroy(dialog);
    }
}

/*************************************************
Function: on_media_date_and_partname_query_clicked
Description: called back when query-media-by-date-and-partname-button clicked. Run a dialog containing a list view to display the information.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_media_date_and_partname_query_clicked(GtkWidget * widget)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("按报道日期和媒体部分名称查询", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * datelabel = gtk_label_new("报道日期：");
    GtkWidget * partnamelabel = gtk_label_new("媒体部分名称：");
    GtkWidget * dateentry = gtk_entry_new();
    GtkWidget * partnameentry = gtk_entry_new();

    GtkWidget * table = gtk_table_new(2, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), datelabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), partnamelabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), dateentry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), partnameentry, 1, 2, 1, 2);

    gtk_table_set_row_spacings(GTK_TABLE(table), 5);
    gtk_table_set_col_spacings(GTK_TABLE(table), 5);
    gtk_container_set_border_width(GTK_CONTAINER(table), 5);
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), table);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    char date[10], partname[20];
    strcpy(date, gtk_entry_get_text(GTK_ENTRY(dateentry)));
    strcpy(partname, gtk_entry_get_text(GTK_ENTRY(partnameentry)));

    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
    {
        GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
        GtkWidget * dialog = gtk_dialog_new_with_buttons("查询结果", GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
        gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
        gtk_widget_set_usize(GTK_WIDGET(dialog), 480, 360);
        g_object_unref(pixbuf), pixbuf = NULL;

        GtkListStore * store = gtk_list_store_new(MEDIA_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER, G_TYPE_POINTER, G_TYPE_POINTER);
        GtkWidget * media_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
        GtkTreeIter iter;
        MediaList temp_head = seek_media_date_partname(head, date, partname);
        MediaNode media_node = temp_head;
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
            gtk_list_store_set(store, &iter, MEDIA_NUMBER_COLUMN, media_node->content.number, MEDIA_DATE_COLUMN, media_node->content.date, TYPE_COLUMN, type, NAME_COLUMN, media_node->content.name, INDEX_COLUMN, media_node->content.index, -1);
            
            media_node = media_node->next;
        }
        free_media_list(temp_head);
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
        gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
        gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox);
        gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

        gtk_widget_show_all(dialog);
        gtk_dialog_run(GTK_DIALOG(dialog));
        
        gtk_widget_destroy(dialog);
    }
}
