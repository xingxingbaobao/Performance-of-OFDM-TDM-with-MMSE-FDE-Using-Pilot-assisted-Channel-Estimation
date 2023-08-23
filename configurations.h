#ifndef _configurations_H_
#define _configurations_H_


#define Nc 256
#define K 16
#define Nm 16 
#define FFT_Window 16
#define max_size 1024
#define GI 16 // 1/16
#define P 1 //Amplifier efficiency
#define N0 0.0003
#define Tc 256
#define beta 10 //HPA saturation level
#define M 2 //Modulation number -> QPSK
#define PI	3.14159265358979323846264338327950288
#define data_size 512
#define SAMPLE_RATE 44100
#define NUM_PATHS 8
#define MAX_DELAY 10
#define CARRIER_FREQ 1000
#define G_frame 4

typedef float real;
typedef struct{
  real Re; real Im;
  } complex;

complex symbols[Nc], d[Nm][Nm], s_g[Nc], signal_with_GI[Nc + GI],
S_g_hat[279], ReceivedSignal[279], noise[279],
ReceivedSignal_no_GI[Nc], ReceivedSignal_FDE[Nc], GI_Received[GI],
r[K][Nm], R[Nc], scratch[FFT_Window], signal[Nc], s_gg[Nc], filtered_GI[GI],
GI_Received_[12][GI], ofdm_tdm[K][Nm], rayleigh[NUM_PATHS], H_g[Nm], hba[Nm], Hen[Nc], GINoise[Nm],weight[Nc], filtered_GI_freq[GI];

double n_e;

//------------------------------------------------------------------Tx---------------------------------------------------------------------------//
void* Datamod(int* Input);
void FFT( complex *v, int n, complex *tmp );
void R_n(complex *input, int n);
void IFFT( complex *v, int n, complex *tmp );
void final_ifft(complex *input, int n);
void *AddGI(complex *signal);
void *HPA(complex *IF_in);
//-----------------------------------------------------------------Channel-----------------------------------------------------------------------//
double* Gaussian(int n);
void* RayleighChannel(complex* x, int path, int length);
//------------------------------------------------------------------Rx---------------------------------------------------------------------------//
void *RemovalofGI(complex *input);
void TDM(const complex* input, complex ofdm_tdm[K][Nm]);
void* FDE_Received(complex* input, complex* w);
void DeModulate(complex* input, int* DemodulatePayload);
double calculateBER(const int* original, const int* received, int dataSize);
double noise_estimate(complex* input_GI_estimate, complex* input_channelgain, complex* input_GI);
void* TFF(complex* input);
void* channel_gain_estimate(complex* input_up, complex* input_down);
void* Highresolution(complex* input);
void* MMSE(complex* inputH, double inputN);
void* generatePq(complex* input);


#endif