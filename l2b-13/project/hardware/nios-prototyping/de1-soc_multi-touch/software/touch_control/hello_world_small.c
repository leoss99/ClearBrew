
#include <stdbool.h>
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"
#include "system.h"
#include "altera_avalon_uart.h"
#include "altera_avalon_uart_regs.h"
//#include "altera_up_avalon_rs232.h"
#include <unistd.h> // For usleep


// ##################################################
// _____________________TYPEDEFS_____________________
// ##################################################
// Definition of Point
typedef struct { int x, y; } Point ;
// Definition of TouchEvent
typedef struct { Point p; bool penDown; } TouchEvent ;


// #####################################################
// _____________________DEFINITIONS_____________________
// #####################################################
// Definitions for buffer sizes
#define RX_BUFFER_SIZE 1024
#define TX_BUFFER_SIZE 1024

// Dimensions of the touch screen
#define XRES 800
#define YRES 480



// #############################################################
// _____________________Function Prototypes_____________________
// #############################################################
void init();
static void handle_touch_interrupt (void* context);
unsigned char transmitChar(unsigned char in_char);
void touchEnable();
void touchDisable();
void ar1100_writeReg (uint8_t reg_addr, uint8_t write_val);
void ar1100_readReg (uint8_t reg_addr, uint8_t num_bytes);
void flush_rxBuffer();
void flush_txBuffer();
char rxBuffer_pop();
int get_rxBuffer_size();
TouchEvent wait_for_penUp();
TouchEvent wait_for_touch_event();



// ##########################################################
// _____________________Global Variables_____________________
// ##########################################################
unsigned short txHead;
unsigned short txTail;
unsigned char txBuffer[TX_BUFFER_SIZE];
unsigned short rxHead;
unsigned short rxTail;
unsigned char rxBuffer[RX_BUFFER_SIZE];




// ###################################################
// _____________________Functions_____________________
// ###################################################

int main()
{ 

//  alt_putstr("Hello from Nios II!\n");

  //init();

  /* Event loop never exits. */
	/*
  while (1){
//	  if (rxTail != rxHead) {
//		alt_printf("%x\n", rxBuffer[rxTail]);
//		rxTail++;
//		if (rxTail >= (RX_BUFFER_SIZE)) rxTail = 0;
//	  }

//	  TouchEvent event = wait_for_penUp();
//
//	  //alt_printf("%x\n", rxBuffer_pop());
//	  alt_printf("PenUp event detected: coordinates are (%x, %x)\n", event.p.x, event.p.y);

	  alt_printf("Waiting for touch event\n");
	  TouchEvent event = wait_for_touch_event();

	  if (event.p.x != -1 && event.p.y != -1) {
		  if (event.penDown) {
			  alt_printf("penDown detected: coordinates are (%x, %x)\n", event.p.x, event.p.y);
		  } else {
			  alt_printf("penUp detected: coordinates are (%x, %x)\n", event.p.x, event.p.y);
		  }
	  }
  }
*/

	alt_printf("Base: %x\n", TOUCH_UART_BASE); // 0x9000
	alt_printf("Divisor: %x\n",IOADDR_ALTERA_AVALON_UART_DIVISOR(TOUCH_UART_BASE));

	alt_printf("RX: %x\n",IOADDR_ALTERA_AVALON_UART_RXDATA(TOUCH_UART_BASE));
	alt_printf("TX: %x\n",IOADDR_ALTERA_AVALON_UART_TXDATA(TOUCH_UART_BASE));

	alt_printf("Status: %x\n",IOADDR_ALTERA_AVALON_UART_STATUS(TOUCH_UART_BASE));
	alt_printf("Control: %x\n",IOADDR_ALTERA_AVALON_UART_CONTROL(TOUCH_UART_BASE));
	alt_printf("EOP: %x\n",IOADDR_ALTERA_AVALON_UART_EOP(TOUCH_UART_BASE));

  return 0;
}

/*
 * Initialization routine.
 *
 * Registers the ISRs, prepared the tx and rx buffers, and enabled the touchscreen.
 */
void init() {

	// Note - The following should be done in QSys:
	// 1. Set Baud rate to 9600
	// 2. Ensure 8 bit data, 1 stop bit, no parity


	alt_putstr("Beginning Init\n");


	// Register the touchscreen ISR
	alt_ic_isr_register(TOUCH_UART_IRQ_INTERRUPT_CONTROLLER_ID, TOUCH_UART_IRQ, handle_touch_interrupt, NULL, 0x0);

	//alt_ic_irq_enable(TOUCH_UART_IRQ_INTERRUPT_CONTROLLER_ID, TOUCH_UART_IRQ);

	IOWR_ALTERA_AVALON_UART_STATUS(TOUCH_UART_BASE, 0);
	// Enable interrupts for rready
	IOWR_ALTERA_AVALON_UART_CONTROL(TOUCH_UART_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);


	// Initialize the buffer pointers
	flush_txBuffer();
	flush_rxBuffer();


	// Disable touch reports to set touch mode
	alt_putstr("Disabling touch reports\n");
	touchDisable();
	usleep(100000);

	// Print the entire received buffer
	while (get_rxBuffer_size() != 0) {
		alt_printf("%x\n", rxBuffer_pop());
	}

	// Read the touchMode register to figure out what touch responses are sent
	alt_putstr("Reading touchMode\n");
	ar1100_readReg(0x0C,1); // Default value is B1
	usleep(100000);

	// Print the entire received buffer
	while (get_rxBuffer_size() != 0) {
		alt_printf("%x\n", rxBuffer_pop());
	}


	// Change touchMode to only send penUp reports
	// TouchMode[7:5] = PD[2:0] Response to event PD (PEN DOWN)
	// 		b000 No touch report issued in response to the event
	// TouchMode[4:3] = PM[1:0] Response to event PM (PEN MOVEMENT)
	//		b000 No touch report issued in response to the event
	// TouchMode[2:0] = PU[2:0] Response to event PU (PEN UP)
	//		b001 Touch report w/ P=0
	alt_putstr("Updating touchMode\n");
	ar1100_writeReg(0x0C, 0x01);
	usleep(100000);

	// Print the entire received buffer
	while (get_rxBuffer_size() != 0) {
		alt_printf("%x\n", rxBuffer_pop());
	}



	alt_putstr("Enabling touch reports\n");
	// Enable touch reports to start detecting touches
	touchEnable();

	usleep(100000);

	// Print the entire received buffer
	while (get_rxBuffer_size() != 0) {
		alt_printf("%x\n", rxBuffer_pop());
	}


	// Clear the transmit and received buffers
	flush_txBuffer();
	flush_rxBuffer();

	alt_putstr("Done Init\n");
}


/*
 * ISR for touchscreen.
 */
static void handle_touch_interrupt (void* context) {
	// Read status register
	int status = IORD_ALTERA_AVALON_UART_STATUS(TOUCH_UART_BASE);


	// Check if there is data to read
	if (status & ALTERA_AVALON_UART_STATUS_RRDY_MSK) {
		// Read data into buffer, inserting at the head
		unsigned char data = IORD_ALTERA_AVALON_UART_RXDATA(TOUCH_UART_BASE);
		rxBuffer[rxHead] = data;
		// Clear interrupt
		IOWR_ALTERA_AVALON_UART_STATUS(TOUCH_UART_BASE, status & (~ALTERA_AVALON_UART_STATUS_RRDY_MSK));
		// Increment head, reseting to 0 when end of buffer reached
		rxHead++;
		if (rxHead >= (RX_BUFFER_SIZE)) rxHead = 0;
	}

	// Check if transmit is ready for a new character
	if(status & ALTERA_AVALON_UART_STATUS_TRDY_MSK) {
		// Check if control wants us to handle TReady interrupts
		if(IORD_ALTERA_AVALON_UART_CONTROL(TOUCH_UART_BASE) & ALTERA_AVALON_UART_CONTROL_TRDY_MSK) {
			// If buffer is not empty, write the data
			if (txTail != txHead){
				// Write character at tail, then increment tail
				IOWR_ALTERA_AVALON_UART_TXDATA(TOUCH_UART_BASE, txBuffer[txTail]);
				txTail++;
				if (txTail > (TX_BUFFER_SIZE -1)) txTail = 0;
			} else {
				// Buffer is empty; lower TReady flag
				IOWR_ALTERA_AVALON_UART_CONTROL(TOUCH_UART_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
			}
		}
	}
}


/*
 * Helper function for transmitting a single character.
 *
 * If possible, the buffer will be transmitted immediately.
 * Else, it will be stored in a buffer and transmitted when TRDY causes an interrupt.
 */
unsigned char transmitChar(unsigned char in_char) {

	// Get status
	unsigned int status = IORD_ALTERA_AVALON_UART_STATUS(TOUCH_UART_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK;

	// If buffer is empty and we are ready to transmit, just transmit the char
	if ((txHead == txTail) && status) {
		IOWR_ALTERA_AVALON_UART_TXDATA(TOUCH_UART_BASE, in_char);
	} else {

		// Check size of buffer, and return -1 if buffer will overflow after updating
		unsigned short size;
		if (txHead >= txTail) {
			size = txHead - txTail;
		} else {
			size = ((TX_BUFFER_SIZE-1) - txTail) + txHead;
		}
		if (size > (TX_BUFFER_SIZE - 3)) return -1;

		// Write the char at the head of the buffer
		txBuffer[txHead] = in_char;

		// Increment head; if end of buffer reached, wrap around to 0
		txHead++;
		if (txHead > (TX_BUFFER_SIZE-1)) txHead = 0;

		// Set TReady flag in control register to indicate that we want TReady interrupts to be handled
		unsigned int ctrl = IORD_ALTERA_AVALON_UART_CONTROL(TOUCH_UART_BASE) | ALTERA_AVALON_UART_CONTROL_TRDY_MSK;
		IOWR_ALTERA_AVALON_UART_CONTROL(TOUCH_UART_BASE, ctrl);

	}
	// Successfully wrote to buffer or transmit register
	return 1;
}

/*
 * Helper function for enabling touch reports
 *	0x55 SYNC
 *	0x01 SIZE
 *	0x12 COMMAND (TOUCH_ENABLE)
 */
void touchEnable(){
	transmitChar(0x55);
	transmitChar(0x01);
	transmitChar(0x12);
}

/*
 * Helper function for disabling touch reports
 *	0x55 SYNC
 *	0x01 SIZE
 *	0x13 COMMAND (TOUCH_DISABLE)
 */
void touchDisable(){
	transmitChar(0x55);
	transmitChar(0x01);
	transmitChar(0x13);
}


// Write a value to a register
void ar1100_writeReg (uint8_t reg_addr, uint8_t write_val) {
	transmitChar(0x55); // Sync
	transmitChar(0x03); // Size = number of regs + 2
	transmitChar(0x21); // Command: write_reg
	transmitChar(0x00); // Register address MSB
	transmitChar(reg_addr); // Register address LSB
	transmitChar(write_val); // Register value
}


void ar1100_readReg (uint8_t reg_addr, uint8_t num_bytes) {
	transmitChar(0x55); // Sync
	transmitChar(0x04); // Size
	transmitChar(0x20); // Command: read_reg
	transmitChar(0x00); // Register address MSB
	transmitChar(reg_addr); // Register address LSB
	transmitChar(num_bytes); // number of bytes to read
}

// Clear all data from the received buffer
void flush_rxBuffer() {
	rxHead = 0;
	rxTail = 0;
}

// Clear all data from the transmit buffer
void flush_txBuffer() {
	txHead = 0;
	txTail = 0;
}


// Helper function waits for a penUp event, then returns the event
TouchEvent wait_for_penUp() {

	TouchEvent event;
	event.p.x = -1;
	event.p.y = -1;

	// Keep waiting for touch events until we get a penUp event
	while (event.p.x <0 || event.p.y < 0 || !event.penDown) {
		event = wait_for_touch_event();
	}

	return event;
}

/*
 * Waits until an entire touch packet is received, then returns it
 * Returns a TouchEvent, containing a point and whether the event is penUp or penDown
 * On error, returns a TouchEvent with coordinates (-1,-1)
 */

TouchEvent wait_for_touch_event() {


	flush_rxBuffer(); // may need to pause touch events to do avoid partial packets?

	// Idle until there is a complete touch response
	while (get_rxBuffer_size() < 5) {
		//alt_printf("Buffer size is: %x\n", get_rxBuffer_size());
		usleep(100000);
	}


	// Touch packet format
	// byte    7  6  5  4   3   2  1  0
	// --------------------------------
	//	1      1  R  R  R   R   R  R  P
	//	2      0  X6 X5 X4  X3  X2 X1 X0
	//	3      0  0  0  X11 X10 X9 X8 X7
	//	4      0  Y6 Y5 Y4  Y3  Y2 Y1 Y0
	//	5      0  0  0  Y11 Y10 Y9 Y8 Y7

	TouchEvent event;

	char c1 = rxBuffer_pop();

	if ((c1 & 0x80) && (c1 & 1)) {
		// penDown event
		event.penDown = true;
	} else if ((c1 & 0x80) && ~(c1 & 1)) {
		// penUp event
		event.penDown = false;
	} else {
		alt_putstr("ERROR: Touch event not recognized\n");
		event.p.x = -1;
		event.p.y = -1;
		return event;
	}

	// get the points
	char x1 = rxBuffer_pop();
	char x2 = rxBuffer_pop();
	char y1 = rxBuffer_pop();
	char y2 = rxBuffer_pop();

	// Convert the received coordinates to coordinates on the display
	event.p.x = ((int) x1 + ((int) x2 << 7)) * XRES / 4096; // scale x coordinate to fit in 800px
	event.p.y = ((int) y1 + ((int) y2 << 7)) * YRES / 4096; // scale y coordinate to fit in 480px

	return event;
}


// Helper function to get size of rxBuffer
int get_rxBuffer_size() {
	if (rxHead == rxTail) {
		return 0;
	} else if (rxHead > rxTail) {
		return rxHead - rxTail;
	} else {
		return rxHead + RX_BUFFER_SIZE - rxTail - 1;
	}
}


// Pops a single char from the tail of rxBuffer
char rxBuffer_pop() {
	// Check that rxBuffer is not empty
	if (rxTail == rxHead) {
		alt_putstr("ERROR: attempted to pop rxBuffer when empty\n");
		return -1;
	}

	// Get the next character in the buffer, then increment tail pointer
	char c = rxBuffer[rxTail];
	rxTail++;
	if (rxTail >= (RX_BUFFER_SIZE)) rxTail = 0;

	// return the popped character
	return c;
}



// SOURCES
// Interrupt handler:
//    https://community.intel.com/t5/Nios-II-Embedded-Design-Suite/NiosII-Uart-Interrupt-Handler/m-p/84690

// Embedded Peripherals Guide (See Page 135):
//    https://d2pgu9s4sfmw1s.cloudfront.net/DITA-technical-publications/PROD/PSG/ug_embedded_ip-19-1-683130-704756.pdf?Expires=1645757692&Key-Pair-Id=APKAJKRNIMMSNYXST6UA&Signature=JTmxM1yV9jxGPXj8byrEyuLdIJlFq6B2rchyvEBp-zDJBWbwhRTsqq6yuPgORD-2DODqn7QaSzB3QfhboAcdOqc4FQ~Bao3jZWu0ZdP7Sqq2WxenM8qJOhsgmeHpMCFbJHOPv9BWGi6EK8bnJUyBuaF6kK6uViQr1rn6EcHJ1AZzDawscLrxzTvrSFrTB2hACl46OIUUdF3RJ6bmJaOQwxLmgiTZnHiy30SogxQBYp~oASrgEJrlbkG5AohonwVH5jkkIHR~Zr~aZ8V0G~DT53V3v8g5xlMA9gGxIn0EqmTKrmtjYMNjnsiMpLS6~GCovflhEFTWf8lfg1eT6Uq3iQ__

// Git page with helpful drivers
//https://github.com/ShreyansK2000/Translaite/blob/master/Hardware/Touch.c

// Github Repos which use similar stuff:
// 	jritter96/Geostash
// 	Si4463Project/RadioCode
// 	ShreyansK2000/Translaite		<-- Very useful, much content used from here
