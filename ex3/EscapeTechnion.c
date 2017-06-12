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
};

ListElement CopyCompanyElement (ListElement element){
    MtmErrorCode error_code = MTM_SUCCESS;
    Company new_comapny;
    new_comapny= CopyCompany(element,&error_code);
    if(error_code!=MTM_SUCCESS)
        return NULL;
    return new_comapny;
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
    Escaper escaper1 = (Escaper)element1;
    Escaper escaper2 = (Escaper)element2;
    assert((escaper1!=NULL) && (escaper2!=NULL));
    if(GetEscaperFaculty(escaper1) > GetEscaperFaculty(escaper2))
        return 1;
    if(GetEscaperFaculty(escaper1) < GetEscaperFaculty(escaper2))
        return -1;
    return 0;
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
    return system;
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

void DestroySystem(System system) {

    listDestroy(system->Companies);
    setDestroy(system->Escapers);
    free(system);
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

MtmErrorCode CompanyRemove(System  system,Company company) {
    ListResult result=LIST_SUCCESS;
    LIST_FOREACH(Company,i,system->Companies){
        if(strcmp(GetCompanyEmail(company),GetCompanyEmail(i))==0){
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



