#include <stdlib.h>
#include "list.h"
#include "Escaper.h"
#include <string.h>
#include <assert.h>

struct escEscaper {
    char* email;
    TechnionFaculty faculty;
    int skill_level;
    List orders;
};

typedef void* listElement;

inline static int EmailIsValid(char* email) {
    int count=0;
    for (int i = 0; i < strlen(email); ++i) {
        if(email[i]=='@')
            count++;
    }
    return (count==1);
}

inline static int ValidateParameters(int id, int day, int hour, int num_people) {
    return (id > 0 && day > -1 && hour > -1 && hour < 24 && num_people > 0);
}

static listElement CopyOrder(listElement element) {
    Order order = (Order)element;
    MtmErrorCode code = MTM_SUCCESS;
    Order new_order = CreateOrder(GetOrderEmail(order),GetOrderFaculty(order),GetID(order),GetRemainingDays(order),
                                  GetRemainingHours(order),GetNumOfPeople(order),&code);
    listElement new_element = (listElement)new_order;
    if(new_order==NULL)
        return NULL;
    return new_element;
}

static void FreeOrder(listElement element) {
    if(element!=NULL) {
        Order order = (Order) element;
        DestroyOrder(order);
    }
}

static int CompareOrders(listElement list_element1 , listElement list_element2) {
    Order Order1 = (Order)list_element1;
    Order Order2 = (Order)list_element2;
    assert((Order1!=NULL) && (Order2!=NULL));
    if(GetID(Order1) > GetID(Order2))
        return 1;
    if(GetID(Order1) < GetID(Order2))
        return -1;
    return 0;
}

Escaper CreateEscaper(char* email, TechnionFaculty faculty, int skill_level,MtmErrorCode* error_code) {
    if(email==NULL) {
        *error_code = MTM_NULL_PARAMETER;
        return NULL;
    }
    if(!EmailIsValid(email)||skill_level<1|| skill_level>10) {
        *error_code = MTM_INVALID_PARAMETER;
        return NULL;
    }
    char* new_email = malloc(strlen(email)+1);
    if(new_email==NULL)
        return NULL;
    Escaper escaper = malloc(sizeof(*escaper));
    if(escaper == NULL)
    {
        free(new_email);
        *error_code=MTM_OUT_OF_MEMORY;
        return NULL;
    }

    CopyListElement copy_order = CopyOrder;
    FreeListElement free_order= FreeOrder;
    escaper->orders = listCreate(copy_order,free_order);
    if(escaper->orders==NULL)
        return NULL;

    strcpy(new_email,email);
    escaper->email=new_email;
    escaper->faculty=faculty;
    escaper->skill_level=skill_level;
    *error_code=MTM_SUCCESS;
    return escaper;
}

Escaper CopyEscaper(Escaper escaper, MtmErrorCode* error_code) {
    assert(escaper!=NULL);
    Escaper new_escaper = CreateEscaper(escaper->email,escaper->faculty,escaper->skill_level,error_code);
    if(*error_code!=MTM_SUCCESS)
        return NULL;
    ListResult result;
    LIST_FOREACH(Order,i,escaper->orders) {
        if(listGetSize(new_escaper->orders)==0)
            result = listInsertFirst(new_escaper->orders,i);
        result = listInsertLast(new_escaper->orders,i);
        if(result!=LIST_SUCCESS) {
            DestroyEscaper(new_escaper);
            switch(result) {
                case LIST_NULL_ARGUMENT:
                    *error_code=MTM_NULL_PARAMETER;
                case LIST_OUT_OF_MEMORY:
                    *error_code=MTM_OUT_OF_MEMORY;
                default:
                    *error_code=MTM_INVALID_PARAMETER;
            }
            return NULL;
        }
    }

    *error_code=MTM_SUCCESS;
    return new_escaper;
}

MtmErrorCode CreateInsertOrderFirst(Escaper escaper,int id, int day, int hour, int num_ppl) {
    assert(escaper!=NULL);
    if(!ValidateParameters(id,day,hour,num_ppl))
        return MTM_INVALID_PARAMETER;
    MtmErrorCode error_code;
    Order order = CreateOrder(escaper->email,escaper->faculty,id,day,hour,num_ppl,&error_code);
    if(error_code!=MTM_SUCCESS)
        return error_code;
    ListResult list_result = listInsertFirst(escaper->orders,order);
    DestroyOrder(order);
    if(list_result==LIST_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    if(list_result==LIST_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    return MTM_SUCCESS;
}

MtmErrorCode CreateInsertOrderLast(Escaper escaper,int id, int day, int hour, int num_ppl) {
    assert(escaper!=NULL);
    if(!ValidateParameters(id,day,hour,num_ppl))
        return MTM_INVALID_PARAMETER;
    MtmErrorCode error_code;
    Order order = CreateOrder(escaper->email,escaper->faculty,id,day,hour,num_ppl,&error_code);
    if(error_code!=MTM_SUCCESS)
        return error_code;
    ListResult list_result = listInsertLast(escaper->orders,order);
    DestroyOrder(order);
    if(list_result==LIST_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    if(list_result==LIST_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    if(list_result==LIST_INVALID_CURRENT)
        return MTM_INVALID_PARAMETER;
    return MTM_SUCCESS;
}

MtmErrorCode CreateInsertOrderBeforeCurrent(Escaper escaper,int id, int day, int hour, int num_ppl) {
    assert(escaper!=NULL);
    if(!ValidateParameters(id,day,hour,num_ppl))
        return MTM_INVALID_PARAMETER;
    MtmErrorCode error_code;
    Order order = CreateOrder(escaper->email,escaper->faculty,id,day,hour,num_ppl,&error_code);
    if(error_code!=MTM_SUCCESS)
        return error_code;
    ListResult list_result = listInsertBeforeCurrent(escaper->orders,order);
    DestroyOrder(order);
    if(list_result==LIST_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    if(list_result==LIST_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    if(list_result==LIST_INVALID_CURRENT)
        return MTM_INVALID_PARAMETER;
    return MTM_SUCCESS;
}

MtmErrorCode CreateInsertOrderAfterCurrent(Escaper escaper,int id, int day, int hour, int num_ppl) {
    assert(escaper!=NULL);
    if(!ValidateParameters(id,day,hour,num_ppl))
        return MTM_INVALID_PARAMETER;
    MtmErrorCode error_code;
    Order order = CreateOrder(escaper->email,escaper->faculty,id,day,hour,num_ppl,&error_code);
    if(error_code!=MTM_SUCCESS)
        return error_code;
    ListResult list_result = listInsertAfterCurrent(escaper->orders,order);
    DestroyOrder(order);
    if(list_result==LIST_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    if(list_result==LIST_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    if(list_result==LIST_INVALID_CURRENT)
        return MTM_INVALID_PARAMETER;
    return MTM_SUCCESS;
}

MtmErrorCode RemoveDestroyOrder(Escaper escaper, Order order) {
    assert(escaper!=NULL);
    if(order==NULL)
        return MTM_NULL_PARAMETER;
    listGetFirst(escaper->orders);
    LIST_FOREACH(Order,i,escaper->orders) {
        if(GetID(i)==GetID(order))
            break;
    }
    ListResult list_result = listRemoveCurrent(escaper->orders);
    if (list_result==LIST_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    if(list_result==LIST_INVALID_CURRENT)
        return MTM_ID_DOES_NOT_EXIST;
    return MTM_SUCCESS;
}

MtmErrorCode DestroyEscaper(Escaper escaper) {
    assert(escaper!=NULL);
    listDestroy(escaper->orders);
    free(escaper->email);
    free(escaper);
    return MTM_SUCCESS;
}

Order GetFirstOrder(Escaper escaper) {
    assert(escaper!=NULL);
    return escaper==NULL ? NULL : listGetFirst(escaper->orders);
}

Order GetNextOrder(Escaper escaper) {
    assert(escaper!=NULL);
    return escaper==NULL ? NULL : listGetNext(escaper->orders);
}

int GetNumOrders(Escaper escaper) {
    assert(escaper!=NULL);
    return escaper==NULL ? -1 : listGetSize(escaper->orders);
}

MtmErrorCode ClearEscaperOrders(Escaper escaper) {
    assert(escaper!=NULL);
    listClear(escaper->orders);
    return MTM_SUCCESS;
}

MtmErrorCode SortOrders(Escaper escaper) {
    assert(escaper!=NULL);
    ListResult listResult = listSort(escaper->orders,CompareOrders);
    if(listResult==LIST_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    if(listResult==LIST_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    return MTM_SUCCESS;
}

int EscaperHasOrder(Escaper escaper,Order order) {
    assert(escaper!=NULL);
    if(order==NULL)
        return -1;
    LIST_FOREACH(Order, i,escaper->orders) {
        if( (GetID(order)==GetID(i)) && (GetOrderFaculty(order)==GetOrderFaculty(i)) )
            return 1;
    }
    return 0;
}

Order GetEscaperOrder(Escaper escaper, TechnionFaculty faculty, int id) {
    assert(escaper!=NULL);
    LIST_FOREACH(Order, i,escaper->orders) {
        if( (id==GetID(i)) && (faculty==GetOrderFaculty(i)) )
            return i;
    }
    return NULL;
}

int GetSkillLevel(Escaper escaper) {
    assert(escaper!=NULL);
    return escaper==NULL ? -1 : escaper->skill_level;
}

char* GetEscaperEmail(Escaper escaper) {
    assert(escaper!=NULL);
    return escaper==NULL ? NULL : escaper->email;
    //returns pointer to email !!!
}

MtmErrorCode DecreaseOrdersDay(Escaper escaper) {
    MtmErrorCode error_code = MTM_SUCCESS;
    Order order=NULL;
    LIST_FOREACH(Order,i,escaper->orders){
        error_code=DecreaseDay(i);
        if(error_code!=MTM_SUCCESS) {
            order = i;
            break;
        }
    }

    if(order==NULL)
        return MTM_SUCCESS;

    LIST_FOREACH(Order,i,escaper->orders){
        if(i==order)
            break;
        IncreaseDay(i);
    }

    return MTM_INVALID_PARAMETER;
}

TechnionFaculty GetEscaperFaculty (Escaper escaper) {
    assert(escaper!=NULL);
    return escaper==NULL ? UNKNOWN : escaper->faculty;
}
