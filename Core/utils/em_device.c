#include "em_device.h" // ����ͷ�ļ� em_device.h�����ļ�Ӧ���� device_state_t �ṹ�����ض���

device_state_t g_device_state; // ����һ��ȫ�ֱ��� g_device_state������Ϊ device_state_t

device_state_t * get_device_state(){
    return &g_device_state; // ����ָ��ȫ�ֱ��� g_device_state ��ָ��
}

void set_device_paper_status(paper_state_e status){
    g_device_state.paper_state = status; // ���� g_device_state �ṹ���е� paper_state �ֶ�Ϊ����� status ֵ
}

void set_read_ble_finish(bool finish){
    g_device_state.read_ble_finish = finish; // ���� g_device_state �ṹ���е� read_ble_finish �ֶ�Ϊ����� finish ֵ
}

void init_device_state(){
    memset(&g_device_state, 0, sizeof(device_state_t)); // ʹ�� memset ������ g_device_state �ṹ��������ֽ�����Ϊ 0
    g_device_state.printer_state = PRINTER_STATUS_INIT; // �� printer_state �ֶ�����Ϊ PRINTER_STATUS_INIT
    g_device_state.battery = 100;                       // �� battery �ֶ�����Ϊ 100����ʾ��ص���Ϊ 100%��
    g_device_state.temperature = 30;                    // �� temperature �ֶ�����Ϊ 30����ʾ�豸�¶�Ϊ 30 �ȣ�
    g_device_state.paper_state = PAPER_STATUS_NORMAL;   // �� paper_state �ֶ�����Ϊ PAPER_STATUS_NORMAL
    g_device_state.read_ble_finish = false;             // �� read_ble_finish �ֶ�����Ϊ false����ʾδ���������ȡ��
}
