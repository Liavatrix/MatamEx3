//
// Created by orerez on 08/06/2017.
//

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "set.h"
#include "Company.h"

struct escCompany {

    char* email;
    TechnionFaculty faculty;
    int income;
    Set rooms;
};

typedef void* setElement;

inline static int EmailIsValid(char* email) {
    int count=0;
    for (int i = 0; i < strlen(email); ++i) {
        if(email[i]=='@')
            count++;
    }
    return (count==1);
}

static int CompareRooms(setElement set_element1 , setElement set_element2) {
    Room room1 = (Room)set_element1;
    Room room2 = (Room)set_element2;
    assert((room1!=NULL) && (room2!=NULL));
    if(GetRoomID(room1) > GetRoomID(room2))
        return 1;
    if(GetRoomID(room1) < GetRoomID(room2))
        return -1;
    return 0;
}

static void FreeRooms(setElement element){
    if(element!=NULL)
    {
        Room room = (Room)element;
        DestroyRoom(room);
    }
}

static setElement CopyRooms(setElement element) {
    Room room1 = (Room)element;
    Room room2 = CopyRoom(room1);
    setElement new_element = (setElement)room2;
    return new_element;
}

Company CreateCompany(char* email, TechnionFaculty faculty,MtmErrorCode *error_code) {
    if(email==NULL) {
        *error_code = MTM_NULL_PARAMETER;
        return NULL;
    }
    if (!EmailIsValid(email)) {
        *error_code = MTM_INVALID_PARAMETER;
        return NULL;
    }
    Company company = malloc(sizeof(*company));
    if(company==NULL) {
        *error_code = MTM_OUT_OF_MEMORY;
        return NULL;
    }
    company->email = malloc(strlen(email)+1);
    if(company->email==NULL) {
        free(company);
        *error_code = MTM_OUT_OF_MEMORY;
        return NULL;
    }
    strcpy(company->email,email);
    company->faculty=faculty;
    company->income=0;

    compareSetElements comp_func = CompareRooms;
    freeSetElements free_func = FreeRooms;
    copySetElements copy_func = CopyRooms;
    company ->rooms = setCreate(copy_func,free_func,comp_func);
    if(company->rooms==NULL) {
        free(email);
        free(company);
        *error_code = MTM_OUT_OF_MEMORY;
        return NULL;
    }
    *error_code = MTM_SUCCESS;
    return company;
}

Company CopyCompany(Company company,MtmErrorCode* error_code) {
    assert(company!=NULL);
    Company new_company = CreateCompany(company->email,company->faculty,error_code);
    if(*error_code!=MTM_SUCCESS)
        return NULL;
    new_company->income=company->income;
    SET_FOREACH(Room,i,company->rooms) {
        SetResult result =setAdd(new_company->rooms,i);
        if(result!=SET_SUCCESS) {
            DestroyCompany(new_company);
            switch (result) {
                case SET_NULL_ARGUMENT:
                    *error_code = MTM_NULL_PARAMETER;
                case SET_OUT_OF_MEMORY:
                    *error_code = MTM_OUT_OF_MEMORY;
                case SET_ITEM_ALREADY_EXISTS:
                    *error_code = MTM_ID_ALREADY_EXIST;
                default:
                    *error_code = MTM_INVALID_PARAMETER;
            }
            return NULL;
        }
    }
    *error_code=MTM_SUCCESS;
    return new_company;
}

MtmErrorCode DestroyCompany(Company company) {
    assert(company != NULL);
    setDestroy(company->rooms);
    assert(company->email!=NULL);
    free(company->email);
    free(company);
    return MTM_SUCCESS;
}

MtmErrorCode CreateInsertRoom(Company company , int id, int price, int num_ppl, int difficulty,
                              int start_time, int end_time) {
    assert(company!=NULL);
    MtmErrorCode error_code;
    Room room = CreateRoom(company->email,company->faculty,id,price,num_ppl,difficulty,start_time,end_time,&error_code);
    if(error_code!=MTM_SUCCESS)
        return error_code;
    SetResult set_result = setAdd(company->rooms,room);
    DestroyRoom(room);
    if(set_result==SET_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    if(set_result==SET_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    if(set_result==SET_ITEM_ALREADY_EXISTS)
        return MTM_ID_ALREADY_EXIST;
    return MTM_SUCCESS;
}

MtmErrorCode RemoveDestroyRoom(Company company, Room room){
    assert(company!=NULL);
   SetResult set_result = setRemove(company->rooms,room);
    if (set_result==SET_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    if(set_result==SET_ITEM_DOES_NOT_EXIST)
        return MTM_ID_DOES_NOT_EXIST;
    return MTM_SUCCESS;

}

MtmErrorCode ClearCompanyRooms(Company company){
    assert(company!=NULL);
    setClear(company->rooms);
    return MTM_SUCCESS;
}

Room GetFirstRoom(Company company){
    if(company==NULL)
        return NULL;
    if(setGetSize(company->rooms)==0)
        return NULL;
    return setGetFirst(company->rooms);
}

Room GetNextRoom(Company company){
    if(company==NULL)
        return NULL;
    if(setGetSize(company->rooms)==0 || setGetSize(company->rooms)==1) //There's no next element
        return NULL;
    return setGetNext(company->rooms);
}

Room SearchRoom(Company company,int id) {
    assert(company!=NULL);
    SET_FOREACH(Room,i,company->rooms) {
        if(GetRoomID(i)==id)
            return i;
    }

    return NULL;
}

Room RoomOfOrder(Company company, Order order){
    assert(company!=NULL);
    return SearchRoom(company,GetID(order));
}

int GetCompanyIncome(Company company) {
    assert(company!=NULL);
    return company == NULL ? -1 : company->income;
}

int GetNumRooms(Company company){
    assert(company!=NULL);
    return company==NULL ? -1 : setGetSize(company->rooms);
}

MtmErrorCode UpdateCompanyIncome(Company company , Order order, int is_from_faculty){
    assert(company!=NULL);
    if(order ==NULL)
        return MTM_NULL_PARAMETER;
    Room room=NULL;
    int days_left = GetRemainingDays(order);
    int hours_left = GetRemainingHours(order);
    if( days_left==-1 || hours_left == -1)
        return MTM_INVALID_PARAMETER;
    if(days_left!=0)
        return MTM_SUCCESS;
    room = RoomOfOrder(company,order);
    if(room==NULL)
        return MTM_NO_ROOMS_AVAILABLE;
    int price = GetRoomPrice(room);
    if(is_from_faculty)
        price= (price*3)/4;
    company->income+=price;
    return MTM_SUCCESS;
}

TechnionFaculty GetCompanyFaculty(Company company) {
    assert(company!=NULL);
    return company==NULL ? UNKNOWN : company->faculty;
}

char* GetCompanyEmail(Company company) {
    assert(company!=NULL);
    return company==NULL ? NULL : company->email;
    //returns pointer to email !!!
}





