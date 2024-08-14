#ifndef _EM_DEVICE_H_ // ���û�ж��� _EM_DEVICE_H_
#define _EM_DEVICE_H_ // ���� _EM_DEVICE_H_

#include "em_config.h" // ���� em_config.h ͷ�ļ�

typedef enum{ // ����һ��ö������ printer_state_e����ʾ��ӡ��״̬
    PRINTER_STATUS_INIT = 0,    // ��ӡ����ʼ״̬
    PRINTER_STATUS_START,       // ��ӡ����ʼ״̬
    PRINTER_STATUS_WORKING,     // ��ӡ������״̬
    PRINTER_STATUS_FINISH,      // ��ӡ�����״̬
}printer_state_e;

typedef enum{ // ����һ��ö������ paper_state_e����ʾֽ��״̬
    PAPER_STATUS_NORMAL = 0,    // ֽ������״̬
    PAPER_STATUS_LACK,          // ֽ��ȱ��״̬
}paper_state_e;

typedef struct // ����һ���ṹ�� device_state_t����ʾ�豸״̬
{
    uint8_t battery;             // ��ص���
    uint8_t temperature;         // �豸�¶�
    paper_state_e paper_state;   // ֽ��״̬
    printer_state_e printer_state; // ��ӡ��״̬
    bool read_ble_finish;        // ������ȡ��ɱ�־
}device_state_t;

void init_device_state(void); // �������� init_device_state�����ڳ�ʼ���豸״̬

device_state_t * get_device_state(void); // �������� get_device_state�������豸״̬��ָ��

void set_read_ble_finish(bool finish); // �������� set_read_ble_finish������������ȡ��ɱ�־

void set_device_paper_status(paper_state_e status); // �������� set_device_paper_status������ֽ��״̬

#endif // ���� _EM_DEVICE_H_ �궨��
