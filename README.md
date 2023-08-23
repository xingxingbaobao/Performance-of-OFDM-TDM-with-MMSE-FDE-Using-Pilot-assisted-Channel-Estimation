# Performance-of-OFDM-TDM-with-MMSE-FDE-Using-Pilot-assisted-Channel-Estimation
This code repository implements the transmission of digital signals in a communication system, is the implementation of the paper "Performance of OFDM/TDM with MMSE-FDE Using Pilot-assisted Channel Estimation". 
It consists of four main components: transmitter, channel, channel estimation, and receiver. Running `main.c` demonstrates the results of processing a digital signal through these modules, and the output is saved in the `print.txt` file.

## Function Modules

### Transmitter

In the transmitter module, digital signals are processed and modulated for transmission over a communication channel. This module is responsible for processing input digital signals, adding pilot signals, generating OFDM/TDM signals and modulating them to suit the characteristics of the channel.

### Channel

The channel module simulates the characteristics of a communication channel, including noise and rayleigh freq-selective fading effects. 

### Channel Estimation

The channel estimation module is used to estimate the characteristics of the channel, including channel gain and noise power, using for the receiver.

### Receiver

The receiver module receives signals that have been transmitted through the channel and performs operations such as demodulation, equalization to recover the original digital signals. 

## Usage

1. Compile and run the `main.c` file.

2. View the `print.txt` file to understand the transmission of digital signals within the communication system.

## Important Notes

- The current system is based on a predefined SNR.


## Author

Author: Yixing Zhang


