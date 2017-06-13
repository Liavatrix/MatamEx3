//
// Created by orerez on 09/06/2017.
//

//
// Created by orerez on 09/06/2017.
//

#include "test_utilities.h"
#include "../Room.h"

static bool testRoomCreate(){
    MtmErrorCode errorCode=MTM_SUCCESS;
    ASSERT_TEST( CreateRoom(NULL,CIVIL_ENGINEERING,1,12,3,4,5,22,&errorCode) == NULL);
    ASSERT_TEST(errorCode==MTM_NULL_PARAMETER);
    ASSERT_TEST( CreateRoom("avi@gmail.com",PHYSICS,-2,16,4,7,2,21,&errorCode) == NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    ASSERT_TEST( CreateRoom("avi@gmail.com",PHYSICS,2,-4,4,7,2,21,&errorCode) == NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateRoom("avi@gmail.com",PHYSICS,2,16,-5,7,2,21,&errorCode)==NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateRoom("avi@gmail.com",PHYSICS,2,16,4,-1,2,21,&errorCode)== NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateRoom("avi@gmail.com",PHYSICS,2,16,4,11,2,21,&errorCode)== NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateRoom("avi@gmail.com",PHYSICS,2,16,4,7,-8,21,&errorCode) == NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateRoom("avi@gmail.com",PHYSICS,2,16,4,7,27,21,&errorCode) == NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateRoom("avi@gmail.com",PHYSICS,2,16,4,7,2,-8,&errorCode) == NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateRoom("avi@gmail.com",PHYSICS,2,16,4,7,2,27,&errorCode) == NULL);
    ASSERT_TEST(errorCode==MTM_INVALID_PARAMETER);
    Room first = CreateRoom("Georgi54@avocado.rus",INDUSTRIAL_ENGINEERING_AND_MANAGEMENT,1,12,3,4,5,22,&errorCode);
    ASSERT_TEST(first != NULL);
    ASSERT_TEST(errorCode==MTM_SUCCESS);
    DestroyRoom(first);

    return true;
}

static bool testRoomDestroy(){

    MtmErrorCode errorCode=MTM_SUCCESS;
    MtmErrorCode errorCode2=MTM_SUCCESS;
    Room first = CreateRoom("sherlock@holmes.co.il",BIOTECHNOLOGY_AND_FOOD_ENGINEERING,1,12,3,4,5,22,&errorCode);
    Room second = CreateRoom("mister@watson.co.il",BIOTECHNOLOGY_AND_FOOD_ENGINEERING,1,12,3,4,5,22,&errorCode2);
    DestroyRoom(first);
    DestroyRoom(second);
    return true;
}


static bool testGetRoomID(){
    MtmErrorCode errorCode=MTM_SUCCESS;
    Room first = CreateRoom("sherlock@holmes.co.il",BIOTECHNOLOGY_AND_FOOD_ENGINEERING,1,12,3,4,5,22,&errorCode);
    ASSERT_TEST(GetRoomID(NULL)==-1);
    int id = GetRoomID(first);
    ASSERT_TEST(id==1);
    DestroyRoom(first);
    return true;
}

static bool testGetRoomPrice(){

    MtmErrorCode errorCode=MTM_SUCCESS;
    Room first = CreateRoom("sherlock@holmes.co.il",BIOTECHNOLOGY_AND_FOOD_ENGINEERING,1,12,3,4,5,22,&errorCode);
    int price = GetRoomPrice(first);
    ASSERT_TEST(price==12);
    DestroyRoom(first);
    return true;
}

static bool testGetRoomNumPeople(){
    MtmErrorCode errorCode=MTM_SUCCESS;
    Room first = CreateRoom("sherlock@holmes.co.il",BIOTECHNOLOGY_AND_FOOD_ENGINEERING,1,12,3,4,5,22,&errorCode);
    int num = GetNumPeople(first);
    ASSERT_TEST(num==3);
    DestroyRoom(first);
    return true;
}

static bool testGetDifficulty(){

    MtmErrorCode errorCode=MTM_SUCCESS;
    Room first = CreateRoom("sherlock@holmes.co.il",BIOTECHNOLOGY_AND_FOOD_ENGINEERING,1,12,3,4,5,22,&errorCode);
    int Difficulty = GetRoomDifficulty(first);
    ASSERT_TEST(Difficulty==4);
    DestroyRoom(first);
    return true;
}

static bool testGetRoomStartTime(){

    MtmErrorCode errorCode=MTM_SUCCESS;
    Room first = CreateRoom("sherlock@holmes.co.il",BIOTECHNOLOGY_AND_FOOD_ENGINEERING,1,12,3,4,5,22,&errorCode);
    int time = GetRoomStartTime(first);
    ASSERT_TEST(time==5);
    DestroyRoom(first);
    return true;
}

static bool testGetRoomCloseTime(){

    MtmErrorCode errorCode=MTM_SUCCESS;
    Room first = CreateRoom("sherlock@holmes.co.il",BIOTECHNOLOGY_AND_FOOD_ENGINEERING,1,12,3,4,5,22,&errorCode);
    int time = GetRoomCloseTime(first);
    ASSERT_TEST(time==22);
    DestroyRoom(first);
    return true;
}


int RoomTests (int argv, char** arc){
    RUN_TEST(testRoomCreate);
    RUN_TEST(testRoomDestroy);
    RUN_TEST(testGetRoomID);
    RUN_TEST(testGetRoomPrice);
    RUN_TEST(testGetRoomNumPeople);
    RUN_TEST(testGetDifficulty);
    RUN_TEST(testGetRoomStartTime);
    RUN_TEST(testGetRoomCloseTime);

    return 0;
}