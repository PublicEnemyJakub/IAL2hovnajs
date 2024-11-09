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


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je 
 * uložen ve stromu (klíč vždy lowercase).
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
// POMOCNE FUNKCE
// Zjištění, zda je znak písmenem
int my_isalpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

// Převedení znaku na malé písmeno
char my_tolower(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch + ('a' - 'A');
    }
    return ch;
}
// Pomocná funkce pro vložení nebo aktualizaci počtu výskytů do BST
void bst_update(bst_node_t **tree, char key) {
    bst_node_content_t value;
    value.type = INTEGER;  // Podle zadaní
    value.value = malloc(sizeof(int));  // Alokace paměti pro hodnotu
    if  (value.value == NULL) { // Kontrola alokace
        return;
    }
    *((int*)value.value) = 1;  // Inicializujeme hodnotu na 1

    bst_node_t *tree_current = *tree;
    bst_node_t *parent = NULL;
    
    while (tree_current != NULL) { // Prohledávání stromu
        parent = tree_current;
        if (key < tree_current->key) {
            tree_current = tree_current->left;
        } else if (key > tree_current->key) {
            tree_current = tree_current->right;
        } else {
            *((int*)tree_current->content.value) += 1; // Pokud se našel uzel, tak se zvětší hodnota
            free(value.value);
            return;
        }
    }

    // Pokud uzel nebyl nalezen, musí se vytvořit
    bst_node_t *new_node = (bst_node_t*)malloc(sizeof(bst_node_t));
     if  (new_node == NULL) { // Kontrola alokace
        return;
    }
    new_node->key = key;
    new_node->content = value;
    new_node->left = new_node->right = NULL;

    // Připojíme nového uzlu
    if (parent == NULL) {
        *tree = new_node;
    } else if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

// Hlavní funkce pro počítání frekvence znaků
void letter_count(bst_node_t **letter_frequency_tree, char *input) {
    bst_init(letter_frequency_tree);
    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        if (my_isalpha(c)) { // Pokud se jedná o pismeno, tak se zavolá zmenšení
            c = my_tolower(c);
        } else if (c == ' ') {
            // Mezera je valid a nijak ji neměním
            c = ' ';
        } else {
            // Ostatní znaky měním na podtržítko
            c = '_';
        }
        bst_update(letter_frequency_tree, c); // Aktualizujeme strom pomocnou funkcí
    }
}