/*
 * =================================================== *
 * Name:       symtable.c                              *
 * Authors:    xsafar27                                * 
 * Last modif: 12/07/2022                              *
 * =================================================== *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "symtable.h"
#include "error.h"

#define AVG_MIN 3
#define AVG_MAX 5


int insertSymtable(htab_t *symtable, htab_list *list){
    
    if(list->lastElement != NULL){
        list->lastElement->next = symtable;
        list->lastElement = symtable;
    }
    else{
        list->lastElement = symtable;  
        list->activeElement = symtable;
    }
    symtable->next = NULL;
    return SUCCESS_ERR;
}

void initSymList(htab_list *list){
    list->lastElement = NULL;
    list->activeElement = NULL;
    
}

size_t htab_hash_function(const char *str)
{
    uint32_t hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

htab_t *htab_init(size_t n)
{
    htab_t *htab = (htab_t*) malloc(sizeof(htab_t));
    htab_item_t **items = (htab_item_t**) malloc(n * sizeof(htab_item_t));

    if(!items) errHandler(INTERNAL_ERR, "Failed to malloc items in htab_init.c\n");

    if(!htab) errHandler(INTERNAL_ERR, "Failed to malloc htab in htab_init.c\n");

    htab->arr_ptr = items;

    for(size_t i = 0; i < n; i++){
        htab->arr_ptr[i] = NULL;    // easier foreach if all is default null
    }

    htab->arr_size = n;
    htab->size = 0;

    return htab;
}

size_t htab_size(const htab_t *t)
{
    return t->size;
}

size_t htab_bucket_count(const htab_t *t)
{
    return t->arr_size;
}


stat_t *htab_find(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);

    htab_item_t *currentPtr = t->arr_ptr[index];
    while(currentPtr){
        if(!strcmp(currentPtr->statement->name, key)){
            return currentPtr->statement;
        }
        currentPtr = currentPtr->next;
    }
    return NULL;
}

stat_t *htab_lookup_add(htab_t *t, htab_key_t name)
{
    size_t index = htab_hash_function(name) % t->arr_size;

    htab_item_t *ptr = t->arr_ptr[index];
    
    while(ptr){
        if(!strcmp(ptr->statement->name, name)){
            return ptr->statement;
        }
        ptr = ptr->next;
    }
    
    ptr = malloc(sizeof(htab_item_t));
    if (!ptr) errHandler(INTERNAL_ERR, "Failed to malloc ptr in htab_lookup_add\n");
    
    stat_t *statement = malloc(sizeof(stat_t));

    statement->name = malloc(strlen(name) + 1);
    strcpy(statement->name, name);

    ptr->statement = statement;
    ptr->next = t->arr_ptr[index];
    t->arr_ptr[index] = ptr;

    t->size++;

    return ptr->statement;
}

bool htab_erase(htab_t *t, htab_key_t name)
{
    size_t index = htab_hash_function(name) % t->arr_size;
    htab_item_t *item = t->arr_ptr[index];
    htab_item_t *prev = NULL;

    while (item) {
        if (!strcmp(item->statement->name, name)){
            
            if (!prev) {
                t->arr_ptr[index] = item->next;
            } else {
                prev->next = item->next;
            }
            
            free(item->statement->name);
            free(item->statement);
            free(item);

            t->size--;
            return true;
        }
        prev = item;
        item = item->next;
    }
    return false;
}

void htab_for_each(const htab_t *t, void (*f)(stat_t *data))
{
    for(int i = 0; i < t->arr_size; i++){
        if (t->arr_ptr[i] == NULL){
            continue;
        }

        htab_item_t *current_ptr = t->arr_ptr[i];

        while(current_ptr){
            f(current_ptr->statement);
            current_ptr = current_ptr->next;
        }
    }
}

void htab_clear(htab_t *t)
{
    for(int i = 0; i < t->arr_size; i++){
        if (t->arr_ptr[i] == NULL){
            continue;
        }

        htab_item_t *current_ptr = t->arr_ptr[i];
        htab_item_t *to_remove;

        while(current_ptr){
            to_remove = current_ptr;
            current_ptr = current_ptr->next;
            free((void* const)to_remove->statement->name);
            free(to_remove->statement);
            free(to_remove);
        }

        t->arr_ptr[i] = NULL;
    }
    t->size = 0;
    free(t->arr_ptr);
}

void htab_free(htab_t *t)
{
    htab_clear(t);
    free(t);
}
