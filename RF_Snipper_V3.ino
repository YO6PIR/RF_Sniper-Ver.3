/*###########################################################################    
#       ___    ___ _______     ______     _______    ___  _______           #
#       \   \ /  //   __   \  /   ___)   |    __  \ |   ||    __  \         #
#        \   /  /|   |  |   ||   |____   |   |__)  ||   ||   |__)  |        #
#         \    / |   |  |   ||    ___  \ |    ____/ |   ||       _/         # 
#         |   |  |   |__|   ||   (___)  ||   |      |   ||   |\   \         #
#         |___|   \_______ /  \________/ |___|      |___||___| \___\        # 
#                                                                           #   
#############################################################################    
           
*  RF-SNIPPER Ver.3.5-20.12.2025 cu GRID-DIP cu scala neagra si Bat. Dec.2025 
*                  https://www.qsl.net/yo6pir/snipper3
*    Processor:    STM32F103CBT6, 128KB Flash, 20KB RAM, EEprom 24Cxx
*    VFO-DDS:      Si5351
*    Display:      ILI9341 SPI
*    LCD Library:  Adafruit_ILI9341.h - Hardware_SPI
*    MEMORY:       EEProm 24Cxx 
*    Based on source code by EB7ME 2019 version developed after Antuino ideea
*    ALL Commands  Tuch-screen assisted by YO6PIR design 
*    Meniuri:      SWR-VOB-SNA-GDO-STR    
*    
* Added upgrade functions by YO6PIR:
*   Actualizare functii Min/Max afisate dupa scanarea grafica 20.12
*   Simbol baterie in coltul dreapta-sus add.19.12
*   Filtru SWR-exponential si citire dBm median in 3 puncte add.18.12
*   
*--------------------------- MAIN WORKING MODE:---------------------------------------------------------------------
* 
* Change working modes POW,SWR,SNA,GDO,STR with touch on the top right corner
* Short tapping on the Grid-Screen shows a vertical dotted line = Marker
*     - Green in SWR mode
*     - Yellow in POW,SNA mode
* Marker inscription on float pointer plus Freq and dBm or SWR depends on the Mode
* Long tapping on the screen fixes the respective frequency on the Center of the Screen; at short touch refresh
* Short touch on the letter [X] switches to the TEMPLATES screen where you can select preset frequencies/Span
* Short touch on the letter [Y] displays the selectable divisions on the "Y" coordinate on the left side of the screen
*     - if a value is selected, that value is taken on the dBm/Div coordinate Y
*     - if you briefly touch the screen, you exit the selection without taking the new value
* Long touch on the first top line of the Grid screen displays the reference values ​​
*     - if a value is selected, that value is taken on the Ref coordinate X-Up
*     - if you briefly touch the screen, you exit the selection without taking the new value
* Long touch on the central coordinate of the Grid screen, it changes to the virtual keyboard "Center Frequency"
*     - type the value and multiplier:( Hz, KHz, MHz) then OK exits with taking the value
*       typed on the central value of the graph
*     - If not type nothing and just press OK the old value of "Central Frequency "will not change
* Short Touch on the SPAN Freq. value at the bottom of the screen switches to the virtual keyboard "SPAN Freq."
*     - type the value and multiplier: (Hz, KHz, MHz) then OK to exit with the value taken
*       typed on the central value of the graph
*     - If nothing is typed and only OK is pressed, the old value of "SPAN Frequency" will not change
* ----------------------------------- MODE SETTINGS:-----------------------------------------------------------
* When starting up in the start-up screen, if you hold down the touch Long anywhere on the screen, you enter the SETTINGS MENU
* by short pressing on the displayed tiles, it changes to:
*     - Touch-Screen Calibration
*     - Si5351 Xtall Calibration
*     - Local Oscillator Frequency
*     - SWR Return-Loss Adjust
*     - EXIT
* Touch Long on one of the selected tiles, you enter the respective menu
* Touch Calibration MENU:
*     - touch the corners of the screen at the marked points and observe the recorded values
*       If the current values ​​are close to the preset values, it is recommended to exit the menu and
*       keep the current values ​​by short pressing the target in the middle of the screen
*       If the current values ​​are big different to the preset values, save the new values ​​with
*       Long press of the central target, more than 5sec; "SAVE" appears on the screen and exits the menu.
* Si5351 Xtall calibration MENU:
*     - check the frequency of 10,000,000 Hz generated on the OUT-RF output with a frequency meter
*     - Hold down the central value displayed in the menu with large green numbers and enter the
*       virtual keyboard
*     - Type the new desired value of correction factor, and press 'OK'
*     - check again on the frequency meter that the generated frequency is as close as possible to 10MHz
*     - if it is OK, hold down the SAVE button and exit the menu saving the new value
* Local Oscillator Frequency MENU:
*     - Connect the two OUT-RF and INPUT-RF device jacks together with a shielded cable
*     - Check the measured dBm value; the value must be as high as possible above 0dBm
*     - keep the Touch pressed on the central green number and enter the virtual keyboard
*     - Type the new desired value and press 'OK'. Return to the previous screen
*     - Check the generated dBm value again and if it is better than the old one, keep pressed
*       SAVE button to save and exit the menu
* SWR Return Loss Adjust MENU:
*     - connect 300 Ohms to the Input device and observe the recorded SWR value; must to be ~6.0 
*     - keep the Touch pressed on the central green number and enter the virtual keyboard
*     - type the new Return-Loss correction value preceded by +/- if necessary and press 'OK'
*     - the current SWR value must be as possible in apropiate value, around SWR 6.0
*     - if the value is 'OK' press the SAVE button and exit the menu
* If all calibration menus have been completed, press the EXIT button and hold for 2 seconds to RESTART
* With the procedures presented, the device is set for
*/
/************ LIBRARIES***********************/
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include <Fonts/TomThumb.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <si5351.h>
#include <at24c02.h>                 //New routine for memory store  
#include "Smeter_bitmap.h"        // array RLE generat pentru indicator analogic
/* Create a eprom object configured at address 0
Sketch assumes that there is an eprom present at this address*/
AT24C02 eprom(AT24C_ADDRESS_0);
/* Create another eprom object configured att address 2
Sketch assumes that there is NO eprom present at this address*/
AT24C02 badEprom(AT24C_ADDRESS_2);

//Pini Hardware
#define   TFT_CS        PA4
#define   TFT_DC        PA3
#define   TFT_RST       PA2
#define   CS_PIN        PC14
#define   TIRQ_PIN      PC15
#define   BAT           PB0             // intrarea de tensiune a bateriei
#define   READING       PB1             // intrarea de semnal citit de la AD8307
#define   RAPORT        1.303030      // Raport de transformare pt R1=100k, R2=330k → Max=4.3 Volts

//Culori TFT si UI
#define NAVY        ILI9341_NAVY
#define BLUE        ILI9341_BLUE
#define CYAN        ILI9341_CYAN
#define GREEN       ILI9341_GREEN
#define MAGENTA     ILI9341_MAGENTA
#define ORANGE      ILI9341_ORANGE
#define YELLOW      ILI9341_YELLOW
#define RED         ILI9341_RED
#define WHITE       ILI9341_WHITE
#define GRAY        0x8410
#define BLACK       ILI9341_BLACK
#define NOSELECTED  ILI9341_WHITE
#define GRIDCOLOR   GRAY
#define ILI9341_GREY 0x5AEB

//GRID si coordonate ecran
#define gridOffsetX   17
#define gridOffsetY   15
#define GridHeight    180
#define GridWidth     300
#define MINPRESSURE   400

const int nrMedii = 8;    //numar de citiri ADCmediat_fast

// Screen coordinates for 240x320 resolution
int X0, Y0, X1, Y1;
int eep_X0, eep_X1, eep_Y0, eep_Y1;
int ACTx;
int ACTy;

int X0_NEW = -10000;
int X1_NEW = 10000;
int Y0_NEW = -10000;
int Y1_NEW = 10000;

int16_t touchX, touchY, touchP;

//Obiecte TFT si Touch
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST); // Use hardware SPI
XPT2046_Touchscreen ts(CS_PIN);

//EEPROM si Calibrare
int           eep_romadd;
uint32_t      eep_LO;               // Xtall Oscillator Calibration
uint32_t      eep_cal_factor;       // Si5351 Calibration factor
int32_t       eep_swr_offset;       // SWR calibration
uint8_t       eep_mode;
float         dBm_zero_offset = 0.0f;    //dBm initial ne-calibrat     

//Timere si variabile de loop
unsigned long         waitInitTime;
unsigned long         frqScanInitTime;
unsigned long         loopTime;
static unsigned long  lastUpdate = 0;             // momentul ultimei actualizări
const unsigned long   refreshInterval = 1000;     // 1000ms → 1sec

//Setari ecran
bool ScreenPortrait = false; // sau true când vrei portrait

//Setari Si5351
Si5351 si5351;

int32_t  cal_factor;                   
uint32_t freq;
uint32_t LO;                        
uint64_t si5351_freq;                  

byte     Si5351OscPower[] = {0,4,4,4}; // Nivel de iesire Si5351 (1,2,3,4)->(2,4,6,8)mA

uint32_t POW_center =  15000000;
uint32_t POW_span   =  28000000;
uint32_t POW_min    =  POW_center - POW_span/2;
uint32_t POW_max    =  POW_center + POW_span/2;   
int      POW_ref    = 0;
int      POW_scale  = 6;

uint32_t SWR_center =  15000000;
uint32_t SWR_span   =  28000000;
uint32_t SWR_min    =  SWR_center - SWR_span/2;
uint32_t SWR_max    =  SWR_center + SWR_span/2;  
int      SWR_ref    = 0;
int      SWR_scale  = 5;   
int32_t  SWR_offset = 0;
bool     SavedSWRoffset = false;

uint32_t SNA_center =  15000000;
uint32_t SNA_span   =  28000000;
uint32_t SNA_min    =  SNA_center - SNA_span/2;
uint32_t SNA_max    =  SNA_center + SNA_span/2;
int      SNA_ref    = 0;
int      SNA_scale  = 6;   

uint32_t working_center;
uint32_t working_span;   
int      working_ref;
int      working_scale; 

long FreqGDO     = 0;
long FreqGDO_OLD = 0;

//Variabile auxiliare si UI logic
int      multiplier;
int      multiPrev; 
bool     newScale;

float    VinFiltered = 0;
bool     KeypadFlag=false, MarkerAct=1, NOmarker=false;
int      Vin; 
float    volts;
float    working_dBm;
float    adc_ref = 3.3;

bool     StartScan = false;    
int      Mode = 2;   // 2=SNA DEFAULT MODE
int      Screen = 0;
int      Focus = 0;
int      sel_1 = 0;
int      sel_2 = 0; 
int      MainScreenOption = 0;

bool     BattControl = false;
bool     drawGraph = false;    // var care controleaza afisarea graficului pe ecran
char     c[10]; 
int      tmp=0; 
String   Str = "Battery=Week";
long     pushTime = 0;  

int      GraphData[300];
int      GraphDataPrev[300];
int      xSel = 0;
int      ySel = 0; 
int      xSelFPrev = 0;
int      ySelFPrev = 0; 
int      xSelSPrev = 0;
int      ySelSPrev = 0;  

uint32_t freqSel = 0;
float      scanMax;
float      scanMin;
uint32_t freqScanMin;
uint32_t freqScanMax;

//Meniu si selectari
bool      MeniuSetingsActiv = false;
bool      MeniuGDO = false;
int       MeniuSetingSelectat = 0;
int       MeniuSetingSelectatPrev = 0;
bool      Cal_selected = false;
bool      LO_selected = false;
bool      SWR_selected = false;
bool      Touch_selected = false;
bool      ChangeSelected = false;
int       panelX = 0;       // poziția X a panoului vertical, folosită pentru detecția touch
uint8_t lastLevel = 255;   // invalid la start

//Tastatura virtuala si Spanuri
const char*    VirtualKeypad[] PROGMEM = {" 1"," 2", " 3", "MHz", " 4", " 5", " 6","KHz", " 7", " 8", " 9"," Hz", "Del", " 0", " ."," OK "}; 
String         VirtualKeypadValue = " ";    // valoarea afisata de tastatura virtuala    

const char*    SpanSel[] PROGMEM  = {" 50 KHz", "200 KHz", "500 KHz", "  5 MHz"," 15 MHz", " 30 MHz"};
const uint32_t SpanNum[] PROGMEM  = {50000, 200000, 500000, 5000000, 15000000, 30000000}; 

const char*    options1[] PROGMEM  = {"Max -> Center", "Min -> Center", "Sel -> Center", "   Return"};
const char*    Spans[] PROGMEM  = {"   5 KHz", "  50 KHz", " 200 KHz", "   1 MHz", "  10 MHz", "FULL HF"};
const uint32_t SpanNum1[] PROGMEM  = {5000, 50000, 200000, 1000000, 10000000, 28000000}; 

const char*    Bands[] PROGMEM  = {"160 m", " 80 m", " 60 m", " 40 m", " 30 m", " 20 m", " 17 m", " 15 m", " 12 m", " 10 m"}; 
const uint32_t BandsNum1[] PROGMEM  = {1840000, 3700000, 5750000,  7100000, 10125000, 14200000, 18100000, 21200000, 24940000, 28500000};

const char*    Modes[] PROGMEM  = {"VOB","SWR", "SNA"};

/************************************ SETUP ***************************************************/
void setup(){
  
  // calibrează ADC-ul
  rcc_clk_enable(RCC_ADC1);
  adc_calibrate(ADC1);
  adc_enable(ADC1);
  
  frqScanInitTime = millis();
  afio_cfg_debug_ports(AFIO_DEBUG_NONE);          // ST-LINK(PB3,PB4,PA15,PA12,PA11) Can be used      
   // inițializare Serial pentru debug
  Serial1.begin(115200);  // 115200 bps este standard, rapid și stabil

  // poți aștepta câteva zecimi de secundă dacă vrei să deschizi monitorul Serial
  delay(100);

  Serial1.println("Debug SWR VNA DIY: start"); 
  Wire.begin();
  Wire.setClock(400000);

  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 27000000, 0);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLB);
  
  Si5351Strengh();
  
  InitParam();  //Initializeaza parametrii Default 
      
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  ts.begin();
  ts.setRotation(1);
  //Initializare pagina de Start  
  StartPage();                             
  delay(1000);
 //* Meniu calibrare daca se apasa touch-ul in timp ce se afiseaza Start Page*
  TS_Point p = ts.getPoint(); 
  if (p.z > MINPRESSURE){  
   while (ts.touched()){ //*Apasare lunga pe touch screen*
    Settings();
    NOtouch();
   InitParam();
   }
  }
  GridScreen();      
}
 uint16_t fastADCRead(uint8_t channel)
{
    ADC1->regs->SQR3 = channel;

    // conversie dummy (discard)
    ADC1->regs->CR2 |= ADC_CR2_ADON;
    while (!(ADC1->regs->SR & ADC_SR_EOC));
    (void)ADC1->regs->DR;

    // conversie valida
    ADC1->regs->CR2 |= ADC_CR2_ADON;
    while (!(ADC1->regs->SR & ADC_SR_EOC));
    return ADC1->regs->DR;
}
/*----------- Citeste intrarea ADC --------------------------*/
unsigned int citireADCmediatFast(uint8_t channel) {
  uint32_t suma = 0;
  for (int i = 0; i < nrMedii; i++) {
    suma += fastADCRead(channel);
  }
  return (unsigned int)(suma / nrMedii);
}

/**************************** MAIN LOOP *******************************************/
void loop(){
    
  if (Screen == 0){
      if (Mode != 1) Scan_dBm();  
      else Scan_SWR();         
    }  
  else  TouchEventsRegular();                 
}

/************************** InitParam ***********************************/ 
void InitParam(){
/*citeste memoria instalata daca are locatia [73] scrisa Daca NU, rescrie! */
  InitEEprom();  

  si5351.set_correction(cal_factor);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLB); 

  Focus = 0;
  sel_1 = 0;
  switch (Mode){
      case 0:   //Mode VOB
        working_center = POW_center;
        working_span =   POW_span; 
        working_ref =    POW_ref;
        working_scale =  POW_scale;              
        si5351.output_enable(SI5351_CLK0, 1);
        si5351.output_enable(SI5351_CLK1, 0);
        si5351.output_enable(SI5351_CLK2, 1); 
       break;  
      case 1:   //Mode SWR
        working_center = SWR_center;
        working_span =   SWR_span; 
        working_ref =    SWR_ref;
        working_scale =  SWR_scale;
        si5351.output_enable(SI5351_CLK0, 0);
        si5351.output_enable(SI5351_CLK1, 1);
        si5351.output_enable(SI5351_CLK2, 1); 
       break;  
      case 2:   //Mode SNA
        working_center = SNA_center;
        working_span =   SNA_span;
        working_ref =    SNA_ref;
        working_scale =  SNA_scale;
        si5351.output_enable(SI5351_CLK0, 0);
        si5351.output_enable(SI5351_CLK1, 0);
        si5351.output_enable(SI5351_CLK2, 1); 
       break;                           
    }
}
/* Rutina de citire a tensiunii bateriei conectata prin divizor de 1:3 pe portul PB0 */
  float battery(){   
    uint16_t Vin = analogRead(PB0);    //Dump read
    if (millis() > loopTime + 20){    
                  Vin=analogRead(PB0);    //Good read
                  volts = (float(Vin) * adc_ref )/ 4095.0;    //calculeaza tensiunea in Volts [3.3V]
   }     
  return volts * RAPORT;                                         //330 * 3.030303 = 999
  }
  
  void DrawBatterySymbol(int16_t x, int16_t y, float Vbat){

    const float Vmax = 4.05f;     // tensiune maximă
    const float Vmin = 3.30f;     // tensiune minimă vizibilă

    // dimensiuni reduse ~3/4
    const int16_t width  = 14;
    const int16_t height = 8;
    const int16_t tip    = 2;

    // limitare tensiune
    if (Vbat < Vmin) Vbat = Vmin;
    if (Vbat > Vmax) Vbat = Vmax;

    // nivel baterie 0..4
    uint8_t level = (uint8_t)(((Vbat - Vmin) / (Vmax - Vmin)) * 4.0f + 0.1f);
    if (level > 4) level = 4;

    // 1. contur baterie
    tft.drawRect(x, y, width, height, ILI9341_WHITE);

    // cap baterie
    tft.fillRect(
        x + width,
        y + height / 4,
        tip,
        height / 2,
        ILI9341_WHITE
    );

    // 2. umplere nivel
    int16_t cellWidth = width / 4;

    for (uint8_t i = 0; i < level; i++)
    {
        tft.fillRect(
            x + i * cellWidth + 2,     // margine stânga
            y + 2,                     // margine sus
            cellWidth - 2,             // lățime interioară
            height - 4,                // înălțime interioară
            ILI9341_WHITE
        );
    }
}
void UpdateBatteryIcon(int16_t x, int16_t y, float Vbat)
{
    

    uint8_t level = BatteryLevelFromVoltage(Vbat);

    if (level != lastLevel)
    {
        // șterge DOAR zona bateriei
        tft.fillRect(x, y, 16, 8, ILI9341_NAVY);

        DrawBatterySymbol(x, y, Vbat);
        lastLevel = level;
    }
}
uint8_t BatteryLevelFromVoltage(float Vbat)
{
    const float Vmax = 4.05f;
    const float Vmin = 3.30f;

    if (Vbat < Vmin) Vbat = Vmin;
    if (Vbat > Vmax) Vbat = Vmax;

    uint8_t level = (uint8_t)(((Vbat - Vmin) / (Vmax - Vmin)) * 4.0f + 0.1f);
    if (level > 4) level = 4;

    return level;   // 0..4
}
