#include <RF24Network.h>        //Libraries
#include <RF24.h>
#include <SPI.h>
#include <string.h>
RF24 radio (9,10);              //Select pins for connection

RF24Network network(radio);

const uint16_t this_node = 01;      //Select number of nodes
const uint16_t other_node = 00;

void setup() {                      //Connection settings
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_2MBPS);
  network.begin(90,this_node);
  delay(5000);
  Serial.println("Base station OK");
}

uint8_t Checksum(const byte *paquete, size_t dimension)         //Creation of the function for calculating the CheckSum
{
  
  uint16_t sum = 0;
  for (size_t i = 0; i < dimension; i++)
  {
     sum += (uint16_t)paquete[i];
 
  if( bitRead(sum,8) == 1){                                     //If the result is 1 in bit nine, we replace it with a zero and add it to the beginning.
    bitWrite(sum,8,0);

    sum =sum+1;
    }
  }
  
   while (sum>>8)
    sum = (sum & 0xF) + (sum >> 8);

  return (uint8_t)~sum;                                 //Return the negated value
}




void loop() {                                   
  char binario[10]={"10"};                  
  int dim=strlen(binario);
  uint8_t sum = Checksum(binario,dim);

  Serial.print("checksum:");
  Serial.println(sum,BIN);

  binario[dim]=sum;
  
  Serial.print("Mensaje a enviar:");
  Serial.println(binario);

  network.update();                               //Send message
  RF24NetworkHeader headerMensaje(other_node);
  bool okComandoEnviado = network.write(headerMensaje,&binario,sizeof(binario));

  if(okComandoEnviado){                         //Validation of Message Sending
    Serial.println("Comand sent");
    Serial.print("El mensaje enviado es:");
    Serial.println(binario);
    }else{
      Serial.println("Comand NOT sent");
      Serial.print("El mensaje NO enviado es:");
      Serial.println(binario);
      }
   delay(1000);                       //Send every 1 second
}
