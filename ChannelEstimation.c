#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "configurations.h"
#include "print.h"



//pilot signal FFT

void* generatePq(complex* input) {
    
    FFT(input, Nm, scratch);
}

//Time-domain First-order Filtering, here assuming that only one frame, thus set the filter_coefficient to 1
void* TFF(complex* input) {
    int filter_coefficient = 1;

    //eq-10
    for (int t = 0; t < Nm; t++) {
       filtered_GI[t].Re = filter_coefficient * (input[t].Re) + (1 - filter_coefficient) * input[t].Re;
       filtered_GI[t].Re = filter_coefficient * (input[t].Re) + (1 - filter_coefficient) * input[t].Re;
       
    }

    //eq-11，The Nm-point FFT is applied to decompose the filtered pilot signal into Nm frequency components
    FFT(filtered_GI, Nm, scratch);

    for (int i = 0; i < Nm; i++) {
        filtered_GI_freq[i].Re =  filtered_GI[i].Re;
        filtered_GI_freq[i].Im =  filtered_GI[i].Im;
    }
}



//Instantaneous channel gain estimate
void* channel_gain_estimate(complex* input_up,complex* input_down ) {
    int filter_coefficient = 1;

    //eq-12
    for (int q = 0; q < Nm; q++) {

        double denominator = input_down[q].Re * input_down[q].Re + input_down[q].Im * input_down[q].Im;

        H_g[q].Re = (input_up[q].Re * input_down[q].Re + input_up[q].Im * input_down[q].Im) / denominator;
        H_g[q].Im = (input_up[q].Im * input_down[q].Re - input_up[q].Re * input_down[q].Im) / denominator;

    }

}


//----------------------------------------------------------------------------------------------------------------------//
//Frequency-domain Interpolation

void* Highresolution(complex* input) {

    //Nm-point IFFT is performed
    IFFT(input, Nm, scratch);
    //print_complex_vector("input \n ", input, Nm);
    for (int x = 0; x < Nm; x++) {
        hba[x].Re = (1.0 / Nm) * input[x].Re;
        hba[x].Im = (1.0 / Nm) * input[x].Im;
    }
    print_complex_vector("----------------------------instantaneous channel gain in time---------------------------- \n ", hba, Nm);
    //Nc-point FFT is applied to obtain the interpolated channel gain estimates for all Nc frequency components
    FFT(hba, Nc, scratch);
    for (int x = 0; x < Nc; x++) {
        Hen[x].Re = (1.0 / Nc) * hba[x].Re;
        Hen[x].Im = (1.0 / Nc) * hba[x].Im;
    }

}


//----------------------------------------------------------------------------------------------------------------------//
//Noise Power Estimation
double noise_estimate(complex* input_GI_estimate, complex* input_channelgain, complex* input_GI) {

    //eq-18
    for (int q = 0; q < Nm; q++) {

        GINoise[q].Re = input_GI_estimate[q].Re - (input_channelgain[q].Re * input_GI[q].Re - input_channelgain[q].Im * input_GI[q].Im);
        GINoise[q].Im = input_GI_estimate[q].Im - (input_channelgain[q].Re * input_GI[q].Im + input_channelgain[q].Im * input_GI[q].Re);
    }
    print_complex_vector("----------------------------estimated noise---------------------------- \n ", GINoise, Nm);
    double sum_abs = 0.0;

    //eq-19
    for (int q = 0; q < Nm; q++) {

        double abs[Nm];

        abs[q] = GINoise[q].Re * GINoise[q].Re + GINoise[q].Im * GINoise[q].Im;
        sum_abs += abs[q];

    }
    double n_e = sum_abs / Nm;

    return n_e;
}
