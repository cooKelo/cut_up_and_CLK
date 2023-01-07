#include <msp430.h>
#include"setclock.h"
#include "oled\oled.h"
#include "keyboard.h"
#define my_Hz  18750
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    int m;
    m=DIVS__16;
    int key_value=0;

    OLED_Init();/*init OLED*/
    OLED_Clear(); /*clear OLED screen*/
    init_key();/*init keyboard*/
    OLED_ShowString(50, 3, "default");

SetClock_MCLK12MHZ_SMCLK12MHZ_ACLK32_768K();//SMCLK 12MHZ
UCSCTL5|=m;//ʹ��USCͳһʱ��ϵͳ����Ԥ��Ƶ����SMCLK����m��Ƶ
TA0EX0|=TAIDEX_4;////A0��������Ƶ��5��Ƶ
TA0CTL|= TASSEL_2+ID__8+MC_1+TACLR;//����A0��������4��Ƶ��ʱ��ԴSMCLK������ģʽ��ͬʱ���������
TA0CCTL0|=CCIE;//ʹ�ܶ�ʱ���жϣ�CCR0��Դ�жϣ�
TA0CCR0 = my_Hz;//����ȽϼĴ���0����ֵΪ18750
P3DIR |= BIT6;//����P3.6Ϊ���
P3OUT &= ~BIT6;//��ʼ��P3.6λ�͵�ƽ

__bis_SR_register(GIE);//ʹ��ȫ���ж�

while(1){
   key_value=key();

   if(key_value!=0){

    switch(key_value){
    case 1: {
        TA0CCR0=my_Hz;
        OLED_ShowChar(15,6,'1');
         break;
    }
    case 2: {
        TA0CCR0=my_Hz*2;
         OLED_ShowChar(15,6,'2');
         break;
    }
    default:{

        OLED_ShowString(50, 3, "default");
    }
     }
   }


}

}
#pragma vector = TIMER0_A0_VECTOR//TA0CCR0�жϷ�����
__interrupt void TIMER0_A0_ISR(void)
{
P3OUT ^= BIT6;//��תP4.7�����
}

