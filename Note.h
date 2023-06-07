#ifndef __NOTE_H__
#define __NOTE_H__

#include <avr/pgmspace.h>
#include "WaveTable.h"

// 音符状态
#define END     0
#define SUSTAIN 1
#define RELEASE 2

// 存放音高周期，每个元素是前一个元素的2^(1/12)倍（十二平均律），长度必须大于34，最末的元素离65535越近，精度越高
const static uint16_t periodTable[] =
{
  9397, 9956, 10548, 11175, 11840, 12544, 13290, 14080, 14917, 15804, 16744, 17740, 18795, 19912, 21096, 22351, 23680, 25088, 26580, 28160, 29834, 31609, 33488, 35479, 37589, 39824, 42192, 44701, 47359, 50175, 53159, 56320, 59669, 63217
};

// 自定义联合体，用于便捷访问16位数的高位和低位
// 注意大小端，基于AVR的Arduino是小端
typedef union {
  uint16_t _16;
  uint8_t _8[2];
} u16_8;

// 音符类
class Note {
  private:
    u16_8    counter;
    uint16_t cursor;
    uint16_t period;
    uint16_t sustainStart;
    uint16_t sustainEnd;
    uint16_t noteEnd;
    uint8_t  sustainStartBuffer;

  public:
    uint8_t  status;
    void init(void);
    void init(uint8_t, uint8_t, uint8_t);
    int8_t sample(void);
};

// 音符数，即复音数
#define NOTE_NUM 5
// 多一个作为监视哨
Note notes[NOTE_NUM + 1];

// 此函数返回一个空闲音符的下标
uint8_t findAvailableNote(void);

#endif
