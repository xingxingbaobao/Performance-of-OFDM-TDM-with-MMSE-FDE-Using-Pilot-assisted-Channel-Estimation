#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "configurations.h"
#include "print.h"


//remove guard interval
void *RemovalofGI(complex *input){
  for(int x=0; x < Nc; x++){
    ReceivedSignal_no_GI[x].Re = input[x+GI].Re;
    ReceivedSignal_no_GI[x].Im = input[x+GI].Im;
  }
  for(int x=0; x < GI; x++){
    GI_Received[x].Re = input[x].Re;
    GI_Received[x].Im = input[x].Im;
  }
}


void TDM(const complex* input, complex ofdm_tdm[K][Nm]) {
    for (int y = 0; y < K; y++) {
        for (int x = 0; x < Nm; x++) {
            ofdm_tdm[y][x] = input[x + y * Nm];
        }
    }
}



//MMSE equalization weight for the nth frequency
void* MMSE(complex* inputH, double inputN) {
    complex H_conj[Nc];
    double Alpha = 0.5;

    //eq-8
    for (int x = 0; x < Nc; x++) {    

        H_conj[x].Re = inputH[x].Re;
        H_conj[x].Im = -inputH[x].Im;
        //printf("H_conj[%d]: %.4f + %.4fi\n", x, H_conj[x].Re, H_conj[x].Im);


        //another part to be added
        weight[x].Re = (Alpha * H_conj[x].Re) / (inputH[x].Re * inputH[x].Re + inputH[x].Im * inputH[x].Im + 2 * inputN);
        weight[x].Im = (Alpha * H_conj[x].Im) / (inputH[x].Re * inputH[x].Re + inputH[x].Im * inputH[x].Im + 2 * inputN);
    }
}

//FDE on received signal

void *FDE_Received(complex *input, complex* w){

  //eq-7
  for(int x = 0; x < Nc; x++){

    ReceivedSignal_FDE[x].Re = input[x].Re * w[x].Re - input[x].Im * w[x].Im;
    ReceivedSignal_FDE[x].Im = input[x].Re * w[x].Im + input[x].Im * w[x].Re;
  }
}




//Demodulation 
void DeModulate(complex* input, int* DemodulatePayload) {
    int y = 0;

    for (int x = 0; x < Nc; x++) {
        if ((input[x].Re > 0) && (input[x].Im >= 0)) { DemodulatePayload[2 * y] = DemodulatePayload[2 * y + 1] = 0; }
        else if ((input[x].Re <= 0) && (input[x].Im > 0)) { DemodulatePayload[2 * y] = 0; DemodulatePayload[2 * y + 1] = 1; }
        else if ((input[x].Re >= 0) && (input[x].Im < 0)) { DemodulatePayload[2 * y] = 1; DemodulatePayload[2 * y + 1] = 0; }
        else if ((input[x].Re < 0) && (input[x].Im <= 0)) { DemodulatePayload[2 * y] = 1; DemodulatePayload[2 * y + 1] = 1; }
        else return 0;
        //printf("Symbol %d: %d%d\n", y, DemodulatePayload[2 * y], DemodulatePayload[2 * y + 1]);
        y++;
    }
}


// Calculate bit error rate
double calculateBER(const int* original, const int* received, int dataSize) {
    int errors = 0;
    for (int i = 0; i < dataSize; i++) {
        if (original[i] != received[i]) {
            errors++;
        }
    }
    printf("-----------------------------errors-----------------------------\n %d\n", errors);
    double BER = (double)errors / dataSize;
    return (double) BER;
}
