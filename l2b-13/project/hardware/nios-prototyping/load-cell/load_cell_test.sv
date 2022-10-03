
// High-level module for testing the load cell and amplifier

module load_cell_test (input logic CLOCK_50,
						input logic [3:0] KEY,
						inout logic [35:0] GPIO_0,
						output logic [9:0] LEDR,
						output logic [6:0] HEX0,
						output logic [6:0] HEX1,
						output logic [6:0] HEX2,
						output logic [6:0] HEX3);

logic delay_start, delay_finish, shift_out;
reg [23:0] load_cell_data, filtered_data;

logic data_in_sync;

// Synchronizer for data_in
doublesync data_sync	(.indata(GPIO_0[0]),
						.outdata(data_in_sync),
						.clk(CLOCK_50),
						.reset(1'b1)); // Active low reset

// 
read_load_cell reader( // Inputs
					.CLK_50(CLOCK_50), .data_in(data_in_sync), .delay_continue(delay_finish),
					// Outputs
					.delay_call(delay_start), .clk_out(GPIO_0[1]),.shift_out(shift_out),
					.data_out(load_cell_data));

delay_1us delay		(// Inputs
					.clk(CLOCK_50), .start(delay_start),
					// Outputs
					.finish(delay_finish));


lp_filter #(.FIFO_DEPTH(32)) lp (.clk(shift_out),.data_in({{4{load_cell_data[23]}},load_cell_data[23:4]}),.data_out(filtered_data));
//lp_filter #(.FIFO_DEPTH(32)) lp (.clk(shift_out),.data_in(load_cell_data),.data_out(filtered_data));
//widereg filter(.indata(load_cell_data),.outdata(filtered_data),.inclk(shift_out));


assign HEX0 = ~filtered_data[6:0];
assign HEX1 = ~filtered_data[13:7];
assign HEX2 = ~filtered_data[20:14];
assign HEX3 = {4'b1111,~filtered_data[23:21]};



// Nios System:
load_cell_pd u0 (
		.clk_clk(CLOCK_50),       //   clk.clk
		.load_cell_pio_export(filtered_data),
		.reset_reset_n(KEY[0])  // reset.reset_n
	);


endmodule
