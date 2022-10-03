
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../address_map_arm.h"
#include "../display/screens.h"
#include "touch.h"

// Open /dev/mem, if not already done, to give access to physical addresses
// Taken from `increment_leds` from Intel University Program, available on the ARM tutorial files
int open_physical (int fd)
{
   if (fd == -1)
      if ((fd = open( "/dev/mem", (O_RDWR | O_SYNC))) == -1)
      {
         printf ("ERROR: could not open \"/dev/mem\"...\n");
         return (-1);
      }
   return fd;
}

// Close /dev/mem to give access to physical addresses
void close_physical (int fd)
{
   close (fd);
}

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 * Taken from `increment_leds` from Intel University Program, available on the ARM tutorial files
 */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
   void *virtual_base;

   // Get a mapping from physical addresses to virtual addresses
   virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
   if (virtual_base == MAP_FAILED)
   {
      printf ("ERROR: mmap() failed...\n");
      close (fd);
      return (NULL);
   }
   return virtual_base;
}

/*
 * Close the previously-opened virtual address mapping
 * Taken from `increment_leds` from Intel University Program, available on the ARM tutorial files
 */
int unmap_physical(void * virtual_base, unsigned int span)
{
   if (munmap (virtual_base, span) != 0)
   {
      printf ("ERROR: munmap() failed...\n");
      return (-1);
   }
   return 0;
}



// Initialize the touch controller
int init_touch(int fd)
{

   // Create virtual memory access to the FPGA light-weight bridge
    if ((fd = open_physical (fd)) == -1)
		return (-1);
    if ((LW_virtual = map_physical (fd, LW_BRIDGE_BASE, LW_BRIDGE_SPAN)) == NULL)
		return (-1);


	// Initialize the virtual addresses for the 4 serial registers
	Touch_RxReg = (unsigned int *) (LW_virtual + TOUCH_UART_RX_REG);
	Touch_TxReg = (unsigned int *) (LW_virtual + TOUCH_UART_TX_REG);
	Touch_ControlReg = (unsigned int *) (LW_virtual + TOUCH_UART_CONTROL_REG);
	Touch_StatusReg = (unsigned int *) (LW_virtual + TOUCH_UART_STATUS_REG);



	// Clear the status and control registers by writing 0
	*Touch_StatusReg = 0;
	*Touch_ControlReg = 0;
	
	
	// Disable touch reports
	if (touch_disable() == -1) {
		printf("Error disabling touch\n");
	}
	
	
	// Read the ar1100's touchmode register
	printf("Reading touchMode\n");
	ar1100_readReg(0x0C,1);
	
	// Read the 5 received signals and print them
	int c1 = get_char_touch();
	int c2 = get_char_touch();
	int c3 = get_char_touch();
	int c4 = get_char_touch();
	int c5 = get_char_touch();
	printf("Read TouchMode response: %x,%x,%x,%x,%x\n", c1,c2,c3,c4,c5);
	
	
	// Enable touch interrupts
    return touch_enable();
}

// Enable touch reports from the AR1100
int touch_enable() {
	// Send the touch enable command
    put_char_touch(0x55);
    put_char_touch(0x1);
    put_char_touch(0x12);
	
	// Get the response
	int c1 = get_char_touch();
	int c2 = get_char_touch();
	int c3 = get_char_touch();
	int c4 = get_char_touch();
	
	// Verify that the response is correct
	if (c1 != 0x55 || c2 != 0x02 || c3 != 0x00 || c4 != 0x12){
		printf("Touch enable failed: %x,%x,%x,%x\n", c1,c2,c3,c4);
		return -1;
	} else {
		printf("Touch enable succeeded!!\n");
		return 1;
	}
}

// Disable touch reports from the AR1100
int touch_disable() {
	// Send the touch disable command
    put_char_touch(0x55);
    put_char_touch(0x1);
    put_char_touch(0x13);
	
	// Get the response
	int c1 = get_char_touch();
	int c2 = get_char_touch();
	int c3 = get_char_touch();
	int c4 = get_char_touch();
	
	// Verify that the response is correct
	if (c1 != 0x55 || c2 != 0x02 || c3 != 0x00 || c4 != 0x13){
		printf("Touch disable failed: %x,%x,%x,%x\n", c1,c2,c3,c4);
		return -1;
	} else {
		printf("Touch disable succeeded!!\n");
		return 1;
	}
}

// Used for writing a value to an AR1100 register
// In particular, we use it to update the touch mode
void ar1100_writeReg (unsigned char reg_addr, unsigned char write_val) {
	put_char_touch(0x55); // Sync
	put_char_touch(0x03); // Size = number of regs + 2
	put_char_touch(0x21); // Command: write_reg
	put_char_touch(0x00); // Register address MSB
	put_char_touch(reg_addr); // Register address LSB
	put_char_touch(write_val); // Register value
}

// Used for reading a value from an AR1100 register
// In particular, we use it to read the touch mode
void ar1100_readReg (unsigned char reg_addr, unsigned char num_bytes) {
	put_char_touch(0x55); // Sync
	put_char_touch(0x04); // Size
	put_char_touch(0x20); // Command: read_reg
	put_char_touch(0x00); // Register address MSB
	put_char_touch(reg_addr); // Register address LSB
	put_char_touch(num_bytes); // number of bytes to read
}

int touch_test_for_received_data(void) {
	// if Status register has RRDY flag true, there is data in the RX register
    return (*Touch_StatusReg & ALTERA_AVALON_UART_STATUS_RRDY_MSK) != 0;
}

int put_char_touch(int c) {
	// wait for Transmit Ready flag to be raised
    while ((*Touch_StatusReg & ALTERA_AVALON_UART_STATUS_TRDY_MSK) != ALTERA_AVALON_UART_STATUS_TRDY_MSK);
	
	// write character to Transmitter register
    *Touch_TxReg = c;
	
	// update the status register, saying we are not tready
	int status = *Touch_StatusReg;
	*Touch_StatusReg = status & (~ALTERA_AVALON_UART_STATUS_RRDY_MSK);
	
	// return the character we printed
    return c;
}


/*
 *	Used to read a character from the AR1100.
 *	Polls the receive register until the value changes, then returns the character
*/
int get_char_touch( void ) {
	// Read the current value of the RX register
	int data = *Touch_RxReg;
	// Wait until the RX register changes, then return the new value
	while(1) {
		int newData = *Touch_RxReg;
		if (newData != data) {
			data = newData;
			break;
		}
	}
	
    return data;
}


/*
 *	Idles until the first byte of a touch response is received.
 *	Touch reports always begin with 0x81, so the following 4 chars contain the coordinate data
*/
void wait_for_touch()
{
	int data = get_char_touch();
    while(data != 0x81){
		data = get_char_touch();
	}
}

// Wait for a touch report, translate the data to screen coordinates, and return the touch location
Point get_press(void)
{
    Point p1;

    wait_for_touch();
	int i2 = get_char_touch();
	int i3 = get_char_touch();
	int i4 = get_char_touch();
	int i5 = get_char_touch();

    int x_coordinate = (i2 + (i3 << 7)) * screen_x / 4096; // scale x coordinate to fit in 800px
    int y_coordinate = (i4 + (i5 << 7)) * screen_y / 4096; // scale y coordinate to fit in 480px

    p1.x = x_coordinate;
    p1.y = y_coordinate;

    return p1;
}

// Identical to get_char_touch, but times out after a specified time
int get_char_touch_timeout(int time)
{
	// Read the current value of the RX register
	int data = *Touch_RxReg;
	// Wait until the RX register changes, then return the new value
	int i;
	for (i = 0; i < time; i++) {
		int newData = *Touch_RxReg;
		if (newData != data) {
			return newData;
		}
	}
	
    return -1;
}

// Identical to wait_for_touch, but times out after a specified time
int wait_for_touch_timeout(int time)
{
	int data = get_char_touch_timeout(time);
	if (data == 0x81){
		return 0;
	} else {
		return -1;
	}
}


// Identical to get_press, but times out after a specified time
Point get_press_timeout(int time)
{
    Point p1;

    if (wait_for_touch_timeout(time)) {
		p1.x = -1;
		p1.y = -1;
		return p1;
	}
	int i2 = get_char_touch();
	int i3 = get_char_touch();
	int i4 = get_char_touch();
	int i5 = get_char_touch();

    int x_coordinate = (i2 + (i3 << 7)) * screen_x / 4096; // scale x coordinate to fit in 800px
    int y_coordinate = (i4 + (i5 << 7)) * screen_y / 4096; // scale y coordinate to fit in 480px

    p1.x = x_coordinate;
    p1.y = y_coordinate;

    return p1;
}