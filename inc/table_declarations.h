/**************************************************************/
/*** Header pour le module gerant la table des declarations ***/
/**************************************************************/

#ifndef _T_DECLARATIONS_
#define _T_DECLARATIONS_

#include "./pile_regions.h"
#include "./table_lexicographique.h"
#include "./table_description_types.h"

#define MAX_T_DECL 10000
#define ZONE_MORTE 7000

#define MAX_DIM 1000
#define NB_PARAM_MAX 500

enum { T_BASE, T_VAR, T_STRUCT, T_ARRAY, T_PROCEDURE, T_FONCTION, T_PARAM, T_MAIN };

typedef struct Cellule_declaration {
    int nature;
    int suivant;
    int region;
    int description;
    int execution;
} Cellule_declaration;

typedef Cellule_declaration* Table_declarations;

/*** Prototype des fonctions associees ***/

extern void yyerror (char* const s);

Table_declarations creer_table_declarations ();

void detruire_table_declarations (Table_declarations t);

void afficher_table_declarations (Table_declarations t);

void ajouter_decl_struct (Table_declarations t, int num_lex, int region, int deplacement);

void ajouter_decl_array (Table_declarations t, int num_lex, int region, int deplacement);

void ajouter_decl_variable (Table_declarations t_dec, int num_lex, int num_lex_type, int type, int region, int deplacement);

int recuperer_type (Table_declarations t_dec, char* nom, int n_lex_var, int n_lex, char* type, int region);

void ajouter_decl_fonction (Table_declarations t, int num_lex, int region, int new_region);

void ajouter_decl_procedure (Table_declarations t, int num_lex, int region, int new_region);

void test_existance_proc (Table_declarations t_dec, int num_lex);

void test_existance_fonc (Table_declarations t_dec, int num_lex);

void test_existance_struct (Table_declarations t_dec, char* nom, int num_lex, int region);

void test_existance_array (Table_declarations t_dec, char* nom, int num_lex, int region);

void changer_taille_declaration (Table_declarations t_dec, int num_lex_type, int taille);

int recup_taille_dec (Table_declarations t_dec, int n_lex, int region);

void changer_taille_dec (Table_declarations t_dec, int n_lex, int taille, int region);

int existance_champ (Table_declarations t_dec, int n_lex, int dec_struct);

#endif
