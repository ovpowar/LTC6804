#include <BMS_balancing.h>
#include <BMS_config.h>
#include <BMS_read.h>


 #include <Arduino.h>
#include <stdint.h>
#include "Linduino.h"
#include "LT_SPI.h"
#include "LTC68041.h"
#include <SPI.h>`

const uint8_t TOTAL_IC = 2;

uint8_t uflag = 0;
uint8_t rflag = 0;

uint8_t sump = 0;
uint8_t sumr = 0;

long sv = 0;
int low_stack = 0;
uint8_t pflag = 0;

uint16_t cell_codes[TOTAL_IC][12];
uint16_t aux_codes[TOTAL_IC][6];
uint16_t stat_codes[TOTAL_IC][8];
int ITMP[TOTAL_IC];
uint8_t tx_cfg[TOTAL_IC][6];
uint8_t rx_cfg[TOTAL_IC][8];
int sum_low_fault = 0;
int sum_critical_fault = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(49, OUTPUT);
  digitalWrite(49,HIGH);
  //pinMode(3, OUTPUT);
  //digitalWrite(3,LOW);
  LTC6804_initialize(); 
  init_cfg();
 pinMode(22,OUTPUT); 
 pinMode(23,OUTPUT);
 pinMode(24,OUTPUT);
}
void loop()
{
 digitalWrite(22,HIGH);
 digitalWrite(23,HIGH);
 digitalWrite(24,HIGH);  
    run_command();
}
int ignore=1;


void run_command()
{
      ignore = 1;
      int8_t error = 0;
      int8_t error_t = 0;
      char input = 0;
      wakeup_sleep();
      LTC6804_wrcfg(TOTAL_IC,tx_cfg);
      while (1)
      {
        wakeup_idle();
        LTC6804_adcv();
        LTC6804_adax();
        delay(10);
        wakeup_idle();
        error = LTC6804_rdcv(0, TOTAL_IC,cell_codes);
        error_t= LTC6804_rdaux(0,TOTAL_IC,aux_codes);
        error = LTC6804_rdstat(0,TOTAL_IC,stat_codes);
        error = readTempValue(aux_codes);
        error = readStat(stat_codes);
//        error = checkUnderVoltageStatus(undervoltagestatus,stat_codes);
//        error = checkOverVoltageStatus(undervoltagestatus, stat_codes);
        error = readInternalTemp(ITMP, stat_codes);
        error = readCellValue(cell_codes);
      if(pflag < 25)
       {
        if (error == -1||error_t==-1)
        {
         pflag = pflag + 1;
        }
        else
        {
          pflag = 0; 
          print_cells();
           print_aux();
           Serial.println(error_t);    
        }
        delay(200);
       }
      else
       {
        //digitalWrite(3,HIGH);
        //digitalWrite(49,LOW);
        print_cells();
         print_aux();  
        pflag = pflag - 1;
       }
    }
      
}


void init_cfg()
{
  for (int i = 0; i<TOTAL_IC; i++)
  {
    tx_cfg[i][0] = 0xFE;
    tx_cfg[i][1] = 0x00 ;
    tx_cfg[i][2] = 0x00 ;
    tx_cfg[i][3] = 0x00 ;
    tx_cfg[i][4] = 0x00 ;
    tx_cfg[i][5] = 0x00 ;
  }
}
int t;
void print_cells()
{
   int x;
   int avg;
   sump = 0;
   sumr = 0;
   int sumu=0;
  for (int current_ic = 0 ; current_ic < TOTAL_IC; current_ic++)
  {
    uflag = 0;
    sv=0;
    rflag = 0; 
    for (int i=0; i<10; i++)
    {
     x =  cell_codes[current_ic][i]*0.1;
     sv = sv + x;
     if(x==0)
     {
      Serial.print("0000");
     }
     else
     {
     Serial.print(x);
     Serial.print(' ');
     }
     /*if(x>=4200)
        {
          uflag=1;
        }*/
      ignore =1;
     if (current_ic==5)   //*****WHY??********//
      {
        ignore=0;
      }
      else
      {
        ignore=1;
      }
     if(x>=4200)     //**It might be an issue in Charging?**
      {
        ignore=0;
      }
     else if(x<3100 && x>=2800)
      {
        if (ignore ==0)
        {
          t=1;
        }
        else
        {
          sum_low_fault +=1;
          // uflag=1;
        }
      }
     else if(x<2800)
      {
        if (ignore ==0)
        {
          t=1; 
        }
        else
        {
          sum_critical_fault += 1;
          Serial.println(sum_critical_fault);
        }
      }
    }
    Serial.print(sv);
    if (sum_low_fault > 5)
        low_stack += 1;
    if (sum_critical_fault >2)
        uflag =1;
        
    if(uflag==1)
    {
     Serial.print(1);
    }
    else if(sump >=25)
    {
     Serial.print(2);
    }
    else if(sumr>=10)
    {
     Serial.print(3);
    }
    else
     Serial.print(0);
      
    if(current_ic < 10)
    {
     Serial.print(0);
    }
    Serial.println(current_ic);
    delay(80); 
    sump = sump + pflag;      //********CLEAN UP THIS*******//
    sumr = sumr + rflag;      //***********PART?*************//
    /*if (low_stack > 3)
    {
        uflag = 1;
    }*/
  if(/*sumr >= 10 || sump >=25 ||*/ uflag==1 )
  {
   
   
       // digitalWrite(3,HIGH);
        digitalWrite(49,LOW);
        Serial.print("fault");
  }    
  else
  {
   
        digitalWrite(49,HIGH);
        //digitalWrite(3,LOW); 
  }
sum_low_fault = 0;
}
if (low_stack > 3)
{
  uflag = 1;
}
sum_critical_fault = 0;  

}
void print_aux()
{
  
  for(int current_ic =0 ; current_ic < TOTAL_IC; current_ic++)
  {
    Serial.print(" IC ");
    Serial.print(current_ic+1,DEC);
    for(int i=0; i < 5; i++)
    {
      Serial.print(" GPIO-");
      Serial.print(i+1,DEC);
      Serial.print(":");
      Serial.print(aux_codes[current_ic][i]*0.0001,4);
      Serial.print(",");
    }
     Serial.print(" Vref2");
     Serial.print(":");
     Serial.print(aux_codes[current_ic][5]*0.0001,4);
     Serial.println();
  }
  Serial.println(); 
}


//*****************WHERE IS THE CONVERSION?*********//

/*
void serial_prin`s  xdc t_hex(uint8_t data)
{
  if (data< 16)
  {
    Serial.print("0");
    Serial.print((byte)data,HEX);
  }
  else
    Serial.print((byte)data,HEX);
}
*/

       
