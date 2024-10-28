/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
// Funkce muze generovat stjeny vysledek pro ruzne vstupy a neni moc dobra
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
    // Checks if pointer to table is valid
    if (table == NULL) {
        return;
    }

    // Iterates through the items and sets them to NULL
    for (int i = 0; i < HT_SIZE; ++i) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    // Checks if pointers to table and key are valid
    if (table == NULL || key == NULL) {
        return NULL;
    }

    // Find initial address of an item with the key
    ht_item_t *item = (*table)[get_hash(key)];


    while (item != NULL) {
        // If the key is found, returns the item
        if (strcmp(item->key, key) == 0) {
            return item;
        }

        // Moves to the next item
        item = item->next;
    }

    // Key not found, return NULL
    return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    // Checks if pointers to table and key are valid
    if (table == NULL || key == NULL) {
        return;
    }

    // Checks if the key exists, if so, updates its value
    ht_item_t *res = ht_search(table, key);
    if (res != NULL) {
        res->value = value;
        return;
    }

    // Creates new item and sets its properties
    ht_item_t *item = malloc(sizeof(ht_item_t));
    item->key = key;
    item->value = value;
    item->next = (*table)[get_hash(key)];

    // Inserts the new item into the hash table
    (*table)[get_hash(key)] = item;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    // Checks if the pointers to table and key are valid
    if (table == NULL || key == NULL) {
        return NULL;
    }

    // Searches for the key, if found, returns pointer to the value
    ht_item_t *item = ht_search(table, key);
    if (item != NULL) {
        return &(item->value);
    }

    // Returns NULL if key not found
    return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    // Checks if pointers to table and key are valid
    if (table == NULL || key == NULL) {
        return;
    }

    // Find initial address of an item with the key
    ht_item_t *item = (*table)[get_hash(key)];
    ht_item_t *prevItem;

    while (item != NULL) {
        // If the key is found, break the loop
        if (strcmp(item->key, key) == 0) {
            break;
        }

        // Sets previous item to the current item and increments the item position
        prevItem = item;
        item = item->next;
    }

    // Sets the next item of the previous item to point to the item after the deleted one
    prevItem->next = item->next;
    // Deallocates memory from teh ite,
    free(item);
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    // Checks if pointer to table is valid
    if (table == NULL) {
        return;
    }

    // Iterate over all the hash table
    for (int i = 0; i < HT_SIZE; ++i) {
        ht_item_t *item = (*table)[i];
        while (item != NULL) {
            ht_item_t *delItem = item;
            item = item->next;

            // Deallocates memory form the item
            free(delItem);
        }

        // Sets the value to NULL
        (*table)[i] = NULL;
    }
}
/*
       _
       \`*-.
        )  _`-.
       .  : `. .
       : _   '  \
       ; *` _.   `*-._
       `-.-'          `-.
         ;       `       `.
         :.       .        \
         . \  .   :   .-'   .
         '  `+.;  ;  '      :
         :  '  |    ;       ;-.
         ; '   : :`-:     _.`* ;
[bug] .*' /  .*' ; .*`- +'  `*'
      `*-*   `*-*  `*-*'
*/