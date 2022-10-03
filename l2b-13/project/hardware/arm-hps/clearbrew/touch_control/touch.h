#ifndef TOUCH_H__
#define TOUCH_H__


// Memory locations for touch controller uart registers
#define TOUCH_UART_BASE 		0x00000120
#define TOUCH_UART_RX_REG 		0x00000120
#define TOUCH_UART_TX_REG 		0x00000124
#define TOUCH_UART_STATUS_REG 	0x00000128
#define TOUCH_UART_CONTROL_REG 	0x0000012c

// Macros For interacting with status and control registers
// Taken from compiled Nios system
#define ALTERA_AVALON_UART_STATUS_PE_MSK              (0x1)
#define ALTERA_AVALON_UART_STATUS_PE_OFST             (0)
#define ALTERA_AVALON_UART_STATUS_FE_MSK              (0x2)
#define ALTERA_AVALON_UART_STATUS_FE_OFST             (1)
#define ALTERA_AVALON_UART_STATUS_BRK_MSK             (0x4)
#define ALTERA_AVALON_UART_STATUS_BRK_OFST            (2)
#define ALTERA_AVALON_UART_STATUS_ROE_MSK             (0x8)
#define ALTERA_AVALON_UART_STATUS_ROE_OFST            (3)
#define ALTERA_AVALON_UART_STATUS_TOE_MSK             (0x10)
#define ALTERA_AVALON_UART_STATUS_TOE_OFST            (4)
#define ALTERA_AVALON_UART_STATUS_TMT_MSK             (0x20)
#define ALTERA_AVALON_UART_STATUS_TMT_OFST            (5)
#define ALTERA_AVALON_UART_STATUS_TRDY_MSK            (0x40)
#define ALTERA_AVALON_UART_STATUS_TRDY_OFST           (6)
#define ALTERA_AVALON_UART_STATUS_RRDY_MSK            (0x80)
#define ALTERA_AVALON_UART_STATUS_RRDY_OFST           (7)
#define ALTERA_AVALON_UART_STATUS_E_MSK               (0x100)
#define ALTERA_AVALON_UART_STATUS_E_OFST              (8)
#define ALTERA_AVALON_UART_STATUS_DCTS_MSK            (0x400)
#define ALTERA_AVALON_UART_STATUS_DCTS_OFST           (10)
#define ALTERA_AVALON_UART_STATUS_CTS_MSK             (0x800)
#define ALTERA_AVALON_UART_STATUS_CTS_OFST            (11)
#define ALTERA_AVALON_UART_STATUS_EOP_MSK             (0x1000)
#define ALTERA_AVALON_UART_STATUS_EOP_OFST            (12)

#define ALTERA_AVALON_UART_CONTROL_PE_MSK             (0x1)
#define ALTERA_AVALON_UART_CONTROL_PE_OFST            (0)
#define ALTERA_AVALON_UART_CONTROL_FE_MSK             (0x2)
#define ALTERA_AVALON_UART_CONTROL_FE_OFST            (1)
#define ALTERA_AVALON_UART_CONTROL_BRK_MSK            (0x4)
#define ALTERA_AVALON_UART_CONTROL_BRK_OFST           (2)
#define ALTERA_AVALON_UART_CONTROL_ROE_MSK            (0x8)
#define ALTERA_AVALON_UART_CONTROL_ROE_OFST           (3)
#define ALTERA_AVALON_UART_CONTROL_TOE_MSK            (0x10)
#define ALTERA_AVALON_UART_CONTROL_TOE_OFST           (4)
#define ALTERA_AVALON_UART_CONTROL_TMT_MSK            (0x20)
#define ALTERA_AVALON_UART_CONTROL_TMT_OFST           (5)
#define ALTERA_AVALON_UART_CONTROL_TRDY_MSK           (0x40)
#define ALTERA_AVALON_UART_CONTROL_TRDY_OFST          (6)
#define ALTERA_AVALON_UART_CONTROL_RRDY_MSK           (0x80)
#define ALTERA_AVALON_UART_CONTROL_RRDY_OFST          (7)
#define ALTERA_AVALON_UART_CONTROL_E_MSK              (0x100)
#define ALTERA_AVALON_UART_CONTROL_E_OFST             (8)
#define ALTERA_AVALON_UART_CONTROL_DCTS_MSK           (0x400)
#define ALTERA_AVALON_UART_CONTROL_DCTS_OFST          (10)
#define ALTERA_AVALON_UART_CONTROL_RTS_MSK            (0x800)
#define ALTERA_AVALON_UART_CONTROL_RTS_OFST           (11)
#define ALTERA_AVALON_UART_CONTROL_EOP_MSK            (0x1000)
#define ALTERA_AVALON_UART_CONTROL_EOP_OFST           (12)

// Pointers to touch control registers for transmitting/receiving data
volatile unsigned int *Touch_RxReg;
volatile unsigned int *Touch_TxReg;
volatile unsigned int *Touch_ControlReg;
volatile unsigned int *Touch_StatusReg;

void *LW_virtual;          // used to map physical addresses for the light-weight bridge


// Datatype to hold the x,y coordinates of a touch/release event
typedef struct { int x, y; } Point ;

// Functions for handling the LW bridge
// Taken from `increment_leds` from Intel University Program, available on the ARM tutorial files
int open_physical (int);
void * map_physical (int, unsigned int, unsigned int);
void close_physical (int);
int unmap_physical (void *, unsigned int);


// Most of the functions below were taken from https://github.com/ShreyansK2000/Translaite/blob/master/Hardware/Touch.h

/**
 * Subroutine to initialise the Touch Port by writing some data
 * to the internal registers.
 *
 * Call this function at the start of the program before you attempt
 * to read or write to data via the Touch port.
 *
 * Refer to UART data sheet for details of registers and programming.
*/
int init_touch(int fd);

/**
 * Sends the touch_enable command to the AR1100 controller
 *
*/
int touch_enable(void);

/**
 * Sends the touch_disable command to the AR1100 controller
 *
*/
int touch_disable(void);

/**
 * Read and write from an AR1100 register
 * Used when checking and updating touch mode
 *
 * Adapted from https://github.com/Si4463Project/RadioCode/blob/master/Source/drv_ar1100.h
*/
void ar1100_writeReg (unsigned char reg_addr, unsigned char write_val);
void ar1100_readReg (unsigned char reg_addr, unsigned char num_bytes);


/**
 * Writes a character to the touch screen serial port.
 */
int put_char_touch(int c);

/**
 * Blocks until a character is received from the touch screen serial port.
 */
int get_char_touch(void);

/**
 * Blocks until a character is received from the touch screen serial port or timeout.
 */
int get_char_touch_timeout(int time);


/**
 * Blocks until a touch event is detected.
 */
void wait_for_touch();

/**
 * Blocks for up to time cycles until a touch event is detected.
 */
int wait_for_touch_timeout(int time);


/**
 * This function polls the UART to determine if any character
 * has been received. It doesn't wait for one, or read it, it simply tests
 * to see if one is available to read from the FIFO
 */
int touch_test_for_received_data(void);

/**
 * This function waits for a touch screen press event and returns X,Y coord
 */
Point get_press(void);

/**
 *	Waits for a press event; returns X,Y coords, or (-1,-1) if timeout
*/
Point get_press_timeout(int time);

#endif