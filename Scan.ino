#define CLAMP_Y(y) \
    if ((y) < gridOffsetY) (y) = gridOffsetY+1; \
    else if ((y) > yBottom) (y) = yBottom;
/***********************************************************************
--------------------------- SCAN dBm ---------------------------------- 
***********************************************************************/      
void Scan_dBm(){   
    frqScanInitTime = millis();
    if (working_span/2 > working_center) working_span =  working_center * 2;
    
    uint32_t scanStep = working_span/300;  
    uint32_t scanInit = working_center - working_span/2;
    uint32_t scanFreq = scanInit;    
    
    int y1; int y2;
    int y3; int y4;
    int y_prev1;
    int y_prev2;
    int yBottom = GridHeight + gridOffsetY;
    uint32_t frecventa;
    int scanMaxAux = -2000;
    uint32_t freqScanMaxAux = 0;
    int scanMinAux = 2000;
    uint32_t freqScanMinAux = 0;    
    int GraphDataAct;
    int GraphDataAnt;
    int Ypost = gridOffsetY + GridHeight + 11;
    int Xpost = 108;   
    
    multiplier = working_scale * 10 * 10;
    
    freq = scanFreq;
    if (Mode == 0) si5351_1();                  
    freq += LO;       
    si5351_3();
    
    // *apasare scurta pe ecran cu MarkerAct=true*    
     if (MarkerAct)GridScreenDivs();     

    for(int x=0; x<300 ; x++){      
      if(MarkerAct) Marker(0);          
         y_prev1 = yBottom - ((GraphDataPrev[x])*GridHeight/multiplier);
        y_prev2 = (x < 299) ? yBottom - ((GraphDataPrev[x+1]) * GridHeight / multiplier) : y_prev1;
         
         CLAMP_Y(y_prev1);
         CLAMP_Y(y_prev2); 
                 
         tft.drawLine(x + gridOffsetX, y_prev1, x + gridOffsetX+1, y_prev2, BLACK);       //sterge linia anterioara trasata 
         tft.drawLine(x + gridOffsetX, y_prev1+1, x + gridOffsetX+1, y_prev2+1, BLACK);   //Dubleaza stergerea liniei anterioare                  
         
         working_dBm = citire_dBm_vobu();   // citește pt frecvența curentă
         
         scanFreq += scanStep;
         freq = scanFreq;
         if (Mode == 0) si5351_1();              
         freq += LO;       
         si5351_3();
         delay(2);
         
         GraphData[x] = working_dBm;
         
         GraphDataAct = working_dBm + 850; 
         GraphDataAct -= (1000 - working_scale*100) - (15-working_ref)*10;         
         
         if (x == 0) GraphDataAnt = GraphDataAct;
          else
          {
            if (working_dBm > scanMaxAux)
              {
                scanMaxAux = working_dBm;
                freqScanMaxAux = scanFreq;
              }    
            if (working_dBm < scanMinAux)
              {
                scanMinAux = working_dBm;
                freqScanMinAux = scanFreq;
              } 
          }

         y1 = yBottom - ((GraphDataAnt)*GridHeight/multiplier);
         y2 = yBottom - ((GraphDataAct)*GridHeight/multiplier);
                   
          CLAMP_Y(y1);
          CLAMP_Y(y2);               
            
          //*********** Traseaza graficul pentru SNA-VOB *************    
         if(Mode==0){//******* Mode VOB ********    
         if (x > 0){
          tft.drawLine(x + gridOffsetX - 1, y1, x + gridOffsetX, y2, CYAN);       //Traseaza linia actuala pentru VOB
          tft.drawLine(x + gridOffsetX - 1, y1+1, x + gridOffsetX, y2+1, CYAN);   //Dubleaza linia actuala VOB
         }
         }
         else{//*Mode SNA*
         if (x > 0){
         tft.drawLine(x + gridOffsetX - 1, y1, x + gridOffsetX, y2, GREEN);       //Traseaza linia actuala SNA                    
         tft.drawLine(x + gridOffsetX - 1, y1+1, x + gridOffsetX, y2+1, GREEN);   //Dubleaza linia actuala SNA
         }
         }
         // 🔄 Actualizăm poziții plotter
         GraphDataPrev[x] = GraphDataAct;   
         GraphDataAnt = GraphDataAct;     
             
         waitInitTime = micros();
         do{  
             if (ts.touched()) TouchEventsRegular();                              
           } while (((waitInitTime + 1000) > micros()) || (MainScreenOption != 0));  
         
         if ((StartScan) || (Screen !=0)) x=300;          
    }  
  
    scanMax = scanMaxAux;
    freqScanMax = freqScanMaxAux;
    scanMin = scanMinAux;
    freqScanMin = freqScanMinAux;

  // Actualizează doar dacă suntem pe ecranul principal si a trecut o secunda
  if ((!StartScan) && (Screen == 0)) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= refreshInterval) {
      lastUpdate = currentMillis;

      // --- Afișare ---
      tft.setFont();
      tft.setTextSize(1);
      tft.setTextColor(BLUE, BLACK);
      // Text principal
      tft.setCursor(23, Ypost);
      tft.print("Min: ");    
      // Frecvență
      tft.setTextColor(ORANGE, BLACK);
      dtostrf(float(freqScanMin) / 1000000, 6, 3, c);
      tft.setCursor(53, Ypost);
      tft.print(c);
      // Valoare minimă măsurată (displayMin)
      dtostrf(float(scanMin) / 10, 5, 1, c);
      tft.setCursor(100, Ypost);
      tft.print(c);
      tft.setTextColor(BLUE, BLACK);
      tft.print(" dBm");
      tft.setTextColor(MAGENTA, BLACK);
      // Text principal
      tft.setCursor(170, Ypost);
      tft.print("Max: ");   
       // Frecvență
      tft.setTextColor(ORANGE, BLACK);
      dtostrf(float(freqScanMax) / 1000000, 6, 3, c);
      tft.setCursor(200, Ypost);
      tft.print(c);
      // Valoare maximă măsurată→(displayMax)
      dtostrf(float(scanMax) / 10, 5, 1, c);
      tft.setCursor(250, Ypost);
      tft.print(c);
      tft.setTextColor(MAGENTA, BLACK);
      tft.print(" dBm");
    }
  } 
  else {    //daca nu suntem in ecranul principal
    StartScan = false;      //opreste scanarea
  }   
}
/***********************************************************************
--------------------------- SCAN SWR ---------------------------------- 
***********************************************************************/  
void Scan_SWR()
{     
    frqScanInitTime = millis();
    
    if (working_span/2 > working_center) working_span =  working_center * 2;
    
    uint32_t scanStep = working_span/300;  
    uint32_t scanInit = working_center - working_span/2;
    uint32_t scanFreq = scanInit;   
  
    int y1; int y2;
    int y3; int y4;
    int y_prev1;
    int y_prev2;
    int yBottom = GridHeight + gridOffsetY - 2;
    float swr_min = 1000.0, swr_max=10000.0;
    uint32_t frecventa;
    int scanMaxAux = 0;
    
    int swr_prev = 0;
    int swr_curr = 0;
    int swr_next = 0;

    uint32_t freqScanMaxAux = 0;
    int scanMinAux = 10000;
    uint32_t freqScanMinAux = 0;        
    float max_dBm;
    int GraphDataActualual;
    int GraphDataAnterior;
    int Ypost = gridOffsetY + GridHeight + 11; 
    float swr;
    multiplier = (working_scale - 1) * 1000;     
   
    if (MarkerAct) GridScreenDivs();   //* apasare scurta pe ecran cu MarkerAct =  true *  
          
    /*------------ Bucla de scanare in 300 puncte pe ecran -----------*/  
     for(int x=0; x<300 ; x++){    
              
         y_prev1 = yBottom - ((GraphDataPrev[x])*GridHeight/multiplier);         
         y_prev2 = (x < 299) ? yBottom - ((GraphDataPrev[x+1]) * GridHeight / multiplier) : y_prev1;
         
         CLAMP_Y(y_prev1);
         CLAMP_Y(y_prev2); 
                  
         tft.drawLine(x + gridOffsetX, y_prev1, x + gridOffsetX+1, y_prev2, BLACK);  
         tft.drawLine(x + gridOffsetX, y_prev1+1, x + gridOffsetX+1, y_prev2+1, BLACK);                         
         
         Vin = analogRead(PB1);
         scanFreq += scanStep;
         freq = scanFreq;
         si5351_2();                 
         freq += LO;       
         si5351_3();
         
        swr = swr_measure();
        swr = filtered_swr(swr);
         
         GraphData[x] = int(swr * 10);

          int swr_aux = int(swr * 100);
         
         if ((swr_aux > scanMaxAux) && (x > 0)){
             scanMaxAux = swr_aux;
             freqScanMaxAux = scanFreq;
         }    
         if (swr_aux < scanMinAux){
             scanMinAux = swr_aux;
             freqScanMinAux = scanFreq;
         }     
         GraphDataActualual = int(swr * 1000) - 1000;
         
         if (x == 0) GraphDataAnterior = GraphDataActualual;
          
         y1 = yBottom - ((GraphDataAnterior) * GridHeight/multiplier);
         y2 = yBottom - ((GraphDataActualual) * GridHeight/multiplier);

          CLAMP_Y(y1);
          CLAMP_Y(y2);
                    
         if (x > 0){                                                                    //Traseaza graficul pentru SWR  
          tft.drawLine(x + gridOffsetX - 1, y1, x + gridOffsetX, y2, YELLOW);           //Traseaza linia actuala SWR
          tft.drawLine(x + gridOffsetX - 1, y1+1, x + gridOffsetX, y2+1, YELLOW);       //Dubleaza lina actuala SWR
         }
         
         // 🔄 Actualizăm poziții plotter               
         GraphDataPrev[x] = GraphDataActualual;
         GraphDataAnterior = GraphDataActualual; 

          waitInitTime = micros();
         do{  
            if (ts.touched()) TouchEventsRegular();
         }while (((waitInitTime + 4000) > micros()) || (MainScreenOption != 0)); 
         if((StartScan) || (Screen !=0)) x=300;         
     }  //*---------- Aici se termina scanarea in 300 puncte ----------------------*/
     
    scanMax = scanMaxAux;
    freqScanMax = freqScanMaxAux;
    scanMin = scanMinAux;
    freqScanMin = freqScanMinAux;

    swr_min = float(scanMin)/100;
    swr_max= float(scanMax/100);
    
// Actualizează doar dacă suntem pe ecranul principal si a trecut o secunda
  if ((!StartScan) && (Screen == 0)) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= refreshInterval) {
      lastUpdate = currentMillis;
      // --- Afișare ---
      tft.setFont();
      tft.setTextSize(1);
      tft.setTextColor(YELLOW, BLACK);
      tft.setCursor(23, Ypost);
      tft.print("Min: ");   
      tft.setCursor(53, Ypost); 
      dtostrf(float(freqScanMin)/1000000, 6, 3, c);
      tft.setTextColor(GREEN, BLACK);
      tft.print(c);
      if (swr_min < 2) dtostrf(swr_min, 4, 2, c);
      else if (swr_min > 9) sprintf(c, ">>>>");
      else dtostrf(swr_min, 4, 1, c);
      tft.setCursor(100, Ypost);
       tft.setTextColor(YELLOW, BLACK);
      tft.print("SWR:");
       tft.setTextColor(GREEN, BLACK);
      tft.print(c);
      tft.setTextColor(MAGENTA, BLACK);
      tft.setCursor(170, Ypost);
      tft.print("Max: ");
      tft.setCursor(200, Ypost); 
      dtostrf(float(freqScanMax)/1000000, 6, 3, c);
      tft.setTextColor(GREEN, BLACK);
      tft.print(c);
      if (swr_max < 2) dtostrf(swr_max, 4, 2, c);
      else if (swr_max > 9) sprintf(c, ">>>>");
      else dtostrf(swr_max, 4, 1, c);
      tft.setCursor(250, Ypost);
       tft.setTextColor(MAGENTA, BLACK);
      tft.print("SWR:");
       tft.setTextColor(GREEN, BLACK);
      tft.print(c);
    }
  } 
  else {
    StartScan = false;
  }
}
float swr_measure(){     
                                      
                  volts=  (float(Vin) * adc_ref )/ 4095.0;
                  working_dBm = (40 * volts) - 85; 
                  working_dBm += float(SWR_offset)/10.0;
                  float v2 = pow(10, working_dBm/20);
                  float r_ant = 51/((1/(v2+0.5))-1);
                  float swr = r_ant / 51;
                  
         if(swr<0 || swr>9.99)swr=9.99;        //nu exista swr negativ acceptat! 
return swr;
}

/*-------- Filtru SWR Linear → Varianta 1 OK --------*/
#define SWR_FILTER_N 5
float swr_buffer[SWR_FILTER_N] = {1,1,1,1,1};
uint8_t swr_idx = 0;

float filter_swr_linear(float swr_curr)
{
    swr_buffer[swr_idx] = swr_curr;
    swr_idx = (swr_idx + 1) % SWR_FILTER_N;
    float sum = 0;
    for(uint8_t i=0;i<SWR_FILTER_N;i++) sum += swr_buffer[i];
    return sum / SWR_FILTER_N;
}
float mapFloat(float x, float in_min, float in_max,
               float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) /
           (in_max - in_min) + out_min;
}

/*----------- Filtru SWR-exponential ---------------------*/
float swr_filtered = 1.0f; 
float alpha = 0.3f;

float filtered_swr(float swr){
    swr_filtered = alpha * swr + (1 - alpha) * swr_filtered;
    return swr_filtered;
}

/*-------- Citire dBm Median3 si normalizare pe punct ------*/
#define DBM_SAMPLES 3

float citire_dBm_vobu(){
    float b[DBM_SAMPLES];

    for (uint8_t i = 0; i < DBM_SAMPLES; i++)
    {
        uint16_t Vin = citireADCmediatFast(9);
        float volts = (float(Vin) * adc_ref) / 4095.0;
        b[i] = (400.0f * volts) - 850.0f;
    }
    // mediană 3
    if (b[1] < b[0]) { float t=b[0]; b[0]=b[1]; b[1]=t; }
    if (b[2] < b[1]) { float t=b[1]; b[1]=b[2]; b[2]=t; }
    if (b[1] < b[0]) { float t=b[0]; b[0]=b[1]; b[1]=t; }

    return b[1];
}
