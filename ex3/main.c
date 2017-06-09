//
// Created by orerez on 07/06/2017.
//

#include "list_mtm1.h"
#include "tests/test_utilities.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static ListElement copyString(ListElement str){
    assert(str);
    char* copy = malloc(strlen(str)+1);
    return copy != NULL ? strcpy(copy, str) : NULL;
}

static void freeString(ListElement str){
    free((char*)str);
}

static bool isLongerThan(ListElement element,ListFilterKey number) {
    char* string = element;
    int num = *(int *)number;
    return (strlen(string) >num) ;
}

static int compareStrings(ListElement element1, ListElement element2)
{
    return strcmp((const char*)element1,(const char*)element2);
}

static bool testListStringCreate() {
    ASSERT_TEST(listCreate(NULL,NULL) == NULL);
    ASSERT_TEST(listCreate(copyString,NULL) == NULL);
    ASSERT_TEST(listCreate(NULL,freeString) == NULL);

    return true;
}

static bool testListStringFilter() {

    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    int key = 2;
    ASSERT_TEST(listFilter(NULL,isLongerThan,&key) == NULL);
    ASSERT_TEST(listFilter(list,NULL,&key) == NULL);
    List filtered = listFilter(list,isLongerThan, &key);
//    ASSERT_TEST(listGetSize(filtered) == 1);
//    ASSERT_TEST(strcmp(listGetFirst(filtered),a[3])==0);
    listDestroy(list);
    listDestroy(filtered);
    return true;
}
static bool testListStringCopy() {

    ASSERT_TEST(listCopy(NULL) == NULL);
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }

    List list2 = listCopy(list);
    listGetFirst(list);
    listGetFirst(list2);
    ASSERT_TEST(listGetSize(list)==listGetSize(list2));
    listGetFirst(list);
    listGetFirst(list2);
    for (int j = 0; j < listGetSize(list); ++j) {
        ASSERT_TEST(strcmp(listGetCurrent(list2),listGetCurrent(list))==0);
        listGetNext(list);
        listGetNext(list2);
    }
    listGetFirst(list);
    listGetFirst(list2);
    listDestroy(list);
    //listDestroy(list2);
    return true;
}

static bool testListStringGetSize() {

    ASSERT_TEST(listGetSize(NULL) == -1);
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    List list = listCreate(copyString,freeString);
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(listGetSize(list) == 5);
    listDestroy(list);
    return true;
}

static bool testListStringGetFirst() {
    ASSERT_TEST(listGetFirst(NULL) == NULL);
    List list=listCreate(copyString,freeString);
    ASSERT_TEST(listGetFirst(list)==NULL);
    char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
    for (int i=0;i <5; ++i){
        listInsertFirst(list,a[i]);
    }
    ASSERT_TEST(strcmp(listGetFirst(list),a[4])==0);
    listDestroy(list);
    return true;
}

static bool testListStringGetNext() {
    ASSERT_TEST(listGetNext(NULL) == NULL);
    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(listGetNext(list) == NULL);
    for (int i = 0; i < 5; ++i) {
        listInsertFirst(list, a[i]);
    }
    listGetFirst(list);
    ASSERT_TEST(strcmp(listGetNext(list),a[3])==0);
    for (int j = 0; j < 4; ++j) {
        listGetNext(list);
    }
    ASSERT_TEST(listGetNext(list) == NULL);
    listDestroy(list);
    return true;
}

static bool testListStringGetCurrent() {
    ASSERT_TEST(listGetCurrent(NULL) == NULL);
    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    List list = listCreate(copyString, freeString);
    for (int i = 0; i < 5; ++i) {
        listInsertFirst(list, a[i]);
    }
    listGetFirst(list);
    listGetNext(list);
    ASSERT_TEST(strcmp(listGetCurrent(list),a[3])==0);
    listDestroy(list);
    return true;
}

static bool testListStringInsertFirst() {

    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(listInsertFirst(NULL,a[0])==LIST_NULL_ARGUMENT);
    for (int i = 0; i < 5; ++i) {
        listInsertFirst(list, a[i]);
    }
    listGetFirst(list);
    for (int j = 0; j < 5; ++j) {
        ListElement element=listGetCurrent(list);
        int n =strcmp(element,a[4-j]);
        ASSERT_TEST(n==0);
        listGetNext(list);
    }
    listDestroy(list);
    return true;
}

static bool testListStringInsertLast() {
    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(listInsertLast(NULL,a[0])==LIST_NULL_ARGUMENT);
    listInsertFirst(list, a[0]);
    for (int i = 1; i < 5; ++i) {
        listInsertLast(list, a[i]);
    }
    listGetFirst(list);
    for (int j = 0; j < 5; ++j) {
        ASSERT_TEST(strcmp(listGetCurrent(list),a[j])==0);
        listGetNext(list);
    }
    listDestroy(list);
    return true;

}

static bool testListStringInsertBeforeCurrent() {
    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(listInsertBeforeCurrent(NULL,a[0])==LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(list,NULL)==LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertBeforeCurrent(list,a[0])==LIST_INVALID_CURRENT);
    listInsertFirst(list, a[0]);
    for (int i = 1; i < 4; ++i) {
        listInsertLast(list, a[i]);
        list=listGetNext(list);
    }
    listInsertBeforeCurrent(list,a[4]);
    listGetFirst(list);
    for (int j = 0; j < 4; ++j) {
        list = listGetNext(list);
    }
    listDestroy(list);
    ASSERT_TEST(strcmp(listGetCurrent(list),a[3])==0);
    return true;
}

static bool testListStringInsertAfterCurrent() {
    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(listInsertAfterCurrent(NULL,a[0])==LIST_NULL_ARGUMENT);
    ASSERT_TEST(listInsertAfterCurrent(list,a[0])==LIST_INVALID_CURRENT);
    listInsertFirst(list, a[0]);
    for (int i = 1; i < 5; ++i) {
        listInsertAfterCurrent(list, a[i]);
        listGetNext(list);
    }
    listGetFirst(list);
    for (int j = 0; j < 5; ++j) {
        ASSERT_TEST(strcmp(listGetCurrent(list),a[j])==0);
        listGetNext(list);
    }
    listDestroy(list);
    return true;

}

static bool testListStringRemoveCurrent() {
    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(listRemoveCurrent(NULL)==LIST_NULL_ARGUMENT);
    listInsertFirst(list, a[0]);
    for (int i = 1; i < 5; ++i) {
        listInsertLast(list, a[i]);
    }
    listGetFirst(list);
    listRemoveCurrent(list);
    listGetFirst(list);
    for (int j = 0; j < listGetSize(list); ++j) {
        ASSERT_TEST(strcmp(listGetCurrent(list),a[j+1])==0);
        list = listGetNext(list);
    }

    listGetFirst(list);
    list=listGetNext(list);
    listRemoveCurrent(list);
    listGetFirst(list);
    list=listGetNext(list);
    ASSERT_TEST(strcmp(listGetCurrent(list),a[2])==0);
    listDestroy(list);
    return true;
}

static bool testListStringSort() {
    char *a[6] = {"aaa", "bbb", "NI", "hello mister fish", "aaa","I"};
    List list = listCreate(copyString, freeString);
    CompareListElements compareFunc = &compareStrings;
    ASSERT_TEST(listSort(NULL,compareStrings)==LIST_NULL_ARGUMENT);
    ASSERT_TEST(listSort(list,NULL)==LIST_NULL_ARGUMENT);
    listInsertFirst(list, a[0]);
    for (int i = 1; i < 6; ++i) {
        listInsertLast(list, a[i]);
    }
    listGetFirst(list);
    listSort(list,compareFunc);
    for (int j = 0; j < listGetSize(list)-1; ++j) {
        ASSERT_TEST(strcmp(listGetCurrent(list),listGetNext(listGetCurrent(list)))<=0);
    }
    listDestroy(list);
    return true;
}

static bool testListStringClear() {

    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    List list = listCreate(copyString, freeString);
    ASSERT_TEST(listClear(NULL)==LIST_NULL_ARGUMENT);
    listInsertFirst(list, a[0]);
    for (int i = 1; i < 5; ++i) {
        listInsertLast(list, a[i]);
    }
    listClear(list);
    ASSERT_TEST((listGetCurrent(list)==NULL && listGetSize(list)==0));
    listDestroy(list);
    return true;
}

static bool testListStringDestroy() {
    char *a[5] = {"aaa", "bbb", "NI", "hello mister fish", "I"};
    char *b[7] = {"escape", "bbb","igNor ant", "bbb", "BBB", "FG%2^df5DF$", ""};
    List list1 =listCreate(copyString, freeString);
    List list2 = listCreate(copyString, freeString);
    for (int i = 0; i < 5; ++i) {
        listInsertFirst(list1,a[i]);
    }
    for (int i = 0; i < 7; ++i) {
        listInsertFirst(list2,b[i]);
    }

    listDestroy(list1);
    listDestroy(list2);
    return true;
}

int main (int argv, char** arc){
    RUN_TEST(testListStringCreate);
    RUN_TEST(testListStringInsertFirst);
    RUN_TEST(testListStringDestroy);
    RUN_TEST(testListStringGetFirst);
    RUN_TEST(testListStringGetNext);
    RUN_TEST(testListStringGetCurrent);
    RUN_TEST(testListStringGetSize);
    RUN_TEST(testListStringInsertLast);
    RUN_TEST(testListStringInsertAfterCurrent);
    RUN_TEST(testListStringInsertBeforeCurrent);
    RUN_TEST(testListStringRemoveCurrent);
    RUN_TEST(testListStringClear);
    RUN_TEST(testListStringCopy);
    RUN_TEST(testListStringSort);
    RUN_TEST(testListStringFilter);

    return 0;
}




