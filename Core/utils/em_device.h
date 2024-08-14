#ifndef _EM_DEVICE_H_ // 如果没有定义 _EM_DEVICE_H_
#define _EM_DEVICE_H_ // 定义 _EM_DEVICE_H_

#include "em_config.h" // 包含 em_config.h 头文件

typedef enum{ // 定义一个枚举类型 printer_state_e，表示打印机状态
    PRINTER_STATUS_INIT = 0,    // 打印机初始状态
    PRINTER_STATUS_START,       // 打印机开始状态
    PRINTER_STATUS_WORKING,     // 打印机工作状态
    PRINTER_STATUS_FINISH,      // 打印机完成状态
}printer_state_e;

typedef enum{ // 定义一个枚举类型 paper_state_e，表示纸张状态
    PAPER_STATUS_NORMAL = 0,    // 纸张正常状态
    PAPER_STATUS_LACK,          // 纸张缺乏状态
}paper_state_e;

typedef struct // 定义一个结构体 device_state_t，表示设备状态
{
    uint8_t battery;             // 电池电量
    uint8_t temperature;         // 设备温度
    paper_state_e paper_state;   // 纸张状态
    printer_state_e printer_state; // 打印机状态
    bool read_ble_finish;        // 蓝牙读取完成标志
}device_state_t;

void init_device_state(void); // 声明函数 init_device_state，用于初始化设备状态

device_state_t * get_device_state(void); // 声明函数 get_device_state，返回设备状态的指针

void set_read_ble_finish(bool finish); // 声明函数 set_read_ble_finish，设置蓝牙读取完成标志

void set_device_paper_status(paper_state_e status); // 声明函数 set_device_paper_status，设置纸张状态

#endif // 结束 _EM_DEVICE_H_ 宏定义
