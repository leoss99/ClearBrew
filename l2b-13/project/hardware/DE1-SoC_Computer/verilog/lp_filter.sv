
module lp_filter
	#(parameter FIFO_DEPTH = 8)
	(	input logic clk,
		input logic [23:0] data_in,
		output reg [23:0] data_out);



logic [23:0] data_in_div, next, accumulate;

// Divide the incoming data by the number of stages in the buffer
assign data_in_div = data_in / FIFO_DEPTH; // TODO: check that this uses a shifter, not divider


// Want FIFO_DEPTH signals of 24 bits each
reg [23:0] fifo_regs [FIFO_DEPTH-1:0];

// data_in_div is the input to the FIFO, and fifo_regs[FIFO_DEPTH-1] is the output
widereg yep0(.indata(data_in_div),.outdata(fifo_regs[0]),.inclk(clk));

genvar i;
generate
	for (i = 1; i < FIFO_DEPTH; i++) begin: fifo
		widereg yep(.indata(fifo_regs[i-1]),.outdata(fifo_regs[i]),.inclk(clk));
	end
endgenerate



// 
assign next = data_in_div - fifo_regs[FIFO_DEPTH-1];

assign data_out = next + accumulate;

widereg accumulate_reg(.indata(data_out),.outdata(accumulate),.inclk(clk));

endmodule