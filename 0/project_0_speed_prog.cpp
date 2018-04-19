#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]){
	FILE* fp;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("results.txt", "r");
	if(fp == NULL){
		fprintf(stderr, "FAILED TO OPEN results.txt\n");
		return 1;
	}

	int i=0;
	float threadsTime[2];
	while((read = getline(&line, &len, fp)) != -1){
		line[strcspn(line, "\n")] = 0;
		threadsTime[i] = atof(line);
		i++;
	}
	fclose(fp);
	if(line)
		free(line);
	float speedup = (threadsTime[0])/(threadsTime[1]);
	printf("            Speedup =%8.2lf\n", speedup);
	float pf = ( 4./3. )*( 1. - (1./speedup));
	printf("  Parallel Fraction =%8.2lf\n", pf);

	return 0;
}
