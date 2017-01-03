/********************************************************/
/*** Header pour le module gerant la pile des regions ***/
/********************************************************/

#ifndef _PILE_REGIONS_

#define _PILE_REGIONS_

typedef struct Cellule_pile {
    int num_region;
    struct Cellule_pile *suivant;
}Cellule_pile;

typedef Cellule_pile *Pile_region;

/*** Prototype des fonctions associees ***/

Pile_region pile_vide();

int est_pile_vide(Pile_region p);

Pile_region creer_pile(int s);

Pile_region empiler(Pile_region p , int e);

Pile_region depiler(Pile_region p);

int regarder_top(Pile_region p);

void afficher_pile(Pile_region p);

void liberer_pile(Pile_region p);

#endif
