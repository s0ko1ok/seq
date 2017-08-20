#define soc_cv_av
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "soc_cv_av/socal/socal.h"
#include "soc_cv_av/socal/hps.h"
#include "soc_cv_av/socal/alt_gpio.h"
#include "hps_0.h"
//#include "led.h"
#include <stdbool.h>

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

volatile unsigned long *h2p_lw_led_addr=NULL;

/* void LEDR_LightCount(unsigned char LightCount){ // 1: light, 0:unlight
    uint32_t Mask = 0;
    int i;
    for(i=0;i<LightCount;i++){
        Mask <<= 1;
        Mask |= 0x01;
    }
    //IOWR_ALTERA_AVALON_PIO_DATA(LEDG_BASE, Mask);  //0:ligh, 1:unlight
    //alt_write_word(h2p_lw_led_addr, Mask );  //0:ligh, 1:unlight
}
void LEDR_OffCount(unsigned char OffCount){ // 1: light, 0:unlight
    uint32_t Mask = 0xfffff;
    int i;  
    for(i=0;i<OffCount;i++){
        Mask >>= 1;
		}
    //IOWR_ALTERA_AVALON_PIO_DATA(LEDG_BASE, Mask);  //0:ligh, 1:unlight
    alt_write_word(h2p_lw_led_addr, Mask );  //0:ligh, 1:unlight
}
void LEDR_AllOn(void)
{
	alt_write_word(h2p_lw_led_addr, 0xFFFFFF);  //0:ligh, 1:unlight
}

void LEDR_AllOn(void)
{
	alt_write_word(h2p_lw_led_addr, 0xFFFFFF);  //0:ligh, 1:unlight
}
void LEDR_AllOff(void)
{
	alt_write_word(h2p_lw_led_addr, 0x00);  //0:ligh, 1:unlight
}
 */

int main(int argc, char **argv)
{
	
	void *virtual_base;
	int fd;
//	int i;
	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );	
	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return(1);
	}
	printf("Hello World!\r\n");
	h2p_lw_led_addr=virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + LED_PIO_BASE ) & ( unsigned long)( HW_REGS_MASK ) );
	int value;
	int adress;
		while (1){
		unsigned int ch = getchar();
		if (ch > 64 && ch < 123 ) {
			value =0;
			adress = ch;
			while(1){
				ch = getchar();
				if (ch == 10 || ch ==13){
					adress <<= 16;
					adress = adress + value;
					printf("final");
					usleep(100*1000);
					printf("%d\n", adress);
					alt_write_word(h2p_lw_led_addr, adress);
					break;
				}
				if (ch > 47 && ch < 58 )
				value = (value * 10) + (ch - 48);
				
			}
			/* adress <<= 16;
			adress = adress + value;
			alt_write_word(h2p_lw_led_addr, adress);
			printf("adress");
			usleep(100*1000);
			printf("%d\n", adress);
			usleep(100*1000);
			printf("value");
			usleep(100*1000);
			printf("%d\n", value);
			usleep(100*1000);
			
			printf("final");
			usleep(100*1000);
			printf("%d\n", adress);
			usleep(100*1000); */
			
		}	
		if (ch == 46)
			break;
		}

	
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );

	}
	close( fd );
	return 0;
}

