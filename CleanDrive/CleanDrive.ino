#include <Wire.h>
#include "SparkFun_ENS160.h"
#include "LedControl.h"
#include <DFRobot_ENS160.h>

SparkFun_ENS160 myENS; 
 
int ensStatus; 
int GasPin = A0;   // 가스센서 입력을 위한 아날로그 핀
LedControl lc = LedControl(12, 11, 10, 4); 

int RED = 5;
int GREEN = 6;
int BLUE = 7;
byte def[8][32] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
// co2 high
byte co2high[8][32] = {
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1},
    {1,0,0,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,0,1,0,0,1,0,1,1,0,1,1,1,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1},
    {0,1,1,0,0,0,1,1,0,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,0,0,1}
};
// co high
byte co1high[8][32] = {
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,0,1,1,1,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1},
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,0,0,1}
};
void clean(){                       // 전체led를 꺼주는 함수
  for(int i = 0; i < 4; i++)
    lc.clearDisplay(i);// clear screen
}
// co2 mid
byte co2mid[8][32] = {
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,1,0},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,1,0,1,1,1,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {0,1,1,0,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,1,0},
};

// co mid
byte co1mid[8][32] = {
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,1,0},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1},
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,1,0},
};

// co2 low
byte co2low[8][32] = {
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,1,0},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {1,0,0,0,0,1,0,0,1,0,1,1,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {1,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {1,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {0,1,1,0,0,0,1,1,0,0,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0},
};

//co low
byte co1low[8][32] = {
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,1,0},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0},
    {0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0},
};

void setup()
{
  pinMode(RED, OUTPUT); 
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);  
    for (int i = 0; i < 4; i++) {      // 4개의 매트릭스 초기화
        lc.shutdown(i, false);          // 디스플레이 활성화
        lc.setIntensity(i, 5);          // 밝기 설정 (1~15)
        lc.clearDisplay(i);             // 화면 초기화
      }
    pinMode(GasPin ,INPUT);   // 아날로그 핀 A0를 입력모드로 설정

    Wire.begin();
    Serial.begin(9600);
    if( !myENS.begin() )
    {
        Serial.println("Did not begin.");
        while(1);
    }
 
    if( myENS.setOperatingMode(SFE_ENS160_RESET) )
        Serial.println("Ready.");
 
    delay(100);
    myENS.setOperatingMode(SFE_ENS160_STANDARD);
    ensStatus = myENS.getFlags();
    Serial.print("Gas Sensor Status Flag: ");
    Serial.println(ensStatus);
}
void def123() {
  for (int row = 0; row < 8; row++) {       // 각 행에 대해
    for (int col = 0; col < 32; col++) {    // 각 열에 대해
      int device = 3 - col / 8;                 // 디바이스 번호 계산 (8열 단위)
      int localCol = col % 8;               // 디바이스 내 열 위치
      lc.setLed(device, row, localCol, def123[row][col]);
    }
  }
}
void co2_hi() {
  for (int row = 0; row < 8; row++) {       // 각 행에 대해
    for (int col = 0; col < 32; col++) {    // 각 열에 대해
      int device = 3 - col / 8;                 // 디바이스 번호 계산 (8열 단위)
      int localCol = col % 8;               // 디바이스 내 열 위치
      lc.setLed(device, row, localCol, co2high[7-row][col]);
    }
  }
}
void co1_hi() {
  for (int row = 0; row < 8; row++) {       // 각 행에 대해
    for (int col = 0; col < 32; col++) {    // 각 열에 대해
      int device = 3 - col / 8;                 // 디바이스 번호 계산 (8열 단위)
      int localCol = col % 8;               // 디바이스 내 열 위치
      lc.setLed(device, row, localCol, co1high[7-row][col]);
    }
  }
  
}
void co1_2_high() {
  co2_hi();
  delay(200);
  clean();
  delay(50);
  co1_hi();
  delay(200);
  clean();
  delay(50);
}

void loop()
{
  if( myENS.checkDataStatus() )
    {
  // analogWrite(RED, 255); // RED ON
  // analogWrite(GREEN, 0);
  // analogWrite(BLUE, 0);
  // delay(1000);
        Serial.println(analogRead(GasPin));   // 가스센서로부터 아날로그 데이터를 받아와 시리얼 모니터로 출력함
        int co1_gas = analogRead(GasPin);
    
        Serial.print("Air Quality Index (1-5) : ");
        Serial.print(myENS.getAQI());
        Serial.print(" ");

        // Serial.print("Total Volatile Organic Compounds: ");
        // Serial.print(myENS.getTVOC());
        // Serial.print("ppb");
        // Serial.print(" ");
        Serial.print("CO2 concentration: ");
        Serial.print(myENS.getECO2());
        Serial.print("ppm");
        Serial.print(" ");

        Serial.print("CO concentration: ");
        Serial.print(co1_gas);
        Serial.println("ppm");

  
    
        if (myENS.getAQI() == 1 ){
          analogWrite(RED, 255); // RED ON
          analogWrite(GREEN, 0);
          analogWrite(BLUE, 0);
        } else if (myENS.getAQI() == 2 ){
          analogWrite(RED,0); // RED ON
          analogWrite(GREEN, 255);
          analogWrite(BLUE, 0);
        } else if (myENS.getAQI() == 3){
          analogWrite(RED, 0); // RED ON
          analogWrite(GREEN, 0);
          analogWrite(BLUE, 255);
        } 
       
        if (myENS.getECO2() > 900 ) {
          while (myENS.getECO2() > 900){
            co1_gas = analogRead(GasPin);
            
            for (int row = 0; row < 8; row++) {       // 각 행에 대해
              for (int col = 0; col < 32; col++) {    // 각 열에 대해
                int device = 3 - col / 8;                 // 디바이스 번호 계산 (8열 단위)
                int localCol = col % 8;               // 디바이스 내 열 위치
                lc.setLed(device, row, localCol, co2high[7-row][col]);
              }
            } 
            if (co1_gas > 300 ){
              co1_2_high();
            }
          }
        }
        if (co1_gas > 300){
          while (co1_gas > 300){
            Serial.println(co1_gas);
            for (int row = 0; row < 8; row++) {       // 각 행에 대해
              for (int col = 0; col < 32; col++) {    // 각 열에 대해
                int device = 3 - col / 8;                 // 디바이스 번호 계산 (8열 단위)
                int localCol = col % 8;               // 디바이스 내 열 위치
                lc.setLed(device, row, localCol, co1high[7-row][col]);
              }
            }
            co1_gas = analogRead(GasPin);
            if (myENS.getECO2() > 900 ){
              co1_2_high();
            }
          }
        }
        else {
          clean();
        }

        

    }
}