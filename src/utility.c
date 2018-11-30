#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_char.h"

bool strEndWith( char *str, char *str_suffix){

	int length_str = strlen( str ) - 1;
	int length_suffix = strlen( str_suffix ) - 1;
	int isRight = true;

	while( length_suffix >= 0 && length_str >= 0 && isRight){
		if( str[ length_str ] != str_suffix[ length_suffix ] ){
			isRight = false;
		}
		length_str -= 1;
		length_suffix -= 1;
	}
	return isRight;
}

/**
 * @brief
 * PreCondition: N/A
 * PostCondition: dato il nome di un file.estensione come stringa e un list_c_handler,
 * inserisce nella coda i caratteri letti dal file specificato
 * @param *filename 
 * @param *list_handler
 * @return NULL se è impossbile aprire il file, un handler della nuova lista altrimenti
 */
list_c_handler *readFile( char *filename, list_c_handler *list_handler ){
    FILE *fin;
    char c = '\0';
    fin = fopen( filename, "r");
    if( fin != NULL){
        while( !feof( fin ) ){
			if( fscanf( fin, "%c", &c) == 1 ){
				printf("c_:%c\n", c);
				list_handler = enqueue( list_handler, c );
			}
        }
        fclose( fin );
    }
    else{
        return NULL;
    }

    return list_handler;
}

/**
 * @brief 
 * PreCondition: N/A
 * PostCondition: Dato il nome di un file.estensione come stringa e un list_c_handler,
 * scrive gli elementi della lista in un file con il nome indicato ( se non esite, viene creato )
 * @param filename 
 * @param list_handler 
 * @return int 0 se è impossbile aprire/scrivere il file, 1 altrimenti
 */
bool writeFile( char *filename, list_c_handler *list_handler ){
	FILE *fout;
    fout = fopen( filename, "w");
    if( fout != NULL){
		list_handler->current = list_handler->head;
		node_char *current = list_handler->current;
        while( !feof( fout ) && current != NULL){
			fprintf( fout, "%c", current->value );
			current = next( list_handler);
        }
        fclose( fout );
    }
    else{
        return false;
    }
    return true;
}

void *replaceFilenameExtension( char filename[], int size_filename, const char extension[] ){
	int length = strnlen( filename, size_filename );
	int i = length ;
	bool b_quit = false;
	while( i >= 0 && !b_quit){
		if( filename[ i ] == '.' ){
			filename[ i ] = '\0';
			b_quit = true;			
		}		
		i-= 1;
	}

	strncat( filename, extension, strlen(extension) );
}