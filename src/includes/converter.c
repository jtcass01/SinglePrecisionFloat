
#include "../headers/converter.h"

// I crammed it all in here so I wouldn't have to worry about allocating any more memory.
SinglePrecisionFloat *create_single_precision_float_from_float(float num) {
	SinglePrecisionFloat *spf_float = malloc(sizeof(SinglePrecisionFloat));
	char temp[32], left_side[32], right_side[32], mantissa_c[23];
	int index = 0, left_size = 0, right_size = 0;
	char *LeftSide, *RightSide;

	#if DEBUG
		printf("Creating a single precision float from %f\n", num);
	#endif

	spf_float->o = num;

	// GET SIGN BIT!!!!!
	if(num < 0) {
		spf_float->sign = create_binary_representation(1, 1);
		num *= -1;
	} else {
		spf_float->sign = create_binary_representation(0, 1);
	}

	int integer = (int)num;
	float decimal = num - integer;

	// GET THE EXPONENT
	/* ====                      =====*/
	/* ==== LEFT SIDE OF DECIMAL =====*/
	/* ====                      =====*/
	// Initialize buffers
	while (index < 32) {
		*(left_side + index) = '0';
		*(temp + index) = '0';
		*(right_side + index) = '0';
		if (index < 23) {
			*(mantissa_c + index) = '0';
		}
		index++;
	}

	index = 0;

	// Store forward direction in temp.
	while (integer != 0 && index < 32) {
		if ((integer % 2) == 0) { // No remainder
			*(temp + index) = '0';
		}
		else {
			*(temp + index) = '1';
		}
		integer /= 2;
		index++;
		left_size++;
	}

	//Reverese the direction
	for (int i = 0; i < left_size; i++) {
		left_side[i] = temp[left_size - i - 1];
	}

	LeftSide = strndup(left_side, left_size);

	#if DEBUG
		printf("Binary representation of left-half: %s of size: %d and exponential shift: %d\n", LeftSide, left_size, get_exponent_from_float(left_side, 0));
	#endif

	// Store exponent.
	if (left_size != 0) {
		spf_float->exponent = create_binary_representation(get_exponent_from_float(LeftSide, 0), 8);
	}

	/* ====                       =====*/
	/* ==== RIGHT SIDE OF DECIMAL =====*/
	/* ====                       =====*/
	// Store forward direction in temp.
	index = 0;

	while (decimal != 0 && index < 32) {
		if ((decimal * 2) >= 1) { // No remainder
			*(right_side + index) = '1';
			decimal *= 2;
			decimal -= 1;
		}
		else {
			*(right_side + index) = '0';
			decimal *= 2;
		}
		index++;
		right_size++;
	}

	RightSide = strndup(right_side, right_size);

	#if DEBUG
		printf("Binary representation of right-half: %s\n", RightSide);
	#endif

	if (left_size == 0) {
		spf_float->exponent = create_binary_representation(get_exponent_from_float(RightSide, 1), 8);
	}

	/* ====                       =====*/
	/* ======= CREATE MANTISSA ========*/
	/* ====                       =====*/
	index = 0;

	// Add left side to mantissa
	for (int i = 1; i < left_size; i++) {
		if (index >= 23) {
			break;
		}
		else {
			mantissa_c[index] = LeftSide[i];
			index++;
		}
	}
	// Add right side to mantissa
	if (index != 0) {
		for (int i = 0; i < right_size; i++) {
			if (index >= 23) {
				break;
			}
			else {
				mantissa_c[index] = RightSide[i];
				index++;
			}
		}
	}
	else {
		int found = 0;
		for (int i = 0; i < right_size; i++) {
			if (found) {
				if (index >= 23) {
					break;
				}
				else {
					mantissa_c[index] = RightSide[i];
					index++;
				}
			}
			else {
				if (RightSide[i] == '1') {
					found = 1;
				}
			}

		}
	}

	spf_float->mantissa = strndup(mantissa_c, 23);

	/* ====                       =====*/
	/* =======GET HEX REPRESENT========*/
	/* ====                       =====*/
	if (num == 0) {
		spf_float->hex = 0;
	}
	else {
		spf_float->hex = get_hex(spf_float->sign, spf_float->exponent, spf_float->mantissa);
	}

	#if DEBUG
		print_float(spf_float);
	#endif
	free(LeftSide);
	free(RightSide);

	return spf_float;
}

SinglePrecisionFloat *create_single_precision_float_from_hex(uint32_t hex_value) {
	SinglePrecisionFloat *spf_float = malloc(sizeof(SinglePrecisionFloat));
	spf_float->hex = hex_value;
	char sign[1], exponent[8], mantissa[23];
	int index = 0;

	#if DEBUG
		printf("Creating SPF_Float from Hex value %X\n", hex_value);
	#endif

	sign[0] = get_mask_value(hex_value, 31-index);
	index++;

	#if DEBUG
		printf("Sign = %s\n", sign);
	#endif

	for (int i = 0; i < 8; i++) {
		exponent[i] = get_mask_value(hex_value, 31 - index);
		index++;
	}

	#if DEBUG
		printf("Exponent = %s\n", exponent);
	#endif

	for (int i = 0; i < 23; i++) {
		mantissa[i] = get_mask_value(hex_value, 31 - index);
		index++;
	}

	#if DEBUG
		printf("Mantissa = %s\n", mantissa);
	#endif

	spf_float->sign = strndup(sign, 1);
	spf_float->exponent = strndup(exponent, 8);
	spf_float->mantissa = strndup(mantissa, 23);

	#if DEBUG
		print_float(spf_float);
	#endif

	spf_float->o = get_float(spf_float->sign, spf_float->exponent, spf_float->mantissa);

	#if DEBUG
		print_float(spf_float);
	#endif

	return spf_float;
}

char get_mask_value(uint32_t hex_value, int bit) {
	if ( (hex_value >> bit) & 1 ) {
		return '1';
	}
	else {
		return '0';
	}
}


void print_float(SinglePrecisionFloat *spf_float) {
	if (spf_float == NULL) {
		printf("This is a NULL object.  Cannot print.\n");
	}
	else {
		printf("Decimal value: %f, sign: %s, exponent: %s, mantissa: %s, Hex: 0x%X\n", spf_float->o, spf_float->sign, spf_float->exponent, spf_float->mantissa, spf_float->hex);
	}
}

void delete_single_precision_float(SinglePrecisionFloat *spf_float) {
	free(spf_float->sign);
	free(spf_float->exponent);
	free(spf_float->mantissa);
	free(spf_float);
}

char *create_binary_representation(int integer, int bits) {
	char temp[32], binary_c[32];
	int index = 0, binary_size = 0;
	char *binary_representation;

	// Initialize buffers
	while (index < 32) {
		*(binary_c + index) = '0';
		*(temp + index) = '0';
		index++;
	}

	index = 0;

	// Store forward direction in temp.
	while (integer != 0 && index < bits) {
		if ((integer % 2) == 0) { // No remainder
			*(temp + index) = '0';
		}
		else {
			*(temp + index) = '1';
		}
		integer /= 2;
		index++;
		binary_size++;
	}

	//Reverese the direction
	index = bits - binary_size;
	for (int i = 0; i < binary_size; i++) {
		if(index > 0) {
			binary_c[i+index] = temp[binary_size - i - 1];
		}
		else {
			binary_c[i] = temp[binary_size - i - 1];
		}
	}

	binary_representation = strndup(binary_c, bits);

	#if DEBUG
		printf("Binary representation of %d is %s\n", initial, binary_representation);
	#endif

	return binary_representation;
}


int get_exponent_from_float(char *binary_left_half, int negative) {
	int exponent_shift = 0;

	if (negative) {
		while (binary_left_half[exponent_shift] != '\0') {
			exponent_shift++;
			if (binary_left_half[exponent_shift] == '1') {
				break;
			}
		}

		exponent_shift *= -1;
	}
	else {
		while (binary_left_half[exponent_shift] != '\0') {
			exponent_shift++;
		}

	}

	return exponent_shift - 1 + 127;
}


int get_exponent_from_hex(char *binary_exponent) {
	return binary_to_int(binary_exponent) - 127;
}


uint32_t get_hex(char *sign, char *exponent, char *mantissa) {
	uint32_t result = 0;
	int two_power = 0;

	for (int i = 22; i >= 0; i--) {
		if (mantissa[i] == '1') {
			result += (1 << two_power);
		}
		two_power++;
	}

	for (int i = 7; i >= 0; i--) {
		if (exponent[i] == '1') {
			result += (1 << two_power);
		}
		two_power++;
	}

	for (int i = 0; i >= 0; i--) {
		if (sign[i] == '1') {
			result += (1 << two_power);
		}
		two_power++;
	}

	return result;
}

float get_float(char *sign, char *exponent, char *mantissa) {
	float result = 0.0, mantissa_float = 0.0;
	int exponent_integer = 0, left_size = 0, index = 0;
	char left_side[32], right_side[32], temp[32];
	char *LeftSide;

	exponent_integer = get_exponent_from_hex(exponent);

	#if DEBUG
		printf("Exponent Integer: %d\n", exponent_integer);
	#endif

	// Initialize Buffers
	while (index < 32) {
		*(left_side + index) = '0';
		*(temp + index) = '0';
		*(right_side + index) = '0';
		index++;
	}

	/* ====                      =====*/
	/* ==== LEFT SIDE OF DECIMAL =====*/
	/* ====                      =====*/

	// Store forward direction in temp.
	if (exponent_integer < 0) {
		result = 0.0;
		left_size++;
		index = 1;
	}
	else {
		left_side[0] = '1';
		left_size++;
		index = 1;
		#if DEBUG
			printf("left_side[0] = 1\n");
		#endif

		for (int i = 0; i < exponent_integer; i++) {
			#if DEBUG
				printf("left_side[%d] = mantissa[%d] = %c\n", index, i, mantissa[i]);
			#endif

			left_side[index] = mantissa[i];
			left_size++;
			index++;
		}


	}
	LeftSide = strndup(left_side, left_size);
	result += (float) binary_to_int(LeftSide);

	#if DEBUG
		printf("LeftSide Binary: %s | LeftSide Integer: %f\n", LeftSide, result);
	#endif

	/* ====                       =====*/
	/* ==== RIGHT SIDE OF DECIMAL =====*/
	/* ====                       =====*/
	if (left_side[0] == '0') { // Decimal only case
		for (int i = 0; mantissa[i] != '\0'; i++) {
			if (mantissa[i] == '1') {
				mantissa_float += ((float) 1 / (float) (1 << (i + 1)));

				#if DEBUG
					printf("1 found @ %d - adding %f or 1 / %d to result\n", i, mantissa_float, (1 << (i + 1)));
				#endif
			}
		}
		mantissa_float += 1.0;

		#if DEBUG
			printf("Manitssa float = %f\n", mantissa_float + 1.0);
		#endif

		for(int i = 0; i < (-1*exponent_integer); i++) {
			mantissa_float /= 2.0;
		}

		#if DEBUG
			printf("Manitssa float = %f\n", mantissa_float);
		#endif

		result = mantissa_float;
	}
	else {
		for (int i = exponent_integer; mantissa[i] != '\0'; i++) {
			if (mantissa[i] == '1') {
				mantissa_float = ((float)1 / (float)(1 << (i - exponent_integer + 1)));

				#if DEBUG
					printf("1 found @ %d - adding %f or 1 / %d to result\n", i, mantissa_float, (1 << (i - exponent_integer + 1)));
				#endif

				result += mantissa_float;
			}
		}
	}

	#if DEBUG
		printf("Result with decimal: %f\n", result);
	#endif



	// Apply sign.
	if (sign[0] == '1') {
		result *= -1;
	}

	free(LeftSide);

	return result;
}

int binary_to_int(char *binary_num) {
	int result = 0, size = 0, index = 0;

	for (int i = 0; *(binary_num + i) != '\0'; i++) {
		size++;
	}

	for (int i = size - 1; i >= 0; i--) {
		if (binary_num[i] == '1') {
			result += (1 << index);
		}
		index++;
	}

	return result;
}
