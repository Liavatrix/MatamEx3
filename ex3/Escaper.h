//
// Created by orerez on 08/06/2017.
//

#include "mtm_ex3.h"
#include "Order.h"

#ifndef EX3ADT_ESCAPER_H
#define EX3ADT_ESCAPER_H

typedef struct escEscaper* Escaper;


Escaper CreateEscaper(char* email, TechnionFaculty faculty, int skill_level,MtmErrorCode* error_code);

Escaper CopyEscaper(Escaper escaper, MtmErrorCode* error_code);

MtmErrorCode DestroyEscaper(Escaper escaper);

MtmErrorCode CreateInsertOrderFirst(Escaper escaper,int id, int day, int hour, int num_ppl);

MtmErrorCode CreateInsertOrderLast(Escaper escaper,int id, int day, int hour, int num_ppl);

MtmErrorCode CreateInsertOrderBeforeCurrent(Escaper escaper,int id, int day, int hour, int num_ppl);

MtmErrorCode CreateInsertOrderAfterCurrent(Escaper escaper,int id, int day, int hour, int num_ppl);

MtmErrorCode RemoveDestroyOrder(Escaper escaper,Order order);

MtmErrorCode ClearEscaperOrders(Escaper escaper);

MtmErrorCode SortOrders(Escaper escaper);

Order GetFirstOrder(Escaper escaper);

Order GetNextOrder(Escaper escaper);

Order GetEscaperOrder(Escaper escaper, TechnionFaculty faculty, int id);

int GetSkillLevel(Escaper escaper);

TechnionFaculty GetEscaperFaculty(Escaper escaper);

char* GetEscaperEmail(Escaper escaper);

int GetNumOrders(Escaper escaper);

MtmErrorCode DecreaseOrdersDay(Escaper escaper);

int EscaperHasOrder(Escaper escaper,Order order);

#endif //EX3ADT_ESCAPER_H