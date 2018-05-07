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

	int i=0, j=0, tracker=0;
	float threadsTime0[6][16], threadsTime1[6][16];
	while((read = getline(&line, &len, fp)) != -1)
	{
		if(tracker == 1)
		{
			line[strcspn(line, "\n")] = 0;
			threadsTime1[i][j] = atof(line);
		}
		else
		{
			line[strcspn(line, "\n")] = 0;
			threadsTime0[i][j] = atof(line);
		}
		j++;
		if( j == 16 )
		{
			if(tracker == 1)
			{
				i++;
				j=0;
				tracker=0;
			}
			else
			{
				tracker++;
			}
		}
	}
	fclose(fp);
	if(line)
		free(line);

	float speedup[5][16];
	for(i=0; i<5; i++)
	{
		for(j=0; j<16; j++)
		{
			speedup[i][j] = (threadsTime0[0][j])/(threadsTime0[i+1][j]);
			int currentThread = (pow(2, i+1));
			printf("         Speedup[%d][%d] =%8.2lf\n", i, j, speedup[i][j]);
			FILE* fptr;
			fptr = fopen("data_type_0.csv", "a");
			if(j == 15)
				fprintf(fptr, "%.2lf\n", speedup[i][j]);
			else
				fprintf(fptr, "%.2lf, ", speedup[i][j]);
			fclose(fptr);
		}
	}
	for(i=0; i<5; i++)
	{
		for(j=0; j<16; j++)
		{
			speedup[i][j] = (threadsTime1[0][j])/(threadsTime1[i+1][j]);
			int currentThread = (pow(2, i+1));
			printf("         Speedup[%d][%d] =%8.2lf\n", i, j, speedup[i][j]);
			FILE* fptr;
			fptr = fopen("data_type_1.csv", "a");
			if(j == 15)
				fprintf(fptr, "%.2lf\n", speedup[i][j]);
			else
				fprintf(fptr, "%.2lf, ", speedup[i][j]);
			fclose(fptr);
		}
	}
	return 0;
}
