/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSQuery.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly provides basic support for data querying
according to the needs.
Function List: 
1.seek_province_name         seek for one province node with
given province name. 
2.seek_province_head         seek for provinces managed by a
linked list with given head name.
3.seek_case_number           seek for cases managed by a
linked list with given case number.
4.seek_case_name_level       seek for cases managed by a 
linked list with given defendant name and his level.
5.seek_media_number_name     seek for medias managed by a
linked list with given case number and media name.
6.seek_media_date_partname   seek for medias managed by a 
linked list with given report date and part of the media 
name.
************************************************************/

#include "ACSHeader.h"

/*************************************************
Function: seek_province_name
Description: seek for one province node with given province name. 
Input: ProvinceList indicating the head of the list
       char * indicating the name of the province
Output: None
Return: ProvinceNode indicating the province node as result
Others: None
*************************************************/
ProvinceNode seek_province_name(ProvinceList head, char * name)
{
    ProvinceNode node = head;
    while (node != NULL)
    {
        if (strcmp(node->content.province, name) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

/*************************************************
Function: seek_province_head
Description: seek for provinces managed by a linked list with given head name.
Input: ProvinceList indicating the head of the list
       char * indicating the head's name of the province
Output: None
Return: ProvinceList indicating the province list as result
Others: list return doesn't have a (NULL) head
*************************************************/
ProvinceList seek_province_head(ProvinceList head, char * headname)
{
    ProvinceNode node = head, p = NULL;
    ProvinceList result = NULL;
    while (node != NULL)
    {
        if (strcmp(node->content.head, headname) == 0)
        {
            if (result == NULL)
            {
                result = create_province_list(node->content);
                p = result;
            }
            else
            {
                p = insert_province(result, p, node->content);
            }
        }
        node = node->next;
    }
    return result;
}

/*************************************************
Function: seek_case_number
Description: seek for cases managed by a linked list with given case number.
Input: ProvinceList indicating the head of the list
       char * indicating the number of the case
Output: None
Return: CaseList indicating the case list as result
Others: None
*************************************************/
CaseList seek_case_number(ProvinceList head, char * number)
{
    ProvinceNode node = head;
    CaseList result = NULL;
    CaseNode p = NULL, temp;
    while (node != NULL)
    {
        temp = node->case_head;
        while (temp != NULL)
        {
            if (strcmp(temp->content.number, number) == 0)
            {
                if (result == NULL)
                {
                    result = create_case_list(temp->content);
                    p = result;
                }
                else
                {
                    p = insert_case(result, p, temp->content);
                }
                break;//因为一个省份内案件编号为主键 可以直接从下一个省开始
            }
            temp = temp->next;
        }
        node = node->next;
    }
    return result;
}

/*************************************************
Function: seek_case_name_level
Description: seek for cases managed by a linked list with given defendant name and his level.
Input: ProvinceList indicating the head of the list
       char * indicating the name of the case's defendant
       char indicating the level of the case's defendant
Output: None
Return: CaseList indicating the case list as result
Others: None
*************************************************/
CaseList seek_case_name_level(ProvinceList head, char * name, char level)
{
    ProvinceNode node = head;
    CaseList result = NULL;
    CaseNode p = NULL, temp;
    while (node != NULL)
    {
        temp = node->case_head;
        while (temp != NULL)
        {
            if ((strcmp(temp->content.defendant, name) == 0) && temp->content.level == level)
            {
                if (result == NULL)
                {
                    result = create_case_list(temp->content);
                    p = result;
                }
                else
                {
                    p = insert_case(result, p, temp->content);
                }
            }
            temp = temp->next;
        }
        node = node->next;
    }
    return result;
}

/*************************************************
Function: seek_media_number_name
Description: seek for medias managed by a linked list with given case number and media name.
Input: ProvinceList indicating the head of the list
       char * indicating the number of the case
       char * indicating the name of the media
Output: None
Return: MediaList indicating the media list as result
Others: None
*************************************************/
MediaList seek_media_number_name(ProvinceList head, char * number, char * name)
{
    ProvinceNode node = head;
    CaseNode case_temp;
    MediaNode media_temp, p = NULL;
    MediaList result = NULL;
    while (node != NULL)
    {
        case_temp = node->case_head;
        while (case_temp != NULL)
        {
            if (strcmp(case_temp->content.number, number) == 0)
            {
                media_temp = case_temp->media_head;
                while(media_temp != NULL)
                {
                    if (strcmp(media_temp->content.name, name) == 0)
                    {
                        if (result == NULL)
                        {
                            result = create_media_list(media_temp->content);
                            p = result;
                        }
                        else
                        {
                            p = insert_media(result, p, media_temp->content);
                        }
                    }
                    media_temp = media_temp->next;
                }
            }
            case_temp = case_temp->next;
        }
        node = node->next;
    }
    return result;
}

/*************************************************
Function: seek_media_date_partname
Description: seek for medias managed by a linked list with given report date and part of the media 
Input: ProvinceList indicating the head of the list
       char * indicating the date of the report
       char * indicating part of the name of the media
Output: None
Return: MediaList indicating the media list as result
Others: None
*************************************************/
MediaList seek_media_date_partname(ProvinceList head, char * date, char * partname)
{
    ProvinceNode node = head;
    CaseNode case_temp;
    MediaNode media_temp, p = NULL;
    MediaList result = NULL;
    while (node != NULL)
    {
        case_temp = node->case_head;
        while (case_temp != NULL)
        {
            media_temp = case_temp->media_head;
            while(media_temp != NULL)
            {
                if ((strcmp(media_temp->content.date, date) == 0) && (strstr(media_temp->content.name, partname)!= NULL))
                {
                    if (result == NULL)
                    {
                        result = create_media_list(media_temp->content);
                        p = result;
                    }
                    else
                    {
                        p = insert_media(result, p, media_temp->content);
                    }
                }
                media_temp = media_temp->next;
            }
            case_temp = case_temp->next;
        }
        node = node->next;
    }
    return result;
}
