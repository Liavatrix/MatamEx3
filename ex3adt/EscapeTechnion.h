//
// Created by orerez on 08/06/2017.
//

#ifndef EX3ADT_SYSTEM_H
#define EX3ADT_SYSTEM_H

#include "Company.h"
#include "Room.h"
#include "Escaper.h"
#include "Order.h"
#include "mtm_ex3.h"
#include "list.h"
#include "set.h"


typedef struct  escEscapeTechnion* System;

System InitiateSystem();

void DestroySystem(System system);

List GetCompanyList(System  system);

Set GetEscapersSet(System system);

MtmErrorCode CompanyAdd(System  system, char* email, TechnionFaculty faculty);

MtmErrorCode CompanyRemove(System  system,Company company);

MtmErrorCode RoomAdd(System  system,char* email, int id, int price, int num_ppl, int difficulty,int start_time, int end_time);

MtmErrorCode RoomRemove(System  system,TechnionFaculty faculty, int id);

Company RoomContainer(TechnionFaculty faculty,int id,List list);

MtmErrorCode EscaperAdd(System system,char* email, TechnionFaculty faculty, int skill_level);

MtmErrorCode EscaperRemove(System  system,char* email);

MtmErrorCode EscaperOrder (System  system,char* email, TechnionFaculty faculty, int id ,int day,int hour ,int num_ppl);

MtmErrorCode EscaperRecommend(System  system,char* email, int num_ppl);
//
//MtmErrorCode ReportDay();
//
//MtmErrorCode ReportBest();

//static MtmErrorCode SearchForEscaper(char* email);
//
//static MtmErrorCode RoomContainer(TechnionFaculty faculty, int room_id);
//
MtmErrorCode SearchForOrder(TechnionFaculty faculty, int room_id);
//
int GetNumberOfCompanies(System system);
//
//static int GetNumberOfEscapers(System system);
//
//static MtmErrorCode SortCompaniesByIncome(System system);
//
//static List FilterCompaniesByFaculty(System system);



#endif //EX3ADT_SYSTEM_H
