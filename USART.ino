#define F_CPU 16000000UL // 16 MHz CPU frequency
#define BAUD 9600         // Baud rate for serial communication
#define BRC ((F_CPU/(16*BAUD) - 1)) // Baud rate calculator (BRC = 103, which in binary is 0000 0000 0110 0111)

// Function to initialize USART
void USART_init(void) {
    UBRR0H = (BRC >> 8);    // Set baud rate high byte, which is 0000 0000
    UBRR0L = BRC;           // Set baud rate low byte, which is 0110 0111

    UCSR0B = (1 << TXEN0);  // Enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame: 8 data bits, 1 stop bit, no parity
}

// Function to send a character through USART
void USART_send(char c) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer is empty
    UDR0 = c;                         // Send the character
}

// Function to send a string through USART
void USART_sendString(char* str) { //char pointer that points to first char and increment while there is a next char
    while (*str) {
        USART_send(*str++);
    }
}
