/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
    // Initializes tree to NULL
    *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    // Save current node
    bst_node_t *node = tree;

    while (node != NULL) {
        // If the key is found, return its value and true
        if (key == node->key) {
            *value = node->key;
            return true;
        }

        // If the key is smaller than the current key continue in left subtree
        // Otherwise continue in the right subtree
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    // Return false if key was not found
    return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    // Create a new node
    bst_node_t *newNode = malloc(sizeof(bst_node_t));
    newNode->key = key;
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;

    // If the tree is empty, insert the new node as root
    if (*tree == NULL) {
        *tree = newNode;
        return;
    }

    bst_node_t *node = *tree;
    while (node != NULL) {
        // If the keys match, replace its value and free the new node
        if (key == node->key) {
            node->value = value;
            free(newNode);
            return;
        }

        // If the key is smaller than the current key continue in left subtree
        // Otherwise continue in the right subtree
        if (key < node->key) {
            if (node->left == NULL) {
                node->left = newNode;
                return;
            }
            node = node->left;
        } else {
            if (node->right == NULL) {
                node->right = newNode;
                return;
            }
            node = node->right;
        }
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    // Checks if the tree is empty
    if (*tree == NULL) {
        return;
    }

    bst_node_t *node = *tree;
    bst_node_t *previous = NULL;
    // Finds the rightmost
    while (node->right != NULL) {
        previous = node;
        node = node->right;
    }

    // Sets key and value of the rightmost to the target
    target->key = node->key;
    target->value = node->value;

    if (previous == NULL) {
        *tree = node->left;
    } else {
        previous->right = node->left;
    }

    free(node);
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    // Checks if pointer to tree is valid
    if (*tree == NULL) {
        return;
    }

    bst_node_t *node = *tree;
    bst_node_t *previous = NULL;

    while (node != NULL && node->key != key) {
        previous = node;

        // If the key is smaller than the current key continue in left subtree
        // Otherwise continue in the right subtree
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    // If the key was not found, return
    if (node == NULL) {
        return;
    }

    if (node->left == NULL && node->right == NULL) {
        // If the node is root, set the tree to NULL
        if (previous == NULL) {
            *tree = NULL;
        } else if (previous->left == node) {
            // If the node is left child, set the left child to NULL
            previous->left = NULL;
        } else if (previous->right == node) {
            // If the node is right child, set the right child to NULL
            previous->right = NULL;
        }

        free(node);
    } else if (node->left != NULL && node->right != NULL) {
        // Replace the node by the rightmost node of the left subtree
        bst_replace_by_rightmost(node, &node->left);
    } else {
        // If the node has only one child, replace it by the child
        bst_node_t *child;
        if (node->left != NULL) {
            child = node->left;
        } else {
            child = node->right;
        }

        if (previous == NULL) {
            *tree = child;
        } else if (previous->left == node) {
            previous->left = child;
        } else if (previous->right == node) {
            previous->right = child;
        }

        free(node);
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    // Checks if pointer to tree is valid
    if (*tree == NULL) {
        return;
    }

    // Initializes stack
    stack_bst_t stack;
    stack_bst_init(&stack);

    // Pushes the root to stack
    stack_bst_push(&stack, *tree);

    while (!stack_bst_empty(&stack)) {
        // Pops the node from the stack
        bst_node_t *node = stack_bst_top(&stack);
        stack_bst_pop(&stack);

        // Pushes the children of the node to the stack
        if (node->left != NULL) {
            stack_bst_push(&stack, node->left);
        }
        if (node->right != NULL) {
            stack_bst_push(&stack, node->right);
        }

        // Frees the current element
        free(node);
    }

    *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
    while (tree != NULL) {
        // Adds the node to items
        bst_add_node_to_items(tree, items);

        // Pushes the right child to the stack
        if (tree->right != NULL) {
            stack_bst_push(to_visit, tree->right);
        }

        // Continues in the left subtree
        tree = tree->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    // Checks if pointers to tree and items are valid
    if (tree == NULL || items == NULL) {
        return;
    }

    // Initializes stack
    stack_bst_t stack;
    stack_bst_init(&stack);

    // Pushes the root to stack
    stack_bst_push(&stack, tree);

    while (!stack_bst_empty(&stack)) {
        // Pops node from the stack
        bst_node_t *node = stack_bst_top(&stack);
        stack_bst_pop(&stack);

        // Adds the node to items
        bst_add_node_to_items(node, items);

        // Pushes the right child to the stack
        if (node->right != NULL) {
            stack_bst_push(&stack, node->right);
        }

        // Continues in the left subtree
        bst_leftmost_preorder(node->left, &stack, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL) {
        // Pushes the node to the stack
        stack_bst_push(to_visit, tree);

        // Continues in the left subtree
        tree = tree->left;
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    // Checks if pointers to tree and items are valid
    if (tree == NULL || items == NULL) {
        return;
    }

    // Initializes stack
    stack_bst_t stack;
    stack_bst_init(&stack);

    while (tree != NULL || !stack_bst_empty(&stack)) {
        // Go to the leftmost
        bst_leftmost_inorder(tree, &stack);

        // Pops node from the stack
        bst_node_t *node = stack_bst_top(&stack);
        stack_bst_pop(&stack);

        // Adds the node to items
        bst_add_node_to_items(node, items);

        // Continues in the right subtree
        tree = node->right;
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
    while (tree != NULL) {
        // Pushes the node to the stack
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);

        // Continues in the left subtree
        tree = tree->left;
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    // Checks if pointers to tree and items are valid
    if (tree == NULL || items == NULL) {
        return;
    }

    // Initializes stacks
    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bool_t first_visit;
    stack_bool_init(&first_visit);

    bst_leftmost_postorder(tree, &stack, &first_visit);

    while (!stack_bst_empty(&stack)) {
        // Pops node from the stack
        bst_node_t *node = stack_bst_top(&stack);
        bool first = stack_bool_top(&first_visit);
        stack_bool_pop(&first_visit);

        if (first) {
            // Pushes the node to the stack
            stack_bool_push(&first_visit, false);

            // Continues in the left subtree
            bst_leftmost_postorder(node->right, &stack, &first_visit);
        } else {
            stack_bst_pop(&stack);

            // Adds the node to items
            bst_add_node_to_items(node, items);
        }
    }
}
