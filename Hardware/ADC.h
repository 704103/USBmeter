#ifndef ADC_H
#define ADC_H
#include <stdint.h>
typedef struct
{
    uint16_t voltage; // 电压
    uint16_t current; // 电流
} tADC_Original;

struct tADC_Value_Struct
{
    float current; // 电压
    float voltage; // 电流
    float temp;    // CPU温度
    // 探头温度
};
extern tADC_Original tADC_OriginalValue;
extern struct tADC_Value_Struct tADC_Value;
extern uint16_t AD_Value[2];
void adc1_init(void);
void ADC_GetValue(void);
#endif
