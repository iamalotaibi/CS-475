#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const float GRAIN_GROWS_PER_MONTH = 8.0;
const float ONE_DEER_EATS_PER_MONTH = 0.5;

const float AVG_PRECIP_PER_MONTH = 6.0;
const float AMP_PRECIP_PER_MONTH = 6.0;
const float RANDOM_PRECIP = 2.0;

const float AVG_TEMP = 50.0;
const float AMP_TEMP = 20.0;
const float RANDOM_TEMP = 10.0;

const float MIDTEMP = 40.0;
const float MIDPRECIP = 10.0;

int NowYear;  // 2014 - 2019
int NowMonth; // 0 - 11

float NowPrecip; // inches of rain per month
float NowTemp;   // temperature this month
float NowHeight; // grain height in inches
int NowNumDeer;  // current deer populationc
void Particular_Month(unsigned int seed);
float Update_Height();
void Watcher();
int Quantity_Interactions();
int God();
float Ranf(float low, float high, unsigned int *seed);

int main(int argc, char *argv[]) {
  srand(time(NULL));
  NowNumDeer = 1;
  NowHeight = 1.;
  NowMonth = 0;
  NowYear = 2014;
  omp_set_num_threads(4);
  while (NowYear < 2020) {
    float NewNowHeight = -1;
    float NewNowNumDeer = -1;
    int luck = -1;
#pragma omp parallel sections
    {
#pragma omp section
      {
        // GrainDeer
        NewNowNumDeer = Quantity_Interactions();
      }
#pragma omp section
      {
        // Grain
        NewNowHeight = Update_Height();
      }
#pragma omp section
      {
        // Watcher
      }
#pragma omp section
      {
        // God
        luck = God();
      }
      // implied barrier: all sections must complete before we get here
    }
    // DoneComputing barrier
#pragma omp barrier
#pragma omp parallel sections
    {
#pragma omp section
      {
        // GrainDeer
        NowNumDeer = NewNowNumDeer;
      }
#pragma omp section
      {
        // Grain
        if (luck == -1) // nothing
          NowHeight = NewNowHeight;
        else if (luck == 0) // bad
          NowHeight = NewNowHeight - 2;
        else if (luck == 1) // good
          NowHeight = NewNowHeight + 2;
        else if (luck == 2) // perfect
          NowHeight = NewNowHeight * 2;
      }
#pragma omp section
      {
        // Watcher
      }
#pragma omp section
      {
        // God
      }
    }
    // DoneAssigning barrier
#pragma omp barrier

    FILE *fp;
    fp = fopen("results.csv", "a");
    fprintf(fp, "%d,%.2f,%.2f,%d,%.2f,%d\n", NowMonth + ((NowYear - 2014) * 12) + 1,
            NowTemp, NowPrecip, NowNumDeer, NowHeight, luck);
    fclose(fp);
#pragma omp parallel sections
    {
#pragma omp section
      {
        // GrainDeer
      }
#pragma omp section
      {
        // Grain
      }
#pragma omp section
      {
        // Watcher
        Watcher();
      }
#pragma omp section
      {
        // God
      }
    }
// DonePrinting barrier
#pragma omp barrier
  }
  return 0;
}
void Particular_Month(unsigned int seed) {
  float ang = (30. * (float)NowMonth + 15.) * (M_PI / 180.);
  float temp = AVG_TEMP - AMP_TEMP * cos(ang);

  NowTemp = temp + Ranf(-RANDOM_TEMP, RANDOM_TEMP, &seed);
  float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
  NowPrecip = precip + Ranf(-RANDOM_PRECIP, RANDOM_PRECIP, &seed);
  if (NowPrecip < 0.)
    NowPrecip = 0.;
}
float Update_Height() {
  float tempFactor = (float)exp(-pow(((NowTemp - MIDTEMP) / 10), 2));
  float precipFactor = (float)exp(-pow(((NowPrecip - MIDPRECIP) / 10), 2));
  float NewNowHeight = NowHeight;
  NewNowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
  NewNowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
  if (NewNowHeight < 0.)
    NewNowHeight = 0.;
  return NewNowHeight;
}
void Watcher() {
  // print results
  printf("NowNumDeer=%d\n", NowNumDeer);
  printf("NowHeight=%.2f\n", NowHeight);
  printf("NowMonth=%d\n", NowMonth);
  printf("NowYear=%d\n", NowYear);
  printf("NowNumDeer=%d\n", NowNumDeer);
  // increment month
  NowMonth++;
  if (NowMonth == 12) {
    NowMonth = 0;
    NowYear++;
  }
  // calculate new temperature and precipitation
  Particular_Month(NowYear * NowMonth);
  printf("==========================%d/%d=========================\n",
         NowMonth + 1, NowYear);
}
int Quantity_Interactions() {
  int NewNowNumDeer = NowNumDeer;
  if (NowNumDeer > NowHeight)
    NewNowNumDeer--;
  else if (NowNumDeer < NowHeight)
    NewNowNumDeer++;
  if (NewNowNumDeer < 0)
    NewNowNumDeer = 0;
  return NewNowNumDeer;
}
int God() {
  int random = rand() % 100;
  if (random < 60) {
    // nothing
    return -1;
  } else if (random < 75) {
    // bad
    printf("---------------------GOD: YOU HAVE CURSED THIS LAND THIS MONTH, I "
           "WILL PUNISH YOU (lost 2)\n");
    return 0;
  } else if (random < 90) {
    // good
    printf("---------------------GOD: YOU AIN\'T THAT BAD OF A PERSON (gained "
           "2)\n");
    return 1;
  } else {
    // perfect
    printf("---------------------GOD: THIS IS YOUR LUCKY MONTH!!!!!!! (gained "
           "double)\n");
    return 2;
  }
}
float Ranf(float low, float high, unsigned int *seed) {
  float r = (float)rand_r(seed); // 0 - RAND_MAX
  return (low + r * (high - low) / (float)RAND_MAX);
}
