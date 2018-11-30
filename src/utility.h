#include "list_char.h"

/*Definizione di "booleani" true e false anche se saranno considerati come interi*/
#define true 1
#define false 0

/**
 * @brief
 * PreCondition: list_handler != NULL
 * PostCondition: dato il nome di un file.estensione come stringa e un list_c_handler,
 * inserisce nella coda i caratteri letti dal file specificato
 * @param *filename 
 * @param *list_handler
 * @return 0 se è impossbile aprire il file altrimenti 1
 */
int readFile( char *filename, list_c_handler *list_handler );