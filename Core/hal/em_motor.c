#include "em_motor.h"
#include "main.h"

// ����һ����̬������������ָ�����ŵĵ�ƽ״̬
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

// ���λ��
uint8_t motor_pos = 0;

// ���������������ÿһ��������״̬
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

// ��������ʱ�����
osTimerId myMotorTimerHandle;

// �����ʱ���ص�������ÿ�ζ�ʱ������ʱִ��
void read_motor_timer_callbackfun(void const * argument) {
    // ���õ������״̬�����Ƶ������
    digitalWrite(PIN_MOTOR_AP, motor_table[motor_pos][0]);
    digitalWrite(PIN_MOTOR_AM, motor_table[motor_pos][1]);
    digitalWrite(PIN_MOTOR_BP, motor_table[motor_pos][2]);
    digitalWrite(PIN_MOTOR_BM, motor_table[motor_pos][3]);
    
    // ���µ��λ��
    motor_pos++;
    if (motor_pos >= 8) {
        motor_pos = 0; // ���õ��λ��
    }
}

// �������
void motor_start() {
    if (myMotorTimerHandle == NULL) {
        // ��������ʱ��
        osTimerDef(myMotorTimer, read_motor_timer_callbackfun);
        // ���������Զ�ʱ��
        myMotorTimerHandle = osTimerCreate(osTimer(myMotorTimer), osTimerPeriodic, NULL);
    }
    // ������ʱ����ÿ2ms����һ�λص�����
    osTimerStart(myMotorTimerHandle, 2); 
}

// ֹͣ���
void motor_stop() {
    // �����е������״̬����Ϊ�͵�ƽ��ֹͣ���
    digitalWrite(PIN_MOTOR_AP, 0);
    digitalWrite(PIN_MOTOR_AM, 0);
    digitalWrite(PIN_MOTOR_BP, 0);
    digitalWrite(PIN_MOTOR_BM, 0);
    
    // �����ʱ�������Ϊ�գ���ֹͣ��ʱ��
    if (myMotorTimerHandle != NULL) {
        osTimerStop(myMotorTimerHandle);
    }
}

// �ֶ����е��һ��
void motor_run() {
    // ���õ������״̬�����Ƶ������
    digitalWrite(PIN_MOTOR_AP, motor_table[motor_pos][0]);
    digitalWrite(PIN_MOTOR_AM, motor_table[motor_pos][1]);
    digitalWrite(PIN_MOTOR_BP, motor_table[motor_pos][2]);
    digitalWrite(PIN_MOTOR_BM, motor_table[motor_pos][3]);
    
    // ���µ��λ��
    motor_pos++;
    if (motor_pos >= 8) {
        motor_pos = 0; // ���õ��λ��
    }
}

// ���е��ָ������
void motor_run_step(uint32_t steps) {
    while (steps) {
        // ���õ������״̬�����Ƶ������
        digitalWrite(PIN_MOTOR_AP, motor_table[motor_pos][0]);
        digitalWrite(PIN_MOTOR_AM, motor_table[motor_pos][1]);
        digitalWrite(PIN_MOTOR_BP, motor_table[motor_pos][2]);
        digitalWrite(PIN_MOTOR_BM, motor_table[motor_pos][3]);
        
        // ���µ��λ��
        motor_pos++;
        if (motor_pos >= 8) {
            motor_pos = 0; // ���õ��λ��
        }
        us_delay(MOTOR_WATI_TIME); // �ӳٵȴ�һ��ʱ��
        steps--; // �ݼ�����
    }
}

// ��ʼ�����������������״̬����Ϊ�͵�ƽ
void init_motor() {
    digitalWrite(PIN_MOTOR_AP, 0);
    digitalWrite(PIN_MOTOR_AM, 0);
    digitalWrite(PIN_MOTOR_BP, 0);
    digitalWrite(PIN_MOTOR_BM, 0);
}
