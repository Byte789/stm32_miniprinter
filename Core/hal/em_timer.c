#include "em_timer.h"  // ������ʱ����ص�ͷ�ļ�
#include "em_config.h" // ����������ص�ͷ�ļ�

bool read_state_timeout = false;  // ��־λ����ʾ��ȡ״̬�Ƿ�ʱ
bool printer_timeout = false;     // ��־λ����ʾ��ӡ�Ƿ�ʱ

osTimerId myStateTimerHandle;     // ����һ����ʱ����������ڶ�ȡ״̬��ʱ��
osTimerId myTimeoutTimerHandle;   // ����һ����ʱ����������ڴ�ӡ��ʱ��ʱ��

/* ��ȡ״̬��ʱ���Ļص����� */
void read_state_timer_callbackfun(void const * argument)
{
  /* �û����뿪ʼ read_state_timer_callbackfun */
  printf("read_state now...\r\n");  // ��ӡ��Ϣ����ʾ���ڶ�ȡ״̬
  read_state_timeout = true;        // ����ȡ״̬��ʱ��־λ��Ϊ true
  /* �û�������� read_state_timer_callbackfun */
}

/* ��ʼ����ʱ�� */
void init_timer(){
  osTimerDef(myStateTimer, read_state_timer_callbackfun); // �����ȡ״̬��ʱ������ص�����
  myStateTimerHandle = osTimerCreate(osTimer(myStateTimer), osTimerPeriodic, NULL); // ����һ�������Զ�ʱ��
  osTimerStart(myStateTimerHandle, 10000); // ������ʱ������������Ϊ 10000 ���루10 �룩
}

/* ��ȡ��ȡ״̬�Ƿ�ʱ */
bool get_state_timeout(){
    return read_state_timeout; // ���ض�ȡ״̬��ʱ��־λ��ֵ
}

/* �����ȡ״̬��ʱ��־λ */
void clean_state_timeout(){
    read_state_timeout = false; // ����ȡ״̬��ʱ��־λ��Ϊ false
}

/* ��ӡ��ʱ��ʱ���Ļص����� */
void read_timeout_timer_callbackfun(void const * argument)
{
  /* �û����뿪ʼ read_state_timer_callbackfun */
  printf("������ӡ��ʱ����...\r\n"); // ��ӡ��Ϣ����ʾ�����˴�ӡ��ʱ����
  printer_timeout = true;              // ����ӡ��ʱ��־λ��Ϊ true
  /* �û�������� read_state_timer_callbackfun */
}

/* ��ȡ��ӡ��ʱ״̬ */
bool get_printer_timeout_status(){
  return printer_timeout; // ���ش�ӡ��ʱ��־λ��ֵ
}

/* ������ӡ��ʱ��ʱ�� */
void open_printer_timeout_timer(){
  printer_timeout = false; // ����ӡ��ʱ��־λ��Ϊ false
  osTimerDef(myTimeoutTimer, read_timeout_timer_callbackfun); // �����ӡ��ʱ��ʱ������ص�����
  myTimeoutTimerHandle = osTimerCreate(osTimer(myTimeoutTimer), osTimerOnce, NULL); // ����һ��һ���Զ�ʱ��
  osTimerStart(myTimeoutTimerHandle, 20000); // ������ʱ�������ó�ʱΪ 20000 ���루20 �룩
}

/* �رմ�ӡ��ʱ��ʱ�� */
void close_printer_timeout_timer(){
  osTimerDelete(myTimeoutTimerHandle); // ɾ����ӡ��ʱ��ʱ��
}
