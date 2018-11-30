#ifndef UTILITY_H
#define UTILITY_H

/*
	Per usare meno spazio occupato da una variabile che io considero essere booleana,
	potrei considerare 'unsigned char' di 1 solo byte per usare il booleano (memorizzando comunque interi) ma non vorrei essere frainteso
	e che venga considerato come errore ...
*/
typedef unsigned short int bool; 
/*Definizione di "booleani" true e false anche se saranno considerati come interi*/
#define true 1
#define false 0

#include "list_char.h"

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
 * PreCondition: N/A
 * PostCondition: dato il nome di un file.estensione come stringa e un list_c_handler,
 * inserisce nella coda i caratteri letti dal file specificato
 * @param *filename 
 * @param *list_handler
 * @return NULL se è impossbile aprire il file, un handler della nuova lista altrimenti
 */
list_c_handler *readFile( char *filename, list_c_handler *list_handler );

bool writeFile( char *filename, list_c_handler *list_handler );

#endif