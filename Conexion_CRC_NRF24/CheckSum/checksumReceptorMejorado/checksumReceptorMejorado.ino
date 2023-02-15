#include <RF24Network.h>      //Libraries
#include <RF24.h>
#include <SPI.h>

RF24 radio (9,10);            //Select pins for connection

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
uint16_t Checksum(const byte *paquete, size_t dimension)    //Creation of the function for calculating the CheckSum
{                                                           
  uint16_t value = 1;
  uint16_t sum = 0;
  for (size_t i = 0; i < dimension; i++)
  {
     sum += (uint16_t)paquete[i];                     

  if( bitRead(sum,8) == 1){                             //If the result is 1 in bit nine, we replace it with a zero and add it to the beginning.
    bitWrite(sum,8,0);

    sum =sum+1;
    }
  }
  
   while (sum>>8)
    sum = (sum & 0xF) + (sum >> 8);

  return (uint8_t)~sum;                               //Return the negated value
}



void loop() {                                       
   uint8_t comp;
   uint8_t sum;
   int numero;
   char msjrecibido[10]="";
   bool ok2=false;


    network.update();                                 //Reception of sent message
    while(network.available()){
      if(network.available() > 0){
    RF24NetworkHeader headermsjrecibido;
    ok2=network.read(headermsjrecibido,&msjrecibido,sizeof(msjrecibido));

    }
  }
   
if(ok2){                                                 //Message receipt validation
  Serial.print("Mensaje recibido con checksum es: ");
  Serial.println(msjrecibido);
  int dim=strlen(msjrecibido);
  sum = Checksum(msjrecibido,dim);                    //Calculating the CheckSum
  comp=msjrecibido[dim-1];            
  msjrecibido[dim-1]='\0';                            //Removing the Checksum from the Message
  Serial.print("Mensaje recibido sin checksum es: ");
  Serial.println(msjrecibido);
    Serial.print("Suma del mensaje transmitido es: ");
  Serial.println(sum,BIN);

if(sum==0)                          //Validation of CheckSum
{
   Serial.println("Mensaje Recibido Correctamente dado que la suma es 0");
  }else{
    
    Serial.println("Mensaje Recibido Con error dado que la suma es 0");
    }
  
  }else{
    Serial.println("Mensaje no recibido ");
    }

    delay(1000);
}
