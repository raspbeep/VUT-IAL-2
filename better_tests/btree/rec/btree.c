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
 * Nájdenie uzlu v strome.
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
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
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
  
  bst_node_t *right = NULL;

  
  if ((*tree)->right == NULL) {
    right = *tree;
    target->key = right->key;
    target->value = right->value;

    target->left = right->left;
    if (target->left != NULL) {
      target->left = right->left;
    }
    free(right);
    return;

  } else if ((*tree)->right->right == NULL) {
    right = (*tree)->right;
    target->key = right->key;
    target->value = right->value;

    (*tree)->right = NULL;

    if (right->left != NULL) {
      (*tree)->right = right->left;
    }
    free(right);
    return;
  }

  bst_replace_by_rightmost(target, &(*tree)->right);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if (*tree == NULL) {
    return;
  }

  bst_node_t *delete = NULL;

  // ak ma aktualny uzol vacsi kluc, pozeram sa na jeho lavy podstrom
  if ((*tree)->key > key) {
    // existuje lavy podstrom
    if ((*tree)->left != NULL) {
      // lavy podstrom ma hladany kluc
      if ((*tree)->left->key == key) {
        delete = (*tree)->left;
        // node ma lavy podstrom
        if (delete->left != NULL) {
          // node ma aj pravy podstrom
          if (delete->right != NULL) {
            bst_replace_by_rightmost(delete, &(delete->left));
            return;
          }
          // node ma iba lavy podstrom
          else {
            (*tree)->left = delete->left;
          }
        } 
        // node ma iba pravy podstrom
        else if (delete->right != NULL) {
            (*tree)->left = delete->right;
        }
        // node nema ani lavy ani pravy podstrom
        else {
          (*tree)->left = NULL;
        }
      free(delete);
      return;
      }
    }
    bst_delete(&(*tree)->left, key);
    return;
  }

  // koren ma mensi kluc, pozeram sa vpravo
  if ((*tree)->key < key) {
    // koren ma pravy podstrom
    if ((*tree)->right != NULL) {
      // pravy podstrom ma hladany kluc
      if ((*tree)->right->key == key) {
        // ulozenie pointra na pravy podstrom, ktory chcem vymazat
        delete = (*tree)->right;
        // node ma lavy podstrom
        if (delete->left != NULL) {
          // node ma aj pravy podstrom
          if (delete->right != NULL) {
            bst_replace_by_rightmost(delete, &(delete->left));
            return;
          }
          // node ma iba lavy podstrom
          else {
            (*tree)->right = delete->left;
          }
        }
        // node ma iba pravy podstrom
        else if (delete->right != NULL) {
          (*tree)->right = delete->right;
        }
        // node nema ziadny podstrom
        else {
          (*tree)->right = NULL;
        }
      free(delete);
      return;
      }
    }
    bst_delete(&(*tree)->right, key);
    return;
  }

  // koren ma hladany kluc
  if ((*tree)->key == key) {
    delete = *tree;
    // ma lavy podstrom
    if (delete->left != NULL) {
      // ma aj pravy podstrom
      if (delete->right != NULL) {
        bst_replace_by_rightmost(delete, &(*tree)->left);
        return;
      }
      // ma iba lavy podstrom
      else {
        (*tree) = delete->left;
      }
    }
    // ma iba pravy podstrom
    else if (delete->right != NULL) {
      (*tree) = delete->right;
    }
    // nema ziadny podstrom
    else {
      (*tree) = NULL;
    }
    free(delete);
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
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
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
