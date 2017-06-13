//
// Created by orerez on 08/06/2017.
//

#include "mtm_ex3.h"

#ifndef EX3ADT_ORDER_H
#define EX3ADT_ORDER_H

typedef struct escOrder* Order;

Order CreateOrder(char* email, TechnionFaculty faculty, int id, int day, int hour, int num_ppl,MtmErrorCode* Error);

MtmErrorCode DestroyOrder(Order order);

MtmErrorCode DecreaseDay(Order order);

void IncreaseDay(Order order);

char* GetOrderEmail(Order order);

TechnionFaculty GetOrderFaculty(Order order);

int GetID(Order order);

int GetRemainingDays(Order order);

int GetRemainingHours(Order order);

int GetNumOfPeople(Order order);


#endif //EX3ADT_ORDER_H
