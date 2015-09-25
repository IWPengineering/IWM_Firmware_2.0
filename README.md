# IWP_Firmware_2.0
Rewrite of IWP Firmware for PIC24F32KA302 (3.3V version of previous micro)

Currently implemented functionality:

1. Checking X/Y Accelerometer Readings (Generating a raw data buffer every 10ms)
	Buffer = 8 readings each
	This code is currently blocking (waits for ADC to complete)
2. Checking Battery Readings (Generating a raw data buffer every 30 minutes)
	Buffer = 8 readings
3. Handle Depth ADC Interrupt Event to make data buffer. This is not called anywhere in the code ATM
	Buffer = 8 readings
4. Check if SIM is on via IOC from SIM Status
5. Check is SIM is on network via IOC & Frequency Counter from SIM Netlight
	Threshold not set
6. Check if WPS is sensing water via IOC & Frequency Counter from WPS Pin
	Threshold not set
7. Delay functions (delayS, delayMS, delayUS)
8. Check RTCC Time via I2C every 1 sec. 
 	If detection fails via timeout, time remains unchanged
9. Build UART Function to send char[]
10. Updated main to catch midnight event and send correct chars via SIM text message
