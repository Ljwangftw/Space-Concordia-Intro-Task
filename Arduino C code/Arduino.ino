#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// Define pin mappings to the Mega 2560 (adjust as necessary)
#define LATCH_DDR  DDRB
#define LATCH_PORT PORTB
#define LATCH_PIN  PB5 // Arduino Digital Pin 11

#define CLOCK_DDR  DDRB
#define CLOCK_PORT PORTB
#define CLOCK_PIN  PB4 // Arduino Digital Pin 10

#define DATA_DDR   DDRB
#define DATA_PORT  PORTB
#define DATA_PIN   PB6 // Arduino Digital Pin 12

#define DT 200 // Delay time in milliseconds

// Function to initialize the ports
void setup() {
    // Set the control pins as outputs
    LATCH_DDR |= (1 << LATCH_PIN);
    CLOCK_DDR |= (1 << CLOCK_PIN);
    DATA_DDR |= (1 << DATA_PIN);
    
    // Initialize USART for serial communication
    USART_init();
}

// Function to shift out a byte to the 74HC595 shift register
void shiftOut(uint8_t data) {
    for (int i = 0; i < 8; i++)  {
        // Clear the clock pin
        CLOCK_PORT &= ~(1 << CLOCK_PIN);
        
        // Set the data pin to the value of the current bit
        if (data & (1 << i)) {
            DATA_PORT |= (1 << DATA_PIN);
        } else {
            DATA_PORT &= ~(1 << DATA_PIN);
        }
        
        // Pulse the clock pin to write the bit
        CLOCK_PORT |= (1 << CLOCK_PIN);
        _delay_us(1); // Small delay to ensure the clock pulse is detected
    }
    
    // Clear the clock pin
    CLOCK_PORT &= ~(1 << CLOCK_PIN);
}

int main(void) {
    // Local variable to hold the LED states
    char buffer[9]; //store 8 bits + null terminator
    uint8_t LED1s = 0b00000000;

    // Setup the ports
    setup();

    while (1) {
        if (LED1s <= 0xFF) {
            // Lower the latch pin before shifting out data
            LATCH_PORT &= ~(1 << LATCH_PIN);
            
            // Shift out the data
            shiftOut(LED1s);
            
            // Latch the data onto the output pins
            LATCH_PORT |= (1 << LATCH_PIN);
            
            // Convert LED states to a binary string and send it if it's not 255
            if (LED1s != 0xFF) {
                itoa(LED1s, buffer, 2); // Convert LED1s to a binary string
                USART_sendString(buffer); // Send binary string
                USART_send('\n'); // Send newline char for better readability
            }
            
            // Increment the LED states
            if (LED1s < 0xFF) {
                LED1s++;
            } else {
                // Stop incrementing and break the loop
                break;
            }
            
            // Delay for the specified time
            _delay_ms(DT);
        }
    }

    // Send a final "11111111" and stop
    itoa(LED1s, buffer, 2); // Convert LED1s to a binary string
    USART_sendString(buffer); // Send binary string
    USART_send('\n'); // Send newline char for better readability
}

