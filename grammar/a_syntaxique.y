/* Ce fichier contient le programme
   YACC generant l'analyseur syntaxique
   capable de reconnaitre la grammaire
   du langage MiniC
*/

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <unistd.h>
    
    #include "inc/pile_regions.h"
    #include "inc/table_lexicographique.h"
    #include "inc/table_declarations.h"
    #include "inc/table_description_types.h"
    #include "inc/arbre_binaire.h"
    #include "inc/table_regions.h"
    #include "inc/association.h"
    #include "inc/pile_execution.h"
    #include "inc/machine_virtuelle.h"
    #include "inc/controle_types.h"
    
    //#define YYDEBUG_LEXER_TEXT yytext
    //int yydebug = 1;

    extern int yylex();

    void yyerror(char* const s);
    
    extern int nb_lignes;
    extern int nb_char;
    extern int i_max;
    extern char *yytext;
    extern FILE *yyin;
    extern FILE *yyout;

    extern Pile_execution p;
    
    extern int base_courante;
    extern int chainage_dynamique;
    
    Table_lexico t_lex = NULL;
    Table_description t_desc = NULL;
    Table_declarations t_dec = NULL;
    Table_hachage t_hash = NULL;
    Pile_region pile_regions = NULL;
    Table_region t_reg = NULL;

    int type;
    int n_lex;
    int erreur = 0;
    int n_lex_type;
    int numlex_save;
    int taille_array;
    int variables_arg;
    int deplacement = 0;
    int save_deplacement;
    int variables_struct;
    int nombre_arguments;
    int nombre_var_struct;
    int nombre_dimensions;
    int plus_haute_region = 0;
    int NIS = 0;
    int indice_desc_types = 0;
    int valeur;
    int type_g;
    int type_d;
    int type_switch;
    int n_lex_es;
    int nombre_formats = 0;
    int nombre_args_appel_es = 0;
    int imbrication_switch = -1;
    int imbrication_struct = 0;
    int imbrication_fct = -1;
    int dans_exp = 0;
    int champ_struct = 0;

    int args_appel_imbriques[NB_IMBRIC_MAX] = {0};
    int args_appel_es[NB_PARAM_MAX] = {0};
    int types_switch[NB_IMBRIC_MAX] = {0};
    
    int dim_imbriques_recup[NB_IMBRIC_MAX] = {0};
    int dim_imbriques_dec[NB_IMBRIC_MAX] = {0};
    int dec_struct_imbriques[NB_IMBRIC_MAX] = {0};
    int dec_tab_imbriques[NB_IMBRIC_MAX] = {0};
    int tab_champ_struct[NB_IMBRIC_MAX] = {0};
    int nature_dec_imbriques[NB_IMBRIC_MAX] = {0};

    char str_type_g[6];
    char str_type_d[6];
    char type_params_imbriques[NB_IMBRIC_MAX][NB_PARAM_MAX + 1];
    char lex_struct_imbriques[NB_IMBRIC_MAX][TAILLE_MAX_LEXEME];
    char str_erreur[200];

%}

/* Type de yylval */
%union {
    char* type_chaine;
    int type_int;
    Arbre_bin * type_abr;
}

/* Declaration symboles non terminaux */
%type <type_abr> contenu_foncproc liste_instructions instruction affectation rand abs fabs sqrt pow exp sin cos tan asin acos atan log log10 floor ceil itof ftoi appel liste_arguments_appel liste_arguments_appel_pleine appel_es liste_arguments_appel_es appel_sleep ifelse else while do switch liste_case for valeur val1 val2 val3 val4 val5 val6 variable var1 var2

%type <type_abr> op_bin op_inc

%type <type_chaine> chaine suite_mots mot format

/* Declaration des symboles terminaux */
 // mots-cles
%token IF ELSE FOR DO WHILE SWITCH CASE BREAK DEFAULT RETURN TYPEDEF STRUCT ARRAY MAIN

 // types
%token <type_int> T_INT T_FLOAT T_BOOL T_CHAR
%token T_VOID

 // operateurs artithmetiques
%token OP_PLUS OP_MOINS OP_MULT OP_DIV OP_MOD

 // operateurs d'affectation
%token OP_AFF OP_LPLUS OP_LMOINS OP_LMULT OP_LDIV OP_LMOD OP_INC OP_DEC
 // operateurs de test
%token OP_INF OP_SUP OP_INFE OP_SUPE OP_EQU OP_NEQ OP_AND OP_OR NOT

 // delimiteurs
%token ACC_O ACC_F PAR_O PAR_F VIRG PVIRG DPOINTS POINT CRO_O CRO_F GUILLEMET

 // identificateurs et nombres
%token <type_chaine> IDF CHAR FLOAT
%token <type_int> INT
%token TRUE FALSE

 // fonctions d'entrees/sorties
%token SCANF PRINTF

 // fonctions rajoutees
%token RAND ABS FABS SQRT POW EXP SIN COS TAN ACOS ASIN ATAN LOG LOG10 FLOOR CEIL ITOF FTOI SLEEP

 // formats d'affichage
%token <type_chaine> MOT POURCENT_F POURCENT_B POURCENT_C POURCENT_I

%%

 /******************************/
 /* Grammaire du langage MiniC */
 /******************************/

programme : liste_declarations fonction_main
          ;

liste_declarations : dec_type PVIRG liste_declarations 
                   | dec_var PVIRG liste_declarations
                   | dec_fct liste_declarations
                   | dec_proc liste_declarations
                   |
                   ;

dec_type : dec_struct
         | dec_array
         ;

dec_struct : TYPEDEF STRUCT IDF { nombre_var_struct = 0;
                                  save_deplacement = deplacement;
                                  n_lex = ajouter_lexeme_table($3, t_lex, t_hash);
                                  numlex_save = n_lex;
                                  test_existance_struct(t_dec, $3, numlex_save, regarder_top(pile_regions));
                                  ajouter_decl_struct(t_dec, n_lex, regarder_top(pile_regions), deplacement);
                                  variables_struct = 1;
                                }
             ACC_O liste_dec_struct ACC_F { changer_nombre_var_struct(t_desc, nombre_var_struct);
                                            variables_struct = 0;
                                            changer_taille_dec(t_dec, numlex_save, (deplacement - save_deplacement), regarder_top(pile_regions));
                                            deplacement = save_deplacement;
                                          }
           ;

dec_array : dec_array_base
          | dec_array_creer
          ;

dec_array_base : TYPEDEF type_base ARRAY IDF { nombre_dimensions = 0;
                                               taille_array = 1;
                                               n_lex = ajouter_lexeme_table($4, t_lex, t_hash);
                                               numlex_save = n_lex;
                                               test_existance_array(t_dec, $4, n_lex, regarder_top(pile_regions));
                                               ajouter_decl_array(t_dec, n_lex, regarder_top(pile_regions), deplacement);
                                               changer_type_array(t_desc, type, NULL, NULL);
                                             }
                 liste_dimension { changer_nombre_dimensions(t_desc, nombre_dimensions);
                                   changer_taille_dec(t_dec, numlex_save, taille_array, regarder_top(pile_regions));
                                 }
               ;

dec_array_creer : TYPEDEF IDF ARRAY IDF { nombre_dimensions = 0;
                                          taille_array = 1;
                                          n_lex = ajouter_lexeme_table($4, t_lex, t_hash);
                                          n_lex_type = recuperer_n_lexico(t_lex , t_hash , $2);
                                          numlex_save = n_lex;
                                          test_existance_array(t_dec, $4, n_lex, regarder_top(pile_regions));
                                          ajouter_decl_array(t_dec, n_lex, regarder_top(pile_regions), deplacement);
                                          changer_type_array(t_desc, n_lex_type, $2, $4);
                                        }
                  liste_dimension { changer_nombre_dimensions(t_desc, nombre_dimensions);
                                    taille_array *= recup_taille_dec(t_dec, n_lex_type, regarder_top(pile_regions));
                                    changer_taille_dec(t_dec, numlex_save, taille_array, regarder_top(pile_regions));
                                  }
                ;

liste_dimension : CRO_O INT CRO_F { if(nombre_dimensions == MAX_DIM){
                                        sprintf(str_erreur, "Un tableau peut avoir au plus %d dimensions", MAX_DIM);
                                        yyerror(str_erreur);
                                    }
                      
                                    nombre_dimensions++;
                                    ajouter_dimension(t_desc, $2); taille_array *= $2;
                                  }
                  liste_dimension
                | CRO_O INT CRO_F { if(nombre_dimensions == MAX_DIM){
                                        sprintf(str_erreur, "Un tableau peut avoir au plus %d dimensions", MAX_DIM);
                                        yyerror(str_erreur);
                                    }
                      
                                    nombre_dimensions++;
                                    ajouter_dimension(t_desc, $2); taille_array *= $2;
                                  }
                ;

liste_dec_struct : dec_var { nombre_var_struct++; } PVIRG liste_dec_struct
                 |
                 ;

dec_var : var_base
        | var_creer
        ;

var_base : type_base IDF { n_lex = ajouter_lexeme_table($2, t_lex, t_hash); n_lex_type = type;
                           if(variables_struct){
                               ajouter_var_struct(t_desc, n_lex, numlex_save, type, nombre_var_struct, (deplacement - save_deplacement));
                               deplacement++;
                           }else if(variables_arg){
                               ajouter_decl_variable(t_dec, n_lex, n_lex_type, type, plus_haute_region + 1, deplacement);
                               ajouter_arg_procfonct(t_desc, n_lex, numlex_save, type, nombre_arguments, deplacement);
                               deplacement++;
                           }else{
                               ajouter_decl_variable(t_dec, n_lex, n_lex_type, type, regarder_top(pile_regions), deplacement);
                               deplacement++;
                           }
                         }
         ;

var_creer : IDF { n_lex_type = recuperer_n_lexico(t_lex , t_hash , $1); } IDF
                    {
                        n_lex = ajouter_lexeme_table($3, t_lex, t_hash);
                        if(variables_struct){
                            ajouter_var_struct(t_desc,
                                               n_lex,
                                               numlex_save,
                                               recuperer_type(t_dec , $3, n_lex, n_lex_type, $1, regarder_top(pile_regions)),
                                               nombre_var_struct,
                                               (deplacement - save_deplacement));
                            deplacement += recup_taille_dec(t_dec, n_lex_type, regarder_top(pile_regions));
                        }else if(variables_arg){
                            type = recuperer_type(t_dec , $3, n_lex, n_lex_type, $1, regarder_top(pile_regions));
                            
                            sprintf(str_erreur, "L'argument '%s' de la fonction/procédure n'est pas un type de base mais est de type [%s]", $1, t_lex[type].lexeme);
                            yyerror(str_erreur);
                            
                            /*ajouter_arg_procfonct(t_desc,
                                                  n_lex,
                                                  numlex_save,
                                                  recuperer_type(t_dec , $3, n_lex, n_lex_type, $1, regarder_top(pile_regions)),
                                                  nombre_arguments,
                                                  deplacement);
                            deplacement += recup_taille_dec(t_dec, n_lex_type, regarder_top(pile_regions));*/
                        }else{
                            ajouter_decl_variable(t_dec,
                                                  n_lex,
                                                  n_lex_type,
                                                  recuperer_type(t_dec , $3, n_lex, n_lex_type, $1, regarder_top(pile_regions)),
                                                  regarder_top(pile_regions),
                                                  deplacement);
                            deplacement += recup_taille_dec(t_dec, n_lex_type, regarder_top(pile_regions));
                        }
                    }
          ;

dec_fct : type_base IDF PAR_O { n_lex = ajouter_lexeme_table($2, t_lex, t_hash);
                                numlex_save = n_lex;
                                NIS++;
                                ajouter_decl_fonction(t_dec, n_lex, regarder_top(pile_regions), plus_haute_region + 1);
                                changer_type_fonction(t_desc, type);
                                nombre_arguments = 0;
                                variables_arg = 1;
                                save_deplacement = deplacement;
                                deplacement = 0;
                              }
          liste_arguments { changer_nombre_arguments(t_desc, nombre_arguments, $2);
                            pile_regions = empiler(pile_regions, plus_haute_region + 1);
                            plus_haute_region++;
                            variables_arg = 0;
                            test_existance_fonc(t_dec, numlex_save);
                          }
          PAR_F ACC_O contenu_foncproc ACC_F { ajouter_region(t_reg, taille_region(regarder_top(pile_regions), nombre_arguments) , NIS , $9);
                                               NIS--;
                                               pile_regions = depiler(pile_regions);
                                               deplacement = save_deplacement;
                                               deplacement++;
                                               fprintf(yyout,"Fonction : %s\n",$2); traverse($9,0); fprintf(yyout,"\n\n\n");
                                             }
        ;

dec_proc : T_VOID IDF { n_lex = ajouter_lexeme_table($2, t_lex, t_hash);
                        numlex_save = n_lex;
                        NIS++;
                        ajouter_decl_procedure(t_dec, n_lex, regarder_top(pile_regions), plus_haute_region + 1);
                        changer_type_fonction(t_desc, -2);
                        nombre_arguments = 0;
                        variables_arg = 1;
                        save_deplacement = deplacement;
                        deplacement = 0;
                      }
           PAR_O liste_arguments { changer_nombre_arguments(t_desc, nombre_arguments, $2);
                                   pile_regions = empiler(pile_regions, plus_haute_region + 1);
                                   plus_haute_region++;
                                   variables_arg = 0;
                                   test_existance_proc(t_dec, numlex_save);
                                 }
           PAR_F ACC_O contenu_foncproc ACC_F { ajouter_region(t_reg, taille_region(regarder_top(pile_regions), nombre_arguments) , NIS , $9);
                                                NIS--;
                                                pile_regions = depiler(pile_regions);
                                                deplacement = save_deplacement;
                                                deplacement++;
                                                fprintf(yyout,"Procédure : %s\n",$2); traverse($9,0); fprintf(yyout,"\n\n\n");
                                              }
         ;

type_base : T_INT   { type = $1; }
          | T_FLOAT { type = $1; }
          | T_BOOL  { type = $1; }
          | T_CHAR  { type = $1; }
          ;

liste_arguments : liste_arguments_pleine
                |
                ;
liste_arguments_pleine : dec_var { nombre_arguments++; } VIRG liste_arguments_pleine
                       | dec_var { nombre_arguments++; }
                       ;

contenu_foncproc : dec_type PVIRG contenu_foncproc { $$ = $3; }
                 | dec_var PVIRG contenu_foncproc { $$ = $3; }
                 | liste_instructions { $$ = $1; }
                 | { $$ = arbre_vide(); }
                 ;

liste_instructions : instruction liste_instructions { $$ = concat_father_brother($1 , $2); }
                   | instruction { $$ = concat_father_brother($1 , arbre_vide()); }
                   ;

instruction : affectation PVIRG { $$ = $1; }
            | appel PVIRG { $$ = $1; }
            | appel_sleep PVIRG { $$ = $1; }
            | appel_es PVIRG { $$ = $1; nombre_formats = 0; nombre_arguments = 0; }
            | ifelse { $$ = $1; }
            | while { $$ = $1; }
            | do PVIRG { $$ = $1; }
            | switch { $$ = $1; }
            | for { $$ = $1; }
            | RETURN valeur PVIRG { type_d = type_exp($2);
                                    if(type_d == -1)
                                        yyerror("Le type de l'expression de l'affectation n'est pas correct");
                                    $$ = concat_father_son(creer_noeud(T_RETURN , -1),
                                                           $2);
                                  }
            ;

affectation : variable op_bin { dans_exp = 1; }
              valeur { $$ = concat_father_son($2,
                                              concat_father_brother($1 , $4));
                       dans_exp = 0;
               
                       type_g = type_exp($1);
                       type_d = type_exp($4);
                       
                       if(type_d == -1)
                           yyerror("Le type de la partie droite de l'affectation n'est pas correct");

                       if(type_g == -1)
                           yyerror("Le type de la partie gauche de l'affectation n'est pas correct");

                       if(type_d != type_g){
                           ecrire_type(str_type_g, type_g);
                           ecrire_type(str_type_d, type_d);
                           sprintf(str_erreur, "L'affectation attend un type [%s], valeur récupérée : [%s]", str_type_g, str_type_d);
                           yyerror(str_erreur);
                       }
                     }
            | variable op_inc { type_g = type_exp($1);

                                if(type_g == -1)
                                    yyerror("Le type de la partie gauche de l'incrémentation/décrémentation n'est pas correct");
                   
                                if(type_g != 0 && type_g != 3){
                                    ecrire_type(str_type_g, type_g);
                                    sprintf(str_erreur, "L'incrémentation/décrémentation se fait sur un type [int] ou un type [char], valeur récupérée : [%s]", str_type_g);
                                    yyerror(str_erreur);
                                }
                                
                                $$ = concat_father_son($2,$1);
                              }
            ;

op_bin : OP_AFF                      { $$ = creer_noeud(T_AFF , -1); }
       | OP_LPLUS                    { $$ = creer_noeud(T_LPLUS , -1); }
       | OP_LMOINS                   { $$ = creer_noeud(T_LMOINS , -1); }
       | OP_LMULT                    { $$ = creer_noeud(T_LMULT , -1); }
       | OP_LDIV                     { $$ = creer_noeud(T_LDIV , -1); }
       | OP_LMOD                     { $$ = creer_noeud(T_LMOD , -1); }
       ;
op_inc : OP_INC                      { $$ = creer_noeud(T_INC , -1); }
       | OP_DEC                      { $$ = creer_noeud(T_DEC , -1); }
       ;

valeur : val1 { $$ = $1; }
       | OP_MOINS val1 { $$ = concat_father_son(creer_noeud(T_NEG, -1) , $2); }
       | NOT val1 { $$ = concat_father_son(creer_noeud(T_NOT , -1) , $2); }
       | val1 OP_INC { $$ = concat_father_son(creer_noeud(T_INC , -1) , $1); }
       | val1 OP_DEC { $$ = concat_father_son(creer_noeud(T_DEC , -1) , $1); }
       ;
val1 : val1 OP_INF val2 { $$ = concat_father_son(creer_noeud(T_INF , -1),
                                                 concat_father_brother($1 , $3)); }
     | val1 OP_SUP val2 { $$ = concat_father_son(creer_noeud(T_SUP , -1),
                                                 concat_father_brother($1 , $3)); }
     | val1 OP_INFE val2 { $$ = concat_father_son(creer_noeud(T_INFE , -1),
                                                  concat_father_brother($1 , $3)); }
     | val1 OP_SUPE val2 { $$ = concat_father_son(creer_noeud(T_SUPE , -1),
                                                  concat_father_brother($1 , $3)); }
     | val2 { $$ = $1; }
     ;
val2 : val2 OP_AND val3 { $$ = concat_father_son(creer_noeud(T_AND , -1),
                                                 concat_father_brother($1 , $3)); }
     | val2 OP_OR val3 { $$ = concat_father_son(creer_noeud(T_OR , -1),
                                                concat_father_brother($1 , $3)); }
     | val3 { $$ = $1; } 
     ;
val3 : val3 OP_EQU val4 { $$ = concat_father_son(creer_noeud(T_EQU , -1),
                                                 concat_father_brother($1 , $3)); }
     | val3 OP_NEQ val4 { $$ = concat_father_son(creer_noeud(T_NEQ , -1),
                                                 concat_father_brother($1 , $3)); }
     | val4 { $$ = $1; }
     ;
val4 : val4 OP_PLUS val5 { $$ = concat_father_son(creer_noeud(T_PLUS , -1),
                                                  concat_father_brother($1 , $3)); }
     | val4 OP_MOINS val5 { $$ = concat_father_son(creer_noeud(T_MOINS , -1),
                                                   concat_father_brother($1 , $3)); }
     | val5 { $$ = $1; }
     ;
val5 : val5 OP_MULT val6 { $$ = concat_father_son(creer_noeud(T_MULT , -1),
                                                  concat_father_brother($1 , $3)); }
     | val5 OP_DIV val6 { $$ = concat_father_son(creer_noeud(T_DIV , -1),
                                                 concat_father_brother($1 , $3)); }
     | val5 OP_MOD val6 { $$ = concat_father_son(creer_noeud(T_MOD , -1),
                                                 concat_father_brother($1 , $3)); }
     | val6 { $$ = $1; }
     ;

val6 : PAR_O valeur PAR_F { $$ = $2; }
     | TRUE               { $$ = creer_noeud(T_BCSTE , 1); }
     | FALSE              { $$ = creer_noeud(T_BCSTE , 0); }
     | INT                { $$ = creer_noeud(T_ICSTE , $1); }
     | FLOAT              { n_lex = ajouter_lexeme_table($1, t_lex, t_hash);
                            $$ = creer_noeud(T_FCSTE , n_lex); }
     | CHAR               { n_lex = ajouter_lexeme_table($1, t_lex, t_hash);
                            $$ = creer_noeud(T_CCSTE , n_lex); }
     | variable           { $$ = $1; }
     | appel              { $$ = $1; }
     | rand               { $$ = $1; }
     | abs                { $$ = $1; }
     | fabs               { $$ = $1; }
     | sqrt               { $$ = $1; }
     | pow                { $$ = $1; }
     | exp                { $$ = $1; }
     | sin                { $$ = $1; }
     | cos                { $$ = $1; }
     | tan                { $$ = $1; }
     | asin               { $$ = $1; }
     | acos               { $$ = $1; }
     | atan               { $$ = $1; }
     | log                { $$ = $1; }
     | log10              { $$ = $1; }
     | floor              { $$ = $1; }
     | ceil               { $$ = $1; }
     | itof               { $$ = $1; }
     | ftoi               { $$ = $1; }
     ;

variable : var1 { $$ = $1; }
         | var1 { champ_struct = 1; }
           POINT variable { $$ = concat_father_son(creer_noeud(T_POINT , -1),
                                                   concat_father_brother($1 , $4));
                            champ_struct = 0;
                          }
         ;
var1 : IDF { if(imbrication_struct >= NB_IMBRIC_MAX){
                 sprintf(str_erreur, "Dépassement de l'imbrication maximale d'indices de tableaux (%d)", NB_IMBRIC_MAX);
                 yyerror(str_erreur);
                 exit(EXIT_FAILURE);
             }

             dim_imbriques_recup[imbrication_struct] = 0;
             
             valeur = recuperer_n_lexico(t_lex , t_hash , $1);

             if(champ_struct){
                 if(nature_dec_imbriques[imbrication_struct] != T_STRUCT){
                     sprintf(str_erreur, "L'identificateur '%s' précédant le champ '%s' n'est pas une structure", lex_struct_imbriques[imbrication_struct], $1);
                     yyerror(str_erreur);
                     
                     valeur = -1;
                 }

                 dec_struct_imbriques[imbrication_struct] = existance_champ(t_dec, valeur, dec_struct_imbriques[imbrication_struct]);
                 
                 if(dec_struct_imbriques[imbrication_struct] == -1){
                     sprintf(str_erreur, "La structure '%s' n'a pas de champ tableau '%s'", lex_struct_imbriques[imbrication_struct], $1);
                     yyerror(str_erreur);

                     valeur = -1;
                     dim_imbriques_dec[imbrication_struct] = -1;
                 }else{
                     type_g = t_dec[dec_struct_imbriques[imbrication_struct]].description;

                     dim_imbriques_dec[imbrication_struct] = t_desc[t_dec[dec_struct_imbriques[imbrication_struct]].description];
                     dec_struct_imbriques[imbrication_struct] = t_desc[t_dec[dec_struct_imbriques[imbrication_struct]].description + 1];
                     nature_dec_imbriques[imbrication_struct] = t_dec[dec_struct_imbriques[imbrication_struct]].nature;
                 }
             }else{
                 valeur = association_nom_var(valeur, T_VAR, regarder_top(pile_regions));
                 
                 if(valeur != -1){
                     type_g = t_dec[valeur].description;

                     dec_struct_imbriques[imbrication_struct] = t_desc[t_dec[t_dec[valeur].description].description + 1];
                     dim_imbriques_dec[imbrication_struct] = t_desc[t_dec[t_dec[valeur].description].description];
                     nature_dec_imbriques[imbrication_struct] = t_dec[dec_struct_imbriques[imbrication_struct]].nature;

                     if(type_g >= 0 && type_g <= 3){
                         ecrire_type(str_type_g, type_g);
                         sprintf(str_erreur, "'%s' n'est pas un tableau mais une variable de type [%s]", $1, str_type_g);
                         yyerror(str_erreur);
                     }
                 }else{
                     sprintf(str_erreur, "Le tableau '%s' n'a pas été déclaré", $1);
                     yyerror(str_erreur);

                     dec_struct_imbriques[imbrication_struct] = -1;
                     dim_imbriques_dec[imbrication_struct] = -1;
                     nature_dec_imbriques[imbrication_struct] = -1;
                 }   
             }

             dec_tab_imbriques[imbrication_struct] = valeur;

             tab_champ_struct[imbrication_struct] = champ_struct;
             champ_struct = 0;

             imbrication_struct++;
           }
      var2 { if(dim_imbriques_dec[imbrication_struct - 1] != -1){
                if(dim_imbriques_recup[imbrication_struct - 1] != dim_imbriques_dec[imbrication_struct - 1]){
                    sprintf(str_erreur, "'%s' est un tableau à %d dimension(s), nombre de dimensions récupéré : %d", $1, dim_imbriques_dec[imbrication_struct - 1], dim_imbriques_recup[imbrication_struct - 1]);
                    yyerror(str_erreur);
                 }
             }

             imbrication_struct--;

             sprintf(lex_struct_imbriques[imbrication_struct], "%s", $1);

             champ_struct = tab_champ_struct[imbrication_struct];
             
             $$ = concat_father_son(creer_noeud(T_CRO , -1),
                                    concat_father_brother(creer_noeud(T_IDF , dec_tab_imbriques[imbrication_struct]) ,
                                                          $3));
       }
     | IDF { valeur = recuperer_n_lexico(t_lex , t_hash , $1);

             if(champ_struct){
                 if(existance_champ(t_dec, valeur, dec_struct_imbriques[imbrication_struct]) == -1){
                     sprintf(str_erreur, "La structure '%s' n'a pas de champ '%s'", lex_struct_imbriques[imbrication_struct], $1);
                     yyerror(str_erreur);
                     
                     valeur = -1;
                     dec_struct_imbriques[imbrication_struct] = -1;
                     nature_dec_imbriques[imbrication_struct] = -1;
                 }else if(valeur != -1){
                      if(nature_dec_imbriques[imbrication_struct] != T_STRUCT){
                          sprintf(str_erreur, "L'identificateur '%s' précédant '%s' n'est pas une structure", lex_struct_imbriques[imbrication_struct], $1);
                          yyerror(str_erreur);
                      
                          valeur = -1;
                          dec_struct_imbriques[imbrication_struct] = -1;
                      }
                     
                      nature_dec_imbriques[imbrication_struct] = t_dec[existance_champ(t_dec, valeur, dec_struct_imbriques[imbrication_struct])].nature;
                     
                      if(existance_champ(t_dec, valeur, dec_struct_imbriques[imbrication_struct]) > 3)
                          dec_struct_imbriques[imbrication_struct] = existance_champ(t_dec, valeur, dec_struct_imbriques[imbrication_struct]);
                 }else{
                     dec_struct_imbriques[imbrication_struct] = -1;
                     nature_dec_imbriques[imbrication_struct] = -1;
                 }
             }else{
                 valeur = association_nom_var(valeur, T_VAR, regarder_top(pile_regions));
             
                 if(valeur == -1){
                     sprintf(str_erreur, "La variable '%s' n'a pas été déclarée", $1);
                     yyerror(str_erreur);
                     
                     dec_struct_imbriques[imbrication_struct] = -1;
                     nature_dec_imbriques[imbrication_struct] = -1;
                 }else{
                     dec_struct_imbriques[imbrication_struct] = t_dec[valeur].description;
                     nature_dec_imbriques[imbrication_struct] = t_dec[t_dec[valeur].description].nature;
                 }
             }

             sprintf(lex_struct_imbriques[imbrication_struct], "%s", $1);

             $$ = creer_noeud(T_IDF , valeur);
          }
     ;
var2 : CRO_O valeur CRO_F { dim_imbriques_recup[imbrication_struct - 1]++;
          
                            type_d = type_exp($2);
                            ecrire_type(str_type_d, type_d);
                                 
                            if(type_d != 0 && type_d != -1){
                                sprintf(str_erreur, "Le type de l'expression de la dimension %d du tableau doit être un type [int], type récupéré : [%s]", dim_imbriques_recup[imbrication_struct - 1], str_type_d);
                                yyerror(str_erreur);
                            }else if(type_d == -1){
                                sprintf(str_erreur, "Le type de l'expression de la dimension %d du tableau n'est pas correct", dim_imbriques_recup[imbrication_struct - 1]);
                                yyerror(str_erreur);
                            }
                          }
       var2 { $$ = concat_father_brother($2 , $5); }
     | CRO_O valeur CRO_F { dim_imbriques_recup[imbrication_struct - 1]++;
           
                            type_d = type_exp($2);
                            ecrire_type(str_type_d, type_d);
                                 
                            if(type_d != 0 && type_d != -1){
                                sprintf(str_erreur, "Le type de l'expression de la dimension %d du tableau doit être un type [int], type récupéré : [%s]", dim_imbriques_recup[imbrication_struct - 1], str_type_d);
                                yyerror(str_erreur);
                            }else if(type_d == -1){
                                sprintf(str_erreur, "Le type de l'expression de la dimension %d du tableau n'est pas correct", dim_imbriques_recup[imbrication_struct - 1]);
                                yyerror(str_erreur);
                            }
                                 
                            $$ = $2;
                          }
     ;

rand : RAND PAR_O valeur VIRG valeur PAR_F { $$ = concat_father_son(creer_noeud(T_RAND , -1) , concat_father_brother($3 , $5));
                                             type_g = type_exp($3);
                                             type_d = type_exp($5);
                                             
                                             if(type_g != 0 && type_g != -1){
                                                 ecrire_type(str_type_g, type_g);
                                                 sprintf(str_erreur, "Le 1e paramètre de la fonction 'rand' doit être un type [int], type récupéré : [%s]", str_type_g);
                                                 yyerror(str_erreur);
                                             }else if(type_g == -1)
                                                 yyerror("Le type de l'expression du 1e paramètre de la fonction 'rand' n'est pas correct");
                           
                                             if(type_d != 0 && type_d != -1){
                                                 ecrire_type(str_type_d, type_d);
                                                 sprintf(str_erreur, "Le 2e paramètre de la fonction 'rand' doit être un type [int], type récupéré : [%s]", str_type_d);
                                                 yyerror(str_erreur);
                                             }else if(type_d == -1)
                                                 yyerror("Le type de l'expression du 2e paramètre de la fonction 'rand' n'est pas correct");
                                           }
     ;

abs : ABS PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_ABS, -1) , $3);
                               type_g = type_exp($3);

                               if(type_g != 0 && type_g != -1){
                                   ecrire_type(str_type_g, type_g);
                                   sprintf(str_erreur, "Le paramètre de la fonction 'abs' doit être un type [int], type récupéré : [%s]", str_type_g);
                                   yyerror(str_erreur);
                               }else if(type_g == -1)
                                   yyerror("Le type de l'expression du paramètre de la fonction 'abs' n'est pas correct");
                             }
    ;

fabs : FABS PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_FABS, -1) , $3);
                                 type_g = type_exp($3);

                                 if(type_g != 1 && type_g != -1){
                                     ecrire_type(str_type_g, type_g);
                                     sprintf(str_erreur, "Le paramètre de la fonction 'fabs' doit être un type [float], type récupéré : [%s]", str_type_g);
                                     yyerror(str_erreur);
                                 }else if(type_g == -1)
                                     yyerror("Le type de l'expression du paramètre de la fonction 'fabs' n'est pas correct");
                               }
     ;

sqrt : SQRT PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_SQRT, -1) , $3);
                                 type_g = type_exp($3);

                                 if(type_g != 1 && type_g != -1){
                                     ecrire_type(str_type_g, type_g);
                                     sprintf(str_erreur, "Le paramètre de la fonction 'sqrt' doit être un type [float], type récupéré : [%s]", str_type_g);
                                     yyerror(str_erreur);
                                 }else if(type_g == -1)
                                     yyerror("Le type de l'expression du paramètre de la fonction 'sqrt' n'est pas correct");
                               }
     ;

pow : POW PAR_O valeur VIRG valeur PAR_F { $$ = concat_father_son(creer_noeud(T_POW , -1) , concat_father_brother($3 , $5));
                                           type_g = type_exp($3);
                                           type_d = type_exp($5);
                                             
                                           if(type_g != 1 && type_g != -1){
                                               ecrire_type(str_type_g, type_g);
                                               sprintf(str_erreur, "Le 1e paramètre de la fonction 'pow' doit être un type [float], type récupéré : [%s]", str_type_g);
                                               yyerror(str_erreur);
                                           }else if(type_g == -1)
                                               yyerror("Le type de l'expression du 1e paramètre de la fonction 'pow' n'est pas correct");
                           
                                           if(type_d != 0 && type_d != -1){
                                               ecrire_type(str_type_d, type_d);
                                               sprintf(str_erreur, "Le 2e paramètre de la fonction 'pow' doit être un type [int], type récupéré : [%s]", str_type_d);
                                               yyerror(str_erreur);
                                           }else if(type_d == -1)
                                               yyerror("Le type de l'expression du 2e paramètre de la fonction 'pow' n'est pas correct");
                                         }
    ;

exp : EXP PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_EXP, -1) , $3);
                               type_g = type_exp($3);

                               if(type_g != 1 && type_g != -1){
                                   ecrire_type(str_type_g, type_g);
                                   sprintf(str_erreur, "Le paramètre de la fonction 'exp' doit être un type [float], type récupéré : [%s]", str_type_g);
                                   yyerror(str_erreur);
                               }else if(type_g == -1)
                                   yyerror("Le type de l'expression du paramètre de la fonction 'exp' n'est pas correct");
                             }
    ;

sin : SIN PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_SIN, -1) , $3);
                               type_g = type_exp($3);

                               if(type_g != 1 && type_g != -1){
                                   ecrire_type(str_type_g, type_g);
                                   sprintf(str_erreur, "Le paramètre de la fonction 'sin' doit être un type [float], type récupéré : [%s]", str_type_g);
                                   yyerror(str_erreur);
                               }else if(type_g == -1)
                                   yyerror("Le type de l'expression du paramètre de la fonction 'sin' n'est pas correct");
                             }
    ;

cos : COS PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_COS, -1) , $3);
                               type_g = type_exp($3);

                               if(type_g != 1 && type_g != -1){
                                   ecrire_type(str_type_g, type_g);
                                   sprintf(str_erreur, "Le paramètre de la fonction 'cos' doit être un type [float], type récupéré : [%s]", str_type_g);
                                   yyerror(str_erreur);
                               }else if(type_g == -1)
                                   yyerror("Le type de l'expression du paramètre de la fonction 'cos' n'est pas correct");
                             }
    ;

tan : TAN PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_TAN, -1) , $3);
                               type_g = type_exp($3);

                               if(type_g != 1 && type_g != -1){
                                   ecrire_type(str_type_g, type_g);
                                   sprintf(str_erreur, "Le paramètre de la fonction 'tan' doit être un type [float], type récupéré : [%s]", str_type_g);
                                   yyerror(str_erreur);
                               }else if(type_g == -1)
                                   yyerror("Le type de l'expression du paramètre de la fonction 'tan' n'est pas correct");
                             }
    ;

asin : ASIN PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_ASIN, -1) , $3);
                                 type_g = type_exp($3);

                                 if(type_g != 1 && type_g != -1){
                                     ecrire_type(str_type_g, type_g);
                                     sprintf(str_erreur, "Le paramètre de la fonction 'asin' doit être un type [float], type récupéré : [%s]", str_type_g);
                                     yyerror(str_erreur);
                                 }else if(type_g == -1)
                                     yyerror("Le type de l'expression du paramètre de la fonction 'asin' n'est pas correct");
                               }
    ;

acos : ACOS PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_ACOS, -1) , $3);
                                 type_g = type_exp($3);

                                 if(type_g != 1 && type_g != -1){
                                     ecrire_type(str_type_g, type_g);
                                     sprintf(str_erreur, "Le paramètre de la fonction 'acos' doit être un type [float], type récupéré : [%s]", str_type_g);
                                     yyerror(str_erreur);
                                 }else if(type_g == -1)
                                     yyerror("Le type de l'expression du paramètre de la fonction 'acos' n'est pas correct");
                               }

atan : ATAN PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_ATAN, -1) , $3);
                                 type_g = type_exp($3);

                                 if(type_g != 1 && type_g != -1){
                                     ecrire_type(str_type_g, type_g);
                                     sprintf(str_erreur, "Le paramètre de la fonction 'atan' doit être un type [float], type récupéré : [%s]", str_type_g);
                                     yyerror(str_erreur);
                                 }else if(type_g == -1)
                                     yyerror("Le type de l'expression du paramètre de la fonction 'atan' n'est pas correct");
                               }

log : LOG PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_LOG, -1) , $3);
                               type_g = type_exp($3);

                               if(type_g != 1 && type_g != -1){
                                   ecrire_type(str_type_g, type_g);
                                   sprintf(str_erreur, "Le paramètre de la fonction 'log' doit être un type [float], type récupéré : [%s]", str_type_g);
                                   yyerror(str_erreur);
                               }else if(type_g == -1)
                                   yyerror("Le type de l'expression du paramètre de la fonction 'log' n'est pas correct");
                             }
    ;

log10 : LOG10 PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_LOG10, -1) , $3);
                               type_g = type_exp($3);

                               if(type_g != 1 && type_g != -1){
                                   ecrire_type(str_type_g, type_g);
                                   sprintf(str_erreur, "Le paramètre de la fonction 'log10' doit être un type [float], type récupéré : [%s]", str_type_g);
                                   yyerror(str_erreur);
                               }else if(type_g == -1)
                                   yyerror("Le type de l'expression du paramètre de la fonction 'log10' n'est pas correct");
                             }
      ;

floor : FLOOR PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_FLOOR, -1) , $3);
                                   type_g = type_exp($3);

                                   if(type_g != 1 && type_g != -1){
                                       ecrire_type(str_type_g, type_g);
                                       sprintf(str_erreur, "Le paramètre de la fonction 'floor' doit être un type [float], type récupéré : [%s]", str_type_g);
                                       yyerror(str_erreur);
                                   }else if(type_g == -1)
                                       yyerror("Le type de l'expression du paramètre de la fonction 'floor' n'est pas correct");
                                 }
      ;

ceil : CEIL PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_CEIL, -1) , $3);
                                 type_g = type_exp($3);

                                 if(type_g != 1 && type_g != -1){
                                     ecrire_type(str_type_g, type_g);
                                     sprintf(str_erreur, "Le paramètre de la fonction 'ceil' doit être un type [float], type récupéré : [%s]", str_type_g);
                                     yyerror(str_erreur);
                                 }else if(type_g == -1)
                                     yyerror("Le type de l'expression du paramètre de la fonction 'ceil' n'est pas correct");
                               }
     ;

itof : ITOF PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_ITOF, -1) , $3);
                                 type_g = type_exp($3);

                                 if(type_g != 0 && type_g != -1){
                                     ecrire_type(str_type_g, type_g);
                                     sprintf(str_erreur, "Le paramètre de la fonction 'itof' doit être un type [int], type récupéré : [%s]", str_type_g);
                                     yyerror(str_erreur);
                                 }else if(type_g == -1)
                                     yyerror("Le type de l'expression du paramètre de la fonction 'itof' n'est pas correct");
                               }
     ;

ftoi : FTOI PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_FTOI, -1) , $3);
                                 type_g = type_exp($3);

                                 if(type_g != 1 && type_g != -1){
                                     ecrire_type(str_type_g, type_g);
                                     sprintf(str_erreur, "Le paramètre de la fonction 'ftoi' doit être un type [float], type récupéré : [%s]", str_type_g);
                                     yyerror(str_erreur);
                                 }else if(type_g == -1)
                                     yyerror("Le type de l'expression du paramètre de la fonction 'ftoi' n'est pas correct");
                               }
     ;

appel_sleep : SLEEP PAR_O valeur PAR_F { $$ = concat_father_son(creer_noeud(T_SLEEP, -1) , $3);
                                   type_g = type_exp($3);

                                   if(type_g != 0 && type_g != -1){
                                       ecrire_type(str_type_g, type_g);
                                       sprintf(str_erreur, "Le paramètre de la fonction 'sleep' doit être un type [int], type récupéré : [%s]", str_type_g);
                                       yyerror(str_erreur);
                                   }else if(type_g == -1)
                                       yyerror("Le type de l'expression du paramètre de la fonction 'sleep' n'est pas correct");
                                 }
      ;

appel : IDF PAR_O { imbrication_fct++;
                    if(imbrication_fct >= NB_IMBRIC_MAX){
                        sprintf(str_erreur, "Dépassement de l'imbrication maximale d'appels de fonctions/procédures (%d) lors de l'appel de la fontion/procédure '%s'", NB_IMBRIC_MAX, $1);
                        yyerror(str_erreur);
                        exit(EXIT_FAILURE);
                    }
                    
                    args_appel_imbriques[imbrication_fct] = 0;
                    initialiser_types_params(type_params_imbriques[imbrication_fct]);
                  }
        liste_arguments_appel PAR_F { valeur = association_nom_fct(type_params_imbriques[imbrication_fct],
                                                                   recuperer_n_lexico(t_lex , t_hash , $1) ,
                                                                   args_appel_imbriques[imbrication_fct]);
                                      
                                      if(valeur == -1){
                                          sprintf(str_erreur, "La fonction/procédure '%s' n'existe pas ou alors pas avec ces paramètres", $1);
                                          yyerror(str_erreur);
                                      }else if(t_dec[valeur].execution == plus_haute_region){
                                          sprintf(str_erreur, "Impossible d'appeler la fonction/procédure '%s' récursivement", $1);
                                          yyerror(str_erreur);
                                      }else if(t_dec[valeur].nature == T_PROCEDURE && dans_exp){
                                          sprintf(str_erreur, "'%s' est une procédure et n'a donc pas de valeur de retour", $1);
                                          yyerror(str_erreur);
                                      }
                                      
                                      $$ = concat_father_son(creer_noeud(T_APPEL , valeur), $4);
                                      
                                      imbrication_fct--;
                                    }
      ;
liste_arguments_appel : liste_arguments_appel_pleine { $$ = $1; }
                      | { $$ = arbre_vide(); }
                      ;
liste_arguments_appel_pleine : valeur { changer_type_argument(type_params_imbriques[imbrication_fct],
                                                              type_exp($1),
                                                              args_appel_imbriques[imbrication_fct]);
                                        args_appel_imbriques[imbrication_fct]++;
                                      }
                               VIRG liste_arguments_appel_pleine { $$ = concat_father_brother($1 , $4); }
                             | valeur { changer_type_argument(type_params_imbriques[imbrication_fct],
                                                              type_exp($1),
                                                              args_appel_imbriques[imbrication_fct]);
                                        args_appel_imbriques[imbrication_fct]++;
                                        
                                        $$ = $1;
                                      }
                             ;


appel_es : SCANF PAR_O GUILLEMET chaine { n_lex_es = ajouter_lexeme_table($4, t_lex, t_hash); }
           GUILLEMET liste_arguments_appel_es PAR_F { if(nombre_args_appel_es < nombre_formats){
                                                          sprintf(str_erreur, "La fonction 'scanf' attend %d valeur(s) en argument, %d valeur(s) récupérée(s)", nombre_formats, nombre_args_appel_es);
                                                          yyerror(str_erreur);
                                                      }
               
                                                      $$ = concat_father_son(creer_noeud(T_SCANF , -1),
                                                                             concat_father_brother(creer_noeud(T_CHAINE, n_lex_es), $7));
                                                    }
         | PRINTF PAR_O GUILLEMET chaine { n_lex_es = ajouter_lexeme_table($4, t_lex, t_hash); }
           GUILLEMET liste_arguments_appel_es PAR_F { if(nombre_args_appel_es < nombre_formats){
                                                          sprintf(str_erreur, "La fonction 'printf' attend %d valeur(s) en argument, %d valeur(s) récupérée(s)", nombre_formats, nombre_args_appel_es);
                                                          yyerror(str_erreur);
                                                      }
               
                                                      $$ = concat_father_son(creer_noeud(T_PRINTF , -1),
                                                                             concat_father_brother(creer_noeud(T_CHAINE, n_lex_es), $7));
                                                    }
         ;
chaine : { nombre_formats = 0; } suite_mots { $$ = strdup($2); nombre_args_appel_es = 0; }
       | { nombre_formats = 0; $$ = strdup(""); nombre_args_appel_es = 0; }
       ;
suite_mots : mot suite_mots { $$ = strdup(strcat($1,$2)); }
           | mot { $$ = strdup($1); }
           ;
mot : MOT { $$ = strdup($1); }
    | format { $$ = strdup($1); }
    ;
format : POURCENT_I { $$ = strdup("%i"); args_appel_es[nombre_formats] = 0; nombre_formats++; }
       | POURCENT_F { $$ = strdup("%f"); args_appel_es[nombre_formats] = 1; nombre_formats++; }
       | POURCENT_B { $$ = strdup("%b"); args_appel_es[nombre_formats] = 2; nombre_formats++; }
       | POURCENT_C { $$ = strdup("%c"); args_appel_es[nombre_formats] = 3; nombre_formats++; }
       ;
liste_arguments_appel_es : VIRG valeur liste_arguments_appel_es { if(nombre_args_appel_es == nombre_formats){
                                                                      sprintf(str_erreur, "La fonction 'printf'/'scanf' n'attend que %d argument(s)", nombre_formats);
                                                                      yyerror(str_erreur);
                                                                   }else{
                                                                       type_g = type_exp($2);
                                                                       ecrire_type(str_type_g, type_g);

                                                                       if(type_g == -1){
                                                                           sprintf(str_erreur, "Le type du %de argument de la fonction 'printf'/'scanf' n'est pas correct", nombre_args_appel_es + 1);
                                                                           yyerror(str_erreur);
                                                                       }else if(type_g != args_appel_es[nombre_formats - nombre_args_appel_es - 1] && nombre_args_appel_es <= nombre_formats){
                                                                           ecrire_type(str_type_d, args_appel_es[nombre_args_appel_es]);
                                                                           sprintf(str_erreur, "Le %de argument de la fonction 'printf'/'scanf' doit être un type [%s], type récupéré : [%s]", nombre_args_appel_es + 1, str_type_d, str_type_g);   
                                                                           yyerror(str_erreur);
                                                                       }
 
                                                                       nombre_args_appel_es++;
                                                                     
                                                                       $$ = concat_father_brother(concat_father_son(creer_noeud(T_VARIABLE , -1),
                                                                                                                    $2),
                                                                                                  $3);
                                                                   }
                                                                 }
                         | { $$ = arbre_vide(); }
                         ;

ifelse : IF PAR_O valeur { type_g = type_exp($3);
                           if(type_g == -1)
                               yyerror("Le type de l'expression de la condition 'if' n'est pas correct");
                         }
         PAR_F ACC_O liste_instructions ACC_F else { $$ = concat_father_son(creer_noeud(T_IF , -1),
                                                                            concat_father_brother($3,
                                                                                                  concat_father_brother(concat_father_son(creer_noeud(T_INSTR,-1),$7),
                                                                                                                        concat_father_son(creer_noeud(T_ELSE,-1),$9))));
                                                   }
       ;
else : ELSE ifelse { $$ = $2; }
     | ELSE ACC_O liste_instructions ACC_F { $$ = $3; }
     | { $$ = arbre_vide(); }
     ;

while : WHILE PAR_O valeur { type_g = type_exp($3);
                             if(type_g == -1)
                                 yyerror("Le type de l'expression de la condition de la boucle 'while' n'est pas correct");
                           }
        PAR_F ACC_O liste_instructions ACC_F { $$ = concat_father_son(creer_noeud(T_WHILE , -1),
                                                                      concat_father_brother($3 ,
                                                                                            concat_father_son(creer_noeud(T_INSTR,-1) ,$7)));
                                             }
      ;

do : DO ACC_O liste_instructions ACC_F WHILE PAR_O valeur { type_g = type_exp($7);
                                                            if(type_g == -1)
                                                                yyerror("Le type de l'expression de la condition de la boucle 'do' n'est pas correct");
                                                          }
     PAR_F { $$ = concat_father_son(creer_noeud(T_DO , -1),
                                    concat_father_brother(concat_father_son(creer_noeud(T_INSTR,-1) ,$3) ,
                                                                            concat_father_son(creer_noeud(T_WHILE,-1) , $7)));
           }
   ;

switch : SWITCH PAR_O valeur { imbrication_switch++;
                               if(imbrication_switch >= NB_IMBRIC_MAX){
                                   sprintf(str_erreur, "Dépassement de l'imbrication maximale de 'switch' (%d)", NB_IMBRIC_MAX);
                                   yyerror(str_erreur);
                                   exit(EXIT_FAILURE);
                               }
                               
                               types_switch[imbrication_switch] = type_exp($3);
                               ecrire_type(str_type_g, types_switch[imbrication_switch]);
                               
                               if(types_switch[imbrication_switch] == 1 || types_switch[imbrication_switch] == 2){
                                   sprintf(str_erreur, "Un 'switch' doit s'effectuer sur un type [int] ou [char], type récupéré : [%s]", str_type_g);
                                   yyerror(str_erreur);
                               }else if(types_switch[imbrication_switch] == -1)
                                   yyerror("Le type de l'expression de l'évaluation du 'switch' n'est pas correct");
                             }
         PAR_F ACC_O liste_case ACC_F { $$ = concat_father_son(creer_noeud(T_SWITCH , -1),
                                                               concat_father_brother(concat_father_son(creer_noeud(T_VARIABLE,-1) ,
                                                                                                       $3) ,
                                                                                     $7));
                                        imbrication_switch--;
                                      }
       ;

liste_case : CASE INT DPOINTS liste_instructions BREAK PVIRG liste_case { if(types_switch[imbrication_switch] != 0){
                                                                              ecrire_type(str_type_g, types_switch[imbrication_switch]);
                                                                              sprintf(str_erreur, "Le 'switch' courant se fait sur un type [%s], type récupéré : [int]", str_type_g);
                                                                              yyerror(str_erreur);
                                                                          }
             
                                                                          $$ = concat_father_brother(concat_father_son(creer_noeud(T_CASE , $2),
                                                                                                                       $4),
                                                                                                     $7);
                                                                        }
           | CASE CHAR DPOINTS liste_instructions BREAK PVIRG liste_case { if(types_switch[imbrication_switch] != 3){
                                                                               ecrire_type(str_type_g, types_switch[imbrication_switch]);
                                                                               sprintf(str_erreur, "Le 'switch' courant se fait sur un type [%s], type récupéré : [char]", str_type_g);
                                                                               yyerror(str_erreur);
                                                                           }
             
                                                                           n_lex = ajouter_lexeme_table($2, t_lex, t_hash);
                                                                           $$ = concat_father_brother(concat_father_son(creer_noeud(T_CASE , n_lex) ,
                                                                                                                        $4),
                                                                                                      $7);
                                                                         }
           | DEFAULT DPOINTS liste_instructions  { $$ = concat_father_son(creer_noeud(T_DEFAULT , -1) , $3);}
           | { $$ = arbre_vide(); }
           ;

for : FOR PAR_O affectation PVIRG valeur { type_g = type_exp($5);
                                           if(type_g == -1){
                                               yyerror("Le type de l'expression de l'évaluation de la boucle 'for' n'est pas correct");
                                           }
                                         }
      PVIRG affectation PAR_F ACC_O liste_instructions ACC_F { $$ = concat_father_son(creer_noeud(T_FOR , -1),
                                                               concat_father_brother(concat_father_son(creer_noeud(T_INSTR,-1) , $3) ,
                                                                                                       concat_father_brother($5 ,
                                                                                                                             concat_father_brother(concat_father_son(creer_noeud(T_INSTR,-1) , $8) ,
                                                                                                                                                   concat_father_son(creer_noeud(T_INSTR,-1) , $11)))));
                                                             }
    ;

fonction_main : MAIN { ajouter_region(t_reg, taille_region(0, 0), 0, NULL);
                       NIS++;
                       n_lex = ajouter_lexeme_table("main", t_lex, t_hash);
                       ajouter_decl_procedure(t_dec, n_lex, regarder_top(pile_regions), plus_haute_region + 1);
                       changer_type_fonction(t_desc , -2);
                       pile_regions = empiler(pile_regions, plus_haute_region + 1);
                       plus_haute_region++;
                     }
                PAR_O PAR_F { changer_nombre_arguments(t_desc, 0, "main"); deplacement = 0; }
                ACC_O contenu_foncproc ACC_F { ajouter_region(t_reg , taille_region(regarder_top(pile_regions), 0) , NIS , $7);
                                               NIS--;
                                               pile_regions = depiler(pile_regions);
                                               fprintf(yyout,"Main :\n"); traverse($7,0); fprintf(yyout,"\n\n\n");
                                             }
              ;

%%

/*********************************************************/
/*********************************************************/

void yyerror (char* const s)
{
    fprintf(stdout, "Ligne %d caractère %d : %s\n", nb_lignes, nb_char, s);
    erreur = 1;
}

int main (int argc, char *argv[], char *envp[])
{
    Cellule_pile_exec res;

    srand(getpid());

    if(argc == 1 || argc >= 4)
    {
        fprintf(stderr , "Usage : %s <fic_src> [<fic_out>]\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    yyin = fopen(argv[1], "r") ;

    if(yyin == NULL)
    {
        fprintf(stderr , "Erreur d'ouverture du fichier source '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    if(argc == 3)
        yyout = fopen(argv[2], "w+");
    else
        yyout = fopen("log", "w+");

    if(yyout == NULL)
    {
        fprintf(stderr , "Erreur d'ouverture du fichier de sortie '%s'\n", (argc == 3)?argv[2]:"log");
        return EXIT_FAILURE;
    }
    
    /* Création de la table de hachage */
    t_hash = creer_table_hachage();

    /* Création de la table lexicographique */
    t_lex = creer_table_lexico(t_hash);

    /* Création de la table des déclarations */
    t_dec = creer_table_declarations();

    /* Création de la table de description des types */
    t_desc = creer_table_description();

    /* Création de la pile des régions */
    pile_regions = creer_pile(plus_haute_region);

    /* Creation de la table des regions */
    t_reg = creer_table_region();

    /* Creation de la pile à l'exécution */
    p = creer_pile_execution();



    /* Analyse lexicale, syntaxique et sémantique */
    yyparse();

    
    
    /* Exécution si aucune erreur de compilation */
    if(!erreur)
    {
        afficher_table_lexicographique(t_lex);
        fprintf(yyout, "\n\n");

        afficher_table_declarations(t_dec);
        fprintf(yyout, "\n\n");

        afficher_table_description(t_desc);
        fprintf(yyout, "\n\n");

        afficher_table_region(t_reg);
        fprintf(yyout,"\n\n");

        /* Ajout de la region 0 dans la pile d'execution */
        ajouter_pile(p, 0);

        chainage_dynamique = 0;

        /* Ajout du main dans la pile d'execution */
        base_courante = ajouter_pile(p, plus_haute_region);

        /* Evaluation du programme en partant du main */
        evaluer_arbre(plus_haute_region ,
                      t_reg[plus_haute_region].arbre ,
                      &res);

        afficher_pile_execution(p);
        fprintf(yyout,"\n\n");

        liberer_pile(pile_regions);
    }

    /* Libération de la mémoire allouée à la compilation */
    detruire_table_hachage(t_hash);
    detruire_table_lexicographique(t_lex);
    detruire_table_declarations(t_dec);
    detruire_table_description(t_desc);
    detruire_arbres_regions(t_reg);
    detruire_table_regions(t_reg);
    detruire_pile_execution(p);
    
    return EXIT_SUCCESS;
}
