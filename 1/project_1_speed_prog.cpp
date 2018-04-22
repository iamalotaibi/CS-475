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

	int i=0, j=0;
	float threadsTime[3][8], threadsTimeAll[3];
	threadsTimeAll[0] = 0;
	threadsTimeAll[1] = 0;
	threadsTimeAll[2] = 0;
	while((read = getline(&line, &len, fp)) != -1)
	{
		line[strcspn(line, "\n")] = 0;
		threadsTime[i][j] = atof(line);
		//printf("treadsTime[%d][%d] = %.2lf\n", i, j, threadsTime[i][j]);
		threadsTimeAll[i]+=threadsTime[i][j];
		j++;
		if( j == 8 )
		{
			i++;
			j=0;
		}
	}
	fclose(fp);
	if(line)
		free(line);

	float speedup[2];
	speedup[0] = (threadsTimeAll[0])/(threadsTimeAll[1]);
	speedup[1] = (threadsTimeAll[0])/(threadsTimeAll[2]);
	printf("         Speedup-2 =%8.2lf\n", speedup[0]);
	printf("         Speedup-4 =%8.2lf\n", speedup[1]);
	float pf[2];
	pf[0]  = ( 2./1. )*( 1. - (1./speedup[0]));
	pf[1]  = ( 4./3. )*( 1. - (1./speedup[1]));

	printf("Parallel Fraction-2 =%8.2lf\n", pf[0]);
	printf("Parallel Fraction-3 =%8.2lf\n", pf[1]);

	FILE* fptr;
	fptr = fopen("data1.cvs", "a");
	fprintf(fptr, "%.2lf, %.2lf\n", speedup[0], pf[0]);
	fprintf(fptr, "%.2lf, %.2lf\n", speedup[1], pf[1]);
	fclose(fptr);

	return 0;
}
