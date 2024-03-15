#ifndef UI_MEASURE_H
#define UI_MEASURE_H
#include <stdint.h>

typedef struct 
{
    float Power;
    float Ah;
    float Wh;    
    uint32_t Time;
}tPower_ValueStruct;
extern tPower_ValueStruct tPower_Value;



void UI_Measure_Load(void);

void UI_Measure_Exit(void);

void UI_Measure_Task(void);

#endif
