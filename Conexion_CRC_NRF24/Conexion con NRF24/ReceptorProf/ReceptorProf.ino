#include <RF24Network.h>  //Libraries
#include <RF24.h>
#include <SPI.h>

RF24 radio (9,10);      //Select pins for connection

RF24Network network(radio);

const uint16_t this_node = 00;    //Select number of nodes
const uint16_t other_node = 01;

void setup() {                    //Connection settings
  Serial.begin(9600);
   SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  network.begin(90,this_node);
  delay(5000);
  Serial.println("Base station OK");

}
int contador=0;
void loop() {
  
  int numero;
  char msjrecibido[15]="";
  bool ok2=false;
  int numerorecibido;  

    //Serial.print("Mensajes totales recibidos :");
 Serial.print(contador);
 Serial.print("\t");
 network.update();                          //Reception of sent message
 while(network.available()){
  if(network.available() > 0){
    RF24NetworkHeader headermsjrecibido;
    ok2=network.read(headermsjrecibido,&numerorecibido,sizeof(numerorecibido));

    }
  }
   
if(ok2){                  //Message receipt validation
      contador++;
  //Serial.print("Mensajes totales enviados : ");
  Serial.print(numerorecibido);
    Serial.print("\n");
  }else{
      Serial.print(numerorecibido);
    Serial.print("\n");
    //Serial.println("Mensaje no recibido ");

    }

    delay(1000);
}
