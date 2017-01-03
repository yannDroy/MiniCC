/**********************************************************/
/*** Header pour le module gerant la table des regions  ***/
/**********************************************************/

#ifndef _T_REGIONS_
#define _T_REGIONS_

#define MAX_REGIONS 500

extern int plus_haute_region;

typedef struct Cellule_region{
    int taille; // MaJ base courante (pile des donness)
    int NIS; //niveaux imbrication statique
    Arbre_bin * arbre;
} Cellule_region;

typedef Cellule_region * Table_region;

/*** Prototype des fonctions associees ***/

Table_region creer_table_region ();

void detruire_table_regions (Table_region t);

int recuperer_derniere_dec (int region);
int recuperer_taille_dec (int num_dec);
int taille_region (int region, int nb_arg);

void ajouter_region (Table_region t_reg , int taille , int NIS , Arbre_bin *arbre);

void detruire_arbres_regions (Table_region t_reg);
void afficher_table_region (Table_region t_reg);

#endif
