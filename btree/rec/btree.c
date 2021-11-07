/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Binary search tree search.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) {
    return false;
  }
  if (tree->key == key) {
    *value = tree->value;
    return true;
  }
  if (tree->key > key) {
    return bst_search(tree->left, key, value);
  }
  return bst_search(tree->right, key, value);
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (*tree == NULL) {
    bst_node_t *new_ptr = malloc(sizeof(bst_node_t));
    if (new_ptr == NULL) {
      return;
    }
    new_ptr->key = key;
    new_ptr->value = value;
    new_ptr->left = NULL;
    new_ptr->right = NULL;
    *tree = new_ptr;
  }

  if ((*tree)->key == key) {
    (*tree)->value = value;
    return;
  }

  if ((*tree)->key > key) {
    bst_insert(&(*tree)->left, key, value);
    return;
  }
  bst_insert(&(*tree)->right, key, value);
}

/*
 *  Binary search tree replace by rightmost node.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree)->right == NULL) {
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    bst_node_t *tmp = (*tree);
    *tree = (*tree)->left;
    free(tmp);
    return;
  }
  bst_replace_by_rightmost(target, &(*tree)->right);
}

/*
 * Removal of node in a tree.
 *
 * If node with the key is not found, the function does nothing.
 * If node has only one child, the child is promoted to the place of the node.
 * If node has two children, the node is replaced by the rightmost node of the left subtree.
 * The rightmost node does not have to be a leaf.
 * Function deallocates the memory of the removed node.
 *
 * Implement recursively using bst_replace_by_rightmost.
 * 
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (*tree == NULL) {
    return;
  }
  if ((*tree)->key == key) {
    if ((*tree)->left == NULL) {
      bst_node_t *tmp = (*tree);
      *tree = (*tree)->right;
      free(tmp);
      return;
    }
    if ((*tree)->right == NULL) {
      bst_node_t *tmp = (*tree);
      *tree = (*tree)->left;
      free(tmp);
      return;
    }
    bst_replace_by_rightmost(*tree, &(*tree)->left);
    return;
  }
  if ((*tree)->key > key) {
    bst_delete(&(*tree)->left, key);
    return;
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if (*tree != NULL) {
    bst_postorder((*tree)->left);
    bst_postorder((*tree)->right);
    free(*tree);
    *tree = NULL;
  } else {
    return;
  }

}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if (tree != NULL) {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Binary search tree inorder traversal.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if (tree != NULL) {
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if (tree != NULL) {
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
