#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// LCD Commands
#define LCD_CLEAR 0x01
#define LCD_4BIT_MODE 0x28
#define LCD_DISPLAY_ON 0x0C
#define LCD_CURSOR_OFF 0x0C
#define LCD_SET_CURSOR 0x80

// Define LCD control pins
#define RS PD0
#define E  PD1

// Function Prototypes
void lcd_init();
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_set_cursor(uint8_t row, uint8_t column);
void lcd_display_time_and_date(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year);
void timer1_init();

volatile uint8_t seconds = 9, minutes = 20, hours = 11;
volatile uint8_t day = 3, month = 11;
volatile uint16_t year = 2024;

int main(void) {
	DDRD = 0xFF; // LCD control pins as output
	DDRC = 0xF0; // LCD data pins as output (4-bit mode)

	lcd_init();  // Initialize LCD
	timer1_init();  // Initialize Timer1 for 1-second intervals
	
	sei();  // Enable global interrupts

	while (1) {
		// Display the current time and date on the LCD
		lcd_display_time_and_date(hours, minutes, seconds, day, month, year);
		_delay_ms(1000);  // Update every 1 second
	}
}

// Timer1 Initialization for 1 second interrupt
void timer1_init() {
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC mode, 1024 prescaler
	OCR1A = 15624;  // Compare value for 1 second (16MHz clock)
	TIMSK |= (1 << OCIE1A);  // Enable Timer1 compare interrupt
}

// Timer1 Compare A Interrupt Service Routine
ISR(TIMER1_COMPA_vect) {
	seconds++;
	if (seconds >= 60) {
		seconds = 0;
		minutes++;
	}
	if (minutes >= 60) {
		minutes = 0;
		hours++;
	}
	if (hours >= 24) {
		hours = 0;
		day++;

		// Handle month and year roll-over
		if ((day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)) ||
		(day > 31 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) ||
		(day > 28 && month == 2 && (year % 4 != 0)) ||
		(day > 29 && month == 2 && (year % 4 == 0))) {
			day = 1;
			month++;
			if (month > 12) {
				month = 1;
				year++;
			}
		}
	}
}

// LCD Initialization
void lcd_init() {
	lcd_command(0x02); // Initialize LCD in 4-bit mode
	lcd_command(LCD_4BIT_MODE);
	lcd_command(LCD_CLEAR);
	lcd_command(LCD_DISPLAY_ON);
	lcd_command(LCD_CURSOR_OFF);
}

// Send command to LCD
void lcd_command(unsigned char cmd) {
	PORTC = (PORTC & 0x0F) | (cmd & 0xF0);  // Send higher nibble
	PORTD &= ~(1 << RS);  // RS = 0 for command
	PORTD |= (1 << E);    // E = 1
	_delay_us(1);
	PORTD &= ~(1 << E);   // E = 0
	_delay_us(200);
	
	PORTC = (PORTC & 0x0F) | (cmd << 4);    // Send lower nibble
	PORTD |= (1 << E);    // E = 1
	_delay_us(1);
	PORTD &= ~(1 << E);   // E = 0
	_delay_ms(2);
}

// Send data to LCD
void lcd_data(unsigned char data) {
	PORTC = (PORTC & 0x0F) | (data & 0xF0); // Send higher nibble
	PORTD |= (1 << RS);    // RS = 1 for data
	PORTD |= (1 << E);     // E = 1
	_delay_us(1);
	PORTD &= ~(1 << E);    // E = 0
	_delay_us(200);
	
	PORTC = (PORTC & 0x0F) | (data << 4);   // Send lower nibble
	PORTD |= (1 << E);     // E = 1
	_delay_us(1);
	PORTD &= ~(1 << E);    // E = 0
	_delay_ms(2);
}

// Set cursor position
void lcd_set_cursor(uint8_t row, uint8_t column) {
	if (row == 0) {
		lcd_command(LCD_SET_CURSOR | (0x00 + column));
		} else if (row == 1) {
		lcd_command(LCD_SET_CURSOR | (0x40 + column));
	}
}

// Display time and date on LCD
void lcd_display_time_and_date(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t day, uint8_t month, uint16_t year) {
	lcd_command(LCD_CLEAR);  // Clear the display
	_delay_ms(2);            // Wait for the clear command to process

	// Display the current time in HH:MM:SS format
	lcd_set_cursor(0, 0);  // First row
	lcd_data('0' + (hours / 10));
	lcd_data('0' + (hours % 10));
	lcd_data(':');
	lcd_data('0' + (minutes / 10));
	lcd_data('0' + (minutes % 10));
	lcd_data(':');
	lcd_data('0' + (seconds / 10));
	lcd_data('0' + (seconds % 10));

	// Display the current date in DD/MM/YYYY format
	lcd_set_cursor(1, 0);  // Second row
	lcd_data('0' + (day / 10));
	lcd_data('0' + (day % 10));
	lcd_data('/');
	lcd_data('0' + (month / 10));
	lcd_data('0' + (month % 10));
	lcd_data('/');
	lcd_data('0' + ((year / 1000) % 10));
	lcd_data('0' + ((year / 100) % 10));
	lcd_data('0' + ((year / 10) % 10));
	lcd_data('0' + (year % 10));
}
