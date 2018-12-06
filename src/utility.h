#ifndef UTILITY_H
#define UTILITY_H

#define DEBUG /*IMPORTANTE: Disabilitare quando prima di consegnare*/
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
 * PreCondition: str e str_suffix devono essere già stringhe definite con "\0"
 * PostCondition: ritorna 1 se  la stringa str termina con str_suffix
 * 	es: strEndWith("la mia stringa", "stringa") -> 1
 * @param str 
 * @param str_suffix 
 * @return 1 (true) se la stringa str termina con str_suffix 
 * @return 0 (false) altrimenti
 */
bool strEndWith( char *str, char *str_suffix);

/**
 * @brief
 * 
 * @param filename 
 * @param size dimensione array di str_filename
 * @param extension stringa che definisce il nome della nuova estensione ( incluso il punto )
 * @return int 
 */
void *replaceFilenameExtension( char *filename, int size_filename, const char *extension );

/**
 * @brief
 * PreCondition: 	N/A
 * PostCondition: dato il nome di un file.estensione come stringa e un list_handler,
 * inserisce nella coda i caratteri letti dal file specificato
 * @param *filename 
 * @param *list_handler
 * @return NULL se è impossbile aprire il file, un handler della nuova lista altrimenti
 */
list_handler *readFile( char *filename, list_handler *l_handler );

/**
 * @brief 
 * PreCondition: I 'value' puntati dai 'list_node' della lista gestita da 'list_handler', devono essere convertibili a char
 * PostCondition: Dato il nome di un file.estensione come stringa e un list_handler,
 * scrive gli elementi della lista in un file con il nome indicato ( se non esite, viene creato )
 * @param filename 
 * @param l_handler 
 * @return int 0 se è impossbile aprire/scrivere il file, 1 altrimenti
 */
bool writeFile( char *filename, list_handler *l_handler );

/**
 * @brief 
 * PreCondition: 	destination_size >= 2
 * 				 	se numero di caratteri di value > destination_size allora verranno inserite solo le prime 'destination_size-1' cifre meno significative nel buffer
 * 					poichè verrà inserito il carattere '\0' alla posizione destination_size-1
 *						es int_toString( 1234, destination, 4 ) -> destination = "234".
 * PostCondition: 	Inserisce nell buffer destination le cifre del valore passato come caratteri
 * @param value 
 * @param destination 
 * @param destination_size 
 * @return true  se sono state inserite nel buffer tutte le cifre di value
 * @return false se numero di caratteri di value > destination_size 
 * 					
 */
bool int_toString( int value, char *destination, int destination_size );

bool isNumber( char *str );
#endif