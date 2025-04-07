#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "garbage.h"

int main(int argc,char ** agrv)
{
	char * category=NULL;
	garbage_init();
	category = garbage_category(category);
	printf("category: %s \n",category);
	garbage_final();
	if(category)
		free(category);
	return 0;
}

