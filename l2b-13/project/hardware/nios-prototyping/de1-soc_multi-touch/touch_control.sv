

module touch_control (
	input CLOCK_50,
	input [3:0] KEY,
	inout [35:0] GPIO_0
);


	touch_control_pd u0 (
		.clk_clk(CLOCK_50),       //   clk.clk
		.reset_reset_n(KEY[0]),  // reset.reset_n
		.touch_uart_rxd(GPIO_0[11]), // touch_uart.rxd (input)
		.touch_uart_txd(GPIO_0[10])  //           .txd (output)
	);


endmodule