/*------------------ START SETTING SCREEN --------------------------*/
void Settings(){  
  //*Citeste valorile actuale salvate in EEprom la intrarea in setari
  InitEEprom();   
  tft.fillScreen(BLACK);  
  tft.setFont(&FreeSerifBold12pt7b); tft.setTextSize(1);
  
  tft.drawRect(1,1,319,239, BLUE);
  tft.drawRect(3,3,315,235, YELLOW);
  tft.drawRect(5,5,311,231, BLUE);

  tft.setTextColor(CYAN);
  tft.setCursor(50,40); tft.println("SETTINGS MENU");
  
  tft.setTextColor(GREEN);     
  tft.setCursor(20, 80);  tft.println("Touch-Screen Calibration"); 
  tft.setCursor(20,115);  tft.println("Si5351 Xtall Calibration");  
  tft.setCursor(20,150);  tft.println("Local Oscilator Frequency");
  tft.setCursor(20,185);  tft.println("SWR Return Loss Adjust");  
  tft.setCursor(120,220); tft.println("EXIT");  
    NOtouch();
  MeniuSetingsActiv = true;
  MenuSetingEvident();
  ts.begin(); ts.setRotation(1);        //activare Touch-Screen
  loopTime = millis();                  //reset Timer
  VinFiltered = 0;
  do{ //* Bucla activa pana cand MeniuSetingsActiv=true *
    TouchEventSettings();  //Rutina de scanare Touch in mod SETTINGS
    
    if (Touch_selected){
            if (ts.touched()){
                 TS_Point point = ts.getPoint(); 
                 if (point.z > MINPRESSURE){    
                     touchX = 320 - (320*(point.x-X1))/(X0-X1);
                     touchY = 240 - (240*(point.y-Y1))/(Y0-Y1); 
                     touchP = point.z;     
    
                     ACTx =  point.x;                 
                     ACTy =  point.y; 
                     
                     if (X0_NEW < point.x) X0_NEW = point.x;
                     if (X1_NEW > point.x) X1_NEW = point.x;
                     if (Y0_NEW < point.y) Y0_NEW = point.y;
                     if (Y1_NEW > point.y) Y1_NEW = point.y;
    
                   // TouchCoordonate(); 
                 }   
            tft.setFont(); tft.setTextSize(2);
            tft.setTextColor(WHITE, NAVY);

            tft.setCursor(50,20); tft.print("point.X0:");tft.print(String(X0_NEW)+"   "); 
            tft.setCursor(50,40); tft.print("point.Y0:");tft.print(String(Y0_NEW)+"   "); 
            tft.setCursor(50,60); tft.print("point.X1:");tft.print(String(X1_NEW)+"   ");        
            tft.setCursor(50,80); tft.print("point.Y1:");tft.print(String(Y1_NEW)+"   "); 
            }      
    }
       else if (LO_selected){     
        if (millis() > loopTime + 250){    
            working_dBm= citire_dBm_vobu();
            tft.setFont(); tft.setTextSize(2);
            tft.setCursor(20,48);
            tft.setTextColor(CYAN, BLACK);  tft.print("OUTPUT level:"); 
            dtostrf(float(working_dBm/10),5,1,c);
            tft.setTextColor(GREEN, BLACK);   
            tft.print(c); 
            tft.setTextColor(CYAN, BLACK); 
            tft.print(" dBm");
            loopTime = millis();
        }
    }  
    else if (SWR_selected){
              Vin = analogRead(PB1);              
              if (millis() > loopTime + 250){    
                 float swr = swr_measure();
                  swr = filtered_swr(swr);
                  
                  tft.setFont();  tft.setTextSize(2);
                  tft.setTextColor(GREEN, BLACK);   
                  tft.setCursor(160,48);
                  dtostrf(swr,4,2,c);
                  if(swr>0)tft.print(c);else tft.print(">>>>   "); 
                  //VinFiltered = 0;
                  loopTime = millis();
              }
    }            
  }while (MeniuSetingsActiv);
}

/*---------- Evidentiaza tastele apasate in SETTINGS ------------------*/ 
void MenuSetingEvident(){
  tft.drawRect(15, 56 + MeniuSetingSelectatPrev * 35, 290, 35, BLACK);
  tft.drawRect(15, 56 + MeniuSetingSelectat * 35, 290, 35, YELLOW);
  MeniuSetingSelectatPrev = MeniuSetingSelectat;  
}
/*------ Meniul SETTINGS deseneaza tastele [EXIT[ si [SAVE] ---------*/
void selectOption1(int opc){
  int ExitButtonColor = BLUE;
  int SaveButtonColor = BLUE;
  int ExitTextColor = BLACK;
  int SaveTextColor = BLACK;

  switch (opc){
      case 0:
          break;
      case 1:
          ExitButtonColor = ORANGE;
          ExitTextColor = WHITE;
          break;
       case 2:  
          SaveButtonColor = ORANGE;
          SaveTextColor = WHITE;
          break;
  }  
  tft.fillRoundRect(70, 150, 70, 30, 3, SaveButtonColor);
  tft.fillRoundRect(180, 150, 70, 30, 3, ExitButtonColor);
  tft.setFont(&FreeSerifBold12pt7b);  tft.setTextSize(1);
  tft.setTextColor(SaveTextColor);
  tft.setCursor(80,170); tft.print("Save");
  tft.setTextColor(ExitTextColor);
  tft.setCursor(195,170); tft.print("Exit"); 
}

/*----------- Deseneaza ecran de calibrare TOUCHSCREEN ---------------------*/
void TouchCalScreen(){
  tft.fillScreen(NAVY);  
  tft.fillCircle(3, 3, 2, WHITE);
  tft.fillCircle(3, 237,2, WHITE);
  tft.fillCircle(317, 3, 2, WHITE);
  tft.fillCircle(317, 237, 2, WHITE);    
  tft.drawCircle(160, 120, 5, YELLOW);  
  tft.drawLine(150,120,170,120, YELLOW);
  tft.drawLine(160,110,160,130, YELLOW);
  
  tft.setFont(); tft.setTextSize(2);
  
  tft.setTextColor(WHITE, NAVY);                
  tft.setCursor(50,20); tft.print("point.X0:");tft.print(String(X0_NEW)+"   "); 
  tft.setCursor(50,40); tft.print("point.Y0:");tft.print(String(Y0_NEW)+"   "); 
  tft.setCursor(50,60); tft.print("point.X1:");tft.print(String(X1_NEW)+"   ");        
  tft.setCursor(50,80); tft.print("point.Y1:");tft.print(String(Y1_NEW)+"   "); 
  
  tft.setTextColor(GREEN, NAVY);
  tft.setCursor(50,140); tft.print("actual X0 :"); tft.print(String(X0));    
  tft.setCursor(50,160); tft.print("actual Y0 :"); tft.print(String(Y0));      
  tft.setCursor(50,180); tft.print("actual X1 :"); tft.print(String(X1));  
  tft.setCursor(50,200); tft.print("actual Y1 :"); tft.print(String(Y1));    
   
  tft.setTextColor(YELLOW, NAVY);
  tft.setCursor(40,113); tft.print("Press ->");                               
  tft.setCursor(190,113); tft.print("to EXIT");                               
}

/*----------- Ecran de calibrare Si5351_Xtall calibration -----------*/
void CalFactorSetting(){
  
  tft.fillScreen(BLACK);
  tft.setFont(&FreeSerifBold12pt7b); tft.setTextSize(1);
  
  tft.drawRect(1,1,319,239, BLUE);
  tft.drawRect(3,3,315,235, YELLOW);
  tft.drawRect(5,5,311,231, BLUE);
  tft.setTextColor(CYAN);
  tft.setCursor(20,30); tft.println("Si5351 Xtall Calibration");
  tft.setCursor(15,60); tft.println("Check 10.000.000 Hz output");

  tft.setFont(&FreeSerifBold12pt7b); tft.setTextSize(2);
  tft.setTextColor(GREEN);   
  tft.setCursor(70,120); tft.print(long(cal_factor)); 
  
  selectOption1(1);
 
  si5351.set_correction(cal_factor);
           
  si5351.output_enable(SI5351_CLK0, 1);
  freq = 10000000;
  si5351_1();
}

/*----------- Ecran de calibrare LO Frequency Calibration --------*/
void LOSetting(){
  tft.fillScreen(BLACK);
  tft.setFont(&FreeSerifBold12pt7b); tft.setTextSize(1);
  
  tft.drawRect(1,1,319,239, NAVY);
  tft.drawRect(3,3,315,235, YELLOW);
  tft.drawRect(5,5,311,231, NAVY);
  tft.setTextColor(CYAN);
  tft.setCursor(15,30); tft.println(" LO Frequency Calibration");

  tft.setFont(&FreeSerifBold12pt7b); tft.setTextSize(2);
  tft.setTextColor(GREEN);   
  tft.setCursor(70,120); tft.print(long(LO));

  selectOption1(1);
  si5351.output_enable(SI5351_CLK0, 1);
  si5351.output_enable(SI5351_CLK1, 0);
  si5351.output_enable(SI5351_CLK2, 1);
  
  freq = 14000000;
  si5351_1(); 

  freq = 14000000 + LO;
  si5351_3(); 
}

/*------- Ecran de afisare SWR Return Loos Adjust -----------------*/
void Afisare_SWRsettings() {

  tft.fillScreen(BLACK);

  // --- rame ---
  tft.drawRect(1,1,319,239, NAVY);
  tft.drawRect(3,3,315,235, YELLOW);
  tft.drawRect(5,5,311,231, NAVY);

  // --- titlu ---
  tft.setFont(&FreeSerifBold12pt7b);
  tft.setTextSize(1);
  tft.setTextColor(CYAN);
  tft.setCursor(20,30);
  tft.println("SWR ReturnLoss Calibrate");

  // --- texte informative ---
  tft.setCursor(90,60);
  tft.print("SWR:");
  tft.setCursor(20,210);
  tft.print("Check SWR with 300 Ohms");

  // --- afisare offset ---
  tft.setTextSize(2);
  tft.setTextColor(GREEN);
  tft.setCursor(70,120);

  // offset dB = SWR_offset / 10
  dtostrf(float(SWR_offset) / 10.0, 6, 1, c);
  tft.print(c);

  tft.setTextSize(1);
  tft.print(" dB");

  // --- selector ---
  selectOption1(1);

  // --- pornire semnale ---
  si5351.output_enable(SI5351_CLK1, 1);
  si5351.output_enable(SI5351_CLK2, 1);

  // --- generare semnal pentru calibrare ---
  freq = 14200000;    // 14.200 MHz
  si5351_2();         // CLK1 = VFO
  freq += LO;         // adaugă offset mixer
  si5351_3();         // CLK2 = BFO/LO
}
