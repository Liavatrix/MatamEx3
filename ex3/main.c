#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list_mtm1.h"
#include <string.h>

ListElement copyString(ListElement str);
void freeString(ListElement str);

int main() {
    List new_list=listCreate(copyString,freeString);
    ListElement element1,element2,element3;
    char* str=malloc(100);
    char* str2=malloc(100);
    char* str3=malloc(100);
    printf("Enter 2 strings\n");
    scanf(" %s",str);
    element1=str;
    scanf(" %s",str2);
    element2=str2;
    scanf(" %s",str3);
    element3=str3;
    listInsertFirst(new_list,element1);
    listInsertBeforeCurrent(new_list,element2);
    listInsertAfterCurrent(new_list,element3);
    free(str);
    free(str2);
    free(str3);
    listDestroy(new_list);
    return 0;
}

ListElement copyString(ListElement str) {
    assert(str);
    char* copy = malloc(strlen(str) + 1);
    return copy ? strcpy(copy, str) : NULL;
}

void freeString(ListElement str){
    assert(str);
    free(str);
}