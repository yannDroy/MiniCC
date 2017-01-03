/******************************************/
/*** Module gerant la table des regions ***/
/******************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inc/arbre_binaire.h"
#include "inc/table_declarations.h"
#include "inc/table_regions.h"

extern FILE* yyout;

extern Table_declarations t_dec;

extern int indice_zone_morte;

Table_region creer_table_region ()
{
    /* Cette fonction alloue la memoire
       pour la table des regions
    */
    
    Table_region t_reg = (Table_region) malloc(MAX_REGIONS * sizeof(Cellule_region));

    if(t_reg == NULL)
    {
        fprintf(stdout, "Impossible de créer la table des regions\n");
        exit(EXIT_FAILURE);
    }

    return t_reg;
}

void detruire_table_regions (Table_region t)
{
    free(t);
}

int recuperer_derniere_dec (int region)
{
    /* Cette fonction renvoit l'indice de la derniere
       declaration presente dans la region region
    */

    int indice = -1;
    int max = -1;
    int i;

    for(i = 4 ; i < indice_zone_morte ; i++)
    {
        if(t_dec[i].region == region && t_dec[i].nature == T_VAR)
        {
            if(t_dec[i].execution > max)
            {
                max = t_dec[i].execution;
                indice = i;
            }
        }
    }

    return indice;
}

int recuperer_taille_dec (int num_dec)
{
    int indice_desc = t_dec[num_dec].description;
    int i;

    for(i = 0 ; i < indice_zone_morte ; i++)
    {
        if(i == indice_desc)
        {            
            if((t_dec[i].nature == T_STRUCT) ||
               (t_dec[i].nature == T_ARRAY) ||
               (t_dec[i].nature == T_BASE))
            {
                return t_dec[i].execution;
            }
        }
    }

    return 0;
}   
    
int taille_region (int region, int nb_arg)
{
    /* Cette fonction calcul la taille
       d'une region a l'execution
    */

    int taille = 0;

    taille += recuperer_taille_dec(recuperer_derniere_dec(region));
    taille += t_dec[recuperer_derniere_dec(region)].execution;

    if(taille == 0)
        taille += nb_arg;

    return taille;
}

void ajouter_region (Table_region t_reg , int taille , int NIS , Arbre_bin *arbre)
{
    /* Cette fonction ajoute
       la region plus_haute_region
       dans t_reg
    */

    if(NIS == 0)
    {
        t_reg[0].taille = taille;
        t_reg[0].NIS = NIS;
        t_reg[0].arbre = arbre;
    }
    else
    {
        t_reg[plus_haute_region].taille = taille;
        t_reg[plus_haute_region].NIS = NIS;
        t_reg[plus_haute_region].arbre = arbre;
    }
}

void detruire_arbres_regions (Table_region t_reg)
{
    /* Cette fonction libere la mémoire allouee
       pour les arbres de chaque region
    */

    int i;

    if(plus_haute_region != 0)
    {
        for(i = 0; i <= plus_haute_region; i++)
        {
            if(t_reg[i].arbre != NULL)
                liberer_arbre(t_reg[i].arbre);
        }
    }
}

void afficher_table_region (Table_region t_reg)
{
    /* Cette fonction affiche la table
       des regions t_reg
    */

    int i;

    fprintf(yyout, "Table des régions :\n");
    fprintf(yyout, " _______________________________\n");
    fprintf(yyout, "|  n  | taille |  NIS  | nature |\n");
    fprintf(yyout, "|-------------------------------|\n");

    if(plus_haute_region != 0)
    {
        for(i = 0 ; i <= plus_haute_region ; i++)
        {
            if(i < 10)
                fprintf(yyout, "|  %d  |", i);
            else if(i < 100)
                fprintf(yyout, "| %d  |", i);
            else
                fprintf(yyout, "| %d |", i);
        
            if(t_reg[i].taille < 10)
                fprintf(yyout, "   %d    |", t_reg[i].taille);
            else if(t_reg[i].taille < 100)
                fprintf(yyout, "   %d   |", t_reg[i].taille);
            else
                fprintf(yyout, "  %d   |", t_reg[i].taille);
        
            if(t_reg[i].NIS < 10)
                fprintf(yyout, "   %d   |", t_reg[i].NIS);
            else if(t_reg[i].taille < 100)
                fprintf(yyout, "   %d  |", t_reg[i].NIS);
            else
                fprintf(yyout, "  %d  |", t_reg[i].NIS);

            if(i != 0)
            {
                if(t_reg[i].arbre != NULL)
                {
                    if(t_reg[i].arbre->nature < 10)
                        fprintf(yyout, "    %d   |\n", t_reg[i].arbre->nature);
                    else if(t_reg[i].arbre->nature < 100)
                        fprintf(yyout, "   %d   |\n", t_reg[i].arbre->nature);
                    else
                        fprintf(yyout, "  %d   |\n", t_reg[i].arbre->nature);
                }
                else
                {
                    fprintf(yyout, "  NULL  |\n");
                }
            }
            else
            {
                fprintf(yyout, "  NULL  |\n");
            }
        }
    }
    else
    {
        fprintf(yyout, " la table est vide !\n");
    }

    fprintf(yyout, "'-------------------------------'");
}
                
