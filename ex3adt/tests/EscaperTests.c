//
// Created by orerez on 11/06/2017.
//

//
// Created by orerez on 10/06/2017.
//

//
// Created by orerez on 09/06/2017.
//

#include "test_utilities.h"
#include "../Escaper.h"
#include <string.h>


static bool testValidateParameters(Escaper first_escaper) {
    MtmErrorCode error_code=MTM_SUCCESS;

    error_code=CreateInsertOrderFirst(first_escaper,0,5,7,2);
    ASSERT_TEST(GetNumOrders(first_escaper)==0);
    ASSERT_TEST(GetFirstOrder(first_escaper)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);

    error_code=CreateInsertOrderFirst(first_escaper,1,-1,7,2);
    ASSERT_TEST(GetNumOrders(first_escaper)==0);
    ASSERT_TEST(GetFirstOrder(first_escaper)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);

    error_code=CreateInsertOrderFirst(first_escaper,1,5,-2,2);
    ASSERT_TEST(GetNumOrders(first_escaper)==0);
    ASSERT_TEST(GetFirstOrder(first_escaper)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);

    error_code=CreateInsertOrderFirst(first_escaper,1,5,24,2);
    ASSERT_TEST(GetNumOrders(first_escaper)==0);
    ASSERT_TEST(GetFirstOrder(first_escaper)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);

    error_code=CreateInsertOrderFirst(first_escaper,1,5,7,-3);
    ASSERT_TEST(GetNumOrders(first_escaper)==0);
    ASSERT_TEST(GetFirstOrder(first_escaper)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);

    return true;
}

static bool testEscaperCreate(){
    MtmErrorCode error_code=MTM_SUCCESS;
    ASSERT_TEST(CreateEscaper(NULL,CHEMISTRY,7,&error_code)==NULL);
    ASSERT_TEST(error_code==MTM_NULL_PARAMETER);
    ASSERT_TEST(CreateEscaper("nicolegmail.com",BIOMEDICAL_ENGINEERING,3,&error_code)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateEscaper("SjjFb6n@@",BIOMEDICAL_ENGINEERING,3,&error_code)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateEscaper("nicole@gmail.com",BIOMEDICAL_ENGINEERING,0,&error_code)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(CreateEscaper("nicole@gmail.com",BIOMEDICAL_ENGINEERING,11,&error_code)==NULL);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);

    Escaper first_Escaper = CreateEscaper("@Nebso123.gov",BIOMEDICAL_ENGINEERING,6,&error_code);
    ASSERT_TEST(first_Escaper != NULL);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    DestroyEscaper(first_Escaper);
    return true;
}

static bool testEscaperDestroy(){

    MtmErrorCode error_code=MTM_SUCCESS;
    MtmErrorCode error_code2=MTM_SUCCESS;
    MtmErrorCode error_code3=MTM_SUCCESS;
    Escaper first_Escaper = CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code);
    Escaper second_Escaper =  CreateEscaper("sonya@sony.china",CHEMICAL_ENGINEERING,1,&error_code2);
    Escaper third_Escaper =  CreateEscaper("ginges@khan",COMPUTER_SCIENCE,10,&error_code3);
    DestroyEscaper(first_Escaper);
    DestroyEscaper(second_Escaper);
    DestroyEscaper(third_Escaper);
    return true;
}

static bool testCreateInsertOrder(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Escaper first_escaper = CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code);
    testValidateParameters(first_escaper);

    error_code=CreateInsertOrderFirst(first_escaper,4,5,7,2);
    Order order = GetFirstOrder(first_escaper);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    ASSERT_TEST(GetNumOrders(first_escaper)==1);
    ASSERT_TEST(order!=NULL);
    ASSERT_TEST(GetID(order)==4);

    error_code=CreateInsertOrderFirst(first_escaper,1,8,2,5);
    order = GetFirstOrder(first_escaper);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    ASSERT_TEST(GetNumOrders(first_escaper)==2);
    ASSERT_TEST(order!=NULL);
    ASSERT_TEST(GetID(order)==1);

    error_code=CreateInsertOrderAfterCurrent(first_escaper,3,6,5,4);
    order = GetNextOrder(first_escaper);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    ASSERT_TEST(GetNumOrders(first_escaper)==3);
    ASSERT_TEST(order!=NULL);
    ASSERT_TEST(GetID(order)==3);

    error_code=CreateInsertOrderBeforeCurrent(first_escaper,2,3,11,3);
    GetFirstOrder(first_escaper); //restart iterator
    order = GetNextOrder(first_escaper);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    ASSERT_TEST(GetNumOrders(first_escaper)==4);
    ASSERT_TEST(order!=NULL);
    ASSERT_TEST(GetID(order)==2);

    error_code=CreateInsertOrderLast(first_escaper,5,17,23,7);
    GetFirstOrder(first_escaper); //restart iterator
    for (int i = 0; i < 3; ++i) {
        GetNextOrder(first_escaper);
    }
    order = GetNextOrder(first_escaper);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    ASSERT_TEST(GetNumOrders(first_escaper)==5);
    ASSERT_TEST(order!=NULL);
    ASSERT_TEST(GetID(order)==5);
    error_code = DestroyEscaper(first_escaper);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    return true;
}

static bool testRemoveDestroyEscaper(){

    MtmErrorCode error_code1=MTM_SUCCESS;
    MtmErrorCode error_code2=MTM_SUCCESS;
    MtmErrorCode error_code3=MTM_SUCCESS;
    Escaper first_escaper = CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code1);
    error_code1=CreateInsertOrderFirst(first_escaper,1,20,4,4);
    GetFirstOrder(first_escaper);
    error_code2=CreateInsertOrderAfterCurrent(first_escaper,5,32,2,6);
    error_code3=CreateInsertOrderLast(first_escaper,4,12,5,7);
    ASSERT_TEST(error_code1==MTM_SUCCESS && error_code2 ==MTM_SUCCESS && error_code3==MTM_SUCCESS);
    ASSERT_TEST(GetNumOrders(first_escaper)==3);
    GetFirstOrder(first_escaper);
    Order order = GetNextOrder(first_escaper);

    error_code1= RemoveDestroyOrder(first_escaper,order);
    order = GetFirstOrder(first_escaper);
    ASSERT_TEST(error_code1==MTM_SUCCESS);
    ASSERT_TEST(GetNumOrders(first_escaper)==2);
    ASSERT_TEST(GetID(order)==1);
    order = GetNextOrder(first_escaper);
    ASSERT_TEST(GetID(order)==4);

    error_code1= RemoveDestroyOrder(first_escaper,order);
    order = GetFirstOrder(first_escaper);
    ASSERT_TEST(error_code1==MTM_SUCCESS);
    ASSERT_TEST(GetNumOrders(first_escaper)==1);
    ASSERT_TEST(GetID(order)==1);

    error_code1=RemoveDestroyOrder(first_escaper,NULL);
    ASSERT_TEST(error_code1==MTM_NULL_PARAMETER);
    order = CreateOrder("@avi",MEDICINE,7,24,6,9,&error_code2);
    ASSERT_TEST(error_code2==MTM_SUCCESS);
    error_code1=RemoveDestroyOrder(first_escaper,order);
    DestroyOrder(order);
    DestroyEscaper(first_escaper);
    ASSERT_TEST(error_code1==MTM_ID_DOES_NOT_EXIST);

    return true;
}

static bool testClearEscaperOrders(){

    MtmErrorCode error_code1=MTM_SUCCESS;
    Escaper first_escaper = CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code1);
    CreateInsertOrderFirst(first_escaper,1,20,4,4);
    CreateInsertOrderLast(first_escaper,5,32,2,2);
    CreateInsertOrderLast(first_escaper,4,12,5,7);
    ClearEscaperOrders(first_escaper);
    ASSERT_TEST(GetNumOrders(first_escaper)==0);
    ASSERT_TEST(GetFirstOrder(first_escaper)==NULL);
    DestroyEscaper(first_escaper);
    return true;
}

static bool testsSortOrders(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Escaper first_escaper =CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code);
    CreateInsertOrderFirst(first_escaper,7,10,19,3);
    CreateInsertOrderFirst(first_escaper,2,3,6,2);
    CreateInsertOrderFirst(first_escaper,1,7,10,5);
    CreateInsertOrderFirst(first_escaper,8,2,1,4);
    CreateInsertOrderFirst(first_escaper,20,30,22,7);
    error_code=SortOrders(first_escaper);
    ASSERT_TEST(GetID(GetFirstOrder(first_escaper))==1);
    ASSERT_TEST(GetID(GetNextOrder(first_escaper))==2);
    ASSERT_TEST(GetID(GetNextOrder(first_escaper))==7);
    ASSERT_TEST(GetID(GetNextOrder(first_escaper))==8);
    ASSERT_TEST(GetID(GetNextOrder(first_escaper))==20);
    DestroyEscaper(first_escaper);
    return true;
}

static bool testOrderFinders(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Escaper first_escaper =CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code);
    CreateInsertOrderFirst(first_escaper,7,10,19,3);
    CreateInsertOrderFirst(first_escaper,2,3,6,2);
    CreateInsertOrderFirst(first_escaper,1,7,10,5);
    CreateInsertOrderFirst(first_escaper,8,2,1,4);
    CreateInsertOrderFirst(first_escaper,20,30,22,7);

    Order order = GetEscaperOrder(first_escaper,COMPUTER_SCIENCE,7);
    ASSERT_TEST(GetRemainingHours(order)==19);
    ASSERT_TEST(GetRemainingDays(order)==10);
    ASSERT_TEST(GetNumOfPeople(order)==3);
    ASSERT_TEST(EscaperHasOrder(first_escaper,order)==1);
    ASSERT_TEST(EscaperHasOrder(first_escaper,NULL)==-1);
    RemoveDestroyOrder(first_escaper,order);

    ASSERT_TEST(GetEscaperOrder(first_escaper,COMPUTER_SCIENCE,7)==NULL);
    order = CreateOrder("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,7,10,19,3,&error_code);
    ASSERT_TEST(EscaperHasOrder(first_escaper,order)==0);
    DestroyOrder(order);
    Order order2 = CreateOrder("hanzamzim12^7*@wallak",ELECTRICAL_ENGINEERING,7,10,19,3,&error_code);
    ASSERT_TEST(EscaperHasOrder(first_escaper,order2)==0);
    DestroyOrder(order2);
    DestroyEscaper(first_escaper);

    return true;
}


static bool testDecreaseDays(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Escaper first_escaper =CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code);
    CreateInsertOrderFirst(first_escaper,7,10,19,3);
    CreateInsertOrderFirst(first_escaper,2,3,6,2);
    CreateInsertOrderFirst(first_escaper,1,7,10,5);
    CreateInsertOrderFirst(first_escaper,8,1,1,4);
    CreateInsertOrderFirst(first_escaper,20,30,22,7);

    error_code=DecreaseOrdersDay(first_escaper);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    Order order1 = GetEscaperOrder(first_escaper,COMPUTER_SCIENCE,7);
    Order order2 = GetEscaperOrder(first_escaper,COMPUTER_SCIENCE,2);
    Order order3 = GetEscaperOrder(first_escaper,COMPUTER_SCIENCE,1);
    Order order4 = GetEscaperOrder(first_escaper,COMPUTER_SCIENCE,8);
    Order order5 = GetEscaperOrder(first_escaper,COMPUTER_SCIENCE,20);

    ASSERT_TEST(GetRemainingDays(order1)==9);
    ASSERT_TEST(GetRemainingDays(order2)==2);
    ASSERT_TEST(GetRemainingDays(order3)==6);
    ASSERT_TEST(GetRemainingDays(order4)==0);
    ASSERT_TEST(GetRemainingDays(order5)==29);

    error_code=DecreaseOrdersDay(first_escaper);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    ASSERT_TEST(GetRemainingDays(order1)==9);
    ASSERT_TEST(GetRemainingDays(order2)==2);
    ASSERT_TEST(GetRemainingDays(order3)==6);
    ASSERT_TEST(GetRemainingDays(order4)==0);
    ASSERT_TEST(GetRemainingDays(order5)==29);

    return true;
}

static bool testGetEscaperFaculty(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Escaper first_escaper =CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code);
    ASSERT_TEST(GetEscaperFaculty(first_escaper)==COMPUTER_SCIENCE);
    DestroyEscaper(first_escaper);
    return true;
}

static bool testGetEscaperEmail(){
    MtmErrorCode error_code=MTM_SUCCESS;
    Escaper first_escaper =CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code);
    ASSERT_TEST(strcmp(GetEscaperEmail(first_escaper),"hanzamzim12^7*@wallak")==0);
    DestroyEscaper(first_escaper);
    return true;
}

static bool testGetSkillLevel() {
    MtmErrorCode error_code=MTM_SUCCESS;
    Escaper first_escaper =CreateEscaper("hanzamzim12^7*@wallak",COMPUTER_SCIENCE,8,&error_code);
    ASSERT_TEST(GetSkillLevel(first_escaper)==8);
    DestroyEscaper(first_escaper);
    return true;
}




int EscaperTests (int argv, char** arc){
    RUN_TEST(testEscaperCreate);
    RUN_TEST(testEscaperDestroy);
    RUN_TEST(testCreateInsertOrder);
    RUN_TEST(testRemoveDestroyEscaper);
    RUN_TEST(testClearEscaperOrders);
    RUN_TEST(testsSortOrders);
    RUN_TEST(testOrderFinders);
    RUN_TEST(testGetSkillLevel);
    RUN_TEST(testGetEscaperFaculty);
    RUN_TEST(testGetEscaperEmail);
    RUN_TEST(testDecreaseDays);
    return 0;
}