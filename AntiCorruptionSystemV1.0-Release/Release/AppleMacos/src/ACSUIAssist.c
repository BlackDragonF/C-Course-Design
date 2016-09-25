/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSUIAssist.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly provides support for showing UI including
creating pixbuf and showing warning/error messages. 
Function List:
1.create_pixbuf             return a pixbuf with the given file
name.
2.warning_message_dialog    run a dialog showing warning
messages.
3.error_message_dialog      run a dialog showing error
messages.
4.information_message_dialog run a dialog showing information
messages.
*************************************************************/

#include "ACSHeader.h"

extern GtkWidget * main_window;

/*************************************************
Function: create_pixbuf
Description: return a pixbuf with the given filename.
Input: const gchar * indicating the name of file
Output: None
Return: GdkPixbuf * indicating the pixbuf that creates.
Others: None
*************************************************/
GdkPixbuf * create_pixbuf(const gchar * filename)
{
    GdkPixbuf * pixbuf;
    GError * error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if (!pixbuf)
    {
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }
    return pixbuf;
}

/*************************************************
Function: warning_message_dialog
Description: run a dialog showing warning messages.
Input: gchar * indicating the title of the dialog
       gchat * indicating the content of the dialog
Output: None
Return: None
Others: None
*************************************************/
void warning_message_dialog(gchar * title, gchar * message)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, message);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    g_object_unref(pixbuf), pixbuf = NULL;
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/*************************************************
Function: error_message_dialog
Description: run a dialog showing error messages.
Input: gchar * indicating the title of the dialog
       gchat * indicating the content of the dialog
Output: None
Return: None
Others: None
*************************************************/
void error_message_dialog(gchar * title, gchar * message)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, message);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    g_object_unref(pixbuf), pixbuf = NULL;
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/*************************************************
Function: warning_information_dialog
Description: run a dialog showing information messages.
Input: gchar * indicating the title of the dialog
       gchat * indicating the content of the dialog
Output: None
Return: None
Others: None
*************************************************/
void information_message_dialog(gchar * title, gchar * message)
{
    GdkPixbuf * pixbuf = create_pixbuf("img/icon.png");
    GtkWidget * dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);
    gtk_window_set_icon(GTK_WINDOW(dialog), pixbuf);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    g_object_unref(pixbuf), pixbuf = NULL;
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
