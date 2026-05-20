/********************************* Touch Screen in Normal Mode**********************************************
---------------------------------------------------------------------------------------------------------------|
| Câmp                   | Explicație                                  | Exemplu din tabel                     |
| ---------------------- | ------------------------------------------- | ------------------------------------- |
| `center`               | frecvența centrală inițială în Hz           | 14000000UL  → 14.000.000 Hz (14 MHz) | 
| `span`                 | intervalul total de analiză (bandă)         | 1000000UL  → 1 MHz                   |
| `ref`                  | nivel de referință pentru grafic            | 0                                     |
| `scale`                | factor de scalare grafic                    | 1.0                                   |
| `clk0`, `clk1`, `clk2` | activează/dezactivează cele 3 ieșiri Si5351 | vezi mai jos                          |
---------------------------------------------------------------------------------------------------------------*/
struct ModeData {
    uint32_t center;   // frecvența centrală de lucru (Hz)
    uint32_t span;     // lățimea de bandă (Hz)
    int16_t  ref;      // nivel de referință (ex: dBm, SWR ref)
    float    scale;    // factor de scalare (ex: pixeli/unitate)
    uint8_t  clk0;     // activare CLK0 pe Si5351
    uint8_t  clk1;     // activare CLK1 pe Si5351
    uint8_t  clk2;     // activare CLK2 pe Si5351
};

ModeData modeData[3] = {
    {15000000UL, 28000000UL, 0, 5.0, 1, 0, 1},  // VOB
    {15000000UL, 28000000UL, 5, 5.0, 0, 1, 1},  // SWR
    {15000000UL, 28000000UL, 3, 5.0, 0, 0, 1}   // SNA
};

uint8_t lastMode = 255;

void PushRegular() {
    StartScan = true;

    // -------- Reîmprospătare doar a zonei gridului ----------
    tft.fillRect(gridOffsetX, gridOffsetY, 300, GridHeight, BLACK);
    GridScreenDivs();

    // --- Salvează valorile curente pentru modul precedent ---
    modeData[Mode].center = working_center;
    modeData[Mode].span   = working_span;
    modeData[Mode].ref    = working_ref;
    modeData[Mode].scale  = working_scale;

    // ------------- Trecem la modul următor ------------------
    if(++Mode == 3)Mode=0;
    MarkerAct=1;
    // --------- EEPROM doar dacă s-a schimbat efectiv ---------
    if (Mode != lastMode) {
         eprom.write(EEP_MODE, Mode);   // sau funcția ta de EEPROM emulată
        lastMode = Mode;
    }

    sel_1 = 0;

    // ----------- Restaurăm valorile pentru noul mod ----------
    working_center = modeData[Mode].center;
    working_span   = modeData[Mode].span;
    working_ref    = modeData[Mode].ref;
    working_scale  = modeData[Mode].scale;

    // --------- Configurăm rapid ieșirile Si5351 --------------
    si5351.output_enable(SI5351_CLK0, modeData[Mode].clk0);
    si5351.output_enable(SI5351_CLK1, modeData[Mode].clk1);
    si5351.output_enable(SI5351_CLK2, modeData[Mode].clk2);

    GridScreenTitles();
}
