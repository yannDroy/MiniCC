/**********************************************************************/
/*** Header pour le module gerant la table de description des types ***/
/**********************************************************************/

#ifndef _T_DESC_TYPE_
#define _T_DESC_TYPE_

#define MAX_T_DESC 6000

typedef int* Table_description;

/*** Prototype des fonctions associees ***/

extern void yyerror (char* const s);

Table_description creer_table_description ();

void detruire_table_description (Table_description t);

void afficher_table_description (Table_description t);

void changer_nombre_var_struct (Table_description t, int n);

void changer_type_array (Table_description t, int type, char* nom_type, char* nom_tab);

void changer_nombre_dimensions (Table_description t, int n);

void ajouter_dimension (Table_description t, int max);

void ajouter_var_struct (Table_description t_desc, int n_lex, int n_lex_f, int type, int n_var, int exec);

void ajouter_arg_procfonct (Table_description t_desc, int n_lex, int n_lex_f, int type, int n_arg, int exec);

void changer_type_fonction (Table_description t, int type);

void changer_nombre_arguments (Table_description t, int n, char* nom);

#endif
