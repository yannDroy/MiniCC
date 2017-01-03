/**********************************************/
/*** Module gerant la table lexicographique ***/
/**********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inc/table_lexicographique.h"

int i_max = 0;

extern FILE* yyout;

Table_lexico creer_table_lexico (Table_hachage t_hash)
{
    /* Cette fonction cree une table lexicographique
       de nb_cellules cellules
    */

    int i;

    /* Creation table lexicographique */
    Table_lexico t_lexico = (Table_lexico) malloc(TAILLE_T_LEX * sizeof(Cellule_lexico));

    if(t_lexico == NULL)
    {
        fprintf(stdout, "Impossible de créer la table lexicographique\n");
        exit(EXIT_FAILURE);
    }

    for(i = 0 ; i < TAILLE_T_LEX ; i++)
    {
        t_lexico[i].longueur = -1;
        t_lexico[i].lexeme = (char *) malloc(TAILLE_MAX_LEXEME * sizeof(char));
        t_lexico[i].suivant = -1;
    }    

    /* Ajout type base a la table lexicographique */
    ajouter_lexeme_table("int", t_lexico , t_hash);
    ajouter_lexeme_table("float", t_lexico , t_hash);
    ajouter_lexeme_table("bool", t_lexico , t_hash);
    ajouter_lexeme_table("char", t_lexico , t_hash);
    
    return t_lexico;
}

Table_hachage creer_table_hachage ()
{
    /* Cette fonction cree et initialise
       une table de hachage de nb_classes
    */

    int *t_hash = (int *) malloc( N_CLASSES * sizeof(int));
    int i;

    for(i = 0 ; i < N_CLASSES ; i++)
    {
        t_hash[i] = -1; 
    }

    return t_hash;
}

int hash_code (char *lexeme , int nb_classes)
{
    /* Cette fonction calcule et
       renvoit le hash code du
       lexeme passé en parametre
    */

    int hc = 0;
    int i = 0;

    while(lexeme[i] != '\0')
    {
        hc += lexeme[i];
        i++;
    }
    
    return hc % N_CLASSES;
}

int ajouter_lexeme_table (char *lexeme , Table_lexico t_lexico , Table_hachage t_hash)
{
    /* Cette fonction ajouter lexeme a
       la table lexicographique en s'aidant
       de la table de hash code et renvoit son
       numero lexicographique dans la table
       lexicographique
    */
    
    int hc = hash_code(lexeme , N_CLASSES);
    int n_lexico = i_max;
    int i;

    if(t_hash[hc] != -1) // Au moins un lexeme present dans cette classe 
    {
        i = t_hash[hc]; // n_lexico du premier lexeme de la classe

        if(!strcmp(t_lexico[i].lexeme , lexeme)) // Lexeme connu
            return i;
        
        while(t_lexico[i].suivant != -1) // Parcours des lexemes de la classe
        {
            i = t_lexico[i].suivant;
            if(!strcmp(t_lexico[i].lexeme , lexeme)) // Lexeme connu
                return i;
            
        }
        t_lexico[i].suivant = n_lexico; // Maintien chainage

    }
    else // Premier lexeme de sa classe
    {
        t_hash[hc] = n_lexico;
    }

    i_max++;

    /* Enregistrement nouveau lexeme */
    t_lexico[n_lexico].longueur = strlen(lexeme);
    strcpy(t_lexico[n_lexico].lexeme , lexeme);
    t_lexico[n_lexico].suivant = -1;

    return n_lexico;
}

int recuperer_n_lexico (Table_lexico t_lexico , Table_hachage t_hash , char *lexeme)
{
    /* Cette fonction renvoit le numero lexicographique
       de lexeme s'il existe, -1 sinon
    */

    int hc = hash_code(lexeme , N_CLASSES);
    int n_lex;
    
    if(t_hash[hc] != -1) // Classe existe, potentiellement le lexeme aussi
    {
        n_lex = t_hash[hc]; // Premier lexeme de la classe

        if(!strcmp(lexeme , t_lexico[n_lex].lexeme)) // Trouve
        {
            return n_lex;
        }

        while(t_lexico[n_lex].suivant != -1) // Parcours des lexemes suivants
        {
            n_lex = t_lexico[n_lex].suivant;
            if(!strcmp(t_lexico[n_lex].lexeme , lexeme)) // Lexeme connu
            {
                return n_lex;
            }
            
        }

        return -1;

    }
    else
    {
        return -1;
    }
}

void afficher_table_hachage (Table_hachage t_hash)
{
    /* Cette table affiche la table
       de hachage
    */

    int i;
    
    fprintf(yyout,"Table de hachage : \n\n");
    for(i = 0 ; i < N_CLASSES ; i++)
    {
        fprintf(yyout,"%d : %d\n",
                i,
                t_hash[i]);
    }
    
}

void afficher_table_lexicographique (Table_lexico t_lexico)
{
    /* Cette fonction affiche la
       table lexicographique
    */

    int i = 0;
    int j;

    fprintf(yyout,"Table lexicographique : \n");
    fprintf(yyout, " ______________________________________________________________________________\n");
    fprintf(yyout, "|  n  | longueur |                       lexème                      | suivant |\n");
    fprintf(yyout, "|------------------------------------------------------------------------------|\n");

    while(t_lexico[i].longueur != -1)
    {
        if(i < 10)
            fprintf(yyout, "|  %d  |", i);
        else if(t_lexico[i].suivant < 100)
            fprintf(yyout, "| %d  |", i);
        else
            fprintf(yyout, "| %d |", i);

        if(t_lexico[i].longueur < 10)
            fprintf(yyout, "     %d    |", t_lexico[i].longueur);
        else if(t_lexico[i].longueur < 100)
            fprintf(yyout, "    %d    |", t_lexico[i].longueur);
        else
            fprintf(yyout, "    %d   |", t_lexico[i].longueur);

        fprintf(yyout, " %s ", t_lexico[i].lexeme);
        for(j = 0; j < 49 - t_lexico[i].longueur; j++)
            fprintf(yyout, " ");
        fprintf(yyout, "| ");

        if(t_lexico[i].suivant < 10 && t_lexico[i].suivant != -1)
            fprintf(yyout, "   %d    |\n", t_lexico[i].suivant);
        else if(t_lexico[i].suivant < 100 || t_lexico[i].suivant == -1)
            fprintf(yyout, "  %d    |\n", t_lexico[i].suivant);
        else
            fprintf(yyout, " %d    |\n", t_lexico[i].suivant);

        i++;
    }

    fprintf(yyout, "'------------------------------------------------------------------------------'\n");
}

void detruire_table_hachage (Table_hachage t)
{
    free(t);
}

void detruire_table_lexicographique (Table_lexico t)
{
    free(t);
}
