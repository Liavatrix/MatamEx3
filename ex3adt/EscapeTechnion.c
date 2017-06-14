//
// Created by orerez on 08/06/2017.
//

#include <stdlib.h>
#include <assert.h>
#include "list.h"
#include "set.h"
#include "mtm_ex3.h"
#include "EscapeTechnion.h"
#include <string.h>

struct escEscapeTechnion {

    List Companies;
    Set Escapers;
    int day;
};

typedef void* listElement;

ListElement CopyCompanyElement (ListElement element){
    MtmErrorCode error_code = MTM_SUCCESS;
    Company new_comapny;
    new_comapny= CopyCompany(element,&error_code);
    if(error_code!=MTM_SUCCESS)
        return NULL;
    return new_comapny;
}

static listElement OrderCopy(listElement element) {
    Order order = (Order)element;
    MtmErrorCode code = MTM_SUCCESS;
    Order new_order = CreateOrder(GetOrderEmail(order),GetOrderFaculty(order),GetID(order),GetRemainingDays(order),
                                  GetRemainingHours(order),GetNumOfPeople(order),&code);
    listElement new_element = (listElement)new_order;
    if(new_order==NULL)
        return NULL;
    return new_element;
}

static void OrderFree (listElement element) {
    if(element!=NULL) {
        Order order = (Order) element;
        DestroyOrder(order);
    }
}

int FurtherInFuture(ListElement element1, ListElement element2){
    if(GetRemainingDays(element1)>GetRemainingDays(element2))
        return 1;
    else if(GetRemainingDays(element1)==GetRemainingDays(element2))
    {
        if(GetRemainingHours(element1)>GetRemainingHours(element2))
            return 1;
        else if(GetRemainingHours(element1)==GetRemainingHours(element2))
            return 0;
        return -1;
    }
    return -1;
}

int SortOrdersByTime(ListElement element1,ListElement element2){
    if(GetRemainingHours(element1)>GetRemainingHours(element2)){
        return 1;
    }
    if(GetRemainingHours(element1)==GetRemainingHours(element2)){
        if(GetOrderFaculty(element1)>GetOrderFaculty(element2)){
            return 1;
        }
        if(GetOrderFaculty(element1)==GetOrderFaculty(element2)){
            if(GetID(element1)>GetID(element2))
                return 1;
            return -1;
        }
        return -1;
    }
    return -1;
}

void FreeCompany(ListElement element) {
    if(element!=NULL)
        DestroyCompany(element);
}

ListElement CopyEscaperElement (ListElement element){
    MtmErrorCode error_code = MTM_SUCCESS;
    Escaper escaper = (Escaper)element;
    Escaper new_escaper = CopyEscaper(escaper,&error_code);
    if(error_code!=MTM_SUCCESS)
        return NULL;
    ListElement new_element = (ListElement)new_escaper;
    return new_element;
}

void FreeEscaper(ListElement element) {
    if(element!=NULL)
        DestroyEscaper(element);
}

int CompareEscapers(ListElement element1, ListElement element2){
    assert((element1!=NULL) && (element2!=NULL));
    if(strcmp(GetEscaperEmail(element1),GetEscaperEmail(element2))==0)
        return 0;
    return 1;
}

static int EmailExists(char* Email, List list,Set set) {
    if(list!=NULL)
    {
        LIST_FOREACH(Company ,i,list) {
            if(strcmp(GetCompanyEmail(i),Email)==0)
                return 1;
        }
    }
    if(set!=NULL)
    {
        SET_FOREACH(Escaper ,i,set) {
            if(strcmp(GetEscaperEmail(i),Email)==0)
                return 1;
        }
    }
    return 0;
}

static int EmailIsValid(char* email) {
    int count=0;
    for (int i = 0; i < strlen(email); ++i) {
        if(email[i]=='@')
            count++;
    }
    return (count==1);
}

static Company SearchForCompany(char* email,List list) {
    LIST_FOREACH(Company, i, list) {
        if (strcmp(email, GetCompanyEmail(i)) == 0) {
            return i;
        }
    }
    return NULL;
}

int GetNumberOfCompanies(System system) {
    int count =0;
    LIST_FOREACH(Company,i,system->Companies) {
        count++;
    }
    return count;
}

List GetCompanyList(System  system) {
    assert(system!=NULL);
    return system->Companies ==NULL ? NULL : system->Companies;
}

Set GetEscapersSet(System system) {
    assert(system!=NULL);
    return system==NULL ? NULL : system->Escapers;

}

Company RoomContainer(TechnionFaculty faculty,int id,List list) {
    LIST_FOREACH(Company,i,list){
        if(GetCompanyFaculty(i)==faculty){
            if(SearchRoom(i,id)!=NULL)
                return i;
        }
    }
    return NULL;
}

Escaper OrderRequester(char* Email,Set set) {
    SET_FOREACH(Escaper,pVoid,set){
        if(strcmp(Email,GetEscaperEmail(pVoid))==0)
            return pVoid;
    }
    return NULL;
}

static int IdAlreadyUsed(TechnionFaculty faculty,int id,List list) {
    Company company = RoomContainer(faculty,id,list);
    if(company==NULL)
        return 0;
    return 1;
}

System InitiateSystem(){
    System system = malloc(sizeof(*system));
    CopyListElement copy_company_func = CopyCompanyElement;
    FreeListElement free_company_func = FreeCompany;
    copySetElements copy_escaper_func = CopyEscaperElement;
    freeSetElements free_escaper_func = FreeEscaper;
    compareSetElements compare_escaper_func = CompareEscapers;

    system->Companies = listCreate(copy_company_func,free_company_func);
    system->Escapers=setCreate(copy_escaper_func,free_escaper_func,compare_escaper_func);
    system->day=0;
    return system;
}

void DestroySystem(System system) {

    listDestroy(system->Companies);
    setDestroy(system->Escapers);
    free(system);
}

MtmErrorCode CompanyAdd(System  system,char* email, TechnionFaculty faculty) {
    MtmErrorCode error_code = MTM_SUCCESS;
    if(EmailExists(email,system->Companies,NULL))
        return MTM_EMAIL_ALREADY_EXISTS;
    Company company = CreateCompany(email,faculty,&error_code);
    if(error_code!=MTM_SUCCESS){
        return error_code;
    }
    ListResult result = listInsertFirst(system->Companies,company);
    DestroyCompany(company);
    if(result==LIST_NULL_ARGUMENT)
        return MTM_NULL_PARAMETER;
    if(result==LIST_OUT_OF_MEMORY)
        return MTM_OUT_OF_MEMORY;
    return MTM_SUCCESS;
}

MtmErrorCode CompanyRemove(System  system,char* email) {
    ListResult result=LIST_SUCCESS;
    LIST_FOREACH(Company,i,system->Companies){
        if(strcmp(email,GetCompanyEmail(i))==0){
            result = listRemoveCurrent(system->Companies);
        }
    }

    if(result!=LIST_SUCCESS){
        if(result==LIST_NULL_ARGUMENT)
            return MTM_NULL_PARAMETER;
        if(result==LIST_INVALID_CURRENT)
            return MTM_INVALID_PARAMETER;
    }

    return MTM_SUCCESS;
}

MtmErrorCode RoomAdd(System  system,char* email, int id, int price, int num_ppl,int start_time, int end_time, int difficulty) {
    Company company = SearchForCompany(email,system->Companies);
    if(company==NULL)
        return MTM_COMPANY_EMAIL_DOES_NOT_EXIST;
    TechnionFaculty company_faculty = GetCompanyFaculty(company);
    if(IdAlreadyUsed(company_faculty,id,system->Companies))
        return MTM_ID_ALREADY_EXIST;
    MtmErrorCode error_code = CreateInsertRoom(company,id,price,num_ppl,difficulty,start_time,end_time);
    if(error_code!=MTM_SUCCESS)
        return error_code;
    return MTM_SUCCESS;
}

MtmErrorCode RoomRemove(System  system,TechnionFaculty faculty, int id){
    MtmErrorCode error_code= MTM_SUCCESS;
    Company company = RoomContainer(faculty,id,system->Companies);
    if(company==NULL)
        return MTM_ID_DOES_NOT_EXIST;
    Room room = SearchRoom(company,id);
    error_code=RemoveDestroyRoom(company,room);
    return error_code;
}

MtmErrorCode EscaperAdd(System system,char* email, TechnionFaculty faculty, int skill_level){
    MtmErrorCode err=MTM_SUCCESS;
    Escaper new_escaper = CreateEscaper(email,faculty,skill_level,&err);
    if(err!=MTM_SUCCESS){
        return err;
    }
    SetResult result;
    result=setAdd(system->Escapers,new_escaper);
    DestroyEscaper(new_escaper);
    if(result!=SET_SUCCESS){
        assert(result!=SET_NULL_ARGUMENT);
        if(result==SET_ITEM_ALREADY_EXISTS){
            return MTM_EMAIL_ALREADY_EXISTS;
        }
        if(result==SET_OUT_OF_MEMORY) {
            return MTM_OUT_OF_MEMORY;
        }
    }
    return MTM_SUCCESS;
}

MtmErrorCode EscaperRemove(System system, char* email){
    assert(system!=NULL);
    SetResult result=SET_ITEM_DOES_NOT_EXIST;
    SET_FOREACH(Escaper,pVoid,system->Escapers){
        if(strcmp(GetEscaperEmail(pVoid),email)==0)
            result=setRemove(system->Escapers,pVoid);
    }
    if(result!=SET_SUCCESS){
        assert(result!=SET_NULL_ARGUMENT);
        if(result==SET_ITEM_DOES_NOT_EXIST)
            return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    }
    return MTM_SUCCESS;
}

MtmErrorCode EscaperOrder(System  system,char* email, TechnionFaculty faculty, int id ,int day,int hour ,int num_ppl){
    if(id < 0 || day <0 || hour <0 || hour >23 || !EmailIsValid(email))
        return MTM_INVALID_PARAMETER;

    List order_list;
    Escaper escaper;
    assert(email!=NULL);
    MtmErrorCode err=MTM_CLIENT_EMAIL_DOES_NOT_EXIST;
    SET_FOREACH(Escaper,pVoid,system->Escapers) {
        if (strcmp(GetEscaperEmail(pVoid), email) == 0) {
            err=MTM_SUCCESS;
            escaper=pVoid;
            order_list = GetListOfEscaper(pVoid);
            LIST_FOREACH(Order,i,order_list){
                if(GetRemainingHours(i)==hour && GetRemainingDays(i)==day)
                    return MTM_CLIENT_IN_ROOM;
            }
        }
    }

    if(err!=MTM_SUCCESS)
        return err;

    Company company = RoomContainer(faculty,id,system->Companies);
    Room room=SearchRoom(company,id);
    if(GetRoomStartTime(room)>hour || GetRoomCloseTime(room)<hour)
        return MTM_ROOM_NOT_AVAILABLE;

    SET_FOREACH(Escaper,pVoid,system->Escapers){
        order_list=GetListOfEscaper(pVoid);
        LIST_FOREACH(Order,order_iterator,order_list){
            if(GetOrderFaculty(order_iterator)==faculty && GetID(order_iterator)==id){
                if(GetRemainingDays(order_iterator)==day && GetRemainingHours(order_iterator)==hour)
                    return MTM_RESERVATION_EXISTS;
            }
        }
    }
    MtmErrorCode err2 = CreateInsertOrderFirst(escaper,faculty,id, day, hour, num_ppl);
    if(err2!=MTM_SUCCESS)
        return err2;
    return err;
}

static void CreateOrderASAP(char* email,TechnionFaculty faculty,int room_id,int num_ppl,MtmErrorCode* err,System system){
    Room room=NULL;
    LIST_FOREACH(Company,pVoid1,system->Companies){
        SET_FOREACH(Room,j,GetCompanyRooms(pVoid1)){
            if(GetRoomID(j)==room_id && GetCompanyFaculty(pVoid1)==faculty){
                room=j;
            }

        }
    }

    int hour = GetRoomStartTime(room),day=0, MinDaysRemain=0;
    int MinHoursRemain=hour;
    Escaper escaper=NULL;
    CompareListElements compare_func=FurtherInFuture;
    FreeListElement free_func = OrderFree;
    CopyListElement copy_func = OrderCopy;
    List AllOrders = listCreate(copy_func,free_func);

    SET_FOREACH(Escaper,pVoid,system->Escapers){
        if(strcmp(GetEscaperEmail(pVoid),email)==0)
            escaper=pVoid;
        List orders_list=GetListOfEscaper(pVoid);
        LIST_FOREACH(Order,i,orders_list){
            if(GetID(i)==room_id && GetOrderFaculty(i)==faculty)
            listInsertFirst(AllOrders,i);
        }
    }
    ListResult sort_result =listSort(AllOrders,compare_func);
    if(sort_result==LIST_NULL_ARGUMENT) {
        *err = MTM_NULL_PARAMETER;
        return;
    }
    if(sort_result==LIST_OUT_OF_MEMORY) {
        *err = MTM_OUT_OF_MEMORY;
        return;
    }
    LIST_FOREACH(Order,j,AllOrders){
        if(GetRemainingDays(j)==day && GetRemainingHours(j)==hour) {
            if (hour == GetRoomCloseTime(room)) {
                day++;
                hour = GetRoomStartTime(room);
            } else
                hour++;
        } else {
            MinDaysRemain = day;
            MinHoursRemain=hour;
            break;
        }
    }
    listDestroy(AllOrders);
    if(escaper!=NULL)
        *err =CreateInsertOrderLast(escaper,faculty,room_id,MinDaysRemain,MinHoursRemain,num_ppl);

}

MtmErrorCode EscaperRecommend(System  system,char* email, int num_ppl){
    assert(system!=NULL);
    if(!EmailIsValid(email)||num_ppl<0)
        return MTM_INVALID_PARAMETER;
    MtmErrorCode err=MTM_SUCCESS;
    Set rooms;
    int recommend_factor_min=9999999,recommend_factor=0;
    int skill_level=0,id=UNKNOWN,faculty_id=99999,room_id=99999;
    SET_FOREACH(Escaper,j,system->Escapers){
        if(strcmp(GetEscaperEmail(j),email)==0) {
            skill_level = GetSkillLevel(j);
            id=GetEscaperFaculty(j);
        }
    }
    if(skill_level==0)
        return MTM_CLIENT_EMAIL_DOES_NOT_EXIST;

    LIST_FOREACH(Company,pVoid,system->Companies){
        rooms=GetCompanyRooms(pVoid);
        SET_FOREACH(Room,i,rooms){
            recommend_factor=(GetNumPeople(i)-num_ppl)*(GetNumPeople(i)-num_ppl)+
                    (GetRoomDifficulty(i)-skill_level)*(GetRoomDifficulty(i)-skill_level);
            if(recommend_factor==recommend_factor_min){
                if(abs(GetCompanyFaculty(pVoid)-id)==abs(faculty_id-id)){
                    if(GetCompanyFaculty(pVoid)==faculty_id){
                        if(GetRoomID(i)<room_id){
                            room_id=GetRoomID(i);
                        }
                    }
                    if(GetCompanyFaculty(pVoid)<faculty_id){
                        recommend_factor_min=recommend_factor;
                        faculty_id=GetCompanyFaculty(pVoid);
                        room_id=GetRoomID(i);
                    }
                }
                if(abs(GetCompanyFaculty(pVoid)-id)<abs(faculty_id-id)){
                    recommend_factor_min=recommend_factor;
                    faculty_id=GetCompanyFaculty(pVoid);
                    room_id=GetRoomID(i);
                }
            }
            if(recommend_factor<recommend_factor_min) {
                recommend_factor_min = recommend_factor;
                faculty_id=GetCompanyFaculty(pVoid);
                room_id=GetRoomID(i);
            }
        }
    }
    if(faculty_id==99999)
        return MTM_NO_ROOMS_AVAILABLE;
    CreateOrderASAP(email,(TechnionFaculty)faculty_id,room_id,num_ppl,&err,system);
    return err;
}

MtmErrorCode ReportDay(System system, FILE* file) {
    int orders_completed = 0;
    MtmErrorCode err=MTM_SUCCESS;
    CompareListElements compare_func = SortOrdersByTime;
    FreeListElement free_func = OrderFree;
    CopyListElement copy_func = OrderCopy;
    List orders_finished = listCreate(copy_func, free_func);
    if (orders_finished == NULL)
        return MTM_OUT_OF_MEMORY;
    List orders_of_escaper=NULL;
    SET_FOREACH(Escaper, pVoid, system->Escapers) {
        orders_of_escaper=GetListOfEscaper(pVoid);
        LIST_FOREACH(Order, i, orders_of_escaper) {
            if (GetRemainingDays(i) == 0) {
                orders_completed++;
                listInsertFirst(orders_finished, i);
                listRemoveCurrent(orders_of_escaper);
                listGetFirst(orders_of_escaper);
            }
        }
    }
    Escaper escaper=NULL;
    Room room=NULL;
    Company company=NULL;
    mtmPrintDayHeader(file,system->day,orders_completed);
    listSort(orders_finished,compare_func);
    int price=0;

    LIST_FOREACH(Order,i,orders_finished){
        escaper=OrderRequester(GetOrderEmail(i),system->Escapers);
        company=RoomContainer(GetOrderFaculty(i),GetID(i),system->Companies);
        room=SearchRoom(company,GetID(i));
        price=GetRoomPrice(room);
        if(GetEscaperFaculty(escaper)==GetOrderFaculty(i)) {
            price = 3 * price / 4;
        }
        price*=GetNumOfPeople(i);
        UpdateCompanyIncome(company,price);
        mtmPrintOrder(file,GetOrderEmail(i),GetSkillLevel(escaper),GetEscaperFaculty(escaper),
                      GetCompanyEmail(company),GetOrderFaculty(i),GetID(i),GetRemainingHours(i),
                      GetRoomDifficulty(room), GetNumOfPeople(i),price);
    }
    listDestroy(orders_finished);
    SET_FOREACH(Escaper,i,system->Escapers){
        err=DecreaseOrdersDay(i);
        if(err!=MTM_SUCCESS)
            return err;
    }
    mtmPrintDayFooter(file,system->day);
    system->day++;
    return err;
}

MtmErrorCode ReportBest(System system, FILE* file) {
    int sum_revenue=0,count_faculties=0;
    int faculty_hist[UNKNOWN];
    int arr_winners_revenue[3];
    int arr_winners[3];

    for (int i = 0; i < UNKNOWN; ++i) {
        faculty_hist[i]=0;
    }


    LIST_FOREACH(Company,pVoid,system->Companies){
        sum_revenue+=GetCompanyIncome(pVoid);
        faculty_hist[GetCompanyFaculty(pVoid)]+=GetCompanyIncome(pVoid);
    }

    for (int j = 0; j < UNKNOWN; ++j) {
        if(faculty_hist[j]!=-1){
            count_faculties++;
        }
    }

    int max=0;
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < UNKNOWN; ++i) {
            if(faculty_hist[i]>max) {
                max=faculty_hist[i];
                arr_winners[j]=i;
            }
        }
        arr_winners_revenue[j]=faculty_hist[arr_winners[j]];
        faculty_hist[arr_winners[j]]=-1;
        max=-1;
    }


    mtmPrintFacultiesHeader(file,  count_faculties, system->day,sum_revenue);

    for (int k = 0; k < 3; ++k) {
        mtmPrintFaculty(file, (TechnionFaculty)arr_winners[k],arr_winners_revenue[k]);
    }
    mtmPrintFacultiesFooter(file);
    return MTM_SUCCESS;
}