#ifndef KEY_H
#define KEY_H
#define KEY_NONE 0
#define KEY_SHORT 1
#define KEY_LONG 2
#define KEY_CNTINUS 3
#define KEY_DOUBLE 4

typedef struct
{
    const uint8_t count;
} tKeyStatus_Struct;
 
void key_init(void);
void led_init(void);
void key2_init(void);
int8_t key_scan(void);
#endif
