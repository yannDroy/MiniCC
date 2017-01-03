/*****************************************/
/*** Module gerant la pile des regions ***/
/*****************************************/
#include <stdlib.h>
#include <stdio.h>

#include "inc/pile_regions.h"

extern FILE* yyout;

Pile_region pile_vide()
{
    /* Cette fonction renvoit
       une pile vide
    */

    return NULL;
}

int est_pile_vide(Pile_region p)
{
    /* Cette fonction renvoit vrai
       si p est une pile vide
    */

    return p == NULL;
}

Pile_region creer_pile(int s)
{
    /* Cette fonction creee une pile
       de num_region s
    */

    Pile_region p = (Pile_region) malloc(1 * sizeof(Cellule_pile));

    p->num_region = s;
    p->suivant = pile_vide();

    return p;
}

Pile_region empiler(Pile_region p , int e)
{
    /*Cette fonction empile
      e sur p
    */

    Pile_region new_cell = creer_pile(e);

    new_cell->suivant = p;

    return new_cell;
}

Pile_region depiler(Pile_region p)
{
    /* Cette fonction depile et
       renvoit le num_region de p
       s'il existe
    */

    Pile_region tmp = p;
    
    if(!est_pile_vide(p))
    {
        p = p->suivant;
        free(tmp);
    }
    else
    {
        fprintf(yyout, "Erreur : Tentative de dépiler une pile vide\n");
        exit(EXIT_FAILURE);
    }

    return p;
}

void afficher_pile(Pile_region p)
{
    Pile_region tmp = p;

    while(!est_pile_vide(tmp)){
        fprintf(yyout, "%d ", tmp->num_region);
        tmp = tmp->suivant;
    }

    fprintf(yyout, "\n");
}

int regarder_top(Pile_region p)
{
    /* Cette fonction renvoie la
       valeur du haut de pile
       sans dépiler
    */

    int top = -1;
    
    if(!est_pile_vide(p))
    {
        top = p->num_region;
    }

    return top;
}

void liberer_pile(Pile_region p)
{
    Pile_region tmp = p;
    
    while(!est_pile_vide(p))
    {
        p = p->suivant;
        free(tmp);
    }
}
