//#include <iostream>
#ifndef GAMEGO
  #define GAMEGO
#include "MYQueue.h"
#include "Menu.h"
#include <AlmostRandom.h>
#include <Arduino_GFX_Library.h>
#include <TM1637.h> //для 4-digit display
#include <Adafruit_NeoPixel.h>

class GameGo{
public:
    //начало игры, инициализация
    GameGo(Arduino_GFX *gfx, Menu *menu);
    //Движение змейки по экрану
    void GoAndEat(Arduino_GFX *gfx);
    //функция используемая в check_food, нужная для проверки не находится ли еда в еде
    //или в теле змейки
    void check_food(int* PFX, int* PFY, int* another_PFX, int* another_PFY);
    //проверяем движение змеки
    void direction();
    //а вот змейка конец свой встретила
    void end(Arduino_GFX *gfx);
    //время на 7-ми сегментном дисплее
    void SetTime(TM1637 *tm1637);
    void  count_print_face(int action,Arduino_GFX *gfx); //1 - просто идет, 2 - ест
    int get_speed();
    void save_rainbow_time();
    void draw_face(int action, int X_1, int Y_1, int X_2, int Y_2, Arduino_GFX *gfx, int X_3, int Y_3);
    ~GameGo(){
    for(int i = 0; i<pixels.numPixels(); i++){
      pixels.setPixelColor(i, pixels.Color(256,256,256));
    }
    pixels.show();}
private:
    MYQueue<int> myLen;
    AlmostRandom ar;
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(24, PIN_LED, NEO_GRB + NEO_KHZ800);
    int X_joy, Y_joy, mpX=150, mpY=230;
    int PosFoodX=mpX, PosFoodY=mpY;
    int PosRNBFoodX=(ar.getRandomUInt()%300)/10*10, PosRNBFoodY=(ar.getRandomUInt()%470)/10*10;
    int way = 1;
    uint16_t color;
    int ind_colour_RNB=0;
    const uint16_t RAINBOW[7] = {RGB565_RED, RGB565_ORANGE, RGB565_YELLOW, RGB565_GREEN, RGB565_BLUE, RGB565_PURPLE, RGB565_PINK};
    unsigned long game_started, game_long, rainbow_time;
    bool have_i_ate_something = false;
};

GameGo::GameGo(Arduino_GFX *gfx, Menu *menu){
     
  gfx->fillScreen(RGB565_BLACK);
     
  gfx->fillCircle(PosFoodX+5, PosFoodY+5,3, RGB565_GREEN);
  while(PosRNBFoodX==PosFoodX && PosRNBFoodY == PosFoodY){
  PosRNBFoodX=(ar.getRandomUInt()%300)/10*10; PosRNBFoodY=(ar.getRandomUInt()%470)/10*10;
  }
     
  gfx->fillCircle(PosRNBFoodX+5, PosRNBFoodY+5,3, RGB565(0,230,255));
  color = menu->get_color();
  game_started = millis();
  pixels.begin(); 
  pixels.show();            
  pixels.setBrightness(50);
}
void GameGo::GoAndEat(Arduino_GFX *gfx){
  if(mpX==PosFoodX && mpY==PosFoodY){
    gfx->fillCircle(PosFoodX+5, PosFoodY+5, 3, RGB565_BLACK);
    check_food(&PosFoodX, &PosFoodY, &PosRNBFoodX, &PosRNBFoodY);
    gfx->fillCircle(PosFoodX+5, PosFoodY+5, 3, RGB565_GREEN);
    myLen.eat(mpX, mpY);
    count_print_face(3,gfx);
    if(have_i_ate_something){ gfx->drawRect(mpX, mpY, 10,10, RAINBOW[ind_colour_RNB]); ind_colour_RNB = (ind_colour_RNB+1)%7;}
    else  gfx->drawRect(mpX, mpY, 10,10, color);
    count_print_face(2, gfx);
    DoINeedToPlayThis = true;
    if(myLen.size()== 1500) gamemode = 4;
  }
  else if(mpX==PosRNBFoodX && mpY==PosRNBFoodY){
    gfx->fillCircle(PosRNBFoodX+5, PosRNBFoodY+5, 3, RGB565_BLACK);
    PosRNBFoodX = -1;PosRNBFoodY = -1;
     
    gfx->drawRect(mpX, mpY, 10,10, RAINBOW[ind_colour_RNB]); ind_colour_RNB = (ind_colour_RNB+1)%7;
     
    gfx->drawRect(myLen.get_f_x(0), myLen.get_f_y(0), 10,10, RGB565_BLACK);
    myLen.go(mpX,mpY);
    count_print_face(3,gfx);
    have_i_ate_something = true;
    count_print_face(2, gfx);
    save_rainbow_time();
    DoINeedToPlayThis = true;

  }
  else{
    if(have_i_ate_something){
      count_print_face(3,gfx);
      gfx->drawRect(mpX, mpY, 10,10, RAINBOW[ind_colour_RNB]); ind_colour_RNB = (ind_colour_RNB+1)%7;
      gfx->drawRect(myLen.get_f_x(0), myLen.get_f_y(0), 10,10, RGB565_BLACK);
      myLen.go(mpX,mpY);
      Serial.println(((millis()-rainbow_time)/1000ul%3600ul)%60ul);
      if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=10){
        have_i_ate_something = false;
        check_food(&PosRNBFoodX, &PosRNBFoodY, &PosFoodX, &PosFoodY);

        gfx->fillCircle(PosRNBFoodX+5, PosRNBFoodY+5, 3, RGB565(0,230,255));
      }
      else{
        if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=9)
        {
          pixels.setPixelColor(23, pixels.Color(256,256,256));
        }
        else if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=8)
        {
          pixels.setPixelColor(22, pixels.Color(256,256,256));
          pixels.setPixelColor(21, pixels.Color(256,256,256));
        }
        else if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=7)
        {
          pixels.setPixelColor(18, pixels.Color(256,256,256));
          pixels.setPixelColor(20, pixels.Color(256,256,256));
          pixels.setPixelColor(19, pixels.Color(256,256,256));
        }
        else if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=6)
        {
          pixels.setPixelColor(17, pixels.Color(256,256,256));
          pixels.setPixelColor(15, pixels.Color(256,256,256));
          pixels.setPixelColor(16, pixels.Color(256,256,256));
        }
        else if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=5)
        {
          pixels.setPixelColor(14, pixels.Color(256,256,256));
          pixels.setPixelColor(12, pixels.Color(256,256,256));
          pixels.setPixelColor(13, pixels.Color(256,256,256));
        }
        else if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=4)
        {
          pixels.setPixelColor(9, pixels.Color(256,256,256));
          pixels.setPixelColor(10, pixels.Color(256,256,256));
          pixels.setPixelColor(11, pixels.Color(256,256,256));
        }
        else if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=3)
        {
          pixels.setPixelColor(6, pixels.Color(256,256,256));
          pixels.setPixelColor(7, pixels.Color(256,256,256));
          pixels.setPixelColor(8, pixels.Color(256,256,256));
        }
        else if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=2)
        {
          pixels.setPixelColor(3, pixels.Color(256,256,256));
          pixels.setPixelColor(4, pixels.Color(256,256,256));
          pixels.setPixelColor(5, pixels.Color(256,256,256));
        }
        else if((((millis()-rainbow_time)/1000ul%3600ul)%60ul)>=1)
        {
          pixels.setPixelColor(1, pixels.Color(256,256,256));
          pixels.setPixelColor(2, pixels.Color(256,256,256));
        }
        else {
          pixels.setPixelColor(0, pixels.Color(256,256,256));
        }
        pixels.show();
      }
    }
    else{
     
      gfx->drawRect(mpX, mpY, 10,10, color);
     
      gfx->drawRect(myLen.get_f_x(0), myLen.get_f_y(0), 10,10, RGB565_BLACK);
      myLen.go(mpX,mpY);
      }
    count_print_face(3,gfx);
     count_print_face(1,gfx);
  }
}
void GameGo::check_food(int* PFX, int* PFY, int* another_PFX, int* another_PFY){
  *PFX=(ar.getRandomUInt()%300)/10*10; *PFY=(ar.getRandomUInt()%470)/10*10;
  bool *check = new bool(1);
  while(check){
    *check = true;
      for(int i = 0; i<=myLen.size();i++){
      if(*PFX==myLen.get_f_x(i) && *PFY==myLen.get_f_y(i)){
          *PFX=(ar.getRandomUInt()%300)/10*10; *PFY=(ar.getRandomUInt()%470)/10*10;
          *check = false;
      }
      }
      if(*PFX==*another_PFX && *PFY == *another_PFY){
        *PFX=(ar.getRandomUInt()%300)/10*10; *PFY=(ar.getRandomUInt()%470)/10*10;
        *check = false;
      }
      if(check) {delete check;break;}
  }
}
void GameGo::direction(){
        X_joy = analogRead(PinX); // считываем аналоговое значение оси Х
        Y_joy = analogRead(PinY); // считываем аналоговое значение оси Y

        if(way == 1 || way == 3){//шли вверх или вниз
            if(Y_joy<=400) way = 4; //повернули направо
            else if(Y_joy>=800) way = 2; //повернули налево
        }
        else {//изначально шли вправо или влево
            if(X_joy<=400) way = 1; //повернули вверх
            else if(X_joy>=800) way = 3; //повернули вниз
        }
        if(way == 1){//вверх 
            mpX+=10;
        }
        else if(way == 2){
            mpY-=10;
        }
        else if(way == 3){ //вниз
            mpX-=10;
        }
        else if(way == 4){
            mpY+=10;
        }
}
void GameGo::end(Arduino_GFX *gfx){
    if(mpX==-10||mpX==320||mpY==-10||mpY==480 ){
        for(int i = 0; i<3; i++)
        {
     
        gfx->drawRect(myLen.get_f_x(myLen.size()), myLen.get_f_y(myLen.size()), 10,10, RGB565_RED);
        delay(300);
     
        gfx->drawRect(myLen.get_f_x(myLen.size()), myLen.get_f_y(myLen.size()), 10,10, color);
        delay(300);
        }
        gamemode = 4;
    }
    else if((myLen.count(myLen.size())!=1)){
        for(int i = 0; i<3; i++)
        {
     
        gfx->drawRect(myLen.get_f_x(myLen.size()), myLen.get_f_y(myLen.size()), 10,10, RGB565_RED);
        delay(300);
     
        gfx->drawRect(myLen.get_f_x(myLen.size()), myLen.get_f_y(myLen.size()), 10,10, color);
        delay(300);
        }
        gamemode = 4;
    }
}
void GameGo::SetTime(TM1637 *tm1637){
  game_long = millis()-game_started;
  float *need;
  need = new float(((game_long/1000ul%3600ul)/60ul*100ul)+((game_long/1000ul%3600ul)%60ul));
  tm1637->displayNum(*need);
  delete need;
}
int GameGo::get_speed(){return (have_i_ate_something)?250:500;}
void GameGo::save_rainbow_time(){
  if(have_i_ate_something){
    rainbow_time = millis();
    for(int i = 0; i<pixels.numPixels(); i++){
      pixels.setPixelColor(i, pixels.Color(238,130,238));
    }
    pixels.show();
  }
}
void GameGo::count_print_face(int action,Arduino_GFX *gfx){
  if(action == 1 ||action == 3){//идет
    if(way == 1){//вверх
      draw_face(action, 3,3,3,7,gfx,0,0);}
    else if(way==2){//влево
      draw_face(action, 3,7,7,7,gfx,0,0);
    }
    else if(way==3)//вниз
    {
     draw_face(action, 7, 3, 7, 7,gfx,0,0);
    }
    else{//вправо
     draw_face(action, 3, 3, 7, 3,gfx,0,0);
    }
  }
  if(action == 2 || action == 4){//cъела
    if(way==1){
      draw_face(action, 3, 3, 3, 7,gfx, 7, 5);
    }
    else if(way==2){
      draw_face(action, 3,7,7, 7,gfx, 5,3);
    }
    else if(way==3){
      draw_face(action, 7,3,7, 7,gfx, 3,5);
    }
    else{
      draw_face(action, 3,3,7,3,gfx,5,7);
    }
    
  }
  
}
void GameGo::draw_face(int action,int X_1, int Y_1, int X_2, int Y_2, Arduino_GFX *gfx, int X_3, int Y_3){
  if(action == 1 || action == 2){//идет
      gfx->fillCircle(mpX+X_1,mpY+Y_1,2,RGB565_WHITE);
     
      gfx->fillCircle(mpX+X_1,mpY+Y_1,1,RGB565(0,0,0));
     
      gfx->fillCircle(mpX+X_2,mpY+Y_2,2,RGB565_WHITE);
     
      gfx->fillCircle(mpX+X_2,mpY+Y_2,1,RGB565(0,0,0));
  }
  if(action == 2){//cъела
    if(have_i_ate_something){gfx->fillCircle(mpX+X_3,mpY+Y_3,1,RAINBOW[ind_colour_RNB]);}
    else {gfx->fillCircle(mpX+X_3,mpY+Y_3,1, color);}
  }
  if (action == 3){//закрасить 
      gfx->fillRect(myLen.get_f_x(myLen.size()-1),myLen.get_f_y(myLen.size()-1),10,10,RGB565_BLACK);
      if(have_i_ate_something){gfx->drawRect(myLen.get_f_x(myLen.size()-1),myLen.get_f_y(myLen.size()-1),10,10,RAINBOW[(ind_colour_RNB+6)%7]);}
      else gfx->drawRect(myLen.get_f_x(myLen.size()-1),myLen.get_f_y(myLen.size()-1),10,10,color);
  }
}
#endif /*MYQUEUE*/