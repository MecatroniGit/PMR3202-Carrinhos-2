#ifndef THUNDER_H_
#define THUNDER_H_

#include "Arduino.h"

#define MotorEF 9
#define MotorET 10
#define MotorDF 7
#define MotorDT 8

#define DIR_ROT 1
#define ESQ_ROT 0

#define SENSORES 4
#define MEDIDAS 3
#define THRESHOLD 300

#define RAPIDO 100
#define LENTO  75

#define TICKS_POR_VOLTA 12

#define MEIO_GIRO 12
#define UM_METRO  31        // 2.61 volta, 31.31 ticks


typedef enum{

    HORARIO = 1,
    ANTI_HORARIO = -1,

}rotation_t;


typedef unsigned int uint16_t;
typedef unsigned char uint8_t;

void motor_init();
void motor(int velE, int velD);

int piloto(char c, int vel);
int piloto(char c, int vel = 200);

class encoder{

    public:
        
        void update_state();

        encoder(uint8_t pin_, uint8_t sentido_, uint8_t threshold_);

        void set_pin(uint8_t pin_);
        uint8_t get_pin();

        void reset_ticks();
        int16_t get_ticks();

        void set_sentido(uint8_t sentido_);
        uint8_t get_sentido();

        void set_threshold(uint16_t threshold_);
        uint16_t get_threshold();

    private:

        uint8_t pin;
        uint8_t prev_state; // 1 branco, 0 preto
        
        int16_t ticks;
        uint8_t sentido;

        uint16_t threshold;

};

void girar_robo(encoder *roda_direita, encoder *roda_esquerda, uint16_t ticks=6, rotation_t sentido = HORARIO, int16_t vel=90);

void ir_frente(encoder *roda_direita, encoder *roda_esquerda, uint16_t ticks=6, rotation_t sentido = HORARIO, 
                        int16_t vel=75, int8_t giro=1);

uint8_t ler_linha(uint8_t *pinos_analog);
void debug_linha(uint8_t *pinos_analog);
void seguir_linha(uint8_t *pinos_analog);

#endif