all: dependency_resolver.c cJSON/cJSON.c
	gcc -o dependency_resolver cJSON/cJSON.c dependency_resolver.c -lm
