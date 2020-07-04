#include "thunder.h"
#include "../SoftPWM/SoftPWM.h"

void motor_init(){

  SoftPWMBegin();  
  SoftPWMSet(9, 0);
  SoftPWMSet(7, 0);

  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);

}

void motor(int velE, int velD) {
  
  // velE = (int) velE*1.1;

  // Gambiarra q eu fiz pro meu carrinho funcionar. O seu não deve precisar.
  // Descomente se o carrinho estiver virando pro lado errado

  // int tmp = velD;
  // velD = velE;
  // velE = tmp;

  velD = (int) velD*1.1;

  velD = (velD > 255 ? 255 : velD);
  velE = (velE > 255 ? 255 : velE);
  
  velE = (velE < -255 ? -255 : velE);
  velD = (velD < -250 ? -255 : velD);

  if (velD > 0) {
    SoftPWMSet(MotorDF, velD);
    digitalWrite(MotorDT, 0);     // Inverta aqui o 0 e o 1 na linha abaixo se o motor direito estiver girando invertido
  } else {
    SoftPWMSet(MotorDF, -velD);
    digitalWrite(MotorDT, 1);
  }

  if (velE > 0) {
    analogWrite(MotorEF, velE);
    digitalWrite(MotorET, 1);   // Inverta aqui o 0 e o 1 na linha abaixo se o motor esquerdo estiver girando invertido
  } else {
    analogWrite(MotorEF, -velE);
    digitalWrite(MotorET, 0);
  }
}

int piloto(char c, int vel){

  if(c == 'w'){
    motor(vel, vel);
  }else if(c == 'a'){
    motor(-vel, vel);
  }else if(c == 's'){
    motor(-vel, -vel);
  }else if(c == 'd'){
    motor(vel, -vel);
  }else if(c == 'l'){
    motor(-vel, -vel);
  }else if(c == ' '){
    motor(0, 0);
  }else if(c == 'q'){
    motor(-vel/2, vel/2);
  }else if(c == 'e'){
    motor(vel/2, -vel/2);
  }

  return 1;
}

encoder::encoder(uint8_t pin_, uint8_t sentido_, uint8_t threshold_){

  pin       = pin_;
  sentido   = sentido_;
  threshold = threshold_;
  
  ticks      = 0;
  prev_state = 0;
}

void encoder::set_pin(uint8_t pin_){ pin = pin_; };
uint8_t  encoder::get_pin(){ return pin; };

void encoder::reset_ticks(){ ticks = 0 ; };
int16_t encoder::get_ticks(){ return ticks; };

void encoder::set_sentido(uint8_t sentido_){ sentido = sentido_; };
uint8_t encoder::get_sentido(){ return sentido; };

void encoder::set_threshold(uint16_t threshold_){ threshold = threshold_; };
uint16_t encoder::get_threshold(){ return threshold; };

void encoder::update_state(){

  uint16_t sensor = analogRead(pin);

  uint8_t state = sensor < threshold ? 1 : 0;

  if(state != prev_state /*&& prev_state == 1*/){ ticks += sentido;}

  prev_state = state;

}

void girar_robo(encoder *roda_direita, encoder *roda_esquerda, uint16_t ticks, rotation_t sentido, int16_t vel){

  ir_frente(roda_direita, roda_esquerda, ticks, sentido, vel, -1);

}

void ir_frente(encoder *roda_direita, encoder *roda_esquerda, uint16_t ticks, rotation_t sentido, int16_t vel, int8_t giro){

  int16_t D_ticks = roda_direita->get_ticks();
  int16_t E_ticks = roda_esquerda->get_ticks();  

  int16_t vel_D = (int16_t) sentido*vel;
  int16_t vel_E = giro*sentido*vel;

  uint8_t done = 0b11;

  while(done != 0){

    // TODO: descomente abaixo qual encoder vc quer usar

    // roda_direita->update_state();
    // roda_esquerda->update_state(); 

    if( (done & (0b01)) && roda_direita->get_ticks() > ticks + D_ticks){
      vel_D = 0;
      vel_E = 0;

      done = 0;
    }

    
    if( (done & (0b10)) && roda_esquerda->get_ticks() > ticks + E_ticks){
      vel_E = 0;
      vel_D = 0;

      done = 0;
    }

    motor(vel_E, vel_D);

    Serial.println(roda_direita->get_ticks() - roda_esquerda->get_ticks());

    delay(20);
  }
}

uint8_t ler_linha(uint8_t *pinos_analog){

  uint8_t linha = 0;

  for(uint8_t i = 0; i < SENSORES; i++){
    
    uint64_t valor = 0;

    for(uint8_t j = 0; j < MEDIDAS; j++)     valor += analogRead(pinos_analog[i]);
    
    if(valor < THRESHOLD){
      linha |= (1<<i);
      Serial.print("1");
      Serial.print(" ");
    }else{
      Serial.print("0");
      Serial.print(" ");    
    }

  }

  Serial.print("\n");

  return linha;
} 

void debug_linha(uint8_t *pinos_analog){

  for(uint8_t i = 0; i < SENSORES; i++){
    
    uint32_t valor = 0;

    for(uint8_t j = 0; j < MEDIDAS; j++)     valor += analogRead(pinos_analog[i]);
    
    Serial.print(valor);
    Serial.print(' ');    

  }

  Serial.print("\n");
}

void seguir_linha(uint8_t *pinos_analog){

  int8_t erro = 0;
  const int8_t K_d = 50;

  while( /*!digitalRead(interrupt_pin)*/ 1 ){

    // if(bluetooth.available()){

    //   char c = bluetooth.read();
    //   bluetooth.print(c);    

    //   if(c == ' ') break;

    // }

    uint8_t linha = ler_linha(pinos_analog);
    
         if(linha == 0b0001) erro = 3;//motor(-LENTO, LENTO);
    else if(linha == 0b0011) erro = 2;//motor(0, LENTO);
    else if(linha == 0b0010) erro = 1;//motor(LENTO/2, LENTO);

    else if(linha == 0b0000) erro = 0;//motor(RAPIDO, RAPIDO);    
    else if(linha == 0b0110) erro = 0;//motor(RAPIDO, RAPIDO);
    
    else if(linha == 0b0100) erro = -1;//motor(LENTO/2, LENTO);
    else if(linha == 0b1100) erro = -2;//motor(LENTO, 0);
    else if(linha == 0b1000) erro = -3;//motor(LENTO, -LENTO);

    // motor(90+Kd*erro, 90-Kd*erro);

    delay(20);
  }

  motor(0, 0);
}