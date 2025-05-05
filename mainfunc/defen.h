#define DF_GFX_SCK 76
#define DF_GFX_MOSI 75
#define DF_GFX_MISO 74
#define DF_GFX_CS 52
#define DF_GFX_DC 28
#define DF_GFX_RST 30
#define DF_GFX_BL 32
#define GFX_BL DF_GFX_BL
#define time_DIO 7
#define time_CLK 6
#define PIN_LED 5 // номер порта к которому подключен модуль

#define SD_CS_PIN 4


const int PinX = A0;
const int PinY = A1;
const int SwitchPin = 50;


int gamemode = 0;
unsigned long set_menu_time;
unsigned long *RepeatAction;


void forLCD(){
  digitalWrite(SD_CS_PIN, HIGH);
  digitalWrite(DF_GFX_CS, LOW);
};
void forSD(){
  digitalWrite(SD_CS_PIN, LOW);
  digitalWrite(DF_GFX_CS, HIGH);
};

bool DoINeedToPlayThis = false;
int ShouldIStopMusic = 0;