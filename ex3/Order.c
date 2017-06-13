//
// Created by orerez on 08/06/2017.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Order.h"



struct escOrder {
    char* email;
    TechnionFaculty faculty;
    int id;
    int day;
    int hour;
    int num_people;

};

static MtmErrorCode ValidateOrderParameters(char* Email,int id, int day, int hour, int num_ppl) {
    if(Email==NULL)
        return MTM_NULL_PARAMETER;
    if(id<=0 || day<0 || hour <0 || hour >23 || num_ppl <0)
        return MTM_INVALID_PARAMETER;
    if(day==0 && hour ==0)
        return MTM_INVALID_PARAMETER;

    return MTM_SUCCESS;

}


Order CreateOrder(char* email, TechnionFaculty faculty, int id, int day, int hour, int num_ppl,MtmErrorCode* Error) {
    *Error = ValidateOrderParameters(email,id,day,hour,num_ppl);
    if(*Error!=MTM_SUCCESS)
        return NULL;
    Order order = malloc(sizeof(*(order)));
    if(order==NULL) {
        *Error = MTM_OUT_OF_MEMORY;
        return NULL;
    }
    char* new_email = malloc(strlen(email)+1);
    if(email==NULL) {
        *Error = MTM_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(new_email,email);
    order->email=new_email;
    order->faculty=faculty;
    order->id=id;
    order->day=day;
    order->hour=hour;
    order->num_people=num_ppl;

    *Error=MTM_SUCCESS;
    return order;
}

MtmErrorCode DestroyOrder(Order order) {
    assert(order!=NULL);
    free(order->email);
    order->email=NULL;
    free(order);
    return MTM_SUCCESS;
}

int GetID(Order order) {
    assert(order!=NULL);
    return order == NULL ? -1 : order->id;
}

char* GetOrderEmail(Order order) {
    assert(order!=NULL);
    return order == NULL ? NULL : order->email;
}

TechnionFaculty GetOrderFaculty(Order order) {
    assert(order!=NULL);
    return order == NULL ? UNKNOWN : order->faculty;
}

int GetRemainingDays(Order order) {
    assert(order!=NULL);
    return order == NULL ? -1 : order->day;
}

MtmErrorCode DecreaseDay(Order order) {
    assert(order!=NULL);
    if(order->day<=0)
        return MTM_INVALID_PARAMETER;
    order->day--;
    return MTM_SUCCESS;
}

void IncreaseDay(Order order) {
    assert(order!=NULL);
    order->day++;
}

int GetRemainingHours(Order order) {
    assert(order!=NULL);
    return order == NULL ? -1 : order->hour;
}

int GetNumOfPeople(Order order) {
    assert(order!=NULL);
    return order == NULL ? -1 : order->num_people;
}

