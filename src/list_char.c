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


typedef struct list_char_handler {
	struct node_char *head; 	/*Punta alla testa ( primo elemento ) della lista*/
	struct node_char *tail; 	/*Punta alla coda ( ultimo elemento valido non NULL ) della lista*/
	struct node_char *current; 	/*Punta al nodo attuale preso in considerazione, il chiamante itera tra i nodi con next() o prev()*/
	int index; 				/*Indice dell'elemento puntato (current)*/
} list_c_handler;

typedef struct node_char {
	char value;							/*valore del nodo*/
	struct node_char *next;				/*puntatore al nodo successivo*/
	struct node_char *prev;				/*puntatore al nodo precedente*/
	/*
	*	gestore della lista:
		- ogni nodo ha un gestore come riferimento
		- tutti gli elementi di una lista devono avere lo stesso handler
		- il chiamante richiama le funzioni per iterare tra i nodi ed ha sempre i riferimenti degli estrami puntati
	*/
	struct list_char_handler *handler;
} node_char;

int size( node_char *head ){
	if( head == NULL ){
		return 0;
	}
	return 1 + size( head->next );
}

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
	node_char *tmp = head;
	while( tmp != NULL){
		printf("%c", tmp->value);
		tmp = tmp->next;
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
	if( handler != NULL){
		node_char *old_tail = handler->tail;
		node_char *new_tail = node_char_new( value, false, handler );
		new_tail->prev = old_tail;
		new_tail->next = NULL; // dovrebbe essere già a NULL dato che era in coda...
		old_tail->next = new_tail;
		handler->tail = new_tail;
		return handler;
	}
	else {
		return NULL;
	}
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

node_char *get( list_c_handler *handler, int index){
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
				return NULL
			}
			return get( handler, index );
		}
	}
	else{
		return NULL;
	}
}

int insert( node_char *head, node_char *node, int index ){
	if( index < 0 ){
		return -1;
	}
	else if( index == 0){
		head->prev->next = node;
		node_last = last( node );
		node_last->next = head;
		head->prev = node_last;
	}
	else{
		return insert( head, node, index - 1);
	}
}

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