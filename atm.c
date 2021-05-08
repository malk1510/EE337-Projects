#include <at89c5131.h>
#include "lcd.h"		//Header file with LCD interfacing functions
#include "serial.c"	//C file with UART interfacing functions

sbit LED=P1^7;

//Test function definitions

void print_dem(unsigned char* den[], unsigned int notes[]) // Function to print the current state of notes - 2000/500/200/100
{
	unsigned int i=0;
	lcd_cmd(0x01); // Initialising the LCD display
	lcd_cmd(0x80);

	for(i=0;i<4;i++){
		if(notes[i]>0){
			lcd_write_string(den[i]); // Write the corresponding denomination on LCD if quantity>0
			if(i<3)
				lcd_write_char(','); // Placing commas between successive denominations
		}
	}
	lcd_cmd(0xC0);
}

//Main function
void main(void)
{
	unsigned char ch=0; // initialising the received character from UART
	unsigned int den_m[4] = {2000, 500, 200, 100}; // Initialising the denominations
	unsigned char* den[4] = {"2000", "500", "200", "100"};
	unsigned int notes[] = {20, 30, 10, 10}; // Initialising the quantities
	unsigned int x[] = {0,0,0,0}; // Array to hold quantities of denominations 
	//Initialize port P1 for output from P1.7-P1.4
	P1 = 0x0F;
	
	//Call initialization functions
	lcd_init();
	uart_init();
	transmit_string("Type the amount you want to withdraw\r\n");
	print_dem(den, notes); // Displaying the initial state
	while(1)
	{
			//Receive a character
			unsigned int amt = 0;
			ch = receive_char(); // Receiving character from UART
		
			if(ch=='d'){
				int i=0;
				lcd_cmd(0x01); // Making LCD ready for display
				lcd_cmd(0x80);
				
				for(i=0;i<2;i++){
					lcd_write_string(den[2*i]); // String 2000/200
					lcd_write_char(':');
					lcd_write_char('0'+(notes[2*i])/10); // Writing the 10s digit of quantity
					lcd_write_char('0'+(notes[2*i])%10); // Writing the 1s digit of quantity
					lcd_write_char(',');
					lcd_write_string(den[2*i+1]); // String 500/100
					lcd_write_char(':');
					lcd_write_char('0'+(notes[2*i+1])/10); // Writing the 10s digit of quantity
					lcd_write_char('0'+(notes[2*i+1])%10); // Writing the 1s digit of quantity
					lcd_write_char(',');
					lcd_cmd(0xC0);
				}
				msdelay(3000); // 3sec delay after printing
				print_dem(den, notes); // Printing finally the denominations available
			}
			else{
				if(ch=='0' || ch=='\r'){ // Invalid input for 0 or enter as the first character
					lcd_write_string("Invalid input");
					msdelay(2000);
					print_dem(den, notes); continue;
				}
				while(ch!='\r'){ // If 1st character received is not 0 or d or enter - ATM operation has to be done
					if(ch>='0' && ch<='9'){ 
						lcd_write_char(ch); // Display the amount typed in UART simultaneously on LCD
						amt = amt*10 + (ch-'0'); // Left shifting the previous bits 
						ch = receive_char(); // Keep received the value
					}
					else
						break; // Stop when character received isn't a digit
				}
				if((ch!='\r')||(amt%100!=0)){ // If last character received isn't enter or doesn't end with 2 zeroes
					print_dem(den, notes);
					lcd_write_string("Invalid input"); // Display "Invalid Input"
				}
				else{ // Case when all conditions for a valid input is satisfied
					unsigned int i;
					unsigned int amount = amt, temp=0;
					for(i=0;i<4;i++)
						x[i] = notes[i]; // Copying the quantities of notes to temporary array for the adjustment
					for(i=0;i<4;i++){
						if(amount==0) 
							break; // When the cash required is taken out successfully
						temp = amount/den_m[i]; // Check number of notes required in decreasing order of magnitude
						amount -= den_m[i] * ((temp>x[i])?x[i]:temp); //if notes required > notes available, then deduct all the available denominations
						x[i] -= (temp>x[i])?(x[i]):temp; // update present available denominations based upon comparison between notes required and notes available
			}
			if(amount==0){ // Update the available notes after atm is ready to output the cash for the input
				for(i=0;i<4;i++)
					notes[i] = x[i];
				print_dem(den, notes);
				lcd_write_string("Collect Cash");}
			else{
				if(notes[3]==0){
				if(amt%2000==100){
					if((amt/2000)==0){
				print_dem(den, notes);
				lcd_write_string("Try again");}
					else{
						if(x[1]>=3 && x[2]>=3)
						{
							x[1] -= 3;
							x[2] -= 3;
							x[0]++;
							for(i=0;i<4;i++)
							notes[i] = x[i];
							print_dem(den, notes);
							lcd_write_string("Collect Cash");
						}
						else
						{
							if(x[1] >=1 && x[2] >=8)
							{
								x[1] -= 1;
								x[2] -= 8;
								x[0]++;
								for(i=0;i<4;i++)
								notes[i] = x[i];
								print_dem(den, notes);
								lcd_write_string("Collect Cash");
							}
							else
							{
								print_dem(den, notes);
								lcd_write_string("Try again");
							}
						}
					}
				}
				else{
					if(x[2] >=3){
						x[1]++;
						x[2]-=3;
						for(i=0;i<4;i++)
							notes[i] = x[i];
							print_dem(den, notes);
							lcd_write_string("Collect Cash");
					}
					else
					{
						print_dem(den, notes);
						lcd_write_string("Try again");
					}
				}
								
							
				}
				else
				{
					print_dem(den, notes);
					lcd_write_string("Try again");
				}
			}
				
			}
			msdelay(2000);
			print_dem(den, notes);
		}
			
	}
}