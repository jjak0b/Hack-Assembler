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
/*
typedef struct symbol{
	char *key;
	int value;
} symbol;

	symbol s_table0[] = {
		{ .key = "R0", .value = 0 },
		{ .key = "R1", .value = 1 },
		{ .key = "R2", .value = 2 },
		{ .key = "R3", .value = 3 },
		{ .key = "R4", .value = 4 },
		{ .key = "R5", .value = 5 },
		{ .key = "R6", .value = 6 },
		{ .key = "R7", .value = 7 },
		{ .key = "R8", .value = 8 },
		{ .key = "R9", .value = 9 },
		{ .key = "R10", .value = 10 },
		{ .key = "R11", .value = 11 },
		{ .key = "R12", .value = 12 },
		{ .key = "R13", .value = 13 },
		{ .key = "R14", .value = 14 },
		{ .key = "R15", .value = 15 },
		{ .key = "SCREEN", .value = 16384 },
		{ .key = "KBD", .value = 24576 },
		{ .key = "SP", .value = 0 },
		{ .key = "LCL", .value = 1 },
		{ .key = "ARG", .value = 2 },
		{ .key = "THIS", .value = 3 },
		{ .key = "THAT", .value = 4 }
	};

	list_handler s_table = {
		.value = { .key = "R0", .value = 0 },
		.next = {
			.value = { .key = "R1", .value = 1 },
			.next = {
				.value = { .key = "R0", .value = 0 },
			}
		}
	};*/

list_handler *assembler( list_handler *lh_input ){	
	list_handler *lh_output = NULL;
	list_node *node_current = lh_input->head;
	while( node_current != NULL ){
		// do stuff
		node_current = next(lh_input);
	}
	return lh_output;
}

int main( int nArgs, char **args ){
	if( nArgs > 1 && nArgs < 3){
		char *filename = args[1];
		char *ptr_char = NULL; // usato temporaneamente
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
						lh_output = lh_input; // testing copia contenuto
						printf("file '%s' letto con successo\n", filename);
						printf("caratteri letti: %d\n", size( lh_output->head, true ) );
						
						// lh_output = assembler( lh_input ); // elabora il contenuto del file, restituendo il contenuto da scrivere su file
						list_node_print( "%c", lh_output->head );

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

