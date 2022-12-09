// Ruth Mueller, Kevin Wan,12/8/2022
// rmueller@hmc.edu, kewan@hmc.edu
// E155 Final Project
//Random Clock, FPGA code
module top(input logic reset,sck,sdi,
	output logic neopixels,
	output logic [4:0] en,
	output logic [7:0] segDisplay); 
        // instantiates the on board 48MHz clock, the shift register that receives SPI transactions from the MCU. 
        // also instantiates the decoder that reformats the shift register output, and the main fsm (num_distribute module). 
	logic clk;
	logic [95:0] clock_num;
	logic [3:0] num1,num2,num3,num4,num5,num6,num7,num8,num9,num10,num11,num12;
	HSOSC hf_osc(.CLKHFPU(1'b1),.CLKHFEN(1'b1),.CLKHF(clk));// initializes onboard clk. 
	clock_spi CLOCK_SPI(sck,sdi,reset,clock_num);
	bit_rearrange BIT_REARRANGE(clock_num,num1,num2,num3,num4,num5,num6,num7,num8,num9,num10,num11,num12);
	num_distribute NUM_DISTRIBUTE(clk,reset,neopixels,en,segDisplay,num1,num2,num3,num4,num5,num6,num7,num8,num9,num10,num11,num12);
endmodule

module num_distribute(
	input logic clk,reset,
	output logic out,
	output logic [4:0] en,
	output logic [7:0] segDisplay,
	input logic [3:0] num1,num2,num3,num4,num5,num6,num7,num8,num9,num10,num11,num12
	); 
	// instantiates the multiplexer that chooses which display gets to light up at any moment. 
	// instantiates a clock scaler that sets the multiplexing frequency to 100Hz. instantiates the core neopixel module. 
	
	// internal logic variables

	logic clk_100Hz; 
	logic clk_slooooow;
    	// num1 refers to the signal sent to the 1 o'clock location, num2 refers to signal sent to the 2 o'clock location, etc. 
	
	logic [3:0] numToDisplay;
		

	always_comb begin 
		case (en)
			5'b00000: numToDisplay = num10 % 10;
			5'b00001: begin 
						if (num10 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b00010: numToDisplay = num9 % 10;
			5'b00011: begin 
						if (num9 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b00100: numToDisplay = num8 % 10;
			5'b00101: begin 
						if (num8 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b00110: numToDisplay = num7 % 10;
			5'b00111: begin 
						if (num7 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b01000: numToDisplay = num6 % 10;
			5'b01001: begin 
						if (num6 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b01010: numToDisplay = num5 % 10;
			5'b01011: begin 
						if (num5 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b01100: numToDisplay = num4 % 10;
			5'b01101: begin 
						if (num4 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b01110: numToDisplay = num3 % 10;
			5'b01111: begin 
						if (num3 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b10000: numToDisplay = num2 % 10;
			5'b10001: begin 
						if (num2 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b10010: numToDisplay = num1 % 10;
			5'b10011: begin 
						if (num1 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b10100: numToDisplay = num12 % 10;
			5'b10101: begin 
						if (num12 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			5'b10110: numToDisplay = num11 % 10;
			5'b10111: begin 
						if (num11 > 9) numToDisplay = 1'b1;
						else numToDisplay = 0;
					  end
			default: numToDisplay = 0;
		endcase	
	end
	


	
	slow_clock slow1(clk, clk_100Hz);  //slow down the clock to 100Hz. 
	en_rotate enables(clk_100Hz, en);  
	display_digit displayTemp(numToDisplay, segDisplay);
	neopixels neos(reset,clk, out);   // CONNECTING TO NEOPIXELS
	
	
endmodule

module slow_clock(
	input logic clk,
	output logic s
	);
	
	logic [20:0] counter; // current count state
	logic [20:0] nextCount; // next count state
	logic 		nexts;  // next led state
	// this module takes in a 48 MHz clock and slows it for the enable
	// next state logic
	always_comb begin
		if(counter ==  7040)  begin // b0000001101110000000
			if (s == 0) nexts = 1;
			else nexts = 0;
			nextCount = 20'b000_0000_0000_0000_0000;  
		end else begin
			nextCount = counter + 1'b1;
			nexts = s;
		end
	end
	
	// flip flop
	always_ff @(posedge clk) begin
		counter <= nextCount;
		s <= nexts;
	end
	
endmodule





module en_rotate(
	input logic clk,
	output logic [4:0] enable
	);
	// unconditionally rotates the enables for each LED display. 
	// internal logic for count state
	logic [4:0] next_count;
	
	// next state logic
	always_comb begin
		if(enable >= 23)  next_count = 0;
		else begin
			next_count = enable + 1;
		end
	end
	
	// flip flop
	always_ff @(posedge clk) begin
		enable <= next_count;
	end
	
	endmodule



module display_digit(
	input logic [3:0] num,  //p37 is the 1s place, p31=2, p35=4, p32=8
	output logic [7:0] display // display = [A,B,C,D,E,F,G] as listed on 
			// the JAMECO VALUEPRO (UA5651-11-R) LED,7-SEGMENT,RED,700NM datasheet
			// (clockwise starting with the center top light, with the middle horizontal light as G
			// all the light configurations are notted so as to 
);

	// a multiplexer to select 1 out of 24 seven-seg displays to be active at a given moment. 
	logic [3:0] binary_number;

	always_comb begin
		binary_number = num;
	end

	always_comb
		case(binary_number)  // I am notting the input number becuase it uses pullup resistors
			0: display 	 = 	~8'b0111_1110;
			1: display 	 = 	~8'b0011_0000;
			2: display 	 = 	~8'b1110_1101;
			3: display 	 = 	~8'b0111_1001;
			4: display 	 = 	~8'b0011_0011;
			5: display 	 = 	~8'b1101_1011;
			6: display 	 = 	~8'b1101_1111;
			7: display 	 = 	~8'b0111_0000;
			8: display 	 = 	~8'b0111_1111;
			9: display 	 = 	~8'b1111_0011;
			10: display  = 	~8'b0111_0111;
			11: display  = 	~8'b0001_1111;
			12: display  = 	~8'b0000_1101;
			13: display  = 	~8'b0011_1101;
			14: display  =  ~8'b0100_1111;
			15: display  =  ~8'b0100_0111;
			default: display = ~8'b000_0001;
		endcase
endmodule

module Low_Encoder(input logic clk,reset,output logic low_out);
	// outputs a 800kHz wave with 33% duty cycle. Assumes the clk coming in is 48MHz.
	logic [5:0] counter = 0;
	always_ff @(posedge clk,posedge reset)
		begin
			if (reset) counter <= 0; 
		    else if (counter == 59) counter<=0;
			else counter <= counter + 1;
		end
	assign low_out = (counter < 20);
	
endmodule
module High_Encoder(input logic clk,reset,output logic high_out);
	// outputs a 800kHz wave with 66% duty cycle. Assumes the clk coming in is 48MHz.
	logic [5:0] counter = 0;
	always_ff @(posedge clk,posedge reset)
		begin
			if (reset) counter <= 0; 
		    else if (counter == 59) counter<=0;
			else counter <= counter + 1;
end
	assign high_out = (counter < 40);
endmodule
module mux2 (input logic d0,
			 input logic d1,
			 input logic S,
			 output logic Y);
	assign Y = S ? d1:d0;
endmodule

module neopixels(input logic reset, input logic int_osc, output logic out);
	// Neopixels master module. 

	logic high,low,color,almost,neo_control; // internal signals. 
	High_Encoder High(int_osc,reset,high);
	Low_Encoder Low(int_osc,reset,low);
	Color_encoderG Red(int_osc,reset,color); // describes the starting color of the display. 
	mux2 MUX2(low,high,color,almost);	
	RST_control rst_control(int_osc,reset,neo_control); // dictates how quickly the colors cycle. 
	assign out = (neo_control & almost); 
endmodule


module Color_encoderG(input logic clk,reset,output logic color); 
	// This Color_encoder specifically gives a green color upon instantiation. 
	logic [11:0] counter = 0;
	always_ff @(posedge clk,posedge reset)
		begin
			if (reset) counter <= 0; 
		    else if (counter == 1439) counter<=0;
			else counter <= counter + 1;
		end
	assign color = (counter<480);
endmodule


module RST_control(input logic clk,reset,output logic reset_control); 
	// controls how quickly the colors cycle through the rainbow. Change the number in this line "else if (counter == 3686403)" to adjust. See in line comments. 
	// at 3638399, no cycle effect is given. 
	logic [24:0] counter = 0;
	always_ff @(posedge clk,posedge reset)
		begin
			if (reset) counter <= 0; 
		    else if (counter == 3686403) counter<=0; // intended number is 3686399. , increment a little for rainbow effect. The larger it is (<xxx420), the faster the rainbow cycles. 3686400 will result in a very slow rainbow cycle (around 2 min)
			else counter <= counter + 1;
		end
	assign reset_control = (counter < 3683520); //previous number - 2880 
endmodule
module clock_spi(input logic sck,sdi,reset,
	output logic [95:0] clock_num);
	// shift register to receive SPI signal from MCU. 
	
	always_ff @(posedge sck)
		if (reset) clock_num = 0;
		else clock_num = {clock_num[94:0],sdi};
endmodule
module bit_rearrange(input logic [95:0] clock_num,output logic [3:0] num1,num2,num3,num4,num5,num6,num7,num8,num9,num10,num11,num12); 
	// rearranges the shift register outputs into format compatible with num_distribute module. 
	assign num1 = clock_num[91:88];
	assign num2 = clock_num[83:80];
	assign num3 = clock_num[75:72];
	assign num4 = clock_num[67:64];
	assign num5 = clock_num[59:56];
	assign num6 = clock_num[51:48];
	assign num7 = clock_num[43:40];
	assign num8 = clock_num[35:32];
	assign num9 = clock_num[27:24];
	assign num10 = clock_num[19:16];
	assign num11 = clock_num[11:8];
	assign num12 = clock_num[3:0];
endmodule

