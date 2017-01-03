/**********************************************************/
/*** Header pour le module gerant la table des regions  ***/
/**********************************************************/

#ifndef _M_VIRTUELLE_
#define _M_VIRTUELLE_

/*** Prototype des fonctions associees **/

void remplir_dim (int *dim , int nb_dim , int i);

int deplacement_champ (int n_lex, int dec_struct);

void recup_infos_tableau (int n_lex, int struct_mere, int* nb_dim, int* taille_case, int* premiere_dim);

int deplacement_tableau_struct (int region, Arbre_bin *a , Cellule_pile_exec *res, int struct_mere);

int deplacement_partie_gauche (int region , Arbre_bin *a , Cellule_pile_exec *res);

void evaluer_arbre (int region , Arbre_bin *a , Cellule_pile_exec *res);

Cellule_pile_exec evaluer_expression (int region, Arbre_bin *a , Cellule_pile_exec *res);

int entier_aleatoire (int min , int max);

void evaluer_scanf (int region , Arbre_bin *a , Cellule_pile_exec *res);

void evaluer_printf (int region, Arbre_bin *a, Cellule_pile_exec *res);

#endif
