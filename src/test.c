#include "headers/test.h"

char float_result[32];

char *ftoa2(float input, char result[]) {
	char const digit[] = "0123456789";
	char *p = result;

	if(input < 0) {
		*p++ = '-';
		input *= -1;
	}

	int left_half = (int) input;
	do{ //Move to where representation ends
			++p;
			left_half = left_half/10;
	}while(left_half);
	*p = '.';
	do{ //Move back, inserting digits as u go
			*--p = digit[left_half%10];
			left_half = left_half/10;
	}while(left_half);


	// Move back to decimal
	while(*p != '.') {
		p++;
	}

	int number_of_decimal_places = 3;
	float right_half = (float)((int) input) - input;

	for(int i = 0; i < number_of_decimal_places; i++) {
		right_half *= 10;
		*++p = digit[((int)right_half) % 10];
	}
  *p = '\0';

	return result;
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = itoa(ipart, res);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        itoa((int)fpart, res + i + 1);
    }
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


  ftoa(42.8008, float_result, 4);

  printf("Testing ftoa function.  %f in ascii is %s", 42.8008, float_result);
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
