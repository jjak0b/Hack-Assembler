typedef struct list_char_handler list_c_handler; /*Gestore della lista*/
typedef struct node_char node_char; /*Bi-lista di caratteri*/

/**
 * @brief
 * PreCondition: N/A
 * PostCondition: Dato un nodo, ritorna la sua lunghezza della lista a seguire (o precedere) da tale nodo
 * se b_reachLast != 0 conta fino all'ultimo elemento della lista ritornado lunghezza positiva
 * altrimenti conta fino al primo elemento ritornando lunghezza negativa
 * @param head 
 * @return La lunghezza della lista
 */
int size( node_char *head, int b_reachLast);

/**
 * @brief Datp una lista ritorna l'ultimo elemento della lista 
 * se la lista è vuota ritorna NULL
 * 
 * @param head 
 * @return node_char*  Il puntatore all'ultimo nodo della lista
 */
node_char *last( node_char *head );

/**
 * @brief 
 * PreCondition: 	N/A
 * PostCondition:	Data una lista e un indice ritorna il nodo alla "index-esima"  posizione (anche negativa ) a partire dal nodo head
 * 					ritorna NULL se head == NULL oppure index < 0 oppure index è superiore (outofbound) alla lunghezza fino agli estremi
 *
 * @param head 
 * @param index 
 * @return node_char* Il nodo alla "index-esima" posizione
 */
node_char *get( node_char *head, int index );
/*
// Obsoleto e non testato
node_char *get_from_handler( list_c_handler *handler, int index);
*/

/**
 * @brief 
 * PreCondition: N/A
 * PostCondition: Dati due nodi di lista e un indice, ritorna la prima lista con inserita la seconda a partire dalla posizione 'index-esima'
 * e collega l'ultimo elemento della seconda lista con la parte restante della prima, ovvero dalla posizione 'index'+1 della prima lista.
 * (Funziona anche con index negativo)
 * es: insert( c-i-a-o, c-i-a-o, 2 ) ->c-i-c-i-a-o-a-o
 * 
 * @param head 
 * @param node 
 * @param index 
 * @return node_char* nodo alla posizione index dopo inserimento
 */
node_char *insert( node_char *head, node_char *node, int index );
/*
// Obosleto e non testato
int insert( list_c_handler *handler, char value, int index )
*/

/**
 * @brief 
 * 	PreCondition:
		si suppone che b_create_handler sia = 1 solo se si vuole creare una nuova lista completa ( e non un singolo nodo )
		mentre se è già presente una lista ma si vuole aggiungere un nodo, il chiamante deve passare una struttura "handler" di una lista precedentemente creata
	PostCondition:
		Crea una nuovo nodo di una lista char assegnando il valore specificato ad esso.
		se "b_create_handler" = 0 assegna il parametro "handler" come struttura "handler" della nuova lista che verrà creata
		se "b_create_handler" != 0 alloca nuova memoria al puntatore "handler" quindi crea una nuova struttura condivisa con tutti nodi della lista:
			la struttura "handler" contiene sempre il puntatore alla testa (->head) ed alla coda (->tail)
 * 
 * @param value 
 * @param b_create_handler 
 * @param handler 
 * @return node_char* 
 */
node_char *node_char_new( char value, const unsigned short int b_create_handler, list_c_handler *handler );

node_char *node_char_reverse( node_char *head );

void node_char_print( node_char *head );

node_char *next( list_c_handler *handler );

node_char *prev( list_c_handler *handler );

list_c_handler *enqueue( list_c_handler *handler, char value);

node_char *dequeue( list_c_handler *handler );
