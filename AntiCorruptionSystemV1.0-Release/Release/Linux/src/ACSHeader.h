/**********************************************************************
Copyright (C), 2016, Zhihao Chen.
File name: ACSHeader.h
Author: ZhihaoChen
Version: 1.0
Date: 2016.7.30
Description: Header file, declares all the function prototypes, data 
structures and defines all macros. 
Others: This Software Adopts GPL Lisence
History: 2016.7.30 Mainly Completes.
**********************************************************************/

/**********************************************************************
AntiCorruptionSystem 1.0 A free software to manage corrupttion infos
Copyright (C) 2016 Zhihao Chen obsidiandragon@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#ifndef ACSHeader_H
#define ACSHeader_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>

//Macro Defines
#define LEVELNUM 8 //行政级别数
#define CHARGENUM 6 //主要罪名数
#define MAXREPORTNUM 10 //报道统计的最多输出数
#define MAXAMOUNTNUM 10 //金额统计的最多输出数

//Data Structures
typedef struct Media
{
    char number[12];
    char date[10];
    char type;
    char name[40];
    char index[50];
    char province[16];
} Media;

typedef struct Case
{
    char number[12];
    char date[10];
    char province[16];
    char defendant[24];
    char level;
    char charge;
    float amount;
    int punishment;
} Case;

typedef struct Province
{
    char province[16];
    char head[24];
    char tel[15];
} Province;

typedef struct MediaPrototype
{
    Media content;
    struct MediaPrototype * next;
} MediaPrototype;

typedef struct CasePrototype
{
    Case content;
    struct CasePrototype * next;
    struct MediaPrototype * media_head;
} CasePrototype;

typedef struct ProvincePrototype
{
    Province content;
    struct ProvincePrototype * next;
    struct CasePrototype * case_head;
} ProvincePrototype;

//统计结果数据类型
typedef struct Charge{
    char name;
    int times;
    double amount;
}Charge;

typedef struct Punishment{
    char level;
    int times;
    int death; //死刑
    int life; //无期
    int high; //>11
    int medium; //6-10
    int low; //1-5
}Punishment;

typedef struct Place{
    char province[16];
    int times;
    double amount;
}Place;

typedef struct Report{
    char number[12];
    char level;
    char charge;
    float amount;
    int punishment;
    int times;
}Report;

typedef struct Amount {
    char number[12];
    char name[24];
    char level;
    char charge;
    int punishment;
    float amount;
}Amount;

typedef MediaPrototype * MediaNode;
typedef CasePrototype  * CaseNode;
typedef ProvincePrototype * ProvinceNode;

typedef MediaPrototype * MediaList;
typedef CasePrototype  * CaseList;
typedef ProvincePrototype * ProvinceList;

//Function Prototypes

//数据处理类函数
ProvinceList create_province_list(Province);
CaseList create_case_list(Case);
MediaList create_media_list(Media);

ProvinceNode insert_province(ProvinceList, ProvinceNode, Province);
CaseNode insert_case(CaseList, CaseNode, Case);
MediaNode insert_media(MediaList, MediaNode, Media);

void delete_province(ProvinceList, ProvinceNode);
void delete_case(CaseList *, CaseNode);
void delete_media(MediaList *, MediaNode);

void free_province_list(ProvinceList);
void free_case_list(CaseList);
void free_media_list(MediaList);
void free_all(ProvinceList);

void save_data(ProvinceList);
int load_data(ProvinceList *);
void backup_data(ProvinceList, char *);
int restore_data(ProvinceList *, char *);


//数据查询类函数
ProvinceNode seek_province_name(ProvinceList, char *);
ProvinceList seek_province_head(ProvinceList, char *);

CaseList seek_case_number(ProvinceList, char *);
CaseList seek_case_name_level(ProvinceList, char *, char);

MediaList seek_media_number_name(ProvinceList, char *, char *);
MediaList seek_media_date_partname(ProvinceList, char *, char *);

//数据统计类函数
Charge * charge_stastical(ProvinceList);
Punishment * punishment_stastical(ProvinceList, char *);
Place * place_stastical(ProvinceList, char *, char *);
Report * report_stastical(ProvinceList);
/*
 *统计涉案金额数最大的10个贪腐案件，按涉案金额降序排序后输出案件编号、
 *被告姓名、被告行政级别、主要罪名、涉案金额、刑事处罚
 */
Amount * amount_stastical(ProvinceList);

void sort_charge_times(Charge *);
void sort_place_times(Place *, int);

//界面类函数
void show_window(GtkWidget **);
void show_notebook(void);
void show_menubar(GtkWidget *);
void show_sidebar(void);
void show_dataview(void);
void show_queryview(void);
void show_stasticalview(void);

//事件类函数
void on_data_clicked(GtkWidget *);
void on_query_clicked(GtkWidget *);
void on_stastical_clicked(GtkWidget *);
void on_province_clicked(GtkWidget *);
void on_case_clicked(GtkWidget *);
void on_media_clicked(GtkWidget *);
void on_confirm_clicked(GtkWidget *);

void on_backup_clicked(GtkWidget *, gpointer);
void on_restore_clicked(GtkWidget *, gpointer);
void on_aboutsystem_clicked(GtkWidget *);
void on_aboutme_clicked(GtkWidget *);
void on_license_clicked(GtkWidget *);

//数据维护事件函数
void province_method(void);
void on_province_add_clicked(GtkWidget *, gpointer);
void on_province_modify_clicked(GtkWidget *, gpointer);
void on_province_delete_clicked(GtkWidget *);
void on_cancel_clicked(GtkWidget *, gpointer);
void reload_province_list(void);
void case_method(void);
void on_case_add_clicked(GtkWidget *, gpointer);
void on_case_modify_clicked(GtkWidget *, gpointer);
void on_case_delete_clicked(GtkWidget *);
void reload_case_list(void);
void media_method(void);
void on_media_add_clicked(GtkWidget *, gpointer);
void on_media_modify_clicked(GtkWidget *, gpointer);
void on_media_delete_clicked(GtkWidget *);
void reload_media_list(void);
void on_province_combo_changed(GtkWidget *, gpointer);

//数据查询事件函数
void on_province_name_query_clicked(GtkWidget *);
void on_province_head_query_clicked(GtkWidget *);
void on_case_number_query_clicked(GtkWidget *);
void on_case_name_and_level_query_clicked(GtkWidget *);
void on_media_number_and_name_query_clicked(GtkWidget *);
void on_media_date_and_partname_query_clicked(GtkWidget *);

//数据统计事件函数
void on_charge_stastical_clicked(GtkWidget *);
void on_punishment_stastical_clicked(GtkWidget *);
void on_place_stastical_clicked(GtkWidget *);
void on_report_stastical_clicked(GtkWidget *);
void on_amount_stastical_clicked(GtkWidget *);

//数据辅助类函数
int is_same_year(char *, char *);
int is_date_between(char *, char *, char *);

int is_unique_province(ProvinceList, const char *);
ProvinceNode province_with_name(ProvinceList, char *);

int is_unique_case(CaseList, const char *);
CaseNode case_with_number(CaseList, char *);

int province_number(ProvinceList);
int case_number(CaseList);
int media_number(MediaList);

ProvinceNode find_previous_province(ProvinceList, ProvinceNode);
CaseNode find_previous_case(CaseList, CaseNode);
MediaNode find_previous_media(MediaList, MediaNode);

//界面辅助类函数
GdkPixbuf * create_pixbuf(const gchar *);
void warning_message_dialog(gchar *, gchar *);
void error_message_dialog(gchar *, gchar *);
void information_message_dialog(gchar *, gchar *);

#endif /* ACSHeader_H */
