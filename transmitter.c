#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "configurations.h"
#include "print.h"


//OPSK,insert chu-sequence and divide into K slots
void* Datamod(int* Input) {

    int j = 0, a = 0;

    //QPSK
    for (int i = 0; i < Nc - Nm; i++) {
        if ((Input[2 * j] == 0) && (Input[2 * j + 1] == 0)) { symbols[i].Re = 1; symbols[i].Im = 1; }
        else if ((Input[2 * j] == 0) && (Input[2 * j + 1] == 1)) { symbols[i].Re = -1; symbols[i].Im = 1; }
        else if ((Input[2 * j] == 1) && (Input[2 * j + 1] == 0)) { symbols[i].Re = 1; symbols[i].Im = -1; }
        else { symbols[i].Re = -1; symbols[i].Im = -1; }
        //printf("symbols[%d] = %f + %fi\n", i, symbols[i].Re, symbols[i].Im);
        j++;
    }

    //insert chu-sequence on k-1 slot
    for (int i = Nc - Nm; i < Nc; i++) {
        symbols[i].Re = cos(PI * pow(i, 2) / Nc);
        symbols[i].Im = sin(PI * pow(i, 2) / Nc);
        //printf("chu-sequence[%d] = %f + %fi\n", i, symbols[i].Re, symbols[i].Im);
    }

    //TDM 
    for (int k = 0; k < Nm; k++) {
        for (int i = 0; i < Nm; i++) {
            //printf("a = %d\n", a);
            d[k][i] = symbols[a]; //divide all carriers into k slots
            //printf("d[%d][%d] = %f + %fi\n", k, i, d[k][i].Re, d[k][i].Im);
            a++;
        }
    }
}

//FFT
void FFT(complex* input, int n, complex* tmp) {
    if (n > 1) {
        complex z, twiddle_factor, * odd, * even;
        even = tmp; odd = tmp + n / 2;
        for (int t = 0; t < n / 2; t++) {
            even[t] = input[2 * t];
            odd[t] = input[2 * t + 1];
        }
        FFT(even, n / 2, input);
        FFT(odd, n / 2, input);
        for (int m = 0; m < n / 2; m++) {
            
            twiddle_factor.Re = cos(2 * PI * m / (double)n);
            twiddle_factor.Im = -sin(2 * PI * m / (double)n);
            
            z.Re = twiddle_factor.Re * odd[m].Re - twiddle_factor.Im * odd[m].Im;
            z.Im = twiddle_factor.Re * odd[m].Im + twiddle_factor.Im * odd[m].Re;
            input[m].Re = even[m].Re + z.Re;
            input[m].Im = even[m].Im + z.Im;
            input[m + n / 2].Re = (even[m].Re - z.Re);
            input[m + n / 2].Im = (even[m].Im - z.Im);
        }
    }
    return;
}

//IFFT
void IFFT(complex* input, int n, complex* tmp)
{
    if (n > 1)
    {
        complex z, twiddle_factor, * odd, * even;
        even = tmp; odd = tmp + n / 2;
        for (int k = 0; k < n / 2; k++)
        {
            even[k] = input[2 * k];
            odd[k] = input[2 * k + 1];
        }
        IFFT(even, n / 2, input);
        IFFT(odd, n / 2, input);
        for (int m = 0; m < n / 2; m++)
        {
            twiddle_factor.Re = cos(2 * PI * m / (double)n);
            twiddle_factor.Im = sin(2 * PI * m / (double)n);
            z.Re = twiddle_factor.Re * odd[m].Re - twiddle_factor.Im * odd[m].Im;
            z.Im = twiddle_factor.Re * odd[m].Im + twiddle_factor.Im * odd[m].Re;
            input[m].Re = even[m].Re + z.Re;
            input[m].Im = even[m].Im + z.Im;
            input[m + n / 2].Re = (even[m].Re - z.Re);
            input[m + n / 2].Im = (even[m].Im - z.Im);
        }
    }
    return;
}

//eq-8,FFT with 1/Nc
void R_n(complex* input, int n) {
    for (int x = 0; x < n; x++) {
        input[x].Re = (1.0 / Nc) *input[x].Re;
        input[x].Im = (1.0 / Nc) *input[x].Im;
    }
}


void final_ifft(complex* input, int n) {
    for (int x = 0; x < n; x++) {
        input[x].Re = (1 / sqrt(Nm)) * input[x].Re;
        input[x].Im = (1 / sqrt(Nm)) * input[x].Im;
    }
}

//Add Guard interval to the frame
void* AddGI(complex* signal) {
    complex sig_GI[Nm];

    for (int x = 0; x < Nm; x++) {
        sig_GI[x] = signal[x + (Nc - GI)];
    }
    for (int x = 0; x < Nc; x++) {
        signal_with_GI[x + GI].Re = signal[x].Re;
        signal_with_GI[x + GI].Im = signal[x].Im;
    }
    for (int x = 0; x < GI; x++) {
        signal_with_GI[x].Re = sig_GI[x].Re;
        signal_with_GI[x].Im = sig_GI[x].Im;
    }
}



//HPA with transmit power
void* HPA(complex* input) {

    double abs[Nc + GI];

    //eq-3
    for (int i = 0; i < Nc + GI; i++) {

        abs[i] = sqrt(input[i].Re * input[i].Re + input[i].Im * input[i].Im);

        if (abs[i] < beta) {
            S_g_hat[i].Re = sqrt(P) * input[i].Re;
            S_g_hat[i].Im = sqrt(P) * input[i].Im;
        }

        else {
            double coefficient = beta / abs[i];

            S_g_hat[i].Re = sqrt(P) * coefficient * input[i].Re;
            S_g_hat[i].Im = sqrt(P) * coefficient * input[i].Im;
        }
    }
}