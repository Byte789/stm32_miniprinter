#include "em_hal.h"    // ����Ӳ��������ͷ�ļ�
#include "em_adc.h"    // ����ADC��ص�ͷ�ļ�
#include "main.h"      // �����������ͷ�ļ�

int freq = 2000;    // ����Ƶ��Ϊ2000kHz
int channel = 0;    // ͨ���ţ�ȡֵ��Χ0 ~ 15
int resolution = 8; // �ֱ��ʣ�ȡֵ��Χ0~20��ռ�ձ�duty���ȡֵΪ2^resolution-1

bool need_report = false; // ��־λ����ʾ�Ƿ���Ҫ����

#define EPISON 1e-7    // ����һ���ǳ�С����ֵ�����ڸ������Ƚ�

// ��ȡָ�����ŵ���������
static int digitalRead(int pin){
    return HAL_GPIO_ReadPin(VHINT_GPIO_Port, VHINT_Pin); // ��ȡGPIO����״̬
}

// ����ָ�����ŵ��������
static void digitalWrite(int pin, int PinState){
    HAL_GPIO_WritePin(GPIOB, LED_Pin, (GPIO_PinState)PinState); // ����GPIO����״̬
}

// ӳ�亯����������ֵ��һ����Χӳ�䵽��һ����Χ
static long map(long x, long in_min, long in_max, long out_min, long out_max) {
    const long dividend = out_max - out_min;
    const long divisor = in_max - in_min;
    const long delta = x - in_min;
    if (divisor == 0) {
        return -1; // �������Ϊ0���򷵻�-1��AVR����-1��SAM����0��
    }
    return (delta * dividend + (divisor / 2)) / divisor + out_min; // ����ӳ����ֵ
}

// ��ȡ��ص���
void read_battery()
{
    get_device_state()->battery = map(get_adc_volts() * 2, 3300, 4200, 0, 100); // ��ADC��ѹֵӳ�䵽0-100�ķ�Χ
    if (get_device_state()->battery > 100)
        get_device_state()->battery = 100; // ����������Ϊ100
    printf("battery = %d\n", get_device_state()->battery); // ��ӡ����ֵ
}

// ��ȡ�¶�
void read_temperature()
{
    float temperatrue = get_adc_temperatrue(); // ��ȡADC�¶�ֵ
    printf("temperature = %f\n", temperatrue); // ��ӡ�¶�ֵ
    if (temperatrue >= EPISON) {
        get_device_state()->temperature = temperatrue; // ����¶�ֵ��Ч�������豸״̬�е��¶�
    } else {
        get_device_state()->temperature = 0; // ����¶�ֵ��Ч����Ϊ0
    }
}

// ���Ʒ��������������ͽ��в���
void run_beep(beep_type_e type) {
    // ʵ�ַ��������Ƶ��߼�
}

// ����LED���������ͽ��в���
void run_led(led_type_e type) {
    switch (type) {
        case LED_CONNECT:
            digitalWrite(PIN_LED, 0); // LED����״̬��Ϩ��LED
            break;
        case LED_DISCONNECT:
            digitalWrite(PIN_LED, 1); // LED�Ͽ�״̬������LED
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

// GPIO�ⲿ�жϻص�����
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin & GPIO_PIN_8) { // �������8�Ƿ񴥷��ж�
        printf("--");
        need_report = true; // ������Ҫ�����־λ
        set_device_paper_status(PAPER_STATUS_LACK); // �����豸ȱֽ״̬
    }
}

// ��ȡֽ��״̬
void read_paper_status()
{
    if (digitalRead(PIN_PAPER) == PAPER_STATUS_NORMAL) {
        get_device_state()->paper_state = PAPER_STATUS_NORMAL; // ���ֽ��״̬�����������豸״̬
    } else {
        get_device_state()->paper_state = PAPER_STATUS_LACK; // ���ֽ��״̬ȱֽ�������豸״̬
    }
    printf("paper_state = %d\n", get_device_state()->paper_state); // ��ӡֽ��״̬
}

// ����Ƿ���Ҫ����ֽ���ж�״̬
bool read_paper_irq_need_report_status()
{
    if (need_report) {
        need_report = false;
        return true; // �����Ҫ���棬����true�������־λ
    } else {
        return false; // ���򷵻�false
    }
}

// ��ȡ����Ӳ��״̬
void read_all_hal()
{
    read_battery();
    read_temperature();
    read_paper_status();
}

// ��ʼ��Ӳ�������
void init_hal()
{
    // ȱֽIO��ʼ��
    // pinMode(PIN_PAPER, INPUT);
    // attachInterrupt(PIN_PAPER, paper_isr, RISING);

    digitalWrite(PIN_LED, 1); // ��ʼ��ʱ����LED
}
