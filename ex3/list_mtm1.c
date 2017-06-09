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

void listDestroy(List list){
    if(list==NULL)
        return;
    Node tmp;
    for(int i=0;i<list->size;i++)
    {
        tmp=list->first->next;
        list->free(list->first->data);
        free(list->first);
        list->first=tmp;
    }
    free(list);
}

ListResult listInsertFirst(List list, ListElement element){
    if(list==NULL)
        return  LIST_NULL_ARGUMENT;
    assert(element!=NULL);
    Node tmp = malloc(sizeof(*tmp));
    assert(tmp!=NULL);
    tmp->data=list->copy(element);
    if(tmp->data==NULL)
        return LIST_OUT_OF_MEMORY;
    list->size++;
    tmp->next=list->first;
    list->first=tmp;
    if(list->size==1)
    {
        list->first->next=NULL;
        list->iterator=list->first;
    }

    return LIST_SUCCESS;
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

List listCopy(List list){
    if(list==NULL)
        return NULL;
    List new_copy=listCreate(list->copy,list->free);
    ListResult func_result = listInsertFirst(new_copy,list->first->data);
    if(func_result!=LIST_SUCCESS)
        return NULL;
    Node my_iterator = list->first->next;
    for (int i = 1; i < list->size; ++i) {
        func_result = listInsertAfterCurrent(new_copy,my_iterator->data);
        if(func_result!=LIST_SUCCESS)
            return NULL;
        new_copy->iterator=new_copy->iterator->next;
        my_iterator=my_iterator->next;
    }

    return new_copy;
}

static int get_iterator_position(List list) {
    Node my_iterator = list->first;
    int count=0;
    for (int i = 0; i < list->size; ++i) {
        if(my_iterator==list->iterator)
        {
            return count;
        }
        count++;
        my_iterator=my_iterator->next;
    }

    return -1;
}

static Node min_list_element(List list,int size_of_list, CompareListElements compareElement) {
    Node min = list->first;
    Node iterator = list->first;
    for (int i = 0; i < list->size; ++i) {
        if(compareElement(min->data,iterator->data)) // current element is "smaller"
            min = iterator;
        iterator=iterator->next;
    }

    return min;
}

static ListResult remove_node(List list, Node node) {
    list->iterator=node;
    ListResult func_result=listRemoveCurrent(list);
    if(func_result!=LIST_SUCCESS)
        return func_result;
    list->iterator=list->first;
    return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement) {
    if(list==NULL|| compareElement==NULL)
        return LIST_NULL_ARGUMENT;
    int iterator_place = get_iterator_position(list);
    assert(iterator_place!=-1);
    ListResult func_result;
    Node min=NULL;
    for(int i=0;i<list->size;i++)
    {
       min = min_list_element(list,(list->size)-i,compareElement);
       func_result=remove_node(list,min);
       if(func_result!=LIST_SUCCESS)
           return func_result;
       func_result=listInsertLast(list,min->data);
       if(func_result!=LIST_SUCCESS)
           return func_result;
    }
    list->iterator=list->first;
    for (int i = 0; i <iterator_place ; ++i) {
        list->iterator=list->iterator->next;
    }
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
    if(list==NULL)
        return LIST_NULL_ARGUMENT;
    if(list->iterator==NULL)
        return LIST_INVALID_CURRENT;
    if(list->iterator==list->first)
    {
        list->first=list->iterator->next;
    } else {
        Node my_iterator=list->first;
        for (int i = 0; i <list->size ; ++i) {
            if(my_iterator->next==list->iterator)
            {
                my_iterator->next=list->iterator->next;
                break;
            }
            my_iterator=my_iterator->next;
        }
    }
    list->free(list->iterator->data);
    free(list->iterator);
    list->size--;
    list->iterator=NULL;
    return LIST_SUCCESS;
}

int listGetSize(List list){
    return (list==NULL) ? -1 : list->size;
}

ListElement listGetFirst(List list){
    if(list==NULL)
        return NULL;
    if(list->iterator==NULL)
        return NULL;
    list->iterator=list->first;
    return list->first->data;
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
    return (list->iterator->data);
}

ListResult listInsertAfterCurrent(List list, ListElement element) {
    if(list==NULL)
        return LIST_NULL_ARGUMENT;
    assert(element!=NULL);
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

ListResult listInsertBeforeCurrent(List list, ListElement element){
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
    for(Node i=list->first;i!=NULL;i=i->next){
        if(i->next==list->iterator)
            tmp=i;
    }
    if(tmp!=NULL) {
        tmp->next = new_element;
        new_element->next = list->iterator;
        list->size++;
        return LIST_SUCCESS;
    }

    return LIST_INVALID_CURRENT;
}

ListResult listInsertLast(List list, ListElement element) {
    if(list==NULL)
        return LIST_NULL_ARGUMENT;
    assert(element!=NULL);
    Node original_iterator=list->iterator;
    list->iterator=list->first;
    for (int i =0 ; i < (list->size)-1; ++i) {
        listGetNext(list);
    }
    Node new_element = malloc(sizeof(Node));
    new_element->data=list->copy(element);
    new_element->next=NULL;
    list->iterator->next=new_element;
    list->iterator=original_iterator;
    list->size++;
    return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    if(list==NULL || filterElement==NULL)
        return NULL;
    List filtered_list = listCreate(list->copy,list->free);
    if (filtered_list==NULL)
        return NULL;
    listGetFirst(list);
    ListResult func_result;
    for (int i = 0; i < list->size; ++i) {
        ListElement ptrToCurrent=listGetCurrent(list);
        Node current = malloc(sizeof(current));
        current->data= list->copy(ptrToCurrent);
        if(filterElement(current->data,key))
        {
            if(filtered_list->iterator==NULL)
            {
                func_result = listInsertFirst(filtered_list,current->data);
                if (func_result != LIST_SUCCESS)
                    return NULL;
            } else {
                func_result = listInsertAfterCurrent(filtered_list, current->data);
                if (func_result != LIST_SUCCESS)
                    return NULL;
            }
        }

        listGetNext(list);
        list->free(current->data);
        free(current);
    }

    listGetFirst(list);
    return filtered_list;
}