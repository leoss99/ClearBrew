

//	clk_out		--------------------+
//	delay_call	------------------+ |
//	shift_out	----------------+ | |
//	State Counter-----------+   | | |
//						   vvvv v v v
`define IDLE			7'b0000_0_0_0
`define DELAY_CALL_1	7'b0001_0_1_0
`define DELAY_WAIT_1	7'b0010_0_0_0
`define PULSE_HIGH		7'b0011_0_0_1
`define DELAY_CALL_2	7'b0101_0_1_1
`define DELAY_WAIT_2	7'b0110_0_0_1
`define PULSE_LOW		7'b0111_0_0_0
`define DELAY_CALL_3	7'b1000_0_1_0
`define DELAY_WAIT_3	7'b1001_0_0_0
`define SHIFT_BIT		7'b1010_0_0_0
`define	INCREMENT_COUNT	7'b1011_0_0_0
`define CHECK_COUNT		7'b1100_0_0_0
`define REGISTER_RESULT	7'b1101_1_0_0


// NOTE: Assumes data_in is synchronized to CLK_50; this needs to be done in the top-level module


module read_load_cell(input logic CLK_50, data_in, delay_continue,
					output logic delay_call, clk_out, shift_out,
					output reg [23:0] data_out = 24'b0);


assign clk_out 		= state[0];
assign delay_call 	= state[1];
assign shift_out	= state[2];


reg [6:0] state = `IDLE; // State bits

// Note: We get 24 bits from the HX711, but we need to send 25 pulses.
//		The last bit read will always be 1, so just ignore it and output 24 bits.
reg [24:0] data_shift = 25'b0; // 25 bits as they are shifted out
reg [4:0] count = 5'b0; // count number of bits that have been shifted

always_ff @(posedge CLK_50)
	case(state)
		`IDLE:				state <= (data_in) ? `IDLE : `DELAY_CALL_1;
		`DELAY_CALL_1:		state <= `DELAY_WAIT_1;
		`DELAY_WAIT_1:		state <= (delay_continue) ? `PULSE_HIGH : state;
		`PULSE_HIGH:		state <= `DELAY_CALL_2;
		`DELAY_CALL_2:		state <= `DELAY_WAIT_2;
		`DELAY_WAIT_2:		state <= (delay_continue) ? `PULSE_LOW : state;
		`PULSE_LOW:			state <= `DELAY_CALL_3;
		`DELAY_CALL_3:		state <= `DELAY_WAIT_3;
		`DELAY_WAIT_3:		state <= (delay_continue) ? `SHIFT_BIT : state;
		`SHIFT_BIT:			begin
								data_shift <= {data_shift[23:0],data_in};
								state <= `INCREMENT_COUNT;
							end
		`INCREMENT_COUNT:	begin
								count <= count + 5'd1;
								state <= `CHECK_COUNT;
							end
		`CHECK_COUNT:		if (count == 25)
								state <= `REGISTER_RESULT;
							else
								state <= `PULSE_HIGH;
		`REGISTER_RESULT:	begin
								count <= 0;
								state <= `IDLE;
							end
		default:			state <= `IDLE;
	endcase
	
// When we go to REGISTER_RESULT, register data_shift
always_ff @(posedge shift_out)
	data_out <= data_shift[24:1];

endmodule