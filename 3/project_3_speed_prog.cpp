#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM									_NUM
#define NUMT	        _NUMT

int main(int argc, char *argv[]){
	FILE* fp;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("results-type-0.txt", "r");
	if(fp == NULL){
		fprintf(stderr, "FAILED TO OPEN results.txt\n");
		return 1;
	}

	int i=0, j=0;
	float threadsTime0[NUMT][NUM];
	while((read = getline(&line, &len, fp)) != -1)
	{
		line[strcspn(line, "\n")] = 0;
		threadsTime0[i][j] = atof(line);
		j++;
		if( j == NUM )
		{
			i++;
			j=0;
		}
	}
	fclose(fp);
	if(line)
		free(line);

	fp = fopen("results-type-1.txt", "r");
	if(fp == NULL){
		fprintf(stderr, "FAILED TO OPEN results.txt\n");
		return 1;
	}
	i=0;
	j=0;
	float threadsTime1[NUMT][NUM];
	while((read = getline(&line, &len, fp)) != -1)
	{
		line[strcspn(line, "\n")] = 0;
		threadsTime1[i][j] = atof(line);
		j++;
		if( j == NUM )
		{
			i++;
			j=0;
		}
	}
	fclose(fp);
	if(line)
		free(line);


	float speedup[NUMT][NUM];
	FILE* fptr;
	for(i=0; i<NUMT; i++)
	{
		for(j=0; j<NUM; j++)
		{
			speedup[i][j] = (threadsTime0[0][j])/(threadsTime0[i][j]);
			int currentThread = (pow(2, i));
			printf("         Speedup[%d][%d] =%8.2lf\n", i, j, speedup[i][j]);
			fptr = fopen("data_type_0.csv", "a");
			if(j == NUM-1)
				fprintf(fptr, "%.2lf\n", speedup[i][j]);
			else
				fprintf(fptr, "%.2lf, ", speedup[i][j]);
			fclose(fptr);
		}
	}
	for(i=0; i<NUMT; i++)
	{
		for(j=0; j<NUM; j++)
		{
			speedup[i][j] = (threadsTime1[0][j])/(threadsTime1[i][j]);
			int currentThread = (pow(2, i));
			printf("         Speedup[%d][%d] =%8.2lf\n", i, j, speedup[i][j]);
			fptr = fopen("data_type_1.csv", "a");
			if(j == NUM-1)
				fprintf(fptr, "%.2lf\n", speedup[i][j]);
			else
				fprintf(fptr, "%.2lf, ", speedup[i][j]);
			fclose(fptr);
		}
	}
	return 0;
}
