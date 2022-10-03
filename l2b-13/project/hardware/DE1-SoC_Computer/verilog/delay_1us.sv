
`define IDLE 6'b000000
`define FINISH 6'b111111


// delay_1us
//		This FSM is used to create a delay of about 1us
//		Takes a 50MHz clock as input
//		Starts the counter when start is called, signals finish when 1us has passed

// 50MHz has a period of 0.02us
// So, we need 50 periods to delay 1us
// To make things simpler, we can just delay by 64 cycles, which is about 1.28 us
// Using this, we can use 6 state bits, where 0 is IDLE and 63 is FINISH

module delay_1us (input logic clk, start,
					output logic finish);

	reg [5:0] state = `IDLE;
	
	always_ff @(posedge clk)
		case(state)
			`IDLE:	begin
						finish <= 1'b0;
						state <= (start) ? 6'b000001 : `IDLE;
					end
			`FINISH: begin
						finish <= 1'b1;
						state <= `IDLE;
					end
			default: state <= state + 6'd1;
		endcase

endmodule