/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree) {
    // Initialize tree to NULL
    *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value) {
    // Check if pointer to tree is valid
    if (tree == NULL) {
        return false;
    }

    // If the current key matches the desired key, return true and its value
    if (key == tree->key) {
        *value = &(tree->content);
        return true;
    }

    // If the key is smaller than the current key continue in left subtree
    // Otherwise continue in the right subtree
    if (key < tree->key) {
        return bst_search(tree->left, key, value);
    }
    return bst_search(tree->right, key, value);
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value) {
    // If the current tree is NULL, insert new node
    if (*tree == NULL) {
        // Allocate memory for new node
        *tree = malloc(sizeof(bst_node_t));
        if (*tree == NULL) {
            return;
        }

        // Set node properties
        (*tree)->key = key;
        (*tree)->content = value;
        (*tree)->right = NULL;
        (*tree)->left = NULL;

        return;
    }

    // If key already exists, update its value
    if (key == (*tree)->key) {
        // Free the original content
        if ((*tree)->content.value != NULL) {
            free((*tree)->content.value);
            (*tree)->content.value = NULL;
        }

        (*tree)->content = value;
        return;
    }

    // If the key is smaller than the current key continue in left subtree
    // Otherwise continue in the right subtree
    if (key < (*tree)->key) {
        bst_insert(&(*tree)->left, key, value);
    } else {
        bst_insert(&(*tree)->right, key, value);
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (tree == NULL) {
        return false;
    }

    // Free the content of the target node
    if (target->content.value != NULL) {
        free(target->content.value);
        target->content.value = NULL;
    }

    if ((*tree)->right == NULL) {
        // Set key and value of the rightmost to the target
        target->key = (*tree)->key;
        target->content = (*tree)->content;

        // Free the right node, but preserve the left one
        bst_node_t *toFree = *tree;
        *tree = (*tree)->left;
        free(toFree);
        return;
    }

    // Find the rightmost
    bst_replace_by_rightmost(target, &(*tree)->right);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    // Check if the pointer to tree is valid
    if (*tree == NULL) {
        return;
    }

    if (key == (*tree)->key) {
        if ((*tree)->content.value != NULL) {
            free((*tree)->content.value);
            (*tree)->content.value = NULL;
        }

        // Free the node
        if ((*tree)->left == NULL && (*tree)->right == NULL) {
            // Tree has no subtrees
            // Free the element
            free(*tree);
            *tree = NULL;
        } else if ((*tree)->left == NULL) {
            // Tree has only right subtree
            // Replaces the current element with the right one
            bst_node_t *toFree = *tree;
            *tree = (*tree)->right;
            free(toFree);
        } else if ((*tree)->right == NULL) {
            // Tree has only left subtree
            // Replaces the current element with the left one
            bst_node_t *toFree = *tree;
            *tree = (*tree)->left;
            free(toFree);
        } else {
            // Tree has both subtrees
            // Replaces the current element with the rightmost
            bst_replace_by_rightmost(*tree, &(*tree)->left);
        }
        return;
    }

    // If the key is smaller than the current key continue in left subtree
    // Otherwise continue in the right subtree
    if (key < (*tree)->key) {
        bst_delete(&(*tree)->left, key);
    } else {
        bst_delete(&(*tree)->right, key);
    }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    // Check if pointer to tree is valid
    if (*tree == NULL) {
        return;
    }

    // Continue disposing the left and right subtrees
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);

    // Free the node content as well
    if ((*tree)->content.value != NULL) {
        free((*tree)->content.value);
        (*tree)->content.value = NULL;
    }

    // Free the current element
    free(*tree);
    *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    // Check if pointers to tree and items are valid
    if (tree == NULL || items == NULL) {
        return;
    }

    // Add current node to items
    bst_add_node_to_items(tree, items);

    // Continue on left and right subtree
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    // Check if pointer to tree and items are valid
    if (tree == NULL || items == NULL) {
        return;
    }

    // Continue on left subtree
    bst_inorder(tree->left, items);

    // Add current node to items
    bst_add_node_to_items(tree, items);

    // Continue on right subtree
    bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    // Check if pointers to tree and items are valid
    if (tree == NULL || items == NULL) {
        return;
    }

    // Continue on left and right subtree
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);

    // Add current node to items
    bst_add_node_to_items(tree, items);
}
