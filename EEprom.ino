/* -------------- EEPROM MAP --------------------*/
#define EEP_MAGIC           0x00   // 1 byte - 73 = initializat
#define EEP_MODE            0x01   // 1 byte
#define EEP_XTAL            0x02   // float (4 bytes)

// Calibrare VNA
#define ADDR_CAL_FACTOR     0x10   // uint32_t
#define ADDR_SWR_OFFSET     0x14   // uint32_t
#define ADDR_X0             0x18   // uint32_t
#define ADDR_X1             0x1C   // uint32_t
#define ADDR_Y0             0x20   // uint32_t
#define ADDR_Y1             0x24   // uint32_t
#define IF_FREQ             24996600u  // Default Crystal IF Frequency

/* -------------- Initializare EEProm -----------------------*/
void InitEEprom() {
    uint8_t magic = eprom.read(EEP_MAGIC);

    if (magic != 73) {                   //Validare EEProm initial Blank

        // Daca EEPROM →virgin → 💾 set valori DEFAULT
        eprom.write(EEP_MODE, 2);             // default mode
        eprom.put(EEP_XTAL, IF_FREQ);         // LO default (ex. 25MHz)
        eprom.put(ADDR_CAL_FACTOR,  171000u);
        eprom.put(ADDR_SWR_OFFSET,  11u);
        eprom.put(ADDR_X0,          3869u);
        eprom.put(ADDR_X1,          295u);
        eprom.put(ADDR_Y0,          3853u);
        eprom.put(ADDR_Y1,          159u);  
        eprom.write(EEP_MAGIC, 73);           // marcare EEPROM inițializat
    }
    // Citire valori actuale
    eprom.get(EEP_MODE,       eep_mode);      Mode = eep_mode;
    eprom.get(EEP_XTAL,       eep_LO);        LO = eep_LO;
    eprom.get(ADDR_CAL_FACTOR, eep_cal_factor);  cal_factor  = eep_cal_factor;
    eprom.get(ADDR_SWR_OFFSET, eep_swr_offset);  SWR_offset  = eep_swr_offset;
    eprom.get(ADDR_X0, eep_X0);  X0 = eep_X0;
    eprom.get(ADDR_X1, eep_X1);  X1 = eep_X1;
    eprom.get(ADDR_Y0, eep_Y0);  Y0 = eep_Y0;
    eprom.get(ADDR_Y1, eep_Y1);  Y1 = eep_Y1;
}

/*----------- Versiune inscriptionata automat la compilare ---------------*/
String getVersion() {
  String date = __DATE__;   //* exemplu: "Nov  3 2025" 
  String day = date.substring(4, 6);
  String month = date.substring(0, 3);

  //* Convertim luna text în număr*
  String months = "JanFebMarAprMayJunJulAugSepOctNovDec";
  int m = (months.indexOf(month) / 3) + 1;

  // * Formatăm: Ver.3.zz.mm
  char buf[20];
  sprintf(buf, "Ver 3.%02d.%02d", day.toInt(), m);
  return String(buf);
}
