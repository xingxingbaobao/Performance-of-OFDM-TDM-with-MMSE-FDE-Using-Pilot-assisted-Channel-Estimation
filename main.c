#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "configurations.h"
#include "print.h"





int main(){
  FILE *file;
  
  //Read from input.txt
  file = fopen("input.txt", "rb");
  unsigned int payload[data_size];
  int DemodulatePayload[data_size];

      for (int i = 0; i < data_size; i++){
        fscanf(file, "%1d", &payload[i]);
    }
     print_int_vector("-----------------------------payload-----------------------------  \n ", payload, data_size);
    
    //------------------------------------------------------------------Tx---------------------------------------------------------------------------//
    //DataModulation,including QPSK,chu pilot insertion，TDM division
     Datamod(payload);
     print_complex_vector("-----------------------------TDM modulated signal-----------------------------  \n ", d, Nc);
     print_complex_vector("-----------------------------chu-sequence-----------------------------  \n ", symbols, GI);

    //eq-1,Nm-point IFFT
    for (int x = 0; x < Nm; x++) {
         IFFT(d[x], Nm, scratch);
    }   

    //eq-2,Nm-point IFFT
    for (int x = 0; x < K; x++) {
        for (int y = 0; y < Nm; y++) {
            s_gg[y + x * K] = d[x][y];
            s_g[y + x * K].Re = (sqrt(2) / sqrt(Nm)) * s_gg[y + x * K].Re;
            s_g[y + x * K].Im = (sqrt(2) / sqrt(Nm)) * s_gg[y + x * K].Im;
        }
    }
      print_complex_vector("-----------------------------OFDM/TDM frame-----------------------------  \n ", s_g, Nc);

      //Nm-sample GI insertion
      AddGI(s_g);
      print_complex_vector("-----------------------------OFDM/TDM frame with GI----------------------------- \n ", signal_with_GI, Nc+GI);

      HPA(signal_with_GI);
      print_complex_vector("-----------------------------final transmitted signal----------------------------- \n ", S_g_hat, Nc + GI);

      //-----------------------------------------------------------------Channel-----------------------------------------------------------------------//
      //L=8-path frequency-selective block Rayleigh fading channel having uniform power delay profile. 
      RayleighChannel(S_g_hat, 8, GI+Nc);
      print_complex_vector("-----------------------------received signal----------------------------- \n ", ReceivedSignal, Nc + GI);

      //------------------------------------------------------------------Rx---------------------------------------------------------------------------//
      RemovalofGI(ReceivedSignal);
      print_complex_vector("-----------------------------received signal no GI----------------------------- \n ", ReceivedSignal_no_GI, Nc);
      print_complex_vector("-----------------------------received GI----------------------------- \n ", GI_Received, GI);

      //Nc-point FFT to get R_n
      FFT(ReceivedSignal_no_GI, Nc, scratch);
      R_n(ReceivedSignal_no_GI, Nc);
      print_complex_vector("-----------------------------received signal no GI in freq----------------------------- \n ", ReceivedSignal_no_GI, Nc);

      //channel estimation 
      //A.Time-domain First-order Filtering

      //generate GI frequency domain components
      generatePq(symbols);
      print_complex_vector("-----------------------------chu-sequence in freq----------------------------- \n ", symbols, Nm);

      //Time-domain First-order Filtering
      TFF(GI_Received);
      print_complex_vector("----------------------------received GI in freq---------------------------- \n ", filtered_GI_freq, GI);
      channel_gain_estimate(filtered_GI_freq, symbols);
      print_complex_vector("----------------------------instantaneous channel gain in freq---------------------------- ", H_g, GI);

      //B.Frequency-domain Interpolation
      Highresolution(H_g);
      print_complex_vector("----------------------------instantaneous channel gain in time---------------------------- \n ", Hen, Nc);

      //C.Noise power estimation
      double noiseEstimate = noise_estimate(filtered_GI_freq, Hen, symbols);
      printf("----------------------------estimated noise power----------------------------\n %.4f \n", noiseEstimate);

      //MMSE FDE 
      MMSE(Hen, noiseEstimate);
      print_complex_vector("----------------------------MMSE equation weight---------------------------- \n ", weight, Nc);

      //one tap FDE is applied to the freq received signal
      FDE_Received(ReceivedSignal_no_GI, weight);
      print_complex_vector("----------------------------received signal after FDE---------------------------- \n ", ReceivedSignal_FDE, Nc);

      //The time-domain OFDM/TDM signal is recovered by applying Nc-point IFFT
      IFFT(ReceivedSignal_FDE, Nc, scratch);
      print_complex_vector("----------------------------received time domain OFDM/TDM signal----------------------------\n ", ReceivedSignal_FDE, Nc);


      //The demodulation of OFDM signal with Nm subcarriers is done using Nm - point FFT
      TDM(ReceivedSignal_FDE, ofdm_tdm);
      for (int x = 0; x < Nm; x++) {
          FFT(ofdm_tdm[x], FFT_Window, scratch);
      }
      print_complex_vector("----------------------------OFDM/TDM demodulation----------------------------\n ", ofdm_tdm, Nc);

      //QPSK Demodulation to get recovered data
      DeModulate(ofdm_tdm, DemodulatePayload);
      print_int_vector("-----------------------------recovered data-----------------------------\n ", DemodulatePayload, data_size);

      //calculate Bit error rate
      double ber = calculateBER(payload, DemodulatePayload, data_size);
      printf("-----------------------------BER-----------------------------\n %.4f\n", ber);

}
