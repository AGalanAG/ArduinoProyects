#include <RF24Network.h>        //Libraries
#include <RF24.h>
#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
RF24 radio (9,10);              //Select pins for connection

RF24Network network(radio);

const uint16_t this_node = 00;         //Select number of nodes
const uint16_t other_node = 01;

void setup() {                        //Connection settings
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
      DoInvert = ('1'==BitString[i]) ^ CRC[4];         // XOR required?

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


   char msjrecibido[10]="";
   bool ok2=false;
 

    network.update();                 //Reception of sent message
    while(network.available()){
      if(network.available() > 0){
    RF24NetworkHeader headermsjrecibido;
    ok2=network.read(headermsjrecibido,&msjrecibido,sizeof(msjrecibido));

    }
  }
   
if(ok2){                              //Message receipt validation
  Serial.print("Mensaje recibido con CRC es: ");
  Serial.println(msjrecibido);
  int dim=strlen(msjrecibido)-5;
  int mov=dim;
  char *Crc = "";
  
char  Result[2];
    char tam[50];
    char tam2[6];
    char *Data;
    int n=0;
    char txcrc[6]="";
    for(int i=0;i<5;i++){       //Store the CRC of the received message in a character string
txcrc[i]=msjrecibido[mov];
mov++;    

      }
msjrecibido[dim]=0;         //Remove the CRC of the message

   Serial.print("Mensaje recibido sin CRC es: ");
  Serial.println(msjrecibido);
     Serial.print(" CRC en mensaje es: ");
  Serial.println(txcrc);

    for(byte j=0;j<dim;j++){             //Read the message bit by bit to write it to a string of characters
    for(int i=7;i>=0;i--){
      tam[n]=bitRead(msjrecibido[j],i)+'0';  
      n++;
     
        }
      }
      tam[n]='\0';
     Data=tam;  

  

 Crc = MakeCRC(Data);               //Calculate CRC-5-USB 
    Serial.print("Mensaje Recibido en binario:");
  Serial.println(Data); 

  Serial.print("TxCRC:");
  Serial.println(txcrc);
    Serial.print("RxCRC:");
      Serial.println(Crc);

if( strcmp(Crc, txcrc) == 0) //Comparison and validation of received CRC with the calculated one
{
   Serial.println("Mensaje Recibido Correctamente dado crc es el mismo");
  }else{
    
    Serial.println("Mensaje Recibido Con error dado  crc no es el mismo");
    }
  
  }else{
    Serial.println("Mensaje no recibido ");
    }

    delay(1000);
}
