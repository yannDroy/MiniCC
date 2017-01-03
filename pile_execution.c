#include <stdio.h>
#include <stdlib.h>
#include "inc/pile_execution.h"

extern FILE* yyout;

extern Table_region t_reg;

extern int i_max;
extern int indice_zone_morte;
extern int chainage_dynamique;

int sommet_pile = 0;

Pile_execution creer_pile_execution ()
{
    /* Cette fonction alloue de la mémoire pour la pile à l'exécution */
    
    Pile_execution p = NULL;
    int i;

    p = (Pile_execution) malloc(TAILLE_PILE_EXEC * sizeof(Cellule_pile_exec));

    if(p != NULL)
    {
        for(i = 0; i < TAILLE_PILE_EXEC; i++)
            p[i].type = -1;
        
        return p;
    }
    else
    {
        fprintf(stdout, "Impossible de créer la pile à l'exécution\n");
        exit(EXIT_FAILURE);
    }
}

int ajouter_pile (Pile_execution p, int region)
{
    int base_courante;

    base_courante = sommet_pile;

    if((base_courante + t_reg[region].taille + 2) >= TAILLE_PILE_EXEC){
        fprintf(stdout, "Dépassement de capacité de la pile à l'exécution\n");
        exit(EXIT_FAILURE);
    }

    if(region != 0)
    {
        // chaînage statique
        p[base_courante].type = 0;
        p[base_courante].valeur.int_val = 0;
        base_courante++;

        // chaînage dynamique
        p[base_courante].type = 0;
        p[base_courante].valeur.int_val = chainage_dynamique;
        base_courante++;
    }

    sommet_pile = base_courante + t_reg[region].taille;

    return base_courante;
}

void afficher_pile_execution (Pile_execution p)
{
    int i;
    
    fprintf(yyout, "\nPile d'exécution :\n\n  ");

    if(sommet_pile > 0)
    {
        fprintf(yyout, "| ");
        
        for(i = 0; i < sommet_pile; i++)
        {
            switch(p[i].type)
            {
            case 0:
                fprintf(yyout, "%d | ", p[i].valeur.int_val);
                break;
            case 1:
                fprintf(yyout, "%f | ", p[i].valeur.float_val);
                break;
            case 2:
                fprintf(yyout, "%s | ", (p[i].valeur.int_val != 0)?"true":"false");
                break;
            case 3:
                fprintf(yyout, "'%c' | ", p[i].valeur.char_val);
                break;
            default:
                fprintf(yyout, "-1 | ");
            }
        }
        fprintf(yyout,"\n");
    }
    else
    {
        fprintf(yyout, "La pile d'exécution est vide !\n");
    }
}

void detruire_pile_execution (Pile_execution p)
{
    free(p);
}
