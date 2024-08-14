#include "em_hal.h"    // 包含硬件抽象层的头文件
#include "em_adc.h"    // 包含ADC相关的头文件
#include "main.h"      // 包含主程序的头文件

int freq = 2000;    // 设置频率为2000kHz
int channel = 0;    // 通道号，取值范围0 ~ 15
int resolution = 8; // 分辨率，取值范围0~20，占空比duty最大取值为2^resolution-1

bool need_report = false; // 标志位，表示是否需要报告

#define EPISON 1e-7    // 定义一个非常小的数值，用于浮点数比较

// 读取指定引脚的数字输入
static int digitalRead(int pin){
    return HAL_GPIO_ReadPin(VHINT_GPIO_Port, VHINT_Pin); // 读取GPIO引脚状态
}

// 设置指定引脚的数字输出
static void digitalWrite(int pin, int PinState){
    HAL_GPIO_WritePin(GPIOB, LED_Pin, (GPIO_PinState)PinState); // 设置GPIO引脚状态
}

// 映射函数，将输入值从一个范围映射到另一个范围
static long map(long x, long in_min, long in_max, long out_min, long out_max) {
    const long dividend = out_max - out_min;
    const long divisor = in_max - in_min;
    const long delta = x - in_min;
    if (divisor == 0) {
        return -1; // 如果除数为0，则返回-1（AVR返回-1，SAM返回0）
    }
    return (delta * dividend + (divisor / 2)) / divisor + out_min; // 计算映射后的值
}

// 读取电池电量
void read_battery()
{
    get_device_state()->battery = map(get_adc_volts() * 2, 3300, 4200, 0, 100); // 将ADC电压值映射到0-100的范围
    if (get_device_state()->battery > 100)
        get_device_state()->battery = 100; // 限制最大电量为100
    printf("battery = %d\n", get_device_state()->battery); // 打印电量值
}

// 读取温度
void read_temperature()
{
    float temperatrue = get_adc_temperatrue(); // 读取ADC温度值
    printf("temperature = %f\n", temperatrue); // 打印温度值
    if (temperatrue >= EPISON) {
        get_device_state()->temperature = temperatrue; // 如果温度值有效，更新设备状态中的温度
    } else {
        get_device_state()->temperature = 0; // 如果温度值无效，设为0
    }
}

// 控制蜂鸣器，根据类型进行操作
void run_beep(beep_type_e type) {
    // 实现蜂鸣器控制的逻辑
}

// 控制LED，根据类型进行操作
void run_led(led_type_e type) {
    switch (type) {
        case LED_CONNECT:
            digitalWrite(PIN_LED, 0); // LED连接状态，熄灭LED
            break;
        case LED_DISCONNECT:
            digitalWrite(PIN_LED, 1); // LED断开状态，点亮LED
            break;
        case LED_WARN:
            digitalWrite(PIN_LED, 0);
            vTaskDelay(200);
            digitalWrite(PIN_LED, 1);
            vTaskDelay(200);
            digitalWrite(PIN_LED, 0);
            vTaskDelay(200);
            digitalWrite(PIN_LED, 1);
            vTaskDelay(200);
            digitalWrite(PIN_LED, 0);
            break;
        case LED_PRINTER_START:
            digitalWrite(PIN_LED, 1);
            vTaskDelay(200);
            digitalWrite(PIN_LED, 0);
            vTaskDelay(200);
            break;
        case LED_BLE_INIT:
            digitalWrite(PIN_LED, 0);
            vTaskDelay(10);
            digitalWrite(PIN_LED, 1);
            break;
        default:
            break;
    }
}

// GPIO外部中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin & GPIO_PIN_8) { // 检查引脚8是否触发中断
        printf("--");
        need_report = true; // 设置需要报告标志位
        set_device_paper_status(PAPER_STATUS_LACK); // 设置设备缺纸状态
    }
}

// 读取纸张状态
void read_paper_status()
{
    if (digitalRead(PIN_PAPER) == PAPER_STATUS_NORMAL) {
        get_device_state()->paper_state = PAPER_STATUS_NORMAL; // 如果纸张状态正常，更新设备状态
    } else {
        get_device_state()->paper_state = PAPER_STATUS_LACK; // 如果纸张状态缺纸，更新设备状态
    }
    printf("paper_state = %d\n", get_device_state()->paper_state); // 打印纸张状态
}

// 检查是否需要报告纸张中断状态
bool read_paper_irq_need_report_status()
{
    if (need_report) {
        need_report = false;
        return true; // 如果需要报告，返回true并清除标志位
    } else {
        return false; // 否则返回false
    }
}

// 读取所有硬件状态
void read_all_hal()
{
    read_battery();
    read_temperature();
    read_paper_status();
}

// 初始化硬件抽象层
void init_hal()
{
    // 缺纸IO初始化
    // pinMode(PIN_PAPER, INPUT);
    // attachInterrupt(PIN_PAPER, paper_isr, RISING);

    digitalWrite(PIN_LED, 1); // 初始化时点亮LED
}
