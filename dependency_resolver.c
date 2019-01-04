/***
 written and refactored by d7x 
git	https://github.com/d-7-x/
blog	https://d7x.promiselabs.net
---
https://www.promiselabs.net
***/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON/cJSON.h"


//function prototypes
int parse_object(cJSON *item, char *pkgparam);

static char *buf; /* output storage buffer for dependencies string */

int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("Usage: dependency_resolver <filename> <jsonfile>");
		return(-1);
	}
	
	FILE *fp;
	fp = fopen(argv[1], "r");
	
	/* check if file exists */
	if (!fp) {
		printf("Error opening file %s", argv[1]);
		return(-1);
	}

	/* allocate file contents into a string */
	long fsize;
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	rewind(fp);
	char *file_contents = malloc(fsize + 1);
	fread(file_contents, fsize, 1, fp);
	fclose(fp);
	file_contents[fsize] = 0;
	
	/* printf("%s\n", file_contents); */
	
	/* parse json stuff */
	cJSON *json = cJSON_Parse(file_contents);
	if (!json) {
		printf("Error before: [%s]\n", cJSON_GetErrorPtr());
		return(-1);
	}
	
	char *out = cJSON_Print(json);
	/* debug: print the json string */
	/* printf("%s\n",out); */
	
	free(out);
	
	/* allocate memory for the storage buffer */
	buf = malloc(100);
	
	/* parse each json string, then pass it recursively as a parameter to parse again using the subvalue */
	parse_object(json, argv[2]);
	
	/* remove last occurance of , */
	buf[strlen(buf)-2] = 0;
	
	/* Print output buffer */
	if (strlen(buf) == 0 || buf[0] == '\0') printf("No dependencies found.\n");
	else printf("[%s]\n", buf);
	
	
	//delete json object
	cJSON_Delete(json);
}

//parse json stuff using cJSON library
int parse_object(cJSON *item, char *pkgparam)
{ 
	int i;
	char *strToCompare;
	
	if (strlen(pkgparam) == 1) {
	  strcpy(strToCompare, "\"");
	  strcat(strToCompare, pkgparam);
	  strcat(strToCompare, "\"");
	} else strToCompare = pkgparam;
		/* debug printf("%s", pkgparam);
		printf("Str%s", strToCompare); */
	
	bool package_exists = false;
	
	for (i = 0 ; i < cJSON_GetArraySize(item) ; i++)
	{
	  cJSON * subitem = cJSON_GetArrayItem(item, i);
		//char *curr = cJSON_Print(subitem);
		//printf("Subitem: %s\n", curr);
	  	  
	  //parse name
	  cJSON *name = cJSON_GetObjectItem(subitem, "name");
	  char *currN = cJSON_Print(name); /* console param */
	  
	  //parse requirement
	  cJSON *requires = cJSON_GetObjectItem(subitem, "requires");
	  char *pkgreqs = cJSON_Print(requires);
	  
	  if (strcmp(currN, strToCompare) == 0) {
		  package_exists = true;
		  
			int inner;
  
			for (inner = 0 ; inner < cJSON_GetArraySize(requires) ; inner++)
			{
				cJSON * subrequires = cJSON_GetArrayItem(requires, inner);
				char *curr = cJSON_Print(subrequires);
				
				/* check if package is already in the list */
				if (strstr(buf, curr)) return;
				
				/* add stuff to output buffer */
				strcat(buf, curr);
				strcat(buf, ", ");
				
				/* run parse_object again with the new string as a param */ 
				parse_object(item, curr);
				
			}
	  }
	  
	  free(pkgreqs);
	  
	}
	  
	if (!package_exists) {
		printf("Error: no such package: %s!\n", pkgparam);
		exit(-1);
	}
}
