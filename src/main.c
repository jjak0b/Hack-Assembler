#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "list.h"
#include <string.h>

#define FILE_INPUT_EXTENSION ".asm" // estensione personalizzabile
#ifndef FILE_INPUT_EXTENSION
#define FILE_INPUT_EXTENSION ".asm"
#endif
#define FILE_OUTPUT_EXTENSION ".hack" // estensione personalizzabile
#ifndef FILE_OUTPUT_EXTENSION
#define FILE_OUTPUT_EXTENSION ".hack"
#endif

typedef struct symbol{
	char *key;
	int value;
} symbol;

symbol *new_symbol( char *key, int value){
	symbol *s = malloc(sizeof( symbol ) );
	s->key = key;
	s->value = value;
	return s;
}

void print_symbols( list_node *head ){
	symbol *s = NULL;
	if( head != NULL ){
		s = (symbol*)head->value;
		if( s != NULL ){
			printf("key: '%s'\tvalue: '%d'\n", s->key, s->value);
		}
		print_symbols( head->next );
	}
	else{
		printf("\n");
	}
}

list_handler *init_default_symbol_table( list_handler *lh_symbol_table ){
	lh_symbol_table = NULL;
	symbol *s;
	char reg[4];
	char *str_RN;
	char str_N[3];
	// crea le etichette dedicate ai registri
	const int size_str_RN = 4;
	for( int i = 0; i < 16; i+= 1){
		str_RN = (char*)malloc(sizeof(char) * size_str_RN);
		strncpy( str_RN, "R", 2 );
		int_toString( i, str_N, size_str_RN-1 ); // converte il valore di i da numero ad una stringa, e la inserisce in str_N
		strncat( str_RN, str_N, size_str_RN-1);// Ri
		lh_symbol_table = enqueue( lh_symbol_table, new_symbol( str_RN, i ) );
	}
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "SCREEN", 16384 ) );
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "KBD", 24576 ) );
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "SP", 0 ) );
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "LCL", 1 ) );
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "ARG", 2 ) );
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "THIS", 3 ) );
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "THAT", 4 ) );
	
	return lh_symbol_table;
}

list_handler *assembler( list_handler *lh_input ){	
	list_handler *lh_symbol_table = NULL;
	lh_symbol_table = init_default_symbol_table( lh_symbol_table );
	print_symbols( lh_symbol_table->head );
	list_handler *lh_output = lh_input;// tmp
	/*
	list_handler *lh_output = NULL;
	list_node *node_current = lh_input->head;
	while( node_current != NULL ){
		// do stuff
		node_current = next(lh_input);
	}*/
	return lh_output;
}


int main( int nArgs, char **args ){

	if( nArgs > 1 && nArgs < 3){
		char *ptr_char = NULL; // usato temporaneamente
		char *filename = args[1];		
		list_handler *lh_input = NULL, *lh_output = NULL;
		
		if( filename == NULL ){
			printf("ERRORE: File input non speficato\n");
		}
		else{
			if( !strEndWith( filename, FILE_INPUT_EXTENSION ) ){
				printf("ERRORE: Estensione file non supportata\n");
			}
			else{
				lh_input = readFile( filename, lh_input );
				if( lh_input == NULL ){
					printf("ERRORE: Impossbile aprire il file '%s'\n", filename );
				}
				else if( lh_input->head != NULL ){
					ptr_char = (char*)lh_input->head->value;
					if( *ptr_char == '\0' ){
						printf("ERRORE: Il file '%s' risulta vuoto\n", filename );	
					}
					else{
						printf("file '%s' letto con successo\n", filename);
						
						#ifdef DEBUG
						printf("caratteri letti: %d\n", size(  lh_input->head, true ) );
						list_node_print( "%c", lh_input->head );
						#endif

						lh_output = assembler( lh_input ); // elabora il contenuto del file, restituendo il contenuto da scrivere su file

						#ifdef DEBUG
						printf("caratteri elaborati: %d\n", size(  lh_output->head, true ) );
						list_node_print( "%c", lh_output->head );
						#endif

						int length_estension = strlen( FILE_OUTPUT_EXTENSION );
						int length_filename = strlen( filename );
						int length_FilenameOut = length_filename + length_estension; // la dimensione non è proprio ottimizzata ma sicuramente non andrà outofbound
						char *filename_out = ( char* ) malloc(sizeof(char) * ( length_FilenameOut + 1 ) );
						strncpy( filename_out, filename, length_FilenameOut );
						replaceFilenameExtension( filename_out, length_filename, FILE_OUTPUT_EXTENSION );
						printf("Scrittura dell'elaborazione su file '%s' in corso...\n", filename_out);
						if( writeFile( filename_out, lh_input) ){
							printf("Scrittura sul file '%s' avvenuta con successo\n", filename_out );
						}
						else{
							printf("ERRORE: Impossibile aprire o scrivere sul file '%s'\n", filename_out );
						}
					}
				}
			}
		}
	}
	else{
		printf("ERRORE: File input mancante, numero parametri trovati: %d\n", nArgs);
	}

	return 0;
}

