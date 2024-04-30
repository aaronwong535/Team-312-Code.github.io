// Team 312 main.c

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c2_master_example.h"
#include "mcc_generated_files/i2c2_master.h"
#include "mcc_generated_files/spi1.h"
#include <stdio.h>
#define I2C_Slave_address 0x36
#define Angled_Address_1 0x0E
#define Angled_Address_2 0x0F
#define Maxvalue 255

uint8_t rxdata2,rxdata1,mdata;
uint8_t fwd = 0b11101111;
uint8_t bwd = 0b11101101;
void EUSART2TX(){
    EUSART2_Receive_ISR();
    if (EUSART2_is_rx_ready()){
        rxdata2=EUSART2_Read();
        if (EUSART1_is_tx_ready()){
            EUSART1_Write(rxdata2);
        }
    }
    //LED_0_Toggle();
}

void EUSART1TX(){
    EUSART1_Receive_ISR();
    if (EUSART1_is_rx_ready()){
        rxdata1=EUSART1_Read();
        if (EUSART2_is_tx_ready()){
            EUSART2_Write(rxdata1);
        }
    }
    //LED_0_Toggle();
}
/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    EUSART1_Initialize();
    EUSART2_Initialize();
    INTERRUPT_Initialize ();
    I2C2_Initialize();
    PIN_MANAGER_Initialize();
    SPI1_Initialize();
    SPI1_Open(SPI1_DEFAULT);
    uint8_t b1,b2;
    uint16_t raw;
    int i;
    float angle;
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
        b1= I2C2_Read1ByteRegister(I2C_Slave_address, Angled_Address_1);
        __delay_ms(50);
        b2= I2C2_Read1ByteRegister(I2C_Slave_address, Angled_Address_2);
        __delay_ms(50);

        raw = (b1<<8) | b2;
        angle = (raw / Maxvalue) * 360.0;
        printf("Angle: %.2f\n\r", angle);
        __delay_ms(1000);
       
        CSN1_SetLow();
        CSN2_SetLow();
        mdata = SPI1_ExchangeByte(fwd);
        CSN1_SetHigh();
        CSN2_SetHigh();
        __delay_ms(5000);
        CSN1_SetLow();
        CSN2_SetLow();  
        __delay_ms(5000);
        // Add your application code
    }
}   
