/*----------- Deseneaza Pagina START-PAGE ------------------------*/
void StartPage(){
  tft.fillScreen(BLACK);
  tft.setFont(&FreeSerifBold12pt7b);
  tft.setTextSize(2);
  
  tft.drawRect(1,1,319,239, NAVY);
  tft.drawRect(3,3,315,235, YELLOW);
  tft.drawRect(5,5,311,231, NAVY);  
  
  tft.setCursor(20,50);
  tft.setTextColor(CYAN);
  tft.println("RF-SNIPPER");
  tft.setCursor(40,95);
  tft.println(getVersion());

  tft.setTextSize(1);

  tft.setCursor(40,160);
  tft.setTextColor(YELLOW);
  tft.println("by EB7ME & YO6PIR");

  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextSize(1);
    
  tft.setCursor(100,190);   
  tft.setTextColor(GREEN);
  tft.print("Batt= ");
  float Ubat = battery();     //citeste tensiunea bateriei
  if(Ubat >= 3.7f){tft.print("Good");}
  tft.setTextColor(YELLOW);
  
  if(Ubat < 3.7f && Ubat >= 3.4f){
    tft.print("Week"); 
    BattControl=true;
    }
  tft.setTextColor(RED);
  if(Ubat < 3.4f){ tft.print("BAD!");delay(5000);}  
  tft.setFont(); 
  tft.setCursor(10,225);
  tft.setTextColor(GREEN);
  tft.println("(Based on the VU2ESE-Antuino)");
     
  tft.setCursor(240,225);   
  tft.setTextColor(CYAN);
  tft.print(getVersion());
}

/*-------------- Deseneaza MAIN SCREEN -------------------*/
void GridScreen(){
  tft.fillScreen(NAVY);
  /*chenar alb in jurul ecranului cu fond negru*/
  tft.drawRect(0, gridOffsetY-2, 320, 205, WHITE); 
  tft.drawRect(1, gridOffsetY-1, 318, 203, WHITE);   
  tft.fillRect(3, gridOffsetY, 315, 200, BLACK);
  
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextSize(1);

  tft.setTextColor(GREEN);
  tft.setCursor(8, 11);   
  tft.print("YO6PIR");
    
  tft.setCursor(90, 11);   
  tft.setTextColor(WHITE);

  tft.print("RF-Snipper 3.5");   

  tft.setCursor(10, 233);   
  tft.print("X");  
  tft.setCursor(170, 233);   
  tft.print("Y");
  if(Mode==1){
  }       
  GridScreenTitles();
}

/*----------------- Deseneaza GRID SCREEN TITLES ------------------------*/
void GridScreenTitles(){ 
  float numberToShow;
  float freqToShow;
  tft.setFont(&FreeMonoBold9pt7b);
  tft.setTextSize(1);
  //*Afiseaza programul de lucru Actual*  
  tft.setTextColor(NAVY);
  tft.fillRect(280 , 0 , 39, 12, NAVY);
  switch(Mode){           //selectie mode de lucru
    case 0://*POW*
      tft.setTextColor(CYAN, NAVY);
      break;
    case 1://*SWR*
      tft.setTextColor(YELLOW, NAVY);
      break;
    case 2://*SNA*
      tft.setTextColor(GREEN, NAVY);
      break;
    }
    tft.setCursor(280,11);  
    tft.print(Modes[Mode]);  
  
  tft.setFont();      //selecteaza cel mai mic font
  tft.setTextSize(1);

 tft.setTextColor(NOSELECTED, NAVY);
  tft.setCursor(30,220); 
  tft.print("Center: ");
  
  tft.setCursor(78,220);  
  dtostrf(float(working_center)/1000000,7,3,c);
  tft.print(c);

  tft.setCursor(120,220);
  tft.print(" MHz");

  tft.setCursor(30,230); 
  tft.print("Span:   "); 
  
  tft.setCursor(78,230);      
  dtostrf(float(working_span)/1000000,7,3,c);
  tft.print(c);
  tft.setCursor(120,230);   
  tft.print(" MHz");

  tft.setCursor(200,220); 
  
  if (Mode != 1) tft.print("Ref ^ =     dBm ");    //nu afiseaza in Mode SWR
  else  tft.print("                ");                          

  
  if (Mode != 1){
      tft.setCursor(250,220);    
      tft.print(int(working_ref));
    }     
  
  tft.setCursor(200,230); 
  if (Mode != 1) tft.print("Scala =    dBm/Div");
  else tft.print ("Scala =           ");
  
  tft.setCursor(250,230);   
  tft.print(int(working_scale));     

  //* Deseneaza gradatiile de pe marginea graficului*    
  tft.fillRect(3, gridOffsetY, gridOffsetX - 3, GridHeight, BLACK);
  tft.setFont(&TomThumb);
  tft.setTextColor(CYAN, BLACK); 
  tft.setTextSize(1);
  
  //*Deseneaza scala gradata in stg ecranului Grid *
  for (int y=0; y <= 10; y += 1){
      if ((y > 0) && (y < 10)) tft.setCursor(5, GridHeight*y/10 + gridOffsetY + 3);  
        else if (y == 0) tft.setCursor(5, 7 + gridOffsetY); 
        else if (y == 10) tft.setCursor(5, GridHeight + gridOffsetY);  
       
      if (Mode != 1){
        float res = working_ref - working_scale*y;
        int rez = int(res);
        String text= String(rez);
        if(Mode==0)  tft.setTextColor(CYAN, BLACK); 
        else   tft.setTextColor(GREEN, BLACK); 
        if(res>0)tft.print("+");
        if (res> -100)tft.println(text);
      }
       else {
          float plotSwrScale = float(working_scale) - y * float(working_scale - 1)/10;
          dtostrf(plotSwrScale, 1, 1, c);     
          tft.setTextColor(YELLOW, BLACK); 
          tft.print(c);
       }       
  } 
    //* Deseneaza inscriptiile dela baza Graficului *
    if(Mode!=0)  tft.setTextColor(CYAN, BLACK); 
    else  tft.setTextColor(WHITE, BLACK); 
    
   int Ypos = 203;
   tft.fillRect(5, gridOffsetY + GridHeight+3, 310, 5, BLACK); 
   tft.setCursor(160,Ypos);  
     
   if (working_center >= 1000000){
       dtostrf(float(working_center)/1000000,5,3,c);
       tft.print(c); 
       
    if (working_span >= 100000) dtostrf(float(working_center - working_span/2)/1000000, 5, 2, c); 
    else dtostrf(-(float(working_span)/2)/1000, 5, 1, c);                     
   }
   else{
     dtostrf(float(working_center)/1000,5,1,c);
     tft.print(c);          
     dtostrf(float(working_center - working_span/2)/1000,5,1,c); 
   }   
          
   tft.setCursor(10,Ypos);
   tft.print(c); 
        
   for (int y=1; y <= 4; y += 1){
      tft.setCursor(8+30*y,Ypos);
      freqToShow = float(working_center - (5-y)*working_span/10);
      if (working_center >= 1000000){
          if (working_span >= 100000){
              numberToShow = freqToShow/1000000;
              dtostrf(numberToShow,5,2,c);
          }
          else{
             numberToShow = (freqToShow - float(working_center))/1000;
             dtostrf(numberToShow,5,1,c);
          }
      }
      else{
          numberToShow = freqToShow/1000; 
          dtostrf(numberToShow,5,1,c);
      }      
      tft.print(c); 
    }
    
   for (int y=6; y <= 9; y += 1){
      tft.setCursor(8+30*y,Ypos);
      freqToShow = float(working_center + (y-5)*working_span/10);
      if (working_center >= 1000000){
          if (working_span >= 100000){
              numberToShow = freqToShow/1000000;
              dtostrf(numberToShow,5,2,c);
          }
          else{
             numberToShow = (freqToShow - float(working_center))/1000;
             dtostrf(numberToShow,5,1,c);
         }            
      }
      else{
          numberToShow = freqToShow/1000; 
          dtostrf(numberToShow,5,1,c);
      }      
      tft.print(c); 
  }  
 
  tft.fillRect(2, gridOffsetY+GridHeight+11,316, 8, BLACK); 
  lastLevel = 255;
  UpdateBatteryIcon(254, 3, battery());  
}

/*---------------- Deseneaza diviziunile pe Grid-Screen --------------------------------*/
void GridScreenDivs(){   
  for (int x=0; x<=300; x += 30){
      tft.drawLine(x + gridOffsetX, gridOffsetY, x + gridOffsetX, GridHeight + gridOffsetY, GRIDCOLOR);
  }
  for (int y=0; y<=GridHeight; y += GridHeight/10){
      tft.drawLine(gridOffsetX, y + gridOffsetY, 300 + gridOffsetX, y + gridOffsetY, GRIDCOLOR);
  }   
  for (int y=0; y<=GridHeight; y += GridHeight/50){
      tft.drawLine(148 + gridOffsetX, y+ gridOffsetY, 152 + gridOffsetX, y + gridOffsetY, GRIDCOLOR);
  } 
  for (int x=0; x<=300; x += 6){
      tft.drawLine(x + gridOffsetX, GridHeight/2 + gridOffsetY - 2, x + gridOffsetX, GridHeight/2 + gridOffsetY + 2, GRIDCOLOR);
  } 
}

/*------------ Meniul TEMPLATES de selectii Frecvente / Span ------------*/
void Templates(){ 
  tft.fillScreen(BLACK);
  tft.setFont(&FreeSerifBold12pt7b);
  tft.setTextSize(1);
  
  tft.drawRect(1,1,319,239, NAVY);
  tft.drawRect(3,3,315,235, YELLOW);
  tft.drawRect(5,5,311,231, NAVY);

  tft.setCursor(35,25); tft.setTextColor(YELLOW);
  tft.print("Band Select          SPAN");

  xSel = 10;
  ySel = 10;

  xSelFPrev = 10;
  ySelFPrev = 10;
  xSelSPrev = 10;
  ySelSPrev = 10;
     
  for (int x=0; x<3; x++){
      for (int y=0; y<6; y++){
          if ((x < 2) && (y < 5)){
              tft.fillRoundRect(10 + 90*x, 35 + 30*y, 80, 25, 4, GRIDCOLOR);
              tft.drawRoundRect(10 + 90*x, 35 + 30*y, 80, 25, 4, CYAN);
              
              tft.setCursor(20 + 90*x, 55 + 30*y); tft.setTextColor(CYAN);
              tft.print(Bands[5*x + y]);
          }        
          else if (x == 2){
              tft.fillRoundRect(195, 35 + 30*y, 110, 25, 4, GRIDCOLOR);
              tft.drawRoundRect(195, 35 + 30*y, 110, 25, 4, CYAN);
                  
              tft.setCursor(198, 55 + 30*y); tft.setTextColor(GREEN);
              tft.print(Spans[y]);
          }
      }
  }
  tft.drawRoundRect(6, 185, 180, 46, 4, YELLOW);   
  tft.fillRoundRect(20, 190, 60, 35, 4,GREEN);
  tft.drawRoundRect(20, 190, 60, 35, 4, CYAN);

  tft.setCursor(30, 213);tft.setTextColor(NAVY);
  tft.print("OK");
    
   tft.fillRoundRect(100, 190, 80, 35, 4, RED);
   tft.drawRoundRect(100, 190, 80, 35, 4, CYAN);

   tft.setCursor(102, 213); tft.setTextColor(WHITE); 
   tft.print("Return");         
}

/*---------- Deseneaza MARKER pe ecran ------------------------------------*/
void Marker(int xpoint) {
    tft.setFont();
    tft.setTextSize(1);
    static int old_xLabel = -1;
    static int old_yLabel = -1;
    static int old_labelWidth = 0;
    static int old_labelHeight = 0;
    static int old_xpoint = -1;

    // 🟥 Când trebuie să ștergem markerul complet
    if (xpoint == 0) {
        if (old_xpoint >= 0) {
            tft.fillRect(old_xpoint + gridOffsetX, gridOffsetY, 1, GridHeight, BLACK);
            if (old_xLabel >= 0)
                tft.fillRect(old_xLabel, old_yLabel, old_labelWidth, old_labelHeight, BLACK);
            GridScreenDivs();
            old_xpoint = -1;
            old_xLabel = -1;
        }
        return;
    }

    // 🟩 Marker nou / mutat
    if (xpoint != old_xpoint) {

        // Ștergere marker și eventual text anterior
        if (old_xpoint >= 0)
            tft.fillRect(old_xpoint + gridOffsetX, gridOffsetY, 1, GridHeight, BLACK);
        if (old_xLabel >= 0)
            tft.fillRect(old_xLabel, old_yLabel, old_labelWidth, old_labelHeight, BLACK);

        GridScreenDivs();
        
        // Linie punctată verticală
        for (int y = gridOffsetY; y <= gridOffsetY + GridHeight; y++) {
            if (((y - gridOffsetY) % 4) < 2) {
                int color = (MainScreenOption == 1) ? RED : ((Mode != 1) ? YELLOW : GREEN);
                tft.drawPixel(xpoint + gridOffsetX, y, color);
            }
        }

        // Calculăm textul comun (frecvență și valoare)
        char freqStr[16], valStr[16];
        dtostrf(float(freqSel) / 1000000, 6, 3, freqStr);
        dtostrf(float(GraphData[xSel]) / 10, 3, 1, valStr);
        String freqLine = String(freqStr) + " MHz";
        String valLine = (Mode == 1) ? "SWR:" + String(valStr) : String(valStr) + " dBm";

        // 🔸 Coordonate comune de afișare (flotante)
        int labelHeight = (MainScreenOption == 1) ? (10 * 3 + 4) : 20;
        int markerY = touchY;
        int yLabel;

        if (markerY < gridOffsetY + GridHeight / 2) {
            yLabel = markerY + 6;
            if (yLabel + labelHeight > gridOffsetY + GridHeight)
                yLabel = gridOffsetY + GridHeight - labelHeight - 1;
        } else {
            yLabel = markerY - labelHeight - 6;
            if (yLabel < gridOffsetY) yLabel = gridOffsetY;
        }

        int labelWidth;
        int xLabel = xpoint + gridOffsetX;

        // Afișare diferită în funcție de mod
        if (MainScreenOption == 1) {
            // 🔶 Sel->Center complet
            const char* header = "Sel->Center";
            int w1 = strlen(header) * 6;
            int w2 = freqLine.length() * 6;
            int w3 = valLine.length() * 6;
            labelWidth = max(w1, max(w2, w3)) + 6;

            xLabel -= labelWidth / 2;
            if (xLabel < gridOffsetX) xLabel = gridOffsetX;
            if (xLabel + labelWidth > gridOffsetX + 300)
                xLabel = gridOffsetX + 300 - labelWidth;

            // fundal și contur
            tft.fillRect(xLabel, yLabel, labelWidth, labelHeight, BLACK);
            tft.drawRect(xLabel, yLabel, labelWidth, labelHeight, WHITE);

            // text
            int textX = xLabel + 3;
            int textY = yLabel + 3;
            tft.setTextColor(ORANGE, BLACK);
            tft.setCursor(textX, textY);
            tft.print(header);

            tft.setTextColor(WHITE, BLACK);
            tft.setCursor(textX, textY + 10);
            tft.print(freqLine);
            tft.setCursor(textX, textY + 20);
            tft.print(valLine);

          // ▼ Triunghi roșu cu vârful în jos deasupra casetei
          int triTipY  = yLabel-2;        // vârful săgeții, atinge baza casetei (partea de sus)
          int triBaseY = yLabel - 7;    // baza triunghiului sus de casetă
          tft.fillTriangle(
          xpoint + gridOffsetX - 4, triBaseY,  // colț stânga bază
          xpoint + gridOffsetX + 4, triBaseY,  // colț dreapta bază
          xpoint + gridOffsetX, triTipY,       // vârful triunghiului, către casetă
          RED
          );
        } 
        else {
            // 🔹 Marker simplu → doar text flotant, fără chenar
            labelWidth = max(freqLine.length(), valLine.length()) * 6 + 6;
            xLabel -= labelWidth / 2;
            if (xLabel < gridOffsetX) xLabel = gridOffsetX;
            if (xLabel + labelWidth > gridOffsetX + 300)
                xLabel = gridOffsetX + 300 - labelWidth;

            // fundal negru mic, pentru lizibilitate Marker_mic
            tft.fillRect(xLabel, yLabel, labelWidth, labelHeight, BLACK);

            int textX = xLabel + 3;
            int textY = yLabel + 3;
            tft.setTextColor(WHITE, BLACK);
            tft.setCursor(textX, textY);
            tft.print(freqLine);
            tft.setCursor(textX, textY + 10);
            tft.print(valLine);
        }

        // 🔄 Actualizăm poziții pentru ștergere ulterioară
        old_xpoint = xpoint;
        old_xLabel = xLabel;
        old_yLabel = yLabel;
        old_labelWidth = labelWidth;
        old_labelHeight = labelHeight;
    }
}
