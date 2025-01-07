#include "displayDriver.h"

#ifdef M5STACK_DISPLAY

#include <Arduino.h>
#include <M5Stack.h>
#include <WiFi.h>

#include "Free_Fonts.h"
#include "monitor.h"
#include "drivers/storage/storage.h"
#include "wManager.h"

extern monitor_data mMonitor;
extern TSettings Settings;
TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

void m5stackDisplay_Init(void)
{
  Serial.println("M5stack display driver initialized");
  M5.begin(); //Init M5Stack
  M5.Power.begin(); //Init power
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setFreeFont(FMB9);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.println("");
  M5.Lcd.println("         NerdMiner");
  M5.Lcd.drawLine(0,25,320,25,GREENYELLOW);
  M5.Lcd.fillRect(0,30,320,20,WHITE);
  M5.Lcd.println("");
  M5.Lcd.println("");
  M5.Lcd.println("HAN SOLO is a solo miner");
  M5.Lcd.println(" on a ESP32."); M5.Lcd.setTextColor(RED);
  M5.Lcd.println("WARNING: you may have to wait");
  M5.Lcd.println(" longer than the current age");
  M5.Lcd.println(" of the universe to find a ");
  M5.Lcd.println(" valid block."); M5.Lcd.setTextColor(WHITE);
  M5.Lcd.drawLine(0,200,320,200,GREENYELLOW);
  M5.Lcd.println("Connect via wifi to HanSoloAP");
  M5.Lcd.println(" with password MineYourCoins");
}

void m5stackDisplay_AlternateScreenState(void)
{
}

void m5stackDisplay_AlternateRotation(void)
{
}

void m5stackDisplay_progressBar(TFT_eSprite *spr, int x, int y, int w, int h, uint8_t val) {
    spr->drawRect(x, y, w, h, 0x09F1);
    spr->fillRect(x + 1, y + 1, w * (((float)val) / 100.0), h - 1, 0x09F1);
}

void m5stackDisplay_NoScreen(unsigned long mElapsed)
{
  spr.createSprite(M5.Lcd.width(), M5.Lcd.height());
  spr.setColorDepth(8);
  mining_data data = getMiningData(mElapsed);

  // Print hashrate to serial
  Serial.printf(">>> Completed %s share(s), %s Khashes, avg. hashrate %s KH/s\n",
                data.completedShares.c_str(), data.totalKHashes.c_str(), data.currentHashRate.c_str());
  Serial.printf(">>> Temperature: %s\n", data.temp.c_str());

  spr.setTextColor(WHITE);
  spr.setFreeFont(FMB9);
  spr.setCursor(0,0);
  spr.fillSprite(BLACK);
  spr.println("");
  spr.println("         NerdMiner");
  spr.drawLine(0,25,320,25,GREENYELLOW);
  spr.fillRect(0,30,320,20,WHITE);
  m5stackDisplay_progressBar(&spr, 0,30,320,20, data.currentHashRate.toInt());
  spr.println("");
  spr.println("");
  spr.print("Avg. hashrate : "); spr.setTextColor(GREEN); spr.print(data.currentHashRate); spr.setTextColor(WHITE); spr.println(" KH/s");
  spr.print("Running time  : "); spr.setTextColor(GREEN); spr.println(data.timeMining); spr.setTextColor(WHITE);
  spr.print("Total hashes  : "); spr.setTextColor(GREEN); spr.print(data.totalKHashes); spr.setTextColor(WHITE); spr.println(" KH");
  spr.print("Block templ.  : "); spr.setTextColor(YELLOW); spr.println(data.templates); spr.setTextColor(WHITE);
  spr.print("Best dificulty: "); spr.setTextColor(YELLOW); spr.println(data.bestDiff); spr.setTextColor(WHITE);
  spr.print("Shares 32bits : "); spr.setTextColor(YELLOW); spr.println(data.completedShares); spr.setTextColor(WHITE);
  spr.print("Valid blocks  : "); spr.setTextColor(RED); spr.println(data.valids); spr.setTextColor(WHITE);
  spr.println("");
  spr.drawLine(0,200,320,200,GREENYELLOW);
  spr.setTextColor(GREENYELLOW); spr.print(Settings.PoolAddress); spr.print(":"); spr.println(Settings.PoolPort); spr.setTextColor(WHITE);
  spr.setTextColor(GREENYELLOW); spr.println(WiFi.localIP()); spr.setTextColor(WHITE);
  spr.println("");

  spr.pushSprite(0,0);
}

void m5stackDisplay_LoadingScreen(void)
{
  Serial.println("Initializing...");
}

void m5stackDisplay_SetupScreen(void)
{
  Serial.println("Setup...");
}

void m5stackDisplay_DoLedStuff(unsigned long frame)
{
}

void m5stackDisplay_AnimateCurrentScreen(unsigned long frame)
{
}

CyclicScreenFunction m5stackDisplayCyclicScreens[] = {m5stackDisplay_NoScreen};

DisplayDriver m5stackDisplayDriver = {
    m5stackDisplay_Init,
    m5stackDisplay_AlternateScreenState,
    m5stackDisplay_AlternateRotation,
    m5stackDisplay_LoadingScreen,
    m5stackDisplay_SetupScreen,
    m5stackDisplayCyclicScreens,
    m5stackDisplay_AnimateCurrentScreen,
    m5stackDisplay_DoLedStuff,
    SCREENS_ARRAY_SIZE(m5stackDisplayCyclicScreens),
    0,
    0,
    0,
};
#endif
