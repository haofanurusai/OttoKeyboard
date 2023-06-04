// 音频输出

#include "Note.h"

// 初始化定时器以及声音输出端口
void timerInit(void)
{
  // 关中断
  cli();

  // 初始化D0-D7为音频输出端口，只需一次读写就可以输出
  // 8位的值，代价是无法使用串口
  // 如果你愿意，可以挑另外两个定时器Timer0、Timer2，
  // 用3,5,6脚输出高频PWM音频，但是音质受影响
  DDRD = 0xFF;
  PORTD = 0x80;

  // 禁用定时器Timer0
  TIMSK0 = 0;

  // 定时器Timer1设为比较模式
  TCCR1A = 0;              // 清除Timer1 control  A
  TCCR1B = 0;              // 清除Timer1 control  B
  TCNT1 = 0;               // 清除Timer1 counter

  OCR1A = 499;             // 设置定时器中断周期。值的计算：主频16MHz/采样率32KHz-1
  TCCR1B |= (1 << WGM12);  // 设置定时器1为CTC模式

  // 设置分频比为1
  TCCR1B |= (1 << CS10);

  // 启用定时器Timer1中断
  TIMSK1 |= (1 << OCIE1A);

  // 开中断
  sei();
}

// 定时器中断服务函数，在该函数中完成采样点的计算并输出
ISR(TIMER1_COMPA_vect) {
  int8_t mixed = 0;
  for (uint8_t i = 0; i < NOTE_NUM; ++i)
  {
    if (notes[i].status) {
      mixed += notes[i].sample();
    }
  }
  PORTD = 0x80 ^ (uint8_t)mixed;
}
