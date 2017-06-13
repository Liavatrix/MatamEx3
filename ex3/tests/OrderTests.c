//
// Created by orerez on 09/06/2017.
//

#include "test_utilities.h"
#include "../Order.h"
#include <string.h>

static bool testOrderCreate(){
    MtmErrorCode error_code=MTM_SUCCESS;
    ASSERT_TEST(CreateOrder(NULL,BIOLOGY,1,1,1,1,&error_code) == NULL);
    ASSERT_TEST(error_code==MTM_NULL_PARAMETER);
    ASSERT_TEST(CreateOrder("avi@gmail.com",AEROSPACE_ENGINEERING,0,1,1,1,&error_code) == NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateOrder("avi@gmail.com",AEROSPACE_ENGINEERING,1,-1,1,1,&error_code) == NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateOrder("avi@gmail.com",AEROSPACE_ENGINEERING,1,1,-2,1,&error_code) == NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateOrder("avi@gmail.com",AEROSPACE_ENGINEERING,1,1,1,-3,&error_code) == NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateOrder("avi@gmail.com",AEROSPACE_ENGINEERING,1,1,24,1,&error_code) == NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateOrder("avi@gmail.com",AEROSPACE_ENGINEERING,1,0,0,1,&error_code) == NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    Order first = CreateOrder("john@walla.co.il",EDUCATION_IN_TECH_AND_SCIENCE,1,1,1,1,&error_code);
    ASSERT_TEST(first != NULL);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    DestroyOrder(first);

    return true;
}

static bool testOrderDestroy(){

    MtmErrorCode error_code=MTM_SUCCESS;
    MtmErrorCode error_code2=MTM_SUCCESS;
    Order first = CreateOrder("john@walla.co.il",MATERIALS_ENGINEERING,4,2,23,3,&error_code);
    Order second =  CreateOrder("hollad@walla.co.il",MATERIALS_ENGINEERING,5,7,18,5,&error_code2);
    DestroyOrder(first);
    DestroyOrder(second);
    return true;
}

static bool testGetID(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Order first = CreateOrder("john@walla.co.il",MATERIALS_ENGINEERING,4,2,23,3,&error_code);
    int id = GetID(first);
    ASSERT_TEST(id==4);
    DestroyOrder(first);
    return true;
}

static bool testGetRemainingDays(){

    MtmErrorCode error_code=MTM_SUCCESS;
    Order first = CreateOrder("john@walla.co.il",MATERIALS_ENGINEERING,4,2,23,3,&error_code);
    int days = GetRemainingDays(first);
    ASSERT_TEST(days==2);
    DestroyOrder(first);
    return true;
}

static bool testGetRemainingHours(){

    MtmErrorCode error_code=MTM_SUCCESS;
    Order first = CreateOrder("john@walla.co.il",MATERIALS_ENGINEERING,4,2,23,3,&error_code);
    int hours = GetRemainingHours(first);
    ASSERT_TEST(hours==23);
    DestroyOrder(first);
    return true;
}

static bool testGetNumOfPeople(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Order first = CreateOrder("john@walla.co.il",MATERIALS_ENGINEERING,4,2,23,3,&error_code);
    int ppl = GetNumOfPeople(first);
    ASSERT_TEST(ppl==3);
    DestroyOrder(first);
    return true;
}

static bool testGetOrderEmail(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Order first = CreateOrder("john@walla.co.il",MATERIALS_ENGINEERING,4,2,23,3,&error_code);
   char* email = GetOrderEmail(first);
    ASSERT_TEST(strcmp(email,"john@walla.co.il")==0);
    DestroyOrder(first);
    return true;
}

static bool testGetOrderFaculty(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Order first = CreateOrder("john@walla.co.il",MATERIALS_ENGINEERING,4,2,23,3,&error_code);
    TechnionFaculty faculty = GetOrderFaculty(first);
    ASSERT_TEST(faculty==MATERIALS_ENGINEERING);
    DestroyOrder(first);
    return true;
}

static bool testDecreaseDay() {
    MtmErrorCode error_code=MTM_SUCCESS;
    Order first = CreateOrder("john@walla.co.il",MATERIALS_ENGINEERING,4,1,23,3,&error_code);
    error_code=DecreaseDay(first);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    ASSERT_TEST(GetRemainingDays(first)==0);
    error_code=DecreaseDay(first);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(GetRemainingDays(first)==0);
    DestroyOrder(first);
    return true;
}

int OrderTests (int argv, char** arc){
    RUN_TEST(testOrderCreate);
    RUN_TEST(testOrderDestroy);
    RUN_TEST(testGetID);
    RUN_TEST(testGetRemainingDays);
    RUN_TEST(testGetRemainingHours);
    RUN_TEST(testGetNumOfPeople);
    RUN_TEST(testGetOrderEmail);
    RUN_TEST(testGetOrderFaculty);
    RUN_TEST(testDecreaseDay);
    return 0;
}