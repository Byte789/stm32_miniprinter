#include "em_motor.h"
#include "main.h"

// 定义一个静态函数用于设置指定引脚的电平状态
static void digitalWrite(int pin, int PinState) {
    if (pin == PIN_MOTOR_AP) {
        HAL_GPIO_WritePin(GPIOB, MOTOR_IN1_Pin, (GPIO_PinState)PinState);
    } else if (pin == PIN_MOTOR_AM) {
        HAL_GPIO_WritePin(GPIOB, MOTOR_IN2_Pin, (GPIO_PinState)PinState);
    } else if (pin == PIN_MOTOR_BP) {
        HAL_GPIO_WritePin(GPIOB, MOTOR_IN3_Pin, (GPIO_PinState)PinState);
    } else if (pin == PIN_MOTOR_BM) {
        HAL_GPIO_WritePin(GPIOB, MOTOR_IN4_Pin, (GPIO_PinState)PinState);
    }
}

// 电机位置
uint8_t motor_pos = 0;

// 电机步进表，定义电机每一步的引脚状态
uint8_t motor_table[8][4] =
    {
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {1, 0, 1, 0},
        {1, 0, 0, 0},
        {1, 0, 0, 1},
        {0, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 1, 0, 0}
    };

// 定义电机定时器句柄
osTimerId myMotorTimerHandle;

// 电机定时器回调函数，每次定时器触发时执行
void read_motor_timer_callbackfun(void const * argument) {
    // 设置电机引脚状态，控制电机运行
    digitalWrite(PIN_MOTOR_AP, motor_table[motor_pos][0]);
    digitalWrite(PIN_MOTOR_AM, motor_table[motor_pos][1]);
    digitalWrite(PIN_MOTOR_BP, motor_table[motor_pos][2]);
    digitalWrite(PIN_MOTOR_BM, motor_table[motor_pos][3]);
    
    // 更新电机位置
    motor_pos++;
    if (motor_pos >= 8) {
        motor_pos = 0; // 重置电机位置
    }
}

// 启动电机
void motor_start() {
    if (myMotorTimerHandle == NULL) {
        // 定义电机定时器
        osTimerDef(myMotorTimer, read_motor_timer_callbackfun);
        // 创建周期性定时器
        myMotorTimerHandle = osTimerCreate(osTimer(myMotorTimer), osTimerPeriodic, NULL);
    }
    // 启动定时器，每2ms触发一次回调函数
    osTimerStart(myMotorTimerHandle, 2); 
}

// 停止电机
void motor_stop() {
    // 将所有电机引脚状态设置为低电平，停止电机
    digitalWrite(PIN_MOTOR_AP, 0);
    digitalWrite(PIN_MOTOR_AM, 0);
    digitalWrite(PIN_MOTOR_BP, 0);
    digitalWrite(PIN_MOTOR_BM, 0);
    
    // 如果定时器句柄不为空，则停止定时器
    if (myMotorTimerHandle != NULL) {
        osTimerStop(myMotorTimerHandle);
    }
}

// 手动运行电机一步
void motor_run() {
    // 设置电机引脚状态，控制电机运行
    digitalWrite(PIN_MOTOR_AP, motor_table[motor_pos][0]);
    digitalWrite(PIN_MOTOR_AM, motor_table[motor_pos][1]);
    digitalWrite(PIN_MOTOR_BP, motor_table[motor_pos][2]);
    digitalWrite(PIN_MOTOR_BM, motor_table[motor_pos][3]);
    
    // 更新电机位置
    motor_pos++;
    if (motor_pos >= 8) {
        motor_pos = 0; // 重置电机位置
    }
}

// 运行电机指定步数
void motor_run_step(uint32_t steps) {
    while (steps) {
        // 设置电机引脚状态，控制电机运行
        digitalWrite(PIN_MOTOR_AP, motor_table[motor_pos][0]);
        digitalWrite(PIN_MOTOR_AM, motor_table[motor_pos][1]);
        digitalWrite(PIN_MOTOR_BP, motor_table[motor_pos][2]);
        digitalWrite(PIN_MOTOR_BM, motor_table[motor_pos][3]);
        
        // 更新电机位置
        motor_pos++;
        if (motor_pos >= 8) {
            motor_pos = 0; // 重置电机位置
        }
        us_delay(MOTOR_WATI_TIME); // 延迟等待一段时间
        steps--; // 递减步数
    }
}

// 初始化电机，将所有引脚状态设置为低电平
void init_motor() {
    digitalWrite(PIN_MOTOR_AP, 0);
    digitalWrite(PIN_MOTOR_AM, 0);
    digitalWrite(PIN_MOTOR_BP, 0);
    digitalWrite(PIN_MOTOR_BM, 0);
}
