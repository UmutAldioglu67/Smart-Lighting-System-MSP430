#include <msp430.h>

const unsigned char font5x7[59][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x5F, 0x00, 0x00}, {0x00, 0x07, 0x00, 0x07, 0x00},
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, {0x24, 0x2A, 0x7F, 0x2A, 0x12}, {0x23, 0x13, 0x08, 0x64, 0x62},
    {0x36, 0x49, 0x55, 0x22, 0x50}, {0x00, 0x05, 0x03, 0x00, 0x00}, {0x00, 0x1C, 0x22, 0x41, 0x00},
    {0x00, 0x41, 0x22, 0x1C, 0x00}, {0x08, 0x2A, 0x1C, 0x2A, 0x08}, {0x08, 0x08, 0x3E, 0x08, 0x08},
    {0x00, 0x50, 0x30, 0x00, 0x00}, {0x08, 0x08, 0x08, 0x08, 0x08}, {0x00, 0x60, 0x60, 0x00, 0x00},
    {0x20, 0x10, 0x08, 0x04, 0x02}, {0x3E, 0x51, 0x49, 0x45, 0x3E}, {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x42, 0x61, 0x51, 0x49, 0x46}, {0x21, 0x41, 0x45, 0x4B, 0x31}, {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39}, {0x3C, 0x4A, 0x49, 0x49, 0x30}, {0x01, 0x71, 0x09, 0x05, 0x03},
    {0x36, 0x49, 0x49, 0x49, 0x36}, {0x06, 0x49, 0x49, 0x29, 0x1E}, {0x00, 0x36, 0x36, 0x00, 0x00},
    {0x00, 0x56, 0x36, 0x00, 0x00}, {0x00, 0x08, 0x14, 0x22, 0x41}, {0x14, 0x14, 0x14, 0x14, 0x14},
    {0x41, 0x22, 0x14, 0x08, 0x00}, {0x02, 0x01, 0x51, 0x09, 0x06}, {0x32, 0x49, 0x79, 0x41, 0x3E},
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, {0x7F, 0x49, 0x49, 0x49, 0x36}, {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, {0x7F, 0x49, 0x49, 0x49, 0x41}, {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x41, 0x51, 0x32}, {0x7F, 0x08, 0x08, 0x08, 0x7F}, {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01}, {0x7F, 0x08, 0x14, 0x22, 0x41}, {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x04, 0x02, 0x7F}, {0x7F, 0x04, 0x08, 0x10, 0x7F}, {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06}, {0x3E, 0x41, 0x51, 0x21, 0x5E}, {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x26, 0x49, 0x49, 0x49, 0x32}, {0x01, 0x01, 0x7F, 0x01, 0x01}, {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, {0x7F, 0x20, 0x18, 0x20, 0x7F}, {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x03, 0x04, 0x78, 0x04, 0x03}, {0x61, 0x51, 0x49, 0x45, 0x43} 
};

void init_MCU(void);
unsigned int read_ADC(void);
void uart_send_string(char *str);
void uart_send_number(unsigned int num);
void i2c_write(unsigned char mode, unsigned char data);
void oled_init(void);
void oled_set_cursor(unsigned char page, unsigned char col);
void oled_clear(void);
void oled_print_char(char c);
void oled_print_string(char *str);
void oled_print_number(unsigned int num);

volatile unsigned char motion_detected = 0; 
unsigned int ldr_value = 0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD; 

    if (CALBC1_1MHZ == 0xFF) while(1);
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    __delay_cycles(100000); 

    init_MCU();
    __enable_interrupt(); 

    oled_init();
    oled_clear();

    uart_send_string("\r\n--- AKILLI AYDINLATMA SISTEMI ---\r\n");

    while(1) {
        ldr_value = read_ADC();

        if (motion_detected) {
            if (ldr_value < 300) TA1CCR1 = 900;       // Karanlık + İnsan = Parlak
            else if (ldr_value < 700) TA1CCR1 = 300;  // Loş + İnsan = Kısık
            else TA1CCR1 = 0;                         // Aydınlık = Kapalı
        } else {
            TA1CCR1 = 0;                              // İnsan Yok = Kapalı
        }

        
        oled_set_cursor(0, 0); 
        oled_print_string("LDR DEGERI: "); 
        oled_print_number(ldr_value);
        
        uart_send_string("LDR DEGERI: "); 
        uart_send_number(ldr_value); 
        uart_send_string("\r\n");

        oled_set_cursor(2, 0); 
        oled_print_string("PIR DURUMU: ");
        uart_send_string("PIR DURUMU: ");
        
        if (motion_detected) {
            oled_print_string("1 (VAR) ");
            uart_send_string("1 (VAR)\r\n");
        } else {
            oled_print_string("0 (YOK) ");
            uart_send_string("0 (YOK)\r\n");
        }

        oled_set_cursor(4, 0); 
        oled_print_string("LED: ");
        uart_send_string("LED DURUMU: ");
        
        if (TA1CCR1 == 900) {
            oled_print_string("PARLAK YANIYOR");
            uart_send_string("PARLAK YANIYOR\r\n");
        } else if (TA1CCR1 == 300) {
            oled_print_string("KISIK YANIYOR ");
            uart_send_string("KISIK YANIYOR\r\n");
        } else {
            oled_print_string("KAPALI        ");
            uart_send_string("KAPALI\r\n");
        }

        // Terminal ekranını bölmek için ayraç
        uart_send_string("------------------------\r\n");

        __delay_cycles(500000); 
    }
}

void init_MCU(void) {
    ADC10CTL0 &= ~ENC;
    ADC10CTL1 = INCH_0 + SHS_0 + ADC10DIV_0 + ADC10SSEL_0 + CONSEQ_0; 
    ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10ON; 
    ADC10AE0 |= BIT0; 

    P2DIR &= ~BIT0; 
    P2REN |= BIT0;  
    P2OUT &= ~BIT0; 
    P2IE |= BIT0;   
    P2IES &= ~BIT0; 
    P2IFG &= ~BIT0; 

    P2DIR |= BIT1;  
    P2SEL |= BIT1;  
    TA1CCR0 = 1000 - 1;       
    TA1CCTL1 = OUTMOD_7;      
    TA1CCR1 = 0;              
    TA1CTL = TASSEL_2 + MC_1; 

    P1SEL |= BIT6 + BIT7;  
    P1SEL2 |= BIT6 + BIT7;
    UCB0CTL1 |= UCSWRST;   
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; 
    UCB0CTL1 = UCSSEL_2 + UCSWRST;        
    UCB0BR0 = 10;                         
    UCB0BR1 = 0;
    UCB0I2CSA = 0x3C; 
    UCB0CTL1 &= ~UCSWRST;  

    P1SEL |= BIT1 + BIT2;  
    P1SEL2 |= BIT1 + BIT2;
    UCA0CTL1 |= UCSSEL_2;  
    UCA0BR0 = 104;        
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;     
    UCA0CTL1 &= ~UCSWRST;  
}

unsigned int read_ADC(void) {
    ADC10CTL0 |= ENC + ADC10SC; 
    while (ADC10CTL1 & ADC10BUSY); 
    return ADC10MEM; 
}

void uart_send_string(char *str) {
    while (*str) {
        while (!(IFG2 & UCA0TXIFG)); 
        UCA0TXBUF = *str++;          
    }
}

void uart_send_number(unsigned int num) {
    char buf[6];
    int i = 4;
    buf[5] = '\0';
    if (num == 0) { 
        uart_send_string("0"); 
        return; 
    }
    while (num > 0) {
        buf[i] = (num % 10) + '0';
        num /= 10;
        i--;
    }
    uart_send_string(&buf[i + 1]);
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
    if (P2IFG & BIT0) { 
        if (P2IN & BIT0) {
            motion_detected = 1;
            P2IES |= BIT0; 
        } else {
            motion_detected = 0;
            P2IES &= ~BIT0; 
        }
        P2IFG &= ~BIT0; 
    }
}

void i2c_write(unsigned char mode, unsigned char data) {
    while (UCB0CTL1 & UCTXSTP); 
    UCB0CTL1 |= UCTR + UCTXSTT; 
    while(!(IFG2 & UCB0TXIFG)); 
    UCB0TXBUF = mode;           
    while(!(IFG2 & UCB0TXIFG));
    UCB0TXBUF = data;           
    while(!(IFG2 & UCB0TXIFG));
    UCB0CTL1 |= UCTXSTP;        
    IFG2 &= ~UCB0TXIFG;
}

void oled_init(void) {
    i2c_write(0x00, 0xAE); 
    i2c_write(0x00, 0x20); 
    i2c_write(0x00, 0x00); 
    i2c_write(0x00, 0x8D); 
    i2c_write(0x00, 0x14); 
    i2c_write(0x00, 0xAF); 
}

void oled_set_cursor(unsigned char page, unsigned char col) {
    i2c_write(0x00, 0xB0 + page);            
    i2c_write(0x00, 0x00 | (col & 0x0F));    
    i2c_write(0x00, 0x10 | (col >> 4));      
}

void oled_clear(void) {
    unsigned char page, col;
    for (page = 0; page < 8; page++) {
        oled_set_cursor(page, 0);
        for (col = 0; col < 128; col++) {
            i2c_write(0x40, 0x00); 
        }
    }
}

void oled_print_char(char c) {
    int i;
    if (c >= 32 && c <= 90) {
        for (i = 0; i < 5; i++) {
            i2c_write(0x40, font5x7[c - 32][i]); 
        }
    }
    i2c_write(0x40, 0x00); 
}

void oled_print_string(char *str) {
    while (*str) {
        oled_print_char(*str++);
    }
}

void oled_print_number(unsigned int num) {
    char buf[5];
    int i = 3;
    int j;
    buf[4] = '\0';
    if (num == 0) {
        oled_print_char('0');
        oled_print_string("   "); 
        return;
    }
    while (num > 0) {
        buf[i] = (num % 10) + '0';
        num /= 10;
        i--;
    }
    for (j = i + 1; j < 4; j++) {
        oled_print_char(buf[j]);
    }
    oled_print_string("   "); 
}