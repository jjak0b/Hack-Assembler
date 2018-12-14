#ifndef LIST_H
#define LIST_H

struct list_handler; // typedef struct list_handler list_handler; /*Gestore della lista*/
struct list_node; // typedef struct list_node list_node; /*Bi-lista di caratteri*/
/*
 * L'idea di questa struttura era quella di trovarsi una unica struttura di riferimento che accumunava i nodi della lista per evitare inutili iterazioni
 * ma si è rivelata molto spesso problematica durante lo sviluppo soprattutto per quanto riguarda l'aggiornamento della testa e della coda che non sempre accadeva;
 * senza condiderare il fatto che index e current non sono stati mai utilizzati effettivamente
*/
typedef struct list_handler {
	struct list_node *head; 	/*Punta alla testa ( primo elemento ) della lista*/
	struct list_node *tail; 	/*Punta alla coda ( ultimo elemento valido non NULL ) della lista*/
	struct list_node *current; 	/*Punta al nodo attuale preso in considerazione, il chiamante itera tra i nodi con next() o prev()*/
	int index; 				/*Indice dell'elemento puntato (current)*/ // supportato ed utilizzato da pochissime funzioni
} list_handler;

typedef struct list_node {
	void *value;						/*	puntatore a un indirizzo generico non specificato: Spiegazione...
										 *	Poiché Malloc restituisce un puntatore a void di uno spazio allocato di una dimensione X
										 *	non consente di accedere direttamente al valore puntato, poichè appunto il dato sarebbe "void", quindi sarà il chiamante
										 *	a occuparsi di utilizzare il puntatore del tipo di dato corretto.
										 *	inoltre, grazie a questo è possibile utilizzare la stessa struttura dati per inserire anche tipi di dato diversi tra loro
										 * 	senza riscrivere stesse porzioni di codice con il tipo cambiato inutilmente
										 */ 
	struct list_node *next;				/*puntatore al nodo successivo*/
	struct list_node *prev;				/*puntatore al nodo precedente*/
	struct list_handler *handler;		/*
										 *	Idea alla base del gestore/handler della lista:
										 *	- ogni nodo ha un gestore come riferimento
										 *	- tutti gli elementi di una lista devono avere lo stesso handler
										 *	- il chiamante richiama le funzioni per iterare tra i nodi ed ha sempre i riferimenti degli estrami puntati
										 */
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

/** // NON TESTATA E NON USATA
 * @brief Il nodo alla "index-esima" posizione
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

/**
 * @brief Dealloca il valore ed il nodo stesso, collegando prima tra loro il nodo successivo e precedente
 * 
 * @param node 
 */
void delete_node( list_node *node);

/**
 * @brief data una lista restituisce la lista invertita
 * 
 * @param head 
 * @return list_node* : puntatore alla nuova testa
 */
list_node *list_node_reverse( list_node *head );

/**
 * @brief Stampa i valori degli elementi di una
 * Precondition: valore puntato da head->value deve essere un tipo supportato da printf (es char*, char, int, float, ecc..)
 * 
 * @param format formato di stampa del valore: la stringa viene passata a printf, quindi usare come se fosse tale funzione
 * @param head testa della lista da cui verranno stampati gli elementi
 */
void list_node_print( const char *format, list_node *head);

/** // NON TESTATA E NON USATA
 * @brief restituisce il puntatore al nodo successivo al nodo attuale puntato dall'handler dato e aggiorna l'indice attuale
 * 
 * @param handler 
 * @return list_node* 
 */
list_node *next( list_handler *handler );

/** // NON TESTATA E NON USATA
 * @brief restituisce il puntatore al nodo precedente al nodo attuale puntato dall'handler dato e aggiorna l'indice attuale
 * 
 * @param handler 
 * @return list_node* 
 */
list_node *prev( list_handler *handler );

/**
 * @brief istanzia un nuovo nodo e vi assegna il puntatore dato, e lo aggiunge alla coda del list_handler e ne restituisce l'handler con la lista aggiornata
 * PostCondition: se handler = NULL viene istanziata e restituita un nuovo handler ( a cui assegna comunque il puntatore dato alla lista )
 * @param handler 
 * @param value 
 * @return list_handler* 
 */
list_handler *enqueue( list_handler *handler, void *value);

/**
 * @brief riomuove dalla coda l'ultimo nodo e ne restituisce il puntatore
 * PostCondition: se handler = NULL restiuisce NULL
 * @param handler 
 * @param value 
 * @return list_node* del nodo rimosso
 */
list_node *dequeue( list_handler *handler );

/** // NON TESTATA E NON USATA
 * @brief Indica se i valori puntati da ogni nodo delle due liste sono uguali
 * 
 * @param head1 
 * @param head2 
 * @return true se tutti i valori puntati da ogni nodo delle due liste sono uguali
 * @return false altrimenti
 */
bool isEqual( list_node *head1, list_node *head2 );

/**
 * @brief Dealloca i nodi dell'handler ed l'handler stesso. se b_delete_values = true allora dealloca i valori puntati dai nodi
 * 
 * @param handler 
 * @param b_delete_values  (true) indica se deallocare i valori puntati dai nodi .(false) altrimenti
 */
void delete_list( list_handler *handler, bool b_delete_values );

/**
 * @brief Collega la lista  l_source dal puntatore dato a l_destination dalla testa assoluta
 * es: *d-e-s-t -> s-o-u-r-c-e
 * PostCondition: in ogni nodo di l_source viene assegnato l'handler di l_destination
 * @param l_destination 
 * @param l_source 
 * @return list_node* : puntatore alla testa dell'intera lista collegata
 */
list_node *append( list_node *l_destination, list_node *l_source);

#endif