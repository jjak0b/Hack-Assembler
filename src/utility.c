#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

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
 * PostCondition: dato il nome di un file.estensione come stringa e un list_handler,
 * inserisce nella coda i caratteri letti dal file specificato
 * @param *filename 
 * @param *list_handler
 * @return NULL se è impossbile aprire il file, un handler della nuova lista altrimenti
 */
list_handler *readFile( char *filename, list_handler *l_handler ){
    FILE *fin;
    char c = '\0';
	char *ptr_char = NULL;
    fin = fopen( filename, "r");
    if( fin != NULL){
        while( !feof( fin ) ){
			if( fscanf( fin, "%c", &c) == 1 ){
				#ifdef DEBUG
				printf("letto: '%c' -> ASCII: %d\n", c, (int)c);
				#endif				
				ptr_char = (char*)malloc( sizeof(char) );
				*ptr_char = c;
				l_handler = enqueue( l_handler, ptr_char );
			}
        }
        fclose( fin );
    }
    else{
        return NULL;
    }

    return l_handler;
}

/**
 * @brief 
 * PreCondition: I 'value' puntati dai 'list_node' della lista gestita da 'list_handler', devono essere convertibili a char
 * PostCondition: Dato il nome di un file.estensione come stringa e un list_handler,
 * scrive gli elementi della lista in un file con il nome indicato ( se non esite, viene creato )
 * @param filename 
 * @param l_handler 
 * @return int 0 se è impossbile aprire/scrivere il file, 1 altrimenti
 */
bool writeFile( char *filename, list_handler *l_handler ){
	FILE *fout;
	char *ptr_char = NULL;
    fout = fopen( filename, "w");
    if( fout != NULL){
		l_handler->current = l_handler->head;
		list_node *current = l_handler->current;
        while( !feof( fout ) && current != NULL){
			ptr_char = current->value;
			fprintf( fout, "%c", *ptr_char );
			current = next( l_handler);
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

/**
 * @brief 
 * PreCondition: 	destination_size >= 2
 * 				 	se numero di caratteri di value > destination_size allora verranno inserite solo le prime 'destination_size-1' cifre meno significative nel buffer
 * 					poichè verrà inserito il carattere '\0' alla posizione destination_size-1
 *						es int_toString( 1234, destination, 4 ) -> destination = "234".
 * PostCondition: 	Inserisce nell buffer destination le cifre del valore passato come caratteri
 * @param value 
 * @param destination 
 * @param destination_size 
 * @return true  se sono state inserite nel buffer tutte le cifre di value
 * @return false se numero di caratteri di value > destination_size 
 * 					
 */
bool int_toString( int value, char *destination, int destination_size ){
	if( destination_size  < 1){
		return false;
	}
	else if( destination_size < 2){
		destination[0] = '\0';
		return false;
	}
	else{
		int state = true;
		if( value / 10 ==  0 ){ // value ha una cifra
			destination[0] = (char)(48+value);
			destination[1] = '\0';
		}
		else{
			state = int_toString( value / 10 , destination, destination_size - 1 );
			 // concateno la/le cifra/e precedenti con quella appena estratta
			char tmp[2];
			int ultima_cifra = value % 10;
			tmp[ 0 ] = (char)(48+ultima_cifra);
			tmp[ 1 ] = '\0';
			strncat( destination, tmp, destination_size );
		}
		return state;
	}
}