
#include "src/SoftPWM/SoftPWM.h"
#include "src/Thunder/thunder.h"
#include <SoftwareSerial.h>  
#include <Servo.h>

SoftwareSerial bluetooth(11, 12);

// O parâmetro sentido não é usado, DEIXE COMO 1 
encoder roda_esq(A4, 1, 110); // O THRESHOLD é o valor da leitura analógica que separa preto e branco
encoder roda_dir(A5, 1, 110); // Pega a média entre o valor lido entre as duas cores

Servo trevas;

int16_t vel = 100;

uint8_t pinos_analog[] = {A0, A1, A2, A3};

void setup() {

  motor_init(); 

  pinMode(A0, INPUT);

  // trevas.attach(3);

  // trevas.write(70);

  Serial.begin(115200);
  bluetooth.begin(115200);
}

bool ligado = false;

void arma_ativa(){

  ligado = !ligado;
  if(ligado){
    trevas.write(70);
  }else{
    trevas.write(120);
  }

}

void radio_controle(){

  // tone(5, 440, 1000);
  while(1){

      char c = ' ';
      if(bluetooth.available()){
        
        c = bluetooth.read();
        bluetooth.print(c);

        if(c == '0') break;

        if(c == 'p') arma_ativa();

        piloto(c);

      }
  }
}

void autonomous(rotation_t sentido){

  /*
    TODO: Atualize a quantidade de TICKS para se aproximar das medidas reais.
    Por exemplo, o diametro da minha roda é 380mm.
    A constante TICKS_POR_VOLTA é igual pros robôs, 12 ticks por volta
  */


  if(sentido == HORARIO){

    ir_frente(&roda_dir, &roda_esq, TICKS_POR_VOLTA-1, HORARIO, 90);  // 300mm
    delay(500);

    girar_robo(&roda_dir, &roda_esq, TICKS_POR_VOLTA/2, sentido, 90);  //90 graus
    delay(500);

    ir_frente(&roda_dir, &roda_esq, TICKS_POR_VOLTA-1, HORARIO, 90);   // 300mm
    delay(500);

    girar_robo(&roda_dir, &roda_esq, TICKS_POR_VOLTA/2-1, -sentido, 90);
    delay(500);

    ir_frente(&roda_dir, &roda_esq, 2*TICKS_POR_VOLTA-5, HORARIO, 90);   // 600mm
    delay(500);
    
    // girar_robo(&roda_dir, &roda_esq, TICKS_POR_VOLTA/2-1, sentido, 90);
    // delay(500);
    
    // ir_frente(&roda_dir, &roda_esq, 2*TICKS_POR_VOLTA-5, HORARIO, 90);   // 600mm
    // delay(500);
  }else{

    ir_frente(&roda_dir, &roda_esq, TICKS_POR_VOLTA-1, HORARIO, 90);  // 300mm
    delay(500);

    girar_robo(&roda_dir, &roda_esq, TICKS_POR_VOLTA/2, sentido, 90);  //90 graus
    delay(500);

    ir_frente(&roda_dir, &roda_esq, TICKS_POR_VOLTA-1, HORARIO, 90);   // 300mm
    delay(500);

    girar_robo(&roda_dir, &roda_esq, TICKS_POR_VOLTA/2-1, -sentido, 90);
    delay(500);

    ir_frente(&roda_dir, &roda_esq, 2*TICKS_POR_VOLTA-5, HORARIO, 90);   // 600mm
    delay(500);
    
    // girar_robo(&roda_dir, &roda_esq, TICKS_POR_VOLTA/2-1, sentido, 90);
    // delay(500);
    
    // ir_frente(&roda_dir, &roda_esq, 2*TICKS_POR_VOLTA-5, HORARIO, 90);   // 600mm
    // delay(500);    

  }

  bluetooth.flush();

  // seguir_linha(pinos_analog);

}

void loop() {

  if( bluetooth.available() ){

    char c = bluetooth.read();
    bluetooth.print(c);

    if(c == 'r') radio_controle();

    if(c == 'h') autonomous(HORARIO);
    if(c == 'n') autonomous(ANTI_HORARIO); 

    bluetooth.flush();
  }

  delay(10);

}



