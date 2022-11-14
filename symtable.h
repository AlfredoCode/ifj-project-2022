/*
 * ======================================================== *
 * Name:       symtable.h                                   *
 * Last modif: 10/28/2022                                   *
 * ======================================================== *
 */

/* 
 * Used from IJC-DU2
 * License: Public domain
 * Edited to fit current usecase
 */

#ifndef __HTAB_H__
#define __HTAB_H__

#include <string.h>     // size_t
#include <stdbool.h>    // bool
#include "parser.h"

typedef const char * htab_key_t;


// Structs
typedef struct htab_item {
    stat_t *statement;
    struct htab_item *next;
} htab_item_t;

typedef struct htab {
    int size;
    int arr_size;
    htab_item_t **arr_ptr;
} htab_t;

/**
 * @brief Hash function
 * @param str string to hash
 * @return output hash
 *
 * Hashes the input str so it can be used as htab index
 */
size_t htab_hash_function(htab_key_t str);

/**
 * @brief Htab initialization
 * @param n size of htab
 * @return ptr to htab
 *
 * Initializes a new htab with size n.
 */
htab_t *htab_init(size_t n);

/**
 * @brief Return amount of items in htab
 * @param t htab
 * @return Item amount
 *
 * Returns amount of item in htab, fancier htab->size.
 */
size_t htab_size(const htab_t * t);

/**
 * @brief Returns size of array
 * @param t htab
 * @return htab array size
 *
 * Returns size of htab array, fancier htab->array_size.
 */
size_t htab_bucket_count(const htab_t * t);

/**
 * @brief Resizes htab
 * @param t htab
 * @param newn new size
 * 
 * Resizes t to size newn, re-hashing every item.
 * Passed pointer will point to the new htab
 * Unused due to segfaults :(
 */
void htab_resize(htab_t *t, size_t newn);

/**
 * @brief Finds item in htab
 * @param t htab
 * @param key key to search
 * @return found item or null
 */
stat_t * htab_find(htab_t *t, htab_key_t key);

/**
 * @brief Adds item to htab or err
 * @param t htab
 * @param key key to add
 * @return added item or null
 *
 * Tries to add key to htab.
 * Returns ERR and error message if key already in htab.
 */
stat_t * htab_lookup_add(htab_t *t, htab_key_t key);

/**
 * @brief Removes item from htab
 * @param t htab
 * @param key key to remove
 * @return bool if erased
 *
 * Returns true on sucessful removal.
 * False if key not found
 */
bool htab_erase(htab_t * t, htab_key_t key);

/**
 * @brief Calls function on each item
 * @param t htab
 * @param f function
 * 
 * Calls function f on each item in htab.
 * Function cannot remove or add items.
 */
void htab_for_each(const htab_t * t, void (*f)(stat_t *data));

/**
 * @brief Clears htab of items
 * @param t htab
 */
void htab_clear(htab_t * t);

/**
 * @brief Frees htab and items
 * @param t htab
 */
void htab_free(htab_t * t);

#endif // __HTAB_H__
