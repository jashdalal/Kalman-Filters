#include <msp430.h> 
#include <math.h>
#define COUNT 5
// 10-bit ADC conversion result array
volatile float results[10], j=0,i=0;
#define COUNT 5

void readADC();
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P1SEL |= BIT6;                            // Set P1.6 to output direction (Timer D0.0 output)
    P1DIR |= BIT6;
    P1SEL |= BIT7;                            // Set P1.7 to output direction (Timer D0.1 output)
    P1DIR |= BIT7;
    P2SEL |= BIT0;                            // Set P2.0 to output direction (Timer D0.2 output)
     P2DIR |= BIT0;
    P1DIR |= 0x01;                            // Set P1.0 to output direction (to drive LED)
    P1OUT |= 0x01;                            // Set P1.0  - turn LED on
   // ADC pins
    P6SEL |= BIT0;
    P6SEL |= BIT1;
    P6SEL |= BIT2;
    P6SEL |= BIT3;
    P6SEL |= BIT4;
    P6SEL |= BIT5;
    //P6DIR |= 0x00;
    // Timer output
    P3SEL |= BIT6;
    P3DIR |= BIT6;

      ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_0;                                         // Turn on ADC12, set sampling time
      ADC12CTL1 = ADC12SHP+ADC12CONSEQ_1 + ADC12SSEL_0;                                        // Use sampling timer, single sequence
      ADC12CTL2 = ADC12RES_2;
      ADC12MCTL0 = ADC12INCH_0;                 // ref+=AVcc, channel = A0
      ADC12MCTL1 = ADC12INCH_1;                 // ref+=AVcc, channel = A1
      ADC12MCTL2 = ADC12INCH_2;
      ADC12MCTL3 = ADC12INCH_3;
      ADC12MCTL4 = ADC12INCH_4;
      ADC12MCTL5 = ADC12INCH_5 + ADC12EOS;
      ADC12IE = 0x08;                           // Enable ADC12IFG.3
      ADC12CTL0 |= ADC12ENC;                    // Enable conversions



    for(i=0;i<COUNT;i++)
    {
        ADC12CTL0 |= ADC12SC;    // Start conversion - software trigger
         __delay_cycles(10000);      // crude delay for sampling and conversion

        readADC();
        readADC();
        readADC();
    }
	return 0;
}
void readADC()
{
     ADC12CTL0 |= ADC12SC;    // Start conversion - software trigger

     __delay_cycles(10000);      // crude delay for sampling and conversion

            results[0] = ADC12MEM0*0.00081/0.0215;                 // Move results, IFG is cleared
            results[1] = ADC12MEM1*0.00081/0.1814;                 // Move results, IFG is cleared
            results[2] = ADC12MEM2*0.00081;
            results[3] = ADC12MEM3*0.00081;
            results[4] = ADC12MEM4*0.00081;
            results[5] = ADC12MEM5*0.00081;
    //At this point, the results are contained in the results array, so check there
}
