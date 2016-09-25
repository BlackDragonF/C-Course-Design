/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSDataAssist.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly provide support for data management
including data processing and data recognition.
Function List:
1.is_same_yead              judge if the given date has the
same year as given year.
2.is_date_between           judge if the given date is
between the given range.
3.is_unique_province        judge if the given province is
unique in given province linked list.
4.province_with_name        find certain province node with
given province name.
5.is_unique_case            judge if the given case is unique
in given case linked list.
6.case_with_number          find certain case node with given
case name
7.province_number           return the number of nodes with
given province linked list.
8.case_number               return the number of nodes with
given case linked list.
9.media_number              return the number of nodes with
given media linked list.
10.find_previous_province   return the previous node of the
given province node.
11.find_previous_case       return the previous node of the
given case node.
12.find_previous_media      return the previous node of the
given media node.
************************************************************/

#include "ACSHeader.h"

/*************************************************
Function: is_same_year
Description: judge if the date has the same year as the given year.
Input: char * indicating the date
       char * indicating the year.
Output: None
Return: 1 if the date has the same year as given year
        0 if not
Others: None
*************************************************/
int is_same_year(char * date, char * year)
{
    int count;
    for (count = 0 ; count < 4 ; count++)
    {
        if (*year != *date)
            return 0;
        year++;
        date++;
    }
    return 1;
}

/*************************************************
Function: is_date_between
Description: judge if given date is between the given range
Input: char * indicating the given date
       char * indicating the start date
       char * indicating the end date
Output: None
Return: 1 if the date is between start and end
        0 if not
Others: None
*************************************************/
int is_date_between(char * date, char * start, char * end)
{
    if ((atoi(date) >= atoi(start)) && (atoi(date) <= atoi(end)))
        return 1;
    else
        return 0;
}

/*************************************************
Function: is_unique_province
Description: judge if given province name is unique in given province list
Input: ProvinceList indicating the head of the province list.
       const char * indicating the province name
Output: None
Return: 1 if the province name is unique
        0 if not
Others: None
*************************************************/
int is_unique_province(ProvinceList head, const char * province)
{
    ProvinceNode node = head->next;
    while (node != NULL)
    {
        if (strcmp(node->content.province, province) == 0)
            return 0;
        node = node->next;
    }
    return 1;
}

/*************************************************
Function: province_with_name
Description: find the province node with given province name
Input: ProvinceList indicating the head of the province list.
       char * indicating the province name
Output: None
Return: ProvinceNode indicating the province node as result
Others: NULL if don't find the proper province node
*************************************************/
ProvinceNode province_with_name(ProvinceList head, char * name)
{
    ProvinceNode node = head->next;
    while (node != NULL)
    {
        if (strcmp(node->content.province, name) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

/*************************************************
Function: is_unique_case
Description: judge if given case number is unique in given case list
Input: CaseList indicating the head of the case list.
       const char * indicating the case number
Output: None
Return: 1 if the case number is unique
        0 if not
Others: None
*************************************************/
int is_unique_case(CaseList head, const char * number)
{
    CaseNode node = head;
    while (node != NULL)
    {
        if (strcmp(node->content.number, number) == 0)
            return 0;
        node = node->next;
    }
    return 1;
}

/*************************************************
Function: case_with_number
Description: find the case node with given case number
Input: CaseList indicating the head of the case list.
       char * indicating the case number
Output: None
Return: CaseNode indicating the case node as result
Others: NULL if don't find the proper case node
*************************************************/
CaseNode case_with_number(CaseList head, char * number)
{
    CaseNode node = head;
    while (node != NULL)
    {
        if (strcmp(node->content.number, number) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

/*************************************************
Function: province_number
Description: count the number of province nodes in a province list
Input: ProvinceList indicating the head of the province list.
Output: None
Return: int indicating the number of province nodes in the province list
Others: the head(NULL head) of the province list is also counted
*************************************************/
int province_number(ProvinceList head)
{
    ProvinceNode node = head;
    int count = 0;
    while (node != NULL)
    {
        count++;
        node = node->next;
    }
    return count;
}

/*************************************************
Function: case_number
Description: count the number of case nodes in a case list
Input: CaseList indicating the head of the case list.
Output: None
Return: int indicating the number of case nodes in the case list
Others: None
*************************************************/
int case_number(CaseList head)
{
    CaseNode node = head;
    int count = 0;
    while (node != NULL)
    {
        count++;
        node = node->next;
    }
    return count;
}

/*************************************************
Function: media_number
Description: count the number of media nodes in a media list
Input: MediaList indicating the head of the media list.
Output: None
Return: int indicating the number of media nodes in the media list
Others: None
*************************************************/
int media_number(MediaList head)
{
    MediaNode node = head;
    int count = 0;
    while (node != NULL)
    {
        count++;
        node = node->next;
    }
    return count;
}

/*************************************************
Function: find_previous_province
Description: find the previous province node of certain province node
Input: ProvinceList indicating the head of the province list.
       ProvinceNode indicating the target province node
Output: None
Return: ProvinceNode indicating the province node as result
Others: NULL if the target province node is head.
*************************************************/
ProvinceNode find_previous_province(ProvinceList head, ProvinceNode node)
{
    ProvinceNode temp = head;
    while (temp != NULL)
    {
        if (temp->next == node)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/*************************************************
Function: find_previous_case
Description: find the previous case node of certain case node
Input: CaseList indicating the head of the case list.
       CaseNode indicating the target case node
Output: None
Return: Case Node indicating the case node as result
Others: NULL if the target case node is head.
*************************************************/
CaseNode find_previous_case(CaseList head, CaseNode node)
{
    CaseNode temp = head;
    while (temp != NULL)
    {
        if (temp->next == node)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

/*************************************************
Function: find_previous_media
Description: find the previous media node of certain media node
Input: MediaList indicating the head of the media list.
       MediaNode indicating the target media node
Output: None
Return: MediaNode indicating the media node as result
Others: NULL if the target media node is head.
*************************************************/
MediaNode find_previous_media(MediaList head, MediaNode node)
{
    MediaNode temp = head;
    while (temp != NULL)
    {
        if (temp->next == node)
            return temp;
        temp = temp->next;
    }
    return NULL;
}
