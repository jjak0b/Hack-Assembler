#include <stdio.h>
#include <stdlib.h>
#include <cstdlib.h>
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

symbol *getFromSymbolTable( list_handler *lh_symbol_table, char *str_label ){
	if( lh_symbol_table == NULL ){
		return NULL;
	}
	else if( str_label == NULL ){
		return NULL;
	}

	list_node *node_symbol_table = lh_symbol_table->head;
	symbol *s = (symbol*)node_symbol_table->value;
	symbol *s_replace = NULL;

	bool b_found_known = false;
	while( node_symbol_table != NULL && !b_found_known ){
		s = (symbol*)node_symbol_table->value;
		if( !strcmp( s->key, str_label ) ){
			s_replace = s;
			b_found_known = true;
		}
		node_symbol_table = node_symbol_table->next;
	}

	return s_replace;
}

/*
Cosa deve fare:
1)Rimuovere i commenti
2)Aggiungere alla symbol_table le label dei salti -> ( label )
3)Aggiungere alla symbol_table le label delle variabili (indirizzi @i)
2)Sostituire le label e variabili con i simboli in symbol_table -> ( @label , @i)
*/

list_handler *replace_symbols( list_handler *lh_input, list_handler *lh_symbol_table ){
	/* 
		Vengono registrate le label nella symbol table ma non vengono memporizzate nell'output
		Vengono inseriti tutti i caratteri delle C ed A istruction da lh_input;
	 	Vengono sostituite le label e le variabili dopo '@' con gli indirizzi numerici trovati nella symbol table
	*/
	list_handler *lh_output = NULL;
	list_handler *lh_labelBuffer = NULL; // buffer per memorizzare i caratteri delle label uno alla volta
	list_handler *lh_current_word = NULL;// non usato
	list_node *node_start_of_line = NULL;// dopo ogni fine righa tiene traccia dell primo nodo nella riga corrente
	list_node *node = lh_input->head; // tiene traccia del nodo (con carattere) puntato corrente
	char *value = NULL; // puntatore del carattere puntato da node
	int address = 16; // indirizzo da cui partire (incluso ) per assegnare le variabili
	int row = 0; // riga attuale della rom ( senza commenti o label )
	bool b_store_label = false; // quando = true i caratteri venngono memorizzati in lh_labelBuffer
	bool b_ignore_line = false;// quando true i caratteri vengono ignorati finche il flag non cambia stato ( a fine riga)
	while( node != NULL ){
		value = (char*)node->value;

		if( *value != ' ' && *value != '\t' ){
			if( *value != '\r') // ignoro questo carattere durante filtro dei caratteri
			{
				// ignora i commenti
				if( *value == '/'  ){
					if( node->next != NULL && ( node->next->value ) == '/' ){
						b_ignore_line = true;
						node = node->next;
					}
					else{
						printf( "Errore: sintassi errata (%s) a riga %d\n","Commento non valido", "", row );
					}
				}
				
				if( !b_ignore_line){
					if( *value == '('){
						b_store_label = true; // inizio a memorizzare la label
					}
					else if( *value == ')'){ // uso ')' come delimitatore di fine label
						/* 
							sono a fine istruzione e stavo memorizzando la label:
							se la stringa memorizzata non è nella tabella simboli
								allora la salvo per usarla in seguito ...
							
							sostituisco la label dell'input con la parte di stringa data con il valore corretto
						*/
						if( b_store_label ){
							char *str_label = toString( lh_labelBuffer->head, NULL );
							symbol *s = getFromSymbolTable( lh_symbol_table, str_label );
							if( s == NULL){ // aggiungo il nuovo simbolo alla lista con indirizzo prossima istruzione
								lh_symbol_table = enqueue( lh_symbol_table, new_symbol( str_label, row + 1 ) );
							}
							b_store_label = false;
							node_start_of_line->next = node->next;
						}
					}
					else if( b_store_label ){
						lh_labelBuffer = enqueue( lh_labelBuffer, value);
					}
					else if( *value == '@' ){// inizio a memporizzare nel buffer i caratteri seguenti dal prossimo carattere
						b_store_label = true;
					}
								
					if( !b_store_label ){ // non voglio inseire i caratteri delle label che voglio sostituire
						lh_output = enqueue( lh_output, value );
					}
				}

				if( *value == '\n' ){
					if( b_store_label ){ // stavo memorizzando una label
						int size_str = 0;
						char *str_label = toString( lh_labelBuffer->head, &size_str );
						symbol *s = getFromSymbolTable( lh_symbol_table, str_label );
						if( s == NULL){ // aggiungo il nuovo simbolo alla lista con indirizzo per una variabile disponibile
							int possible_address = atoi( str_label );
							if( str_label[0] != '0' && possible_address != 0 ){// la label è un indirizzo
								s = new_symbol( str_label, possible_address );
							}
							else if( str_label[0] == '0' && size_str <= 2 && possible_address == 0){// la label ha indirizzo 0
								s = new_symbol( str_label, 0 );
							}
							else{ // la label è per una variabile (stringa)
								s = new_symbol( str_label, address );
							}
							lh_symbol_table = enqueue( lh_symbol_table, s );
						}
						// converto il numero in stringa eaggiungo ogni singolo carattere (eccetto '\0')
						// nella lista di output
						// dopo @ : invece che scrivere il nome della variabile inserisce l'indirizzo assegnato
						char *str_address = int_to_str( s->value );
						int length_str_address = strlen( str_address );
						for( int i = 0; i < length_str_address; i+= 1 ){
							lh_output = enqueue( lh_output, &str_address[i] );
						}
						b_store_label = false;
					}
					
					if( !b_ignore_line ){// non sto ignorando la memorizzazione, quindi tengo traccia della prossima istruzione
						row += 1;
					}
					b_ignore_line = false; // quando sono a capo il non ho pià l'effetto del commento
					node_start_of_line = node->next;// tengo traccia del primo dono della riga
				}
			}
		}
		node = node->next;
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
	lh_input = replace_symbols( lh_input, lh_symbol_table );
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

