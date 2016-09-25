/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSDataCaseUI.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly bridges the UI layer and data layer. This
includes building certain UI, providing certain signal-event 
drives and calls data functions.This module mainly manages
case data and UI.
Function List: 
1.case_method               configure a window to display 
information(listview) about case. 
2.on_case_add_clicked       called back when add button
clicked.Run a dialog for information adding.
3.on_case_modify_clicked    called back when modify button
clicked.Run a dialog for information modifying.
4.on_case_delete_clicked    called back when delete button
clicked.Delete certain case node.
5.reload_case_list          reload all data, this usually
because of the change of data such as adding, modifying and
deleting data.
************************************************************/

#include "ACSHeader.h"

GtkWidget * case_list;

enum {NUMBER_COLUMN, DATE_COLUMN, CASE_PROVINCE_COLUMN, DEFENDANT_COLUMN, LEVEL_COLUMN, CHARGE_COLUMN, AMOUNT_COLUMN, PUNISHMENT_COLUMN, CASE_PROVINCE_ADDRESS_COLUMN, CASE_ADDRESS_COLUMN, CASE_COLUMNS};

extern ProvinceList head;
extern GtkWidget * main_window;

/*************************************************
Function: case_method
Description: configure a window to display information(listview) about case. 
Input: None
Output: None
Return: None
Others: None
*************************************************/
void case_method()
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_icon(GTK_WINDOW(window), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;
    gtk_window_set_title(GTK_WINDOW(window), "贪腐案件信息管理");
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

    GtkListStore * store = gtk_list_store_new(CASE_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
    case_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
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

    GtkWidget * hbox = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox), cancelbtn, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox), deletebtn, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox), modifybtn, FALSE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(hbox), addbtn, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(G_OBJECT(addbtn), "clicked", G_CALLBACK(on_case_add_clicked), window);
    g_signal_connect(G_OBJECT(modifybtn), "clicked", G_CALLBACK(on_case_modify_clicked), window);
    g_signal_connect(G_OBJECT(deletebtn), "clicked", G_CALLBACK(on_case_delete_clicked), NULL);
    g_signal_connect(G_OBJECT(cancelbtn), "clicked", G_CALLBACK(on_cancel_clicked), window);

    reload_case_list();

    gtk_widget_show_all(window);
}

/*************************************************
Function: on_case_add_clicked
Description: called back when add button clicked.Run a dialog for information adding.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer data indicating the window that passes
Output: None
Return: None
Others: None
*************************************************/
void on_case_add_clicked(GtkWidget * widget, gpointer data)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("贪腐案件信息录入", GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * numberlabel = gtk_label_new("案件编号：");
    GtkWidget * datelabel = gtk_label_new("立案日期：");
    GtkWidget * provincelabel = gtk_label_new("所属省份：");
    GtkWidget * defendantlabel = gtk_label_new("被告人姓名:");
    GtkWidget * levellabel = gtk_label_new("行政级别：");
    GtkWidget * chargelabel = gtk_label_new("主要罪名：");
    GtkWidget * amountlabel = gtk_label_new("涉案金额：");
    GtkWidget * punishmentlabel = gtk_label_new("刑事处罚：");

    GtkWidget * numberentry = gtk_entry_new();
    GtkWidget * dateentry = gtk_entry_new();
    GtkWidget * defendantentry = gtk_entry_new();
    GtkWidget * amountentry = gtk_entry_new();
    GtkWidget * punishmententry = gtk_entry_new();

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

    GtkListStore * level_store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "副科级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "正科级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "副处级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "正处级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "副厅级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "正厅级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "副部级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "正部级及以上", -1);
    GtkWidget * levelcombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(level_store));
    g_object_unref(level_store);
    GtkCellRenderer * levelrenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(levelcombo), levelrenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(levelcombo), levelrenderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 0);

    GtkListStore * charge_store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "受贿", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "贪污", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "挪用公款", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "渎职", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "滥用职权", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "其他", -1);
    GtkWidget * chargecombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(charge_store));
    g_object_unref(charge_store);
    GtkCellRenderer * chargerenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(chargecombo), chargerenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(chargecombo), chargerenderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(chargecombo), 0);

    GtkWidget * table = gtk_table_new(8, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), numberlabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), datelabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), provincelabel, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), defendantlabel, 0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), levellabel, 0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), chargelabel, 0, 1, 5, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), amountlabel, 0, 1, 6, 7);
    gtk_table_attach_defaults(GTK_TABLE(table), punishmentlabel, 0, 1, 7, 8);

    gtk_table_attach_defaults(GTK_TABLE(table), numberentry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), dateentry, 1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), provincecombo, 1, 2, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), defendantentry, 1, 2, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), levelcombo, 1, 2, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), chargecombo, 1, 2, 5, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), amountentry, 1, 2, 6, 7);
    gtk_table_attach_defaults(GTK_TABLE(table), punishmententry, 1, 2, 7, 8);

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
            if (*gtk_entry_get_text(GTK_ENTRY(numberentry)) == '\0')
            {
                error_message_dialog("错误", "案件编号不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(dateentry)) == '\0')
            {
                error_message_dialog("错误", "立案日期不能为空！");
            }
            else if (gtk_combo_box_get_active(GTK_COMBO_BOX(provincecombo)) == -1)
            {
                error_message_dialog("错误", "请先录入对应的省份信息！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(defendantentry)) == '\0')
            {
                error_message_dialog("错误", "被告人姓名不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(amountentry)) == '\0')
            {
                error_message_dialog("错误", "涉案金额不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(punishmententry)) == '\0')
            {
                error_message_dialog("错误", "刑事处罚不能为空！");
            }
            else if (is_unique_case(province_with_name(head, gtk_combo_box_get_active_text(GTK_COMBO_BOX(provincecombo)))->case_head, gtk_entry_get_text(GTK_ENTRY(numberentry))) == 0)
            {
                error_message_dialog("错误", "案件编号重复！");
            }
            else if (atof(gtk_entry_get_text(GTK_ENTRY(amountentry))) == 0)
            {
                error_message_dialog("错误", "涉案金额的格式不能被识别，请重新输入！");
            }
            else if (atof(gtk_entry_get_text(GTK_ENTRY(amountentry))) < 0)
            {
                error_message_dialog("错误", "涉案金额不可以是负数，请重新输入！");
            }
            else if (atoi(gtk_entry_get_text(GTK_ENTRY(punishmententry))) == 0)
            {
                error_message_dialog("错误", "刑事处罚的格式不能被识别，请重新输入！");
            }
            else
            {
                Case temp;
                strcpy(temp.number, gtk_entry_get_text(GTK_ENTRY(numberentry)));
                strcpy(temp.date, gtk_entry_get_text(GTK_ENTRY(dateentry)));
                strcpy(temp.province, gtk_combo_box_get_active_text(GTK_COMBO_BOX(provincecombo)));
                strcpy(temp.defendant, gtk_entry_get_text(GTK_ENTRY(defendantentry)));

                if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 0)
                    temp.level = 'A';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 1)
                    temp.level = 'B';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 2)
                    temp.level = 'C';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 3)
                    temp.level = 'D';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 4)
                    temp.level = 'E';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 5)
                    temp.level = 'F';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 6)
                    temp.level = 'G';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 7)
                    temp.level = 'H';

                if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 0)
                    temp.charge = '1';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 1)
                    temp.charge = '2';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 2)
                    temp.charge = '3';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 3)
                    temp.charge = '4';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 4)
                    temp.charge = '5';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 5)
                    temp.charge = '6';

                temp.amount = atof(gtk_entry_get_text(GTK_ENTRY(amountentry)));
                temp.punishment = atoi(gtk_entry_get_text(GTK_ENTRY(punishmententry)));

                ProvinceNode province_node = province_with_name(head, temp.province);
                CaseList case_head = province_node->case_head;

                if (case_head == NULL)
                {
                    province_node->case_head = create_case_list(temp);
                }
                else
                {
                    insert_case(case_head, case_head, temp);
                }

                reload_case_list();

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
Function: on_case_modify_clicked
Description: called back when modify button clicked.Run a dialog for information modifying.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer data indicating the window that passes
Output: None
Return: None
Others: None
*************************************************/
void on_case_modify_clicked(GtkWidget * widget, gpointer data)
{
    GtkTreeIter iter;
    GtkTreeModel * model;
    ProvinceNode province_node;
    CaseNode case_node;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(gtk_tree_view_get_selection(GTK_TREE_VIEW(case_list))), &model, &iter))
    {
        gtk_tree_model_get(model, &iter, CASE_PROVINCE_ADDRESS_COLUMN, &province_node, CASE_ADDRESS_COLUMN, &case_node,-1);
    }
    else
    {
        warning_message_dialog("未选中任何条目", "请先选中一个条目");
        return;
    }

    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("贪腐案件信息录入", GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * numberlabel = gtk_label_new("案件编号：");
    GtkWidget * datelabel = gtk_label_new("立案日期：");
    GtkWidget * provincelabel = gtk_label_new("所属省份：");
    GtkWidget * defendantlabel = gtk_label_new("被告人姓名:");
    GtkWidget * levellabel = gtk_label_new("行政级别：");
    GtkWidget * chargelabel = gtk_label_new("主要罪名：");
    GtkWidget * amountlabel = gtk_label_new("涉案金额：");
    GtkWidget * punishmentlabel = gtk_label_new("刑事处罚：");

    GtkWidget * numberentry = gtk_entry_new();
    GtkWidget * dateentry = gtk_entry_new();
    GtkWidget * defendantentry = gtk_entry_new();
    GtkWidget * amountentry = gtk_entry_new();
    GtkWidget * punishmententry = gtk_entry_new();

    GtkListStore * level_store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "副科级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "正科级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "副处级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "正处级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "副厅级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "正厅级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "副部级", -1);
    gtk_list_store_append(level_store, &iter);
    gtk_list_store_set(level_store, &iter, 0, "正部级及以上", -1);
    GtkWidget * levelcombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(level_store));
    g_object_unref(level_store);
    GtkCellRenderer * levelrenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(levelcombo), levelrenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(levelcombo), levelrenderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 0);

    GtkListStore * charge_store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "受贿", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "贪污", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "挪用公款", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "渎职", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "滥用职权", -1);
    gtk_list_store_append(charge_store, &iter);
    gtk_list_store_set(charge_store, &iter, 0, "其他", -1);
    GtkWidget * chargecombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(charge_store));
    g_object_unref(charge_store);
    GtkCellRenderer * chargerenderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(chargecombo), chargerenderer, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(chargecombo), chargerenderer, "text", 0, NULL);
    gtk_combo_box_set_active(GTK_COMBO_BOX(chargecombo), 0);

    char amount[40], punishment[10];
    gtk_entry_set_text(GTK_ENTRY(numberentry), case_node->content.number);
    gtk_entry_set_text(GTK_ENTRY(dateentry), case_node->content.date);
    GtkWidget * provincecontent = gtk_label_new(case_node->content.province);
    gtk_entry_set_text(GTK_ENTRY(defendantentry), case_node->content.defendant);

    if (case_node->content.level == 'A')
        gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 0);
    else if (case_node->content.level == 'B')
        gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 1);
    else if (case_node->content.level == 'C')
        gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 2);
    else if (case_node->content.level == 'D')
        gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 3);
    else if (case_node->content.level == 'E')
        gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 4);
    else if (case_node->content.level == 'F')
        gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 5);
    else if (case_node->content.level == 'G')
        gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 6);
    else if (case_node->content.level == 'H')
        gtk_combo_box_set_active(GTK_COMBO_BOX(levelcombo), 7);

    if (case_node->content.charge == '1')
        gtk_combo_box_set_active(GTK_COMBO_BOX(chargecombo), 0);
    else if (case_node->content.charge == '2')
        gtk_combo_box_set_active(GTK_COMBO_BOX(chargecombo), 1);
    else if (case_node->content.charge == '3')
        gtk_combo_box_set_active(GTK_COMBO_BOX(chargecombo), 2);
    else if (case_node->content.charge == '4')
        gtk_combo_box_set_active(GTK_COMBO_BOX(chargecombo), 3);
    else if (case_node->content.charge == '5')
        gtk_combo_box_set_active(GTK_COMBO_BOX(chargecombo), 4);
    else if (case_node->content.charge == '6')
        gtk_combo_box_set_active(GTK_COMBO_BOX(chargecombo), 5);

    snprintf(amount, 40, "%f", case_node->content.amount);
    gtk_entry_set_text(GTK_ENTRY(amountentry), amount);
    snprintf(punishment, 10, "%d", case_node->content.punishment);
    gtk_entry_set_text(GTK_ENTRY(punishmententry), punishment);

    GtkWidget * table = gtk_table_new(8, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(table), numberlabel, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), datelabel, 0, 1, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), provincelabel, 0, 1, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), defendantlabel, 0, 1, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), levellabel, 0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), chargelabel, 0, 1, 5, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), amountlabel, 0, 1, 6, 7);
    gtk_table_attach_defaults(GTK_TABLE(table), punishmentlabel, 0, 1, 7, 8);

    gtk_table_attach_defaults(GTK_TABLE(table), numberentry, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), dateentry, 1, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table), provincecontent, 1, 2, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(table), defendantentry, 1, 2, 3, 4);
    gtk_table_attach_defaults(GTK_TABLE(table), levelcombo, 1, 2, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(table), chargecombo, 1, 2, 5, 6);
    gtk_table_attach_defaults(GTK_TABLE(table), amountentry, 1, 2, 6, 7);
    gtk_table_attach_defaults(GTK_TABLE(table), punishmententry, 1, 2, 7, 8);

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
            if (*gtk_entry_get_text(GTK_ENTRY(numberentry)) == '\0')
            {
                error_message_dialog("错误", "案件编号不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(dateentry)) == '\0')
            {
                error_message_dialog("错误", "立案日期不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(defendantentry)) == '\0')
            {
                error_message_dialog("错误", "被告人姓名不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(amountentry)) == '\0')
            {
                error_message_dialog("错误", "涉案金额不能为空！");
            }
            else if (*gtk_entry_get_text(GTK_ENTRY(punishmententry)) == '\0')
            {
                error_message_dialog("错误", "刑事处罚不能为空！");
            }
            else if ((is_unique_case(province_node->case_head, gtk_entry_get_text(GTK_ENTRY(numberentry))) == 0) && (strcmp(case_node->content.number, gtk_entry_get_text(GTK_ENTRY(numberentry))) != 0))
            {
                error_message_dialog("错误", "案件编号重复！");
            }
            else if (atof(gtk_entry_get_text(GTK_ENTRY(amountentry))) == 0)
            {
                error_message_dialog("错误", "涉案金额的格式不能被识别，请重新输入！");
            }
            else if (atof(gtk_entry_get_text(GTK_ENTRY(amountentry))) < 0)
            {
                error_message_dialog("错误", "涉案金额不可以是负数，请重新输入！");
            }
            else if (atoi(gtk_entry_get_text(GTK_ENTRY(punishmententry))) == 0)
            {
                error_message_dialog("错误", "刑事处罚的格式不能被识别，请重新输入！");
            }
            else
            {
                if (strcmp(case_node->content.number, gtk_entry_get_text(GTK_ENTRY(numberentry))) != 0)
                {
                    strcpy(case_node->content.number, gtk_entry_get_text(GTK_ENTRY(numberentry)));
                    MediaNode node = case_node->media_head;
                    while (node != NULL)
                    {
                        strcpy(node->content.number, gtk_entry_get_text(GTK_ENTRY(numberentry)));
                        node = node->next;
                    }
                }
                strcpy(case_node->content.date, gtk_entry_get_text(GTK_ENTRY(dateentry)));
                strcpy(case_node->content.defendant, gtk_entry_get_text(GTK_ENTRY(defendantentry)));

                if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 0)
                    case_node->content.level = 'A';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 1)
                    case_node->content.level = 'B';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 2)
                    case_node->content.level = 'C';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 3)
                    case_node->content.level = 'D';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 4)
                    case_node->content.level = 'E';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 5)
                    case_node->content.level = 'F';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 6)
                    case_node->content.level = 'G';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(levelcombo)) == 7)
                    case_node->content.level = 'H';

                if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 0)
                    case_node->content.charge = '1';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 1)
                    case_node->content.charge = '2';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 2)
                    case_node->content.charge = '3';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 3)
                    case_node->content.charge = '4';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 4)
                    case_node->content.charge = '5';
                else if (gtk_combo_box_get_active(GTK_COMBO_BOX(chargecombo)) == 5)
                    case_node->content.charge = '6';

                case_node->content.amount = atof(gtk_entry_get_text(GTK_ENTRY(amountentry)));
                case_node->content.punishment = atoi(gtk_entry_get_text(GTK_ENTRY(punishmententry)));

                reload_case_list();

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
Function: on_case_delete_clicked
Description: called back when delete button clicked. Delete certain case node.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_case_delete_clicked(GtkWidget * widget)
{
    GtkTreeIter iter;
    GtkTreeModel * model;
    ProvinceNode province_node = NULL;
    CaseNode case_node = NULL;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(gtk_tree_view_get_selection(GTK_TREE_VIEW(case_list))), &model, &iter))
    {
        gtk_tree_model_get(model, &iter, CASE_PROVINCE_ADDRESS_COLUMN, &province_node, CASE_ADDRESS_COLUMN, &case_node, -1);
    }
    else
    {
        warning_message_dialog("未选中任何条目", "请先选中一个条目");
        return;
    }
    delete_case(&province_node->case_head, case_node);
    reload_case_list();
}

/*************************************************
Function: reload_case_list
Description: reload all data, this usually because of the change of data such as adding, modifying and deleting data.
Input: None
Output: None
Return: None
Others: None
*************************************************/
void reload_case_list(void)
{
    GtkListStore * store;
    GtkTreeModel * model;
    GtkTreeIter iter;
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(case_list)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(case_list));
    gtk_list_store_clear(store);
    ProvinceNode province_node = head;
    CaseNode case_node = NULL;
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
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
            gtk_list_store_set(store, &iter, NUMBER_COLUMN, case_node->content.number, DATE_COLUMN, case_node->content.date, CASE_PROVINCE_COLUMN, case_node->content.province, DEFENDANT_COLUMN, case_node->content.defendant, LEVEL_COLUMN, level, CHARGE_COLUMN, charge, AMOUNT_COLUMN, case_node->content.amount, PUNISHMENT_COLUMN, case_node->content.punishment, CASE_PROVINCE_ADDRESS_COLUMN, province_node, CASE_ADDRESS_COLUMN, case_node, -1);
            
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
}

