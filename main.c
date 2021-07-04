// This program demonstrates how to initialize a frame buffer for a
// 1024 x 768 display, and how to draw on it using a simple checker board
// pattern.

// Included header files
#include "uart.h"
#include "framebuffer.h"
#include "snes_uart.h"
#include "systimer.h"
#include "gpio.h"

#define BUTTON_B  (1<<0)
#define BUTTON_Y  (1<<1)
#define BUTTON_SEL  (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_UP  (1<<4)
#define BUTTON_DOWN  (1<<5)
#define BUTTON_LEFT  (1<<6)
#define BUTTON_RIGHT  (1<<7)
#define BUTTON_A  (1<<8)
#define BUTTON_X  (1<<9)
#define BUTTON_L  (1<<10)
#define BUTTON_R  (1<<11)
#define BLACK     0x00000000
#define WHITE     0x00FFFFFF
#define RED       0x00FF0000
#define GREEN     0x00008000
#define MAZESIZEY 12
#define MAZESIZEX 16
#define SQUARESIZE 64

//Functions
void drawSquare(int rowStart, int columnStart, int squareSize, unsigned int color);


int maze[12][16] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {2, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
    {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 3},
    {1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int squareX, squareY;

//Starting position of the player
void startState(){
    squareX = 0;
    squareY = 2;
}

//Function to draw user square
void drawPlayer(unsigned int color){
    drawSquare(squareY*SQUARESIZE, squareX*SQUARESIZE, SQUARESIZE, color);
}

//Draws the game
void drawGame(){
    for(unsigned int x = 0; x <MAZESIZEX; x++){
        for(unsigned int y = 0; y <MAZESIZEY; y++){
            changeSquare(x,y); 
        }
    }
}

//If the part of the maze is on a valid square it will return 'v'. If it is invalid, it will return an 'i'
//Else return a 'w', which in this case would be the winning state
char move(int x, int y){
    if (maze[y][x] == 0 || maze [y][x] == 2){
        return 'v';
    }
    else if (maze [y][x] == 1){
        return 'i';
    }
    else{
        return 'w';
    }
}

//Function made to change thh color of a square. 
void changeSquare(int x, int y){
    if(maze[y][x] == 0 || maze [y][x] == 2 || maze [y][x] == 3){
        drawSquare(y*SQUARESIZE, x*SQUARESIZE, SQUARESIZE, WHITE);
    }
    else if (maze [y][x] == 1){
        drawSquare(y*SQUARESIZE, x*SQUARESIZE, SQUARESIZE, BLACK);
    }
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       main
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function initializes the UART terminal and initializes
//                  a frame buffer for a 1024 x 768 display. Each pixel in the
//                  frame buffer is 32 bits in size, which encodes an RGB value
//                  (plus an 8-bit alpha channel that is not used). The program
//                  then draws and displays an 18 x 12 checker board pattern.
//
////////////////////////////////////////////////////////////////////////////////

void main()
{   
    unsigned short data;
    unsigned short currentState = 0xFFFF;

    //Initialize the UART terminal
    uart_init();

    // Set up GPIO9
    //init_GPIO9_to_output();

    //Set up GPIO11
    //init_GPIO11_to_output();

    //Set up GPIO10 to input
    //init_GPIO10_to_input();

    //Clear GPIO 9 pin
    //clear_GPIO9();
    
    //Set up GPIO 11 for SNES 
    //set_GPIO11();

    //get_SNES(); would retrieve the one that uses the GPIO but for this purpose I used the keyboard one.

    //Initialize the frame buffer
    initFrameBuffer();

    //Draw on the frame buffer and display it
    drawGame();

    // Loop forever, echoing characters received from the console
    // on a separate line with : : around the character
    while (1) {
        // Wait for a character input from the console
        data = get_SNES();

        if(data!= currentState){
            if((data & BUTTON_START) == BUTTON_START){
                if ((maze[squareY][squareX] == 1) || (maze[squareY][squareX]  == 3)){
                    drawGame();
                    startState();
                    changeSquare(squareX, squareY);
                    drawPlayer(RED);
                }
               
            }
            if((data & BUTTON_LEFT) == BUTTON_LEFT){
                if((move(squareX-1, squareY) == 'i') ){
                    squareX += 0;
                    changeSquare(squareX, squareY);
                    drawPlayer(RED);
       
                }
                else if((move(squareX, squareY) == 'v')){
                  changeSquare(squareX, squareY); 
                  squareX -= 1; 
                  drawPlayer(RED);
                }
            }
            if((data & BUTTON_RIGHT) == BUTTON_RIGHT){
                if((move(squareX+1, squareY) == 'i')){
                    squareX -= 0;
            
            
                }
                else if((move(squareX, squareY) == 'v')){
                  changeSquare(squareX, squareY);
                  squareX += 1; 
                  drawPlayer(RED); 
                }
            }

            if((data & BUTTON_UP) == BUTTON_UP){
                if((move(squareX, squareY-1) == 'i')){
                    
                    
                   squareY += 0;
                    changeSquare(squareX, squareY);
                    drawPlayer(RED);
               
                }
                else if((move(squareX, squareY) == 'v')){
                  changeSquare(squareX, squareY); 
                  squareY -= 1; 
                  drawPlayer(RED); 
                }
            }
            if((data & BUTTON_DOWN) == BUTTON_DOWN){
                if((move(squareX, squareY+1) == 'i')){
                  
                    squareY -= 0;
         
                 
                }
                else if((move(squareX, squareY) == 'v')){
                  changeSquare(squareX, squareY);
                  squareY += 1; 
                  drawPlayer(RED); 
                }
            }  
            currentState = data;
        }


        if (maze[squareY][squareX] == 3){
            changeSquare(squareX,squareY);
            drawPlayer(GREEN);
        }

        microsecond_delay(33333); 
        }

    }



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO9_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 9 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO9_to_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 0
    r = *GPFSEL0;

    // Clear bits 27 - 29. This is the field FSEL9, which maps to GPIO pin 9.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 27);

    // Set the field FSEL9 to 001, which sets pin 9 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 27);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 0
    *GPFSEL0 = r;


    // Disable the pull-up/pull-down control line for GPIO pin 9.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN0 register
    r = *GPPUPPDN0;

    // Zero out bits 18-19 in this bit pattern, since this maps to
    // GPIO pin 9. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 18);

    // Write the modified bit pattern back to the
    // GPPUPPDN0 register
    *GPPUPPDN0 = r;
}




////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO9
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 9
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////


void set_GPIO9()
{
    register unsigned int r;
	  
    // Put a 1 into the SET9 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 9);
    *GPSET0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO9
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 9
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO9()
{
    register unsigned int r;
	  
    // Put a 1 into the CLR9 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 9);
    *GPCLR0 = r;
}


////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO11_to_output
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 11 to an output pin without
//                  any pull-up or pull-down resistors.
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO11_to_output()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL1;

    // Clear bits 3 - 5. This is the field FSEL11, which maps to GPIO pin 11.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << 3);

    // Set the field FSEL11 to 001, which sets pin 9 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << 3);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 1
    *GPFSEL1 = r;

    
    // Disable the pull-up/pull-down control line for GPIO pin 11.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN0 register
    r = *GPPUPPDN0;

    // Zero out bits 22-23 in this bit pattern, since this maps to
    // GPIO pin 11. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 22);

    // Write the modified bit pattern back to the
    // GPPUPPDN0 register
    *GPPUPPDN0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       set_GPIO11
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets the GPIO output pin 11
//                  to a 1 (high) level.
//
////////////////////////////////////////////////////////////////////////////////

void set_GPIO11()
{
    register unsigned int r;
	  
    // Put a 1 into the SET11 field of the GPIO Pin Output Set Register 0
    r = (0x1 << 11);
    *GPSET0 = r;
}


////////////////////////////////////////////////////////////////////////////////
//
//  Function:       clear_GPIO11
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function clears the GPIO output pin 11
//                  to a 0 (low) level.
//
////////////////////////////////////////////////////////////////////////////////

void clear_GPIO11()
{
    register unsigned int r;
	  
    // Put a 1 into the CLR11 field of the GPIO Pin Output Clear Register 0
    r = (0x1 << 11);
    *GPCLR0 = r;
}


////////////////////////////////////////////////////////////////////////////////
//
//  Function:       init_GPIO10_to_input
//
//  Arguments:      none
//
//  Returns:        void
//
//  Description:    This function sets GPIO pin 10 to an input pin without
//                  any internal pull-up or pull-down resistors. Note that
//                  a pull-down (or pull-up) resistor must be used externally
//                  on the bread board circuit connected to the pin. Be sure
//                  that the pin high level is 3.3V (definitely NOT 5V).
//
////////////////////////////////////////////////////////////////////////////////

void init_GPIO10_to_input()
{
    register unsigned int r;
    
    
    // Get the current contents of the GPIO Function Select Register 1
    r = *GPFSEL1;

    // Clear bits 0 - 2. This is the field FSEL10, which maps to GPIO pin 10.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin.
    r &= ~(0x7 << 0);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 1
    *GPFSEL1 = r;


    // Disable the pull-up/pull-down control line for GPIO pin 10.
    // Enabling and disabling pullups/pulldowns has changed with the
    // Pi 4, as shown below:

    // Get the current bit pattern of the GPPUPPDN0 register
    r = *GPPUPPDN0;

    // Zero out bits 20-21 in this bit pattern, since this maps to
    // GPIO pin 10. The bit pattern 00 disables pullups/pulldowns.
    r &= ~(0x3 << 20);

    // Write the modified bit pattern back to the
    // GPPUPPDN0 register
    *GPPUPPDN0 = r;
}



////////////////////////////////////////////////////////////////////////////////
//
//  Function:       get_GPIO10
//
//  Arguments:      none
//
//  Returns:        1 if the pin level is high, and 0 if the pin level is low.
//
//  Description:    This function gets the current value of pin 10.
//
////////////////////////////////////////////////////////////////////////////////

unsigned int get_GPIO10()
{
    register unsigned int r;
	  
	  
    // Get the current contents of the GPIO Pin Level Register 0
    r = *GPLEV0;
	  
    // Isolate pin 10, and return its value (a 0 if low, or a 1 if high)
    return ((r >> 10) & 0x1);
}
