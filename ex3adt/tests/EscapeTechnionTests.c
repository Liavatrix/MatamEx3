//
// Created by orerez on 12/06/2017.
//


#include "test_utilities.h"
#include "../EscapeTechnion.h"



static bool testInitialiseSystem(){
    System  system = InitiateSystem();
    assert(system!=NULL);
    DestroySystem(system);
    return true;
}

static bool testDestroySystem(){

    System system1= InitiateSystem();
    System system2= InitiateSystem();
    System system3= InitiateSystem();

    DestroySystem(system1);
    DestroySystem(system2);
    DestroySystem(system3);

    return true;
}

static bool testCompanyAdd(){

    MtmErrorCode error_code=MTM_SUCCESS;
    MtmErrorCode error_code2=MTM_SUCCESS;
    MtmErrorCode error_code3=MTM_SUCCESS;
    MtmErrorCode error_code4=MTM_SUCCESS;
    MtmErrorCode error_code5=MTM_SUCCESS;
    MtmErrorCode error_code6=MTM_SUCCESS;

    System first_System = InitiateSystem();
    System second_System =  InitiateSystem();
    System third_System = InitiateSystem();

    error_code=CompanyAdd(first_System,"Liav@gma",COMPUTER_SCIENCE);
    error_code2=CompanyAdd(first_System,"jgnjsd@",ELECTRICAL_ENGINEERING);
    error_code3=CompanyAdd(first_System,"8745@@",ELECTRICAL_ENGINEERING);
    error_code4=CompanyAdd(first_System,"Liav@gma",MEDICINE);
    error_code5=CompanyAdd(second_System,"Liav@gma",COMPUTER_SCIENCE);
    error_code6=CompanyAdd(third_System,"Liav@gma",COMPUTER_SCIENCE);

    ASSERT_TEST(error_code==MTM_SUCCESS);
    ASSERT_TEST(error_code2==MTM_SUCCESS);
    ASSERT_TEST(error_code3==MTM_INVALID_PARAMETER);
    ASSERT_TEST(error_code4==MTM_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(error_code5==MTM_SUCCESS);
    ASSERT_TEST(error_code6==MTM_SUCCESS);



    DestroySystem(first_System);
    DestroySystem(second_System);
    DestroySystem(third_System);
    return true;
}

static bool testCompanyRemove(){
    MtmErrorCode error_code=MTM_SUCCESS;

    System first_System = InitiateSystem();
    System second_System =  InitiateSystem();
    System third_System = InitiateSystem();

    CompanyAdd(first_System,"Liav@gma",COMPUTER_SCIENCE);
    CompanyAdd(first_System,"jgnjsd@",ELECTRICAL_ENGINEERING);
    CompanyAdd(first_System,"@dffdgs",ELECTRICAL_ENGINEERING);
    CompanyAdd(first_System,"34643@67",ELECTRICAL_ENGINEERING);
    CompanyAdd(first_System,"874546@",ELECTRICAL_ENGINEERING);
    CompanyAdd(first_System,"874ifgfg@",ELECTRICAL_ENGINEERING);
    CompanyAdd(first_System,"dfdsa@dsa",ELECTRICAL_ENGINEERING);
    CompanyAdd(first_System,"Liav@gma",MEDICINE);
    CompanyAdd(second_System,"Liav@gma",COMPUTER_SCIENCE);
    CompanyAdd(third_System,"Liav@gma",COMPUTER_SCIENCE);

    int x = GetNumberOfCompanies(first_System);
    for (int i = 0; i < x; ++i) {
        error_code=CompanyRemove(first_System,listGetFirst(GetCompanyList(first_System)));
        ASSERT_TEST(error_code==MTM_SUCCESS);
    }

    DestroySystem(first_System);
    DestroySystem(second_System);
    DestroySystem(third_System);
    return true;
    return true;
}

static bool testRoomAdd(){

    MtmErrorCode error_code=MTM_SUCCESS;
    System system = InitiateSystem();
    CompanyAdd(system,"Liav@gma",COMPUTER_SCIENCE);
    CompanyAdd(system,"jgnjsd@",ELECTRICAL_ENGINEERING);
    CompanyAdd(system,"harab@shtaal",ELECTRICAL_ENGINEERING);
    CompanyAdd(system,"tsu@berbooler",BIOMEDICAL_ENGINEERING);

    error_code=RoomAdd(system,"Liav@gma",3,16,5,2,18,7);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    error_code=RoomAdd(system,"Liav@gma",5,32,3,1,20,2);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    error_code=RoomAdd(system,"jgnjsd@",3,16,5,2,18,7);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    error_code=RoomAdd(system,"harab@shtaal",7,20,5,12,1,4);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    error_code=RoomAdd(system,"jgnjsd@",2,5,7,14,16,8);
    ASSERT_TEST(error_code==MTM_INVALID_PARAMETER);
    error_code=RoomAdd(system,"gangam@style",4,12,3,4,17,4);
    ASSERT_TEST(error_code==MTM_COMPANY_EMAIL_DOES_NOT_EXIST);
    error_code=RoomAdd(system,"harab@shtaal",3,24,7,6,18,10);
    ASSERT_TEST(error_code==MTM_ID_ALREADY_EXIST);
    error_code=RoomAdd(system,"tsu@berbooler",3,8,5,2,18,7);
    ASSERT_TEST(error_code==MTM_SUCCESS);

    DestroySystem(system);

    return true;
}

static bool testRoomRemove(){

    MtmErrorCode error_code=MTM_SUCCESS;
    System system = InitiateSystem();
    CompanyAdd(system,"Liav@gma",COMPUTER_SCIENCE);
    CompanyAdd(system,"jgnjsd@",ELECTRICAL_ENGINEERING);
    CompanyAdd(system,"harab@shtaal",ELECTRICAL_ENGINEERING);
    CompanyAdd(system,"tsu@berbooler",BIOMEDICAL_ENGINEERING);

    RoomAdd(system,"Liav@gma",3,16,5,2,18,7);
    RoomAdd(system,"Liav@gma",5,32,3,1,20,2);
    RoomAdd(system,"jgnjsd@",3,16,5,2,18,7);
    RoomAdd(system,"harab@shtaal",7,20,5,12,1,4);
    RoomAdd(system,"tsu@berbooler",3,8,5,2,18,7);

    Company company = RoomContainer(ELECTRICAL_ENGINEERING,3,GetCompanyList(system));
    Room room = SearchRoom(company,3);
    ASSERT_TEST(room!=NULL);
    error_code=RoomRemove(system,ELECTRICAL_ENGINEERING,3);
    room = SearchRoom(company,3);
    ASSERT_TEST(room==NULL);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    error_code=RoomRemove(system,ELECTRICAL_ENGINEERING,3);
    ASSERT_TEST(error_code==MTM_ID_DOES_NOT_EXIST);
    company = RoomContainer(COMPUTER_SCIENCE,3,GetCompanyList(system));
    room = SearchRoom(company,3);
    ASSERT_TEST(room!=NULL);
    error_code=RoomRemove(system,COMPUTER_SCIENCE,3);
    room = SearchRoom(company,3);
    ASSERT_TEST(room==NULL);
    ASSERT_TEST(error_code==MTM_SUCCESS);
    DestroySystem(system);

    return true;
}

static bool testEscaperAdd(){
    System  system = InitiateSystem();
    assert(system!=NULL);
    MtmErrorCode err=EscaperAdd(system,"liav@gmail",COMPUTER_SCIENCE,4);
    MtmErrorCode err2=EscaperAdd(system,"bob@gmail",COMPUTER_SCIENCE,7);
    MtmErrorCode err3=EscaperAdd(system,"moti@gmail",COMPUTER_SCIENCE,8);
    MtmErrorCode err4=EscaperAdd(system,"shlomo@gmail",COMPUTER_SCIENCE,9);
    MtmErrorCode err5=EscaperAdd(system,"david@gmail",COMPUTER_SCIENCE,1);

    ASSERT_TEST(err==MTM_SUCCESS);
    ASSERT_TEST(err2==MTM_SUCCESS);
    ASSERT_TEST(err3==MTM_SUCCESS);
    ASSERT_TEST(err4==MTM_SUCCESS);
    ASSERT_TEST(err5==MTM_SUCCESS);

    DestroySystem(system);
    return true;
}

static bool testEscaperRemove(){
    System  system = InitiateSystem();
    assert(system!=NULL);
    MtmErrorCode err=EscaperAdd(system,"liav@gmail",COMPUTER_SCIENCE,4);
    MtmErrorCode err2=EscaperAdd(system,"bob@gmail",MEDICINE,7);
    MtmErrorCode err3=EscaperAdd(system,"moti@gmail",CIVIL_ENGINEERING,8);
    MtmErrorCode err4=EscaperAdd(system,"shlomo@gmail",AEROSPACE_ENGINEERING,9);
    MtmErrorCode err5=EscaperAdd(system,"david@gmail",INDUSTRIAL_ENGINEERING_AND_MANAGEMENT,9);
    MtmErrorCode err6=EscaperAdd(system,"bob@gmail",MEDICINE,7);
    MtmErrorCode err7=EscaperAdd(system,"shlomo@gmail",MEDICINE,7);
    MtmErrorCode err8=EscaperAdd(system,"or@gmail",MEDICINE,7);
    MtmErrorCode err9=EscaperAdd(system,"erez@gmail",MEDICINE,7);
    MtmErrorCode err10=EscaperAdd(system,"barak@gmail",MEDICINE,7);

    ASSERT_TEST(err==MTM_SUCCESS);
    ASSERT_TEST(err2==MTM_SUCCESS);
    ASSERT_TEST(err3==MTM_SUCCESS);
    ASSERT_TEST(err4==MTM_SUCCESS);
    ASSERT_TEST(err5==MTM_SUCCESS);
    ASSERT_TEST(err6==MTM_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(err7==MTM_EMAIL_ALREADY_EXISTS);
    ASSERT_TEST(err8==MTM_SUCCESS);
    ASSERT_TEST(err9==MTM_SUCCESS);
    ASSERT_TEST(err10==MTM_SUCCESS);

    err=EscaperRemove(system,"liav@gmail");
    err2=EscaperRemove(system,"bob@gmail");
    err3=EscaperRemove(system,"moti@gmail");
    err4=EscaperRemove(system,"shlomo@gmail");
    err5=EscaperRemove(system,"david@gmail");
    err6=EscaperRemove(system,"bob@gmail");
    err7=EscaperRemove(system,"shlomo@gmail");
    err8=EscaperRemove(system,"or@gmail");
    err9=EscaperRemove(system,"erez@gmail");
    err10=EscaperRemove(system,"barak@gmail");

    ASSERT_TEST(err==MTM_SUCCESS);
    ASSERT_TEST(err2==MTM_SUCCESS);
    ASSERT_TEST(err3==MTM_SUCCESS);
    ASSERT_TEST(err4==MTM_SUCCESS);
    ASSERT_TEST(err5==MTM_SUCCESS);
    ASSERT_TEST(err6==MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(err7==MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    ASSERT_TEST(err8==MTM_SUCCESS);
    ASSERT_TEST(err9==MTM_SUCCESS);
    ASSERT_TEST(err10==MTM_SUCCESS);
    DestroySystem(system);
    return true;
}

static bool testsEscaperOrder(){
    System  system = InitiateSystem();
    assert(system!=NULL);
    CompanyAdd(system,"MARVEL@",PHYSICS);
    MtmErrorCode err=EscaperAdd(system,"liav@gmail",COMPUTER_SCIENCE,4);
    EscaperAdd(system,"dfksdk@",MEDICINE,5);
    RoomAdd(system,"MARVEL@",22,64,3,2,16,4);
    RoomAdd(system,"DC@",13,64,3,2,16,4);
    RoomAdd(system,"MARVEL@",9,40,3,5,8,4); //Time
    err=EscaperOrder(system,"liav@gmail",PHYSICS,22,2,4,2);
    ASSERT_TEST(err==MTM_SUCCESS);
    ASSERT_TEST(setGetFirst(GetEscapersSet(system))!=NULL);
    err=EscaperOrder(system,"dfksdk@",PHYSICS,22,2,4,2);
    ASSERT_TEST(err==MTM_RESERVATION_EXISTS);
    err=EscaperOrder(system,"li@gmail",PHYSICS,22,3,7,4);
    ASSERT_TEST(err==MTM_CLIENT_EMAIL_DOES_NOT_EXIST);
    err=EscaperOrder(system,"liav@gmail",PHYSICS,13,2,4,3);
    ASSERT_TEST(err==MTM_CLIENT_IN_ROOM);
    err=EscaperOrder(system,"liav@gmail",PHYSICS,9,5,2,3);
    ASSERT_TEST(err==MTM_ROOM_NOT_AVAILABLE);
    DestroySystem(system);
    return true;
}

static bool testEscaperRecommend(){
    MtmErrorCode error;
    System  system = InitiateSystem();
    assert(system!=NULL);
    CompanyAdd(system,"MARVEL@",PHYSICS);
    EscaperAdd(system,"liav@gmail",COMPUTER_SCIENCE,4);
    EscaperAdd(system,"dfksdk@",MEDICINE,5);
    RoomAdd(system,"MARVEL@",22,64,3,2,16,4);
    RoomAdd(system,"DC@",13,64,3,2,16,4);
    RoomAdd(system,"MARVEL@",9,40,3,5,16,5); //Time

    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,0,2,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,0,3,6);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,0,5,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,0,6,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperRecommend(system,"liav@gmail",2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"dfksdk@",PHYSICS,22,0,4,5);
    ASSERT_TEST(error==MTM_RESERVATION_EXISTS);
    DestroySystem(system);
    return true;

}

static bool testsReportDay(){
    MtmErrorCode error=MTM_SUCCESS;

    System  system = InitiateSystem();
    assert(system!=NULL);

    error=CompanyAdd(system,"MARVEL@",PHYSICS);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=CompanyAdd(system,"DC@",HUMANITIES_AND_ARTS);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=EscaperAdd(system,"liav@gmail",COMPUTER_SCIENCE,4);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperAdd(system,"or@",MEDICINE,5);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperAdd(system,"Moshe@",HUMANITIES_AND_ARTS,8);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=RoomAdd(system,"MARVEL@",22,64,3,2,16,4);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"MARVEL@",12,48,7,4,20,6);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"MARVEL@",9,40,3,5,16,5);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"DC@",9,28,8,0,23,10);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,0,2,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,1,3,6);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,9,0,5,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,1,6,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"or@",PHYSICS,12,0,5,3);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"or@",PHYSICS,9,15,5,5);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"Moshe@",HUMANITIES_AND_ARTS,9,0,5,9);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=ReportDay(system,stdout);
    ASSERT_TEST(error==MTM_SUCCESS);
    DestroySystem(system);
    return true;
}

static bool testsReportBest(){
    MtmErrorCode error=MTM_SUCCESS;

    System  system = InitiateSystem();
    assert(system!=NULL);

    error=CompanyAdd(system,"MARVEL@",PHYSICS);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=CompanyAdd(system,"DC@",HUMANITIES_AND_ARTS);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=CompanyAdd(system,"MARCHIM@",ELECTRICAL_ENGINEERING);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=CompanyAdd(system,"GROOT@",BIOLOGY);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=CompanyAdd(system,"METABOL@",MATHEMATICS);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=CompanyAdd(system,"Con@Spiracy",COMPUTER_SCIENCE);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=EscaperAdd(system,"liav@gmail",COMPUTER_SCIENCE,4);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperAdd(system,"or@",MEDICINE,5);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperAdd(system,"Moshe@",HUMANITIES_AND_ARTS,8);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=RoomAdd(system,"MARVEL@",22,64,3,2,16,4);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"MARVEL@",12,48,7,4,20,6);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"MARVEL@",9,40,3,5,16,5);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"DC@",9,28,8,0,23,10);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"GROOT@",14,100,2,7,24,3);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"GROOT@",17,200,4,5,18,7);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"METABOL@",5,84,1,3,21,1);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=RoomAdd(system,"Con@Spiracy",30,400,5,2,23,10);
    ASSERT_TEST(error==MTM_SUCCESS);


    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,3,2,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,1,3,6);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,9,0,5,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",PHYSICS,22,3,6,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"or@",PHYSICS,12,0,5,3);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"or@",PHYSICS,9,15,5,5);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"or@",PHYSICS,12,1,5,3);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=EscaperOrder(system,"or@",HUMANITIES_AND_ARTS,9,2,5,5);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"Moshe@",HUMANITIES_AND_ARTS,9,0,5,9);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=EscaperOrder(system,"Moshe@",COMPUTER_SCIENCE,30,1,7,2);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"or@",COMPUTER_SCIENCE,30,1,8,5);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=EscaperOrder(system,"liav@gmail",COMPUTER_SCIENCE,30,0,22,4);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=ReportDay(system,stdout);
    ASSERT_TEST(error==MTM_SUCCESS);
    error=ReportDay(system,stdout);
    ASSERT_TEST(error==MTM_SUCCESS);

    error=ReportBest(system,stdout);
    ASSERT_TEST(error==MTM_SUCCESS);

    DestroySystem(system);
    return true;
}


int TestsEscape (int argv, char** arc){
    RUN_TEST(testInitialiseSystem);
    RUN_TEST(testDestroySystem);
    RUN_TEST(testCompanyAdd);
    RUN_TEST(testCompanyRemove);
    RUN_TEST(testRoomAdd);
    RUN_TEST(testRoomRemove)
    RUN_TEST(testEscaperAdd);
    RUN_TEST(testEscaperRemove);
    RUN_TEST(testsEscaperOrder);
    RUN_TEST(testEscaperRecommend);
    RUN_TEST(testsReportDay);
    RUN_TEST(testsReportBest);
    return 0;
}