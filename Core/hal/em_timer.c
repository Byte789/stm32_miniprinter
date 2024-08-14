#include "em_timer.h"  // 包含定时器相关的头文件
#include "em_config.h" // 包含配置相关的头文件

bool read_state_timeout = false;  // 标志位，表示读取状态是否超时
bool printer_timeout = false;     // 标志位，表示打印是否超时

osTimerId myStateTimerHandle;     // 定义一个定时器句柄，用于读取状态定时器
osTimerId myTimeoutTimerHandle;   // 定义一个定时器句柄，用于打印超时定时器

/* 读取状态定时器的回调函数 */
void read_state_timer_callbackfun(void const * argument)
{
  /* 用户代码开始 read_state_timer_callbackfun */
  printf("read_state now...\r\n");  // 打印信息，表示正在读取状态
  read_state_timeout = true;        // 将读取状态超时标志位设为 true
  /* 用户代码结束 read_state_timer_callbackfun */
}

/* 初始化定时器 */
void init_timer(){
  osTimerDef(myStateTimer, read_state_timer_callbackfun); // 定义读取状态定时器及其回调函数
  myStateTimerHandle = osTimerCreate(osTimer(myStateTimer), osTimerPeriodic, NULL); // 创建一个周期性定时器
  osTimerStart(myStateTimerHandle, 10000); // 启动定时器，设置周期为 10000 毫秒（10 秒）
}

/* 获取读取状态是否超时 */
bool get_state_timeout(){
    return read_state_timeout; // 返回读取状态超时标志位的值
}

/* 清除读取状态超时标志位 */
void clean_state_timeout(){
    read_state_timeout = false; // 将读取状态超时标志位设为 false
}

/* 打印超时定时器的回调函数 */
void read_timeout_timer_callbackfun(void const * argument)
{
  /* 用户代码开始 read_state_timer_callbackfun */
  printf("触发打印超时错误...\r\n"); // 打印信息，表示触发了打印超时错误
  printer_timeout = true;              // 将打印超时标志位设为 true
  /* 用户代码结束 read_state_timer_callbackfun */
}

/* 获取打印超时状态 */
bool get_printer_timeout_status(){
  return printer_timeout; // 返回打印超时标志位的值
}

/* 开启打印超时定时器 */
void open_printer_timeout_timer(){
  printer_timeout = false; // 将打印超时标志位设为 false
  osTimerDef(myTimeoutTimer, read_timeout_timer_callbackfun); // 定义打印超时定时器及其回调函数
  myTimeoutTimerHandle = osTimerCreate(osTimer(myTimeoutTimer), osTimerOnce, NULL); // 创建一个一次性定时器
  osTimerStart(myTimeoutTimerHandle, 20000); // 启动定时器，设置超时为 20000 毫秒（20 秒）
}

/* 关闭打印超时定时器 */
void close_printer_timeout_timer(){
  osTimerDelete(myTimeoutTimerHandle); // 删除打印超时定时器
}
