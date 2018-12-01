#include "list.h"
#include <stdio.h>
#include <stdlib.h>
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
int size( list_node *head, bool b_reachLast){
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
 * @return list_node*  Il puntatore all'ultimo nodo della lista
 */
list_node *last( list_node *head ){
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
 * @return list_node* Il nodo alla "index-esima" posizione
 */
list_node *get( list_node *head, int index ){

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
list_node *get_from_handler( list_handler *handler, int index){
	list_node *tmp = NULL;
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
 * @return list_node* nodo alla posizione index dopo inserimento
 */
list_node *insert( list_node *head, list_node *node, int index ){
	if( head == NULL  ){
		return NULL;
	}
	else if( index == 0){
		if( head != NULL ){
			if( head->prev != NULL){ // casistica normale
				head->prev->next = node;
				list_node *node_last = last( node );
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
int insert( list_handler *handler, char value, int index ){
	if( index < 0 || handler == NULL ||  index > ( size( handler )-1 ) ){
		return -1;
	}
	else{
		list_node *prev_node = get( handler, index - 1 );
		if( prev_node == NULL ){ // non esiste l'indice precedente
			return false;
		}
		else{
			list_node *next_node = prev_node->next;
			list_node *current_node = list_node_new( value, false, handler );
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
 * @return list_node* 
 */
list_node *list_node_new( void *value, const bool b_create_handler, list_handler *handler ){
	list_node *head;
	head = ( list_node* ) malloc( sizeof( list_node ) );

	head->next = NULL;
	head->prev = NULL;
	head->value = value;
	if( b_create_handler ){
		handler = ( list_handler* ) malloc( sizeof( list_handler ) );
		handler->head = head;
		handler->tail = head;
	}

	head->handler = handler;

	return head;
}

list_node *list_node_reverse( list_node *head ){
	list_node  *head_new, *head_old;
	head_new = NULL ;
	while (head != NULL){
		head_old = head;
		head = head->next;
		head_old->next = head_new;
		head_new = head_old;
	}
	return head_new;
}

/**
 * @brief Stampa i valori degli elementi di una
 * Precondition: valore puntato da head->value deve essere un tipo supportato da printf (es char*, char, int, float, ecc..)
 * 
 * @param format formato di stampa del valore: la stringa viene passata a printf, quindi usare come se fosse tale funzione
 * @param head testa della lista da cui verranno stampati gli elementi
 */
void list_node_print( const char *format, list_node *head){
	char *ptr_char =NULL;
	if( head != NULL ){
		ptr_char = head->value;
		printf( format, *ptr_char);
		list_node_print( format, head->next);
	}
}

list_node *next( list_handler *handler ){
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

list_node *prev( list_handler *handler ){
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

list_handler *enqueue( list_handler *handler, void *value ){
	bool b_create_handler = handler == NULL;
	list_node *new_tail = list_node_new( value, b_create_handler, handler );
	if( b_create_handler ){
		handler = new_tail->handler;
	}
	list_node *old_tail = handler->tail;
	new_tail->prev = old_tail;
	new_tail->next = NULL; // dovrebbe essere già a NULL dato che era in coda...
	old_tail->next = new_tail;
	handler->tail = new_tail;
	return handler;
}

list_node *dequeue( list_handler *handler ){
	if( handler != NULL ){
		list_node *old_head = handler->head;	
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