/*
 * =================================================== *
 * Name:       htab.c                                  *
 * Authors:    xsafar27                                * 
 * Last modif: 10/17/2022                              *
 * =================================================== *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "htab.h"

#define AVG_MIN 3
#define AVG_MAX 5

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
    htab_item **items = (htab_item**) malloc(n * sizeof(htab_item));

    if(items == NULL){
        fprintf(stderr, "Failed to malloc items in htab_init.c");
        return NULL;
    }

    if(htab == NULL){
        fprintf(stderr, "Failed to malloc htab in htab_init.c");
        return NULL;
    }

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

void htab_resize(htab_t *t, size_t newn)
{
    htab_t *newTab = htab_init(newn);
    if(newTab == NULL){
        return;
    }
    
    size_t htabSize_old = htab_bucket_count(t);
    htab_item *currentPtr;

    for(size_t i = 0; i < htabSize_old; i++){
        currentPtr = t->arr_ptr[i];
        while(currentPtr){
            htab_pair_t *check = htab_lookup_add(newTab, currentPtr->pair->key);
            if(check == NULL){
                return;
            }
            currentPtr = currentPtr->next;
        }
    }

    t = newTab;
}

htab_pair_t *htab_find(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);

    htab_item *currentPtr = t->arr_ptr[index];
    while(currentPtr){
        if(!strcmp(currentPtr->pair->key, key)){
            return currentPtr->pair;
        }
        currentPtr = currentPtr->next;
    }
    return NULL;
}

htab_pair_t *htab_lookup_add(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);

    htab_item *ptr = t->arr_ptr[index];
    
    while(ptr){
        if(!strcmp(ptr->pair->key, key)){
            return ptr->pair;
        }
        ptr = ptr->next;
    }
    
    if(ptr == NULL){
        ptr = t->arr_ptr[index];

        htab_pair_t *pair = (htab_pair_t*) malloc (sizeof(htab_pair_t));
        htab_item *item = (htab_item*) malloc (sizeof(htab_item));
        pair->key = malloc(sizeof(htab_key_t));

        if(pair == NULL){
            fprintf(stderr, "Failed to malloc pair in htab_lookup_add.c");
            return NULL;
        }

        if(item == NULL){
            fprintf(stderr, "Failed to malloc item in htab_lookup_add.c");
            return NULL;
        }

        if(pair->key == NULL){
            fprintf(stderr, "Failed to malloc pair->key in htab_lookup_add.c");
            return NULL;
        }

        memcpy((void * restrict)pair->key, key, strlen(key));
        pair->value = 0;
        item->pair = pair;

        if (ptr == NULL){
            t->arr_ptr[index] = item;
        } else {
            while(ptr->next){
                ptr = ptr->next;
            }
            ptr->next = item;
        }

        t->size++;
        ptr = item;
    }

    int sum = 0;
    int amn = 0;
    htab_item *currentPtr = NULL;

    for(int i = 0; i < t->arr_size; i++){
        if (t->arr_ptr[i] == NULL) { continue; }
        amn++;
        currentPtr=t->arr_ptr[i];
        while(currentPtr){
            sum++;
            currentPtr = currentPtr->next;
        }
    }
    if (sum/amn > AVG_MAX){
        // htab_resize(t, t->arr_size * 2);
    }

    return ptr->pair;
}

bool htab_erase(htab_t *t, htab_key_t key)
{
    size_t index = htab_hash_function(key) % htab_bucket_count(t);
    
    htab_item *currentPtr = t->arr_ptr[index];
    htab_item *secondaryPtr;
    while(currentPtr){
        if(currentPtr->pair->key == key){
            secondaryPtr = currentPtr;
            currentPtr = currentPtr->next;
            secondaryPtr->next = currentPtr->next;
            free(currentPtr->pair);
            free(currentPtr);

            int tmp = 0;
            int sum = 0;
            htab_item *currentPtr;

            for(int i = 0; i < t->arr_size; i++){
                if(t->arr_ptr[i] == NULL){
                    continue;
                }
                currentPtr = t->arr_ptr[i];
                while(currentPtr){
                    sum++;
                    currentPtr = currentPtr->next;
                }
                tmp++;
            }

            if (sum/tmp < AVG_MIN){
                // htab_resize(t, t->arr_size/2);
                return htab_find(t, key);
            }

            return true;
        }
        currentPtr = currentPtr->next;
    }
    return false;
}

void htab_for_each(const htab_t *t, void (*f)(htab_pair_t *data))
{
    for(int i = 0; i < t->arr_size; i++){
        if (t->arr_ptr[i] == NULL){
            continue;
        }

        htab_item *current_ptr = t->arr_ptr[i];

        while(current_ptr){
            f(current_ptr->pair);
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

        htab_item *current_ptr = t->arr_ptr[i];
        htab_item *to_remove;

        while(current_ptr){
            to_remove = current_ptr;
            current_ptr = current_ptr->next;
            free((void* const)to_remove->pair->key);
            free(to_remove->pair);
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
