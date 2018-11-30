#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "list_char.h"
#include <string.h>

#define FILE_INPUT_EXTENSION ".asm" // estensione personalizzabile
#ifndef FILE_INPUT_EXTENSION
#define FILE_INPUT_EXTENSION ".asm"
#endif
#define FILE_OUTPUT_EXTENSION ".hack" // estensione personalizzabile
#ifndef FILE_OUTPUT_EXTENSION
#define FILE_OUTPUT_EXTENSION ".hack"
#endif

list_c_handler *assembler( list_c_handler *lh_input ){
	list_c_handler *lh_output = NULL;
	node_char *node_current = lh_input->head;
	while( node_current != NULL ){
		// do stuff
		node_current = next(lh_input);
	}
	return lh_output;
}

int main( int nArgs, char **args ){

	if( nArgs > 1 && nArgs < 3){
		char *filename = args[1];
		list_c_handler *lh_input = NULL, *lh_output = NULL;
		
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
				else if( lh_input->head != NULL && lh_input->head->value == '\0'){
					printf("ERRORE: Il file '%s' risulta vuoto\n", filename );
				}
				else{
					lh_output = lh_input; // testing copia contenuto
					printf("file '%s' letto\n", filename);
					printf("caratteri letti: %d\n", size( lh_output->head, true ) );
					
					// lh_output = assembler( lh_input ); // elabora il contenuto del file, restituendo il contenuto da scrivere su file
					node_char_print( lh_output->head );


					int length_estension = strlen( FILE_OUTPUT_EXTENSION );
					int length_filename = strlen( filename );
					int length_FilenameOut = length_filename + length_estension; // la dimensione non è proprio ottimizzata ma sicuramente non andrà outofbound
					char *filename_out = ( char* ) malloc(sizeof(char) * ( length_FilenameOut + 1 ) );
					strncpy( filename_out, filename, length_FilenameOut );
					replaceFilenameExtension( filename_out, length_filename, FILE_OUTPUT_EXTENSION );
					printf("Scrittura elaborazione su file '%s' in corso...\n", filename_out);
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
	else{
		printf("ERRORE: File input mancante, paramteri trovati: %d\n", nArgs);
	}

	return 0;
}

