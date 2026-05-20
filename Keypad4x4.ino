/******************** Draw Numeric VIRTUAL KEYPAD ***********************/
void Keypad(){
  float Nomber_tastat=0;                         //initializare Numar tastat 
  uint32_t old_central_freq = working_center;    //salveaza temporar frecventa centrala
  uint32_t old_span_freq = working_span;         //salveaza temporar frecventa span
  uint32_t old_FreqGDO=FreqGDO;
  TS_Point q = ts.getPoint();
  
  int xOffset=(ScreenPortrait)?0:45;   
  int yOffset = (ScreenPortrait)?120:75;
  
  tft.fillScreen(BLACK);
  tft.setFont(&FreeMonoBold12pt7b);
  tft.setTextSize(1);

  tft.fillRoundRect(xOffset, yOffset-70, 230, 25, 4, WHITE);
  tft.setTextColor(BLUE);
  tft.setCursor(5+xOffset,yOffset-53);
   if(MeniuSetingsActiv == false){  //*daca rutina de calibrare NU este activa *   
    if(Focus==2) tft.print(" SPAN Frequency ");  
    else tft.print("Center Frequency");  
   }
   else{
    switch(MeniuSetingSelectat){
      case 0: break;
      case 1:
            Str="Si5351 CalFactor";
            break;
      case 2:
            Str="  LO Frequency  ";
            break;
      case 3:
            Str="  SWR OFFSET    ";
            break;
      case 4:
            break;                  
    }
    tft.print(Str);
   }
  tft.setTextColor(WHITE);

  for (int x=0; x<4; x++){
      for (int y=0; y<4; y++){
        if ((x != 3) || (y != 3)) tft.fillRoundRect(xOffset + 60*x, yOffset + 40*y, 50, 30, 4, BLUE );
          else tft.fillRoundRect(xOffset + 60*x, yOffset + 40*y, 50, 30, 4, RED);
        tft.setCursor(5+xOffset + 60*x, 20+yOffset + 40*y);
       
        if(((x+4*y)==3 || (x+4*y)==7) && MeniuSetingsActiv == true)tft.print("   ");
        else if((x+4*y)==11 && MeniuSetingsActiv == true)tft.print("+/-");
          else tft.print(VirtualKeypad[x + 4*y]);      
      }
  } 
  tft.fillRoundRect(xOffset, yOffset-30, 230, 25, 4, BLUE);
  tft.setTextColor(YELLOW, BLUE);
  tft.setFont(&FreeMonoBold12pt7b);
  tft.setCursor(25+xOffset, yOffset-15);  

byte ValoareReturnata;
int multiplicator=1;
bool mega=false, dot=false,sgn=false;    //indicator pt inscriptionare MHz,KHz,Hz,[.],[-] 
VirtualKeypadValue = "";
 NOtouch();
   for(;;) {
    ValoareReturnata = Touch2Nomber();
          if ((ValoareReturnata >= 46) && (ValoareReturnata <= 57)){
            if(ValoareReturnata==46 && !dot) dot=1; 
            else if((ValoareReturnata==46 && dot)||mega)continue;
            
            if (VirtualKeypadValue.length() < 9) {
            if (sgn && !VirtualKeypadValue.startsWith("-")) {
            VirtualKeypadValue = "-" + VirtualKeypadValue;
            }
            VirtualKeypadValue.concat(char(ValoareReturnata));
            }
            tft.fillRoundRect(xOffset, yOffset-30, 230, 25, 4, BLUE);   
            tft.setCursor(15 + ((16 * 15 - VirtualKeypadValue.length() * 15) / 2), yOffset-10
            );
           
            tft.print(VirtualKeypadValue);
          }
    //********** Rutine incluse/excluse din Calibrare*******************          
    if(MeniuSetingsActiv == false){  //*daca rutina de calibrare NU este activa *    
           if(ValoareReturnata==77 && !mega){
            multiplicator=1000000;          
            tft.print(" ");
            tft.print(char(ValoareReturnata)); //"MHz"
            tft.print("Hz");
            mega=true;dot=true;
          }
          else if (ValoareReturnata==75 && !mega) {
            multiplicator=1000;
            tft.print(" ");
            tft.print(char(ValoareReturnata));  //"KHz"
            tft.print("Hz");
            mega=true;dot=true;
          }
          else if(ValoareReturnata==32 && !mega){
            multiplicator=1;
            tft.print(" ");
            tft.print(char(ValoareReturnata));  // " Hz"
            tft.print("Hz");
            mega=true; dot=true;
          }
    }
    else{ //*daca rutina de calibrare ESTE Activa nu imprima MHz-KHz pe taste*  
            if(ValoareReturnata == 77) tft.print("   ");
            else if (ValoareReturnata == 75) tft.print("   ");
            else if(ValoareReturnata == 32){
            sgn=true;
            tft.setCursor(65+xOffset, yOffset-10); tft.print("-");
          }   
    }
          if (ValoareReturnata == 60) {   //*daca e apasata tasta [Del] *
            VirtualKeypadValue = ""; 
            multiplicator=1;              //reseteaza multiplicatorul
            mega=false; dot=false;        //reseteaza indicatorul de M,K,Hz
            tft.fillRoundRect(xOffset, yOffset-30, 230, 25, 4, BLUE);   
          }       
          else if  (ValoareReturnata == 62){//* Daca este apasata tasta [OK] *
            Nomber_tastat = atof(VirtualKeypadValue.c_str())* multiplicator;

          //******************** Calculeaza valoarea tastata daca este in Mode Regular ***************
           if(MeniuSetingsActiv == false){
            if(Nomber_tastat<500000) {
              if(MeniuGDO){
                working_center= old_FreqGDO * 1000UL;
              }
              else  working_center = old_central_freq; //Central Freq Min limitat la 500KHz          
            }
            else  {
                 if (Focus !=2) working_center = Nomber_tastat;
            }
            if (Focus==2 && Nomber_tastat>=1000) {      //SPAN Min limitat la 1KHz
              working_span = Nomber_tastat;
              Focus=0;
            }
            
            if (working_span/2 > working_center) working_span =  working_center * 2;
             
             if(MeniuGDO){
               return;
              }
              else {
                GridScreen();
              }
           }
           else{ //*********** Calculeaza valoarea tastata daca este in Mode Calibrare ***************
            switch(MeniuSetingSelectat){
               case 0:
                    break;
               case 1: 
                    cal_factor=Nomber_tastat;  
                    CalFactorSetting();

                    break;
               case 2:
                       LO=Nomber_tastat; 
                       LOSetting();
                       break;
               case 3:
                      SWR_offset=Nomber_tastat*10; 
                      Afisare_SWRsettings();
                       break;
               case 4:
                       break;            
            }               
           }  
              
          KeypadFlag=true;  
          StartScan=1 ;   //incepe scanarea dela zero
          MarkerAct=1;
             
          return;  //* Paraseste rutina de calcul*                   
          }
          NOtouch();                  
   } 
}

/*------------ Functia de conversie Touch → Number -----------------*/
int Touch2Nomber(){
  int xOffset = (ScreenPortrait)?0:45;
  int yOffset = (ScreenPortrait)?120:75;
  byte KeyNumber;
  byte KeyPush;
    
  for(;;){
      TS_Point q = ts.getPoint();
      KeyNumber = 0;
      KeyPush = 100;
      bool xTouch = false;
      bool yTouch = false;      
      
      if (q.z > MINPRESSURE){  
        if(ScreenPortrait){  
        //rotatie=1
        touchX = (240*(q.y - Y1)) / (Y0 - Y1);      // y devine axa X
        touchY = 320 - (320*(q.x - X1)) / (X0 - X1); // x devine axa Y inversată
        }
        else{//rotatie=0
          touchX = 320 - (320*(q.x-X1))/(X0-X1);
          touchY = 240 - (240*(q.y-Y1))/(Y0-Y1);  
        }
          if ((touchY > yOffset) && (touchY < (yOffset + 150)) && (touchX > xOffset) && (touchX < (xOffset + 230))){  
              for (int x=0; x<3; x++){
                  if ((touchX > (xOffset + 60*x)) && (touchX < (xOffset + 60*x + 50)))
                  {
                    KeyPush = x;
                    xTouch = true;
                  }
              }                     
              if ((touchX > (xOffset + 60*3)) && (touchX < (xOffset + 60*3 + 50))){
                KeyPush = 13;
                xTouch = true;
              }
              if (xTouch){
                  for (int y=0; y<4; y++){
                      if ((touchY > (yOffset + 40*y)) && (touchY < (yOffset + 40*y + 30))){
                        yTouch = true;
                        KeyPush += (3 * y);
                      }
                  }  
              }           
            }  
          if (xTouch && yTouch){
              KeyNumber = KeyPush + 49;                     //1,2,3,4,5,6,7,8,9
              if (KeyNumber == 58) KeyNumber = 60;       //DEL
              else if (KeyNumber == 59) KeyNumber = 48;     //0
              else if (KeyNumber == 60) KeyNumber = 46;     //"."  
              else  if (KeyNumber == 62) KeyNumber = 77;    //MHz
              else if (KeyNumber == 65) KeyNumber = 75;     //KHz
              else if (KeyNumber == 68) KeyNumber = 32;     //Hz
              else if (KeyNumber == 71) KeyNumber = 62;     //OK
                                     
            return KeyNumber;                
            } 
        }
    }   
}
