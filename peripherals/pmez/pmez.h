#ifndef PMEZ_H
#define PMEZ_H

typedef struct {
    float voltage;
    float current;
    float power;
    int power_factor;
} pzem_data_t

void pmez_init(void);
bool pmez_read_data(pzem_data_t *data);




#endif