/*
	Author: Jacopo Rimediotti
	Convenzioni usate:
		b_nomevar / isNomeVar 	-> usata per variabili booleane
		n_nomevar 				-> usata (non spesso) per indicare un intero
		str_nomevar 			-> usata per stringhe ( array di char con '\0' finale )
		lenght_nomevar			-> usata in genere nelle stringhe per indicare la lunghezza della stringa prima del carattere '\0'
		size_nomevar			-> usata per indicare la dimensione totale di una lista o array ( nelle quindi indica la dimensione allocata all'array )
		lh_nomevar 				-> usata per indicare una struttura di tipo list_handler
		l_nomevar 				-> usata per indicare una struttura di tipo lista
*/
#include <stdio.h>
#include <stdlib.h> // #include <cstdlib>
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
			#ifdef DEBUG
			printf("Trovato simbolo %s in lista: %d\n", s->key, s->value);
			#endif
		}
		node_symbol_table = node_symbol_table->next;
	}

	return s_replace;
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
		int_to_strbuffer( i, str_N, size_str_RN-1 ); // converte il valore di i da numero ad una stringa, e la inserisce in str_N
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

list_handler *init_default_comp_table( list_handler *lh_symbol_table ){
	lh_symbol_table = NULL;
																									// a-XXXXXX
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "0",  2 + 8 + 32  ) ); 					// 0-101010
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "1", 64 - 1 ) ); 						// 0-111111
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "-1",  2 + 8 + 16 + 32 ) ); 			// 0-111010
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D", 4 + 8 ) ); 						// 0-001100
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "A", 16 + 32  ) ); 						// 0-111000
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "M", 16 + 32 + 64 ) );					// 1-111000
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "!D", 1 + 4 + 8 ) ); 					// 0-001101
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "!A", 1 + 16 + 32 ) ); 					// 0-110001
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "!M", 1 + 16 + 32 + 64) );				// 1-110001
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "-D", 1 + 2 + 4 + 8 ) ); 				// 0-001111
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "-A", 1 + 2 + 16 + 32 ) ); 				// 0-110011
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "-M", 1 + 2 + 16 + 32 + 64 ) );			// 1-110011
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D+1", 1 + 2 + 4 + 8 + 16 ) ); 			// 0-011111
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "A+1", 1 + 2 + 4 + 16 + 32 ) );			// 0-110111
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "M+1", 1 + 2 + 4 + 16 + 32 + 64 ) );	// 1-110111
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D-1", 2 + 4 + 8 ) ); 					// 0-001110
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "A-1", 2 + 16 + 32 ) ); 				// 0-110010
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "M-1", 2 + 16 + 32 + 64 ) );			// 1-110010
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D+A", 2 ) ); 							// 0-000010
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D+M", 2 + 64 ) );						// 1-000010
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D-A", 1 + 2 + 16 ) ); 					// 0-010011
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D-M", 1 + 2 + 16 + 64 ) );				// 1-010011
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "A-D", 1 + 2 + 4 ) ); 					// 0-000111
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "M-D", 1 + 2 + 4 + 64 ) );				// 1-000111
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D&A", 0 ) );							// 0-000000
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D&M", 64 ) );							// 1-000000
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D|A", 1 + 4 + 16 ) );					// 0-010101
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "D|M", 1 + 4 + 16 + 32 ) );				// 1-010101
	return lh_symbol_table;
}

list_handler *init_default_jmp_table( list_handler *lh_symbol_table ){
	lh_symbol_table = NULL;
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "JGT", 1 ) );	// 001
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "JEQ", 2 ) );	// 010
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "JGE", 3 ) );	// 011
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "JLT", 4 ) );	// 100
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "JNE", 5 ) );	// 101
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "JLE", 6 ) );	// 110
	lh_symbol_table = enqueue( lh_symbol_table, new_symbol( "JMP", 7 ) );	// 111
	return lh_symbol_table;
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
	#ifdef DEBUG
	printf("->replace_symbols()\n");
	#endif
	list_handler *lh_output = NULL;
	list_handler *lh_labelBuffer = NULL; // buffer per memorizzare i caratteri delle label uno alla volta
	list_handler *lh_current_word = NULL;// non usato
	list_node *node_start_of_line = NULL;// dopo ogni fine righa tiene traccia dell primo nodo nella riga corrente
	list_node *node = lh_input->head; // tiene traccia del nodo (con carattere) puntato corrente
	char *value = NULL; // puntatore del carattere puntato da node
	char *next_value = NULL;// puntatore del carattere successivo puntato da node
	int address = 16; // indirizzo da cui partire (incluso ) per assegnare le variabili
	int row = 0; // riga attuale della rom ( senza commenti o label )
	bool b_store_label = false; // quando = true i caratteri venngono memorizzati in lh_labelBuffer
	bool b_ignore_line = false;// quando true i caratteri vengono ignorati finche il flag non cambia stato ( a fine riga)
	bool b_all_labels_read = false;
	bool b_skip_char = false;// indica se saltare il carattere corrente durante la memorizzazione

	for( int i = 0; i < 3; i+=1){
		#ifdef DEBUG
		if( i == 0){
			printf("!!!!!!!!!!! RIMOZIONE COMMENTI !!!!!!!!\n");
		}
		if( i == 1){
			printf("!!!!!!!!!!! LETTURA LABELS !!!!!!!!\n");
		}
		else{
			printf("!!!!!!!!!!! SOSTITUZIONE LABELS !!!!!!!!\n");
		}
		#endif
		// il primo ciclo si occuerà di rimuovere i i commenti
		// il seocnod ciclo si occuperà di leggere le definizione delle label e rimoverle,
		// il terzo ciclo si occuperà di sostituire le label( o varaibili) nelle A instructiomn (@)
		while( node != NULL ){
			value = (char*)node->value;
			if( node->next != NULL ){
				next_value = (char*)node->next->value;
			}
			else{
				next_value = NULL;
			}

			if( *value != ' ' && *value != '\t' && *value != '\r'){ // ignoro questi caratteri durante filtro dei caratteri
				if( i == 0 ){ // PRIMO CICLO: ignora i commenti
					if( !b_ignore_line){
						if( *value == '/'  ){
							if( next_value != NULL && *next_value == '/' ){
								b_ignore_line = true;
								node = node->next;
								#ifdef DEBUG
								printf(" #Ignorando:%c", *value );
								#endif
							}
							else{
								printf( "Errore: sintassi errata (%s) a riga %d\n","Commento non valido", "", row );
								return NULL;
							}
						}
					}
				}
				else if( i == 1 ){	// SECONDO ciclo: LEGGI LABELS
					if( !b_ignore_line){
						if( *value == '('){
							b_store_label = true; // inizio a memorizzare la label
							b_skip_char = true; // salto la memorizzazione di ')'
						}
						else if( *value == ')'){ // uso ')' come delimitatore di fine label
							/* 
								sono a fine istruzione e stavo memorizzando la label:
								se la stringa memorizzata non è nella tabella simboli
									allora la salvo per usarla in seguito ...
								
								sostituisco la label dell'input con la parte di stringa data con il valore corretto
							*/
							if( b_store_label ){
								char *str_label = list_to_string( lh_labelBuffer->head, NULL );
								symbol *s = getFromSymbolTable( lh_symbol_table, str_label );
								if( s == NULL){ // aggiungo il nuovo simbolo alla lista con indirizzo prossima istruzione
									s = new_symbol( str_label, row );
									lh_symbol_table = enqueue( lh_symbol_table, s );
									printf("[NUOVO SIMBOLO :(%s) -> %d]", s->key, s->value );
								}
								b_store_label = false; // smetto di memorizzare la label 
								b_ignore_line = true; // posso ignorare il resto della riga
								b_skip_char = true; // il prossimo carattere sarà un '\n', ma ho già uno '\n' della riga precedence, di conseguenza avrei una linea vuota senza questo
								delete_list( lh_labelBuffer, false );
								lh_labelBuffer = NULL;
								// node_start_of_line->next = node->next;
							}
						}
					}
				}
				else{ // TERZO CICLO: sostituisco le label dopo @ in base a quelle memorizzate in symbol_table
					if( *value == '@'){
						b_store_label = true;
						// lo inserisco manualmente
						lh_output = enqueue( lh_output, value );
						b_skip_char = true; // non registrare '@'
					}
					else if( *value == '\n' ){ // sostituisci la label con il giusto indirizzo quando arrivi a fine riga
						if( b_store_label ){ // stavo memorizzando una label

							int size_str = 0;
							char *str_label = list_to_string( lh_labelBuffer->head, &size_str );// converte quello che ho memorizzato in stringa
							#ifdef DEBUG
							printf("\nstringa memorizzata: %s\n", str_label );
							#endif
							symbol *s = getFromSymbolTable( lh_symbol_table, str_label );
							bool isLabel = false;
							if( s == NULL){ // aggiungo il nuovo simbolo alla lista con indirizzo per una variabile disponibile
								int possible_address = atoi( str_label );

								if( size_str <= 2 && str_label[0] == '0' && possible_address == 0){// la label ha indirizzo 0
									s = new_symbol( str_label, 0 );
								}
								else if( /*str_label[0] != '0' &&*/ possible_address != 0 ){// la label è un indirizzo preimpostato
									s = new_symbol( str_label, possible_address );
								}
								else{ // la label non è un numero: è una stringa (stringa)
									if( isNumber( str_label, false ) ){// controllo il primo carattere ( per false )
										printf("ERRORE: (riga: %d) La label non può iniziare con un numero e continuare come stringa\n", row );
										return NULL;
									}
									else{
										isLabel = true;
										s = new_symbol( str_label, address );
										address += 1;
									}									
								}
								#ifdef DEBUG
								printf("Nuova etichetta GENERATA:\nKey: %s\tValue: %d\n", s->key, s->value );
								#endif
								if( isLabel ){
									lh_symbol_table = enqueue( lh_symbol_table, s );
								}
							}
							#ifdef DEBUG
							else{
								printf("Etichetta TROVATA:\nKey: %s\tValue: %d\n", s->key, s->value );
							}
							#endif
							// else-> s != NULL -> un simbolo è già stato definito: sarà una label già registrata

							// converto il numero in stringa e aggiungo ogni singolo carattere (eccetto '\0') nella lista di output
							// dopo @ : invece che scrivere il nome della variabile inserisce l'indirizzo assegnato
							char *str_address = int_to_string( s->value );//La lista d'output usa caratteri: dopo devo convertirli in carattere
							list_handler *lh_address = string_to_list( str_address );
							list_node *head_address = lh_address->head;
							#ifdef DEBUG
							// printf("Indirizzo [%d] = \"%s\"\n", s->value, str_address );
							#endif

							// libero la memoria inutilizzata
							free( lh_address );
							lh_address = NULL;

							setupNodesHandler( head_address, lh_output);
							
							// collego L'ultimo elemento registrato nell'output ( ovvero '@' ) con il primo dell'indirizzo registrato
							list_node *old_tail = lh_output->tail;
							list_node *last_address = last( head_address );
							head_address->prev = old_tail;
							old_tail->next = head_address;
							last_address->next = NULL;
							lh_output->tail = last_address;

							// list_node_print( "%c", lh_output->head );

							// libero la memoria inutilizzata
							delete_list( lh_labelBuffer, false );
							lh_labelBuffer = NULL;

							// non sto più memorizzando
							b_store_label = false;
						}
					}
				}

				printf("%c", *value );
				if( !b_ignore_line || ( b_ignore_line && *value == '\n')  ){
					if( !b_skip_char ){
						if( b_store_label ){// memorizzo il carattere
							lh_labelBuffer = enqueue( lh_labelBuffer, value);
						}
						else{ // non voglio inserire i caratteri delle label che voglio sostituire
							lh_output = enqueue( lh_output, value );
						}
					}
					else{ // devo saltare il prosimo carattere
						if(*value == '\n'){// sto saltando questo \n quindi non la conto come riga
							row -= 1; // la decremento perchè verrà incrementata dopo
						}
						b_skip_char = false;// non ignorare il prossimo
					}
				}

				if( *value == '\n' ){
					row += 1;// tengo traccia della prossima istruzione
					b_ignore_line = false; // quando sono a capo, non ho più l'effetto del commento
					node_start_of_line = node->next;// tengo traccia del primo nodo della riga
				}
			}
			node = node->next;
		}

		#ifdef DEBUG
		printf("Ciclo elaborazione %d concluso.\n", i+1 );
		printf("------Simboli in memoria:------\n");
		print_symbols( lh_symbol_table->head );
		printf("-------------------------------\n");
		list_node_print( "%c", lh_output->head);
		printf("-------------------------------\n");
		#endif

		if( i < 2){
			lh_input = lh_output;
			lh_output = NULL;
			row = 0;
			node = lh_input->head; // ricomincio con il risultato della prima elaborazione
			b_all_labels_read = true; // ora devo sostituire non pià leggere ( o ignorare )
			// resetto i valori iniziali
			b_ignore_line = false;
			b_skip_char = false;
			b_store_label = false;
			node_start_of_line = node;
		}
	}

	// list_node_print( "%c", lh_output->head);
	return lh_output; 
}

list_handler *replace_instructions( list_handler *lh_input ){
	list_handler *lh_comp_table = init_default_comp_table( NULL );
	list_handler *lh_jmp_table = init_default_jmp_table( NULL );	
	list_node *node = lh_input->head;
	list_node *node_last = last( lh_input->head );
	list_handler *lh_buffer = NULL, *lh_output = NULL;
	lh_output = malloc( sizeof( list_handler ) );
	lh_output->head = NULL;
	lh_output->current = NULL;
	lh_output->tail = NULL;

	char *value = NULL;
	bool b_store_constant = false;
	bool b_store_instruction = false;
	int row = 0;// a puro scopo di informativo in caso di errore di sintassi
	while( node != NULL ){
		value = (char*)node->value;
		#ifdef DEBUG
		// printf( "%c", *value);
		#endif
		if( !b_store_constant && !b_store_instruction ){
			if( *value == '@' ){
				b_store_constant = true;
			}
			else{
				b_store_instruction = true;
			}
			#ifdef DEBUG
			printf( "\nRIGA: %d----------------------------\n", row);
			printf("Storing: ");
			#endif
		}
		if( *value == '\n' || node == node_last ){
			if( node == node_last ){
				lh_buffer = enqueue( lh_buffer, node->value );
			}
			if( b_store_constant){

				char *str_constant = list_to_string( lh_buffer->head, NULL );
				int constant = atoi( str_constant );
				#ifdef DEBUG
				printf( "\nNuova A-Istruction:\n");
				list_node_print( "%c", lh_buffer->head);
				printf( "\n");
				#endif
				list_handler *lh_constant = int_to_binary_list( constant, 16 );
				bool *first = lh_constant->head->value;
				*first = false; // lo imposto a 0 come default per le A instruction
				char *str_binary = list_binary_to_string( lh_constant->head, NULL );
				list_handler *lh_str_binary = string_to_list( str_binary );
				free( str_binary );
				str_binary = NULL;

				// collegamento liste
				lh_output->head = append( lh_output->head, lh_str_binary->head );

				#ifdef DEBUG
				printf( "Costante \"%s\" codificata in:\t", str_constant );
				list_node_print( "%c", lh_str_binary->head);
				printf( "\n");
				#endif
				free( str_constant );
				free( lh_constant );
				lh_constant = NULL;
				b_store_constant = false;
			}
			else if( b_store_instruction ){
				#ifdef DEBUG
				printf( "\nNuova C-Istruction:\n");
				list_node_print( "%c", lh_buffer->head);
				printf("\n");
				#endif

				list_handler *lh_binary = NULL;
				
				// Aggiungo 3 '1' alla lista
				int *n_tmp = NULL;
				for( int i = 1; i <= 3; i+=1 ){
					n_tmp = malloc( sizeof( int ));
					*n_tmp = true;
					lh_binary = enqueue( lh_binary, n_tmp );
				}
				n_tmp = NULL;

				int length_str;
				char *str_instruction = list_to_string( lh_buffer->head, &length_str );

				int start_dest_instruction = 0;
				int end_dest_instruction = length_str;

				bool hasDest = isSubstr( str_instruction , "=" , &end_dest_instruction );

				if(!hasDest){
					end_dest_instruction = 0;
				}

				int start_jmp_instruction;
				int end_jmp_instruction = 0;

				bool hasJump = isSubstr( str_instruction , ";" , &start_jmp_instruction ) || isSubstr( str_instruction , "," , &start_jmp_instruction );
				// printf("HAS JUMP:%d\t%s\n", hasJump, str_instruction);
				// dest=comp;jump
				int start_comp_instruction = end_dest_instruction;
				int end_comp_instruction = start_jmp_instruction;

				if( hasJump ){ // caso ;jump
					// printf( "jump\n");
					end_jmp_instruction = length_str;
					end_comp_instruction = start_jmp_instruction;
					start_jmp_instruction += 1;
					if( hasDest ){ // caso dest=comp;jump
						start_comp_instruction = end_dest_instruction + 1;
					}
					else{ // caso comp;jump
						start_comp_instruction = 0;
					}
				}
				else{
					// printf( "NOT jump\n");
					end_jmp_instruction = start_jmp_instruction;
					if( hasDest ){ // caso dest=comp
						start_comp_instruction = end_dest_instruction + 1;
						end_comp_instruction = length_str;
					}
					else{ // caso comp 
						start_comp_instruction = 0;
						end_comp_instruction = length_str;
					}
				}
				
				int n_3bit_dest = 0; // numero decimale da convertire in binario - inizializzato per caso "null" ( non memorizzato )
				int n_3bit_jmp = 0; //numero decimale da convertire in binario - inizializzato per casocaso "null" ( nessun salto )
				// 	printf( "sequenza: %d\n", isSubstr( str_instruction , "=" , &end_dest_instruction ));

				int length_dest = end_dest_instruction - start_dest_instruction;
				char *str_dest = malloc( sizeof( char ) * length_dest + 1);
				str_dest[0] = '\0';
				if( hasDest ){
					// printf( "2:%s\t%d\n", str_instruction, end_dest_instruction );
					strncpy( str_dest, str_instruction, length_dest );
					str_dest[ length_dest ] = '\0';
					// printf( "3:%s\t%d\n", str_instruction, end_dest_instruction );
					// printf( "4:%s\t%d\n", str_dest, end_dest_instruction );
					// printf( "Dest:\t%s\n",  str_dest);
					for( int i = 0; i < end_dest_instruction; i+=1 ){
						if( str_dest[i] == 'M' ){
							n_3bit_dest += 1; // 2^0
						}
						else if( str_dest[i] == 'D' ){
							n_3bit_dest += 2; // 2^1
						}
						else if( str_dest[i] == 'A' ){
							n_3bit_dest += 4; // 2^2
						}
					}
					// free( str_dest );
				}
				#ifdef DEBUG
				printf( "Dest:\t%s\t|%d\t|%d\n", str_dest, length_dest, end_dest_instruction );
				#endif
				list_handler *lh_binary_dest = int_to_binary_list( n_3bit_dest, 3 );
				// printf( "1:%s\t%d\n", str_instruction, end_dest_instruction );

				#ifdef DEBUG
				printf( "Sequenza ottenuta (dest)(%s):\n", str_dest );
				list_node_print( "%d", lh_binary_dest->head);
				printf( "\n" );
				#endif

				//printf("start_jump %d\n", start_jmp_instruction );
				//printf("end_jump %d\n", end_jmp_instruction );
				int length_jmp = end_jmp_instruction - start_jmp_instruction;
				char *str_jump = malloc( sizeof( char ) * ( length_jmp + 1) );
				if( hasJump ){
					strncpy( str_jump, str_instruction + start_jmp_instruction, length_jmp );
					str_jump[ length_jmp ] = '\0';

					symbol *s_jmp = getFromSymbolTable( lh_jmp_table, str_instruction + start_jmp_instruction );
					if( s_jmp != NULL ){
						n_3bit_jmp = s_jmp->value;
					}
				}
				// length_jmp =  length_str - (start_jmp_instruction+1);
				list_handler *lh_binary_jmp = int_to_binary_list( n_3bit_jmp, 3 );

				#ifdef DEBUG
				printf( "Jump:\t%s\t%d\t%d\n", str_jump/*str_instruction + start_jmp_instruction*/, length_jmp, end_jmp_instruction);
				#endif

				#ifdef DEBUG
				//printf("Jump:\t%s\t|%d\t|%d\n", str_jump, s);
				printf( "Sequenza ottenuta (jump)(%s):\n", str_jump );
				list_node_print( "%d", lh_binary_jmp->head);
				printf( "\n" );
				#endif

				//	printf("start_comp %d\n", start_comp_instruction );
				//	printf("end_comp %d\n", end_comp_instruction );
			
				int length_str_comp = end_comp_instruction - start_comp_instruction ;
				// int length_str_comp = (end_comp_instruction - start_comp_instruction) ;
				char *str_comp = malloc( sizeof( char ) * ( length_str_comp + 1 ) );
				strncpy( str_comp, str_instruction + start_comp_instruction, length_str_comp );
				str_comp[ length_str_comp ] = '\0';
				symbol *s_comp = getFromSymbolTable( lh_comp_table, str_comp );

				list_handler *lh_binary_comp = NULL;
				if( s_comp != NULL ){
					lh_binary_comp = int_to_binary_list( s_comp->value, 7 );
				}
				else{
					printf("Errore: Istruzione \"%s\" (lunghezza:%d) sconosciuta a riga %d\n", str_comp, length_str_comp,row );
				}

				#ifdef DEBUG
				printf( "Sequenza ottenuta (comp)(%s):\t", str_comp );
				list_node_print( "%d", lh_binary_comp->head);
				printf( "\n" );
				#endif

				// collego le varie liste: 111->comp->dest->jump
				list_handler *lh_tmp = lh_binary_comp;
				lh_binary->head = append( lh_binary->head, lh_binary_comp->head );
				free( lh_tmp );
				lh_tmp = lh_binary_dest;
				lh_binary->head = append( lh_binary->head, lh_binary_dest->head );
				free( lh_tmp );
				lh_tmp = lh_binary_jmp;
				lh_binary->head = append( lh_binary->head, lh_binary_jmp->head );
				free( lh_tmp );

				char *str_binary = list_binary_to_string( lh_binary->head, NULL );
				list_handler *lh_binary_string = string_to_list( str_binary );
				lh_output->head = append( lh_output->head, lh_binary_string->head );
				#ifdef DEBUG
				printf( "Istruzione \"%s\" assemblata in:\t", str_instruction );
				list_node_print( "%c", lh_binary_string->head);
				printf( "\n" );
				#endif
				b_store_instruction = false;
			}

			delete_list( lh_buffer , true );
			lh_buffer = NULL;

			char *new_ln = malloc(sizeof(char)*2);
			strcpy( new_ln, "\r\n");
			list_handler *lh_nl = string_to_list( new_ln );
			lh_output->head = append( lh_output->head, lh_nl->head );
			row += 1;			
		}
		else if( *value != '@'){
			if( b_store_constant || b_store_instruction){
				printf( "%c", *value);
				lh_buffer = enqueue( lh_buffer, value );
			}
		}
		node = node->next;
	}

	return lh_output;
}

list_handler *assembler( list_handler *lh_input ){	
	list_handler *lh_symbol_table = NULL;
	list_handler *lh_output = NULL;
	lh_symbol_table = init_default_symbol_table( lh_symbol_table );
	print_symbols( lh_symbol_table->head );

	#ifdef DEBUG
	printf( "Inizio Rimpiazzo Simboli...\n");
	#endif
	lh_output = replace_symbols( lh_input, lh_symbol_table );

	#ifdef DEBUG
	printf( "Inizio codifica istruzioni...\n");
	#endif
	lh_output = replace_instructions( lh_output );

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
						printf( "\n" );
						#endif

						if( lh_output != NULL ){
							int length_estension = strlen( FILE_OUTPUT_EXTENSION );
							int length_filename = strlen( filename );
							int length_FilenameOut = length_filename + length_estension; // la dimensione non è proprio ottimizzata ma sicuramente non andrà outofbound
							char *filename_out = ( char* ) malloc(sizeof(char) * ( length_FilenameOut + 1 ) );
							strncpy( filename_out, filename, length_FilenameOut );
							replaceFilenameExtension( filename_out, length_filename, FILE_OUTPUT_EXTENSION );
							printf("Scrittura dell'elaborazione su file '%s' in corso...\n", filename_out);
							if( writeFile( filename_out, lh_output) ){
								printf("Scrittura sul file '%s' avvenuta con successo\n", filename_out );
							}
							else{
								printf("ERRORE: Impossibile aprire o scrivere sul file '%s'\n", filename_out );
							}
						}
						else{
							printf("ERRORE: Impossbile completare l'operazione a causa di un errore durante l'elaborazione\n");
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