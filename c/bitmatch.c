#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

void dbg_printf (const char *file, int line, const char *func, const char *fmt, ...)
{
	va_list tl;

	printf("[%s@%d %-15s]\t", file, line, func);
	va_start(tl, fmt);
	vprintf(fmt, tl);
	va_end(tl);

}

void print_bin(unsigned long long int bits, int bit_size)
{
#if DEBUG
	int bitsinsource = bit_size;
	unsigned long long int tmp = bits;

	tmp = bits;
	while ( bitsinsource )
	{
		char on = (tmp >> (bitsinsource-1)) & 1;

		if (on)
			printf("1");
		else 
			printf("0");

		bitsinsource--;
		if ((bitsinsource % 8) ==0)
			printf(" ");
	}
#endif
}


void print_max_bin(unsigned long long int bits)
{
    print_bin(bits, 64);
}


#if DEBUG 
# define dbg(fmt, ...)	dbg_printf(__FILE__, __LINE__, __func__ , fmt, ##__VA_ARGS__)
#else 
# define dbg(fmt, ...)
#endif

#if DEBUG 
#define dbg_print_bin(prefix, msg, gValue) \
	dbg("%s %s", prefix, msg);	\
	print_bin((gValue), sizeof(gValue)*8);	\
	printf("\n");
#else 
# define dbg_print_bin(prefix, msg , gValue)
#endif

unsigned long long int make_a_mask(unsigned int bits)
{
	unsigned long long int b = (1ULL)<<(bits+1);
	unsigned long long int o = b-1;
#if DEBUG
	dbg("Bits from b: ");
	print_bin(b, sizeof(b)*8);
	printf("\n");
	dbg("Bits from o: ");
	print_bin(o, sizeof(o)*8);
	printf("\n");
#endif
	return o;
}


unsigned long long int get_flipped(unsigned long long int  tmp, unsigned long long int bits)
{
    unsigned long long int rev = 0;
    int i = 0;
    dbg("flip: %d bits\n", bits);
    dbg_print_bin("RV", "Orginal: " , tmp);
	while( i < bits )
	{
        rev |= ((tmp >> i) & 1) << (bits-i);
        i++;
	}
    dbg_print_bin("RV", "Reverse: " , rev);

	return rev;
}


int get_char(int *hi)
{
	int c = getchar();
	*hi = ( c>=0 );
    dbg_print_bin("INPUT", "Raw char  : " , c);

    if ( c<0 ) {
        dbg("No char, input dried..\n");
        return 0;
    } else {
        dbg("char: %c / %d / %x, Has input: %d\n", c, c, c, *hi);
 	    return c & 0xFF;
    }
}

int search_pattern_bits(unsigned int streambyte, unsigned long long int pattern_to_search_for, unsigned long long int search_pattern_bit_count, int *save_at)
{
    int atbit = 0;
    int last_at_bit = search_pattern_bit_count+8;

    while( atbit < last_at_bit)
    {
        unsigned int pattern_forward = (pattern_to_search_for >> atbit) & 0xFF;
        unsigned int at_forward = streambyte & pattern_forward;

        dbg("-  at %d -----------------------------------------------------------------------------\n", atbit);
        dbg_print_bin("search",  "streambyte      : " , streambyte);
        dbg_print_bin("search",  "pattern_forward : " , pattern_forward);
        dbg_print_bin("search",  "at_forward      : " , at_forward);

        if (at_forward && pattern_forward)
        {


            if ( (
                    ( at_forward == streambyte) && 
                    (pattern_forward == streambyte)
               	 ) || (
		    (atbit == 0) && (at_forward == pattern_forward)
	  	 )
               )
            {
                *save_at = atbit;
                dbg("Found @ %d\n", atbit );
                return 1;
            }
        }
        atbit++;
    }
    // KYL   01101000 00111110
    // EI    01101000 00111100
    return 0;
}

int sweep_char(unsigned char streambyte, unsigned long long int pattern_to_search_for, unsigned long long int search_pattern_bit_count, int hasInput )
{
    static unsigned short last_off = 0;
    static unsigned int last_byte = 0;
    static unsigned int last_pattern = 0;
    static unsigned short last_set = 0;
    static unsigned int last_seen  = 0;
    static unsigned int bits_processed = 0;
    static int found = 0;    
    static unsigned int at_bit_of_pattern = 0;
    int index = 0;
    unsigned long long int workarea = 0;
    unsigned long long int mask_of_pattern;
    unsigned long long int workarea_pattern = pattern_to_search_for;

    workarea |= streambyte;
    if ( last_set ) {
        workarea <<= 8;
        workarea |= last_byte;
    }

    index = 0; 
    last_byte = streambyte;
    last_set = 1;
    search_pattern_bit_count += 8;
    dbg("last workarea                    : %x\n", workarea);
    dbg("last last_byte                   : %x\n", last_byte);
    dbg("last search_pattern_bit_count    : %d\n", search_pattern_bit_count);
    dbg_print_bin("setup",  "workarea      : " , workarea);

    mask_of_pattern = make_a_mask(search_pattern_bit_count) << 8;
    dbg("last last_byte                   : %x\n", last_byte);
    int last_state_of_stream = 0;

    int save_at = 0;
    int ok = 0;
    
    while(workarea > 0) 
    {
        ok |= search_pattern_bits(workarea & 0xFF, pattern_to_search_for, search_pattern_bit_count, &save_at);
        workarea >>= 8;
    }

    if ( ! ok )
    {
       last_byte = streambyte;
       last_set = 1;
    }

    dbg("at sweep_char end: %d, save_at:%d\n", ok, save_at);    
    return ok;

}

int sweep(unsigned long long streambuffer, unsigned long long int pattern_to_search_for, unsigned long long int search_pattern_bit_count , int hasInput)
{
    static unsigned long long int bits_processed = 0;

    unsigned long long int tmp = streambuffer;

    unsigned long workarea_mask = ~(0UL);
    unsigned long workarea = streambuffer & workarea_mask; // get workarea
    
    dbg_print_bin("sweep bit setup", "Got bits from stream      : " , streambuffer);
    dbg_print_bin("sweep bit setup", "Got bits for workarea MSK : " , workarea_mask);
    dbg_print_bin("sweep bit setup", "Got bits for workarea     : " , workarea);
    dbg_print_bin("sweep bit setup", "Got bits from user        : " , pattern_to_search_for);

    int sweep_ok = 0;
    while(tmp)
    {
        dbg_print_bin("sweep bit while", "Tmp bits                  : " , tmp);
        sweep_ok += sweep_char(tmp & 0xFF , pattern_to_search_for, search_pattern_bit_count, hasInput);
        tmp >>= 8;
   }

    dbg("RET: sweep_ok: %d\n", sweep_ok);
	return sweep_ok;	
}

unsigned long long int scan_at_input(unsigned long long int pattern, unsigned long long int pattern_bits)
{
    int hasInput;
    int rd = 0;
    int sweep_ok = 0;
    unsigned long long int bits_to_do = (pattern_bits < (sizeof(unsigned long int)*8)) ? pattern_bits : sizeof(unsigned long int)*8;
    unsigned long long readbuffer = 0;
    int chars_to_read = ((bits_to_do + 7) / 8) ; // Read how many chars
    int bits_to_read = bits_to_do;
    int bits_to_read_leftoff = bits_to_do % 8;
    int carry_flag;

    do {
        dbg("= RAW INPUT S =========================================\n");
        unsigned char c=0;

        do {
                dbg("Inputting %d chars, skeeking for %llu bits to buffer\n", chars_to_read, pattern_bits);
                dbg_print_bin("input", "RAW INPUT " , c);
                c = get_char(&hasInput) & 0xFF;
                readbuffer = c;

                dbg("===================================================================\n");
                dbg("=  SWEEP INPUT   ==================================================\n");
                dbg("===================================================================\n");
                dbg("readbuffer: %llx vs %llx (%d bits), hasmore:%d\n", readbuffer, pattern, pattern_bits, hasInput);

                rd += 1;
                sweep_ok += sweep(readbuffer, pattern, pattern_bits, hasInput);

        } while (((rd < chars_to_read) && hasInput));


/*        do {
            sweep_ok += sweep( pattern, pattern_bits,  &aremainder, &aremainder_bits);

            has_been_sweep_ok += sweep_ok;
        } while( (aremainder_bits > 0) );
  */      
        dbg("SWEEP OK: %d \n", sweep_ok);
        
        dbg("= RAW INPUT E =========================================\n");
    } while(hasInput && (!sweep_ok) );
    dbg("= RAW INPUT while exit done %d & %d\n", hasInput, sweep_ok);

    dbg("Bits: %x rd: %d.. ok: %d\n", pattern_bits, rd, sweep_ok);
    return sweep_ok;
}
// 64<<1 + 1

int main(int argc, char *argv[])
{

	unsigned long long int input_target =0;
	unsigned long long int input_target_bits = 0;
	unsigned long long int input_target_rev = 0;
	char *pStart = argv[1];
	int ok = 1;
	unsigned int hex_len_left_to_process = strlen(pStart);

	input_target_bits 	= strtoll(argv[2], NULL, 0);

	while ( hex_len_left_to_process > 0 )
	{
		unsigned int hex_len = strlen(pStart);

		if ( hex_len > 8 )
			hex_len = 8;

		 hex_len_left_to_process -= hex_len;

		input_target 		= strtoll(pStart, NULL, 16);
		ok += scan_at_input(input_target, input_target_bits);
		pStart += hex_len;
	}

    dbg("Found %d times.", ok);
    return (ok == 0);

}

