
typedef struct list_char_handler list_c_handler; /*Gestore della lista*/
typedef struct node_char node_char; /*Bi-lista di caratteri*/

/**
 * @brief 
 * 	precondition:
		si suppone che b_create_handler sia = 1 solo se si vuole creare una nuova lista completa ( e non un singolo nodo )
		mentre se è già presente una lista ma si vuole aggiungere un nodo, il chiamante deve passare una struttura "handler" di una lista precedentemente creata
	Postcondition:
		Crea una nuovo nodo di una lista char assegnando il valore specificato ad esso.
		se "b_create_handler" = 0 assegna il parametro "handler" come struttura "handler" della nuova lista che verrà creata
		se "b_create_handler" = 1 alloca nuova memoria al puntatore "handler" quindi crea una nuova struttura condivisa con tutti nodi della lista:
			la struttura "handler" contiene sempre il puntatore alla testa (->head) ed alla coda (->tail)
 * 
 * @param value 
 * @param b_create_handler 
 * @param handler 
 * @return node_char* 
 */
node_char *node_char_new( char value, const unsigned short int b_create_handler, list_c_handler *handler );

void node_char_init( node_char *head, list_c_handler *handler, char value );

node_char *next( list_c_handler *handler );

node_char *prev( list_c_handler *handler );

list_c_handler *enqueue( list_c_handler *handler, char value);

node_char *dequeue( list_c_handler *handler );

node_char *node_char_reverse( node_char *head );

void node_char_print( node_char *head );