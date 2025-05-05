#include <Arduino_GFX_Library.h>
#include "defen.h"
#include "GameGo.h"
#include "Menu.h"
#include <TM1637.h> //для 4-digit display
#include "SD.h"
#include <AutoAnalogAudio.h>


#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */
Arduino_DataBus *bus = new Arduino_HWSPI(DF_GFX_DC,DF_GFX_CS );
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, DF_GFX_RST, 0 /* rotation */, false /* IPS */);
#endif /* !defined(DISPLAY_DEV_KIT) */


GameGo *go;
Menu *menu;
TM1637 *tm1637;
File path_for_bgm; 
File myFile_bgm;
File myFile_sem;
AutoAnalog aaAudio;
#include "PlayBgMusic.h"


void setup() {
  Serial.begin(9600);

  pinMode(SD_CS_PIN, OUTPUT);
  pinMode(DF_GFX_CS, OUTPUT);
  pinMode(GFX_BL, OUTPUT);
  
  digitalWrite(GFX_BL, HIGH);
  
  forLCD();
  gfx->begin(6000000);
  forSD();
  digitalWrite(DF_GFX_CS, HIGH);
  if (!SD.begin(SD_CS_PIN)){
    Serial.print("Error SD");
  }
  aaAudio.begin(0, 1); 
  aaAudio.autoAdjust = 0;
  //Serial.println("open playAudio");
  playAudio("game/music/");
  //Serial.println("close playAudio");
  forLCD();
  pinMode(PinX, INPUT);
  pinMode(PinY, INPUT);
  pinMode(SwitchPin, INPUT_PULLUP);
  menu = new Menu(gfx);
  tm1637 = new TM1637(time_CLK,time_DIO);
    tm1637->init();
    tm1637->set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
    tm1637->displayNum(0);    
}


void loop() {
//игра идет
  if(gamemode==3){ 
    go->GoAndEat(gfx);
    if(DoINeedToPlayThis){
      forSD();
      playSoundTrack("/game/sounds/nyam.wav");
      forLCD();
      DoINeedToPlayThis = false;
    }
    go->SetTime(tm1637);
    delay(go->get_speed());
    go->direction();
    go->end(gfx);
  }
//игра прекратилась 
  else if(gamemode == 4){
    forSD();
    playSoundTrack("/game/sounds/end.wav");
    forLCD();
    gfx->fillScreen(RGB565_GREEN);
    delete go;
    tm1637->displayStr("0000");
    gamemode = 0; 
  }
//меню рисуется
  else if(gamemode == 0){ 
    menu->print_main_things();
    gamemode = 1;
    set_menu_time = millis();
    RepeatAction =  new unsigned long;
    *RepeatAction = millis();
  }
//меню идет
  else if(gamemode == 1){
    if((((millis()-set_menu_time)/1000ul%3600ul)/60ul*100ul)<1){
      menu->if_clicked();
      if((millis()-*RepeatAction)>300){
        menu->dvig();
        menu->action_after_btn_clicked();
        if(DoINeedToPlayThis){
          forSD();
          playSoundTrack("/game/sounds/tap.wav");
          forLCD();
          DoINeedToPlayThis = false;
        }
        menu->direction();
        *RepeatAction = millis();
        digitalWrite(GFX_BL, HIGH);
      }
    }
    else {menu->direction(); digitalWrite(GFX_BL, LOW); aaAudio.disableDAC(); 
        if((((millis()-set_menu_time)/1000ul%3600ul)/60ul*100ul)<1) aaAudio.feedDAC(channelSelection);}
  }
//игра подгатавливается
  else if (gamemode == 2){
    go = new GameGo(gfx, menu);
    gamemode = 3;
    delete RepeatAction; 
  }
}