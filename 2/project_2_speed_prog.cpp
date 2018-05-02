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
	float threadsTime[6][4];
	while((read = getline(&line, &len, fp)) != -1)
	{
		line[strcspn(line, "\n")] = 0;
		threadsTime[i][j] = atof(line);
		j++;
		if( j == 4 )
		{
			i++;
			j=0;
		}
	}
	fclose(fp);
	if(line)
		free(line);

	float speedup[5][4], pf[5][4];
	for(i=0; i<5; i++)
	{
		for(j=0; j<4; j++)
		{
			speedup[i][j] = (threadsTime[0][j])/(threadsTime[i+1][j]);
			int currentThread = (pow(2, i+1));
			pf[i][j]  = ((float)currentThread/1. )*( 1. - (1./speedup[i][j]));
			printf("         Speedup[%d][%d] =%8.2lf\n", i, j, speedup[i][j]);
			printf("           currentThread =%8d\n", currentThread);
			printf("Parallel Fraction[%d][%d] =%8.2lf\n", i, j, pf[i][j]);
		}
	}

	return 0;
}
