# OttoKeyboard
使用Arduino制作的音乐（？）键盘。Related video: bilibili.com/BV1R8411o7kC

## 电路设计
假设您使用Arduino Uno/Nano/Pro Mini等基于ATmega168p/328p的板子，或者最小系统：
- D0-D7接入一个R-2R电阻网络DAC，进行音频输出（D7为最高位，D0为最低位）。后级可以根据需要选择是否接功放。
- D8-D13接矩阵键盘的输出端，A0-A3接矩阵键盘的输入端。键盘细节如图：![](https://github.com/haofanurusai/OttoKeyboard/blob/main/keyboard_design_and_note.png?raw=true)
- A4和A5为I2C预留。

## 采样的制作
每段采样为8位有符号整数数组，存在Flash中。
> TODO: 将下述流程自动化
1. 用Audition或其它工具将备好的采样转为16000Hz、8位、单声道的WAV文件（若使用Audition，请勿勾选保存元数据）
2. 定好循环区段的头尾后，将上述WAV文件另存两个副本，其中一个截到循环的开头为止，另外一个截到循环的结尾为止
3. 用二进制编辑器将3份WAV的文件头去除。关于WAV的文件头相关细节，请自行搜索
4. 记录下两个WAV副本的大小（多少字节）。之后它们的使命就结束了，可以删除了
5. 再写一小段程序，将剩下的那一个WAV，每个字节与0x80进行异或后，转为C语言数组格式进行输出，将输出内容贴到WaveTable.h中使用即可。
6. 步骤4中记录的WAV大小对应循环节起始地址偏移和结束地址偏移。在WaveTable.h中对应地进行修改即可

## 注意事项
- 由于AVR系列单片机性能孱弱，为最大程度提高性能，代码中多处直接对寄存器进行访问，因此代码只能适用于基于ATmega168p/328p单片机的板子。如果需要移植到其它单片机，请按对应的数据手册修改代码。
- 经实测，16MHz主频下，~~5复音会拖慢代码，导致跑调。可考虑提升时钟频率至20MHz，或修改定时器相关代码降低采样率。~~经过优化判定树，并减少不必要的访存后，已经彻底修复。

## English translations are welcomed
