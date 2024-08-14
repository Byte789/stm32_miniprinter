#include "em_config.h"
#include "main.h"
#include "math.h"

extern ADC_HandleTypeDef hadc1;

#define ADC_READ_TIME 10

uint32_t ADC_Value[2];

void adc_init() {
//  pinMode(PIN_ADC_EN, OUTPUT);
//  analogReadResolution(BATTERY_ADC_BIT);
//  digitalWrite(PIN_ADC_EN, LOW);
	HAL_ADC_Start(&hadc1);	
	HAL_ADC_PollForConversion(&hadc1,1000);	
}


uint32_t adc_alg_handle(uint32_t *adc, int size) {
    uint32_t sum = 0;
    uint32_t min_val = adc[0];
    uint32_t max_val = adc[0];

    for (int i = 0; i < size; i++) {
        if (adc[i] < min_val) {
            min_val = adc[i];
        }
        else if (adc[i] > max_val) {
            max_val = adc[i];
        }
        sum += adc[i];
    }
    sum = sum - (min_val + max_val);
    uint32_t avg_val = sum / (size - 2);
    return avg_val;
}


/*
*   函数内容：
				移动平均滤波：
				使用一个固定大小的窗口（FILTER_WINDOW_SIZE）来存储最新的ADC值。
				每次读取新值时，更新窗口中的值并计算窗口中所有值的平均值。
*   函数参数：
*   返回值：
*/
/**
 * @brief 获取ADC引脚电压值，加入移动平均滤波算法
 * 
 * @return int 返回处理后的电压值
 */
/*int get_adc_volts()
{
    uint32_t data = 0;  // 存储最终计算的电压值
    uint32_t adc_values[ADC_READ_TIME];  // 存储每次读取的ADC值
    uint32_t filtered_values[FILTER_WINDOW_SIZE];  // 用于移动平均滤波的数组
    int filter_index = 0;  // 当前过滤数组的索引
    uint32_t sum = 0;  // 存储过滤数组的总和

    // 初始化过滤数组，将所有元素设为0
    for (int i = 0; i < FILTER_WINDOW_SIZE; i++) {
        filtered_values[i] = 0;
    }

    // 读取ADC值并进行滤波处理
    for (int sample_ptr = 0; sample_ptr < ADC_READ_TIME; sample_ptr++)
    {
        // 启动ADC转换
        HAL_ADC_Start(&hadc1);
        // 轮询ADC转换是否完成，超时时间为100毫秒
        if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
        {
            // 获取ADC转换值并存储在数组中
            adc_values[sample_ptr] = HAL_ADC_GetValue(&hadc1);
        }
        // 停止ADC转换
        HAL_ADC_Stop(&hadc1);

        // 更新过滤数组和总和
        sum -= filtered_values[filter_index];  // 从总和中减去即将被替换掉的值
        filtered_values[filter_index] = adc_values[sample_ptr];  // 更新过滤数组中的值
        sum += filtered_values[filter_index];  // 将新值加到总和中
        filter_index = (filter_index + 1) % FILTER_WINDOW_SIZE;  // 更新过滤数组的索引
    }

    // 计算滤波后的平均值
    uint32_t avg_filtered_value = sum / FILTER_WINDOW_SIZE;

    // 打印滤波后的ADC电压值
    printf(" Filtered ADC end value = ->%1.3fV \r\n", avg_filtered_value * 3.3f / 4096);
    // 计算电压值并赋值给data
    data = avg_filtered_value * 3.3f / 4096;

    // 再次停止ADC转换以确保ADC已停止
    HAL_ADC_Stop(&hadc1);
    
    // 返回计算出的电压值
    return data;
}
*/
/**
 * @brief 获取ADC引脚电压值，可根据需要加入滤波算法
 * 
 * @return int 
 */
int get_adc_volts()
{
  uint32_t data = 0;
  uint32_t adc1[ADC_READ_TIME];
  uint32_t adc2[ADC_READ_TIME];

  for (int sample_ptr = 0; sample_ptr < ADC_READ_TIME; sample_ptr++)
  {
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
		{
			 adc1[sample_ptr] = HAL_ADC_GetValue(&hadc1);
		}
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
		{
			 adc2[sample_ptr] = HAL_ADC_GetValue(&hadc1);
		}
		HAL_ADC_Stop(&hadc1);
  }
  ADC_Value[0] = adc_alg_handle(adc1, ADC_READ_TIME);
  ADC_Value[1] = adc_alg_handle(adc2, ADC_READ_TIME);
	
  printf(" ADC channel0 end value = ->%1.3fV \r\n", ADC_Value[0] * 3.3f / 4096);
  printf(" ADC channel1 end value = ->%1.3fV \r\n", ADC_Value[1] * 3.3f / 4096);
  data = ADC_Value[0] * 3.3f / 4096;

  HAL_ADC_Stop(&hadc1);
  return data;
}

/**
 公式法：
NTC 热敏电阻温度计算公式：Rt = Rp *EXP(B*(1/T1-1/T2))
可以得到温度T1与电阻Rt的关系T1=1/（log（Rt/Rp）/B+1/T2）

T1和T2指的是K度，即开尔文温度,K度=273.15(绝对温度)+摄氏度。
T2=(273.15+25)
Rt 是热敏电阻在T1温度下的阻值。
Rp 是热敏电阻在T2常温下的标称阻值。100K的热敏电阻25℃的值为100K（即R=100K）。T2=(273.15+25)
B值是热敏电阻的重要参数  B25/50= 3950K ±1%
最终对应的摄氏温度t=T1-273.15,同时+0.5的误差矫正。

查表法：
ADC采样率8位 串联电阻R1=10K R2=热敏电阻 电压3.3V
已知
Rt = R*(3.3-VR)/VR
VR = 3.3*ADC_Value/1024
得出
ADC_Value = VR*4096/3.3 =3.3*R/(Rt+R)*4096/3.3 = R/(Rt+R)*1024
ADC_Value=3.3/(C5+10)*10/3.3*1023
如果需要用到小数点后面的温度，正确的方法是使用公式RT＝R0*exp(B (1/T-1/T0))在excel中计算得到步进值为0.1℃的温度表
Rt=100*exp(3950*(1/(273.15+T1)-1/(273.15+25)))
Rt=100*EXP(3990*(1/(273.15+T1)-1/(273.15+25)))
*/

/**
 * @brief 阻值转换温度
 *
 * @param Rt 热敏电阻阻值
 * @return float 返回摄氏度温度
 */
float em_temp_calculate(float Rt)
{
    float Rp = 30000; // 30k
    float T2 = 273.15 + 25;
    float Bx = 3950; // B值
    float Ka = 273.15;
    float temp = 0.0f;

    temp = 1 / (log(Rt / Rp) / Bx + 1 / T2) - Ka + 0.5;
    return temp;
}

/**
 * @brief Get the adc temperatrue object
 * 
 * @return float 
 */
float get_adc_temperatrue(){
  float temp = 0.0f;
  float Rt=0;
  float vol=0;
  //ADC转换为电压 vol=AD/4096*VCC
  vol=(float)ADC_Value[1]*3.3f/4096;
	printf("ADC temperatrue analog value = %f\n",vol);
  //电压转换为阻值 原理图为10k 1%精度  vol/VCC=Rt/(R+Rt)  vol/3.3=Rt/(10000+Rt)
  Rt=(vol*10000)/(3.3-vol);
	printf("ADC temperatrue Rt = %f\n",Rt);
  temp = em_temp_calculate(Rt);
  return temp;
}

void em_adc_test(){
  float Rt=0;
  float vol=3.0f;
  Rt=(vol*10000)/(3.3-vol);
  printf("Rt = %f\n",Rt);
  
  Rt = 60000; //60k 10°C
  float temp = 0.0f;
  temp = em_temp_calculate(Rt);
  printf("temp = %f\n",temp);

  if(temp >= 1e-7){
      printf("正\n");
  }else{
      printf("负\n");
  }
}
