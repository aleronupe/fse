#include "temperature.h"
float potentiometer = 0.0, internal = 0.0, external = 0.0, terminal = 0.0, reference = 0.0;
void * update_temperatures(void * vargp){
    CSV();
    pid_configura_constantes(5, 1, 5);
    DISPLAY();
    I2C();
    GPIO();
    while(1){
        int uart = UART();
        potentiometer = get_potentiometer_temperature_uart(uart);
        internal = get_internal_temperature_uart(uart);
        double controle = pid_controle(internal);
        if(controle > 0){
            turn_on_resistor(controle);
        }else{
            turn_on_fan(controle);
        }
        external = get_external_temperature_i2c();
        if(get_reference_temperature()==0){
            set_reference_temperature(potentiometer);
        }
        reference = get_reference_temperature();
        
        char first[30], second[30];
        sprintf(first, "TI:%.2fTE:%.2f", internal, external);
        sprintf(second, "TP:%.2fTR:%.2f", potentiometer, reference);
        write_first(first);
        write_second(second);
        float temperatures[5] = {internal, external, potentiometer, terminal, reference};
        insert_line(temperatures, 5);
        sleep(1);
    }
}
float get_potentiometer_temperature(){
    return potentiometer;
}
float get_internal_temperature(){
    return internal;
}
float get_external_temperature(){
    return external;
}
float get_terminal_temperature(){
    return terminal;
}
void set_terminal_temperature(float temperature){
    terminal = temperature;
}