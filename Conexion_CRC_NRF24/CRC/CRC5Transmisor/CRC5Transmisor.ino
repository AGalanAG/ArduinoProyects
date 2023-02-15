#include <RF24Network.h>          //Libraries
#include <RF24.h>
#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
RF24 radio (9,10);                 //Select pins for connection

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


char *MakeCRC(char *BitString)
   {
   static char Res[6];                                 // CRC Result
   char CRC[5];
   int  i;
   char DoInvert;
   
   for (i=0; i<5; ++i)  CRC[i] = 0;                    // Init before calculation
   
   for (i=0; i<strlen(BitString); ++i)
      {
      DoInvert = ('1'==BitString[i]) ^ CRC[4];         // XOR required

      CRC[4] = CRC[3];
      CRC[3] = CRC[2];
      CRC[2] = CRC[1] ^ DoInvert;
      CRC[1] = CRC[0];
      CRC[0] = DoInvert;
      }
      
   for (i=0; i<5; ++i)  Res[4-i] = CRC[i] ? '1' : '0'; // Convert binary to ASCII
   Res[5] = 0;                                         // Set string terminator

   return(Res);
   }

void loop() {
  char binario[10]={"hola"};
  int dim=strlen(binario); 
    char  *Result;
    char tam[50];
    char *Data;
    int n=0;
   
    for(byte j=0;j<dim;j++){                //Read the message bit by bit to write it to a string of characters
    for(int i=7;i>=0;i--){
      tam[n]=bitRead(binario[j],i)+'0';  
      n++;
     
        }
      }
      tam[n]='\0';
     Data=tam;  
                                     
   Result = MakeCRC(Data);                     //Calculate CRC-5-USB               

    Serial.print("Mensaje:");
  Serial.println(Data); 
  Serial.print("CRC:");
  Serial.println(Result);
  strcat(binario,Result);

Serial.print("Mensaje a enviar:");
  Serial.println(binario);



  network.update();                           //Send message
  RF24NetworkHeader headerMensaje(other_node);
  bool okComandoEnviado = network.write(headerMensaje,&binario,sizeof(binario));

  if(okComandoEnviado){               //Validation of Message Sending
    Serial.println("Comand sent");
    Serial.print("El mensaje enviado es:");
    Serial.println(binario);
    }else{
      Serial.println("Comand NOT sent");
      Serial.print("El mensaje NO enviado es:");
      Serial.println(binario);
      }
   delay(1000);                     //Send every 1 second
}
