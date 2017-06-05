#include <stdlib.h>
#include <assert.h>
#include "list_mtm1.h"
#include <stdio.h>

typedef struct Snode{
    ListElement data;
    struct Snode* next;
} *Node;

typedef struct list_t{
    Node iterator;
    int size;
    Node first;
    CopyListElement copy;
    FreeListElement free;
}list_var;

List listCreate(CopyListElement copyElement, FreeListElement freeElement){
    if(freeElement==NULL||copyElement==NULL)
        return NULL;
    List new_list=malloc(sizeof(list_var));
    if(new_list==NULL)
        return NULL;
    new_list->free=freeElement;
    new_list->copy=copyElement;
    new_list->iterator=NULL;
    new_list->size=0;
    return new_list;
}

//should be written
List listCopy(List list){
    if(list==NULL)
        return NULL;
    List new_list=listCreate(list->copy,list->free);
    if(new_list==NULL)
        return NULL;
    LIST_FOREACH(ListElement,i,list){

    }
    return new_list;
}

void listDestroy(List list){
    if(list==NULL)
        return;
    Node tmp;
    for(int i=0;i<list->size;i++)
    {
        tmp=list->first->next;
        if(list->first->data!=NULL)
            list->free(list->first->data);
        free(list->first);
        list->first=tmp;
    }
    free(list);
}

ListResult listClear(List list){
    if(list==NULL)
        return LIST_NULL_ARGUMENT;
    LIST_FOREACH(ListElement,i,list){
        list->free(i);
        list->size--;
    }
    return LIST_SUCCESS;
}

ListResult listInsertFirst(List list, ListElement element){
    if(list==NULL)
        return  LIST_NULL_ARGUMENT;
    Node tmp = malloc(sizeof(*tmp));
    assert(tmp!=NULL);
    tmp->data=list->copy(element);
    if(tmp->data==NULL)
        return LIST_OUT_OF_MEMORY;
    list->size++;
    tmp->next=list->first;
    list->first=tmp;
    return LIST_SUCCESS;
}

int listGetSize(List list){
    return (list==NULL) ? -1 : list->size;
}

ListElement listGetFirst(List list){
    if(list==NULL)
        return NULL;
    list->iterator=list->first;
    if(list->first->data!=NULL)
        return list->first->data;
    return NULL;
}

ListElement listGetNext(List list){
    if(list==NULL)
        return NULL;
    if (list->iterator==NULL)  //if already pointing to last element
        return NULL;
    list->iterator=list->iterator->next;
    if (list->iterator==NULL)   //if the next element is NULL
        return NULL;
    return list->iterator->data;
}

ListElement listGetCurrent(List list){
    if(list==NULL)
        return NULL;
    if(list->iterator==NULL)
        return NULL;
    assert(list->iterator->data!=NULL);
    return (list->iterator->data);
}

ListResult listInsertAfterCurrent(List list, ListElement element) {
    if(list==NULL)
        return LIST_NULL_ARGUMENT;
    if(listGetCurrent(list)==NULL)
        return LIST_INVALID_CURRENT;
    Node new_element=malloc(sizeof(Node));
    if(new_element==NULL)
        return LIST_OUT_OF_MEMORY;
    new_element->data=list->copy(element);
    if(new_element->data==NULL)
        return LIST_OUT_OF_MEMORY;
    Node tmp = list->iterator->next;
    list->iterator->next=new_element;
    new_element->next=tmp;
    list->size++;
    return LIST_SUCCESS;
}

/*ListResult listInsertBeforeCurrent(List list, ListElement element){
    if(list==NULL)
        return LIST_NULL_ARGUMENT;
    if(listGetCurrent(list)==NULL)
        return LIST_INVALID_CURRENT;
    Node new_element=malloc(sizeof(Node));
    if(new_element==NULL)
        return LIST_OUT_OF_MEMORY;
    new_element->data=list->copy(element);
    if(new_element->data==NULL)
        return LIST_OUT_OF_MEMORY;
    Node tmp=NULL;
    for(Node i=list->first;i;i=i->next){
        if(i->next==list->iterator)
            tmp=i;
    }
    if(tmp!=NULL) {
        tmp->next = new_element;
        new_element->next = list->iterator;
        list->size++;
        return LIST_SUCCESS;
    }
}*/