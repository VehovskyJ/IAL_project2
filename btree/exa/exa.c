/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný 
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

// Convert letter to lowercase
char _toLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

// Categorize character and return either letter, space or underscore
char _categorizeLetter(char c) {
    c = _toLower(c);
    if ((c >= 'a' && c <= 'z') || c == ' ') {
        return c;
    }
    return '_';
}

size_t _len(const char *string) {
    size_t length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);

    for (int i = 0; i < _len(input); ++i) {
        char c = _categorizeLetter(input[i]);

        // Search if character is already in the tree
        bst_node_content_t* result = NULL;
        if (bst_search(*tree, c, &result)) {
            // Increment the character count in the node
            (*((int*)result->value))++;
            continue;
        }

        // Character is not in the tree, insert new node
        bst_node_content_t item = {
                .type = INTEGER,
                .value = malloc(sizeof(int))
        };
        *((int*)(item.value)) = 1;
        bst_insert(tree, c, item);
    }
}
