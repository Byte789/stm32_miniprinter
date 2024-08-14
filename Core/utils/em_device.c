#include "em_device.h" // 包含头文件 em_device.h，该文件应包含 device_state_t 结构体和相关定义

device_state_t g_device_state; // 声明一个全局变量 g_device_state，类型为 device_state_t

device_state_t * get_device_state(){
    return &g_device_state; // 返回指向全局变量 g_device_state 的指针
}

void set_device_paper_status(paper_state_e status){
    g_device_state.paper_state = status; // 设置 g_device_state 结构体中的 paper_state 字段为传入的 status 值
}

void set_read_ble_finish(bool finish){
    g_device_state.read_ble_finish = finish; // 设置 g_device_state 结构体中的 read_ble_finish 字段为传入的 finish 值
}

void init_device_state(){
    memset(&g_device_state, 0, sizeof(device_state_t)); // 使用 memset 函数将 g_device_state 结构体的所有字节设置为 0
    g_device_state.printer_state = PRINTER_STATUS_INIT; // 将 printer_state 字段设置为 PRINTER_STATUS_INIT
    g_device_state.battery = 100;                       // 将 battery 字段设置为 100（表示电池电量为 100%）
    g_device_state.temperature = 30;                    // 将 temperature 字段设置为 30（表示设备温度为 30 度）
    g_device_state.paper_state = PAPER_STATUS_NORMAL;   // 将 paper_state 字段设置为 PAPER_STATUS_NORMAL
    g_device_state.read_ble_finish = false;             // 将 read_ble_finish 字段设置为 false（表示未完成蓝牙读取）
}
