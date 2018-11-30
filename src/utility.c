#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include "list_char.h"

/**
 * @brief
 * PreCondition: N/A
 * PostCondition: dato il nome di un file.estensione come stringa e un list_c_handler,
 * inserisce nella coda i caratteri letti dal file specificato
 * @param *filename 
 * @param *list_handler
 * @return 0 se è impossbile aprire il file, 1 altrimenti
 */
int readFile( char *filename, list_c_handler *list_handler ){
    FILE *fin;
    char c = "\0";
    fin = fopen( filename, "r");
    if( fin != NULL){
        while( !feof( fin ) ){
            fscanf( fin, "%c", &c);
            list_handler = enqueue( list_handler, c );
        }
        fclose( fin );
    }
    else{
        return false;
    }
    return true;
}

int extractFilename( char *filename, char *returnBuffer){
}