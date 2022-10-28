/*
 * ======================================================== *
 * Name:       htab.h                                       *
 * Last modif: 10/28/2022                                   *
 * ======================================================== *
 */


// htab.h -- rozhraní knihovny htab (řešení IJC-DU2)
// Licence: žádná (Public domain)

// následující řádky zabrání násobnému vložení:
#ifndef __HTAB_H__
#define __HTAB_H__

#include <string.h>     // size_t
#include <stdbool.h>    // bool
#include "parser.h"

typedef const char * htab_key_t;        // typ klíče


// Struktury
typedef struct htab_item {
    stat_t *statement;
    struct htab_item *next;
} htab_item_t;

typedef struct htab {
    int size;
    int arr_size;
    htab_item_t **arr_ptr;
} htab_t;

// Rozptylovací (hash) funkce (stejná pro všechny tabulky v programu)
// Pokud si v programu definujete stejnou funkci, použije se ta vaše.
size_t htab_hash_function(htab_key_t str);

// Funkce pro práci s tabulkou:
htab_t *htab_init(size_t n);                    // konstruktor tabulky
size_t htab_size(const htab_t * t);             // počet záznamů v tabulce
size_t htab_bucket_count(const htab_t * t);     // velikost pole
// void htab_resize(htab_t *t, size_t newn);       // změna velikosti pole
                                                // (umožňuje rezervaci místa)

stat_t * htab_find(htab_t *t, htab_key_t key);  // hledání
stat_t * htab_lookup_add(htab_t *t, htab_key_t key);

bool htab_erase(htab_t * t, htab_key_t key);    // ruší zadaný záznam

// for_each: projde všechny záznamy a zavolá na ně funkci f
// Pozor: f nesmí měnit klíč .key ani přidávat/rušit položky
void htab_for_each(const htab_t * t, void (*f)(stat_t *data));

void htab_clear(htab_t * t);    // ruší všechny záznamy
void htab_free(htab_t * t);     // destruktor tabulky
#endif // __HTAB_H__
