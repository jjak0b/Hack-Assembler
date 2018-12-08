#ifndef LIST_H
#define LIST_H

struct list_handler; // typedef struct list_handler list_handler; /*Gestore della lista*/
struct list_node; // typedef struct list_node list_node; /*Bi-lista di caratteri*/

typedef struct list_handler {
	struct list_node *head; 	/*Punta alla testa ( primo elemento ) della lista*/
	struct list_node *tail; 	/*Punta alla coda ( ultimo elemento valido non NULL ) della lista*/
	struct list_node *current; 	/*Punta al nodo attuale preso in considerazione, il chiamante itera tra i nodi con next() o prev()*/
	int index; 				/*Indice dell'elemento puntato (current)*/
} list_handler;

typedef struct list_node {
	void *value;						/*puntatore a un indirizzo generico*/
	struct list_node *next;				/*puntatore al nodo successivo*/
	struct list_node *prev;				/*puntatore al nodo precedente*/
	/*
	*	gestore della lista:
		- ogni nodo ha un gestore come riferimento
		- tutti gli elementi di una lista devono avere lo stesso handler
		- il chiamante richiama le funzioni per iterare tra i nodi ed ha sempre i riferimenti degli estrami puntati
	*/
	struct list_handler *handler;
} list_node;

#include "utility.h"

/**
 * @brief
 * PreCondition: N/A
 * PostCondition: Dato un nodo, ritorna la sua lunghezza della lista a seguire (o precedere) da tale nodo
 * se b_reachLast != 0 conta fino all'ultimo elemento della lista ritornado lunghezza positiva
 * altrimenti conta fino al primo elemento ritornando lunghezza positiva
 * @param head 
 * @return La lunghezza della lista
 */
int size( list_node *head, bool b_reachLast);

/**
 * @brief Data una lista ritorna il primo elemento della lista 
 * se la lista è vuota ritorna NULL
 * 
 * @param node 
 * @return list_node*  Il puntatore al primo nodo della lista
 */
list_node *first( list_node *node );

/**
 * @brief Datp una lista ritorna l'ultimo elemento della lista 
 * se la lista è vuota ritorna NULL
 * 
 * @param head 
 * @return list_node*  Il puntatore all'ultimo nodo della lista
 */
list_node *last( list_node *head );

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
list_node *get( list_node *head, int index );
/*
// Obsoleto e non testato
list_node *get_from_handler( list_handler *handler, int index);
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
list_node *insert( list_node *head, list_node *node, int index );
/*
// Obosleto e non testato
int insert( list_handler *handler, char value, int index )
*/

/**
 * @brief Imposta l'handler per tutti i nodi della lista ( precedenti e successivi)
 * PreCondition: handler != NULL
 * @param node 
 * @param handler 
 */
void *setupNodesHandler( list_node *node, list_handler *handler);

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
list_node *list_node_new( void *value, const bool b_create_handler, list_handler *handler );

list_node *list_node_reverse( list_node *head );

/**
 * @brief Stampa i valori degli elementi di una
 * Precondition: valore puntato da head->value deve essere un tipo supportato da printf (es char*, char, int, float, ecc..)
 * 
 * @param format formato di stampa del valore: la stringa viene passata a printf, quindi usare come se fosse tale funzione
 * @param head testa della lista da cui verranno stampati gli elementi
 */
void list_node_print( const char *format, list_node *head);

list_node *next( list_handler *handler );

list_node *prev( list_handler *handler );

list_handler *enqueue( list_handler *handler, void *value);

list_node *dequeue( list_handler *handler );

bool isEqual( list_node *head1, list_node *head2 );

void delete_list( list_handler *handler, bool b_delete_values );

#endif