/********************************************************/
/*** Header pour le module gerant la pile d'exécution ***/
/********************************************************/

#ifndef _PILE_EXEC_
#define _PILE_EXEC_

#include "./arbre_binaire.h"
#include "./table_declarations.h"
#include "./table_regions.h"

#define TAILLE_PILE_EXEC 10000
#define NB_IMBRIC_MAX 1000

typedef union {
    int int_val;
    float float_val;
    int bool_val;
    char char_val;
} union_val;

typedef struct Cellule_pile_exec {
    int type;
    union_val valeur;
} Cellule_pile_exec;

typedef Cellule_pile_exec* Pile_execution;

/*** Prototype des fonctions associees ***/

Pile_execution creer_pile_execution ();

//int* declarations_region (Table_declarations t, int region);

//void detruire_declarations_region (int* t);

int ajouter_pile (Pile_execution p, int region);

void afficher_pile_execution (Pile_execution p);

void detruire_pile_execution (Pile_execution p);

#endif
