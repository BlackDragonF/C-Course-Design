/*************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSStasticalUI.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly provides basic support for UI including
managing/configuring main windows and providing UI-related
callback functions.
Function List:
1.show_window                           configure the main
window including side bar, data-management window, data-
query window, data-stastical window as well as menu bar.
2.show_menubar                          configure the menu
bar providing file backup/restore function and about me
function.
3.show_sidebar                          configure the side
bar providing the switch function for the three main windows.
4.show_notebook                         note book is the basic
implementaion of the side bar and it is hidden.
5.show_dataview                         configure the data
view providing data-management functions.
6.show_queryview                        configure the query
view providing data-query functions.
7.show_stasticalview                    configure the stastical
view providing data-stastical functions.
8.on_data_clicked                       called back when data
button on the side bar clicked. Reconfigure the button on the
side bar and switch view.
9.on_query_clicked                      called back when query
button on the side bar clicked. Reconfigure the button on the
side bar and switch view.
10.on_stastical_clicked                  called back when
stastical button on the side bar clicked. Reconfigure the
button on the side bar and switch view.
11.on_province_clicked                  called back when
province button on the data view clicked. Reconfigure the
button on the data view.
12.on_case_clicked                      called back when case
button on the data view clicked. Reconfigure the button on the
data view.
13.on_media_clicked                     called back when media
button on the data view clicked. Reconfigure the button on the
data view.
14.on_confirm_clicked                   called back when
confirm button on the data view clicked. Poping up certain
window according to the button(province/case/media button)
user choose.
15.on_backup_clicked                    called back when
backup Mi on the menu bar activated. Open a file chooser for
user to save the backup file.
16.on_restore_clicked                   called back when
restore Mi on the menu bar activated. Open a file chooser for
user to choose the backup file.
17.on_aboutsystem_clicked               called back when
about-system Mi on the menu bar activated. Run a dialog to
display some basic information about the system.
18.on_aboutme_clicked                   called back when
about-me Mi on the menu bar activated. Run a dialog to display
some basic information about the author.
19.on_license_clicked                   called back when
license Mi on the menu bar activated. Run a dialog to display
some basic information about the license this program adopts.
20.on_cancel_clicked                    called back when
cancel button clicked and then the passing argument will be
destroyed.
*************************************************************/

#include "ACSHeader.h"

static GtkWidget * fixed;
static GtkWidget * notebook;
static GtkWidget * data, * query, * stastical;
static int state = 1;
static GtkWidget * dataview, * queryview, * stasticalview;
static GtkWidget * province, * cases, * media;
static int data_type = 1;

extern ProvinceList head;
extern GtkWidget * main_window;

/*************************************************
Function: show_window
Description: configure the main window
Input: GtkWidget ** pointer to the window widget
Output: None
Return: None
Others: None
*************************************************/
void show_window(GtkWidget ** pwindow)
{
    GdkPixbuf * icon;
    GtkWidget * window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    *pwindow = window;
    gtk_window_set_title(GTK_WINDOW(window), "反贪腐系统V1.0");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
    gtk_widget_set_usize(GTK_WIDGET(window), 960, 658);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    icon = create_pixbuf("img/icon.png");
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_object_unref(icon), icon = NULL;

    show_menubar(window);
    show_sidebar();
    show_notebook();
    show_dataview();
    show_queryview();
    show_stasticalview();

    gtk_widget_show_all(window);
}

/*************************************************
Function: show_menubar
Description: configure the menu bar
Input: GtkWidget * indicating main window
Output: None
Return: None
Others: None
*************************************************/
void show_menubar(GtkWidget * window)
{
    GtkWidget * menubar_vbox;

    GtkWidget * menubar;

    GtkWidget * filemenu;
    GtkWidget * aboutmenu;

    GtkWidget * fileMi;
    GtkWidget * backupMi;
    GtkWidget * restoreMi;
    GtkWidget * quitMi;
    GtkWidget * aboutMi;
    GtkWidget * meMi;
    GtkWidget * systemMi;
    GtkWidget * licenseMi;

    menubar_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(fixed), menubar_vbox);
    gtk_widget_set_usize(menubar_vbox, 960, 20);

    menubar = gtk_menu_bar_new();
    filemenu = gtk_menu_new();
    aboutmenu = gtk_menu_new();

    fileMi = gtk_menu_item_new_with_label("文件");
    backupMi = gtk_menu_item_new_with_label("数据备份");
    restoreMi = gtk_menu_item_new_with_label("数据还原");
    quitMi = gtk_menu_item_new_with_label("退出");
    aboutMi = gtk_menu_item_new_with_label("关于");
    meMi = gtk_menu_item_new_with_label("关于作者");
    systemMi = gtk_menu_item_new_with_label("关于本系统");
    licenseMi = gtk_menu_item_new_with_label("开源协议");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), filemenu);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(aboutMi), aboutmenu);

    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), backupMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), restoreMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quitMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(aboutmenu), meMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(aboutmenu), systemMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(aboutmenu), gtk_separator_menu_item_new());
    gtk_menu_shell_append(GTK_MENU_SHELL(aboutmenu), licenseMi);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), aboutMi);
    gtk_box_pack_start(GTK_BOX(menubar_vbox), menubar, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(backupMi), "activate", G_CALLBACK(on_backup_clicked), main_window);
    g_signal_connect(G_OBJECT(restoreMi), "activate", G_CALLBACK(on_restore_clicked), main_window);
    g_signal_connect(G_OBJECT(quitMi), "activate", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(systemMi), "activate", G_CALLBACK(on_aboutsystem_clicked), NULL);
    g_signal_connect(G_OBJECT(meMi), "activate", G_CALLBACK(on_aboutme_clicked), NULL);
    g_signal_connect(G_OBJECT(licenseMi), "activate", G_CALLBACK(on_license_clicked), NULL);
}

/*************************************************
Function: show_sidebar
Description: configure the side bar
Input: None
Output: None
Return: None
Others: None
*************************************************/
void show_sidebar(void)
{
    GtkWidget * sidebar_fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(fixed), sidebar_fixed);

    GtkWidget * background = gtk_image_new_from_file("img/sidebar.png");
    gtk_widget_set_usize(background, 212, 640);
    gtk_fixed_put(GTK_FIXED(sidebar_fixed), background, 0, 18);

    data = gtk_image_new_from_file("img/highlighted_data.png");
    GtkWidget * datalabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(datalabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>数据维护</span>");
    gtk_fixed_put(GTK_FIXED(sidebar_fixed), datalabel, 65, 224);
    GtkWidget * databox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(databox), FALSE);
    gtk_container_add(GTK_CONTAINER(databox), data);
    gtk_fixed_put(GTK_FIXED(sidebar_fixed), databox, 26, 68);

    query = gtk_image_new_from_file("img/query.png");
    GtkWidget * querylabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(querylabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>数据查询</span>");
    gtk_fixed_put(GTK_FIXED(sidebar_fixed), querylabel, 65, 404);
    GtkWidget * querybox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(querybox), FALSE);
    gtk_container_add(GTK_CONTAINER(querybox), query);
    gtk_fixed_put(GTK_FIXED(sidebar_fixed), querybox, 26, 248);

    stastical = gtk_image_new_from_file("img/stastical.png");
    GtkWidget * stasticallabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(stasticallabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>数据统计</span>");
    gtk_fixed_put(GTK_FIXED(sidebar_fixed), stasticallabel, 65, 584);
    GtkWidget * stasticalbox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(stasticalbox), FALSE);
    gtk_container_add(GTK_CONTAINER(stasticalbox), stastical);
    gtk_fixed_put(GTK_FIXED(sidebar_fixed), stasticalbox, 26, 428);

    g_signal_connect(databox, "button_press_event", G_CALLBACK(on_data_clicked), NULL);
    g_signal_connect(querybox, "button_press_event", G_CALLBACK(on_query_clicked), NULL);
    g_signal_connect(stasticalbox, "button_press_event", G_CALLBACK(on_stastical_clicked), NULL);

}

/*************************************************
Function: show_notebook
Description: configure the note book
Input: None
Output: None
Return: None
Others: None
*************************************************/
void show_notebook(void)
{
    notebook = gtk_notebook_new();
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
    gtk_container_add(GTK_CONTAINER(fixed), notebook);
}

/*************************************************
Function: show_dataview
Description: configure the data view
Input: None
Output: None
Return: None
Others: None
*************************************************/
void show_dataview(void)
{
    dataview = gtk_fixed_new();
    GtkWidget * typelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(typelabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 19.5'>选择数据类型</span>");
    gtk_fixed_put(GTK_FIXED(dataview), typelabel, 505, 170);

    province = gtk_image_new_from_file("img/highlighted_province.png");
    GtkWidget * provincelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(provincelabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 15'>所属省份</span>");
    gtk_fixed_put(GTK_FIXED(dataview), provincelabel, 328, 335);
    GtkWidget * provincebox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(provincebox), FALSE);
    gtk_container_add(GTK_CONTAINER(provincebox), province);
    gtk_fixed_put(GTK_FIXED(dataview), provincebox, 309, 221);

    cases = gtk_image_new_from_file("img/case.png");
    GtkWidget * caselabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(caselabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 15'>贪腐案件</span>");
    gtk_fixed_put(GTK_FIXED(dataview), caselabel, 544, 335);
    GtkWidget * casebox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(casebox), FALSE);
    gtk_container_add(GTK_CONTAINER(casebox), cases);
    gtk_fixed_put(GTK_FIXED(dataview), casebox, 525, 221);

    media = gtk_image_new_from_file("img/media.png");
    GtkWidget * medialabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(medialabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 15'>媒体报道</span>");
    gtk_fixed_put(GTK_FIXED(dataview), medialabel, 760, 335);
    GtkWidget * mediabox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(mediabox), FALSE);
    gtk_container_add(GTK_CONTAINER(mediabox), media);
    gtk_fixed_put(GTK_FIXED(dataview), mediabox, 741, 221);

    GtkWidget * confirm  = gtk_image_new_from_file("img/button1.png");
    GtkWidget * confirmbox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(confirmbox), FALSE);
    gtk_container_add(GTK_CONTAINER(confirmbox), confirm);
    gtk_fixed_put(GTK_FIXED(dataview), confirmbox, 479, 391);
    GtkWidget * confirmlabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(confirmlabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 19.5'>确认</span>");
    gtk_fixed_put(GTK_FIXED(dataview), confirmlabel, 559, 397);

    g_signal_connect(provincebox, "button_press_event", G_CALLBACK(on_province_clicked), NULL);
    g_signal_connect(casebox, "button_press_event", G_CALLBACK(on_case_clicked), NULL);
    g_signal_connect(mediabox, "button_press_event", G_CALLBACK(on_media_clicked), NULL);
    g_signal_connect(confirmbox, "button_press_event", G_CALLBACK(on_confirm_clicked), NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), dataview, NULL);
}

/*************************************************
Function: show_queryview
Description: configure the query view
Input: None
Output: None
Return: None
Others: None
*************************************************/
void show_queryview(void)
{
    queryview = gtk_fixed_new();

    GtkWidget * provincelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(provincelabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 19.5'>所属省份监管信息查询</span>");
    gtk_fixed_put(GTK_FIXED(queryview), provincelabel, 223, 125);

    GtkWidget * proname = gtk_image_new_from_file("img/button2.png");
    GtkWidget * pronamebox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(pronamebox), FALSE);
    gtk_container_add(GTK_CONTAINER(pronamebox), proname);
    gtk_fixed_put(GTK_FIXED(queryview), pronamebox, 251, 177);
    GtkWidget * pronamelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(pronamelabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按省份名称查找</span>");
    gtk_fixed_put(GTK_FIXED(queryview), pronamelabel, 340, 187);

    GtkWidget * chargename = gtk_image_new_from_file("img/button2.png");
    GtkWidget * chargenamebox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(chargenamebox), FALSE);
    gtk_container_add(GTK_CONTAINER(chargenamebox), chargename);
    gtk_fixed_put(GTK_FIXED(queryview), chargenamebox, 601, 177);
    GtkWidget * chargenamelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(chargenamelabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按纪检负责人姓名查找</span>");
    gtk_fixed_put(GTK_FIXED(queryview), chargenamelabel, 658, 187);

    GtkWidget * caselabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(caselabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 19.5'>贪腐案件基本信息查询</span>");
    gtk_fixed_put(GTK_FIXED(queryview), caselabel, 223, 290);

    GtkWidget * casenumber = gtk_image_new_from_file("img/button2.png");
    GtkWidget * casenumberbox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(casenumberbox), FALSE);
    gtk_container_add(GTK_CONTAINER(casenumberbox), casenumber);
    gtk_fixed_put(GTK_FIXED(queryview), casenumberbox, 251, 342);
    GtkWidget * casenumberlabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(casenumberlabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按案件编号查找</span>");
    gtk_fixed_put(GTK_FIXED(queryview), casenumberlabel, 340, 352);

    GtkWidget * namelevel = gtk_image_new_from_file("img/button2.png");
    GtkWidget * namelevelbox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(namelevelbox), FALSE);
    gtk_container_add(GTK_CONTAINER(namelevelbox), namelevel);
    gtk_fixed_put(GTK_FIXED(queryview), namelevelbox, 601, 342);
    GtkWidget * namelevellabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(namelevellabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按被告姓名和行政级别查找</span>");
    gtk_fixed_put(GTK_FIXED(queryview), namelevellabel, 638, 352);

    GtkWidget * medialabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(medialabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 19.5'>媒体报道基本信息查询</span>");
    gtk_fixed_put(GTK_FIXED(queryview), medialabel, 223, 455);

    GtkWidget * numbername = gtk_image_new_from_file("img/button2.png");
    GtkWidget * numbernamebox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(numbernamebox), FALSE);
    gtk_container_add(GTK_CONTAINER(numbernamebox), numbername);
    gtk_fixed_put(GTK_FIXED(queryview), numbernamebox, 251, 507);
    GtkWidget * numbernamelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(numbernamelabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按案件编号和媒体名称查找</span>");
    gtk_fixed_put(GTK_FIXED(queryview), numbernamelabel, 288, 517);

    GtkWidget * datename = gtk_image_new_from_file("img/button2.png");
    GtkWidget * datenamebox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(datenamebox), FALSE);
    gtk_container_add(GTK_CONTAINER(datenamebox), datename);
    gtk_fixed_put(GTK_FIXED(queryview), datenamebox, 601, 507);
    GtkWidget * datenamelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(datenamelabel), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按报道日期和媒体名称查找</span>");
    gtk_fixed_put(GTK_FIXED(queryview), datenamelabel, 638, 517);

    g_signal_connect(G_OBJECT(pronamebox), "button_press_event", G_CALLBACK(on_province_name_query_clicked), NULL);
    g_signal_connect(G_OBJECT(chargenamebox), "button_press_event", G_CALLBACK(on_province_head_query_clicked), NULL);
    g_signal_connect(G_OBJECT(casenumberbox), "button_press_event", G_CALLBACK(on_case_number_query_clicked), NULL);
    g_signal_connect(G_OBJECT(namelevelbox), "button_press_event", G_CALLBACK(on_case_name_and_level_query_clicked), NULL);
    g_signal_connect(G_OBJECT(numbernamebox), "button_press_event", G_CALLBACK(on_media_number_and_name_query_clicked), NULL);
    g_signal_connect(G_OBJECT(datenamebox), "button_press_event", G_CALLBACK(on_media_date_and_partname_query_clicked), NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), queryview, NULL);
}

/*************************************************
Function: show_stasticalview
Description: configure the stastical view
Input: None
Output: None
Return: None
Others: None
*************************************************/
void show_stasticalview(void)
{
    stasticalview = gtk_fixed_new();

    GtkWidget * chargelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(chargelabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 15'>以主要罪名为条件分别统计历年来各种罪名贪腐案件的发生数和涉案金额，\n并降序输出。</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), chargelabel, 245, 70);
    GtkWidget * chargebtn = gtk_image_new_from_file("img/button3.png");
    GtkWidget * chargebox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(chargebox), FALSE);
    gtk_container_add(GTK_CONTAINER(chargebox), chargebtn);
    gtk_fixed_put(GTK_FIXED(stasticalview), chargebox, 623, 127);
    GtkWidget * chargetitle = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(chargetitle), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按主要罪名统计</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), chargetitle, 702, 136);

    GtkWidget * levellabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(levellabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 15'>以行政级别为条件分别统计某年度内各种行政级别贪腐案件的发生数和涉案\n金额并统计不同刑期的案件分别是多少起。</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), levellabel, 245, 180);
    GtkWidget * levelbtn = gtk_image_new_from_file("img/button3.png");
    GtkWidget * levelbox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(levelbox), FALSE);
    gtk_container_add(GTK_CONTAINER(levelbox), levelbtn);
    gtk_fixed_put(GTK_FIXED(stasticalview), levelbox, 623, 237);
    GtkWidget * leveltitle = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(leveltitle), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按行政级别统计</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), leveltitle, 702, 246);

    GtkWidget * provincelabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(provincelabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 15'>以案件发生地所属省份为条件分别统计某个时间段内各省份贪腐案件的发生\n数，涉案金额总数，并降序输出。</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), provincelabel, 245, 290);
    GtkWidget * provincebtn = gtk_image_new_from_file("img/button3.png");
    GtkWidget * provincebox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(provincebox), FALSE);
    gtk_container_add(GTK_CONTAINER(provincebox), provincebtn);
    gtk_fixed_put(GTK_FIXED(stasticalview), provincebox, 623, 347);
    GtkWidget * provincetitle = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(provincetitle), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按所属省份统计</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), provincetitle, 702, 356);

    GtkWidget * reportlabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(reportlabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 15'>统计媒体报道数最多的10个贪腐案件，输出案件编号、被告行政级别、主要\n罪名、涉案金额、刑事处罚和媒体报道次数，并降序输出。</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), reportlabel, 245, 400);
    GtkWidget * reportbtn = gtk_image_new_from_file("img/button3.png");
    GtkWidget * reportbox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(reportbox), FALSE);
    gtk_container_add(GTK_CONTAINER(reportbox), reportbtn);
    gtk_fixed_put(GTK_FIXED(stasticalview), reportbox, 623, 457);
    GtkWidget * reporttitle = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(reporttitle), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按媒体报道统计</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), reporttitle, 702, 466);

    GtkWidget * amountlabel = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(amountlabel), "<span foreground='#60646d' font_desc='Microsoft YaHei 15'>统计涉案金额数最大的10个贪腐案件，输出案件编号、被告姓名、被告行政\n级别、主要罪名、涉案金额和刑事处罚，并降序输出。</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), amountlabel, 245, 510);
    GtkWidget * amountbtn = gtk_image_new_from_file("img/button3.png");
    GtkWidget * amountbox = gtk_event_box_new();
    gtk_event_box_set_visible_window(GTK_EVENT_BOX(amountbox), FALSE);
    gtk_container_add(GTK_CONTAINER(amountbox), amountbtn);
    gtk_fixed_put(GTK_FIXED(stasticalview), amountbox, 623, 567);
    GtkWidget * amounttitle = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(amounttitle), "<span foreground='#FFFFF7' font_desc='Microsoft YaHei 15'>按涉案金额统计</span>");
    gtk_fixed_put(GTK_FIXED(stasticalview), amounttitle, 702, 576);

    g_signal_connect(G_OBJECT(chargebox), "button_press_event", G_CALLBACK(on_charge_stastical_clicked), NULL);
    g_signal_connect(G_OBJECT(levelbox), "button_press_event", G_CALLBACK(on_punishment_stastical_clicked), NULL);
    g_signal_connect(G_OBJECT(provincebox), "button_press_event", G_CALLBACK(on_place_stastical_clicked), NULL);
    g_signal_connect(G_OBJECT(reportbox), "button_press_event", G_CALLBACK(on_report_stastical_clicked), NULL);
    g_signal_connect(G_OBJECT(amountbox), "button_press_event", G_CALLBACK(on_amount_stastical_clicked), NULL);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), stasticalview, NULL);
}

/*************************************************
Function: on_data_clicked
Description: called back when data button on the side bar clicked.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_data_clicked(GtkWidget * widget)
{
    if (state != 1)
    {
        gtk_image_set_from_file(GTK_IMAGE(data), "img/highlighted_data.png");
        gtk_image_set_from_file(GTK_IMAGE(query), "img/query.png");
        gtk_image_set_from_file(GTK_IMAGE(stastical), "img/stastical.png");
        if (state == 2)
        {
            gtk_notebook_prev_page(GTK_NOTEBOOK(notebook));
        }
        else if (state == 3)
        {
            gtk_notebook_prev_page(GTK_NOTEBOOK(notebook));
            gtk_notebook_prev_page(GTK_NOTEBOOK(notebook));
        }
        state = 1;
    }
}

/*************************************************
Function: on_query_clicked
Description: called back when query button on the side bar clicked.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_query_clicked(GtkWidget * widget)
{
    if (state != 2)
    {
        gtk_image_set_from_file(GTK_IMAGE(data), "img/data.png");
        gtk_image_set_from_file(GTK_IMAGE(query), "img/highlighted_query.png");
        gtk_image_set_from_file(GTK_IMAGE(stastical), "img/stastical.png");
        if (state == 1)
        {
            gtk_notebook_next_page(GTK_NOTEBOOK(notebook));
        }
        else if (state == 3)
        {
            gtk_notebook_prev_page(GTK_NOTEBOOK(notebook));
        }
        state = 2;
    }
}

/*************************************************
Function: on_stastical_clicked
Description: called back when stastical button on the side bar clicked.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_stastical_clicked(GtkWidget * widget)
{
    if (state != 3)
    {
        gtk_image_set_from_file(GTK_IMAGE(data), "img/data.png");
        gtk_image_set_from_file(GTK_IMAGE(query), "img/query.png");
        gtk_image_set_from_file(GTK_IMAGE(stastical), "img/highlighted_stastical.png");
        if (state == 1)
        {
            gtk_notebook_next_page(GTK_NOTEBOOK(notebook));
            gtk_notebook_next_page(GTK_NOTEBOOK(notebook));
        }
        else if (state == 2)
        {
            gtk_notebook_next_page(GTK_NOTEBOOK(notebook));
        }
        state = 3;
    }
}

/*************************************************
Function: on_province_clicked
Description: called back when province button on the data view clicked.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_province_clicked(GtkWidget * widget)
{
    if (data_type != 1)
    {
        gtk_image_set_from_file(GTK_IMAGE(province), "img/highlighted_province.png");
        gtk_image_set_from_file(GTK_IMAGE(cases), "img/case.png");
        gtk_image_set_from_file(GTK_IMAGE(media), "img/media.png");
        data_type = 1;
    }
}

/*************************************************
Function: on_case_clicked
Description: called back when case button on the data view clicked.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_case_clicked(GtkWidget * widget)
{
    if (data_type != 2)
    {
        gtk_image_set_from_file(GTK_IMAGE(province), "img/province.png");
        gtk_image_set_from_file(GTK_IMAGE(cases), "img/highlighted_case.png");
        gtk_image_set_from_file(GTK_IMAGE(media), "img/media.png");
        data_type = 2;
    }
}

/*************************************************
Function: on_media_clicked
Description: called back when media button on the data view clicked.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_media_clicked(GtkWidget * widget)
{
    if (data_type != 3)
    {
        gtk_image_set_from_file(GTK_IMAGE(province), "img/province.png");
        gtk_image_set_from_file(GTK_IMAGE(cases), "img/case.png");
        gtk_image_set_from_file(GTK_IMAGE(media), "img/highlighted_media.png");
        data_type = 3;
    }
}

/*************************************************
Function: on_confirm_clicked
Description: called back when confirm button on the data view clicked.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_confirm_clicked(GtkWidget * widget)
{
    switch (data_type)
    {
        case 1:
        province_method();
        break;
        case 2:
        case_method();
        break;
        case 3:
        media_method();
        break;
    }
}

/*************************************************
Function: on_backup_clicked
Description: called back when backup Mi on the menu bar activated.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer indicating the window that passes in
Output: None
Return: None
Others: None
*************************************************/
void on_backup_clicked(GtkWidget * widget, gpointer data)
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Save File",
				      GTK_WINDOW(data),
				      GTK_FILE_CHOOSER_ACTION_SAVE,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
				      NULL);

    char timestr[29];
    time_t result = time(NULL);
    strcpy(timestr, asctime(localtime(&result)));
    timestr[10] = '\0';
    strncat(timestr, ".bak", 4);

    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), timestr);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        backup_data(head, filename);
        information_message_dialog("备份完成", "备份已完成。");
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

/*************************************************
Function: on_restore_clicked
Description: called back when restore Mi on the menu bar activated.
Input: GtkWidget * indicating the widget that activates the signal
       gpointer indicating the window that passes in
Output: None
Return: None
Others: None
*************************************************/
void on_restore_clicked(GtkWidget * widget, gpointer data)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_dialog_new_with_buttons("警告", GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    g_object_unref(pixbuf), pixbuf = NULL;

    GtkWidget * label = gtk_label_new("您确认要恢复数据吗？\n如果这么做的话，您会丢失当前所有的数据。");
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), label);

    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    switch (result)
    {
        case GTK_RESPONSE_OK:
        {
            GtkWidget *choosedialog;
            choosedialog = gtk_file_chooser_dialog_new ("打开备份文件",
                            GTK_WINDOW(data),
                            GTK_FILE_CHOOSER_ACTION_OPEN,
                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                            NULL);

            if (gtk_dialog_run (GTK_DIALOG (choosedialog)) == GTK_RESPONSE_ACCEPT)
            {
                char * filename;
                filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (choosedialog));
                restore_data(&head, filename);
                g_free (filename);
            }

            gtk_widget_destroy (choosedialog);
        }
        break;
    }
    gtk_widget_destroy(dialog);
}

/*************************************************
Function: on_aboutsystem_clicked
Description: called back when about-system Mi on the menu bar activated.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_aboutsystem_clicked(GtkWidget * widget)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_about_dialog_new();
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "反贪腐系统");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "V1.0");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Zhihao Chen");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "本系统用来管理各省的贪腐案件以及媒体报道信息");
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);

    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    g_object_unref(pixbuf), pixbuf = NULL;

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/*************************************************
Function: on_aboutme_clicked
Description: called back when about-me Mi on the menu bar activated.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_aboutme_clicked(GtkWidget * widget)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK, "华中科技大学 CSEE1501\nZhihaoChen\nobsidiandragon2016@gmail.com");

    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_window_set_title(GTK_WINDOW(dialog), "关于作者");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    g_object_unref(pixbuf), pixbuf = NULL;

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/*************************************************
Function: on_license_clicked
Description: called back when license Mi on the menu bar activated.
Input: GtkWidget * indicating the widget that activates the signal
Output: None
Return: None
Others: None
*************************************************/
void on_license_clicked(GtkWidget * widget)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK, "<反贪腐系统> 第1版，版权所有（C）<2016> <Zhihao Chen>\n本程序从未提供品质担保。这是款自由软件，欢迎你在满足一定条件后对其再发布。");

    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_window_set_title(GTK_WINDOW(dialog), "开源协议");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    g_object_unref(pixbuf), pixbuf = NULL;

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/*************************************************
Function: on_cancel_clicked
Description: called back when cancel button clicked
Input: GtkWidget * indicating the widget that activates the signal
       gpointer indicating the widget that passes in
Output: None
Return: None
Others: None
*************************************************/
void on_cancel_clicked(GtkWidget * widget, gpointer data)
{
    gtk_widget_destroy(GTK_WIDGET(data));
}
