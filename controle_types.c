#include <stdio.h>
#include <stdlib.h>
#include "inc/table_lexicographique.h"
#include "inc/table_declarations.h"
#include "inc/table_description_types.h"
#include "inc/arbre_binaire.h"
#include "inc/controle_types.h"
#include "inc/association.h"

extern Table_lexico t_lex;
extern Table_declarations t_dec;
extern Table_description t_desc;

int type_exp (Arbre_bin* a)
{
    char str_type_g[6];
    char str_type_d[6];
    char erreur[TAILLE_MAX_ERREUR];
    int type_g;
    int type_d;
    
    if(!est_vide(a))
    {
        switch(a->nature)
        {
        case T_ICSTE:
            return 0;
            break;
        case T_FCSTE:
            return 1;
            break;
        case T_BCSTE:
            return 2;
            break;
        case T_CCSTE:
            return 3;
            break;
        case T_PLUS:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1) ||
                    (type_g == 3 && type_d == 3))
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '+' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_MOINS:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1) ||
                    (type_g == 3 && type_d == 3))
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '-' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_MULT:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1))
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '*' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_DIV:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1))
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '/' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_MOD:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if(type_g == 0 && type_d == 0)
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '%%' ne peut se faire avec un [%s] et un [%s] (seulement [int])", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_INF:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1) ||
                    (type_g == 3 && type_d == 3))
                return 2;
            else
            {
                sprintf(erreur, "L'opération '<' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_INFE:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1) ||
                    (type_g == 3 && type_d == 3))
                return 2;
            else
            {
                sprintf(erreur, "L'opération '<=' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_SUP:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1) ||
                    (type_g == 3 && type_d == 3))
                return 2;
            else
            {
                sprintf(erreur, "L'opération '>' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_SUPE:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1) ||
                    (type_g == 3 && type_d == 3))
                return 2;
            else
            {
                sprintf(erreur, "L'opération '>=' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_EQU:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1) ||
                    (type_g == 3 && type_d == 3))
                return 2;
            else
            {
                sprintf(erreur, "L'opération '==' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_NEQ:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if((type_g == 0 && type_d == 0) ||
                    (type_g == 1 && type_d == 1) ||
                    (type_g == 3 && type_d == 3))
                return 2;
            else
            {
                sprintf(erreur, "L'opération '!=' ne peut se faire avec un [%s] et un [%s]", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_AND:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if(type_g == 2 && type_d == 2)
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '&&' ne peut se faire avec un [%s] et un [%s] (seulement [bool])", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_NOT:
            type_g = type_exp(a->left);
            
            ecrire_type(str_type_g, type_g);
            
            if(type_g == -1)
                return -1;
            else if(type_g == 2)
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '!' ne peut se faire avec un [%s] (seulement [bool])", str_type_g);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_OR:
            type_g = type_exp(a->left);
            type_d = type_exp(a->left->right);
            
            ecrire_type(str_type_g, type_g);
            ecrire_type(str_type_d, type_d);
            
            if(type_d == -1 || type_g == -1)
                return -1;
            else if(type_g == 2 && type_d == 2)
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '||' ne peut se faire avec un [%s] et un [%s] (seulement [bool])", str_type_g, str_type_d);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_INC:
            type_g = type_exp(a->left);
            
            ecrire_type(str_type_g, type_g);
            
            if(type_g == -1)
                return -1;
            else if(type_g == 0 || type_g == 3)
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '++' ne peut se faire avec un [%s]", str_type_g);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_DEC:
            type_g = type_exp(a->left);
            
            ecrire_type(str_type_g, type_g);
            
            if(type_g == -1)
                return -1;
            else if(type_g == 0 || type_g == 3)
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '--' ne peut se faire avec un [%s]", str_type_g);
                yyerror(erreur);
                return -1;
            }
                    
            break;
        case T_NEG :
            type_g = type_exp(a->left);
            
            ecrire_type(str_type_g, type_g);
            
            if(type_g == -1)
                return -1;
            else if(type_g == 0 ||type_g == 1)
                return type_g;
            else
            {
                sprintf(erreur, "L'opération '-' (négation) ne peut se faire avec un [%s]", str_type_g);
                yyerror(erreur);
                return -1;
            }
            
            break;
        case T_IDF:
            return type_idf(a->valeur);
            break;
        case T_CRO :
            return type_array(a->left->valeur);
            break;
        case T_POINT:
            return type_struct(a);
            break;
        case T_RAND :
            return 0;
            break;
        case T_ABS :
            return 0;
            break;
        case T_FABS :
            return 1;
            break;
        case T_SQRT :
            return 1;
            break;
        case T_EXP :
            return 1;
            break;
        case T_POW :
            return 1;
            break;
        case T_SIN :
            return 1;
            break;
        case T_COS :
            return 1;
            break;
        case T_TAN :
            return 1;
            break;
        case T_ASIN :
            return 1;
            break;
        case T_ACOS :
            return 1;
            break;
        case T_ATAN :
            return 1;
            break;
        case T_LOG :
            return 1;
            break;
        case T_LOG10 :
            return 1;
            break;
        case T_FLOOR :
            return 1;
            break;
        case T_CEIL :
            return 1;
            break;
        case T_ITOF :
            return 1;
            break;
        case T_FTOI :
            return 0;
            break;
        case T_APPEL :
            return type_fct(a->valeur);
            break;
        }
    }

    return -1;
}

int type_idf (int n_lex)
{
    if(n_lex != -1)
        return t_dec[n_lex].description;
    else
        return -1;
}

int type_struct (Arbre_bin* a)
{
    Arbre_bin* b = a;
    int struct_mere = 0;

    if(b->left->nature == T_IDF){
        if(b->left->valeur != -1)
            struct_mere = t_dec[b->left->valeur].description;
        else
            return -1;
    }else{
        if(b->left->left->valeur)
            struct_mere = t_desc[t_dec[t_dec[b->left->left->valeur].description].description + 1];
        else
            return -1;
    }
    b = b->left->right;
    
    while(b->nature == T_POINT)
    {
        if(b->left->nature == T_IDF)
        {
            if(b->left->valeur != -1)
                struct_mere = existance_champ(t_dec, b->left->valeur, struct_mere);
            else
                return -1;
        }
        else
        {
            if(b->left->left->valeur != -1){
                struct_mere = existance_champ(t_dec, b->left->left->valeur, struct_mere);
                struct_mere = t_desc[t_dec[struct_mere].description + 1];
            }else{
                return -1;
            }
        }
        
        b = b->left->right;
    }

    if(b->nature == T_IDF){
        if(b->valeur != -1)
            return existance_champ(t_dec, b->valeur, struct_mere);
    }else{
        if(b->left->valeur != -1)
            return t_desc[existance_champ(t_dec, b->left->valeur, struct_mere) + 1];
    }

    return -1;
}

int type_fct (int n_lex)
{
    if(n_lex != -1)
        return t_desc[t_dec[n_lex].description];

    return -1;
}

int type_array (int n_dec)
{
    if(n_dec != -1)
        return t_desc[t_dec[t_dec[n_dec].description].description + 1];

    return -1;
}

void ecrire_type (char* s, int type)
{
    switch(type){
    case 0:
        sprintf(s, "int");
        break;
    case 1:
        sprintf(s, "float");
        break;
    case 2:
        sprintf(s, "bool");
        break;
    case 3:
        sprintf(s, "char");
        break;
    default :
        sprintf(s, "???");
    }
}

void initialiser_types_params (char *s)
{
    int i;

    for(i = 0; i < NB_PARAM_MAX + 1; i++)
        s[i] = 'x';

    s[NB_PARAM_MAX] = '\0';
}

void changer_type_argument (char *s, int type, int i)
{
    switch(type){
    case 0:
        s[i] = '0';
        break;
    case 1:
        s[i] = '1';
        break;
    case 2:
        s[i] = '2';
        break;
    case 3:
        s[i] = '3';
        break;
    }
}
