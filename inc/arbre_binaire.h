/*******************************************/
/*** Header pour le module arbre_binaire ***/
/*******************************************/

#ifndef _ARBRE_BIN_

#define _ARBRE_BIN_

#include "./table_lexicographique.h"

enum { T_POINT , T_CRO , T_IDF , T_BCSTE , T_ICSTE , T_FCSTE , T_CCSTE , T_VARIABLE , T_CHAINE ,T_PLUS , T_MOINS , T_MULT , T_DIV , T_MOD , T_AFF , T_LPLUS , T_LMOINS , T_LMULT , T_LDIV , T_LMOD , T_INC , T_DEC , T_WHILE , T_FOR , T_DO , T_SWITCH , T_CASE , T_DEFAULT ,  T_IF , T_ELSE , T_NOT , T_INF , T_SUP , T_INFE , T_SUPE , T_AND , T_OR , T_EQU , T_NEQ , T_INSTR , T_PRINTF , T_SCANF , T_RETURN , T_RAND , T_ABS , T_FABS , T_SQRT , T_POW , T_EXP , T_SIN , T_COS , T_TAN , T_ACOS , T_ASIN , T_ATAN , T_LOG , T_LOG10 , T_FLOOR , T_CEIL , T_ITOF , T_FTOI , T_SLEEP , T_APPEL , T_LISTE , T_NEG };
    
typedef struct Arbre_bin{
    int nature;
    int valeur;
    struct Arbre_bin *left;
    struct Arbre_bin *right;
} Arbre_bin;

/*** Prototype des fonctions associees ***/

Arbre_bin *arbre_vide ();

int est_vide (Arbre_bin *a);

Arbre_bin *creer_noeud (int nature , int valeur);

Arbre_bin *concat_father_brother (Arbre_bin *a1 , Arbre_bin *a2);

Arbre_bin *concat_father_son (Arbre_bin *a1 , Arbre_bin *a2);

void traverse (Arbre_bin *a , int prof_recursion);

void liberer_arbre (Arbre_bin *a);

#endif
