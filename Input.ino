#define NOP                      \
  do                             \
  {                              \
    __asm__ __volatile__("nop"); \
  } while (0)

// 矩阵键盘扫描

// 存放旧的按键状态
uint8_t oldKeyStatusGlobal[] = {0xFF, 0xFF, 0xFF, 0xFF};

// 按键与音符/功能键映射表
const static uint8_t keyNoteTable[4][6] = {
    {'C', 'D', 16, 17, 19, 21},
    {'E', 'F', 10, 12, 14, 'M'},
    {'G', 'A', 5, 7, 9, '+'},
    {'#', 'B', 0, 2, 4, '-'},
};

// 初始化矩阵键盘IO
void inputInit(void)
{
  // D8-D13设置为输入，并启用内部上拉
  DDRB &= 0xC0;
  PORTB |= 0x3F;

  // A0-A3设置为输出
  DDRC &= 0xF0;
  PORTC &= 0xF0;
}

// 扫描矩阵键盘
// keyCallback为按键触发时的回调函数，其第一参数为按键所对应的
// keyNoteTable中的值；第二参数指示按下（=0）/弹起（非0）
void inputScan(void (*keyCallback)(uint8_t, uint8_t))
{
  uint8_t i = 0;
  uint8_t i_mask;
  uint8_t j;
  uint8_t j_mask;
  uint8_t newKeyStatus;
  uint8_t oldKeyStatus;
  uint8_t con;
  for (i_mask = 1; i < 4; ++i, i_mask <<= 1)
  {
    DDRC = i_mask;
    oldKeyStatus = oldKeyStatusGlobal[i];

    // 因为把定时器Timer0关了，所以延时函数和计时函数都作废
    // 用NOP指令产生微小延时
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;
    NOP;

    // 读入D8-D13引脚状态，并更新状态
    newKeyStatus = PINB;
    oldKeyStatusGlobal[i] = newKeyStatus;

    for (j = 0, j_mask = 1; j < 6; ++j, j_mask <<= 1)
    {
      con = (j_mask & oldKeyStatus) ? 0 : 2;
      con |= (j_mask & newKeyStatus) ? 1 : 0;
      switch (con)
      {
      case 0:
      case 3:
        keyCallback(keyNoteTable[i][j], con);
        break;
      default:
        break;
      }
    }
  }
}

// 此函数返回升调键状态，按下为1，未按下为0
inline uint8_t risingKey(void)
{
  return 1 - (oldKeyStatusGlobal[3] & 1);
}

// 此函数返回M键状态，为了提高性能，没有取反，按下为0，未按下为非0
inline uint8_t mKey(void)
{
  return oldKeyStatusGlobal[1] & 0x20;
}
