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


typedef struct  escEscapeTechnion* System;

System InitiateSystem();

void DestroySystem(System system);

List GetCompanyList(System  system);

MtmErrorCode CompanyAdd(System  system, char* email, TechnionFaculty faculty);

MtmErrorCode CompanyRemove(System  system,Company company);
//
//MtmErrorCode RoomAdd(char* email, int id, int price, int num_ppl, char*);
//
//MtmErrorCode RoomRemove(TechnionFaculty faculty, int id);
//
//MtmErrorCode EscaperAdd(char* email, TechnionFaculty faculty, int skill_level);
//
//MtmErrorCode EscperRemove(char* email);
//
//MtmErrorCode EscaperOrder (char* email, TechnionFaculty faculty, int id ,char* time ,int num_ppl);
//
//MtmErrorCode EscaperRecommend(char* email, int num_ppl);
//
//MtmErrorCode ReportDay();
//
//MtmErrorCode ReportBest();
//
//static MtmErrorCode SearchForCompany(char* email);
//
//static MtmErrorCode SearchForEscaper(char* email);
//
//static MtmErrorCode SearchForRoom(TechnionFaculty faculty, int room_id);
//
//static MtmErrorCode SearchForOrder(TechnionFaculty faculty, int room_id);
//
int GetNumberOfCompanies(System system);
//
//static int GetNumberOfEscapers(System system);
//
//static MtmErrorCode SortCompaniesByIncome(System system);
//
//static List FilterCompaniesByFaculty(System system);



#endif //EX3ADT_SYSTEM_H
