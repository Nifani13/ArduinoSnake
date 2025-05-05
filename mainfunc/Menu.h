//#include <iostream>
#ifndef MENU
  #define MENU
#include <Arduino_GFX_Library.h>
class Menu{
public:
//конструктор инициализирующий пару вещей
    Menu(Arduino_GFX *g);
//выведение элементов интерфейса на экран
    void print_main_things();
//движение змейки
    void dvig();
//рисование самой змейки, используемое в print_main_things()
    void print_zmeu_menu(uint16_t color);
//считывание данных с джостика
    void direction();
//выделение объекта, с которым сейчас можно взаимодействовать
    void printMyPos(uint16_t color);
//если кнопка нажата
    void if_clicked();
//действие после нажатия кнопки
    bool action_after_btn_clicked();
//получить цвет змейки
    uint16_t get_color();
//деструктор
    ~Menu(){
        for(unsigned int i = 0; i < 6; ++i) {
            delete[] arr_menu[i];
        }
        delete[] arr_menu;
        delete[] color_of_zmeu;
    }
private:
    uint16_t* color_of_zmeu;
    int** arr_menu;
    int color_ind = 0;
    int where_am_i = 1; //0 - volume, 1-play, 2 - left, 3 -right
    Arduino_GFX *gfx;
    int X_joy, Y_joy;
    const uint16_t bg_c_menu = RGB565(52, 42, 63); //backgroung colour menu
    bool check_if_btn_clicked = false;
    int volume = 1; // 0 - выключено, 1 - включено
};
Menu::Menu(Arduino_GFX *g):gfx(g){
    color_of_zmeu = new uint16_t[10];
    color_of_zmeu[0] = RGB565(188,50,0);
    color_of_zmeu[1] = RGB565(255,128,0);
    color_of_zmeu[2] = RGB565_YELLOW;
    color_of_zmeu[3] = RGB565(159,236,43);
    color_of_zmeu[4] = RGB565(47,148,54);
    color_of_zmeu[5] = RGB565(132,238,238);
    color_of_zmeu[6] = RGB565(132,192,238);
    color_of_zmeu[7] = RGB565(7,85,240);
    color_of_zmeu[8] = RGB565(124, 83, 236);
    color_of_zmeu[9] = RGB565(185, 83, 236);
    arr_menu = new int*[6];
    for(int i = 0; i<6; i++){
        arr_menu[i] = new int[3];
    }
    arr_menu[0][0] = 230; arr_menu[0][1] = 150; arr_menu[0][2] = 0;//need in dvig()
    arr_menu[1][0] = 245; arr_menu[1][1] = 180; arr_menu[1][2] = 1;
    arr_menu[2][0] = 260; arr_menu[2][1] = 210; arr_menu[2][2] = 2;
    arr_menu[3][0] = 245; arr_menu[3][1] = 240; arr_menu[3][2] = 3;
    arr_menu[4][0] = 230; arr_menu[4][1] = 270; arr_menu[4][2] = 0;
    arr_menu[5][0] = 245; arr_menu[5][1] = 300; arr_menu[5][2] = 1;
}
void Menu::print_main_things(){
    digitalWrite(DF_GFX_CS, HIGH);
    gfx->fillScreen(bg_c_menu);
    //Rect
     
    gfx->fillRect(80, 120, 120, 240, RGB565(192, 192, 192));
     
    gfx->drawRect(80, 120, 120, 240, RGB565_WHITE);
     
    //P
     
    gfx->fillRect(105, 135, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(119, 135, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(133, 135, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(147, 135, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 135, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(133, 149, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 149, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(133, 163, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(147, 163, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 163, 14, 14, RGB565_WHITE);
     
    //L
    gfx->fillRect(105, 191, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(119, 191, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(133, 191, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(147, 191, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 191, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(105, 205, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(105, 219, 14, 14, RGB565_WHITE);
     
    //A
    gfx->fillRect(105, 247, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(119, 247, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(133, 247, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(147, 247, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 247, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(133, 261, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 261, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(105, 275, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(119, 275, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(133, 275, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(147, 275, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 275, 14, 14, RGB565_WHITE);
     
    //Y
    gfx->fillRect(147, 303, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 303, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(105, 317, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(119, 317, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(133, 317, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(147, 331, 14, 14, RGB565_WHITE);
     
    gfx->fillRect(161, 331, 14, 14, RGB565_WHITE);
    //стрелка влево
     
    gfx->fillTriangle(260, 80, 280, 120, 240, 120, RGB565_WHITE);
    //стрелка вправо
     
    gfx->fillTriangle(260, 400, 280, 360, 240, 360, RGB565_WHITE);
    //кномка регулирования громкости звука
     
    gfx->fillRect(10, 390, 60, 60, RGB565_WHITE);
    if (volume >= 0){
        gfx->fillRect(12, 392, 56, 56, RGB565_BLACK);
        gfx->fillRect(28, 396, 24, 9, RGB565_WHITE);
    }
    if (volume == 1){
    
        gfx->fillRect(28, 409, 24, 9, RGB565_WHITE);
        gfx->fillTriangle(52, 409, 52, 417, 56, 417, RGB565_WHITE);
        gfx->fillTriangle(28, 409, 28, 417, 24, 417, RGB565_WHITE);
        gfx->fillRect(24, 422, 54-22, 9, RGB565_WHITE);
        gfx->fillTriangle(56, 422, 56, 430, 60, 430, RGB565_WHITE);
        gfx->fillTriangle(24, 422, 24, 430, 20, 430, RGB565_WHITE);
        gfx->fillRect(20, 435, 54-14, 9, RGB565_WHITE);
        gfx->fillTriangle(60, 435, 60, 443, 64, 443, RGB565_WHITE);
        gfx->fillTriangle(20, 435, 20, 443, 16, 443, RGB565_WHITE);
    }
    //змейка
    for(int i = 0; i<6; i++){
        gfx->drawRect(arr_menu[i][0], arr_menu[i][1], 30, 30, color_of_zmeu[color_ind]);
    }
    gfx->fillCircle(arr_menu[5][0]+7,arr_menu[5][1]+7,7,RGB565_WHITE);
    gfx->fillCircle(arr_menu[5][0]+7,arr_menu[5][1]+7,3,RGB565(0,0,0));
    gfx->fillCircle(arr_menu[5][0]+22,arr_menu[5][1]+7,7,RGB565_WHITE);
    gfx->fillCircle(arr_menu[5][0]+22,arr_menu[5][1]+7,3,RGB565(0,0,0));
    //позиция
    printMyPos(RGB565(210, 105, 30));
}
void Menu::dvig(){
    gfx->fillCircle(arr_menu[5][0]+7,arr_menu[5][1]+7,7,bg_c_menu);
    gfx->fillCircle(arr_menu[5][0]+22,arr_menu[5][1]+7,7,bg_c_menu);
    for(int i = 0; i <6;i++){
     
        gfx->drawRect(arr_menu[i][0], arr_menu[i][1], 30, 30, bg_c_menu);
        if(arr_menu[i][2]==0) arr_menu[i][0]+=15;
        else if(arr_menu[i][2]==1) arr_menu[i][0]+=15;
        else if(arr_menu[i][2]==2) arr_menu[i][0]-=15;
        else arr_menu[i][0]-=15; //3
     
        gfx->drawRect(arr_menu[i][0], arr_menu[i][1], 30, 30, color_of_zmeu[color_ind]);
        arr_menu[i][2] = (arr_menu[i][2]+1)%4;   
    }
    gfx->fillCircle(arr_menu[5][0]+7,arr_menu[5][1]+7,7,RGB565_WHITE);
    gfx->fillCircle(arr_menu[5][0]+7,arr_menu[5][1]+7,3,RGB565(0,0,0));
    gfx->fillCircle(arr_menu[5][0]+22,arr_menu[5][1]+7,7,RGB565_WHITE);
    gfx->fillCircle(arr_menu[5][0]+22,arr_menu[5][1]+7,3,RGB565(0,0,0));
}
void Menu::direction(){
    X_joy = analogRead(PinX); // считываем аналоговое значение оси Х
    Y_joy = analogRead(PinY); // считываем аналоговое значение оси Y
    if(where_am_i==1){
        if(X_joy<=400 && Y_joy<=400){printMyPos(bg_c_menu); where_am_i = 3; set_menu_time = millis();}
        else if(X_joy<=400 && Y_joy>=400){printMyPos(bg_c_menu); where_am_i = 2;set_menu_time = millis();}
        else if (X_joy>=600){printMyPos(bg_c_menu); where_am_i = 0;set_menu_time = millis();}
    }
    else if(where_am_i==2){//left
        if(X_joy>=800 && Y_joy>400 && Y_joy<800){printMyPos(bg_c_menu); where_am_i = 1;set_menu_time = millis();}
        else if(Y_joy<=400 && X_joy>400 && X_joy<800){printMyPos(bg_c_menu); where_am_i = 3;set_menu_time = millis();}
    }
    else if(where_am_i==3){//right
        if(X_joy>=800 && Y_joy>400 && Y_joy<800){printMyPos(bg_c_menu); where_am_i = 1;set_menu_time = millis();}
        else if(Y_joy>=800 && X_joy>400 && X_joy<800){printMyPos(bg_c_menu); where_am_i = 2;set_menu_time = millis();}
    }
    else{//0
        if(X_joy<=400){printMyPos(bg_c_menu); where_am_i = 1;set_menu_time = millis();}
    }
    printMyPos(RGB565(210, 105, 30));
}
void Menu::printMyPos(uint16_t color){//play
    if(where_am_i == 1){
        for(int i = 1; i<=5; i++){
     
            gfx->drawRect(80-i, 120-i, 120+i*2, 240+i*2, color);
        }
    }
    else if(where_am_i == 2){ //left
        for(int i = 1; i<=5; i++){
     
            gfx->drawTriangle(260, 80-i, 280+i, 120+i, 240-i, 120+i, color);
        }
    }
    else if(where_am_i == 3){ //right
        for(int i = 1; i<=5; i++){
     
            gfx->drawTriangle(260, 400+i, 280+i, 360-i, 240-i, 360-i, color);
        }
    }
    else{
        for(int i = 1; i<=5; i++){
     
            gfx->drawRect(10-i, 390-i, 60+i*2, 60+i*2, color);
        }
    }
}
void Menu::if_clicked(){
    if(!digitalRead(SwitchPin)){
        check_if_btn_clicked = true;
    }
}
bool Menu::action_after_btn_clicked(){
    if(check_if_btn_clicked){
        if(where_am_i==1){
            gamemode = 2;
        }
        else if(where_am_i==2){ //left
            if(color_ind>0) color_ind--;
        }
        else if(where_am_i==3){ //right
            if(color_ind<9) color_ind++;
        }
        else{ //0
            if (volume == 0){
            gfx->fillRect(12, 392, 56, 56, RGB565_BLACK);
            gfx->fillRect(28, 396, 24, 9, RGB565_WHITE);
            ShouldIStopMusic = 1;
            }
            else {//3
            gfx->fillRect(28, 409, 24, 9, RGB565_WHITE);
            gfx->fillTriangle(52, 409, 52, 417, 56, 417, RGB565_WHITE);
            gfx->fillTriangle(28, 409, 28, 417, 24, 417, RGB565_WHITE);
            gfx->fillRect(24, 422, 54-22, 9, RGB565_WHITE);
            gfx->fillTriangle(56, 422, 56, 430, 60, 430, RGB565_WHITE);
            gfx->fillTriangle(24, 422, 24, 430, 20, 430, RGB565_WHITE);
            gfx->fillRect(20, 435, 54-14, 9, RGB565_WHITE);
            gfx->fillTriangle(60, 435, 60, 443, 64, 443, RGB565_WHITE);
            gfx->fillTriangle(20, 435, 20, 443, 16, 443, RGB565_WHITE);
            ShouldIStopMusic = 0;
            }
            volume = (volume + 1)%2;
        }
        check_if_btn_clicked = false;
        return !check_if_btn_clicked;
    }
    return check_if_btn_clicked;
}
uint16_t Menu::get_color(){
    return color_of_zmeu[color_ind];
}
#endif /*MENU*/