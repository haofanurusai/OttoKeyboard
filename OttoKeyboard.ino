#include "Note.h"

#define OCTAVE_MAX 10
#define OCTAVE_MIN 5

// 记录八度的变量，但是-1才是升一个八度，+1是降一个八度
uint8_t octave = 7;

// 主音，0为C，11为B
uint8_t root = 0;

// 音色
uint8_t waveform = 0;

// 键与音符下标绑定表
uint8_t keyBindTable[21];

// 给ABCDEFG键用的映射表
const static uint8_t keyRootTable[] = {9, 11, 0, 2, 4, 5, 7};
const static uint8_t keyWaveformTable[] = {0, 1, 2, 3, 4, 5, 6};

extern Note notes[NOTE_NUM + 1];

// 按键事件回调函数
void keyCallback(uint8_t key, uint8_t updn)
{
  if (updn)
  { // 弹起
    if (key <= 21)
    {
      notes[keyBindTable[key]].status = RELEASE;
      keyBindTable[key] = NOTE_NUM;
    }
  }
  else
  { // 按下
    uint8_t idx;
    switch (key)
    {
    case 'M':
    case '#':
      break;
    case '+':
      if (mKey())
      {
        if (octave > OCTAVE_MIN)
          --octave;
      }
      else
      {
        ++root;
        if (root > 11)
        {
          if (octave > OCTAVE_MIN)
          {
            root = 0;
            --octave;
          }
          else
            root = 11;
        }
      }
      break;
    case '-':
      if (mKey())
      {
        if (octave < OCTAVE_MAX)
          ++octave;
      }
      else
      {
        --root;
        // 注意：无符号整数
        if (root == 255)
        {
          if (octave < OCTAVE_MAX)
          {
            root = 11;
            ++octave;
          }
          else
            root = 0;
        }
      }
      break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
      if (mKey())
      {
        root = keyRootTable[key - 'A'] + risingKey();
        if (root > 11)
          root = 0;
      }
      else
      {
        waveform = keyWaveformTable[key - 'A'];
      }
      break;
    default:
      idx = findAvailableNote();
      if (idx >= NOTE_NUM)
        break;
      notes[idx].init(waveform, root + key + risingKey(), octave);
      keyBindTable[key] = idx;
      break;
    }
  }
}

void setup()
{
  memset(keyBindTable, NOTE_NUM, sizeof(keyBindTable));
  for (uint8_t i = 0; i <= NOTE_NUM; ++i)
  {
    notes[i].init();
  }
  inputInit();
  timerInit();
  for (;;)
  {
    inputScan(keyCallback);
  }
}

void loop()
{
}
