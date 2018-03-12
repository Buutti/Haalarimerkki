#include <stdbool.h>



//Declarations for ctr_functions.c
void initPorts(void);

void setPin(bool state, uint8_t LEDNumber);

void blinkLED(uint8_t LEDNumber, int period_us, uint8_t dutyCycle);

void lightAll(uint8_t loops, int period_us, uint8_t dutyCycle);

void lightWaveAll(uint8_t loops, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightWaveSingle(uint8_t loops, uint8_t slowing, int pediod_us, uint8_t minBrightness, uint8_t maxBrightness);

void lightWaveBlocks(uint8_t loops, uint8_t slowing, int period_us, uint8_t minBrightness, uint8_t maxBrightness);

