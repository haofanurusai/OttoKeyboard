#ifndef __NOTE_H__
#define __NOTE_H__

#include <avr/pgmspace.h>
#include "WaveTable.h"

// 音符状态
#define END     0
#define SUSTAIN 1
#define RELEASE 2

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
    int8_t   sustainStartBuffer;

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
