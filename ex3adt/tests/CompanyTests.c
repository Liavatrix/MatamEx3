//
// Created by orerez on 10/06/2017.
//

//
// Created by orerez on 09/06/2017.
//

#include "test_utilities.h"
#include "../Order.h"
#include "../Company.h"
#include <string.h>



static bool testCompanyCreate(){
    MtmErrorCode error_code=MTM_SUCCESS;
    ASSERT_TEST(CreateCompany(NULL,MEDICINE,&error_code)==NULL);
    ASSERT_TEST(error_code==MTM_NULL_PARAMETER);
    ASSERT_TEST(CreateCompany("shakedgmail.com",ELECTRICAL_ENGINEERING,&error_code)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateCompany("SjjFb6n@@",ELECTRICAL_ENGINEERING,&error_code)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    Company first_company = CreateCompany("@Maxim72.gov",MECHANICAL_ENGINEERING,&error_code);
    ASSERT_TEST(first_company != NULL);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    DestroyCompany(first_company);
    return true;
}

static bool testCompanyDestroy(){

    MtmErrorCode error_code=MTM_SUCCESS;
    MtmErrorCode error_code2=MTM_SUCCESS;
    Company first_company = CreateCompany("goornish@gmail,om",HUMANITIES_AND_ARTS,&error_code);
    Company second_company =  CreateCompany("S jjb6n@",HUMANITIES_AND_ARTS,&error_code2);
    DestroyCompany(first_company);
    DestroyCompany(second_company);
    return true;
}

static bool testCreateInsertRoom(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Company first_company = CreateCompany("goornish@gmail,om",HUMANITIES_AND_ARTS,&error_code);
    error_code=CreateInsertRoom(first_company,1,2,3,4,5,6);
    ASSERT_TEST(GetNumRooms(first_company)==0);
    ASSERT_TEST(GetFirstRoom(first_company)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    error_code=CreateInsertRoom(first_company,1,20,3,4,5,22);
    ASSERT_TEST(GetNumRooms(first_company)==1);
    ASSERT_TEST(GetFirstRoom(first_company)!=NULL);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    error_code=CreateInsertRoom(first_company,1,32,3,2,7,20);
    ASSERT_TEST(GetNumRooms(first_company)==1);
    ASSERT_TEST(error_code==MTM_ID_ALREADY_EXIST);
    error_code=CreateInsertRoom(first_company,2,32,3,2,7,20);
    ASSERT_TEST(GetNumRooms(first_company)==2);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    GetFirstRoom(first_company); //Redefine set iterator
    ASSERT_TEST(GetNextRoom(first_company)!=NULL);
    error_code = DestroyCompany(first_company);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    return true;
}

static bool testRemoveDestroyRoom(){

    MtmErrorCode error_code1=MTM_SUCCESS;
    MtmErrorCode error_code2=MTM_SUCCESS;
    MtmErrorCode error_code3=MTM_SUCCESS;
    Company first_company = CreateCompany("goornish@gmail,om",HUMANITIES_AND_ARTS,&error_code1);
    error_code1=CreateInsertRoom(first_company,1,20,4,4,7,22);
    error_code2=CreateInsertRoom(first_company,5,32,2,2,6,19);
    error_code3=CreateInsertRoom(first_company,4,12,5,7,7,1);
    ASSERT_TEST(error_code1==MTM_SUCCESS && error_code2 ==MTM_SUCCESS && error_code3==MTM_SUCCESS);
    ASSERT_TEST(GetNumRooms(first_company)==3);
    GetFirstRoom(first_company);
    Room room = GetNextRoom(first_company);
    error_code1= RemoveDestroyRoom(first_company,room);
    room = GetFirstRoom(first_company);
    ASSERT_TEST(error_code1==MTM_SUCCESS);
    ASSERT_TEST(GetNumRooms(first_company)==2);
    ASSERT_TEST(GetRoomID(room)==1);
    room = GetNextRoom(first_company);
    ASSERT_TEST(GetRoomID(room)==5);
    error_code1=RemoveDestroyRoom(first_company,NULL);
    ASSERT_TEST(error_code1==MTM_NULL_PARAMETER);
    room = CreateRoom("@avi",MEDICINE,7,24,6,9,3,18,&error_code2);
    ASSERT_TEST(error_code2==MTM_SUCCESS);
    error_code1=RemoveDestroyRoom(first_company,room);
    DestroyRoom(room);
    DestroyCompany(first_company);
    ASSERT_TEST(error_code1==MTM_ID_DOES_NOT_EXIST);

    return true;
}

static bool testClearCompanyRooms(){

    MtmErrorCode error_code1=MTM_SUCCESS;
    Company first_company = CreateCompany("goornish@gmail,om",HUMANITIES_AND_ARTS,&error_code1);
    CreateInsertRoom(first_company,1,20,4,4,7,22);
    CreateInsertRoom(first_company,5,32,2,2,6,19);
    CreateInsertRoom(first_company,4,12,5,7,7,1);
    ClearCompanyRooms(first_company);
    ASSERT_TEST(GetNumRooms(first_company)==0);
    ASSERT_TEST(GetFirstRoom(first_company)==NULL);
    DestroyCompany(first_company);
    return true;
}

static bool testSearchRoom(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Company first_company = CreateCompany("goornish@gmail,om",HUMANITIES_AND_ARTS,&error_code);
    CreateInsertRoom(first_company,1,20,4,4,7,22);
    CreateInsertRoom(first_company,5,32,2,2,6,19);
    CreateInsertRoom(first_company,4,12,5,7,7,1);
    Room room1 = SearchRoom(first_company,7);
    Room room2 =SearchRoom(first_company,5);
    ASSERT_TEST(room1==NULL);
    GetFirstRoom(first_company);
    room1=GetNextRoom(first_company);
    ASSERT_TEST(room2=room1);
    DestroyCompany(first_company);
    return true;
}

static bool testGetCompanyFaculty(){
    MtmErrorCode error_code = MTM_SUCCESS;
    Company first_company = CreateCompany("goornish@gmail,om",HUMANITIES_AND_ARTS,&error_code);
    ASSERT_TEST(GetCompanyFaculty(first_company)==HUMANITIES_AND_ARTS);
    DestroyCompany(first_company);
    return true;
}

static bool testGetCompanyEmail(){
    MtmErrorCode error_code = MTM_SUCCESS;
    Company first_company = CreateCompany("goornish@gmail,om",HUMANITIES_AND_ARTS,&error_code);
    ASSERT_TEST(strcmp(GetCompanyEmail(first_company),"goornish@gmail,om")==0);
    DestroyCompany(first_company);
    return true;
}

int CompanyTests (int argv, char** arc){
    RUN_TEST(testCompanyCreate);
    RUN_TEST(testCompanyDestroy);
    RUN_TEST(testCreateInsertRoom);
    RUN_TEST(testRemoveDestroyRoom);
    RUN_TEST(testClearCompanyRooms);
    RUN_TEST(testSearchRoom);
    RUN_TEST(testGetCompanyFaculty);
    RUN_TEST(testGetCompanyEmail);
    return 0;
}