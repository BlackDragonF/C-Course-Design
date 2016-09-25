/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: main.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly declares the global variables and provides
the entry for the program.
Function List:
1.main 						The entry of the program, loading
data, initializing gtk and saving data when closed.
*************************************************************/

#include "ACSHeader.h"

ProvinceList head;
GtkWidget * main_window;

/*************************************************
Function: main
Description: The entry of the program.
Input: int argc, char * argv[]
Output: None
Return: a int value indicating the program status.
Others: None
*************************************************/
int main (int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    show_window(&main_window);
    if (load_data(&head) != 28)
    {
    	warning_message_dialog("未能读取数据", "未能读取数据，这可能是因为数据文件损坏或者缺少数据文件。\n请使用数据恢复功能恢复数据或重新录入数据。");
    }
    gtk_main();
    save_data(head);
    return 0;
}

