/**********************************************************/
/*** Header pour le module gerant l'association de noms ***/
/**********************************************************/

#ifndef _ASSOCIATION_
#define _ASSOCIATION__

/*** Prototypes des fonctions associees ***/

int association_nom_var (int n_lex , int nature , int region);

int association_nom_fct (char* types_params , int n_lex , int nb_arg);

#endif
