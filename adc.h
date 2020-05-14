// ADC Functions for the MCP3001
// B.W.Metcalfe 2016

// Define the oscillator frequency, needed for delays
#define _XTAL_FREQ  4000000

// Define the ping connections
#define CS RA2
#define CLK RA0
#define DIN RA1

// Read a single (8-bit) sample from the ADC
unsigned int readADC();