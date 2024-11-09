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

// Implementace strdup, abych nemusel importovat dalsi knihovny
char *strdup(const char *s) {
    char *dup = malloc(strlen(s) + 1);  // Alokuje místo pro kopii řetězce
    if (dup != NULL) {
        strcpy(dup, s);  // Zkopíruje obsah původního řetězce
    }
    return dup;
}

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
    if (key == NULL){
        return 0;
    }  
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
    if (table == NULL){
        return;
    }
    for (int i = 0; i < MAX_HT_SIZE; i++) {
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
    if (table == NULL || key == NULL){
        return NULL;
    }
    int index = get_hash(key) % MAX_HT_SIZE;
    ht_item_t *current = (*table)[index];
    
    while (current != NULL) { // Porovnávání jednotlivých klíčů
        if (strcmp(current->key, key) == 0) { // Klíče se shodují
            return current;
        }
        current = current->next;
    }
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
    if (table == NULL || key == NULL){
        return;
    }
    ht_item_t *found = ht_search(table, key);
    if (found != NULL) {
        // Prvek již existuje, nahraďí se hodnota
        found->value = value;
    } else {
        // Prvek neexistuje, vytvoříme nový a vložíme ho na začátek seznamu
        int index = get_hash(key) % MAX_HT_SIZE;
        ht_item_t *new = (ht_item_t *)malloc(sizeof(ht_item_t)); // Vytvoření nového prvku
        if (new == NULL){
          return;
        }
        new->key = strdup(key);
        new->value = value;
        new->next = (*table)[index];
        (*table)[index] = new;
    }
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
    if (table == NULL || key == NULL){
        return NULL;
    }
    ht_item_t *found = ht_search(table, key);
    return (found != NULL) ? &found->value : NULL;
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
    if (table == NULL || key == NULL){
        return;
    }
    int index = get_hash(key) % MAX_HT_SIZE;
    ht_item_t *current = (*table)[index];
    ht_item_t *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) { // Nalezení prvku
            if (previous == NULL) { // Napojení zbytku
                (*table)[index] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current->key); // Uvolnění paměti
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    if (table == NULL){
        return;
    }
    for (int i = 0; i < MAX_HT_SIZE; i++) { // Promazání tabulky
        ht_item_t *current = (*table)[i];
        while (current != NULL) { // Postupné promazání všech prvků
            ht_item_t *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
        (*table)[i] = NULL;
    }
}
