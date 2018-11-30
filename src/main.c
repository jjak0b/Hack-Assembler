#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "list_char.h"

list_c_handler *assembler( list_c_handler *lh_input ){
	list_c_handler *lh_output = NULL;
	node_char *node_current = lh_input->head;
	while( node_current != NULL ){
		// do stuff
		node_current = next(lh_input);
	}
	return lh_output;
}

int main( int nArgs, char **args ){
	char *filename = args[1];

	list_c_handler *lh_input = NULL, *lh_output = NULL;
	if( filename == NULL ){
		printf("FIle input non speficato\n");
	}
	else if( !readFile( filename, lh_input ) ){
		printf("Impossbile aprire il file '%s'", filename );
	}
	else{
		// lh_output = assembler( lh_input );

	}
	return 0;
}

