CC = gcc
OPTIONS = -Wall -O3 -g
MATH = -lm

MiniCC : y.tab.c lex.yy.o association.o table_lexicographique.o table_description_types.o table_declarations.o arbre_binaire.o pile_regions.o  table_regions.o pile_execution.o machine_virtuelle.o controle_types.o
	$(CC) $(OPTIONS) y.tab.c association.o table_lexicographique.o table_description_types.o table_declarations.o arbre_binaire.o pile_regions.o table_regions.o pile_execution.o machine_virtuelle.o controle_types.o lex.yy.o -ly -ll -o MiniCC $(MATH)


y.tab.c : grammar/a_syntaxique.y
	yacc -v -d -t grammar/a_syntaxique.y

lex.yy.o : lex.yy.c
	$(CC) -O3 -g -c lex.yy.c

lex.yy.c : grammar/a_lexical.l
	lex grammar/a_lexical.l


table_lexicographique.o : table_lexicographique.c inc/table_lexicographique.h
	$(CC) $(OPTIONS) table_lexicographique.c -c

table_declarations.o : table_declarations.c inc/table_declarations.h
	$(CC) $(OPTIONS) table_declarations.c -c

table_description_types.o : table_description_types.c inc/table_description_types.h
	$(CC) $(OPTIONS) table_description_types.c -c

arbre_binaire.o : arbre_binaire.c inc/arbre_binaire.h
	$(CC) $(OPTIONS) arbre_binaire.c -c

pile_regions.o : pile_regions.c inc/pile_regions.h
	$(CC) $(OPTIONS) pile_regions.c -c

table_regions.o : table_regions.c inc/table_regions.h
	$(CC) $(OPTIONS) table_regions.c -c

association.o : association.c inc/association.h
	$(CC) $(OPTIONS) association.c -c

pile_execution.o : pile_execution.c inc/pile_execution.h
	$(CC) $(OPTIONS) pile_execution.c -c

machine_virtuelle.o : machine_virtuelle.c inc/machine_virtuelle.h
	$(CC) $(OPTIONS) machine_virtuelle.c -c $(MATH)

controle_types.o : controle_types.c inc/controle_types.h
	$(CC) $(OPTIONS) controle_types.c -c


clean :
	rm MiniCC log lex.yy.c y.output y.tab.c y.tab.h *.o
