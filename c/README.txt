To build:

    gcc bitmatch.c -o bitmatch

Then to use:
	
	echo "h>0?" | ./bitmatch f8c 11 || echo not found
	echo "h<0?" | ./bitmatch f8c 11 || echo not found


