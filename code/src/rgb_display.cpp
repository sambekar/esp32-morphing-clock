
#include "rgb_display.h"

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include "common.h"

VirtualMatrixPanel_T<PANEL_CHAIN_TYPE, MyScanTypeMapping>* virtualDisp = nullptr;
HUB75_I2S_CFG mxconfig(PANEL_RES_X*2,PANEL_RES_Y/2,2);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// From: https://gist.github.com/davidegironi/3144efdc6d67e5df55438cc3cba613c8
uint16_t colorWheel(uint8_t pos) {
  if(pos < 85) {
    return dma_display->color565(pos * 3, 255 - pos * 3, 0);
  } else if(pos < 170) {
    pos -= 85;
    return dma_display->color565(255 - pos * 3, 0, pos * 3);
  } else {
    pos -= 170;
    return dma_display->color565(0, pos * 3, 255 - pos * 3);
  }
}

void setupPins() {
  mxconfig.gpio.r1 = 25;
  mxconfig.gpio.g1 = 26;
  mxconfig.gpio.b1 = 27;

  mxconfig.gpio.r2 = 14;
  mxconfig.gpio.g2 = 12;
  mxconfig.gpio.b2 = 13;

  mxconfig.gpio.a  = 23;
  mxconfig.gpio.b  = 19;
  mxconfig.gpio.c  = 5;
  mxconfig.gpio.d  = 17;
  mxconfig.gpio.e  = 18;

  mxconfig.gpio.lat = 4;
  mxconfig.gpio.oe  = 15;
  mxconfig.gpio.clk = 16;
}

void display_init() {
  setupPins();  
    mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_10M;
    mxconfig.clkphase = false;
    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(50); //0-255
    dma_display->clearScreen();
  virtualDisp = new VirtualMatrixPanel_T<PANEL_CHAIN_TYPE, MyScanTypeMapping>(
        VDISP_NUM_ROWS,
        VDISP_NUM_COLS,
        PANEL_RES_X,
        PANEL_RES_Y
    );
     virtualDisp->setDisplay(*dma_display);
}

void logStatusMessage(const char *message) {
  Serial.println(message);
  // Clear the last line first!
  //virtualDisp->fillRect(0, 56, 128, 8, 0);

  //virtualDisp->setTextSize(1);     // size 1 == 8 pixels high
  //virtualDisp->setTextWrap(false); // Don't wrap at end of line - will do ourselves

  //virtualDisp->setCursor(0, 56);   // Write on last line

  //virtualDisp->setTextColor(LOG_MESSAGE_COLOR);
  //virtualDisp->print(message);

  messageDisplayMillis = millis();
  logMessageActive = true;
}


void logStatusMessage(String message) {
  Serial.println(message);
  // Clear the last line first!
  //virtualDisp->fillRect(0, 56, 128, 8, 0);

  //virtualDisp->setTextSize(1);     // size 1 == 8 pixels high
  //virtualDisp->setTextWrap(false); // Don't wrap at end of line - will do ourselves

  //virtualDisp->setCursor(0, 56);   // Write on last line

  //virtualDisp->setTextColor(dma_display->color444(255,0,0));
  //virtualDisp->print(message);

  messageDisplayMillis = millis();
  logMessageActive = true;
}

void clearStatusMessage() {
   //virtualDisp->fillRect(0, 56, 128, 8, 0); 
   logMessageActive = false;
}

void displaySensorData() {
  if (sensorDead) {
    //virtualDisp->fillRect(SENSOR_DATA_X, SENSOR_DATA_Y, SENSOR_DATA_WIDTH, SENSOR_DATA_HEIGHT, 0);
    //virtualDisp->setTextSize(1);     // size 1 == 8 pixels high
    //virtualDisp->setTextWrap(false); // Don't wrap at end of line - will do ourselves
    //virtualDisp->setTextColor(SENSOR_ERROR_DATA_COLOR);
    
    //virtualDisp->setCursor(SENSOR_DATA_X, SENSOR_DATA_Y);   
    //virtualDisp->print("No sensor data!");
  }

  if (newSensorData) {
    //virtualDisp->fillRect(SENSOR_DATA_X, SENSOR_DATA_Y, SENSOR_DATA_WIDTH, SENSOR_DATA_HEIGHT, 0);
    //virtualDisp->setTextSize(1);     // size 1 == 8 pixels high
    //virtualDisp->setTextWrap(false); // Don't wrap at end of line - will do ourselves
    //virtualDisp->setTextColor(SENSOR_DATA_COLOR);
//    dma_display->setFont(&FreeSerifBold12pt7b);

    //virtualDisp->setCursor(SENSOR_DATA_X, SENSOR_DATA_Y);   
    //dma_display->printf("%4.1f C           %3d%%", sensorTemp, sensorHumi);
    
    // Draw the degree symbol manually
    //virtualDisp->fillRect(SENSOR_DATA_X + 25, SENSOR_DATA_Y, 2, 2, SENSOR_DATA_COLOR);
    
    newSensorData = false;
  }
}

void displayLightData(float luxValue) {
  //virtualDisp->fillRect(LIGHT_DATA_X, LIGHT_DATA_Y, LIGHT_DATA_WIDTH, LIGHT_DATA_HEIGHT, 0);
  
  //virtualDisp->setTextSize(1);     // size 1 == 8 pixels high
  //virtualDisp->setTextWrap(false); // Don't wrap at end of line - will do ourselves
  //virtualDisp->setTextColor(SENSOR_DATA_COLOR);
  //    dma_display->setFont(&FreeSerifBold12pt7b);

  //virtualDisp->setCursor(LIGHT_DATA_X, LIGHT_DATA_Y);   
  //virtualDisp->printf("%4.1f lx", luxValue);
    

}

void displayForecastData() {

}

// Simple R/G/B screen fill, for testing displays
void displayTest(int delayMs) {
  virtualDisp->fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, dma_display->color565(255, 0, 0));
  delay(delayMs);
  virtualDisp->fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, dma_display->color565(0, 255, 0));
  delay(delayMs);
  virtualDisp->fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, dma_display->color565(0, 0, 255));
  delay(delayMs);
  virtualDisp->fillRect(0, 0, MATRIX_WIDTH, MATRIX_HEIGHT, dma_display->color565(0, 0, 0));
}