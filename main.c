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
UCSCTL5|=m;//使用USC统一时钟系统进行预分频，将SMCLK进行m分频
TA0EX0|=TAIDEX_4;////A0计数器分频，5分频
TA0CTL|= TASSEL_2+ID__8+MC_1+TACLR;//配置A0计数器，4分频，时钟源SMCLK，上升模式，同时清除计数器
TA0CCTL0|=CCIE;//使能定时器中断（CCR0单源中断）
TA0CCR0 = my_Hz;//捕获比较寄存器0设置值为18750
P3DIR |= BIT6;//设置P3.6为输出
P3OUT &= ~BIT6;//初始化P3.6位低电平

__bis_SR_register(GIE);//使能全局中断

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
#pragma vector = TIMER0_A0_VECTOR//TA0CCR0中断服务函数
__interrupt void TIMER0_A0_ISR(void)
{
P3OUT ^= BIT6;//翻转P4.7的输出
}

