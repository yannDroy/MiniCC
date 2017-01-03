/******************************************/
/*** Module gerant la machine virtuelle ***/
/******************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "inc/table_lexicographique.h"
#include "inc/table_declarations.h"
#include "inc/arbre_binaire.h"
#include "inc/table_regions.h"
#include "inc/pile_execution.h"
#include "inc/machine_virtuelle.h"
#include "inc/controle_types.h"

extern Table_lexico t_lex;
extern Table_declarations t_dec;
extern Table_description t_desc;
extern Table_region t_reg;

Pile_execution p;
int chainage_dynamique;

int r_courante = -1;
int sortir = 0;
int base_courante;
int dans_scanf = 0;

void remplir_dim(int *dim , int nb_dim , int i)
{
    int j;

    for(j = 0 ; j < nb_dim ; j++)
    {
        dim[j] = t_desc[i] + 1;
        i += 2;
    }
}

int deplacement_champ (int n_lex, int dec_struct)
{
    int indice;
    int i;

    indice = t_dec[dec_struct].description;

    for(i = indice + 1; i < (indice + 3 * t_desc[indice]); i += 3){
        if(t_desc[i] == n_lex)
            return t_desc[i + 2];
    }

    return -1;
}

void recup_infos_tableau (int n_lex, int struct_mere, int* nb_dim, int* taille_case, int* premiere_dim)
{
    int i;
    int indice;

    indice = t_dec[struct_mere].description;
    
    for(i = indice + 1; i < (indice + 3 * t_desc[indice]); i += 3){
        if(t_desc[i] == n_lex){
            *taille_case = t_dec[t_desc[t_dec[t_desc[i + 1]].description]].execution;
            *nb_dim = t_desc[t_dec[t_desc[i + 1]].description];
            *premiere_dim = t_dec[t_desc[i + 1]].description + 3;
            break;
        }
    }
}

int deplacement_tableau_struct (int region, Arbre_bin *a , Cellule_pile_exec *res, int struct_mere)
{
    Arbre_bin* b;
    
    int n_lex;
    int nb_dim = 0;
    int taille_case = 1;
    int deplacement = 0;
    int premiere_dim = 0;
    int produit;
    int i, j;

    int choix[MAX_DIM];
    int dim[MAX_DIM];

    n_lex = a->valeur;

    recup_infos_tableau(n_lex, struct_mere, &nb_dim, &taille_case, &premiere_dim);

    deplacement = deplacement_champ(n_lex, struct_mere);
        
    b = a->right;
    
    remplir_dim(dim , nb_dim , premiere_dim);

    i = 0;
    while(!est_vide(b))
    {
        choix[i] = evaluer_expression(region, b , res).valeur.int_val;
        b = b->right;
        i++;
    }

    for(i = 0 ; i < nb_dim ; i++) // Gestion OutOfBounds
    {
        if(choix[i] + 1 > dim[i] || choix[i] < 0)
        {
            fprintf(stdout, "Erreur de segmentation ! L'indice %d sort de la dimension %d de taille %d du tableau\n", choix[i], i + 1, dim[i]);
            exit(EXIT_FAILURE);
        }
    }
    for(i = 0 ; i < nb_dim ; i++) // Determination deplacement
    {
        produit = choix[i];
        for(j = i + 1; j < nb_dim ; j++)
        {
            produit *= dim[j];
        }
        deplacement += produit;
    }

    return (taille_case * deplacement);
}

int deplacement_partie_gauche (int region, Arbre_bin *a, Cellule_pile_exec *res)
{
    /* Renvoit le deplacement d'une variable */
    
    Arbre_bin *b;

    int n_dec;
    int nb_dim;
    int dim[MAX_DIM];
    int choix[MAX_DIM];
    int deplacement = 0;
    int struct_mere;
    int type_tab;
    int taille_case;
    int produit;
    int i, j;
    
    if(est_vide(a))
        return 0;
    
    switch(a->nature)
    {
    case T_POINT :
        b = a;

        deplacement = deplacement_partie_gauche(region, b->left, res);

        if(b->left->nature == T_IDF)
            struct_mere = t_dec[b->left->valeur].description;
        else
            struct_mere = t_desc[t_dec[t_dec[b->left->left->valeur].description].description + 1];

        b = b->left->right;

        while(b->nature == T_POINT)
        {
            if(b->left->nature == T_IDF)
            {
                deplacement += deplacement_champ(b->left->valeur, struct_mere);
                struct_mere = existance_champ(t_dec, b->left->valeur, struct_mere);
            }
            else
            {
                deplacement_tableau_struct(region, b->left->left , res, struct_mere);
                struct_mere = existance_champ(t_dec, b->left->left->valeur, struct_mere);
                struct_mere = t_desc[t_dec[struct_mere].description + 1];
            }
        
            b = b->left->right;
        }
        
        if(b->nature == T_IDF)
            deplacement += deplacement_champ(b->valeur, struct_mere);
        else
            deplacement += deplacement_tableau_struct(region, b->left , res, struct_mere);

        return deplacement;
        
        break;
    case T_CRO :
        n_dec = a->left->valeur;
        nb_dim = t_desc[t_dec[t_dec[n_dec].description].description];

        type_tab = t_desc[t_dec[t_dec[n_dec].description].description + 1];
        taille_case = t_dec[type_tab].execution;
        
        b = a->left->right;

        remplir_dim(dim , nb_dim , t_dec[t_dec[n_dec].description].description + 3);

        i = 0;
        while(!est_vide(b))
        {
            choix[i] = evaluer_expression(region, b , res).valeur.int_val;
            b = b->right;
            i++;
        }

        for(i = 0 ; i < nb_dim ; i++) // Gestion OutOfBounds
        {
            if(choix[i] + 1 > dim[i] || choix[i] < 0)
            {
                fprintf(stdout, "Erreur de segmentation ! L'indice %d sort de la dimension %d de taille %d du tableau\n", choix[i], i + 1, dim[i]);
                exit(EXIT_FAILURE);
            }
        }
        for(i = 0 ; i < nb_dim ; i++) // Determination deplacement
        {
            produit = choix[i];
            for(j = i + 1; j < nb_dim ; j++)
            {
                produit *= dim[j];
            }
            deplacement += produit;
        }

        return (taille_case * deplacement) + deplacement_partie_gauche(region , a->left , res);
        break;
    case T_IDF:
        return t_dec[a->valeur].execution;
        break;
    }

    return 0;
}

int recuperer_region_partie_gauche(Arbre_bin *a)
{
    /* Renvoit la region dans laquelle
       est declaree la partie gauche
    */

    if(est_vide(a))
        return 0;
    
    switch(a->nature)
    {
    case T_POINT :
        return recuperer_region_partie_gauche(a->left);
        break;
    case T_CRO :
        return recuperer_region_partie_gauche(a->left);
        break;
    case T_IDF:
        return t_dec[a->valeur].region;
        break;
    }

    return 0;
}

void evaluer_arbre(int region , Arbre_bin *a , Cellule_pile_exec *res)
{
    /* Cette fonction evalue
       l'arbre a de la region
       dont les donnees commence
       a l'indice base_courante
    */
    
    Arbre_bin *b = NULL;
    int deplacement;

    int next_region;
    int compteur;
    int new_bc;
    int indice;

    if(region != r_courante)
    {
        sortir = 0;
        r_courante = region;
    }

    if(!est_vide(a))
    {
        if(!sortir)
        {
            switch(a->nature)
            {
                /* Affectations */
            case T_AFF :
                deplacement = deplacement_partie_gauche(region , a->left , res);
            
                *res = evaluer_expression(region, a->left->right , res);
                
                if(recuperer_region_partie_gauche(a->left) == 0) // Variable globale
                    p[deplacement] = *res;
                else
                    p[base_courante + deplacement] = *res;
            
                break;
            case T_LPLUS :
                deplacement = deplacement_partie_gauche(region , a->left , res);
            
                *res = evaluer_expression(region, a->left->right , res);
            
                if(recuperer_region_partie_gauche(a->left) == 0) // Variable globale
                    indice = deplacement;
                else
                    indice = base_courante + deplacement;

                switch(p[indice].type)
                {
                case 0:
                    p[indice].valeur.int_val += res->valeur.int_val;
                    break;
                case 1:
                    p[indice].valeur.float_val += res->valeur.float_val;
                    break;
                case 2:
                    break;
                case 3:
                    p[indice].valeur.char_val += res->valeur.char_val;
                    break;
                }
            
                break;
            case T_LMOINS :
                deplacement = deplacement_partie_gauche(region , a->left , res);
            
                *res = evaluer_expression(region, a->left->right , res);
            
                if(recuperer_region_partie_gauche(a->left) == 0) // Variable globale
                    indice = deplacement;
                else
                    indice = base_courante + deplacement;

                switch(p[indice].type)
                {
                case 0:
                    p[indice].valeur.int_val -= res->valeur.int_val;
                    break;
                case 1:
                    p[indice].valeur.float_val -= res->valeur.float_val;
                    break;
                case 2:
                    p[indice].valeur.bool_val = (p[indice].valeur.bool_val && res->valeur.bool_val);
                    if(p[indice].valeur.bool_val != 0)
                        p[indice].valeur.bool_val = 1;
                    break;
                case 3:
                    p[indice].valeur.char_val -= res->valeur.char_val;
                    break;
                }
            
                break;
            case T_LMULT:
                deplacement = deplacement_partie_gauche(region , a->left , res);
            
                *res = evaluer_expression(region, a->left->right , res);
            
                if(recuperer_region_partie_gauche(a->left) == 0) // Variable globale
                    indice = deplacement;
                else
                    indice = base_courante + deplacement;

                switch(p[indice].type)
                {
                case 0:
                    p[indice].valeur.int_val *= res->valeur.int_val;
                    break;
                case 1:
                    p[indice].valeur.float_val *= res->valeur.float_val;
                    break;
                case 2:
                    break;
                case 3:
                    break;
                }
            
                break;
            case T_LDIV :
                deplacement = deplacement_partie_gauche(region , a->left ,  res);

                *res = evaluer_expression(region, a->left->right , res);
                
                if(recuperer_region_partie_gauche(a->left) == 0) // Variable globale
                    indice = deplacement;
                else
                    indice = base_courante + deplacement;

                switch(p[indice].type)
                {
                case 0:
                    if(res->valeur.int_val == 0){
                        fprintf(stdout, "Division par zéro lors de l'opération '/=' sur des entiers\n");
                        exit(EXIT_FAILURE);
                    }
                    
                    p[indice].valeur.int_val /= res->valeur.int_val;
                    break;
                case 1:
                    if(res->valeur.float_val == 0){
                        fprintf(stdout, "Division par zéro lors de l'opération '/=' sur des flottants\n");
                        exit(EXIT_FAILURE);
                    }
                    
                    p[indice].valeur.float_val /= res->valeur.float_val;
                    break;
                case 2:
                    break;
                case 3:
                    break;
                }
            
                break;
            case T_LMOD :
                deplacement = deplacement_partie_gauche(region , a->left ,  res);
                
                *res = evaluer_expression(region, a->left->right ,  res);

                if(recuperer_region_partie_gauche(a->left) == 0) // Variable globale
                    indice = deplacement;
                else
                    indice = base_courante + deplacement;

                switch(p[indice].type)
                {
                case 0:
                    p[indice].valeur.int_val %= res->valeur.int_val;
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                }
            
                break;
            case T_INC :
                deplacement = deplacement_partie_gauche(region , a->left ,  res);
                
                if(recuperer_region_partie_gauche(a->left) == 0) // Variable globale
                    indice = deplacement;
                else
                    indice = base_courante + deplacement;

                switch(p[indice].type)
                {
                case 0:
                    p[indice].valeur.int_val++;
                    break;
                case 1:
                    p[indice].valeur.float_val += 1.0;
                    break;
                case 2:
                    break;
                case 3:
                    p[indice].valeur.char_val++;
                    break;
                }
            
                break;
            case T_DEC :
                deplacement = deplacement_partie_gauche(region , a->left ,  res);
                
                if(recuperer_region_partie_gauche(a->left) == 0) // Variable globale
                    indice = deplacement;
                else
                    indice = base_courante + deplacement;

                switch(p[indice].type)
                {
                case 0:
                    p[indice].valeur.int_val--;
                    break;
                case 1:
                    p[indice].valeur.float_val -= 1.0;
                    break;
                case 2:
                    break;
                case 3:
                    p[indice].valeur.char_val--;
                    break;
                }
                
                break;
            
                /* Entrées sorties */
            case T_PRINTF:
                evaluer_printf(region, a,   res);
                
                break;
            case T_SCANF:
                evaluer_scanf(region , a,   res);
                
                break;
            
                /* Conditionnelles */
            case T_IF:
                switch(type_exp(a->left))
                {
                case 0:
                    if(evaluer_expression(region, a->left ,   res).valeur.int_val != 0)
                        evaluer_arbre(region, a->left->right->left ,  res);
                    else if(!est_vide(a->left->right->right->left))
                        evaluer_arbre(region, a->left->right->right->left ,  res);
                    break;
                case 1:
                    if(evaluer_expression(region, a->left ,   res).valeur.float_val != 0.0)
                        evaluer_arbre(region, a->left->right->left ,  res);
                    else if(!est_vide(a->left->right->right->left))
                        evaluer_arbre(region, a->left->right->right->left ,  res);
                    break;
                case 2:
                    if(evaluer_expression(region, a->left ,   res).valeur.bool_val != 0)
                        evaluer_arbre(region, a->left->right->left ,  res);
                    else if(!est_vide(a->left->right->right->left))
                        evaluer_arbre(region, a->left->right->right->left ,  res);
                    break;
                case 3:
                    if(evaluer_expression(region, a->left ,   res).valeur.char_val != 0)
                        evaluer_arbre(region, a->left->right->left ,  res);
                    else if(!est_vide(a->left->right->right->left))
                        evaluer_arbre(region, a->left->right->right->left ,  res);
                    break;
                }
            
                break;
            case T_SWITCH:
                b = a->left->right;

                *res = evaluer_expression(region, a->left->left,   res);
                
                switch(type_exp(a->left->left))
                {
                case 0: // Entier
                    while(!est_vide(b) && b->nature != T_DEFAULT)
                    {
                        if(res->valeur.int_val == b->valeur)
                            break;
                        
                        b = b->right;
                    }

                    if(!est_vide(b))
                        evaluer_arbre(region, b->left ,   res);
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    while(!est_vide(b) && b->nature != T_DEFAULT)
                    {
                        if(res->valeur.char_val == t_lex[b->valeur].lexeme[1])
                            break;
                        
                        b = b->right;
                    }

                    if(!est_vide(b))
                        evaluer_arbre(region, b->left ,   res);
                    break;
                }
            
                break;

                /* Boucles */            
            case T_DO:
                b = a->left->left;

                switch(type_exp(a->left->right->left))
                {
                case 0:
                    do
                        evaluer_arbre(region, b ,  res);
                    while(evaluer_expression(region, a->left->right->left ,   res).valeur.int_val != 0);
                    break;
                case 1:
                    do
                        evaluer_arbre(region, b ,  res);
                    while(evaluer_expression(region, a->left->right->left ,   res).valeur.float_val != 0.0);
                    break;
                case 2:
                    do
                        evaluer_arbre(region, b ,  res);
                    while(evaluer_expression(region, a->left->right->left ,   res).valeur.bool_val != 0);
                    break;
                case 3:
                    do
                        evaluer_arbre(region, b ,  res);
                    while(evaluer_expression(region, a->left->right->left ,   res).valeur.char_val != 0);
                    break;
                }
            
                break;
            case T_WHILE:
                b = a->left->right->left;

                switch(type_exp(a->left))
                {
                case 0:
                    while(evaluer_expression(region, a->left ,  res).valeur.int_val != 0)
                        evaluer_arbre(region, b ,  res);
                    break;
                case 1:
                    while(evaluer_expression(region, a->left ,  res).valeur.float_val != 0.0)
                        evaluer_arbre(region, b ,  res);
                    break;
                case 2:
                    while(evaluer_expression(region, a->left ,  res).valeur.bool_val != 0)
                        evaluer_arbre(region, b ,  res);
                    break;
                case 3:
                    while(evaluer_expression(region, a->left ,  res).valeur.char_val != 0)
                        evaluer_arbre(region, b ,  res);
                    break;
                }
            
                break;
            case T_FOR:
                switch(type_exp(a->left->right))
                {
                case 0:
                    for(evaluer_arbre(region, a->left->left ,  res);
                        evaluer_expression(region, a->left->right ,  res).valeur.int_val != 0;
                        evaluer_arbre(region, a->left->right->right->left ,  res))
                    {
                        evaluer_arbre(region, a->left->right->right->right->left ,  res);
                    }
                    break;
                case 1:
                    for(evaluer_arbre(region, a->left->left ,  res);
                        evaluer_expression(region, a->left->right ,  res).valeur.float_val != 0.0;
                        evaluer_arbre(region, a->left->right->right->left ,  res))
                    {
                        evaluer_arbre(region, a->left->right->right->right->left ,  res);
                    }
                    break;
                case 2:
                    for(evaluer_arbre(region, a->left->left ,  res);
                        evaluer_expression(region, a->left->right ,  res).valeur.bool_val != 0;
                        evaluer_arbre(region, a->left->right->right->left ,  res))
                    {
                        evaluer_arbre(region, a->left->right->right->right->left ,  res);
                    }
                    break;
                case 3:
                    for(evaluer_arbre(region, a->left->left ,  res);
                        evaluer_expression(region, a->left->right ,  res).valeur.char_val != 0;
                        evaluer_arbre(region, a->left->right->right->left ,  res))
                    {
                        evaluer_arbre(region, a->left->right->right->right->left ,  res);
                    }
                    break;
                }
            
                break;
            
                /* Appels de fonctions et procédures */
            case T_APPEL:
                
                chainage_dynamique = base_courante;
            
                next_region = t_dec[a->valeur].execution;
            
                new_bc = ajouter_pile(p , next_region);
            
                b = a->left;
                compteur = 0;
            
                while(!est_vide(b))
                {
                    *res = evaluer_expression(region, b, res);
                    p[new_bc + compteur] = *res;
                    compteur++;
                    b = b->right;
                }

                base_courante = new_bc;
                
                evaluer_arbre(next_region, t_reg[next_region].arbre, res);

                base_courante = p[base_courante - 1].valeur.int_val;
            
                break;
            case T_RETURN:
                *res = evaluer_expression(region, a->left , res);
                sortir = 1;
                break;
            case T_SLEEP:
                sleep(evaluer_expression(region, a->left, res).valeur.int_val);
                break;
            }
        }

        if(!est_vide(a->right))
            evaluer_arbre(region, a->right , res);
    }
    
    
}

Cellule_pile_exec evaluer_expression(int region, Arbre_bin *a , Cellule_pile_exec *res)
{
    /* Cette fonction renvoit le
       resultat de l'evaluation de a
    */

    Arbre_bin *b;
    Cellule_pile_exec cell;
    
    int delta;
    int next_region;
    int compteur;
    int new_bc;
    int min;
    int max;
    float test;
    
    if(est_vide(a) || sortir)
    {
        cell.type = 0;
        cell.valeur.int_val = 0;
        return cell;
    }
    
    switch(a->nature)
    {
    case T_PLUS :
        switch(type_exp(a->left))
        {
        case 0:
            cell.type = 0;
            cell.valeur.int_val = evaluer_expression(region, a->left , res).valeur.int_val + evaluer_expression(region, a->left->right , res).valeur.int_val;
            return cell;
            break;
        case 1:
            cell.type = 1;
            cell.valeur.float_val = evaluer_expression(region, a->left , res).valeur.float_val + evaluer_expression(region, a->left->right , res).valeur.float_val;
            return cell;
            break;
        case 2:
            break;
        case 3:
            cell.type = 3;
            cell.valeur.char_val = evaluer_expression(region, a->left , res).valeur.char_val + evaluer_expression(region, a->left->right , res).valeur.char_val;
            return cell;
            break;
        }
        
        break;
    case T_MOINS :
        switch(type_exp(a->left))
        {
        case 0:
            cell.type = 0;
            cell.valeur.int_val = evaluer_expression(region, a->left , res).valeur.int_val - evaluer_expression(region, a->left->right , res).valeur.int_val;
            return cell;
            break;
        case 1:
            cell.type = 1;
            cell.valeur.float_val = evaluer_expression(region, a->left , res).valeur.float_val - evaluer_expression(region, a->left->right , res).valeur.float_val;
            return cell;
            break;
        case 2:
            break;
        case 3:
            cell.type = 3;
            cell.valeur.char_val = evaluer_expression(region, a->left , res).valeur.char_val - evaluer_expression(region, a->left->right , res).valeur.char_val;
            return cell;
            break;
        }

        break;
    case T_MULT :
        switch(type_exp(a->left))
        {
        case 0:
            cell.type = 0;
            cell.valeur.int_val = evaluer_expression(region, a->left , res).valeur.int_val * evaluer_expression(region, a->left->right , res).valeur.int_val;
            return cell;
            break;
        case 1:
            cell.type = 1;
            cell.valeur.float_val = evaluer_expression(region, a->left , res).valeur.float_val * evaluer_expression(region, a->left->right , res).valeur.float_val;
            return cell;
            break;
        case 2:
            break;
        case 3:
            break;
        }
        
        break;
    case T_DIV :
        switch(type_exp(a->left))
        {
        case 0:
            min = evaluer_expression(region, a->left->right , res).valeur.int_val;
            if(min == 0){
                fprintf(stdout, "Division par zéro lors de l'opération '/'\n");
                exit(EXIT_FAILURE);
            }
            
            cell.type = 0;
            cell.valeur.int_val = evaluer_expression(region, a->left , res).valeur.int_val / min;
            return cell;
            break;
        case 1:
            test = evaluer_expression(region, a->left->right , res).valeur.float_val;
            if(test == 0){
                fprintf(stdout, "Division par zéro lors de l'opération '/'\n");
                exit(EXIT_FAILURE);
            }
            
            cell.type = 1;
            cell.valeur.float_val = evaluer_expression(region, a->left , res).valeur.float_val / test;
            return cell;
            break;
        case 2:
            break;
        case 3:
            break;
        }
        
        break;
    case T_MOD :
        cell.type = 0;
        cell.valeur.int_val = evaluer_expression(region, a->left , res).valeur.int_val % evaluer_expression(region, a->left->right , res).valeur.int_val;
        return cell;
        
        break;
    case T_NOT :
        cell.type = 2;
        cell.valeur.bool_val = 1 - evaluer_expression(region, a->left , res).valeur.bool_val;
        return cell;
            
        break;
    case T_INC :
        switch(type_exp(a->left))
        {
        case 0:
            cell.type = 0;
            cell.valeur.int_val = evaluer_expression(region, a->left , res).valeur.int_val + 1;
            return cell;
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            cell.type = 3;
            cell.valeur.char_val = evaluer_expression(region, a->left , res).valeur.char_val + 1;
            return cell;
            break;
        }
        
        break;
    case T_DEC :
        switch(type_exp(a->left))
        {
        case 0:
            cell.type = 0;
            cell.valeur.int_val = evaluer_expression(region, a->left , res).valeur.int_val - 1;
            return cell;
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            cell.type = 3;
            cell.valeur.char_val = evaluer_expression(region, a->left , res).valeur.char_val - 1;
            return cell;
            break;
        }
        
        break;
    case T_INF :
        cell.type = 2;
        cell.valeur.bool_val = (evaluer_expression(region, a->left , res).valeur.bool_val < evaluer_expression(region, a->left->right , res).valeur.bool_val);
        
        if(cell.valeur.bool_val != 0)
            cell.valeur.bool_val = 1;
        
        return cell;
        
        break;
    case T_SUP :
        cell.type = 2;
        cell.valeur.bool_val = (evaluer_expression(region, a->left , res).valeur.bool_val > evaluer_expression(region, a->left->right , res).valeur.bool_val);
        
        if(cell.valeur.bool_val != 0)
            cell.valeur.bool_val = 1;
        
        return cell;
        
        break;
    case T_INFE :
        cell.type = 2;
        cell.valeur.bool_val = (evaluer_expression(region, a->left , res).valeur.bool_val <= evaluer_expression(region, a->left->right , res).valeur.bool_val);
        
        if(cell.valeur.bool_val != 0)
            cell.valeur.bool_val = 1;
        
        return cell;
        
        break;
    case T_SUPE :
        cell.type = 2;
        cell.valeur.bool_val = (evaluer_expression(region, a->left , res).valeur.bool_val >= evaluer_expression(region, a->left->right , res).valeur.bool_val);
        
        if(cell.valeur.bool_val != 0)
            cell.valeur.bool_val = 1;
        
        return cell;
        
        break;
    case T_EQU :
        cell.type = 2;
        cell.valeur.bool_val = (evaluer_expression(region, a->left , res).valeur.bool_val == evaluer_expression(region, a->left->right , res).valeur.bool_val);
        
        if(cell.valeur.bool_val != 0)
            cell.valeur.bool_val = 1;
        
        return cell;
        
        break;
    case T_NEQ :
        cell.type = 2;
        cell.valeur.bool_val = (evaluer_expression(region, a->left , res).valeur.bool_val != evaluer_expression(region, a->left->right , res).valeur.bool_val);
        
        if(cell.valeur.bool_val != 0)
            cell.valeur.bool_val = 1;
        
        return cell;
        
        break;
    case T_AND :
        cell.type = 2;
        cell.valeur.bool_val = (evaluer_expression(region, a->left , res).valeur.bool_val && evaluer_expression(region, a->left->right , res).valeur.bool_val);
        
        if(cell.valeur.bool_val != 0)
            cell.valeur.bool_val = 1;
        
        return cell;
        
        break;
    case T_OR :
        cell.type = 2;
        cell.valeur.bool_val = (evaluer_expression(region, a->left , res).valeur.bool_val || evaluer_expression(region, a->left->right , res).valeur.bool_val);
        
        if(cell.valeur.bool_val != 0)
            cell.valeur.bool_val = 1;
        
        return cell;
        
        break;
    case T_ICSTE :
        cell.type = 0;
        cell.valeur.int_val = a->valeur;
        return cell;
        break;
    case T_FCSTE :
        cell.type = 1;
        cell.valeur.float_val = atof(t_lex[a->valeur].lexeme);
        return cell;
        break;
    case T_BCSTE :
        cell.type = 2;
        cell.valeur.bool_val = a->valeur;
        return cell;
        break;
    case T_CCSTE :
        cell.type = 3;
        cell.valeur.char_val = t_lex[a->valeur].lexeme[1];
        return cell;
        break;
    case T_IDF :
        delta = deplacement_partie_gauche(region , a ,  res);

        if(recuperer_region_partie_gauche(a) != 0) // Variable globale
            delta += base_courante;

        if(p[delta].type == -1 && !dans_scanf)
            fprintf(stdout, "Attention ! Utilisation d'une variable non initialisée\n");

        return p[delta];
        break;
    case T_POINT :
        delta = deplacement_partie_gauche(region , a , res);
        
        if(recuperer_region_partie_gauche(a) != 0) // Variable globale
            delta += base_courante;
        
        return p[delta];
        break;
    case T_CRO :
        delta = deplacement_partie_gauche(region , a , res);
        
        if(recuperer_region_partie_gauche(a) != 0) // Variable globale
            delta += base_courante;
        
        return p[delta];
        break;
    case T_RAND :
        min = evaluer_expression(region, a->left, res).valeur.int_val;
        max = evaluer_expression(region, a->left->right, res).valeur.int_val;
        cell.type = 0;
        cell.valeur.int_val = entier_aleatoire(min , max);
        return cell;
        break;
    case T_ABS :
        cell.type = 0;
        cell.valeur.int_val = abs(evaluer_expression(region, a->left, res).valeur.int_val);
        return cell;
        break;
    case T_FABS :
        cell.type = 1;
        cell.valeur.float_val = fabs(evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_SQRT :
        test = evaluer_expression(region, a->left, res).valeur.float_val;
        if(test < 0){
            fprintf(stdout, "La fonction 'sqrt' n'est définie que sur les flottants sur [0 ; +inf[\n");
            exit(EXIT_FAILURE);
        }
        cell.type = 1;
        cell.valeur.float_val = sqrt(test);
        return cell;
        break;
    case T_POW :
        cell.type = 1;
        cell.valeur.float_val = pow(evaluer_expression(region, a->left, res).valeur.float_val, (float) evaluer_expression(region, a->left->right, res).valeur.int_val);
        return cell;
        break;
    case T_EXP :
        cell.type = 1;
        cell.valeur.float_val = exp(evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_SIN :
        cell.type = 1;
        cell.valeur.float_val = sin(evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_COS :
        cell.type = 1;
        cell.valeur.float_val = cos(evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_TAN :
        cell.type = 1;
        cell.valeur.float_val = tan(evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_ASIN :
        test = evaluer_expression(region, a->left, res).valeur.float_val;
        if(test < -1 || test > 1){
            fprintf(stdout, "La fonction 'asin' n'est définie que sur les flottants sur [-1 ; 1]\n");
            exit(EXIT_FAILURE);
        }
        
        cell.type = 1;
        cell.valeur.float_val = asin(test);
        return cell;
        break;
    case T_ACOS :
        test = evaluer_expression(region, a->left, res).valeur.float_val;
        if(test < -1 || test > 1){
            fprintf(stdout, "La fonction 'acos' n'est définie que sur les flottants sur [-1 ; 1]\n");
            exit(EXIT_FAILURE);
        }
        
        cell.type = 1;
        cell.valeur.float_val = acos(test);
        return cell;
        break;
    case T_ATAN :
        cell.type = 1;
        cell.valeur.float_val = atan(evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_LOG :
        test = evaluer_expression(region, a->left, res).valeur.float_val;
        if(test <= 0){
            fprintf(stdout, "La fonction 'log' n'est définie que sur les flottants sur ]0 ; +inf[\n");
            exit(EXIT_FAILURE);
        }
        
        cell.type = 1;
        cell.valeur.float_val = log(test);
        return cell;
        break;
    case T_LOG10 :
        test = evaluer_expression(region, a->left, res).valeur.float_val;
        if(test <= 0){
            fprintf(stdout, "La fonction 'log10' n'est définie que sur les flottants sur ]0 ; +inf[\n");
            exit(EXIT_FAILURE);
        }
        
        cell.type = 1;
        cell.valeur.float_val = log10(test);
        return cell;
        break;
    case T_FLOOR :
        cell.type = 1;
        cell.valeur.float_val = floor(evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_CEIL :
        cell.type = 1;
        cell.valeur.float_val = ceil(evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_ITOF :
        cell.type = 1;
        cell.valeur.float_val = (float) (evaluer_expression(region, a->left, res).valeur.int_val);
        return cell;
        break;
    case T_FTOI :
        cell.type = 0;
        cell.valeur.int_val = (int) (evaluer_expression(region, a->left, res).valeur.float_val);
        return cell;
        break;
    case T_NEG :
        switch(type_exp(a->left))
        {
        case 0:
            cell.type = 0;
            cell.valeur.int_val = -evaluer_expression(region, a->left , res).valeur.int_val;
            return cell;
            break;
        case 1:
            cell.type = 1;
            cell.valeur.float_val = -evaluer_expression(region, a->left , res).valeur.float_val;
            return cell;
            break;
        case 2:
            break;
        case 3:
            break;
        }
        
        break;
    case T_APPEL:
        chainage_dynamique = base_courante;

        next_region = t_dec[a->valeur].execution;

        new_bc = ajouter_pile(p , next_region);

        b = a->left;
        compteur = 0;

        while(!est_vide(b))
        {
            *res = evaluer_expression(region, b, res);
            p[new_bc + compteur] = *res;
            compteur++;
            b = b->right;
        }

        base_courante = new_bc;
        
        evaluer_arbre(next_region, t_reg[next_region].arbre, res);

        sortir = 1 - sortir;

        base_courante = p[base_courante - 1].valeur.int_val;

        return *res;
        break;
    }

    cell.type = 0;
    cell.valeur.int_val = 0;
    return cell;
}

void evaluer_scanf (int region , Arbre_bin *a,  Cellule_pile_exec *res)
{
    Arbre_bin* b = NULL;
    char* str = NULL;
    char c;
    int deplacement;
    int i;

    dans_scanf = 1;
    
    str = strdup(t_lex[a->left->valeur].lexeme);
    b = a->left;
    
    for(i = 0; i < strlen(str); i++)
    {
        if(str[i] == '%')
        {
            i++;
            
            if(str[i] == 'i')
            {
                b = b->right;
                
                deplacement = deplacement_partie_gauche(region , b->left ,  res);
                
                if(recuperer_region_partie_gauche(a->left->right->left) == 0){ // Variable globale
                    fscanf(stdin, "%d", &(p[deplacement].valeur.int_val));
                    p[deplacement].type = 0;
                }else{
                    fscanf(stdin, "%d", &(p[base_courante + deplacement].valeur.int_val));
                    p[base_courante + deplacement].type = 0;
                }
            }
            else if(str[i] == 'f')
            {
                b = b->right;
                
                deplacement = deplacement_partie_gauche(region , b->left ,  res);
                
                if(recuperer_region_partie_gauche(a->left->right->left) == 0){ // Variable globale
                    fscanf(stdin, "%f", &(p[deplacement].valeur.float_val));
                    p[deplacement].type = 1;
                }else{
                    fscanf(stdin, "%f", &(p[base_courante + deplacement].valeur.float_val));
                    p[base_courante + deplacement].type = 1;
                }
            }
            else if(str[i] == 'b')
            {
                b = b->right;
                
                deplacement = deplacement_partie_gauche(region , b->left ,  res);
                
                if(recuperer_region_partie_gauche(a->left->right->left) == 0) // Variable globale
                {
                    fscanf(stdin, "%d", &(p[deplacement].valeur.bool_val));
                    
                    if(p[deplacement].valeur.bool_val != 0)
                        p[deplacement].valeur.bool_val = 1;

                    p[deplacement].type = 2;
                }
                else
                {
                    fscanf(stdin, "%d", &(p[base_courante + deplacement].valeur.bool_val));

                    if(p[base_courante + deplacement].valeur.bool_val != 0)
                        p[base_courante + deplacement].valeur.bool_val = 1;

                    p[base_courante + deplacement].type = 2;
                }
            }
            else if(str[i] == 'c')
            {
                b = b->right;
                
                deplacement = deplacement_partie_gauche(region , b->left ,  res);
                
                if(recuperer_region_partie_gauche(a->left->right->left) == 0){ // Variable globale
                    fscanf(stdin, "%c", &(p[deplacement].valeur.char_val));
                    p[deplacement].type = 3;
                }else{
                    fscanf(stdin, "%c", &(p[base_courante + deplacement].valeur.char_val));
                    p[base_courante + deplacement].type = 3;
                }
            }
            
            while((c = getchar()) != EOF && c != '\n');
        }
    }

    dans_scanf = 0;
}

int entier_aleatoire(int min , int max)
{
    /* Renvoit un entier aleatoire entre
       min et max inclus
    */

    return rand()%(max - min + 1) + min;
}
void evaluer_printf (int region, Arbre_bin *a, Cellule_pile_exec *res)
{
    char* str = NULL;
    Arbre_bin* b = NULL;
    
    int i;
    int var_non_init = 0;

    str = strdup(t_lex[a->left->valeur].lexeme);
    
    b = a->left->right;
    while(!est_vide(b))
    {
        if(evaluer_expression(region, b->left , res).type == -1)
        {
            var_non_init = 1;
            break;
        }
        b = b->right;
    }

    if(!var_non_init)
    {
        b = a->left;
        
        for(i = 0; i < strlen(str); i++)
        {
            if(str[i] == '%')
            {
                i++;
                    
                if(str[i] == 'i')
                {
                    b = b->right;
                    fprintf(stdout, "%d", evaluer_expression(region, b->left , res).valeur.int_val);
                }
                else if(str[i] == 'f')
                {
                    b = b->right;
                    fprintf(stdout, "%f", evaluer_expression(region, b->left , res).valeur.float_val);
                }
                else if(str[i] == 'b')
                {
                    b = b->right;
                    fprintf(stdout, "%s", (evaluer_expression(region, b->left , res).valeur.bool_val == 0)?"false":"true");
                }
                else if(str[i] == 'c')
                {
                    b = b->right;
                    fprintf(stdout, "%c", evaluer_expression(region, b->left , res).valeur.char_val);
                }
                else
                {
                    fprintf(stdout, "%%");
                    i--;
                }
            }
            else if(str[i] == '\\')
            {
                i++;
                    
                if(str[i] == 'n')
                    fprintf(stdout, "\n");
                else if(str[i] == 't')
                    fprintf(stdout, "\t");
                else
                {
                    fprintf(stdout, "\\");
                    i--;
                }
            }
            else
                fprintf(stdout, "%c", str[i]);
        }
    }else{
        fprintf(stdout, "Attention ! Utilisation d'une variable non initialisée\n");
    }
}
