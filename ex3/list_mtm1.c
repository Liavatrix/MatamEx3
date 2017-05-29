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
        list->iterator=list->first;
    return LIST_SUCCESS;
}

/*List listCopy(List list){
    if(list==NULL)
        return NULL;
    List new_copy=listCreate(list->copy,list->free);

}*/

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
    Node new_element=malloc(sizeof(Node));
    if(new_element==NULL) // Why isn't this check being referred in errors?
        return LIST_OUT_OF_MEMORY;
    new_element->data=list->copy(element);
    if(new_element->data==NULL)
        return LIST_OUT_OF_MEMORY;
    Node tmp = current_element->next;
    current_element->next=new_element;
    new_element->next=tmp;
    list->size++;
    return LIST_SUCCESS;
}