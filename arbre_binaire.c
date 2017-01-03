#include <stdlib.h>
#include <stdio.h>
#include "inc/arbre_binaire.h"

extern FILE* yyout;

extern Table_lexico t_lex;

Arbre_bin *arbre_vide ()
{
    /* Cette fonction retourne un pointeur vers
       un arbre vide
    */

    return NULL;
}

int est_vide (Arbre_bin *a)
{
    /* Cette fonction renvoit vrai si
       a est vide, faux sinon
    */
    
    return (a == arbre_vide());
}

Arbre_bin *creer_noeud (int nature , int valeur)
{
    /* Cette fontion renvoit un pointeur
       vers un arbre dont la racine est
       passee en parametre
    */

    Arbre_bin *a = (Arbre_bin*) malloc(sizeof(Arbre_bin));

    if(a == NULL)
    {
        fprintf(stdout, "Impossible de créer un noeud\n");
        exit(EXIT_FAILURE);
    }
    
    a->nature = nature;
    a->valeur = valeur;
    
    a->left = arbre_vide();
    a->right = arbre_vide();

    return a;
}

Arbre_bin *concat_father_brother (Arbre_bin *a1 , Arbre_bin *a2)
{
    /* Cette fonction concatene a2 au frere droit de a1
     */

    a1->right = a2;

    return a1;
}

Arbre_bin *concat_father_son (Arbre_bin *a1 , Arbre_bin *a2)
{
    /* Cette fonction concatene a2 au fils gauche de a1
     */

    a1->left = a2;

    return a1;
}

void traverse (Arbre_bin *a, int prof_recursion)
{
    /* Cette fonction affiche l'arbre a
     */

    int i;
    
    if(!est_vide(a))
    {
        for(i = 0 ; i < prof_recursion ; i++)
            fprintf(yyout," ");

        switch(a->nature)
        {
        case T_POINT :
            fprintf(yyout,"T_POINT\n");
            break;
        case T_CRO :
            fprintf(yyout,"T_CRO\n");
            break;
        case T_IDF:
            fprintf(yyout,"IDF(%d)\n", a->valeur);
            break;
        case T_ICSTE :
            fprintf(yyout,"%d\n",a->valeur);
            break;
        case T_FCSTE :
            fprintf(yyout,"%s\n",t_lex[a->valeur].lexeme);
            break;
        case T_CCSTE :
            fprintf(yyout,"'%c'\n",t_lex[a->valeur].lexeme[1]);
            break;
        case T_BCSTE :
            fprintf(yyout,"%s\n",(a->valeur != 0)?"true":"false");
            break;
        case T_VARIABLE:
            fprintf(yyout,"T_VARIABLE\n");
            break;
        case T_CHAINE :
            fprintf(yyout,"%d\n",a->valeur);
            break;
        case T_AFF :
            fprintf(yyout,"%s\n","=");
            break;
        case T_LPLUS :
            fprintf(yyout,"%s\n","+=");
            break;
        case T_LMOINS :
            fprintf(yyout,"%s\n","-=");
            break;
        case T_LMULT :
            fprintf(yyout,"%s\n","*=");
            break;
        case T_LDIV :
            fprintf(yyout,"%s\n","/=");
            break;
        case T_LMOD :
            fprintf(yyout,"%s\n","%=");
            break;
        case T_INC :
            fprintf(yyout,"%s\n","++");
            break;
        case T_DEC :
            fprintf(yyout,"%s\n","--");
            break;
        case T_WHILE :
            fprintf(yyout,"T_WHILE\n");
            break;
        case T_FOR :
            fprintf(yyout,"T_FOR\n");
            break;
        case T_DO :
            fprintf(yyout,"T_DO\n");
            break;
        case T_SWITCH :
            fprintf(yyout,"T_SWITCH\n");
            break;
        case T_CASE :
            fprintf(yyout,"T_CASE (%d)\n",a->valeur);
            break;
        case T_DEFAULT :
            fprintf(yyout,"T_DEFAULT\n");
            break;
        case T_IF :
            fprintf(yyout,"T_IF\n");
            break;
        case T_ELSE :
            fprintf(yyout,"T_ELSE\n");
            break;
        case T_NOT :
            fprintf(yyout,"%s\n","!");
            break;
        case T_INF :
            fprintf(yyout,"%s\n","<");
            break;
        case T_SUP :
            fprintf(yyout,"%s\n",">");
            break;
        case T_INFE :
            fprintf(yyout,"%s\n","<=");
            break;
        case T_SUPE :
            fprintf(yyout,"%s\n",">=");
            break;
        case T_AND :
            fprintf(yyout,"%s\n","&&");
            break;
        case T_OR :
            fprintf(yyout,"%s\n","||");
            break;
        case T_EQU :
            fprintf(yyout,"%s\n","==");
            break;
        case T_NEQ :
            fprintf(yyout,"%s\n","!=");
            break;
        case T_PLUS :
            fprintf(yyout,"%s\n","+");
            break;
        case T_MOINS :
            fprintf(yyout,"%s\n","-");
            break;
        case T_MULT :
            fprintf(yyout,"%s\n","*");
            break;
        case T_DIV :
            fprintf(yyout,"%s\n","/");
            break;
        case T_MOD :
            fprintf(yyout,"%s\n","\%");
            break;
        case T_RAND :
            fprintf(yyout,"T_RAND\n");
            break;
        case T_ABS :
            fprintf(yyout,"T_ABS\n");
            break;
        case T_FABS :
            fprintf(yyout,"T_FABS\n");
            break;
        case T_SQRT :
            fprintf(yyout,"T_SQRT\n");
            break;
        case T_EXP :
            fprintf(yyout,"T_EXP\n");
            break;
        case T_POW :
            fprintf(yyout,"T_POW\n");
            break;
        case T_SIN :
            fprintf(yyout,"T_SIN\n");
            break;
        case T_COS :
            fprintf(yyout,"T_COS\n");
            break;
        case T_TAN :
            fprintf(yyout,"T_TAN\n");
            break;
        case T_ASIN :
            fprintf(yyout,"T_ASIN\n");
            break;
        case T_ACOS :
            fprintf(yyout,"T_ACOS\n");
            break;
        case T_ATAN :
            fprintf(yyout,"T_ATAN\n");
            break;
        case T_LOG :
            fprintf(yyout,"T_LOG\n");
            break;
        case T_LOG10 :
            fprintf(yyout,"T_LOG10\n");
            break;
        case T_FLOOR :
            fprintf(yyout,"T_FLOOR\n");
            break;
        case T_CEIL :
            fprintf(yyout,"T_CEIL\n");
            break;
        case T_ITOF :
            fprintf(yyout,"T_ITOF\n");
            break;
        case T_FTOI :
            fprintf(yyout,"T_FTOI\n");
            break;
        case T_SLEEP :
            fprintf(yyout,"T_SLEEP\n");
            break;
        case T_APPEL :
            fprintf(yyout,"T_APPEL(%d)\n", a->valeur);
            break;
        case T_NEG :
            fprintf(yyout,"T_NEG\n");
            break;
        case T_INSTR :
            fprintf(yyout,"T_INSTR\n");
            break;
        case T_PRINTF :
            fprintf(yyout,"T_PRINTF\n");
            break;
        case T_SCANF :
            fprintf(yyout,"T_SCANF\n");
            break;
        case T_RETURN :
            fprintf(yyout,"T_RETURN\n");
            break;
        }
        
        if(!est_vide(a->left))
            traverse(a->left , prof_recursion + 5);
        
        if(!est_vide(a->right))
            traverse(a->right , prof_recursion);
    }
}

void liberer_arbre (Arbre_bin *a)
{
    /* Cette fonction libere recursivement l'espace memoire
       alloue pour a
    */

    if(!est_vide(a))
    {
        liberer_arbre(a->left);
        liberer_arbre(a->right);

        free(a);
    }
}
