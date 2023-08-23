#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "configurations.h"
#include "print.h"



double* Gaussian(int n) {
    int i;
    double* values = (double*)calloc(n, sizeof(double));// allocate address to store array

    if (values) {

        for (i = 0; i < n; i += 2) {
            double x, y;
            
            x = rand() / (double)RAND_MAX; //Random number between 0 and 1
            y = rand() / (double)RAND_MAX;

            values[i]     =  cos(2.0 * PI * y)* sqrt(-2.0 * log(x));
            values[i + 1] =  sin(2.0 * PI * y)* sqrt(-2.0 * log(x));

            //printf("values[%d]: %lf\n", i, values[i]);
            //printf("values[%d]: %lf\n", i + 1, values[i + 1]);
        }
    }
    return values;
}


//Rayleigh Channel
void* RayleighChannel(complex* x, int path, int length) {
    int outputsize = path + length - 1;
    double noise_variance = (2 * N0) / Tc;
    double* noise_elements;
    double* rayleigh_elements_Re;
    double* rayleigh_elements_Im;
    noise_elements = Gaussian(outputsize);
    rayleigh_elements_Re = Gaussian(path);
    rayleigh_elements_Im = Gaussian(path);


    for (int i = 0; i < outputsize; i++) {

        //generate AWGN
        noise[i].Re = sqrt(noise_variance / 2) * noise_elements[i];
        noise[i].Im = sqrt(noise_variance / 2) * noise_elements[i];

        for (int j = 0; j < path; j++) {

            //generate rayleigh channel response
            rayleigh[j].Re = rayleigh_elements_Re[j];
            rayleigh[j].Im = rayleigh_elements_Im[j];

            //convolution
              if (i - j >= 0 && i - j < length) {
                    ReceivedSignal[i].Re += (rayleigh[j].Re * x[i-j].Re - rayleigh[j].Im * x[i-j].Im);
                    ReceivedSignal[i].Im += (rayleigh[j].Re * x[i-j].Im + rayleigh[j].Im * x[i-j].Re);
              }
        }

       }
    //eq-5,add AWGN
    for (int i = 0; i < outputsize; i++) {
        ReceivedSignal[i].Re = sqrt(2 * P) * (ReceivedSignal[i].Re + noise[i].Re);
        ReceivedSignal[i].Im = sqrt(2 * P) * (ReceivedSignal[i].Im + noise[i].Im);
    }

}


