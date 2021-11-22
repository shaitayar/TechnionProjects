#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "amount_set_str.h"

typedef struct Node_t {

    char* string;
    double amount;
    struct Node_t* next;

} *Node;

struct AmountSet_t {

    Node first;
    Node iter;

};


AmountSet asCreate()
{
    AmountSet new_set=malloc(sizeof(*new_set));
    if(!new_set)
    {
        return NULL;
    }

    // necessery?
    new_set->first=NULL;
    new_set->iter=NULL;

    return new_set;
}

void asDestroy(AmountSet set)
{
    if(!set)
    {
        return;
    }

    asClear(set);

    free(set);
}

Node copyNode(Node node)
{
    Node copy=malloc(sizeof(*copy));
    if(!copy)
    {
        return NULL;
    }

    copy->amount=node->amount;
    copy->next=node->next;

    copy->string=malloc(sizeof(*(node->string)));
    if(!copy->string)
    {
        free(copy);
        return NULL;
    }

    strcpy(copy->string,node->string);

    return copy;
}

AmountSet asCopy(AmountSet set)
{
    AmountSet copy_set=asCreate();
    if(!copy_set)
    {
        return NULL;
    }

    Node iterator=set->first;
    Node iterator_copy=copyNode(iterator);
    copy_set->first=iterator_copy;
    
    
    // Node* copy_node;
    while(iterator->next)
    {
        
        iterator_copy->next=copyNode(iterator->next);
        if(!iterator_copy)
        {
            free(copy_set);
            return NULL;
        }

        if(iterator->next==set->iter)
        {
            copy_set->iter=iterator_copy;
        }

        iterator_copy=iterator_copy->next;
        iterator=iterator->next;
    }

    return copy_set;
}

int asGetSize(AmountSet set)
{
    if(!set)
    {
        return -1;
    }

    int counter;
    Node iterator=set->first;
    while(iterator)
    {
        counter+=1;
        iterator=iterator->next;
    }

    return counter;
}

bool asContains(AmountSet set, const char* element)
{
    if(!set)
    {
        return NULL;
    }

    Node iterator=set->first;
    while(iterator)
    {
        if(strcmp(iterator->string,element)==0)
        {
            return true;
        }
        iterator=iterator->next;
    }

    return false;
}

AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount)
{
    if(!set || !element || !outAmount)
    {
        return AS_NULL_ARGUMENT;
    }

    Node iterator=set->first;
    while(iterator)
    {
        if(strcmp(iterator->string,element)==0)
        {
            *outAmount=iterator->amount;
            return AS_SUCCESS;
        }
        iterator=iterator->next;
    }

    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asRegister(AmountSet set, const char* element)
{
    if(!set || !element)
    {
        return AS_NULL_ARGUMENT;
    }

    if(!set->first)
    {
        Node new_node=malloc(sizeof(*new_node));
        if(!new_node)
        {
            return AS_OUT_OF_MEMORY;
        }
        new_node->amount=0;
        new_node->next=set->iter;
        set->first=new_node;
        new_node->string=malloc(sizeof(element));
        if(!new_node->string)
        {
            free(new_node);
            return AS_OUT_OF_MEMORY;
        }
        strcpy(new_node->string,element);

        return AS_SUCCESS;
    }
    
    set->iter=set->first;
    if(strcmp(set->iter->string,element)>0)
    {
        Node new_node=malloc(sizeof(*new_node));
        if(!new_node)
        {
            return AS_OUT_OF_MEMORY;
        }
        new_node->amount=0;
        new_node->next=set->iter;
        set->first=new_node;
        new_node->string=malloc(sizeof(element));
        if(!new_node->string)
        {
            free(new_node);
            return AS_OUT_OF_MEMORY;
        }

        return AS_SUCCESS;
    } 
    else if(strcmp(set->iter->string,element)==0)
    {
        return AS_ITEM_ALREADY_EXISTS;
    }

    Node new_node=malloc(sizeof(*new_node));
    if(!new_node)
    {
        return AS_OUT_OF_MEMORY;
    }
    while(set->iter->next)
    {
        if(strcmp(set->iter->next->string,element)==0)
        {
            free(new_node);
            return AS_ITEM_ALREADY_EXISTS;
        } else if(strcmp(set->iter->next->string,element)>0)
        {
            new_node->string=malloc(sizeof(*new_node));
            if(!new_node->string)
            {
                free(new_node);
                return AS_OUT_OF_MEMORY;
            }

            strcpy(new_node->string,element);
            new_node->amount=0;
            new_node->next=set->iter->next;
            set->iter->next=new_node;
            return AS_SUCCESS;
        }
        set->iter=set->iter->next;
    }

    new_node->string=malloc(sizeof(*new_node));
    if(!new_node->string)
    {
        free(new_node);
        return AS_OUT_OF_MEMORY;
    }

    strcpy(new_node->string,element);
    new_node->amount=0;
    new_node->next=set->iter->next;
    set->iter->next=new_node;
    return AS_SUCCESS;

}

AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount)
{
    if(!set || !element)
    {
        return AS_NULL_ARGUMENT;
    }

    Node iterator=set->first;
    while(iterator)
    {
        if(strcmp(iterator->string,element)==0)
        {
            if(amount<0 && (iterator->amount+amount<0))
            {
                return AS_INSUFFICIENT_AMOUNT;
            }
            iterator->amount+=amount;
            return AS_SUCCESS;
        }
        iterator=iterator->next;
    }

    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asDelete(AmountSet set, const char* element)
{
    if(!set || !element)
    {
        return AS_NULL_ARGUMENT;
    }

    Node iterator=set->first;
    if(strcmp(iterator->string,element)==0)
    {
        Node to_delete=iterator;
        set->first=iterator->next;

        free(to_delete->string);
        free(to_delete);
        return AS_SUCCESS;
    }
    while(iterator->next)
    {
        if(strcmp(iterator->next->string,element)==0)
        {
            Node to_delete=iterator->next;
            iterator->next=iterator->next->next;
            free(to_delete->string);
            free(to_delete);
            return AS_SUCCESS;
        }
        iterator=iterator->next;
    }

    return AS_ITEM_DOES_NOT_EXIST;
}

AmountSetResult asClear(AmountSet set)
{
    if(!set)
    {
        return AS_NULL_ARGUMENT;
    }

    while(set->first)
    {
        Node to_delete=set->first;
        set->first=set->first->next;
        free(to_delete->string);
        free(to_delete);
    }

    return AS_SUCCESS;
}

char* asGetFirst(AmountSet set)
{
    if(!set || !set->first)
    {
        return NULL;
    }

    set->iter=set->first;
    return set->iter->string;
}

char* asGetNext(AmountSet set)
{
    if(!set || !set->iter->next)
    {
        return NULL;
    }

    set->iter=set->iter->next;
    return set->iter->string;
}