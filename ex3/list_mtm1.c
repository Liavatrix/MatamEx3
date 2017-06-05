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

List listCopy(List list){
    if(list==NULL)
        return NULL;
    List new_copy=listCreate(list->copy,list->free);
    ListResult func_result = listInsertFirst(new_copy,list->first);
    if(func_result!=LIST_SUCCESS)
        return NULL;
    Node my_iterator = list->first->next;
    for (int i = 1; i < list->size; ++i) {
        func_result = listInsertAfterCurrent(new_copy,my_iterator);
        if(func_result!=LIST_SUCCESS)
            return NULL;
        new_copy->iterator=new_copy->iterator->next;
        my_iterator=my_iterator->next;
    }

    return new_copy;
}



static int get_iterator_position(List list)
{
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

static Node max_list_element(List list, CompareListElements compareElement)
{
    Node max = list->first;
    Node iterator = list->first;
    for (int i = 0; i < list->size; ++i) {
        if(compareElement(iterator->data,max->data)) // current element is "bigger"
            max = iterator;
        iterator=iterator->next;
    }

    return max;
}

static void point_iterator_to_node(List list, Node node)
{
    list->iterator=list->first;
    for (int i = 0; i < list->size; ++i) {
        if(list->iterator==node)
        {
            return;
        }
    }
}

ListResult remove_node(List list, Node node)
{
    point_iterator_to_node(list,node);
    ListResult func_result=listRemoveCurrent(list);
    if(func_result!=LIST_SUCCESS)
        return func_result;
    list->iterator=list->first;
}

ListResult listSort(List list, CompareListElements compareElement)
{
    if(list==NULL|| compareElement==NULL)
        return LIST_NULL_ARGUMENT;
    List sorted_list = listCreate(list->copy,list->free);
    if(sorted_list==NULL)
        return LIST_OUT_OF_MEMORY;
    int iterator_place = get_iterator_position(list);
    if(iterator_place==-1) // This check is not in h file
        return LIST_INVALID_CURRENT;

   Node max = max_list_element(list,compareElement);
   ListResult func_result = listInsertFirst(sorted_list,max->data);
    if(func_result!=LIST_SUCCESS)
        return func_result;
    func_result = remove_node(list,max);
    if(func_result!=LIST_SUCCESS)
        return func_result;
   while(list->size!=0)
   {
       max = max_list_element(list,compareElement);
       func_result=listInsertAfterCurrent(sorted_list,max->data);
       if(func_result!=LIST_SUCCESS)
           return func_result;
       func_result=remove_node(list,max);
       if(func_result!=LIST_SUCCESS)
           return func_result;
   }

    for (int i = 0; i <iterator_place ; ++i) {
        sorted_list->iterator=sorted_list->iterator->next;
    }
    list = sorted_list;
    return LIST_SUCCESS;

}


ListResult listRemoveCurrent(List list)
{
    if(list==NULL)
        return LIST_NULL_ARGUMENT;
    if(list->iterator==NULL)
        return LIST_INVALID_CURRENT;
    Node tmp=NULL;
    if(list->iterator==list->first)
    {
        tmp=list->first->next;
        list->first->next=NULL;
        list->free(list->first);
        list->first=tmp;
    } else {

        Node my_iterator=list->first;
        for (int i = 0; i <list->size ; ++i) {
            if(my_iterator->next==list->iterator)
            {
                tmp = list->iterator->next;
                my_iterator->next=NULL;
                list->free(list->iterator);
                my_iterator->next=tmp;
                break;
            }
            my_iterator=my_iterator->next;
        }
    }
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

ListResult listInsertAfterCurrent(List list, ListElement element)
{
    if(list==NULL)
        return LIST_NULL_ARGUMENT;
    Node current_element = list->iterator;
    if(current_element==NULL)
        return LIST_INVALID_CURRENT;
    Node new_element=malloc(sizeof(*new_element));
    if(new_element==NULL) // Why isn't this check being referred in errors?
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
    if(list->size==0)
        listInsertFirst(list,element);
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