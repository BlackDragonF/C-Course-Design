/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSData.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly manages the basic data about the system
including creating linked lists, inserting nodes, freeing
linked lists and saving/loading/backing up/restoring data.
Function List:
1.create_province_list      create a first-class linked list
managing province information.
2.create_case_list          create a second-class linked
list managing case information.
3.create_media_list         create a third-class linked list
managing media information.
4.insert_province           malloc a new province node and
insert it into certain position of certain linked list.
5.insert_case               malloc a new case node and
insert it into certain position of certain linked list.
6.insert media              malloc a new media node and
insert it into certain position of certain linked list.
7.delete_province           delete certain province node.
8.delete_case               delete certain case node.
9.delete_media              delete certain media node.
10.free_province_list       free certain province linked
list.
11.free_case_list           free certain case linked list.
12.free_media_list          free certain media linked list.
13.save_data                save all data to files.
14.load_data                load datas from files.
15.backup_data              back up data to certain file.
16.restore_data             restore data from certain file.
17.free_all                 free all node based on given
province linked list head.
***********************************************************/

#include "ACSHeader.h"

/*************************************************
Function: create_province_list
Description: create a new province list
Input: a province struct indicating the content
Output: None
Return: head of the province list
Others: None
*************************************************/
ProvinceList create_province_list(Province content)
{
    ProvinceList head = malloc(sizeof(ProvincePrototype));
    head->content = content;
    head->next = NULL;
    head->case_head= NULL;
    return head;
}

/*************************************************
Function: create_case_list
Description: create a new case list
Input: a case struct indicating the content
Output: None
Return: head of the case list
Others: None
*************************************************/
CaseList create_case_list(Case content)
{
    CaseList head = malloc(sizeof(CasePrototype));
    head->content = content;
    head->next = NULL;
    head->media_head = NULL;
    return head;
}

/*************************************************
Function: create_media_list
Description: create a new media list
Input: a media struct indicating the content
Output: None
Return: head of the media list
Others: None
*************************************************/
MediaList create_media_list(Media content)
{
    MediaList head = malloc(sizeof(MediaPrototype));
    head->content = content;
    head->next = NULL;
    return head;
}

/*************************************************
Function: insert_province
Description: insert a new province node to a province list
Input: the head of the list, the position node of the list, a province struct
Output: None
Return: the newly insert node
Others: None
*************************************************/
ProvinceNode insert_province(ProvinceList head, ProvinceNode node, Province content)
{
    ProvinceNode temp = malloc(sizeof(ProvincePrototype));
    temp->content = content;
    temp->next = node->next;
    temp->case_head = NULL;
    node->next = temp;
    return temp;
}

/*************************************************
Function: insert_case
Description: insert a new case node to a case list
Input: the head of the list, the position node of the list, a case struct
Output: None
Return: the newly insert node
Others: None
*************************************************/
CaseNode insert_case(CaseList head, CaseNode node, Case content)
{
    CaseNode temp = malloc(sizeof(CasePrototype));
    temp->content = content;
    temp->next = node->next;
    temp->media_head = NULL;
    node->next = temp;
    return temp;
}

/*************************************************
Function: insert_media
Description: insert a new media node to a media list
Input: the head of the list, the position node of the list, a media struct
Output: None
Return: the newly insert node
Others: None
*************************************************/
MediaNode insert_media(MediaList head, MediaNode node, Media content)
{
    MediaNode temp = malloc(sizeof(MediaPrototype));
    temp->content = content;
    temp->next = node->next;
    node->next = temp;
    return temp;
}

/*************************************************
Function: delete_province
Description: delete a province node
Input: the head of the list, the node to be deleted
Output: None
Return: None
Others: None
*************************************************/
void delete_province(ProvinceList head, ProvinceNode node)
{
    find_previous_province(head, node)->next = node->next;
    free(node);
}

/*************************************************
Function: delete_case
Description: delete a case node
Input: the head of the list, the node to be deleted
Output: None
Return: None
Others: None
*************************************************/
void delete_case(CaseList * phead, CaseNode node)
{
    if (*phead == node)
    {
        *phead = (*phead)->next;
        free(node);
    }
    else
    {
        find_previous_case(*phead, node)->next = node->next;
        free(node);
    }
}

/*************************************************
Function: media_province
Description: delete a media node
Input: the head of the list, the node to be deleted
Output: None
Return: None
Others: None
*************************************************/
void delete_media(MediaList * phead, MediaNode node)
{
    if (*phead == node)
    {
        *phead = (*phead)->next;
        free(node);
    }
    else
    {
        find_previous_media(*phead, node)->next = node->next;
        free(node);
    }
}

/*************************************************
Function: free_province_list
Description: free the entire province list
Input: the head of the list
Output: None
Return: None
Others: None
*************************************************/
void free_province_list(ProvinceList head)
{
    ProvinceNode temp, node = head;
    while(node != NULL)
    {
        temp = node->next;
        free(node);
        node = temp;
    }
}

/*************************************************
Function: free_case_list
Description: free the entire case list
Input: the head of the list
Output: None
Return: None
Others: None
*************************************************/
void free_case_list(CaseList head)
{
    CaseNode temp, node = head;
    while (node != NULL)
    {
        temp = node->next;
        free(node);
        node = temp;
    }
}

/*************************************************
Function: free_media_list
Description: free the entire media list
Input: the head of the list
Output: None
Return: None
Others: None
*************************************************/
void free_media_list(MediaList head)
{
    MediaNode temp, node = head;
    while (node != NULL)
    {
        temp = node->next;
        free(node);
        node = temp;
    }
}

/*************************************************
Function: save_data
Description: save the data into files
Input: the head of the province(first class) list
Output: None
Return: None
Others: The data files locates in dat folder.
*************************************************/
void save_data(ProvinceList head)
{
    FILE * province_fp, * case_fp, * media_fp;
    province_fp = fopen("dat/province.bin", "wb+");
    case_fp = fopen("dat/case.bin", "wb+");
    media_fp = fopen("dat/media.bin", "wb+");
    ProvinceNode province_node = head->next;
    CaseNode case_node;
    MediaNode media_node;
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        fwrite(&(province_node->content), sizeof(Province), 1, province_fp);
        while (case_node != NULL)
        {
            media_node = case_node->media_head;
            fwrite(&(case_node->content), sizeof(Case), 1, case_fp);
            while (media_node != NULL)
            {
                fwrite(&(media_node->content), sizeof(Media), 1, media_fp);
                media_node = media_node->next;
            }
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
    fclose(province_fp);
    fclose(case_fp);
    fclose(media_fp);
}

/*************************************************
Function: load_data
Description: load the data from files
Input: the pointer to the head of the province(first class) list
Output: None
Return: a int value indicating the result of the load process
        28 if all the data files are loaded
        not 28 if something goes wrong
Others: 
The data files locates in dat folder.
*************************************************/
int load_data(ProvinceList * phead)
{
    *phead = malloc(sizeof(ProvincePrototype));
    (*phead)->next = NULL;
    (*phead)->case_head = NULL;
    int result = 0;
    FILE * fp;

    fp = fopen("dat/province.bin", "rb");
    if (fp == NULL)
        return result;
    Province temp_province;
    while (fread(&temp_province, sizeof(Province), 1, fp) != 0)
    {
            insert_province(*phead, *phead, temp_province);
    }
    fclose(fp);
    result += 4;
    fp = fopen("dat/case.bin", "rb");
    if (fp == NULL)
        return result;
    Case temp_case;
    ProvinceNode province_node = *phead;
    while (fread(&temp_case, sizeof(Case), 1, fp) != 0)
    {
        province_node = *phead;
        while (province_node != NULL)
        {
            if (strcmp(province_node->content.province, temp_case.province) == 0)
                break;
            province_node = province_node->next;
        }
        if (province_node != NULL)
        {
            if (province_node->case_head == NULL)
                province_node->case_head = create_case_list(temp_case);
            else
                insert_case(province_node->case_head, province_node->case_head , temp_case);
        }
    }
    fclose(fp);
    result += 8;
    fp = fopen("dat/media.bin", "rb");
    if (fp == NULL)
        return result;
    Media temp_media;
    CaseNode case_node = NULL;
    int is_same_case;
    while (fread(&temp_media, sizeof(Media), 1, fp) != 0)
    {
        province_node = *phead;
        while (province_node != NULL)
        {
            case_node = province_node->case_head;
            is_same_case = 0;
            while(case_node != NULL)
            {
                if (strcmp(temp_media.number, case_node->content.number) == 0)
                {
                    is_same_case = 1;
                    break;
                }
                case_node = case_node->next;
            }
            if ((is_same_case) && (strcmp(temp_media.province, province_node->content.province) == 0))
                break;
            province_node = province_node->next;
        }
        if (province_node != NULL)
        {
            if (case_node->media_head == NULL)
                case_node->media_head = create_media_list(temp_media);
            else
                insert_media(case_node->media_head, case_node->media_head, temp_media);
        }
    }
    fclose(fp);
    result += 16;
    return result;
}

/*************************************************
Function: backup_data
Description: back up data into certain file
Input: the head of the province(first class) list
       the char * indicating the file location and name 
Output: None
Return: None
Others: The data file is picked up by file chooser
        using absolute location
*************************************************/
void backup_data(ProvinceList head, char * filename)
{
    int provincenumber = province_number(head);
    provincenumber--;
    int casenumber = 0, medianumber = 0;
    ProvinceNode province_node = head->next;
    CaseNode case_node;
    MediaNode media_node;
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        casenumber += case_number(case_node);
        while(case_node != NULL)
        {
            media_node = case_node->media_head;
            medianumber += media_number(media_node);
            while (media_node !=NULL)
            {
                media_node = media_node->next;
            }
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
    FILE * fp = fopen(filename, "wb");
    fwrite(&provincenumber, sizeof(int), 1, fp);
    fwrite(&casenumber, sizeof(int), 1, fp);
    fwrite(&medianumber, sizeof(int), 1, fp);
    province_node = head->next;
    while (province_node != NULL)
    {
        fwrite(&(province_node->content), sizeof(Province), 1, fp);
        province_node = province_node->next;
    }
    province_node = head->next;
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            fwrite(&(case_node->content), sizeof(Case), 1, fp);
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
    province_node = head->next;
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            media_node = case_node->media_head;
            while(media_node != NULL)
            {
                fwrite(&(media_node->content), sizeof(Media), 1, fp);
                media_node = media_node->next;
            }
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
    fclose(fp);
}

/*************************************************
Function: restore_data
Description: restore data from certain file
Input: the pointer to the head of the province(first class) list
       the char * indicating the file location and name 
Output: None
Return: None
Others: The data file is picked up by file chooser
        using absolute location
*************************************************/
int restore_data(ProvinceList * phead, char * filename)
{
    free_all(*phead);
    *phead = malloc(sizeof(ProvincePrototype));
    (*phead)->next = NULL;
    (*phead)->case_head = NULL;
    FILE * fp = fopen(filename, "rb");
    int provincenumber, casenumber, medianumber;
    if (fp == NULL)
        return 0;
    fread(&provincenumber, sizeof(int), 1, fp);
    fread(&casenumber, sizeof(int), 1, fp);
    fread(&medianumber, sizeof(int), 1, fp);
    int currentnumber = 0;
    Province temp_province;
    while ((currentnumber < provincenumber) && (fread(&temp_province, sizeof(Province), 1, fp) != 0))
    {
        insert_province(*phead, *phead, temp_province);
        currentnumber++;
    }
    currentnumber = 0;
    Case temp_case;
    ProvinceNode province_node = *phead;
    while ((currentnumber < casenumber) && (fread(&temp_case, sizeof(Case), 1, fp) != 0))
    {
        province_node = *phead;
        while (province_node != NULL)
        {
            if (strcmp(province_node->content.province, temp_case.province) == 0)
                break;
            province_node = province_node->next;
        }
        if (province_node != NULL)
        {
            if (province_node->case_head == NULL)
                province_node->case_head = create_case_list(temp_case);
            else
                insert_case(province_node->case_head, province_node->case_head , temp_case);
            currentnumber++;
        }
    }
    currentnumber = 0;
    Media temp_media;
    CaseNode case_node = NULL;
    int is_same_case;
    while ((currentnumber < medianumber) && (fread(&temp_media, sizeof(Media), 1, fp) != 0))
    {
        province_node = *phead;
        while (province_node != NULL)
        {
            case_node = province_node->case_head;
            is_same_case = 0;
            while(case_node != NULL)
            {
                if (strcmp(temp_media.number, case_node->content.number) == 0)
                {
                    is_same_case = 1;
                    break;
                }
                case_node = case_node->next;
            }
            if ((is_same_case) && (strcmp(temp_media.province, province_node->content.province) == 0))
                break;
            province_node = province_node->next;
        }
        if (province_node != NULL)
        {
            if (case_node->media_head == NULL)
                case_node->media_head = create_media_list(temp_media);
            else
                insert_media(case_node->media_head, case_node->media_head, temp_media);
            currentnumber++;
        }
    }
    fclose(fp);
    return 1;
}

void free_all(ProvinceList head)
{
    ProvinceNode province_node = head;
    CaseNode case_node;
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            free_media_list(case_node->media_head);
            case_node = case_node->next;
        }
        free_case_list(province_node->case_head);
        province_node = province_node->next;
    }
    free_province_list(head);
}
