#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NUM									_NUM
#define NUMT	        _NUMT
#define TYPE	        			_TYPE

struct s
{
	float value;
  int pad[NUM];
} Array[4];

int main( int argc, char *argv[ ] )
{
#ifndef _OPENMP
  fprintf( stderr, "OpenMP is not available\n" );
  return 1;
#endif
	omp_set_num_threads( NUMT );
	int SomeBigNumber = 1000000000;
  double time0, time1 = 0.;
	if(TYPE == 0)
	{
		time0 = omp_get_wtime( );
		#pragma omp parallel for
		for( int i = 0; i < 4; i++ )
		{
			float tmp = Array[ i ].value;
			for( int j = 0; j < SomeBigNumber; j++ )
			{
				tmp = tmp + 2.;
			}
			Array[ i ].value = tmp;
		}
		time1 = omp_get_wtime( );
	}
	else
	{
		time0 = omp_get_wtime( );
		#pragma omp parallel for
		for( int i = 0; i < 4; i++ )
		{
			for( int j = 0; j < SomeBigNumber; j++ )
			{
				Array[ i ].value = Array[ i ].value + 2.;
			}
		}
		time1 = omp_get_wtime( );
	}
	double timeSpent = (time1-time0);
	printf("       Elapsed Time = %8.2lf microseconds\n", timeSpent);

	FILE* fp;
	if(TYPE == 0)
	{
		fp = fopen("results-type-0.txt", "a");
		fprintf(fp, "%.2lf\n", timeSpent);
	}
	else
	{
		fp = fopen("results-type-1.txt", "a");
		fprintf(fp, "%.2lf\n", timeSpent);
	}
	fclose(fp);
	return 0;

}
