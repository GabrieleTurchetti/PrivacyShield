#pragma once

/* -------------------------------------------------------------------------- */
/*  I2S Microphone (MEMS) pins                                              */
/* -------------------------------------------------------------------------- */
#define PIN_I2S_MIC_BCLK    6    /* Bit clock */
#define PIN_I2S_MIC_LRCLK   4    /* Left-right clock (word select) */
#define PIN_I2S_MIC_DIN     5    /* Data in (from mic) */

/* -------------------------------------------------------------------------- */
/*  I2S Amplifier (MAX98357A) pins                                          */
/* -------------------------------------------------------------------------- */
#define PIN_AMP_BCLK        15   /* Bit clock */
#define PIN_AMP_LRCLK       16   /* Left-right clock (word select) */
#define PIN_AMP_DOUT        17   /* Data out (to amp) */
#define PIN_AMP_SD          18   /* Shutdown pin (active low — pull high to enable) */

/* -------------------------------------------------------------------------- */
/*  Status LED                                                               */
/* -------------------------------------------------------------------------- */
#define PIN_LED             48   /* Built-in RGB LED (or GPIO for external) */

/* -------------------------------------------------------------------------- */
/*  Node defaults                                                            */
/* -------------------------------------------------------------------------- */
#define DEFAULT_NODE_ID     1    /* Change per device (1–254). 0 = hub. */
