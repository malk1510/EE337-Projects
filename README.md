# EE337-Projects
This repo comprises of the project topics I've completed from the required projects for EE 337

## Obstacle
This program generates the Google Dinosaur game on the LCD, with a 'D' representing the dinosaur and a * representing the obstacles that appear.
The orientation of these obstacles updates every second, such that the user always has a chance to dodge the obstacle.
The dinosaur D stays in the third column of the LCD, and it's position can be toggled up by pressing W or brought down by pressing S.
Each obstacle avoided gets you one point, and whenever the dinosaur hits an obstacle, the game ends.
The file for this program is obstacle.c

## ATM
This program generates a makeshift ATM system on the LCD using Uart communication. This is done by entering the amount to withdraw on the keyboard, or pressing the character 'D' to check the amount of notes of each denomination left in the ATM.
If any amount requested exceeds the notes available in the atm, or cannot be withdrawn using the given denominations present in the atm, the LCD shows that the requested amount is invalid.
The file for this program is atm.c

## Autofare
This program generates a auto rickshaw meter on the LCD. There are 4 different speeds with which the auto can run, which are set using the SW0 and SW1 switches of the LCD. 
This program calculates and displays the amount charged at each second of its runtime.
This amount charged starts off with Rs. 21 for the first 1.5 km of the trip; after which the rate is Rs. 14 per kilometer.
The switch SW3 is set if the auto is waiting on a standstill. In that case, Rs. 1.40 are charged for every 5 seconds of waiting time.
The file for this program is autofare.asm

## Bankqueue
This program generates a queue of customers waiting to be served by four bank employees: a,b,c,d on four separate counters.
Each new customer can be given a token if they press the character t on the keyboard. Then, they would be added to the queue of waiting customers.
Whenever the switch corresponding to each of the bank counters is toggled, the top of this queue is popped off the tokens list, and sent to the respective bank counter.
This continues until 100 tokens are given, after which the tokens get reset and start back from 0.
However, whenever the tokens are not added to the queue, the four bank counters along with the tokens at each counter are shown on the LCD.
