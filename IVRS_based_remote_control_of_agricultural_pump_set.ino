//Voice playback module
#define P3_4 Msg1
#define P3_3 Msg2
#define P1_6 Msg3
#define P6_6 Msg4
#define P3_2 Msg5
#define P2_7 Msg6
#define P4_2 Msg7
#define P4_1 Msg8

//DTMF decoder
#define P1_5 D4
#define P1_4 D3
#define P1_3 D2
#define P1_2 D1
#define P2_4 VT

//Voltage measurement
#define P6_1 R
#define P6_2 Y
#define P6_3 B
#define P6_4 BAT

//Motor Start/Stop,Battery relay
#define P4_3 START
#define P4_0 STOP
#define P3_7 BAT_RELAY

//GSM Module
#define P2_5 RING
#define P2_0 HOLD

volatile int key=0;
unsigned int prevTime=0;
void setup()
{
  pinMode(Msg1,OUTPUT);
  pinMode(Msg2,OUTPUT);
  pinMode(Msg3,OUTPUT);
  pinMode(Msg4,OUTPUT);
  pinMode(Msg5,OUTPUT);
  pinMode(Msg6,OUTPUT);
  pinMode(Msg7,OUTPUT);
  pinMode(Msg8,OUTPUT);
  
  digitalWrite(Msg1,HIGH);
  digitalWrite(Msg2,HIGH);
  digitalWrite(Msg3,HIGH);
  digitalWrite(Msg4,HIGH);
  digitalWrite(Msg5,HIGH);
  digitalWrite(Msg6,HIGH);
  digitalWrite(Msg7,HIGH);
  digitalWrite(Msg8,HIGH);
  
  
  pinMode(D4,INPUT);
  pinMode(D3,INPUT);
  pinMode(D2,INPUT);
  pinMode(D1,INPUT);
  attachInterrupt(VT,dtmf_detect,FALLING);
  
  pinMode(START,OUTPUT);
  pinMode(STOP,OUTPUT);
  digitalWrite(START,LOW);
  digitalWrite(STOP,LOW);
  
  pinMode(BAT_RELAY,OUTPUT);
  digitalWrite(BAT_RELAY,LOW);
  
  Serial.begin(9600);
  attachInterrupt(RING,ivrs,FALLING);
  pinMode(HOLD,INPUT);
  
}

void loop()
{
  
  while(key!=0)
  {
    if(key==1)
    {
    start_motor();
    key=0;
    }
    if(key==2)
    {
      stop_motor();
      key=0;
    }
    if(key==3)
    {
      start_motor();
      delay_min(30);
      stop_motor();
    }
    if(key==4)
    {
      start_motor();
      delay_min(60);
      stop_motor();
    } 
  }
  unsigned long currentTime=millis();
  if(currentTime - prevTime > 3600000) 
  {
    prevTime = currentTime;
    bat_normalize();
  }     
}

int mains_check()
{
  int Vth=410;
  int Vr,Vy,Vb;
  Vr=analogRead(R);
  Vy=analogRead(Y);
  Vb=analogRead(B);
  
  if((Vr<Vth)||(Vy<Vth)||(Vb<Vth))
  return 0;
  else
  return 1;
}

void bat_normalize()
{
  Vbat=analogRead(BAT);
  if(Vbat<3500)
  digitalWrite(BAT_RELAY,HIGH);
  else
  digitalWrite(BAT_RELAY,LOW);
}

void start_motor()
{ 
  int v;
  v=mains_check();
  if(v==1)
  {
  digitalWrite(START,HIGH);
  delay(4000);
  digitalWrite(START,LOW);
  play_msg(3);
  end_call();
  }
  else
  play_msg(2);
}


void stop_motor()
{
  digitalWrite(STOP,HIGH);
  delay(2000);
  digitalWrite(STOP,LOW);
  play_msg(4);
  end_call();
}
  
void dtmf_decode()
{
  int d[4];
  d[3]=digitalRead(D4);
  d[2]=digitalRead(D3);
  d[1]=digitalRead(D2);
  d[0]=digitalRead(D1);
  key=(d[3]<<3)+(d[2]<<2)+(d[1]<<1)+(d[0]);  
}

void attend_call()
{
  int x;
  delay(5000);
  x=digitalRead(HOLD);
  if(x==1)
  Serial.println("ATA");
}

void end_call()
{
  delay(3500);
  Serial.println("ATH");
}

void ivrs()
{
  attend_call();
  play_msg(1);
}

void play_msg(int m)
{
  switch(m)
  
  case 1:
  digitalWrite(Msg1,LOW);
  delay(1000);
  digitalWrite(Msg1,HIGH);
  break;
  
  case 2:
  digitalWrite(Msg2,LOW);
  delay(1000);
  digitalWrite(Msg2,HIGH);
  break;
  
  case 3:
  digitalWrite(Msg3,LOW);
  delay(1000);
  digitalWrite(Msg3,HIGH);
  break;
  
  case 4:
  digitalWrite(Msg4,LOW);
  delay(1000);
  digitalWrite(Msg4,HIGH);
  break;
  
  case 5:
  digitalWrite(Msg5,LOW);
  delay(1000);
  digitalWrite(Msg5,HIGH);
  break;
  
  case 6:
  digitalWrite(Msg6,LOW);
  delay(1000);
  digitalWrite(Msg6,HIGH);
  break;
  
  case 7:
  digitalWrite(Msg7,LOW);
  delay(1000);
  digitalWrite(Msg7,HIGH);
  break;
  
  case 8:
  digitalWrite(Msg8,LOW);
  delay(1000);
  digitalWrite(Msg8,HIGH);
  break;
 
}

void delay_min(int min)
{
  int c;
  for(c=0;c<min;c++)
  delay(60000);
}
  


