# DE1-SoC Computer

Our project runs on a modified version of the DE1-SoC computer, which is
avaiable as part of the [Intel Monitor Program](https://fpgacademy.org/tools.html).
In order to use the DE1-SoC Computer on the DE1-SoC, you will need to make an
rbf file which you can use to program the FPGA from the HPS.
In Platform Designer, generate the system based on `Computer_System.qsys`.
In Quartus (18.1), compile the system after including the generated HDL files.
This takes me (unfortunately) about 40 minutes to generate the sof file.
Once the sof file is generated, follow [page 60 of this document](https://ftp.intel.com/Public/Pub/fpgaup/pub/Teaching_Materials/current/Tutorials/Linux_On_DE_Series_Boards.pdf)
to generate an rbf file, called `DE1_SoC_Computer.rbf`. Once generated,
go to the `hardware/arm-hps` section of this repository to see how to program the DE1.

### Touch Controller

The DE1-SoC Computer iteracts with an AR1100 touch controller via an RS-232 serial port.
This is simply an IP core in Platform Designer.
The serial port transmits through GPIO_0 pin 10 and receives through GPIO_0 pin 11.
Note that transmit and receive pins should be crossed over when connecting to the
AR1100; DE1 transmit connects to AR1100 receive, and so on.

### Load Cell

The HX711 load cell amplifier produces a serial, digital signal.
When a measurement is ready, pulses can be made to its clock pin and data
will be shifted out on its data pin.
To make it easy to read data from the HX711 via the HPS, a custom Verilog
state machine has been used to parallelize the data when it is ready.
Then, a custom FIR averaging filter is used to de-noise the weight signal;
as a result, it takes longer for the signal to reach a steady weight, but
the signal is much steadier when it gets there.

The HPS interacts with the load cell through a 24-bit PIO. Data can be read whenever it is needed.
The HX711 clock pin connects to GPIO_0 pin 1, and the data pin connects to GPIO_0 pin 0.