#include "list_char.h";
#include <stdio.h>
#include "utility.h"

/*
	Idea Lista applicata:

	O := nodo
	handler :=
	- head	->	O 0
				|
				O 1
				|
	- current->	O 2 = index
				|
				O 3
				|
	- tail	->	O 4
				|
				NULL
*/

/**
 * @brief
 * PreCondition: N/A
 * PostCondition: Dato un nodo, ritorna la sua lunghezza della lista a seguire (o precedere) da tale nodo
 * se b_reachLast != 0 conta fino all'ultimo elemento della lista ritornado lunghezza positiva
 * altrimenti conta fino al primo elemento ritornando lunghezza negativa
 * @param head 
 * @return La lunghezza della lista
 */
int size( node_char *head, int b_reachLast){
	if( head == NULL ){
		return 0;
	}
	if( b_reachLast ){
		return 1 + size( head->next, b_reachLast);
	}
	else{
		return -1 + size( head->prev, b_reachLast );
	}
}

/**
 * @brief Datp una lista ritorna l'ultimo elemento della lista 
 * se la lista è vuota ritorna NULL
 * 
 * @param head 
 * @return node_char*  Il puntatore all'ultimo nodo della lista
 */
node_char *last( node_char *head ){
	if( head == NULL ){
		return NULL;
	}
	else if( head->next == NULL ){
		return head;
	}
	else{
		return last( head->next);
	}
}

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
node_char *get( node_char *head, int index ){

	if( head == NULL ){ // Considera anche caso index sia oltre la lunghezza di fino agli estremi ( prev o next ) poichè head diventerà NULL se l'ultimo head->next o head->prev == NULL
		return NULL;
	}
	else if( index == 0){
		return head;
	}
	else{
		if( index > 0){
			return get( head->next, index-1);
		}
		else{
			return get( head->prev, index+1);
		}
	}
}
/*
// Obsoleto e non testato
node_char *get_from_handler( list_c_handler *handler, int index){
	node_char *tmp = NULL;
	if( handler != NULL && index > 0){
		if( handler->index == index ){
			return handler->current;
		}
		else{
			if(handler->index > index ){
				tmp = prev( handler );
			}
			else{
				tmp = next( handler );
			}

			if( tmp == NULL ){
				return NULL;
			}
			return get( handler, index );
		}
	}
	else{
		return NULL;
	}
}
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
node_char *insert( node_char *head, node_char *node, int index ){
	if( head == NULL  ){
		return NULL;
	}
	else if( index == 0){
		if( head != NULL ){
			if( head->prev != NULL){ // casistica normale
				head->prev->next = node;
				node_char *node_last = last( node );
				node_last->next = head;
				head->prev = node_last;
			}
			else{ // caso insert( l, i-s-t, size(l) )
				head->next = node;
			}
		}
		else{ // caso insert( NULL, l-i-s-t, *any )
			head = node;
		}
	}
	else{
		if( index > 0 ){
			return insert( head->next, node, index-1);
		}
		else{
			return insert( head->prev, node, index+1);
		}
	}
}
/*
// Obosleto e non testato
int insert( list_c_handler *handler, char value, int index ){
	if( index < 0 || handler == NULL ||  index > ( size( handler )-1 ) ){
		return -1;
	}
	else{
		node_char *prev_node = get( handler, index - 1 );
		if( prev_node == NULL ){ // non esiste l'indice precedente
			return false;
		}
		else{
			node_char *next_node = prev_node->next;
			node_char *current_node = node_char_new( value, false, handler );
			current_node->prev = prev_node;
			prev_node->next = current_node;
			current_node->next = next_node;
			next_node->prev = current_node;
			return true;
		}
	}
}
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
node_char *node_char_new( char value, const unsigned short int b_create_handler, list_c_handler *handler ){
	node_char *head;
	head = ( node_char* ) malloc( sizeof( node_char ) );

	head->next = NULL;
	head->prev = NULL;
	head->value = value;
	if( b_create_handler ){
		handler = ( list_c_handler* ) malloc( sizeof( list_c_handler ) );
		handler->head = head;
		handler->tail = head;
	}

	head->handler = handler;

	return head;
}

node_char *node_char_reverse( node_char *head ){
	node_char  *head_new, *head_old;
	head_new = NULL ;
	while (head != NULL){
		head_old = head;
		head = head->next;
		head_old->next = head_new;
		head_new = head_old;
	}
	return head_new;
}

void node_char_print( node_char *head ){
	if( head != NULL ){
		printf("%c", head->value);
		node_char_print( head->next);
	}
}

node_char *next( list_c_handler *handler ){
	if( handler != NULL ){
		if( handler->current != NULL ){
			handler->current = handler->current->next;
			handler->index += 1;
		}
		return handler->current;
	}
	else{
		return NULL;
	}
}

node_char *prev( list_c_handler *handler ){
	if( handler != NULL ){
		if( handler->current != NULL ){
			handler->current = handler->current->prev;
			handler->index -= 1;
		}
		return handler->current;
	}
	else{
		return NULL;
	}
}

list_c_handler *enqueue( list_c_handler *handler, char value){
	int b_create_handler = handler == NULL;
	node_char *new_tail = node_char_new( value, b_create_handler, handler );
	if( b_create_handler ){
		handler = new_tail->handler;
	}
	node_char *old_tail = handler->tail;
	new_tail->prev = old_tail;
	new_tail->next = NULL; // dovrebbe essere già a NULL dato che era in coda...
	old_tail->next = new_tail;
	handler->tail = new_tail;
	return handler;
}

node_char *dequeue( list_c_handler *handler ){
	if( handler != NULL ){
		node_char *old_head = handler->head;	
		handler->head = old_head->next;
		if( handler->current == old_head ){
			handler->current = handler->head;
		}
		old_head->prev = NULL; // dovrebbe essere già a NULL dato che era in testa...
		old_head->next->prev = NULL;
		return old_head;
	}
	else{
		return NULL;
	}
}
