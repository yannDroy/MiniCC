/*******************************************************/
/*** Module gerant la table de description des types ***/
/*******************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inc/table_description_types.h"
#include "inc/pile_execution.h"

extern FILE* yyout;

extern Table_lexico t_lex;

extern int indice_desc_types;
extern int deplacement;

Table_description creer_table_description ()
{
    /* Cette fonction alloue de la mémoire pour la table de description des types */

    Table_description t = NULL;
    int i;

    /* Allocation */
    t = (Table_description) malloc(MAX_T_DESC * sizeof(int));

    /* Test de la création */
    if(t == NULL)
    {
        fprintf(stdout, "Impossible de créer la table de description des types\n");
        exit(EXIT_FAILURE);
    }
    
    /* Initialisation de toute la table */
    for(i = 0; i < MAX_T_DESC; i++)
        t[i] = -1;

    /* Types de base en début de table */
    t[0] = 0;
    t[1] = 1;
    t[2] = 2;
    t[3] = 3;

    /* On commence à remplir la table à l'indice 4 */
    indice_desc_types = 4;

    return t;
}

void detruire_table_description (Table_description t)
{
    /* Cette fonction rend la mémoire allouée pour la table des déclarations */
    free(t);
}

void afficher_table_description (Table_description t)
{
    /* Cette fonction affiche la table de description des types */

    int i = 0;

    fprintf(yyout, "Table de représentation des types :\n");
    fprintf(yyout, " _____________\n");
    fprintf(yyout, "|  n   | t[n] |\n");
    fprintf(yyout, "|-------------|\n");

    while(t[i] != -1)
    {
        if(i < 10)
            fprintf(yyout, "|  %d   ", i);
        else if(i < 100)
            fprintf(yyout, "|  %d  ", i);
        else if(i < 1000)
            fprintf(yyout, "| %d  ", i);
        else
            fprintf(yyout, "| %d ", i);

        if(t[i] < 10 && t[i] >= 0)
            fprintf(yyout, "|  %d   |\n", t[i]);
        else if(t[i] < 100 || t[i] < 0)
            fprintf(yyout, "|  %d  |\n", t[i]);
        else if(t[i] < 1000)
            fprintf(yyout, "|  %d |\n", t[i]);
        else
            fprintf(yyout, "| %d |\n", t[i]);

        i++;
    }

    fprintf(yyout, "'-------------'\n");
}

void changer_nombre_var_struct (Table_description t, int n)
{
    /* Cette fonction change le nombre de variables d'une structure */
    t[indice_desc_types - (3 * n) - 1] = n;
}

void changer_type_array (Table_description t, int type, char* nom_type, char* nom_tab)
{
    /* Cette fonction change le type d'un tableau */

    char erreur[TAILLE_MAX_ERREUR];
    
    if(type == -1)
    {
        sprintf(erreur, "Le type tableau '%s' ne peut pas être créé car le type [%s] n'existe pas", nom_tab, nom_type);
        yyerror(erreur);
    }

    t[indice_desc_types - 1] = type;
}

void changer_nombre_dimensions (Table_description t, int n)
{
    /* Cette fonction change le nombre de dimensions d'un tableau */
    t[indice_desc_types - (2 * n) - 2] = n;
}

void ajouter_dimension (Table_description t, int max)
{
    /* Cette fonction ajoute une dimension à un tableau, dimension de 0 à max */
    t[indice_desc_types] = 0; // borne inférieure
    indice_desc_types++;
    
    t[indice_desc_types] = (max - 1); // borne supérieure
    indice_desc_types++;
}

void ajouter_var_struct (Table_description t_desc, int n_lex, int n_lex_f, int type, int n_var, int exec)
{
    /* Cette fonction ajoute une déclaration de variable à une structure */

    int i;
    char erreur[TAILLE_MAX_ERREUR];
    char nom_s[TAILLE_MAX_LEXEME];
    char nom_a[TAILLE_MAX_LEXEME];
    char type_v[TAILLE_MAX_LEXEME];

    for(i = indice_desc_types - (3 * n_var); i < indice_desc_types; i += 3)
    {
        if(t_desc[i] == n_lex && t_desc[i + 1] == type)
        {
            strcpy(type_v, t_lex[type].lexeme);
            strcpy(nom_a, t_lex[t_desc[i]].lexeme);
            strcpy(nom_s, t_lex[n_lex_f].lexeme);

            sprintf(erreur, "La variable '%s' de type [%s] a déjà été définie dans la liste des champs de la structure '%s'", nom_a, type_v, nom_s);
            yyerror(erreur);
        }
    }

    t_desc[indice_desc_types] = n_lex; // numéro lexicographique
    indice_desc_types++;
    
    t_desc[indice_desc_types] = type; // type de la variable
    indice_desc_types++;

    t_desc[indice_desc_types] = exec; // champ exécution
    indice_desc_types++;
}

void ajouter_arg_procfonct (Table_description t_desc, int n_lex, int n_lex_f, int type, int n_arg, int exec)
{
    /* Cette fonction ajoute un argument à une procédure ou une fonction */

    int i;
    char erreur[TAILLE_MAX_ERREUR];
    char nom_f[TAILLE_MAX_LEXEME];
    char nom_a[TAILLE_MAX_LEXEME];
    char type_a[TAILLE_MAX_LEXEME];
    
    for(i = indice_desc_types - (3 * n_arg); i < indice_desc_types; i += 3)
    {
        if(t_desc[i] == n_lex && t_desc[i + 1] == type)
        {
            strcpy(type_a, t_lex[type].lexeme);
            strcpy(nom_a, t_lex[t_desc[i]].lexeme);
            strcpy(nom_f, t_lex[n_lex_f].lexeme);

            sprintf(erreur, "L'argument '%s' de type [%s] a déjà été défini dans la liste d'arguments de la fonction/procédure '%s'", nom_a, type_a, nom_f);
            yyerror(erreur);
        }
    }

    t_desc[indice_desc_types] = n_lex; // numéro lexicographique
    indice_desc_types++;
    
    t_desc[indice_desc_types] = type; // type de la variable
    indice_desc_types++;

    t_desc[indice_desc_types] = exec; // champ exécution
    indice_desc_types++;
}

void changer_type_fonction (Table_description t, int type)
{
    /* Cette fonction change le type d'une fonction */

    t[indice_desc_types - 2] = type;
}

void changer_nombre_arguments (Table_description t, int n, char* nom)
{
    /* Cette fonction change le nombre d'arguments d'une fonction ou d'une procédure */

    char erreur[TAILLE_MAX_ERREUR];

    if(n <= NB_PARAM_MAX)
    {
        t[indice_desc_types - (3 * n) - 1] = n;
    }
    else
    {
        sprintf(erreur, "La fonction '%s' prend plus de %d paramètres (%d)", nom, NB_PARAM_MAX, n);
        yyerror(erreur);
    }
}
