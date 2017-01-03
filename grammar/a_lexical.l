/* Ce fichier contient le programme
   LEX generant l'analyseur lexical
   capable de reconnaître les lexemes
   du langage MiniC
*/

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "./inc/table_lexicographique.h"
    #include "./inc/arbre_binaire.h"
    #include "y.tab.h"

    extern void yyerror(char* const s);
    char *dupstr(const char *s);
    void count();

    char error[200];

    int nb_char = 0;
    int nb_lignes = 1;
    int com = 0;
    int chaine = 0;
%}

%%
"\""                       { count(); if(!com){ chaine = 1 - chaine; return GUILLEMET; } }
"/*"                       { count(); if(!com){ com = 1; } }
"//"                       { count(); if(!com){ com = 2; } }
"*/"                       { count(); if(!com){ yyerror("syntax error"); }else if(com == 1){ com = 0; } }

"if"                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return IF; } }
"else"                     { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return ELSE; } }
"for"                      { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return FOR; } }
"do"                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return DO; } }
"while"                    { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return WHILE; } }
"switch"                   { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return SWITCH; } }
"case"                     { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return CASE; } }
"break"                    { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return BREAK; } }
"default"                  { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return DEFAULT; } }
"return"                   { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return RETURN; } }
"typedef"                  { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return TYPEDEF; } }
"struct"                   { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return STRUCT; } }
"array"                    { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return ARRAY; } }
"main"                     { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return MAIN; } }

"rand"                     { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return RAND; } }
"abs"                      { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return ABS; } }
"fabs"                     { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return FABS; } }
"sqrt"                     { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return SQRT; } }
"pow"                      { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return POW; } }
"exp"                      { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return EXP; } }
"sin"                      { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return SIN; } }
"cos"                      { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return COS; } }
"tan"                      { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return TAN; } }
"asin"                     { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return ASIN; } }
"acos"                     { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return ACOS; } }
"atan"                     { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return ATAN; } }
"log"                      { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return LOG; } }
"log10"                    { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return LOG10; } }
"floor"                    { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return FLOOR; } }
"ceil"                     { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return CEIL; } }
"itof"                     { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return ITOF; } }
"ftoi"                     { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return FTOI; } }
"sleep"                    { count(); if(chaine){ yylval.type_chaine = strdup(yytext); return MOT; }else if(!com){ return SLEEP; } }

"scanf"                    { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return SCANF; } }
"printf"                   { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return PRINTF; } }
"%i"                       { count(); if(chaine){ yylval.type_chaine = dupstr("%i"); return POURCENT_I; }else if(!com){ sprintf(error, "Erreur fatale, caractère inattendu '%s'", yytext); yyerror(error); exit(EXIT_FAILURE); } }
"%f"                       { count(); if(chaine){ yylval.type_chaine = dupstr("%f"); return POURCENT_F; }else if(!com){ sprintf(error, "Erreur fatale, caractère inattendu '%s'", yytext); yyerror(error); exit(EXIT_FAILURE); } }
"%b"                       { count(); if(chaine){ yylval.type_chaine = dupstr("%b"); return POURCENT_B; }else if(!com){ sprintf(error, "Erreur fatale, caractère inattendu '%s'", yytext); yyerror(error); exit(EXIT_FAILURE); } }
"%c"                       { count(); if(chaine){ yylval.type_chaine = dupstr("%c"); return POURCENT_C; }else if(!com){ sprintf(error, "Erreur fatale, caractère inattendu '%s'", yytext); yyerror(error); exit(EXIT_FAILURE); } }

"int"                      { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ yylval.type_int = 0; return T_INT; } }
"float"                    { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ yylval.type_int = 1; return T_FLOAT; } }
"bool"                     { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ yylval.type_int = 2; return T_BOOL; } }
"char"                     { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ yylval.type_int = 3; return T_CHAR; } }
"void"                     { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return T_VOID; } }

"true"                     { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return TRUE; } }
"false"                    { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return FALSE; } }

"+"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_PLUS; } }
"-"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_MOINS; } }
"*"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_MULT; } }
"/"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_DIV; } }
"%"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_MOD; } }

"="                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_AFF; } }
"+="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_LPLUS; } }
"-="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_LMOINS; } }
"*="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_LMULT; } }
"/="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_LDIV; } }
"%="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_LMOD; } }
"++"                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_INC; } }
"--"                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_DEC; } }

"<"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_INF; } }
">"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_SUP; } }
"<="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_INFE; } }
">="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_SUPE; } }
"=="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_EQU; } }
"!"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return NOT; } }
"!="                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_NEQ; } }
"&&"                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_AND; } }
"||"                       { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return OP_OR; } }

"{"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return ACC_O; } }
"}"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return ACC_F; } }
"("                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return PAR_O; } }
")"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return PAR_F; } }
","                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return VIRG; } }
";"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return PVIRG; } }
":"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return DPOINTS; } }
"."                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return POINT; } }
"["                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return CRO_O; } }
"]"                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }else if(!com){ return CRO_F; } }

[a-z][_a-zA-Z0-9]*         { count(); yylval.type_chaine = dupstr(yytext);
                             if(chaine){ return MOT; }
                             else if(!com){ if(strlen(yytext) < TAILLE_MAX_LEXEME){ return IDF; }
                             else { sprintf(error, "Erreur fatale, dépassement de capacité pour l'identificateur '%s'", yytext); yyerror(error); exit(EXIT_FAILURE); } } }

\'[a-zA-Z0-9_ .&\'\"\(\[|\\\)\]{}$%,;:!?#=/+*-]\' { count(); yylval.type_chaine = dupstr(yytext); if(chaine){return MOT; }else if(!com){ return CHAR; } }

[-]?(0|([1-9][0-9]*))      { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }
                                      else if(!com){ yylval.type_int = atoi(yytext); return INT; } }

[-]?((0|([1-9][0-9]*))\.(([0-9]*[1-9])|0)) { count(); yylval.type_chaine = dupstr(yytext); if(chaine){ return MOT; }else if(!com){ return FLOAT; } }

"\n"                       { count(); nb_lignes++; if(com == 2){ com = 0; } }
"\t"                       { count(); }
" "                        { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; } }

"\\n"                      { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }
                                      else if(!com){ sprintf(error, "Erreur fatale, caractère inattendu '%s'", yytext); yyerror(error); exit(EXIT_FAILURE); } }
"\\t"                      { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }
                                      else if(!com){ sprintf(error, "Erreur fatale, caractère inattendu '%s'", yytext); yyerror(error); exit(EXIT_FAILURE); } }
.                          { count(); if(chaine){ yylval.type_chaine = dupstr(yytext); return MOT; }
                                      else if(!com){ sprintf(error, "Erreur fatale, caractère inattendu '%s'", yytext); yyerror(error); exit(EXIT_FAILURE); } }

%%

char* dupstr(const char *s) {
    char *result = malloc(strlen(s) + 1);
    
    if(result != NULL)
        strcpy(result, s);
    
    return result;
}

void count()
{
    int i;
    
    for (i = 0; yytext[i] != '\0'; i++)
        if (yytext[i] == '\n')
            nb_char = 0;
        else if (yytext[i] == '\t')
            nb_char += 8 - (nb_char % 8);
        else
            nb_char++;
}
