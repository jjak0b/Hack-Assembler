#ifndef UTILITY_H
#define UTILITY_H

// #define DEBUG

/*
	Per usare meno spazio occupato da una variabile che io considero essere booleana,
	potrei considerare 'unsigned char' di 1 solo byte per usare il booleano (memorizzando comunque interi) ma non vorrei essere frainteso
	e che venga considerato come errore ...
*/
typedef unsigned short int bool; 
/*Definizione di "booleani" true e false anche se saranno considerati come interi*/
#define true 1
#define false 0

#include "list.h"


/**
 * @brief 
 * PreCondition: str e str_suffix devono essere già _strbufferhe definite con "\0"
 * PostCondition: ritorna 1 se  la _strbuffera str termina con str_suffix
 * 	es: strEndWith("la mia _strbuffera", "_strbuffera") -> 1
 * @param str 
 * @param str_suffix 
 * @return 1 (true) se la _strbuffera str termina con str_suffix 
 * @return 0 (false) altrimenti
 */
bool strEndWith( char *str, char *str_suffix);

/**
 * @brief
 * 
 * @param filename 
 * @param size dimensione array di str_filename
 * @param extension _strbuffera che definisce il nome della nuova estensione ( incluso il punto )
 * @return int 
 */
void *replaceFilenameExtension( char *filename, int size_filename, const char *extension );

/**
 * @brief
 * PreCondition: 	N/A
 * PostCondition: dato il nome di un file.estensione come _strbuffera e un list_handler,
 * inserisce nella coda i caratteri letti dal file specificato
 * @param *filename 
 * @param *list_handler
 * @return NULL se è impossbile aprire il file, un handler della nuova lista altrimenti
 */
list_handler *readFile( char *filename, list_handler *l_handler );

/**
 * @brief 
 * PreCondition: I 'value' puntati dai 'list_node' della lista gestita da 'list_handler', devono essere convertibili a char
 * PostCondition: Dato il nome di un file.estensione come _strbuffera e un list_handler,
 * scrive gli elementi della lista in un file con il nome indicato ( se non esite, viene creato )
 * @param filename 
 * @param l_handler 
 * @return int 0 se è impossbile aprire/scrivere il file, 1 altrimenti
 */
bool writeFile( char *filename, list_handler *l_handler );

/**
 * @brief indica se la stringa (o singolo carattere ) passata è un numero
 * PreCondition: str deve essere una stringa con già inserito '\0'
 * @param str 
 * @param isStr Indica se il puntatore punta a una stringa, oppure a un carattere
 * @return true se tutti i caratteri sono codificati in cifre da 0 a 9
 * @return false altrimenti
 */
bool isNumber( char *str, bool isStr);

/*To string*/

/**
 * @brief 
 * PreCondition: 	destination_size >= 2
 * 				 	se numero di caratteri di value > destination_size allora verranno inserite solo le prime 'destination_size-1' cifre meno significative nel buffer
 * 					poichè verrà inserito il carattere '\0' alla posizione destination_size-1
 *						es int_to_strbuffer( 1234, destination, 4 ) -> destination = "234".
 * PostCondition: 	Inserisce nell buffer destination le cifre del valore passato come caratteri
 * @param value 
 * @param destination 
 * @param destination_size 
 * @return true  se sono state inserite nel buffer tutte le cifre di value
 * @return false se numero di caratteri di value > destination_size 
 * 					
 */
bool int_to_strbuffer( int value, char *destination, int destination_size );

/**
 * @brief Converte un numero intero in una stringa, contenente come caratteri le cifre del numero dato
 * PreCondition: n > 0
 * @param n 
 * @return char* : il puntatore alla stringa (contenente \0)
 */
char *int_to_string( int n );


/**
 * @brief coverte una lista a una stringa
 * PreCondition: list_node *head deve contenere nodi aventi dei char come valori puntati 
 *				 size_str deve essere = NULL se non si vuole ottenere la dimensione
 * PostCondition: Ritorna il puntatore alla stringa creata con i valori della lista passata
 *				  inoltre se in size_str viene passato un indirizzo, viene assegnato il valore della dimensione all'indirizzo puntato
 * @param head : il nodo da cui partire per ottere la stringa
 * @param size_str : dimensione dell'array contente la stringa
 * @return char* 
 */
char *list_to_string( list_node *head, int *size_str );

/**
 * @brief Data una lista contenente booleani (bool definito da questa libreria), restituisce il puntatore alla stringa che ne contenente le cifre booleane '1'-'0'
 * PreCondition: 	head deve essere una lista dedicata a valori di tipo bool
 * 					size_str = NULL se non si vuole ottenere la dimensione allocata alla stringa
 * PostCondition: 	nel variabile puntata da size_str viene memorizzata la dimensione allocata della stringa restituita
 * @param head puntatore alla lista dedicata a valori di tipo bool
 * @param size_str indirizzo della variabile in cui memorizzare la dimensione della stringa
 * @return char* 
 */
char *list_binary_to_string( list_node *head, int *size_str );

/*To list*/

/**
 * @brief crea una lista con (una copia de)gli elementi della stringa (eccetto '\0')
 * 	PreCondition: str deve essere una stringa con '\0' già inserito
 * @param str 
 * @return list_handler* 
 */
list_handler *string_to_list( char *str );

/**
 * @brief Dato un numero restituisce il puntatore alla list_handler contenente la lista delle cifre intere (allocate) del valore intero dato
 * PreCondition: n > 0
 * @param n 
 * @return list_handler* 
 */
list_handler *int_to_list( int n );

/**
 * @brief dato un valore e un numero massimo di bit, resitituisce la lista delle cifre booleane (allocate) codificate dal valore intero dato in sequenza binaria
 * PreCondition: n > 0
 * PostCondition: Sono inseriti nella lista solo i primi max_bit_count bit (ovvero di tipo bool) più significativi
 * @param n : l'intero da convertire in binario
 * @param max_bit_count Il numero di bit massimi da ottenere
 * @return list_handler*  puntatore alla list_handler contenente la lista delle cifre booleane (allocate) codificate dal valore intero dato
 */
list_handler *int_to_binary_list( int n, int unsigned max_bit_count );

/**
 * @brief indica se subStr è una sottostringa in str
 * Precondition:  str e subStr devono essere stringhe con '\0'
 * 				  se start_index != NULL verrà memorizzato l'indice dove inizia subStr in str
 * @param str 
 * @param subStr 
 * @return true se subStr è una sotto stringa di str
 * @return false altrimenti
 */
bool isSubstr(const char *str, const char *subStr, int *start_index);

#endif