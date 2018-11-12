#include "headers/test.h"

char *ftoa(float input, char result[]) {
	char const digit[] = "0123456789"
	char *p = result;

	if(input < 0) {
		*p = '-';
		*p++;
		input *= -1;
	}

	int left_half = (int) input;
	do{ //Move to where representation ends
			++p;
			left_half = left_half/10;
	}while(left_half);
	*result = '.';
	do{ //Move back, inserting digits as u go
			*--p = digit[input%10];
			input = input/10;
	}while(input);


	// Move back to decimal
	while(*p != '.') {
		p++;
	}

	int number_of_decimal_places = 3;
	float right_half = (float)((int) input) - input;

	for(int i = 0; i < number_of_decimal_places; i++) {
		right_half *= 10;
		*++p = digit[input%10];
	}

	return result;
}

void prompt_convert_number(void) {
  float classic_float;
  SinglePrecisionFloat *spf_float;

  printf("Enter a number to convert from float to binary: ");
  scanf("%f", &classic_float);

  spf_float = create_single_precision_float_from_float(classic_float);
  printf("%f represented as hex is %X\n", spf_float->o, spf_float->hex);
  uint32_t hex_value = spf_float->hex;
  delete_single_precision_float(spf_float);

  spf_float = create_single_precision_float_from_hex(hex_value);
  printf("%X represented as float is %f\n", spf_float->hex, spf_float->o);
  delete_single_precision_float(spf_float);

  printf("Testing ftoa function.  %f in ascii is %s", 42.8008, ftoa(42.8008));
}

int menu(void) {
  int response = 0;

  printf("=== Calculator Menu ====\n");
  printf("1) test conversion\n");
  printf("0) quit\n");
  scanf("%d", &response);

  return response;
}


int main()
{
  int response = 1;

  while(response != 0) {
    response = menu();

    switch(response) {
      case 1:
        prompt_convert_number();
        break;
    }
  }

  return 0;
} // end main
