#include <RF24Network.h>   //Libraries
#include <RF24.h>
#include <SPI.h>

RF24 radio (9,10);				//Select pins for connection

RF24Network network(radio);

const uint16_t this_node = 01;  //Select number of nodes
const uint16_t other_node = 00;





void setup() {					//Connection settings
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_2MBPS);
  network.begin(90,this_node);
  delay(5000);
  Serial.println("Base station OK");
}

int numero=0  ;

void loop() {
      numero++;
//  char mensaje[15]="Hi T Data";

  network.update();										//Send message
  RF24NetworkHeader headerMensaje(other_node);
  bool okComandoEnviado = network.write(headerMensaje,&numero,sizeof(numero));

  if(okComandoEnviado){				//Validation of Message Sending

    Serial.println("Comand sent");
    Serial.print("El mensaje enviado es:");
    Serial.println(numero);
    }else{
      Serial.println("Comand NOT sent");
         Serial.print("El mensaje NO enviado es:");
    Serial.println(numero);
      }
delay(1000);					//Send every 1 second
}
