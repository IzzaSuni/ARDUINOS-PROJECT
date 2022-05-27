#define BLYNK_TEMPLATE_ID "TMPLs94mmoYi"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd1(0x27,16,2);
#include <EEPROM.h>
#define trigPin 14
#define echoPin 12
#define buttonAdd 15
#define buttonMenu 16 //Active LOW
#define buttonPop 0
#define ledStatusAndRelay 13 //active LOW
#define Buzzer 3
#define sizeE 512
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
SimpleTimer timer;

byte addr=0;      //address set tinggi
byte addr2=200;    //addr flag kondisi 0 untuk tersedia 1 mengisi
byte persen;
byte flag;
byte flag2=7;
byte flag1=0 ;
char auth[]="5q1tiZ8KGMZ635rWWjgJPeUJhazfRAMK";
char ssid[] = "KONTRAKAN UYE";
char pass[] = "KUSANG123";
float tinggi,cm,temp,kosong,full;
long duration; 
int distance,baca;
int SISTEM=1,buttonMenuOnline;  //Virtual Pin
bool isConnecteds,tryReconnect=false;
WidgetLCD lcd(V1);

void setup() {
  Serial.begin(9600);
  int tinggiVal;
  EEPROM.begin(sizeE);
  pinMode(ledStatusAndRelay, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(SISTEM, INPUT);
  pinMode(buttonMenu,INPUT);
  pinMode(buttonAdd,INPUT);
  pinMode(buttonPop, INPUT); 
  digitalWrite(ledStatusAndRelay,LOW);
  lcd1.init();
  lcd1.backlight();
  lcd1.print("SISTEM POMPA AIR");
  SoundOn();
  lcd1.clear();
  lcd1.print(" SETTING TINGGI");
  lcd1.setCursor(0,1);
  lcd1.print("TINGGI:");
//   EEPROM.put(addr,tinggiVal=77);
  tinggi=EEPROM.get(addr,tinggiVal);
  delay(500);
  lcd1.print(tinggi);
  lcd1.print("cm"); 
  delay(1000);
  lcd1.clear();
  full=0.9*tinggi;
  kosong=tinggi-full;
//  Blynk.begin(auth, ssid, pass);
  lcd1.clear();
  lcd1.print("SET KONEKSI.");
  lcd1.setCursor(0,1);
  lcd1.print("KE INTERNET.");
  WiFi.begin(ssid, pass); 
  Blynk.config(auth);  // in place of Blynk.begin(auth, ssid, pass);
  Blynk.connect(1111);
  isConnecteds=Blynk.connect();
  lcd1.clear();
  if(isConnecteds==true){
  lcd1.print("SUKSES KONEKSI.");   
   lcd1.setCursor(0,1);
  lcd1.print("MODE ONLINE");
  }
  else if(isConnecteds==false){
  lcd1.print("GAGAL KONEKSI.");   
  lcd1.setCursor(0,1);
  lcd1.print("MODE OFFLINE");
  }
  delay(1000);
  EEPROM.end();
}
void loop() { 
isConnecteds=Blynk.connected();
if(isConnecteds==true){
  Blynk.run(); 
if(SISTEM==1){
  while(SISTEM==1){
  lcd1.backlight();
  Blynk.run(); 
  EEPROM.begin(sizeE);
  GetData();
  ShowData();
  Condition();
if(digitalRead(buttonMenu)==LOW){
   SetTinggi();
}
if(buttonMenuOnline==1){
    Blynk.run();
   SetTinggiOnline();
}
  EEPROM.end();
  Blynk.virtualWrite(V10,persen);
  Blynk.virtualWrite(V11,cm);
}
}
else if(SISTEM==0){
 while(SISTEM==0){
 lcd1.noBacklight();
 lcd1.setCursor(0,0);
 lcd1.print("Sistem Shut Down");
 lcd1.setCursor(0,1);
 lcd1.print("POWER SAVING...");
 lcd.print(0,0,"Sistem Shut Down");
 lcd.print(0,1,"POWER SAVING...");  
 digitalWrite(Buzzer,LOW);
 digitalWrite(ledStatusAndRelay,LOW);
}
lcd.clear();
lcd1.clear();
}
 Blynk.virtualWrite(V10,persen);
 Blynk.virtualWrite(V11,persen);

}
else if(isConnecteds==false){
  lcd1.backlight(); 
  EEPROM.begin(sizeE);
  GetData();
  ShowData();
  Condition();
if(digitalRead(buttonMenu)==LOW){
   SetTinggi();
}
  EEPROM.end();
}
}
BLYNK_WRITE(V5){
  int Value=param.asInt();
  SISTEM=Value;
}
BLYNK_WRITE(V6){
  int Value=param.asInt();
  buttonMenuOnline=Value;
}
void SetTinggi(){
 EEPROM.begin(sizeE);
 int tinggiVal;
 lcd1.clear();
 lcd.clear();
 while(digitalRead(buttonMenu)==LOW){ 
 SoundMenu();
 lcd1.setCursor(0,0);
 lcd1.print("Ofline SetTinggi");
 lcd.print(0,0,"Ofline SetTinggi");
 digitalWrite(ledStatusAndRelay,LOW); 
 if(digitalRead(buttonAdd)==HIGH){
  tinggi+=1.0;
  delay(1);
 }
 if(digitalRead(buttonPop)==LOW){
  tinggi-=1.0;
  if(tinggi<30){
    tinggi=30;
    }
  delay(1);
 }
 if(tinggi<100){
  lcd1.setCursor(14,1);
  lcd1.print(" ");
 }
 lcd1.setCursor(0,1);
 lcd1.print("TINGGI:");
 lcd.print(0,1,"TINGGI:");
 lcd1.print(tinggi);
 lcd1.print("cm");
 lcd.print(7,1,tinggi);
 
 }
full=0.9*tinggi;
 kosong=tinggi-full;
 lcd1.clear();
 lcd.clear();
 EEPROM.put(addr,tinggiVal=tinggi);
 lcd1.setCursor(0,0);
 lcd1.print("SUDAH DI SET");
 lcd.print(0,0,"SUDAH DI SET");
  lcd1.setCursor(0,1);
lcd1.print("TINGGI:");
lcd.print(0,1,"TINGGI:");
lcd1.print(tinggi);
lcd1.print("cm"); 
lcd.print(7,1,tinggi);
delay(1000);
lcd1.clear();
lcd.clear();
EEPROM.end();
}
BLYNK_WRITE(V0){
  if(buttonMenuOnline==1){
  int Value=param.asInt();
  tinggi=Value;
  if(tinggi<30)tinggi=30;
  }
}
void SetTinggiOnline(){
 Blynk.run();
 EEPROM.begin(sizeE);
 int tinggiVal;
 lcd1.clear();
 lcd.clear();
 while(buttonMenuOnline==1){ 
 Blynk.run();
 SoundMenu();
 lcd1.setCursor(0,0);
 lcd1.print("Online SetTinggi");
 lcd.print(0,0,"Online SetTinggi");
 digitalWrite(ledStatusAndRelay,LOW); 
 if(tinggi<100){
  lcd1.setCursor(14,1);
  lcd1.print(" ");
 }
 lcd1.setCursor(0,1);
 lcd1.print("TINGGI:");
 lcd.print(0,1,"TINGGI:");
 lcd1.print(tinggi);
 lcd1.print("cm");
 lcd.print(7,1,tinggi );
 
 }
 Blynk.run();
 
full=0.9*tinggi;
 kosong=tinggi-full;
 lcd1.clear();
 lcd.clear();
 EEPROM.put(addr,tinggiVal=tinggi);
 lcd1.setCursor(0,0);
 lcd1.print("SUDAH DI SET");
 lcd.print(0,0,"SUDAH DI SET");
  lcd1.setCursor(0,1);
lcd1.print("TINGGI:");
lcd.print(0,1,"TINGGI:");
lcd1.print(tinggi);
lcd1.print("cm"); 
lcd.print(7,1,tinggi);
delay(1000);
lcd1.clear();
lcd.clear();

EEPROM.end();
}
void ShowData(){
 if(flag1!=5){

 lcd1.setCursor(0,0);
 lcd1.print(persen);
 lcd1.print("%");
 lcd.print(0,0,persen);
 if(persen>=100){
  lcd.print(3,0,"% ");
 }
  if(persen<100)
  {
  lcd1.setCursor(3,0);
  lcd1.print("  ");
  lcd.print(2,0,"%  ");
  }
  if(persen<10)
  {
  lcd1.setCursor(2,0);
  lcd1.print("  ");
  lcd.print(1,0,"%  ");
  }
 
 }
}
void GetData(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // menerima suara ultrasonic
  cm = duration * 0.034 / 2; 
  temp=cm-kosong;
  baca=full-temp;
  persen=(baca/full)*100;
  Blynk.virtualWrite(V7,cm); 
}
void Condition(){
  EEPROM.begin(sizeE);
  int flagVal;
  flag=EEPROM.get(addr2,flagVal);
  if(flag==1&&tryReconnect==true&&WiFi.status() == WL_CONNECTED&&Blynk.connected()==false){
    lcd1.print("RECONNECT...");
    digitalWrite(ledStatusAndRelay,LOW);
    Blynk.connect(3333);
    if(Blynk.connect()==true){
    lcd1.setCursor(0,1);
    lcd1.print("SUKSES");
    }
    else if(Blynk.connect()==false){
    lcd1.setCursor(0,1);
    lcd1.print("GAGAL");      
    }
    delay(1000);
    lcd1.clear();
    tryReconnect=false;
  }
  
 if(persen<=20){
  if(flag1!=1||flag1!=2){
   lcd1.clear();
   lcd.clear();
   flag1=1;
  }
  digitalWrite(ledStatusAndRelay,HIGH);
  lcd1.setCursor(5,0);
  lcd1.print("SISTEM ON");
  lcd.print(5,0,"SISTEM ON");
  lcd1.setCursor(3,1);
  lcd1.print("MENGISI AIR");
  lcd.print(3,1,"MENGISI AIR");
  EEPROM.put(addr2,flagVal=1);
  flag=EEPROM.get(addr2,flagVal);
  }
  else if(persen>20&&persen<100&&flag==1){
  if(flag1!=2){
    lcd1.clear();
    lcd.clear();
    flag1=2;
   }

  digitalWrite(ledStatusAndRelay,HIGH);
  lcd1.setCursor(5,0);
  lcd1.print("SISTEM ON");
  lcd.print(5,0,"SISTEM ON");
  lcd1.setCursor(3,1);
  lcd1.print("MENGISI AIR"); 
  lcd.print(3,1,"MENGISI AIR");        
  digitalWrite(ledStatusAndRelay,HIGH);
  
  }
  else if(persen>20&&persen<100&&flag==0){
    if(flag1!=3){
    lcd1.clear();
    lcd.clear();
    flag1=3;
   }
    
    digitalWrite(ledStatusAndRelay,LOW);
     lcd1.setCursor(5,0);
    lcd1.print("SISTEM OFF");
    lcd.print(5,0,"SISTEM OFF");
    lcd1.setCursor(3,1);
    lcd1.print("AIR TERSEDIA");
    lcd.print(3,1,"AIR TERSEDIA");
  }  
else if(persen>=100){
  if(flag1!=4||flag1!=3||flag1!=2){
    lcd1.clear();
    lcd.clear(); 
  SoundOff();
  flag1=4;
  }
  
  digitalWrite(ledStatusAndRelay,LOW);
  lcd1.setCursor(5,0);
  lcd1.print("SISTEM OFF");
  lcd.print(5,0,"SISTEM OFF");
  lcd1.setCursor(3,1);
  lcd1.print("AIR TERSEDIA");
  lcd.print(3,1,"AIR TERSEDIA");
  EEPROM.put(addr2,flagVal=0);
  flag=EEPROM.get(addr2,flagVal);
  
}
else if(persen>=105||cm<3){
  if(flag1!=5){
    lcd1.clear();
    lcd.clear(); 
  }
  SoundMenu();
  flag1=5;
  digitalWrite(ledStatusAndRelay,LOW);
  lcd1.setCursor(0,0);
  lcd1.print("ERR!Cek Sensor!");
  lcd.print(0,0,"ERR!Cek Sensor!");
  lcd1.setCursor(0,1);
  lcd1.print("atau Set Tinggi");
  lcd.print(0,1,"atau Set Tinggi");
  EEPROM.put(addr2,flagVal=0);
  flag=EEPROM.get(addr2,flagVal);
  digitalWrite(ledStatusAndRelay,LOW); 
}
EEPROM.end(); 
if(flag==0){
   Blynk.virtualWrite(V4, "POMPA MATI");
 }
 else if(flag==1){
   Blynk.virtualWrite(V4,"POMPA HIDUP");
 } 
}
void SoundOn(){
  tone(Buzzer, 500); // Send 1KHz sound signal...
  delay(200);
  tone(Buzzer, 600); // Send 1KHz sound signal...
  delay(200);// ...for 1 sec
  tone(Buzzer, 500); // Send 1KHz sound signal...
  delay(200);
  tone(Buzzer, 800); // Send 1KHz sound signal...
  delay(200);
  tone(Buzzer, 500); // Send 1KHz sound signal...
  delay(200);
   tone(Buzzer, 800); // Send 1KHz sound signal...
  delay(200);
   tone(Buzzer, 1000); // Send 1KHz sound signal...
  delay(200);
   tone(Buzzer, 1500); // Send 1KHz sound signal...
  delay(200);
  noTone(Buzzer);     // Stop sound...
}
void SoundOff(){
  tone(Buzzer, 1000); // Send 1KHz sound signal...
  delay(200);
  tone(Buzzer, 900); // Send 1KHz sound signal...
  delay(200);// ...for 1 sec
  tone(Buzzer, 800); // Send 1KHz sound signal...
  delay(200);
  tone(Buzzer, 500); // Send 1KHz sound signal...
  delay(200);
  tone(Buzzer, 200); // Send 1KHz sound signal...
  delay(200);
  tone(Buzzer, 800); // Send 1KHz sound signal...
  delay(200);
  tone(Buzzer, 500); // Send 1KHz sound signal...
  delay(200);
    tone(Buzzer, 1500); // Send 1KHz sound signal...
  delay(200);
  noTone(Buzzer);     // Stop sound...
}
void SoundMenu(){
  tone(Buzzer, 1000); // Send 1KHz sound signal...
  delay(50);
  noTone(Buzzer);     // Stop sound...
}
