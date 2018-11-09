#ifndef CONVERTER
#define CONVERTER

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "constants.h"

typedef struct {
  float o;
  uint32_t hex;
  char *sign;
  char *exponent;
  char *mantissa;
} SinglePrecisionFloat;

SinglePrecisionFloat *create_single_precision_float_from_float(float);
SinglePrecisionFloat *create_single_precision_float_from_hex(uint32_t);
char get_mask_value(uint32_t, int);
void print_float(SinglePrecisionFloat *);
void delete_single_precision_float(SinglePrecisionFloat *);

char *create_binary_representation(int , int );

int get_exponent_from_float(char *, int);
int get_exponent_from_hex(char *);
uint32_t get_hex(char *, char *, char *);
float get_float(char *, char *, char *);
int binary_to_int(char *);

#endif
