/************************************************************
Copyright (C), 2016, Zhihao Chen.
FileName: ACSStastical.c
Author: Zhihao Chen
Version : 1.0
Date: 2016.7.30
Description:
This module mainly provides basic support for data stastics
according to the needs.
Function List: 
1.charge_stastical                  Give the stastical result
with the condition of main charge.
2.punishment_stastical              Give the stastical result
with the condition of level of defendant.
3.place_stastical                   Give the stastical result
with the condition of where the case takes place.
4.report_stastical                  Give the stastical result
with the condition of the time media reports.
5.amount_stastical                  Give the stastical result
with the condition of the amount that case involves.
6.sort_charge_times                 Sort the charge stastical
result according to the number of the cases.
7.sort_place_times                  Sort the place stastical
result according to the number of the cases.
************************************************************/

#include "ACSHeader.h"

/*************************************************
Function: charge_stastical
Description: Give the stastical result with the condition of main charge.
Input: ProvinceList indicating the head of the list
Output: None
Return: Charge * indicating the pointer to the result array. 
Others: None
*************************************************/
Charge * charge_stastical(ProvinceList head)
{
    ProvinceNode province_node = head->next;
    CaseNode case_node;
    Charge * result = malloc(sizeof(Charge) * CHARGENUM);
    int count, index;
    for (count = 0 ; count < CHARGENUM ; count++)
    {
        result[count].name = count + '1';
        result[count].times = 0;
        result[count].amount = 0;
    }
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            index = case_node->content.charge-'1';
            result[index].times++;
            result[index].amount += case_node->content.amount;
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
    sort_charge_times(result);
    return result;;
}


/*************************************************
Function: punishment_stastical
Description: Give the stastical result with the condition of level of defendant.
Input: ProvinceList indicating the head of the list
       char * indicating the year.
Output: None
Return: Punishment * indicating the pointer to the result array. 
Others: None
*************************************************/
Punishment * punishment_stastical(ProvinceList head, char * year)
{
    ProvinceNode province_node = head->next;
    CaseNode case_node;
    Punishment * result = malloc(sizeof(Punishment) * LEVELNUM);
    int count, index;
    for (count = 0 ; count < LEVELNUM ; count++)
    {
        result[count].level = 'A' + count;
        result[count].times = 0;
        result[count].death = 0;
        result[count].life = 0;
        result[count].low = 0;
        result[count].medium = 0;
        result[count].high = 0;
    }
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            if (is_same_year(case_node->content.date, year))
            {
                index = case_node->content.level-'A';
                result[index].times++;
                if (case_node->content.punishment < 0)
                    result[index].death++;
                else if (case_node->content.punishment >= 100)
                    result[index].life++;
                else if (case_node->content.punishment >= 11)
                    result[index].high++;
                else if (case_node->content.punishment >= 6)
                    result[index].medium++;
                else if (case_node->content.punishment >= 1)
                    result[index].low++;
            }
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
    return result;
}

/*************************************************
Function: place_stastical
Description: Give the stastical result with the condition of where the case takes place.
Input: ProvinceList indicating the head of the list
       char * indicating the start date
       char * indicating the end date 
Output: None
Return: Place * indicating the pointer to the result array. 
Others: None
*************************************************/
Place * place_stastical(ProvinceList head, char * start, char * end)
{
    ProvinceNode province_node = head->next;
    CaseNode case_node;
    Place * result = malloc(sizeof(Place) * (province_number(head)-1));
    int index = 0, count;
    double amount;
    while (province_node != NULL)
    {
        count = 0;
        amount = 0;
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            if (is_date_between(case_node->content.date, start, end))
            {
                count++;
                amount += case_node->content.amount;
            }
            case_node = case_node->next;
        }
        strcpy(result[index].province, province_node->content.province);
        result[index].times = count;
        result[index].amount = amount;
        province_node = province_node->next;
        index++;
    }
    sort_place_times(result, province_number(head)-1);
    return result;
}

/*************************************************
Function: report_stastical
Description: Give the stastical result with the condition of the time media reports.
Input: ProvinceList indicating the head of the list
Output: None
Return: Report * indicating the pointer to the result array. 
Others: None
*************************************************/
Report * report_stastical(ProvinceList head)
{
    ProvinceNode province_node = head->next;
    CaseNode case_node;
    Report * result = malloc(sizeof(Report) * MAXREPORTNUM);
    int count, temp, times;
    for (count = 0 ; count < MAXREPORTNUM ; count++)
    {
        result[count].times = -1;
        result[count].level = 0;
        result[count].charge = 0;
        result[count].amount = 0;
        result[count].punishment = 0;
    }
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            times = media_number(case_node->media_head);
            for (count = 0 ; count < MAXREPORTNUM ; count++)
            {
                if (times > result[count].times)
                {
                    for (temp = MAXREPORTNUM - 2 ; temp >= count; temp--)
                        result[temp + 1] = result[temp];
                    strcpy(result[count].number, case_node->content.number);
                    result[count].level = case_node->content.level;
                    result[count].charge = case_node->content.charge;
                    result[count].amount = case_node->content.amount;
                    result[count].punishment = case_node->content.punishment;
                    result[count].times = times;
                    break;
                }
            }
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
    return result;
}

/*************************************************
Function: amount_stastical
Description: Give the stastical result with the condition of the amount that case involves.
Input: ProvinceList indicating the head of the list
Output: None
Return: Amount * indicating the pointer to the result array. 
Others: None
*************************************************/
Amount * amount_stastical(ProvinceList head)
{
    ProvinceNode province_node = head->next;
    CaseNode case_node;
    Amount * result = malloc(sizeof(Amount) * MAXAMOUNTNUM);
    int count, temp;
    float amount;
    for (count = 0 ; count < MAXAMOUNTNUM ; count++)
    {
        result[count].amount = -1;
        result[count].level = 0;
        result[count].charge = 0;
        result[count].punishment = 0;
    }
    while (province_node != NULL)
    {
        case_node = province_node->case_head;
        while (case_node != NULL)
        {
            amount = case_node->content.amount;
            for (count = 0 ;count < MAXAMOUNTNUM ; count++)
            {
                if (amount > result[count].amount)
                {
                    for (temp = MAXAMOUNTNUM - 2 ; temp >= count ; temp--)
                        result[temp + 1] = result[temp];
                    strcpy(result[count].number, case_node->content.number);
                    strcpy(result[count].name, case_node->content.defendant);
                    result[count].level = case_node->content.level;
                    result[count].charge = case_node->content.charge;
                    result[count].amount = case_node->content.amount;
                    result[count].punishment = case_node->content.punishment;
                    break;
                }
            }
            case_node = case_node->next;
        }
        province_node = province_node->next;
    }
    return result;
}

/*************************************************
Function: sort_charge_times
Description: Sort the charge stastical result according to the number of the cases.
Input: Charge * indicating the unsort result
Output: None
Return: None
Others: operating the same array
*************************************************/
void sort_charge_times(Charge * unsort)
{
    int i, j;
    Charge temp;
    for (i = 0 ; i < CHARGENUM - 1 ; i++)
        for (j = 0 ; j < CHARGENUM - i - 1 ; j++)
        {
            if (unsort[j].times < unsort[j+1].times)
            {
                temp = unsort[j];
                unsort[j] = unsort[j+1];
                unsort[j+1] = temp;
            }
        }
}

/*************************************************
Function: sort_place_times
Description: Sort the place stastical result according to the number of the cases.
Input: Place * indicating the unsort result
Output: None
Return: None
Others: operating the same array
*************************************************/
void sort_place_times(Place * unsort, int number)
{
    int i, j;
    Place temp;
    for (i = 0 ; i < number - 1 ; i++)
        for (j = 0 ; j < number - i - 1 ; j++)
        {
            if (unsort[j].times < unsort[j+1].times)
            {
                temp = unsort[j];
                unsort[j] = unsort[j+1];
                unsort[j+1] = temp;
            }
        }
}
