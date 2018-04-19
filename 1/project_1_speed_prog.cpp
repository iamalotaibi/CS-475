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
	float threadsTime[3][8];
	while((read = getline(&line, &len, fp)) != -1)
	{
		line[strcspn(line, "\n")] = 0;
		threadsTime[i][j] = atof(line);
		//printf("treadsTime[%d][%d] = %.2lf\n", i, j, threadsTime[i][j]);
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

	float threadsTimeMax[3];
	threadsTimeMax[0] = threadsTime[0][7];
	threadsTimeMax[1] = threadsTime[1][7];
	threadsTimeMax[2] = threadsTime[2][7];

	float speedup[2];
	speedup[0] = (threadsTimeMax[0])/(threadsTimeMax[1]);
	speedup[1] = (threadsTimeMax[0])/(threadsTimeMax[2]);
	printf("         Speedup[0] =%8.2lf\n", speedup[0]);
	printf("         Speedup[1] =%8.2lf\n", speedup[1]);
	float pf[2];
	pf[0]  = ( 2./1. )*( 1. - (1./speedup[0]));
	pf[1]  = ( 4./3. )*( 1. - (1./speedup[1]));

	printf("Parallel Fraction[0] =%7.2lf\n", pf[0]);
	printf("Parallel Fraction[1] =%7.2lf\n", pf[1]);

	return 0;
}
