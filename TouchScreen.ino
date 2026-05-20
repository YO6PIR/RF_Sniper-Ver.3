/*--------------- TOUCH SCREEN in mode normal -------------------------------------------*/
void  TouchEventsRegular(){ 
  TS_Point p = ts.getPoint(); 
  if (p.z > MINPRESSURE){         
      touchX = 320 - (320*(p.x-X1))/(X0-X1);
      touchY = 240 - (240*(p.y-Y1))/(Y0-Y1);
      touchP = p.z;
      loopTime = millis();      
   // TouchDebug();                                                
//*********** Ce se executa daca se apasa touch-ul in ecranul principal******************     
      if (Screen == 0){
          if (MainScreenOption == 0){     /*Optiunea Zero = Ecranul de lucru Focus pe [POW,SWR sau SNA] */
            //*ce se executa daca se apasa in interior ecranu GRID **************
            if ((touchX>gridOffsetX)&&(touchX < gridOffsetX + 300) && (touchY > gridOffsetY) && (touchY < gridOffsetY + GridHeight)){
                  loopTime = millis(); //contor timer
                  while (ts.touched()){ /* Apasare lunga pe touch screen */
                      if (millis() > (loopTime + 300)){ //timpul de apasare > 600ms
                        if ((touchY > gridOffsetY) && (touchY < gridOffsetY + 20) && ((Mode == 0) || (Mode == 2))) { /*Apasare lunga pe linia de sus REFERENCE*/
                                option_ref(touchX, touchY); 
                                NOmarker = true;    //nu afisa Markerul                                
                        }
                        else if ((touchX>gridOffsetX+145)&&(touchX<gridOffsetX+155)&&(touchY>gridOffsetY+30))  {
                    
                          Keypad(); /*Apasare lunga pe linia de mijloc*/  
                        }
                        else   { //* apasare Lunga oriunde pe ecranul GRID *  
                          MainScreenOption = 1;  
                          MarkerAct=0; 
                          xSel = touchX - gridOffsetX;
                           freqSel = working_center - (working_span/2) + (xSel * (working_span/300));    //frecventa punctata pe ecran      
                          Marker(xSel);                                                  
                        }
                        NOtouch();   
                         
                        StartScan=true ;   //incepe scanarea dela zero                                                        
                      }
                  } 
               
                 //***************** Daca se apasa oriunde pe Grid Screen in afara centrului ecran...*****************  
                      if(MainScreenOption != 1)MarkerAct^=1;   //toggle marker
                      if (KeypadFlag)MarkerAct= true;          //la iesire din tastatura activeaza stergere marker
                      xSel = touchX - gridOffsetX; 
                      if(!KeypadFlag){                      
                       
                        if(!NOmarker && !MarkerAct){  //daca nu este interzis de alta conditie afiseaza Markerul pe ecran
                          freqSel = working_center - (working_span/2) + (xSel * (working_span/300));    //frecventa punctata pe ecran               
                          if(MainScreenOption!=2) Marker(xSel);   
                        }
                        else Marker(0);
                      }
                      KeypadFlag=false;                           
            }
            //**** Ce se executa daca se apasa in afara ecranului GRID pe Focus [VOB,SWR,SNA] ***                     
            //*******  Selectie scurta pe litera [X] Bottom in screen ***************************
            else if ((touchY>(gridOffsetY + GridHeight + 25)&&(touchX<30))) {    
                  Screen = 1;
                  Templates();        
            }
            //*******  Selectie scurta pe litera [Y] Bottom Grid ***************************
            else if((touchY>(gridOffsetY + GridHeight)&&(touchX<195)&&(touchX>145))){            
               option_scala(); 
            }              
            //* Schimbare moduri de lucru selectie scurta pe [SNA, SWR, VOB]*********************
            else if(touchX>270 && touchY<20){
                 Focus=0;
                 PushRegular();
            }
            //* Selectie scurta pe YO6PIR -> Mode GRID-DIP Meter *********************
            else if(touchX<80 && touchY<10){
                 Focus=1;
                 GridDipMeter();
            }
            //*******  Selectie scurta pe SPAN VALUES in Bottom screen *************************
            else if((touchY>(gridOffsetY + GridHeight)&&(touchX<130)&&(touchX>40))){            
               Focus=2;
               Keypad();
            }
            //********* Selectie scurta pe CAL in coltul dreapta jos ***************************
            else if((touchY>(gridOffsetY + GridHeight)&&(touchX>260))){
              Focus=2;
              NOtouch();
              //calibrareZero_dBm();    //  executa calibrarea la zero dBm
              // GridScreenDivs();
            }
          }
          //***** Optiunea 1 = Punctare pe ecran in WorkSpace *********************************           
          else if (MainScreenOption == 1){    
                  if (freqSel>0){ 
                  working_center = freqSel;
                  if (working_span/2 > working_center) working_span =  working_center * 2;
                  }
                  MainScreenOption = 0; MarkerAct=1;
                  tft.fillRect(3, gridOffsetY, 315, 200, BLACK);                        //Refresh Screen Black
                 GridScreenDivs();
                 GridScreenTitles();
          }
        //* Optiunea 2 Banda cu valorile de REFERINTA ale graficului sus * Versiunea AI-OK  *
          else if (MainScreenOption == 2){    
          int aux_offsetY = 33;
          //* Verificăm dacă touch-ul este pe panoul vertical, folosind panelX salvat*
          if ((touchX > panelX) && (touchX < panelX + 40) &&
              (touchY > aux_offsetY) && (touchY < aux_offsetY + 10*18)) {
              int ySel = (touchY - aux_offsetY) / 18;  // fiecare căsuță la pas de 18 px
              working_ref = 15 - ySel*5;  
          } 
          // *Resetare și reafișare grid*
          MainScreenOption = 0; MarkerAct=1;
          StartScan = true;                                         
          tft.fillRect(3, gridOffsetY, 315, 200, BLACK);             
          GridScreenDivs();                                          
          GridScreenTitles();                                        
          NOmarker = false;                   
      }

    //* Optiunea 3 cu valorile pt SCALA graficului stg (tnx AI) OK fara High-Light ****
    else if (MainScreenOption == 3) {      
      //* Verifică dacă touch-ul e în zona scalei*
      if ((touchX >= 30) && (touchX < 65) &&  // latimea scalei
        (touchY >= 35) && (touchY <= 212)) {

        //* Calculează indexul dalei apăsate*
        int yOffset = touchY - 35;          // distanța de la marginea superioară
        int yIndex = yOffset / 18;           // 0..9
        if (yIndex > 9) yIndex = 9;

        multiPrev = multiplier;

        // Dalele sus→jos: 1..10
        ySel = yIndex;                       // 0 sus → dala 1, 9 jos → dala 10
        working_scale = 10-ySel;            // 1 sus, 10 jos
      }

    MainScreenOption = 0; 
    MarkerAct = true;
    StartScan = true;                       // începe scanarea de la zero

    tft.fillRect(3, gridOffsetY, 315, 200, BLACK);  // resetează ecranul
    GridScreenDivs();
    GridScreenTitles();
  }
   NOtouch();         
 } //*Aici se termina optiunile din ecranul Principal*/
        
/****************** Ce se executa in ecranul auxiliar de selectie TEMPLATES tnx AI *********/
  else if (Screen == 1) {                                   

  // --- Dimensiuni layout ---
  const int X0 = 10;          // margine stânga
  const int Y0 = 35;          // margine sus
  const int BTN_W = 80;       // lățime buton
  const int BTN_H = 25;       // înălțime buton
  const int SPACE_X = 10;     // distanță orizontală între butoane
  const int SPACE_Y = 5;      // distanță verticală între butoane

  // --- Calculul coloanei și a rândului ---
  if (touchX >= X0 && touchX < 320 && touchY >= Y0 && touchY < 240) {
    xSel = (touchX - X0) / (BTN_W + SPACE_X);
    ySel = (touchY - Y0) / (BTN_H + SPACE_Y);
  } else {
    xSel = -1;
    ySel = -1;
  }

  tft.setFont(&FreeSerifBold12pt7b);
  tft.setTextSize(1);

  // Zona stângă – BANDS
  if ((xSel >= 0) && (xSel < 2) && (ySel >= 0) && (ySel < 5)) {
    // Restaurare buton anterior
    if (xSelFPrev < 10) {
      tft.fillRoundRect(X0 + (BTN_W + SPACE_X) * xSelFPrev,
                        Y0 + (BTN_H + SPACE_Y) * ySelFPrev,
                        BTN_W, BTN_H, 4, GRIDCOLOR);
      tft.drawRoundRect(X0 + (BTN_W + SPACE_X) * xSelFPrev,
                        Y0 + (BTN_H + SPACE_Y) * ySelFPrev,
                        BTN_W, BTN_H, 4, CYAN);
      tft.setCursor(X0 + 10 + (BTN_W + SPACE_X) * xSelFPrev,
                    Y0 + 20 + (BTN_H + SPACE_Y) * ySelFPrev);
      tft.setTextColor(CYAN);
      tft.print(Bands[5 * xSelFPrev + ySelFPrev]);
    }

    // Highlight buton curent
    tft.fillRoundRect(X0 + (BTN_W + SPACE_X) * xSel,
                      Y0 + (BTN_H + SPACE_Y) * ySel,
                      BTN_W, BTN_H, 4, BLUE);
    tft.drawRoundRect(X0 + (BTN_W + SPACE_X) * xSel,
                      Y0 + (BTN_H + SPACE_Y) * ySel,
                      BTN_W, BTN_H, 4, WHITE);
    tft.setCursor(X0 + 10 + (BTN_W + SPACE_X) * xSel,
                  Y0 + 20 + (BTN_H + SPACE_Y) * ySel);
    tft.setTextColor(WHITE);
    tft.print(Bands[5 * xSel + ySel]);

    xSelFPrev = xSel;
    ySelFPrev = ySel;
  }
  // Zona dreaptă – SPANS
  else if ((xSel >= 2) && (xSel <= 3) && (ySel >= 0) && (ySel < 6)) {
    // Restaurare buton anterior
    if (ySelSPrev < 10) {
      tft.fillRoundRect(195, Y0 + (BTN_H + SPACE_Y) * ySelSPrev,
                        110, BTN_H, 4, GRIDCOLOR);
      tft.drawRoundRect(195, Y0 + (BTN_H + SPACE_Y) * ySelSPrev,
                        110, BTN_H, 4, CYAN);
      tft.setCursor(198, Y0 + 20 + (BTN_H + SPACE_Y) * ySelSPrev);
      tft.setTextColor(GREEN);
      tft.print(Spans[ySelSPrev]);
    }
    // Highlight curent
    tft.fillRoundRect(195, Y0 + (BTN_H + SPACE_Y) * ySel,
                      110, BTN_H, 4, BLUE);
    tft.drawRoundRect(195, Y0 + (BTN_H + SPACE_Y) * ySel,
                      110, BTN_H, 4, WHITE);
    tft.setCursor(198, Y0 + 20 + (BTN_H + SPACE_Y) * ySel);
    tft.setTextColor(WHITE);
    tft.print(Spans[ySel]);

    xSelSPrev = xSel;
    ySelSPrev = ySel;
  }
  // Taste funcționale OK și Return
  else if (touchY >= 190) {

    // *Buton OK*
    if ((touchX >= 20) && (touchX < 20 + 60) && (touchY >= 190) && (touchY < 190 + 35)) {
      // Highlight
      tft.fillRoundRect(20, 190, 60, 35, 4, BLUE);
      tft.drawRoundRect(20, 190, 60, 35, 4, WHITE);
      tft.setCursor(30, 213);
      tft.setTextColor(WHITE);
      tft.print("OK");

      // Acțiune OK
      if ((xSelFPrev < 2) && (ySelFPrev < 5))
        working_center = BandsNum1[xSelFPrev * 5 + ySelFPrev];
      if (ySelSPrev < 5)
        working_span = SpanNum1[ySelSPrev];
      if (ySelSPrev == 5) {
        working_center = 15000000;
        working_span = 28000000;        
      }

      delay(120); // vizualizare
      // Restaurare culoare statica
      tft.fillRoundRect(20, 190, 60, 35, 4, GREEN);
      tft.drawRoundRect(20, 190, 60, 35, 4, CYAN);
      tft.setCursor(30, 213);
      tft.setTextColor(NAVY);
      tft.print("OK");
      MarkerAct=1;
      GridScreen();
      Screen = 0;
      NOtouch();
      return;
    }

    // Buton Return
    if ((touchX >= 100) && (touchX < 100 + 80) && (touchY >= 190) && (touchY < 190 + 35)) {
      tft.fillRoundRect(100, 190, 80, 35, 4, BLUE);
      tft.drawRoundRect(100, 190, 80, 35, 4, WHITE);
      tft.setCursor(102, 213);
      tft.setTextColor(WHITE);
      tft.print("Return");
      delay(120);
      
      // Restaurare culoare statica
      tft.fillRoundRect(100, 190, 80, 35, 4, RED);
      tft.drawRoundRect(100, 190, 80, 35, 4, CYAN);
      tft.setCursor(102, 213);
      tft.setTextColor(WHITE);
      tft.print("Return");
      MarkerAct=1;
      GridScreen();
      Screen = 0;
      NOtouch();
      return;
    }
  }
  // finalizează detectarea touch si asteapta eliberarea pointerului
  NOtouch();
  } //*Aici se termina optiunile din ecranul TEMPLATES***
 } //*Final sesizare apasare touch peste MINIMPRESSURE***
}

/*------------- Rutina de evenimente Touch in mod SETTINGS ---------------------------------*/
void TouchEventSettings(){ 
    
   TS_Point p = ts.getPoint(); 
    if (p.z > MINPRESSURE){         
      touchX = 320 - (320*(p.x-X1))/(X0-X1);
      touchY = 240 - (240*(p.y-Y1))/(Y0-Y1);
      touchP = p.z;
      loopTime = millis(); 
    //  TouchDebug();                                                  //Debug Touch Position     
    if (!Touch_selected && !Cal_selected && ! LO_selected && !SWR_selected){   

    //****************************Apasare pe dala [Touch Calibration] *************************
    if(touchY<85 && touchY>55){
      MeniuSetingSelectat = 0;     //Touch Calibration
      MenuSetingEvident();
            loopTime = millis();                                           //contor timer
                  while (ts.touched()){ //*Apasare lunga pe touch screen*                                                            //daca se tine apasat touch
                      if (millis() > (loopTime + 500)){                   //timpul de apasare este lung de 600ms
                       // PushSettings();    
                        Touch_selected = true;                //Selectie meniu calib. touch screen
                        TouchCalScreen();                     //afiseaza ecranul cu tinte  
                        NOtouch();                                        //reset touch
                      }
                  } 
    }
    //*Apasare pe dala [Si5351 Calibration] *
    if(touchY<123 && touchY>93){
      MeniuSetingSelectat = 1;      //Si5351 Calibration
      MenuSetingEvident();
            loopTime = millis();                                           //contor timer
                  while (ts.touched()){ //*Apasare lunga pe touch screen*                                                            //daca se tine apasat touch
                      if (millis() > (loopTime + 500)){                  //timpul de apasare este lung de 600ms
                      //PushSettings();    
                      Cal_selected = true;
                      CalFactorSetting();
                      NOtouch();                                       //reset touch
                      }
                  } 
    }
    //****************************Apasare pe dala [LO Calibration] ************************
    if(touchY<160 && touchY>131){
      MeniuSetingSelectat = 2;      //XTALL Calibration
      MenuSetingEvident();
            loopTime = millis();                                          //contor timer
                  while (ts.touched()){ //*Apasare lunga pe touch screen*                                                            //daca se tine apasat touch
                      if (millis() > (loopTime + 500)){                  //timpul de apasare este lung de 600ms
                        //PushSettings();    
                          LO_selected = true;
                          LOSetting();
                          NOtouch(); 
                      }
                  } 
    }
    //***************Apasare pe dala [SWR Offset Calibration] *****************************
        if(touchY<197 && touchY>167){
      MeniuSetingSelectat = 3;      //SWR Offset Calibration
      MenuSetingEvident();
            loopTime = millis();                                          //contor timer
                  while (ts.touched()){ //*Apasare lunga pe touch screen*                                                            //daca se tine apasat touch
                      if (millis() > (loopTime + 500)){                 //timpul de apasare este lung de 600ms
                        //PushSettings();    
                        SWR_selected = true;
                        Afisare_SWRsettings();
                        NOtouch();                                      //asteapta eliberarea touch-ului
                      }
                  } 
    }
    //************** Apasare pe dala [EXIT] din meniul SETTINGS **************************
    if(touchY<232 && touchY>204){
      MeniuSetingSelectat = 4;      
      MenuSetingEvident();
     
            loopTime = millis();                                         //contor timer
                  while (ts.touched()){ //*Apasare lunga pe touch screen*                                                            //daca se tine apasat touch
                      if (millis() > (loopTime + 500)){                //timpul de apasare este lung de 600ms
                        MeniuSetingsActiv = false;
                         tft.fillScreen(NAVY);  
                         return;     // → repornește curat
                         
                      }
                  } 
        }
    }
     //******************** Apasare pe butonul [EXIT] din sub-meniurile de calibrare *****************
    else if(Cal_selected ||  LO_selected || SWR_selected){
    if(touchX<250 && touchX>180 && touchY<180 && touchY>152){       
            sel_2=0;
             selectOption1(sel_2 + 1);
                        Cal_selected=false;LO_selected=false;SWR_selected=false;
                       
                         ResetSi5351();  
                         NOtouch();                                             //reset touch
                        Settings();  
                         
      }
      //********************* Apasare pe butonul [SAVE] din sub-meniurile de calibrare *****************
      else if(touchX<140 && touchX>70 && touchY<180 && touchY>152){        
            loopTime = millis();                                                           
            sel_2=1;
             selectOption1(sel_2 + 1);
                  while (ts.touched()){ //*Apasare lunga pe touch screen*    
                    if(MeniuSetingSelectat==3){
                     // calibrateSWR(9);  
                    }
                      if (millis() > (loopTime + 500)){                     //timpul de apasare este lung de 600ms
                   switch (MeniuSetingSelectat){
                      case 0: 
                            break;
                      case 1:
                              eep_romadd = 0x010;                           
                              eep_cal_factor=cal_factor;                       
                              eprom.put((eep_romadd),eep_cal_factor);
                              break;          
                     case 2:
                             eep_LO = LO;
                             eprom.put((EEP_XTAL),eep_LO);                             
                             break;
                     case 3:
                            eep_romadd = 0x010;    
                            eep_swr_offset=SWR_offset;
                            eprom.put(eep_romadd+4,eep_swr_offset);                
                            break;         
                   }       
                       Cal_selected=false;LO_selected=false;SWR_selected=false;
                        Settings();     //revenire la meniul Settings  
                         NOtouch();     //asteapta eliberare touch                                    
                      }
                  } 
      }
      //**************** Apasare pe [NUMERE MARI] din meniurile calibrare ************************
      else if(touchX<230 && touchX>100 && touchY<120 && touchY>90){        
            sel_2=2;
             selectOption1(sel_2 + 1);
                        Keypad();    
                        NOtouch();                                           //reset touch
      }
    }
    if(Touch_selected){//* Meniul de calibrare al Touch-Screen-ului este  selectat*
    //**************** Apasare pe punctul + [TINTA CENTRALA] din meniul de cal-touch ************
       if(touchX<170 && touchX>150 && touchY<130 && touchY>110){       
           loopTime = millis();                                                           
         
            while (ts.touched()){ //*Apasare lunga pe touch screen*        
               if (millis() > (loopTime + 3000)){ 
                  X0 = X0_NEW;
                    X1 = X1_NEW;
                    Y0 = Y0_NEW;
                    Y1 = Y1_NEW; 
                    //* Memoreaza noile valori de calibrare ale touch-screen  *
                    eep_romadd = 0x010;     
                    eep_X0=X0;
                    eprom.put(eep_romadd+8,eep_X0);
                    eep_X1=X1;
                    eprom.put(eep_romadd+12,eep_X1);
                    eep_Y0=Y0;
                    eprom.put(eep_romadd+16,eep_Y0);
                    eep_Y1=Y1;
                    eprom.put(eep_romadd+20,eep_Y1);
                                            
                    tft.setFont(&FreeMonoBold12pt7b);
                    tft.setTextSize(2);
                    tft.setTextColor(ORANGE);               
                    tft.setCursor(75,130); tft.print("SAVED");      
               }
            }
             Touch_selected=false;
             Settings();     
       }
     }    
   }      
}

/*----------- Rutina de asteptare eliberare pointer cu dublu clik ------------*/
void NOtouch(){
  long auxtime = millis();
  bool touched = true;
  do {
    TS_Point p = ts.getPoint();   
    if (p.z < 10 ) touched=false;        
    } while (touched);      
    pushTime = millis() - auxtime;                 
}

/*------------------ Pop-UP meniu [Ref^] pe Grid in partea de sus ----------------*/
void option_ref(int touchX, int touchY){
    MainScreenOption = 2;
    tft.setFont(&FreeMonoBold9pt7b);
    tft.setTextSize(1);
    tft.setTextColor(WHITE);

    int aux_offsetY = 33;

    // Calculăm X-ul panoului și salvăm în panelX
    if (touchX > 50 + gridOffsetX) {
        panelX = touchX - 50;   // panoul apare în stânga cursorului
    } else {
        panelX = gridOffsetX;   // marginea stângă a grid-ului
    }

    // Desenăm panoul vertical
    tft.fillRoundRect(panelX - 5, aux_offsetY - 16, 50, 197, 3, NAVY);
    tft.drawRoundRect(panelX - 5, aux_offsetY - 16, 50, 197, 3, WHITE);
    tft.setCursor(panelX, aux_offsetY - 4);
    tft.print("Ref^");

    for (int n = 0; n < 10; n++){   
        tft.drawRoundRect(panelX, aux_offsetY + (n*18), 40, 16, 3, YELLOW);
        tft.fillRoundRect(panelX + 1, aux_offsetY + 1 + (n*18), 38, 14, 3, BLUE);
        tft.setCursor(panelX + 3, aux_offsetY + 13 + (n*18));
        if((15 - n*5) > 0) tft.print("+");
        tft.print(15 - n*5); 
    }    
}

/*---------------- Pop-UP meniu [SCALA] la apasare Tasta [Y] ----------------*/
void option_scala()
{
  MainScreenOption = 3;
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);

  int aux_offsetX = 30;
  int aux_offsetY = 33;
  
  tft.fillRoundRect(aux_offsetX - 5, aux_offsetY - 16, 50, 197, 3, NAVY);
  tft.drawRoundRect(aux_offsetX - 5, aux_offsetY - 16, 50, 197, 3, WHITE);
  tft.setCursor(aux_offsetX-3 , aux_offsetY -4);
  if(Mode!=1)tft.print("/div");
  else tft.print("SWR^"); 
  
  for (int n = 0; n < 10; n++){   
        tft.drawRoundRect(aux_offsetX, aux_offsetY + (n* 18), 40, 16, 3, YELLOW);
        tft.fillRoundRect(aux_offsetX + 1, aux_offsetY + 1 + (n* 18), 38, 14, 3, BLUE);
        tft.setCursor(aux_offsetX + 3 , aux_offsetY + 13 + (n* 18));       
        tft.print(" ");tft.print(10 - n);         
    }  
}     

/*------------ Rutina "DEBUG" de afisare a coordonatelor instant touch ---------------*/
void TouchDebug(){   
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(ORANGE, NAVY); 
    tft.setCursor(250,50);
    //dtostrf(float(touchX),4,0,c);
    tft.print(int(touchX));tft.print("  X");
    tft.setCursor(250,70);
    //dtostrf(float(touchY),4,0,c);
    tft.print(int(touchY));tft.print("  Y");
}
