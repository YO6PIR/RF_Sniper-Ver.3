/* ---------------- Global Variables ----------------*/
#define MENU_CHANGE_MHZ       0
#define MENU_CHANGE_KHZ       1
#define MENU_MODE_DIP         2
#define MENU_MODE_PWR         3
#define BMP_WIDTH  240
#define BMP_HEIGHT 120

const int centerX =           120;
const int centerY =           155;
const int needleLength =      150;
unsigned long sampleWindow =  10;
float oldAngle =              -90;
int oldVal =                  0;
unsigned long lastupdate =    0;
int uiFocus = MENU_CHANGE_KHZ, uiSelected = -1;
int BTN = 55;                                   // dimensiunea butonului
int GAP = 4;                                    // distanta fixa intre butoane

// variabile globale pentru timing       
unsigned long pressStartTime = 0;  // momentul când tasta a fost apăsată
bool flag1sec = false;
bool flag3sec = false;
bool keyPreviouslyPressed = false;  // starea anterioară a tastelor UP/DOWN
   
// poziția butonului DOWN (ancoră)
int Ydown = 320 - BTN;        // lipit de jos
int Xcenter = (240 - BTN) / 2;

// UP – deasupra lui DOWN
int Yup = Ydown - BTN - GAP;

// noile butoane jos (sub OK)
int Ybottom = Yup + BTN + GAP;  // sub butonul OK si <|>
float scaleWide = 1.5;
int BTN_wide = BTN * scaleWide;

// coordonate corecte pentru butoanele late
int Xleft  = Xcenter - GAP - BTN_wide;
int Xright = Xcenter + GAP + BTN;
int XbottomLeft  = Xcenter - GAP - BTN_wide;
int XbottomRight = Xcenter + GAP + BTN;
bool modeSelectat=1;    //Default Mode Selectat → GRID-DIP-METER
 
// Structura unui buton
struct TouchButton {
    int x0, y0, x1, y1;   // coordonate dreptunghi
    bool state;           // toggle curent
    bool prevTouch;       // starea anterioară touch
    int id;               // optional, identificator
};

TouchButton buttons[7] = {
    {215,   0, 270,  85, false,false,0},  // buton RIGHT
    {215,  85, 270, 150, false,false,1},  // buton UP
    {215, 150, 270, 240, false,false,2},  // buton LEFT
    {270,   0, 320,  85, false,false,3},  // buton SCAN
    {270,  85, 320, 150, false,false,4},  // buton DOWN
    {270, 150, 320, 240, false,false,5},  // buton MODE
    {140,   0, 200, 240, false,false,6},  // buton NOMBER
};

/*------- Rutina de evenimente Touch in mod DIP ------------------*/
void TouchEventDip() {
    TS_Point p = ts.getPoint();

    if (p.z > MINPRESSURE) {
        touchX = 320 - (320*(p.x-X1))/(X0-X1);
        touchY = 240 - (240*(p.y-Y1))/(Y0-Y1);
        touchP = p.z;
    } else {
        touchX = -1;
        touchY = -1;
        touchP = 0;
    }

    // verificăm toate butoanele
    for (int i = 0; i < 7; i++) {
        TouchButton* b = &buttons[i];
        bool current_Touch = (touchX >= b->x0 && touchX <= b->x1 &&
                         touchY >= b->y0 && touchY <= b->y1 &&
                         touchP > 0);

        // starea butonului este egală cu current_Touch
        b->state = current_Touch;

        // salvăm starea anterioară dacă mai ai nevoie pentru flanc rising
        b->prevTouch = current_Touch;
    }
}

/*--------------- Desenare Buton -------------------------------------------*/
void drawBtn(int x, int y, const char *label, float scaleX = 1.0) {
  int width = BTN * scaleX;  // lățimea butonului scalată
  int height = BTN;           // înălțimea rămâne BTN
  int radius = 6;

  // fundal 
  tft.fillRoundRect(x + 1, y + 1, width - 2, height - 2, radius, ILI9341_BLUE);

  // contur 
  tft.drawRoundRect(x + 1, y + 1, width - 2, height - 2, radius, ILI9341_BLUE);

  // contur alb interior
  tft.drawRoundRect(x + 2, y + 2, width - 4, height - 4, radius, ILI9341_WHITE);

  // centrare text
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  int16_t bx, by;
  uint16_t bw, bh;
  tft.getTextBounds(label, 0, 0, &bx, &by, &bw, &bh);
  tft.setCursor(x + (width - bw) / 2, y + (height - bh) / 2);
  tft.print(label);
}

/*------------------------- Formatare cifre afisate --------------------*/
void format_xxx_xxx(long val, char *out) {
    if (val < 0) val = 0;
    if (val > 999999) val = 999999;

    if (val < 1000) {
        // Sub 1000 → 3 spații + 3 cifre
        sprintf(out, "  0.%03ld", val);
    } else {
        int high = val / 1000;
        int low  = val % 1000;

        if (high < 10)
            sprintf(out, "  %d.%03d", high, low);   // 1 cifră high → 2 spații
        else if (high < 100)
            sprintf(out, " %d.%03d", high, low);    // 2 cifre high → 1 spațiu
        else
            sprintf(out, "%d.%03d", high, low);     // 3 cifre high → 0 spații
    }
}

/*------------ Schimbare mod de lucru GDO/STRENGH -----------------*/
void SwitchMode(int state){
  
  if(state==1){    //GDO-SWR Meter
    si5351.output_enable(SI5351_CLK0, 0);
    si5351.output_enable(SI5351_CLK1, 1);
    si5351.output_enable(SI5351_CLK2, 1); 
  }
  else{   //STRENGTH Meter
    si5351.output_enable(SI5351_CLK0, 0);
    si5351.output_enable(SI5351_CLK1, 0);
    si5351.output_enable(SI5351_CLK2, 1); 
  }   
}

/*---------- Desenare ceas indicator analogic -------------------*/
void drawRLEBitmap16() {
  uint32_t rleIndex = 0;
  uint32_t px = 0, py = 0;

  while (rleIndex < sizeof(rle_bitmap)/sizeof(rle_bitmap[0])) {
    uint16_t count = rle_bitmap[rleIndex++];
    uint16_t color = rle_bitmap[rleIndex++];
    while (count--) {
      tft.drawPixel(px, py, color);
      px++;
      if (px >= BMP_WIDTH) { px=0; py++; }
      if (py >= BMP_HEIGHT) return;
    }
  }
}

/*-------- Redesenare ceas analog doar zona afectată de ac ------------*/
void drawRLEBitmapBox(int xStart, int yStart, int xEnd, int yEnd) {
  uint32_t rleIndex = 0;
  uint32_t px = 0, py = 0;

  while (rleIndex < sizeof(rle_bitmap)/sizeof(rle_bitmap[0])) {
    uint16_t count = rle_bitmap[rleIndex++];
    uint16_t color = rle_bitmap[rleIndex++];
    while (count--) {
      if (px >= xStart && px <= xEnd && py >= yStart && py <= yEnd) {
        tft.drawPixel(px, py, color);
      }
      px++;
      if (px >= BMP_WIDTH) { px=0; py++; }
      if (py > yEnd) return;
    }
  }
}

/*-------------- Deseneaza acul nou pe ecran --------------------*/
void drawNeedle(float angle) {
  float rad = angle * DEG_TO_RAD;
  int visibleLength = needleLength / 2;
  int startOffset = needleLength - visibleLength;

  int xStart = centerX + startOffset * cos(rad);
  int yStart = centerY + startOffset * sin(rad);
  int xEnd   = centerX + needleLength * cos(rad);
  int yEnd   = centerY + needleLength * sin(rad);

  tft.drawLine(xStart-1, yStart, xEnd-1, yEnd, ILI9341_RED);
  tft.drawLine(xStart,   yStart, xEnd,   yEnd, ILI9341_MAGENTA);
  tft.drawLine(xStart+1, yStart, xEnd+1, yEnd, ILI9341_RED);
}

/*-- Ștergere ac vechi prin redraw RLE doar pe zona acului --*/
void eraseOldNeedle(float angle) {
  float rad = angle * DEG_TO_RAD;
  int visibleLength = needleLength / 2;
  int startOffset = needleLength - visibleLength;

  int xStart = centerX + startOffset * cos(rad);
  int yStart = centerY + startOffset * sin(rad);
  int xEnd   = centerX + needleLength * cos(rad);
  int yEnd   = centerY + needleLength * sin(rad);

  // bounding box cu 1px extra pentru grosime 3px
  int bxStart = min(xStart, xEnd) - 1;
  int bxEnd   = max(xStart, xEnd) + 1;
  int byStart = min(yStart, yEnd) - 1;
  int byEnd   = max(yStart, yEnd) + 1;

  if (bxStart < 0) bxStart = 0;
  if (byStart < 0) byStart = 0;
  if (bxEnd >= BMP_WIDTH) bxEnd = BMP_WIDTH - 1;
  if (byEnd >= BMP_HEIGHT) byEnd = BMP_HEIGHT - 1;

  drawRLEBitmapBox(bxStart, byStart, bxEnd, byEnd);
} 

/*-------------Bucla Loop de executie GRID-DIP-METER ------------------------*/
void GridDipMeter(){
  initial:
  uint16_t scala_min, scala_max;
  // flag pentru prima desenare
 bool firstDraw = true;
  MeniuGDO = true; 
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  drawRLEBitmap16();                        // deseneaza bitmap complet Indicator Analogic
  tft.drawRect(0,0,240,124,ILI9341_WHITE);  //deseneaza chenar in jururl indicatorului
  tft.drawRect(1,1,238,122,ILI9341_GREY);
   
  modeSelectat = Mode;
  displayMode((Mode == 1) ? 1 : 0 );        //GDO sau STR in functie de Mode preluat din scanare
  FreqGDO = working_center/1000;
  if(Mode != 1)modeSelectat=0;              //VOB || SNA → STR; SWR → GDO
  SwitchMode(modeSelectat);                 //seteaza modul de lucru

  char buf[16];                             // buffer pentru formatul xxx.xxx
  //Deseneaza butoanele Touch
  drawBtn(Xcenter, Ydown, "DWN");           
  drawBtn(Xcenter, Yup,   "UP");            
  drawBtn(Xleft,   Yup,   "<", scaleWide); 
  drawBtn(Xright,  Yup,   ">", scaleWide);  
  drawBtn(XbottomLeft,  Ybottom, "MODE", scaleWide);
  drawBtn(XbottomRight, Ybottom, "SCAN", scaleWide); 
/*----------- Bucla Loop care ruleaza liber -------------------*/
 for(;;){
   TouchEventDip();           //verifica apasarea butoanelor Touch
    if(buttons[0].state) uiFocus = MENU_CHANGE_KHZ;
    if(buttons[2].state) uiFocus = MENU_CHANGE_MHZ;
    if(buttons[5].state){
      modeSelectat ^=1;                   //toggle mode selectat
      displayMode(modeSelectat);     
      SwitchMode(modeSelectat);           //seteaza modul de lucru
        while (buttons[5].state) {
         TouchEventDip();                 //  rutina de update butoane          
      }
    }
    switch (modeSelectat){
    case 0:                               //Mode STRENGTH
        scala_min=0;                      //-85dBm
        scala_max=2901;                   //+0dBm         
        break;
    case 1:                               //Mode GRID-DIP
        scala_min= 1900;                  //-29.3 dBm → SWR:1.0
        scala_max=2340;                   //-8.8dBm → SWR:9.99  
        break;    
  }  

// citim stările tuturor butoanelor
    bool* states = GetButtonStates();
    bool anyPressed = false;
    
// verificăm dacă vreun buton e activ
    for (int i = 0; i < 7; i++) {
      if (states[i]) {
        anyPressed = true;
        break;
      }
    }

  uiFreq();
  if(modeSelectat){
    freq = FreqGDO*1000UL;
    si5351_2();                  
    freq += LO;       
    si5351_3(); 
  }
  else {
     freq = FreqGDO*1000UL;
     freq += LO;       
     si5351_3();
  }
  format_xxx_xxx(FreqGDO, buf);  
  
  tft.setFont(&FreeMonoBold12pt7b); 
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);   
  tft.setCursor(0,180); 
  tft.print(buf);   
  tft.setTextSize(1);
  tft.print("MHz");
 
/*---------- Update pozitie ac SMOOTH-OPTIMIZAT -----------------*/
// citire ADC
int raw = citireADCmediatFast(9);
// deadband – ignoră fluctuațiile mici
const int deadband = 3;
if (abs(raw - oldVal) <= deadband) {
    raw = oldVal;
} else {
    oldVal = raw;
}

// mapare pe unghi Min→(-134); Max→(-40);
float targetAngle = map(raw, scala_min, scala_max, -134, -40);
targetAngle = constrain(targetAngle, -134, -45);

if (firstDraw) {    //este prima afisare?...
    // Prima afișare → fără smoothing
    eraseOldNeedle(oldAngle);     // opțional, dacă acul inițial este desenat
    drawNeedle(targetAngle);
    oldAngle = targetAngle;
    firstDraw = false;
}
else {    //NU este prima afisare → facem smooth normal
    // smoothing normal
    const float smoothing = 0.15f;
    float newAngle = oldAngle + smoothing * (targetAngle - oldAngle);

    // desenăm doar dacă unghiul s-a schimbat suficient
    if (fabs(newAngle - oldAngle) > 0.1f) {
        eraseOldNeedle(oldAngle);
        drawNeedle(newAngle);
        oldAngle = newAngle;
    }
}        
    if(buttons[3].state){   //Daca Tasta3→[SCAN] este apasata...      
      MeniuGDO=false;                           //dezactiveaza meniul GDO 
      tft.setRotation(1);
      GridScreen();    
      Screen=0;
      StartScan=1 ;                            //incepe scanarea dela zero  
      working_center = FreqGDO * 1000UL;
      Focus=0;  
      si5351.output_enable(SI5351_CLK0, 0);
      si5351.output_enable(SI5351_CLK1, 0);
      si5351.output_enable(SI5351_CLK2, 0); 
      return;                                   //iesire din modul GDO → SCANARE 
    }
      if(buttons[6].state){
      tft.setRotation(0);
      ScreenPortrait=true;
      Keypad(); 
      FreqGDO = working_center/1000;
      ScreenPortrait=false;
      goto initial;
      }
 }
}

/*------------- citeste starea butoanelor apasate -----------*/
bool* GetButtonStates() {
    static bool states[7];
    for (int i = 0; i < 7; i++) {
        states[i] = buttons[i].state;
    }
    return states;
}

/*-------- funcție Up_Down cu auto-repeat progresiv----*/
int Up_Down() {
    bool upPressed = buttons[1].state;   // UP
    bool downPressed = buttons[4].state; // DOWN
    bool anyPressed = upPressed || downPressed;

    int result = 0;

    if (anyPressed) {
        unsigned long now = millis();

        if (!keyPreviouslyPressed) {
            // prima apăsare
            pressStartTime = now;
            flag1sec = false;
            flag3sec = false;
            keyPreviouslyPressed = true;

            // increment/decrement inițial
            if (upPressed) result++;
            if (downPressed) result--;
        } else {
            // tasta este ținută apăsată
            unsigned long heldTime = now - pressStartTime;

            if (heldTime >= 500) flag1sec = true;
            if (heldTime >= 3000) flag3sec = true;

            // incrementare progresivă
            if (flag3sec) {
                // după 3 sec: increment rapid
                if (upPressed) result += 20;   // poți ajusta viteza
                if (downPressed) result -= 20;
            } else if (flag1sec) {
                // după 1 sec: increment mediu
                if (upPressed) result += 2;
                if (downPressed) result -= 2;
            }
            // altfel, ținut <1sec deja am incrementat o dată la apăsare inițială
        }
    } else {
        // tasta eliberată
        keyPreviouslyPressed = false;
        flag1sec = false;
        flag3sec = false;
    }
    return result;
}
/*--------------- Rutina de modificare a frecventei --------------------------*/
void uiFreq() {

  int dir = Up_Down();                          // 1. Citește UP/DOWN
  static int lastFocus = -1;                    // 2. SALVEAZĂ uiFocus vechi și FreqGDO vechi
  unsigned long oldFreq = FreqGDO;

/* ---------------- CREȘTERE ---------------- */
if (dir > 0) {
    if (uiFocus == MENU_CHANGE_MHZ) {
        if (FreqGDO + 1000 <= 100000UL) {
            FreqGDO += 1000;
        }
    }

    else if (uiFocus == MENU_CHANGE_KHZ) {

        int khz = FreqGDO % 1000;

        // Noua regulă:
        // +1 chiar dacă depășește 999, dar *nu schimb Focus*
        if (khz < 999) {
            FreqGDO += 1;
        } 
        else {
            // eram la xxx.999 → +1 → mergem în xxx+1.000, dar Focus rămâne KHZ
            if (FreqGDO + 1 <= 100000UL)
                FreqGDO += 1;
        }
    }
}
/* ------------------------- SCĂDERE ------------------------- */
else if (dir < 0) {

    if (uiFocus == MENU_CHANGE_MHZ) {
        // -1000 până la 0, apoi trec pe KHz
        if (FreqGDO >= 1000UL) {
            FreqGDO -= 1000;
        } else {
            // am ajuns la 0 → comut pe KHZ
            uiFocus = MENU_CHANGE_KHZ;

            // continui scăderea cu -1 dar până la minimul 400
            if (FreqGDO > 400UL)
                FreqGDO -= 1;
        }
    }
    else if (uiFocus == MENU_CHANGE_KHZ) {
        // -1 până la 400
        if (FreqGDO > 400UL)
            FreqGDO -= 1;
    }
}

// Clamp global final
if (FreqGDO < 400UL) FreqGDO = 400UL;
if (FreqGDO > 100000UL) FreqGDO = 100000UL;

  if (uiFocus != lastFocus) {     // 4. DACA S-A SCHIMBAT UIFOCUS → redesenezi doar highlight
    if (uiFocus == MENU_CHANGE_MHZ) {
      tft.fillRect(114,150,82,33,ILI9341_BLACK);
      tft.fillRect(0,150,83,33,ILI9341_NAVY);
    } 
    else {
      tft.fillRect(114,150,82,33,ILI9341_NAVY);
      tft.fillRect(0,150,83,33,ILI9341_BLACK);
    }

    lastFocus = uiFocus;
    return;
  }

  // 5. DACA S-A SCHIMBAT FREQGDO → redesenezi cifrele
  if (FreqGDO != oldFreq) {
    tft.fillRect(114,150,82,33,ILI9341_BLACK);
    tft.fillRect(0,150,83,33,ILI9341_BLACK);
    FreqGDO_OLD = FreqGDO;
    return;
  }
}

/*-------------- Afisare text pe cadran ceas indicator -------*/
void displayMode(int mode){
  tft.setFont(); 
  tft.setTextSize(2);
  tft.fillRect(40, 104,100,16,ILI9341_BLACK);  
  if(mode){
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(40, 104);
  tft.print("GRID-DIP-METER");
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(41, 105);
  tft.print("GRID-DIP-METER");
  }
  else{
  tft.setTextColor(ILI9341_ORANGE);
  tft.setCursor(40, 104);
  tft.print("STRENGTH-METER");  
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(41, 105);
  tft.print("STRENGTH-METER");  
  }
  
}
