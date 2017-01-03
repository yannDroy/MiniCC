/**********************************************************/
/*** Header pour le module gerant le controle ds types  ***/
/**********************************************************/

#ifndef _CONTROLE_
#define _CONTROLE_

/*** Prototype des fonctions associees ***/

int type_exp(Arbre_bin* a);

int type_idf(int n_lex);

int type_struct (Arbre_bin* a);

int type_fct(int n_lex);

int type_array(int n_dec);

void ecrire_type(char* s, int type);

void initialiser_types_params(char *s);

void changer_type_argument(char *s, int type, int i);

#endif
