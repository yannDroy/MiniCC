/*************************************************************/
/*** Header pour le module gerant la table lexicographique ***/
/*************************************************************/

#ifndef _T_LEXICO_
#define _T_LEXICO_

#define N_CLASSES 16
#define TAILLE_MAX_LEXEME 500
#define TAILLE_MAX_ERREUR 200

#define TAILLE_T_LEX 5000

typedef struct Cellule_lexico {
    int longueur;
    char* lexeme;
    int suivant;
} Cellule_lexico;

typedef Cellule_lexico* Table_lexico;

typedef int* Table_hachage;

/*** Prototype des fonctions associees ***/

Table_lexico creer_table_lexico(Table_hachage t_hash);

Table_hachage creer_table_hachage();

int hash_code(char *lexeme , int nb_classes);

int ajouter_lexeme_table(char *lexeme , Table_lexico t_lexico , Table_hachage t_hash);

int recuperer_n_lexico(Table_lexico t_lexico , Table_hachage t_hash , char *lexeme);

void afficher_table_hachage(Table_hachage t_hash);

void afficher_table_lexicographique(Table_lexico t_lexico);

void detruire_table_hachage (Table_hachage t);

void detruire_table_lexicographique (Table_lexico t);

#endif
