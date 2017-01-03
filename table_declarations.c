/************************************************/
/*** Module gerant la table des declarations ****/
/************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inc/table_declarations.h"
#include "inc/association.h"

extern FILE* yyout;

extern Table_lexico t_lex;
extern Table_description t_desc;

extern int indice_desc_types;

int indice_zone_morte = ZONE_MORTE;

Table_declarations creer_table_declarations ()
{
    /* Cette fonction alloue de la mémoire pour la table des déclarations */

    Table_declarations t = NULL;
    int i;

    /* Allocation */
    t = (Table_declarations) malloc(MAX_T_DECL * sizeof(Cellule_declaration));

    /* Test de la création */
    if(t == NULL)
    {
        fprintf(stdout, "Impossible de créer la table des déclarations\n");
        exit(EXIT_FAILURE);
    }
    
    /* Initialisation de toute la table */
    for(i = 0; i < MAX_T_DECL; i++)
    {
        t[i].nature = -1;
        t[i].suivant = -1;
        t[i].region = -1;
        t[i].description = -1;
        t[i].execution = -1;
    }

    /* Types de base en début de table */
    t[0].nature = T_BASE; t[0].region = 0; t[0].description = 0; t[0].execution = 1;
    t[1].nature = T_BASE; t[1].region = 0; t[1].description = 1; t[1].execution = 1;
    t[2].nature = T_BASE; t[2].region = 0; t[2].description = 2; t[2].execution = 1;
    t[3].nature = T_BASE; t[3].region = 0; t[3].description = 3; t[3].execution = 1;

    return t;
}

void detruire_table_declarations (Table_declarations t)
{
    /* Cette fonction rend la mémoire allouée pour la table des déclarations */
    
    free(t);
}

void afficher_table_declarations (Table_declarations t)
{
    /* Cette fonction affiche la table des déclarations */
    
    int i;
    
    fprintf(yyout, "Table des déclarations :\n");
    fprintf(yyout, " _________________________________________________________________\n");
    fprintf(yyout, "|  n   |   nature    | suivant | region | description | exécution |\n");
    fprintf(yyout, "|-----------------------------------------------------------------|\n");

    for(i = 0; i < MAX_T_DECL; i++)
    {
        if(t[i].nature != -1)
        {
            if(i < 10)
                fprintf(yyout, "|  %d   |", i);
            else if(i < 100)
                fprintf(yyout, "|  %d  |", i);
            else if(i < 1000)
                fprintf(yyout, "| %d  |", i);
            else
                fprintf(yyout, "| %d |", i);

            switch(t[i].nature)
            {
            case T_BASE:
                fprintf(yyout, "   T_BASE    |");
                break;
            case T_VAR:
                fprintf(yyout, "   T_VAR     |");
                break;
            case T_ARRAY:
                fprintf(yyout, "  T_ARRAY    |");
                break;
            case T_STRUCT:
                fprintf(yyout, "  T_STRUCT   |");
                break;
            case T_FONCTION:
                fprintf(yyout, " T_FONCTION  |");
                break;
            case T_PROCEDURE:
                fprintf(yyout, " T_PROCEDURE |");
                break;
            }
        
            if(t[i].suivant < 10 && t[i].suivant != -1)
                fprintf(yyout, "    %d    |", t[i].suivant);
            else if(t[i].suivant < 100 || t[i].suivant == -1)
                fprintf(yyout, "   %d    |", t[i].suivant);
            else if(t[i].suivant < 1000 || t[i].suivant == -1)
                fprintf(yyout, "   %d   |", t[i].suivant);
            else
                fprintf(yyout, "   %d  |", t[i].suivant);

            if(t[i].region < 10)
                fprintf(yyout, "   %d    |", t[i].region);
            else if(t[i].suivant < 100)
                fprintf(yyout, "   %d   |", t[i].region);
            else
                fprintf(yyout, "  %d   |", t[i].region);

            if(t[i].description < 10)
                fprintf(yyout, "      %d      |", t[i].description);
            else if(t[i].description < 100)
                fprintf(yyout, "     %d      |", t[i].description);
            else
                fprintf(yyout, "     %d     |", t[i].description);

            if(t[i].execution < 10)
                fprintf(yyout, "     %d     |\n", t[i].execution);
            else if(t[i].execution < 100)
                fprintf(yyout, "    %d     |\n", t[i].execution);
            else if(t[i].execution < 1000)
                fprintf(yyout, "    %d    |\n", t[i].execution);
            else
                fprintf(yyout, "   %d    |\n", t[i].execution);
        }
    }
    
    fprintf(yyout, "'-----------------------------------------------------------------'\n");
}

void ajouter_decl_struct (Table_declarations t, int num_lex, int region, int deplacement)
{
    /* Cette fonction ajoute une déclaration de structure */

    if(t[num_lex].nature == -1)
    { /* cas première déclaration */
        t[num_lex].nature = T_STRUCT;
        t[num_lex].suivant = -1;
        t[num_lex].region = region;
        t[num_lex].description = indice_desc_types;
        indice_desc_types++;
        t[num_lex].execution = deplacement;
    }
    else
    { /* sinon recherche du dernier suivant */
        while(t[num_lex].suivant != -1)
            num_lex = t[num_lex].suivant;

        t[num_lex].suivant = indice_zone_morte;

        t[indice_zone_morte].nature = T_STRUCT;
        t[indice_zone_morte].suivant = -1;
        t[indice_zone_morte].region = region;
        t[indice_zone_morte].description = indice_desc_types;
        indice_desc_types++;
        t[indice_zone_morte].execution = deplacement;

        indice_zone_morte++;
    }
}

void ajouter_decl_array (Table_declarations t, int num_lex, int region, int deplacement)
{
    /* Cette fonction ajoute une déclaration de tableau */

    if(t[num_lex].nature == -1)
    {
        t[num_lex].nature = T_ARRAY;
        t[num_lex].suivant = -1;
        t[num_lex].region = region;
        t[num_lex].description = indice_desc_types;
        indice_desc_types += 2; // on saute un champ pour le type du tableau
        t[num_lex].execution = deplacement;
    }
    else
    {
        while(t[num_lex].suivant != -1)
            num_lex = t[num_lex].suivant;

        t[num_lex].suivant = indice_zone_morte;

        t[indice_zone_morte].nature = T_ARRAY;
        t[indice_zone_morte].suivant = -1;
        t[indice_zone_morte].region = region;
        t[indice_zone_morte].description = indice_desc_types;
        indice_desc_types += 2;
        t[indice_zone_morte].execution = deplacement;

        indice_zone_morte++;
    }
}

void ajouter_decl_variable (Table_declarations t_dec, int num_lex, int num_lex_type, int type, int region, int deplacement)
{
    /* Cette fonction ajoute une déclaration de variable */

    char nom[TAILLE_MAX_LEXEME];
    char erreur[TAILLE_MAX_ERREUR];

    if(num_lex_type != -1)
    {
        strcpy(nom, t_lex[num_lex].lexeme);

        if(t_dec[num_lex].nature == -1)
        {
            t_dec[num_lex].nature = T_VAR;
            t_dec[num_lex].suivant = -1;
            t_dec[num_lex].region = region;
            t_dec[num_lex].description = type;
            t_dec[num_lex].execution = deplacement;
        }
        else
        {
            if(association_nom_var(num_lex, T_VAR, region) != -1)
            {
                sprintf(erreur, "La variable '%s' a déjà été définie", nom);
                yyerror(erreur);
            }
        
            t_dec[num_lex].suivant = indice_zone_morte;

            t_dec[indice_zone_morte].nature = T_VAR;
            t_dec[indice_zone_morte].suivant = -1;
            t_dec[indice_zone_morte].region = region;
            t_dec[indice_zone_morte].description = type;
            t_dec[indice_zone_morte].execution = deplacement;

            indice_zone_morte++;
        }
    }
}

int recuperer_type (Table_declarations t_dec, char* nom, int n_lex_var, int n_lex, char* type, int region)
{
    /* Cette fonction récupère l'indice de déclaration du type dans la table des déclarations */

    char erreur[TAILLE_MAX_ERREUR];
    int max;

    if(region == 0)
        max = 0;
    else
        max = region;
    
    /* Recherche de l'indice dans la table des déclarations */
    if((t_dec[n_lex].nature == T_STRUCT || t_dec[n_lex].nature == T_ARRAY) && (t_dec[n_lex].region == max || t_dec[n_lex].region == 0))
        return n_lex;//t_dec[n_lex].description;
    
    while(t_dec[n_lex].suivant != -1)
    {
        n_lex = t_dec[n_lex].suivant;

        if((t_dec[n_lex].nature == T_STRUCT || t_dec[n_lex].nature == T_ARRAY) && t_dec[n_lex].region == max)
            return n_lex;//t_dec[n_lex].description;
    }

    sprintf(erreur, "La variable '%s' ne peut être déclarée car le type [%s] n'a pas été défini", nom, type);
    yyerror(erreur);

    return -1;
}

void ajouter_decl_fonction (Table_declarations t, int num_lex, int region, int new_region)
{
    /* Cette fonction ajoute la déclaration d'une fonction */
    if(t[num_lex].nature == -1)
    {
        t[num_lex].nature = T_FONCTION;
        t[num_lex].suivant = -1;
        t[num_lex].region = region;
        t[num_lex].description = indice_desc_types;
        indice_desc_types += 2; // on saute un champ pour le nombre d'arguments
        t[num_lex].execution = new_region;
    }
    else
    {
        while(t[num_lex].suivant != -1)
            num_lex = t[num_lex].suivant;

        t[num_lex].suivant = indice_zone_morte;

        t[indice_zone_morte].nature = T_FONCTION;
        t[indice_zone_morte].suivant = -1;
        t[indice_zone_morte].region = region;
        t[indice_zone_morte].description = indice_desc_types;
        indice_desc_types += 2;
        t[indice_zone_morte].execution = new_region;

        indice_zone_morte++;
    }
}

void ajouter_decl_procedure (Table_declarations t, int num_lex, int region, int new_region)
{
    /* Cette fonction ajoute une déclaration de procédure */

    if(t[num_lex].nature == -1)
    {
        t[num_lex].nature = T_PROCEDURE;
        t[num_lex].suivant = -1;
        t[num_lex].region = region;
        t[num_lex].description = indice_desc_types;
        indice_desc_types += 2;
        t[num_lex].execution = new_region;
    }
    else
    {
        while(t[num_lex].suivant != -1)
            num_lex = t[num_lex].suivant;

        t[num_lex].suivant = indice_zone_morte;

        t[indice_zone_morte].nature = T_PROCEDURE;
        t[indice_zone_morte].suivant = -1;
        t[indice_zone_morte].region = region;
        t[indice_zone_morte].description = indice_desc_types;
        indice_desc_types += 2;
        t[indice_zone_morte].execution = new_region;

        indice_zone_morte++;
    }
}

void test_existance_proc (Table_declarations t_dec, int num_lex)
{
    /* Cette fonction teste l'existance d'une procédure */

    int num_teste = num_lex;
    int i, j;
    int n_args;
    int depart;
    int comparaisons;
    char nom_p[TAILLE_MAX_LEXEME];
    char erreur[TAILLE_MAX_ERREUR];

    strcpy(nom_p, t_lex[num_lex].lexeme);

    // On va à la dernière déclaration de ce lexème
    while(t_dec[num_lex].suivant != -1)
        num_lex = t_dec[num_lex].suivant;

    // On parcourt tous les lexèmes de même numéro lexicographique
    while(t_dec[num_teste].suivant != -1)
    {
        // Si même nature (ici procédure) ET même région
        if((t_dec[num_lex].region == t_dec[num_teste].region) && (t_dec[num_lex].nature == t_dec[num_teste].nature))
        {
            // On va regarder la table de description des types
            i = t_dec[num_lex].description + 1;
            j = t_dec[num_teste].description + 1;

            // Test du nombre d'arguments
            if(t_desc[i] == t_desc[j])
            {
                n_args = t_desc[i];

                if(n_args > 0)
                {
                    i++;
                    j++;
                    comparaisons = 0;
                    depart = i;

                    // Comparaison de chaque argument, si un seul différent, c'est bon
                    while(i < depart + n_args * 3)
                    {
                        if(t_desc[i + 1] == t_desc[j + 1])
                            comparaisons++;
                        else
                            break;

                        i += 3;
                        j += 3;
                    }

                    // Si tous identiques, erreur
                    if(comparaisons == n_args)
                    {
                        if(n_args == 1)
                            sprintf(erreur, "La procédure '%s' a déjà été définie avec le même argument", nom_p);
                        else
                            sprintf(erreur, "La procédure '%s' a déjà été définie avec les %d mêmes arguments", nom_p, n_args);

                        yyerror(erreur);
                    }
                }
                else
                {
                    sprintf(erreur, "La procédure '%s' a déjà été définie sans argument", nom_p);
                    yyerror(erreur);
                }
            }
        }
        num_teste = t_dec[num_teste].suivant;
    }
}

void test_existance_fonc (Table_declarations t_dec, int num_lex)
{
    /* Cette fonction teste l'existance d'une fonction */

    int num_teste = num_lex;
    int i, j;
    int n_args;
    int depart;
    int comparaisons;
    char nom_f[TAILLE_MAX_LEXEME];
    char type[TAILLE_MAX_LEXEME];
    char erreur[TAILLE_MAX_ERREUR];

    strcpy(nom_f, t_lex[num_lex].lexeme);

    while(t_dec[num_lex].suivant != -1)
        num_lex = t_dec[num_lex].suivant;

    while(t_dec[num_teste].suivant != -1)
    {
        if(t_dec[num_lex].nature == t_dec[num_teste].nature)
        {
            i = t_dec[num_lex].description;
            j = t_dec[num_teste].description;

            // test du type de retour
            if(t_desc[i] == t_desc[j])
            {
                strcpy(type, t_lex[t_desc[i]].lexeme);
                i++;
                j++;

                if(t_desc[i] == t_desc[j])
                {
                    n_args = t_desc[i];

                    if(n_args > 0)
                    {
                        i++;
                        j++;
                        comparaisons = 0;
                        depart = i;

                        while(i < depart + n_args * 3)
                        {
                            if(t_desc[i + 1] == t_desc[j + 1])
                                comparaisons++;
                            else
                                break;

                            i += 3;
                            j += 3;
                        }

                        if(comparaisons == n_args)
                        {
                            if(n_args == 1)
                                sprintf(erreur, "La fonction '%s' (retour : [%s]) a déjà été définie avec le même argument", nom_f, type);
                            else
                                sprintf(erreur, "La fonction '%s' (retour : [%s]) a déjà été définie avec les %d mêmes arguments", nom_f, type, n_args);

                            yyerror(erreur);
                        }
                        
                    }
                    else
                    {
                        sprintf(erreur, "La fonction '%s' (retour : [%s]) a déjà été définie sans argument", nom_f, type);
                        yyerror(erreur);
                    }
                }
            }
        }
        num_teste = t_dec[num_teste].suivant;
    }
}

void test_existance_struct (Table_declarations t_dec, char* nom, int num_lex, int region)
{
    /* Cette fonction teste l'existance d'une structure */

    char nom_s[TAILLE_MAX_LEXEME];
    char erreur[TAILLE_MAX_ERREUR];

    strcpy(nom_s, nom);

    if(association_nom_var(num_lex, T_STRUCT, region) != -1)
    {
        sprintf(erreur, "La structure '%s' a déjà été définie", nom_s);
        yyerror(erreur);
    }
}

void test_existance_array (Table_declarations t_dec, char* nom, int num_lex, int region)
{
    /* Cette fonction teste l'existance d'un tableau */

    char nom_t[TAILLE_MAX_LEXEME];
    char erreur[TAILLE_MAX_ERREUR];

    strcpy(nom_t, nom);

    if(association_nom_var(num_lex, T_ARRAY, region) != -1)
    {
        sprintf(erreur, "Le type tableau '%s' a déjà été défini", nom_t);
        yyerror(erreur);
    }
}

int recup_taille_dec (Table_declarations t_dec, int n_lex, int region)
{
    /* Cette fonction renvoie la taille d'une déclaration d'un type */

    if((t_dec[n_lex].nature == T_STRUCT || t_dec[n_lex].nature == T_ARRAY) && (t_dec[n_lex].region == region || t_dec[n_lex].region == 0))
        return t_dec[n_lex].execution;

    while(t_dec[n_lex].suivant != -1)
    {
        n_lex = t_dec[n_lex].suivant;

        if((t_dec[n_lex].nature == T_STRUCT || t_dec[n_lex].nature == T_ARRAY) && (t_dec[n_lex].region == region || t_dec[n_lex].region == 0))
            return t_dec[n_lex].execution;
    }

    return -1;
}

void changer_taille_dec (Table_declarations t_dec, int n_lex, int taille, int region)
{
    /* Cette fonction change la taille d'un type déclaré */

    if((t_dec[n_lex].nature == T_STRUCT || t_dec[n_lex].nature == T_ARRAY) && t_dec[n_lex].region == region)
        t_dec[n_lex].execution = taille;

    while(t_dec[n_lex].suivant != -1)
    {
        n_lex = t_dec[n_lex].suivant;

        if((t_dec[n_lex].nature == T_STRUCT || t_dec[n_lex].nature == T_ARRAY) && t_dec[n_lex].region == region)
            t_dec[n_lex].execution = taille;
    }
}

int existance_champ (Table_declarations t_dec, int n_lex, int dec_struct)
{
    /* Cette fonction renvoit le numéro de déclaration du champ n_lex dans la structure dec_struct s'il existe */
    
    int indice;
    int i;

    if(dec_struct != -1){
        indice = t_dec[dec_struct].description;

        for(i = indice + 1; i < (indice + 3 * t_desc[indice]); i += 3){
            if(t_desc[i] == n_lex)
                return t_desc[i + 1];
        }
    }

    return -1;
}
