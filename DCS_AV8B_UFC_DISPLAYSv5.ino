#include <SPI.h>
#include <Wire.h>
#include <U8x8lib.h>
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

// DCS-AV-8B-UFC by MilKris

// LICENSE: Creative Commons Attribution-NonCommercial 4.0 International Public License

// SSD1322 (SPI) für UFC Scratchpad Display (CS: 10, DC: 9, RESET: 8)
U8X8_SSD1322_NHD_256X64_4W_HW_SPI u8x8(10, 9, 8);

// SSD1306 (I²C) - UFC Comm1 Display (SCL: A5, SDA: A4)
U8X8_SSD1306_128X64_NONAME_HW_I2C oledComm1(U8X8_PIN_NONE);
U8X8_SSD1306_128X64_NONAME_HW_I2C oledComm2(U8X8_PIN_NONE);

// LEDs für Master Caution und Master Warning
DcsBios::LED mcLight(0x787c, 0x0004, 5);
DcsBios::LED mwLight(0x787c, 0x0008, 6);

// Speicher für Display-Werte
char ufcComm1Text[3] = " ";
char lastUfcComm1Text[3] = " ";
char ufcComm2Text[3] = " ";
char lastUfcComm2Text[3] = " ";
char ufcScratchpadText[13] = " ";
char lastUfcScratchpadText[13] = " ";

// Funktion zur Zentrierung des Textes auf dem Display
int getCenteredPosition(int displayWidth, int textLength, int charWidth) {
    return (displayWidth - (textLength * charWidth)) / (2 * charWidth);
}

// Callback für UFC Comm1 Display
void onUfcComm1DisplayChange(char* newValue) {
    memset(ufcComm1Text, 0, sizeof(ufcComm1Text));
    strncpy(ufcComm1Text, newValue, 2);
    if (strcmp(lastUfcComm1Text, ufcComm1Text) != 0) {
        strcpy(lastUfcComm1Text, ufcComm1Text);
        updateComm1Display();
    }
}
DcsBios::StringBuffer<2> ufcComm1DisplayBuffer(0x7954, onUfcComm1DisplayChange);

// Callback für UFC Comm2 Display
void onUfcComm2DisplayChange(char* newValue) {
    memset(ufcComm2Text, 0, sizeof(ufcComm2Text));
    strncpy(ufcComm2Text, newValue, 2);
    if (strcmp(lastUfcComm2Text, ufcComm2Text) != 0) {
        strcpy(lastUfcComm2Text, ufcComm2Text);
        updateComm2Display();
    }
}
DcsBios::StringBuffer<2> ufcComm2DisplayBuffer(0x7956, onUfcComm2DisplayChange);

// Callback für UFC Scratchpad Display
void onUfcScratchpadChange(char* newValue) {
    memset(ufcScratchpadText, 0, sizeof(ufcScratchpadText));
    strncpy(ufcScratchpadText, newValue, 12);
    if (strcmp(lastUfcScratchpadText, ufcScratchpadText) != 0) {
        strcpy(lastUfcScratchpadText, ufcScratchpadText);
        updateScratchpadDisplay();
    }
}
DcsBios::StringBuffer<12> ufcScratchpadBuffer(0x7976, onUfcScratchpadChange);

// OLED-Update für Comm1 Display
void updateComm1Display() {
    oledComm1.clear();
    oledComm1.setFont(u8x8_font_inb21_2x4_r);
    int cursorPos = getCenteredPosition(16, strlen(ufcComm1Text), 2);
    oledComm1.setCursor(cursorPos + 3, 4);
    oledComm1.print(ufcComm1Text);
    oledComm1.refreshDisplay();
}

// OLED-Update für Comm2 Display
void updateComm2Display() {
    oledComm2.clear();
    oledComm2.setFont(u8x8_font_inb21_2x4_r);
    int cursorPos = getCenteredPosition(16, strlen(ufcComm2Text), 2);
    oledComm2.setCursor(cursorPos + 3, 4);
    oledComm2.print(ufcComm2Text);
    oledComm2.refreshDisplay();
}

// OLED-Update für das Scratchpad Display
void updateScratchpadDisplay() {
    u8x8.clearDisplay();
    u8x8.setFont(u8x8_font_inb21_2x4_r);
    int cursorPos = getCenteredPosition(32, strlen(ufcScratchpadText), 2);
    u8x8.setCursor(cursorPos + 2, 3);
    u8x8.print(ufcScratchpadText);
}

// Setup-Funktion: Initialisierung von Displays und LEDs
void setup() {
    DcsBios::setup();
    Wire.begin();
    oledComm1.setI2CAddress(0x3C << 1);
    oledComm2.setI2CAddress(0x3D << 1);
    oledComm1.begin();
    oledComm2.begin();
    u8x8.begin();

    oledComm1.setFont(u8x8_font_inb21_2x4_r);
    oledComm2.setFont(u8x8_font_inb21_2x4_r);
    u8x8.setFont(u8x8_font_inb21_2x4_r);

    oledComm1.clear();    
    oledComm1.setCursor(+ 3, 4);
    oledComm1.print("COM1");
    oledComm1.refreshDisplay();

    oledComm2.clear();
    oledComm2.setCursor(+ 3, 4);
    oledComm2.print("COM2");
    oledComm2.refreshDisplay();

    u8x8.clearDisplay();
    u8x8.setCursor(0, 3);
    u8x8.print("UFC BY MILKRIS");
    
    delay(2000);
    updateScratchpadDisplay();
    updateComm1Display();
    updateComm2Display();
}

// Haupt-Loop
void loop() {
    DcsBios::loop();
}
