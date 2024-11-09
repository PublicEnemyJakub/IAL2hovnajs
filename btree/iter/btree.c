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
// Autor: Jakub Smička - xsmickj00
/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{   
    *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
    if (tree == NULL) { // Pro jistotu kontrola NULLu ;)
        return false;
    }
    while (tree != NULL) { // Procházení stromu
        if (key == tree->key) {
            *value = &tree->content; // Nalezeno
            return true;
        } else if (key < tree->key) { // Posunutí se dál podle hodnoty klíče
            tree = tree->left;
        } else {
            tree = tree->right;
        }
    }
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
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
    if (tree == NULL) { // Pro jistotu kontrola NULLu ;)
        return;
    }
    while (*tree != NULL) {
        if (key == (*tree)->key) {
            if ((*tree)->content.value != NULL) {
                free((*tree)->content.value); // Uvolnění hodnoty, pokud existuje
            }
            (*tree)->content = value; // Nahrazení hodnoty novou
            return;
        } 
    else if (key < (*tree)->key) {
            tree = &(*tree)->left;
        } else {
            tree = &(*tree)->right;
        }
    }
    // Vytvoříme nový uzel, pokud jsme ho nenašli
    bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (new_node != NULL) { // Provedeme pokud prošla alokace 
        new_node->key = key;
        new_node->content = value;
        new_node->left = NULL;
        new_node->right = NULL;
        *tree = new_node;
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
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
    if (tree == NULL || target == NULL) { // Pro jistotu kontrola NULLu ;)
        return;
    }

    // Najdeme nejpravější uzel
    while ((*tree)->right != NULL) {
        tree = &(*tree)->right;
    }

    // Uložíme hodnotu uzlu, který bude nahrazen
    bst_node_t *to_delete = *tree; // Temp uzel

    // Zkopírujeme hodnoty z nejpravějšího uzlu do cílového uzlu
    target->key = (*tree)->key;
    if (target->content.value != NULL) {
        free(target->content.value); // Uvolnění původní hodnoty pokud existuje
    }
    target->content = (*tree)->content;

    *tree = (*tree)->left; // Dědění levého potomka
    free(to_delete);
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
void bst_delete(bst_node_t **tree, char key)
{
     if (tree == NULL) { // Pro jistotu kontrola NULLu ;)
        return;
    }
    while (*tree != NULL) {
        if (key == (*tree)->key) {
            bst_node_t *to_delete = *tree;

            // Pokud má uzel pouze jeden nebo žádný potomek, převezmeme příslušný podstrom
            if ((*tree)->left == NULL) {
                *tree = (*tree)->right;
            } else if ((*tree)->right == NULL) {
                *tree = (*tree)->left;
            } else {
                // Uzel má oba potomky
                bst_replace_by_rightmost(*tree, &(*tree)->left);
                return; // Po nahrazení uzlu se nic víc nedělá
            }
            // Uvolníme hodnotu v content, pokud existuje
            if (to_delete->content.value != NULL) {
                free(to_delete->content.value);
            }
            // Po nahrazení nebo odstranění uzlu uvolníme paměť samotného uzlu
            free(to_delete);
            return;
        } else if (key < (*tree)->key) { // Procházení stzrromu
            tree = &(*tree)->left;
        } else {
            tree = &(*tree)->right;
        }
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

// Inspirováno z přednášky
void bst_dispose(bst_node_t **tree) {
    stack_bst_t node_stack;
    stack_bst_init(&node_stack); // Inicializace zásobníku

    bst_node_t *ptr = *tree;

    do {
        if (ptr == NULL) {
            // Pokud je aktuální uzel NULL, vezmeme uzel ze zásobníku
            if (!stack_bst_empty(&node_stack)) {
                ptr = stack_bst_pop(&node_stack);
            }
        } else {
            // Pokud má pravého potomka, uložíme jej do zásobníku
            if (ptr->right != NULL) {
                stack_bst_push(&node_stack, ptr->right);
            }
            // Přesuneme se doleva
            bst_node_t *aux = ptr;
            ptr = ptr->left;

            // Pokud je to nutné, uvolníme dynamicky alokovanou paměť v content
            if (aux->content.value != NULL) {
                free(aux->content.value);  // Uvolníme hodnotu v content, pokud je alokována
            }

            // Uvolníme samotný uzel
            free(aux);
        }
    } while (ptr != NULL || !stack_bst_empty(&node_stack));

    *tree = NULL; // Nastavíme ukazatel na strom na NULL
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
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
    while (tree != NULL) { // Procházení stromem
        bst_add_node_to_items(tree, items);
        stack_bst_push(to_visit, tree);
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
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
    stack_bst_t node_stack;
    stack_bst_init(&node_stack);

    bst_leftmost_preorder(tree, &node_stack, items); // Využití funkce

    while (!stack_bst_empty(&node_stack)) { // Procházení zásobníku
        bst_node_t *node = stack_bst_pop(&node_stack);
        bst_leftmost_preorder(node->right, &node_stack, items);
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
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
    while (tree != NULL) { // Nejdříve nejlevější od shora
        stack_bst_push(to_visit, tree);
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
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
    stack_bst_t node_stack;
    stack_bst_init(&node_stack);

    bst_leftmost_inorder(tree, &node_stack); // Využití pomocné funkce

    while (!stack_bst_empty(&node_stack)) {
        bst_node_t *node = stack_bst_pop(&node_stack);
        bst_add_node_to_items(node, items);
        bst_leftmost_inorder(node->right, &node_stack);
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
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
    while (tree != NULL) { 
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
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
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
    stack_bst_t node_stack;
    stack_bool_t visit_stack;
    stack_bst_init(&node_stack);
    stack_bool_init(&visit_stack);

    bst_leftmost_postorder(tree, &node_stack, &visit_stack); // Vyutití pomocné funkce

    while (!stack_bst_empty(&node_stack)) {
        bst_node_t *node = stack_bst_pop(&node_stack);
        bool first_visit = stack_bool_pop(&visit_stack);

        if (first_visit) {
            stack_bst_push(&node_stack, node);
            stack_bool_push(&visit_stack, false);
            bst_leftmost_postorder(node->right, &node_stack, &visit_stack);
        } else {
            bst_add_node_to_items(node, items);
        }
    }
}
