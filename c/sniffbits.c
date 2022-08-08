#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int roll_on_bigger(unsigned long long int  a, 	unsigned long long int  b, int bits)
{
	int ok = 0;
	int not_ok = !ok;

	printf("input A: %llx\n", a);
	printf("input B: %llx\n", b);
	unsigned long long int bigger = (a>b) ? a : b;
	unsigned long long int smaller = (a>b) ? b : a;
	printf("bigger A: %llx\n", bigger);
	printf("smaller B: %llx\n", smaller);

	int now_at_bigger = 0;
	int now_at_smaller = 0;
	int trigger_armed = 0;
	int check_to_complete = bits;
	int last_fail_at = 0;

	do {

		char bigger_bit_at 	= (bigger  >> now_at_bigger)  & 0x01;
		char smaller_bit_at 	= (smaller >> now_at_smaller) & 0x01;

		int smaller_at_big = (bigger_bit_at == smaller_bit_at);
		printf("BITS ARMD A: %d B: %d => b@ %d , s@ %d TRG:%d, COMP:%d\n", 
			bigger_bit_at, 
			smaller_bit_at, 
			now_at_bigger,
			now_at_smaller,
			trigger_armed, 
			check_to_complete);

		now_at_bigger++;
		now_at_smaller++;

		if ( now_at_bigger > (sizeof(b)*8) ) {
			printf("stream part done...\n");
			ok = 1; // NOK
			break;
		}

		if ( now_at_smaller == bits)
		{
			now_at_smaller=0;
			printf("reset to now_at_smaller == 0\n");
		}

		if ( smaller_at_big )
		{

			trigger_armed = 1;
			check_to_complete--;
		} else {
			if (trigger_armed)
			{
				if (check_to_complete == 0)
				{
					ok = 0;
					break;
				}
				now_at_smaller=0;
				printf("reset by trigger to now_at_smaller == 0\n");
				printf("reset to check_to_complete == bits\n");
				check_to_complete = bits;
			}
			trigger_armed=0;
		}		

		not_ok = !ok;
	} while ( not_ok );

	return ok;
}

unsigned long long int form_stream_buffer_int(char *buf)
{
	unsigned long long int tmp = 0;

#define calc_shift_bits(index) 	(8*index)
#define calc_shift(index) 	(64 - calc_shift_bits(index) - 8)
#define get_casted_from(val)	((unsigned long long)(val))
#define get_8bit_val(val)	(get_casted_from(val) & get_casted_from(0xFF))

	int i = 7;

	tmp |=  get_8bit_val(buf[i]) << calc_shift(i); i--;
	tmp |=  get_8bit_val(buf[i]) << calc_shift(i); i--;
	tmp |=  get_8bit_val(buf[i]) << calc_shift(i); i--;
	tmp |=  get_8bit_val(buf[i]) << calc_shift(i); i--;
	tmp |=  get_8bit_val(buf[i]) << calc_shift(i); i--;
	tmp |=  get_8bit_val(buf[i]) << calc_shift(i); i--;
	tmp |=  get_8bit_val(buf[i]) << calc_shift(i); i--;
	tmp |=  get_8bit_val(buf[i]) << calc_shift(i);
	
	printf("Value assembled %llx\n", tmp);
	return tmp;
}


int main(int argc, char *argv[])
{
	unsigned char buffer[8] = { 0 }; /* 8 * 8 bit*/

	unsigned long long int bitsToSearch = 0;
	int i = 0;

	int sft = 0;
	unsigned long long int input =0;
	unsigned long long int input_bits = 0;

	input 		= strtoll(argv[1], NULL, 16);
	input_bits 	= strtoll(argv[2], NULL, 0);
	printf("input: %llx -> %llu\n", input, input);
	printf("input_bits: %llu ... 8 = %d\n", input_bits, (int)input_bits % 8);

	int input_bits_over_charachter = input_bits % 8;
	int input_bits_charachters = ((input_bits - input_bits_over_charachter) / 8);

	int input_bit_char_count = input_bits_charachters + 1;
	int stream_bit_char_count = 0;
	int c;

	/* Bytes to bits - 8 bit*/
	int charAt = 0;

	while (	((c = getchar()) >= 0)
			&&
		(charAt < input_bits_over_charachter)
		)
	{
		printf("while: %d\n", c);
		/* Build buffer to check */
		buffer[charAt++] = c & 0xFF;
	}

	bitsToSearch = form_stream_buffer_int(buffer);

	/* Check passed ? */
	int same = roll_on_bigger(bitsToSearch, input, input_bits);
	printf("exit: %d\n", same);

	exit(same);
}

