/********************************************/
/*** Module gerant l'association des noms ***/
/********************************************/

#include <stdlib.h>
#include <stdio.h>
#include "inc/table_declarations.h"
#include "inc/table_description_types.h"
#include "inc/association.h"

extern Table_declarations t_dec;
extern Table_description t_desc;

int association_nom_var (int n_lex , int nature , int region)
{
    /* Renvoit le numero de declaration
       associe au nom de l'objet
       tmp contient l'indice de la
       declaration globale si elle
       existe, -1 sinon
    */

    int tmp = -1;

    if(n_lex == -1)
        return -1;

    if((t_dec[n_lex].nature == nature) && (t_dec[n_lex].region == region)) // Bonne declaration tout de suite
    {
        return n_lex;
    }
    else if((t_dec[n_lex].nature == nature) && (t_dec[n_lex].region == 0)) // Variable globale correspond au cas ou
        tmp = n_lex; // On sauve l'indice

    while(t_dec[n_lex].suivant != -1) // Parcours des suivants
    {
        n_lex = t_dec[n_lex].suivant;

        if((t_dec[n_lex].nature == nature) && (t_dec[n_lex].region == region)) // Bonne declaration tout de suite
            return n_lex;
        else if((t_dec[n_lex].nature == nature) && (t_dec[n_lex].region == 0)) // Variable globale correspond au cas ou
            tmp = n_lex; // On sauve l'indice
    }

    return tmp;
}

int association_nom_fct (char* types_params, int n_lex , int nb_arg)
{
    int compteur;
    int i, j;

    if(n_lex == -1)
        return -1;

    if((t_dec[n_lex].nature == T_FONCTION) || (t_dec[n_lex].nature == T_PROCEDURE)) // Bon type tout de suite
    {
        if(t_desc[t_dec[n_lex].description + 1] == nb_arg) // Bon nombre arguments (2eme case description type)
        {
            compteur = 0;
            j = 0;

            // Boucle sur les types des arguments dans la table de description des types
            for(i = t_dec[n_lex].description + 3; i < (t_dec[n_lex].description + 3) + 3 * nb_arg; i += 3)
            {
                if(t_desc[i] == types_params[j] - '0')
                    compteur++;

                j++;
            }

            // Si tous les arguments sont bons, OK
            if(compteur == nb_arg)
                return n_lex;
        }
    }

    while(t_dec[n_lex].suivant != -1) // Parcours des suivants
    {
        n_lex = t_dec[n_lex].suivant;
        
        if((t_dec[n_lex].nature == T_FONCTION) || (t_dec[n_lex].nature == T_PROCEDURE)) // Bon type tout de suite
        {
            if(t_desc[t_dec[n_lex].description + 1] == nb_arg) // Bon nombre arguments (2eme case description type)
            {
                compteur = 0;
                j = 0;
            
                // Boucle sur les types des arguments dans la table de description des types
                for(i = t_dec[n_lex].description + 3; i < (t_dec[n_lex].description + 3) + 3 * nb_arg; i += 3)
                {
                    if(t_desc[i] == types_params[j] - '0')
                        compteur++;
                
                    j++;
                }
            
                // Si tous les arguments sont bons, OK
                if(compteur == nb_arg)
                    return n_lex;
            }
        }
    }
    
    return -1;
}
