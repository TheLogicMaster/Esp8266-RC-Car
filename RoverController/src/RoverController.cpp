#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <vector>
#include <Adafruit_NeoPixel.h>
#include <DNSServer.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <RHReliableDatagram.h>

#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0
#define TEMPO 1

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int songPachabel[] = {
  NOTE_FS4,2, NOTE_E4,2,
  NOTE_D4,2, NOTE_CS4,2,
  NOTE_B3,2, NOTE_A3,2,
  NOTE_B3,2, NOTE_CS4,2,
  NOTE_FS4,2, NOTE_E4,2,
  NOTE_D4,2, NOTE_CS4,2,
  NOTE_B3,2, NOTE_A3,2,
  NOTE_B3,2, NOTE_CS4,2,
  NOTE_D4,2, NOTE_CS4,2,
  NOTE_B3,2, NOTE_A3,2,
  NOTE_G3,2, NOTE_FS3,2,
  NOTE_G3,2, NOTE_A3,2,

  NOTE_D4,4, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,4, NOTE_FS4,8, NOTE_G4,8, 
  NOTE_A4,4, NOTE_B3,8, NOTE_CS4,8, NOTE_D4,8, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, 
  NOTE_FS4,4, NOTE_D4,8, NOTE_E4,8, NOTE_FS4,4, NOTE_FS3,8, NOTE_G3,8,
  NOTE_A3,8, NOTE_G3,8, NOTE_FS3,8, NOTE_G3,8, NOTE_A3,2,
  NOTE_G3,4, NOTE_B3,8, NOTE_A3,8, NOTE_G3,4, NOTE_FS3,8, NOTE_E3,8, 
  NOTE_FS3,4, NOTE_D3,8, NOTE_E3,8, NOTE_FS3,8, NOTE_G3,8, NOTE_A3,8, NOTE_B3,8,

  NOTE_G3,4, NOTE_B3,8, NOTE_A3,8, NOTE_B3,4, NOTE_CS4,8, NOTE_D4,8,
  NOTE_A3,8, NOTE_B3,8, NOTE_CS4,8, NOTE_D4,8, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2,
  NOTE_A4,4, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,4,
  NOTE_FS4,8, NOTE_G4,8, NOTE_A4,8, NOTE_A3,8, NOTE_B3,8, NOTE_CS4,8,
  NOTE_D4,8, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_FS4,4, NOTE_D4,8, NOTE_E4,8,
  NOTE_FS4,8, NOTE_CS4,8, NOTE_A3,8, NOTE_A3,8,

  NOTE_CS4,4, NOTE_B3,4, NOTE_D4,8, NOTE_CS4,8, NOTE_B3,4,
  NOTE_A3,8, NOTE_G3,8, NOTE_A3,4, NOTE_D3,8, NOTE_E3,8, NOTE_FS3,8, NOTE_G3,8,
  NOTE_A3,8, NOTE_B3,4, NOTE_G3,4, NOTE_B3,8, NOTE_A3,8, NOTE_B3,4,
  NOTE_CS4,8, NOTE_D4,8, NOTE_A3,8, NOTE_B3,8, NOTE_CS4,8, NOTE_D4,8, NOTE_E4,8,
  NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2,  
};

int songMii[] = {
  
  // Mii Channel theme 
  // Score available at https://musescore.com/user/16403456/scores/4984153
  // Uploaded by Catalina Andrade 
  
  NOTE_FS4,8, REST,8, NOTE_A4,8, NOTE_CS5,8, REST,8,NOTE_A4,8, REST,8, NOTE_FS4,8, //1
  NOTE_D4,8, NOTE_D4,8, NOTE_D4,8, REST,8, REST,4, REST,8, NOTE_CS4,8,
  NOTE_D4,8, NOTE_FS4,8, NOTE_A4,8, NOTE_CS5,8, REST,8, NOTE_A4,8, REST,8, NOTE_F4,8,
  NOTE_E5,-4, NOTE_DS5,8, NOTE_D5,8, REST,8, REST,4,
  
  NOTE_GS4,8, REST,8, NOTE_CS5,8, NOTE_FS4,8, REST,8,NOTE_CS5,8, REST,8, NOTE_GS4,8, //5
  REST,8, NOTE_CS5,8, NOTE_G4,8, NOTE_FS4,8, REST,8, NOTE_E4,8, REST,8,
  NOTE_E4,8, NOTE_E4,8, NOTE_E4,8, REST,8, REST,4, NOTE_E4,8, NOTE_E4,8,
  NOTE_E4,8, REST,8, REST,4, NOTE_DS4,8, NOTE_D4,8, 

  NOTE_CS4,8, REST,8, NOTE_A4,8, NOTE_CS5,8, REST,8,NOTE_A4,8, REST,8, NOTE_FS4,8, //9
  NOTE_D4,8, NOTE_D4,8, NOTE_D4,8, REST,8, NOTE_E5,8, NOTE_E5,8, NOTE_E5,8, REST,8,
  REST,8, NOTE_FS4,8, NOTE_A4,8, NOTE_CS5,8, REST,8, NOTE_A4,8, REST,8, NOTE_F4,8,
  NOTE_E5,2, NOTE_D5,8, REST,8, REST,4,

  NOTE_B4,8, NOTE_G4,8, NOTE_D4,8, NOTE_CS4,4, NOTE_B4,8, NOTE_G4,8, NOTE_CS4,8, //13
  NOTE_A4,8, NOTE_FS4,8, NOTE_C4,8, NOTE_B3,4, NOTE_F4,8, NOTE_D4,8, NOTE_B3,8,
  NOTE_E4,8, NOTE_E4,8, NOTE_E4,8, REST,4, REST,4, NOTE_AS4,4,
  NOTE_CS5,8, NOTE_D5,8, NOTE_FS5,8, NOTE_A5,8, REST,8, REST,4, 

  REST,2, NOTE_A3,4, NOTE_AS3,4, //17 
  NOTE_A3,-4, NOTE_A3,8, NOTE_A3,2,
  REST,4, NOTE_A3,8, NOTE_AS3,8, NOTE_A3,8, NOTE_F4,4, NOTE_C4,8,
  NOTE_A3,-4, NOTE_A3,8, NOTE_A3,2,

  REST,2, NOTE_B3,4, NOTE_C4,4, //21
  NOTE_CS4,-4, NOTE_C4,8, NOTE_CS4,2,
  REST,4, NOTE_CS4,8, NOTE_C4,8, NOTE_CS4,8, NOTE_GS4,4, NOTE_DS4,8,
  NOTE_CS4,-4, NOTE_DS4,8, NOTE_B3,1,
  
  NOTE_E4,4, NOTE_E4,4, NOTE_E4,4, REST,8,//25

  //repeats 1-25

  NOTE_FS4,8, REST,8, NOTE_A4,8, NOTE_CS5,8, REST,8,NOTE_A4,8, REST,8, NOTE_FS4,8, //1
  NOTE_D4,8, NOTE_D4,8, NOTE_D4,8, REST,8, REST,4, REST,8, NOTE_CS4,8,
  NOTE_D4,8, NOTE_FS4,8, NOTE_A4,8, NOTE_CS5,8, REST,8, NOTE_A4,8, REST,8, NOTE_F4,8,
  NOTE_E5,-4, NOTE_DS5,8, NOTE_D5,8, REST,8, REST,4,
  
  NOTE_GS4,8, REST,8, NOTE_CS5,8, NOTE_FS4,8, REST,8,NOTE_CS5,8, REST,8, NOTE_GS4,8, //5
  REST,8, NOTE_CS5,8, NOTE_G4,8, NOTE_FS4,8, REST,8, NOTE_E4,8, REST,8,
  NOTE_E4,8, NOTE_E4,8, NOTE_E4,8, REST,8, REST,4, NOTE_E4,8, NOTE_E4,8,
  NOTE_E4,8, REST,8, REST,4, NOTE_DS4,8, NOTE_D4,8, 

  NOTE_CS4,8, REST,8, NOTE_A4,8, NOTE_CS5,8, REST,8,NOTE_A4,8, REST,8, NOTE_FS4,8, //9
  NOTE_D4,8, NOTE_D4,8, NOTE_D4,8, REST,8, NOTE_E5,8, NOTE_E5,8, NOTE_E5,8, REST,8,
  REST,8, NOTE_FS4,8, NOTE_A4,8, NOTE_CS5,8, REST,8, NOTE_A4,8, REST,8, NOTE_F4,8,
  NOTE_E5,2, NOTE_D5,8, REST,8, REST,4,

  NOTE_B4,8, NOTE_G4,8, NOTE_D4,8, NOTE_CS4,4, NOTE_B4,8, NOTE_G4,8, NOTE_CS4,8, //13
  NOTE_A4,8, NOTE_FS4,8, NOTE_C4,8, NOTE_B3,4, NOTE_F4,8, NOTE_D4,8, NOTE_B3,8,
  NOTE_E4,8, NOTE_E4,8, NOTE_E4,8, REST,4, REST,4, NOTE_AS4,4,
  NOTE_CS5,8, NOTE_D5,8, NOTE_FS5,8, NOTE_A5,8, REST,8, REST,4, 

  REST,2, NOTE_A3,4, NOTE_AS3,4, //17 
  NOTE_A3,-4, NOTE_A3,8, NOTE_A3,2,
  REST,4, NOTE_A3,8, NOTE_AS3,8, NOTE_A3,8, NOTE_F4,4, NOTE_C4,8,
  NOTE_A3,-4, NOTE_A3,8, NOTE_A3,2,

  REST,2, NOTE_B3,4, NOTE_C4,4, //21
  NOTE_CS4,-4, NOTE_C4,8, NOTE_CS4,2,
  REST,4, NOTE_CS4,8, NOTE_C4,8, NOTE_CS4,8, NOTE_GS4,4, NOTE_DS4,8,
  NOTE_CS4,-4, NOTE_DS4,8, NOTE_B3,1,
  
  NOTE_E4,4, NOTE_E4,4, NOTE_E4,4, REST,8,//25

  //finishes with 26
  //NOTE_FS4,8, REST,8, NOTE_A4,8, NOTE_CS5,8, REST,8, NOTE_A4,8, REST,8, NOTE_FS4,8
   
};

int songTetris[] = {

  //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  

  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,

};

int songKeyboardCat[] = {

  // Keyboard cat
  // Score available at https://musescore.com/user/142788/scores/147371

    REST,1,
    REST,1,
    NOTE_C4,4, NOTE_E4,4, NOTE_G4,4, NOTE_E4,4, 
    NOTE_C4,4, NOTE_E4,8, NOTE_G4,-4, NOTE_E4,4,
    NOTE_A3,4, NOTE_C4,4, NOTE_E4,4, NOTE_C4,4,
    NOTE_A3,4, NOTE_C4,8, NOTE_E4,-4, NOTE_C4,4,
    NOTE_G3,4, NOTE_B3,4, NOTE_D4,4, NOTE_B3,4,
    NOTE_G3,4, NOTE_B3,8, NOTE_D4,-4, NOTE_B3,4,

    NOTE_G3,4, NOTE_G3,8, NOTE_G3,-4, NOTE_G3,8, NOTE_G3,4, 
    NOTE_G3,4, NOTE_G3,4, NOTE_G3,8, NOTE_G3,4,
    NOTE_C4,4, NOTE_E4,4, NOTE_G4,4, NOTE_E4,4, 
    NOTE_C4,4, NOTE_E4,8, NOTE_G4,-4, NOTE_E4,4,
    NOTE_A3,4, NOTE_C4,4, NOTE_E4,4, NOTE_C4,4,
    NOTE_A3,4, NOTE_C4,8, NOTE_E4,-4, NOTE_C4,4,
    NOTE_G3,4, NOTE_B3,4, NOTE_D4,4, NOTE_B3,4,
    NOTE_G3,4, NOTE_B3,8, NOTE_D4,-4, NOTE_B3,4,

    NOTE_G3,-1, 
  
};

int songTakeOnMe[] = {

  // Take on me, by A-ha
  // Score available at https://musescore.com/user/190926/scores/181370
  // Arranged by Edward Truong

  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  NOTE_FS5,8, NOTE_FS5,8,NOTE_D5,8, NOTE_B4,8, REST,8, NOTE_B4,8, REST,8, NOTE_E5,8, 
  REST,8, NOTE_E5,8, REST,8, NOTE_E5,8, NOTE_GS5,8, NOTE_GS5,8, NOTE_A5,8, NOTE_B5,8,
  
  NOTE_A5,8, NOTE_A5,8, NOTE_A5,8, NOTE_E5,8, REST,8, NOTE_D5,8, REST,8, NOTE_FS5,8, 
  REST,8, NOTE_FS5,8, REST,8, NOTE_FS5,8, NOTE_E5,8, NOTE_E5,8, NOTE_FS5,8, NOTE_E5,8,
  
};

int songPanther[] = {

  // Pink Panther theme
  // Score available at https://musescore.com/benedictsong/the-pink-panther
  // Theme by Masato Nakamura, arranged by Teddy Mason

  REST,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,2, REST,4, REST,8, NOTE_DS4,4,

  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_G4,8, NOTE_B4,-8, NOTE_E5,8,
  NOTE_DS5,1,   
  NOTE_D5,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, 
  NOTE_E4,-4, REST,4,
  REST,4, NOTE_E5,-8, NOTE_D5,8, NOTE_B4,-8, NOTE_A4,8, NOTE_G4,-8, NOTE_E4,-8,
  NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8, NOTE_AS4,16, NOTE_A4,-8,   
  NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, NOTE_E4,16, NOTE_E4,16, NOTE_E4,2,
 
};

int songMario[] = {

  // Super Mario Bros theme
  // Score available at https://musescore.com/user/2123/scores/2145
  // Theme by Koji Kondo
  
  
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,8, //1
  NOTE_G5,4, REST,4, NOTE_G4,8, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 3
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4,NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  
  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  REST,4, NOTE_G5,8, NOTE_FS5,8, NOTE_F5,8, NOTE_DS5,4, NOTE_E5,8,//repeats from 7
  REST,8, NOTE_GS4,8, NOTE_A4,8, NOTE_C4,8, REST,8, NOTE_A4,8, NOTE_C5,8, NOTE_D5,8,
  REST,4, NOTE_DS5,4, REST,8, NOTE_D5,-4,
  NOTE_C5,2, REST,2,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4,//11
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,8,//13
  REST,1, 
  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4,
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,4,
  NOTE_G5,4, REST,4, NOTE_G4,4, REST,4, 
  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // 19
  
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4, NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  NOTE_C5,-4, NOTE_G4,8, REST,4, NOTE_E4,-4, // repeats from 19
  NOTE_A4,4, NOTE_B4,4, NOTE_AS4,8, NOTE_A4,4,
  NOTE_G4,-8, NOTE_E5,-8, NOTE_G5,-8, NOTE_A5,4, NOTE_F5,8, NOTE_G5,8,
  REST,8, NOTE_E5,4, NOTE_C5,8, NOTE_D5,8, NOTE_B4,-4,

  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,//23
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //26
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,

  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,//repeats from 23
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //26
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,8, NOTE_E5,8,
  REST,1,

  NOTE_C5,8, NOTE_C5,4, NOTE_C5,8, REST,8, NOTE_C5,8, NOTE_D5,4, //33
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2,
  NOTE_E5,8, NOTE_E5,8, REST,8, NOTE_E5,8, REST,8, NOTE_C5,8, NOTE_E5,4,
  NOTE_G5,4, REST,4, NOTE_G4,4, REST,4, 
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_D5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_A5,-8, NOTE_G5,-8, NOTE_F5,-8,
  
  NOTE_E5,8, NOTE_C5,4, NOTE_A4,8, NOTE_G4,2, //40
  NOTE_E5,8, NOTE_C5,4, NOTE_G4,8, REST,4, NOTE_GS4,4,
  NOTE_A4,8, NOTE_F5,4, NOTE_F5,8, NOTE_A4,2,
  NOTE_B4,8, NOTE_F5,4, NOTE_F5,8, NOTE_F5,-8, NOTE_E5,-8, NOTE_D5,-8,
  NOTE_C5,8, NOTE_E4,4, NOTE_E4,8, NOTE_C4,2,
  
  //game over sound
  NOTE_C5,-4, NOTE_G4,-4, NOTE_E4,4, //45
  NOTE_A4,-8, NOTE_B4,-8, NOTE_A4,-8, NOTE_GS4,-8, NOTE_AS4,-8, NOTE_GS4,-8,
  NOTE_G4,8, NOTE_D4,8, NOTE_E4,-2,  

};

// https://musescore.com/user/206457/scores/1032696
int songDragonBallSuper[] = {
  NOTE_FS3, 2, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_FS4, 2, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_G3, 2,
  NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_G3, 2, REST, 16, NOTE_B4, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_FS4, 2, REST, 16,
  NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_FS4, 2, REST, 16, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_G3, 2, REST, 16,
  NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_G3, 2, REST, 16, NOTE_B4, 8, NOTE_A4, 8, NOTE_B4, 4,
  // Melody
  NOTE_FS4, -2, NOTE_FS4, 4, NOTE_E4, 4, NOTE_D4, 4, NOTE_CS4, 4, NOTE_D4, 4, NOTE_B3, 1, REST, -4, 
  NOTE_FS4, 8, NOTE_G4, 4, NOTE_A4, -4, NOTE_G4, 4, NOTE_FS4, 8, NOTE_E4, 4, NOTE_G4, -4, NOTE_FS4, 4, NOTE_E4, 8, NOTE_D4, 4, NOTE_D4, 1,
  REST, 1, NOTE_A4, -2, NOTE_A4, 4, NOTE_E4, 4, NOTE_D4, 4, NOTE_CS4, 4, NOTE_D4, 4, NOTE_B3, 1, REST, 2, NOTE_FS4, 8, NOTE_G4, 4, NOTE_A4, -4, 
  NOTE_G4, 4, NOTE_FS4, 8, NOTE_E4, 4, NOTE_G4, -4, NOTE_FS4, 4, NOTE_E4, 8, NOTE_D4, 4, NOTE_D4, 1, REST, -2, NOTE_A4, -4, NOTE_A4, 4,
  NOTE_A4, 8, NOTE_A4, 4, NOTE_A4, -4, NOTE_G4, 4, NOTE_FS4, 8, NOTE_G4, -4, NOTE_A4, -1, REST, 4, 
  NOTE_D4, 2, NOTE_C4, 2, NOTE_AS3, 2, NOTE_AS3, 2, NOTE_E4, 2, NOTE_D4, 2, NOTE_C4, 2, NOTE_D4, 4, REST, 4,
  NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 8, NOTE_D4, 4, NOTE_D4, -4, NOTE_CS4, 4, REST, 4, NOTE_CS4, 4, REST, 4,
  NOTE_CS4, 4, NOTE_CS4, 8, NOTE_CS4, 4, NOTE_CS4, 4, NOTE_CS4, 8, NOTE_D4, 2, NOTE_B3, 2,
  NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 8, NOTE_CS4, -4, NOTE_B3, -1, NOTE_B3, 4, NOTE_CS4, 4, NOTE_D4, 2, NOTE_B3, 2, NOTE_CS4, 2, NOTE_D4, -4, REST, 8,
  NOTE_D4, 2, NOTE_A3, 2, NOTE_FS4, 2, REST, 2, NOTE_D4, 4, NOTE_B3, 4, NOTE_CS4, 8, NOTE_D4, 4, REST, 8, 
  NOTE_D4, 4, NOTE_B3, 4, NOTE_CS4, 8, NOTE_D4, 4, REST, 8, NOTE_AS3, 1, NOTE_CS4, 1, 
  NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 8, NOTE_D4, 4, NOTE_D4, -4, NOTE_CS4, 4, REST, 4, NOTE_CS4, 4, REST, 4,
  NOTE_CS4, 4, NOTE_CS4, 8, NOTE_CS4, 4, NOTE_CS4, 4, NOTE_CS4, 8, NOTE_D4, 2, NOTE_B3, 2,
  NOTE_D4, 4, NOTE_D4, 4, NOTE_D4, 8, NOTE_CS4, -4, NOTE_B3, -1, NOTE_B3, 4, NOTE_CS4, 4, NOTE_D4, 4, NOTE_CS4, 4, NOTE_B3, 4, NOTE_D4, 4,
  NOTE_CS4, 4, NOTE_CS4, 8, NOTE_CS4, -4, NOTE_D4, 8, NOTE_D4, 2, REST, 8, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_FS4, -2, 
  NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_E4, -2, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_E4, -2, NOTE_B3, 8, NOTE_A4, 8, NOTE_B3, 8, NOTE_FS4, -1
  };

// https://musescore.com/user/8852331/scores/3739996
int songSpongebobMedley[] = {
// Theme Song
NOTE_D4, 12, NOTE_E4, 12, NOTE_D4, 12, NOTE_B3, 12, NOTE_G3, 12, NOTE_B3, 12, 
NOTE_D4, 12, NOTE_E4, 12, NOTE_D4, 12, NOTE_B3, 4, NOTE_G4, 4, NOTE_D4, 4, NOTE_B3, 4, NOTE_G3, 4, 
NOTE_G4, 12, NOTE_A4, 12, NOTE_G4, 12, NOTE_E4, 12, NOTE_C4, 12, NOTE_E4, 12, 
NOTE_G4, 12, NOTE_A4, 12, NOTE_G4, 12, NOTE_E4, 4, NOTE_C5, 4, NOTE_G4, 4, NOTE_E4, 4, NOTE_C4, 4, 
// Repeat
NOTE_D4, 12, NOTE_E4, 12, NOTE_D4, 12, NOTE_B3, 12, NOTE_G3, 12, NOTE_B3, 12, 
NOTE_D4, 12, NOTE_E4, 12, NOTE_D4, 12, NOTE_B3, 4, NOTE_G4, 4, NOTE_D4, 4, NOTE_B3, 4, NOTE_G3, 4, 
NOTE_G4, 12, NOTE_A4, 12, NOTE_G4, 12, NOTE_E4, 12, NOTE_C4, 12, NOTE_E4, 12, 
NOTE_G4, 12, NOTE_A4, 12, NOTE_G4, 12, NOTE_E4, 4, NOTE_C5, 4, NOTE_G4, 4, NOTE_E4, 4, NOTE_C4, 4, 
// Continue
NOTE_G4, 4, NOTE_E4, 4, NOTE_D4, 4, NOTE_B3, 4, NOTE_G4, 4, NOTE_E4, 4, NOTE_D4, 4, NOTE_B3, 4,
NOTE_G4, 4, NOTE_E4, 4, NOTE_D4, 4, NOTE_B3, 4, NOTE_D4, -8, NOTE_D4, 16, NOTE_E4, 2, NOTE_FS4, 4, 
NOTE_G4, -8, NOTE_G4, 16, NOTE_FS4, 12, NOTE_G4, 12, NOTE_FS4, 12, NOTE_E4, 12, NOTE_FS4, 12, NOTE_E4, 12, 
NOTE_D4, 12, NOTE_E4, 12, NOTE_D4, 12, NOTE_C4, 12, NOTE_D4, 12, NOTE_C4, 12, 
NOTE_B3, 12, NOTE_C4, 12, NOTE_B3, 12, NOTE_A3, 12, NOTE_B3, 12, NOTE_A3, 12, NOTE_G3, 4, 
NOTE_G4, 12, NOTE_A4, 12, NOTE_B4, 12, NOTE_A4, -8, NOTE_B4, 16, NOTE_G4, -8, NOTE_D4, 16, NOTE_G4, 4, REST, 2,

// F.U.N. Song
NOTE_C4, 8, NOTE_C4, 4, NOTE_D4, 8, NOTE_E4, 4, NOTE_C4, 4, NOTE_A3, 8, NOTE_A3, 4, NOTE_B3, 8, NOTE_C4, 4, NOTE_D4, 4, 
NOTE_C4, 8, NOTE_C4, 4, NOTE_D4, 8, NOTE_E4, 4, NOTE_G4, 4, NOTE_D4, 4, NOTE_G4, -8, NOTE_A4, 16, NOTE_G4, -8, NOTE_F4, 16, NOTE_E4, -8, NOTE_D4, 16,
NOTE_C4, 4, NOTE_C4, -8, NOTE_D4, 16, NOTE_E4, -8, NOTE_D4, 16, NOTE_C4, 4, NOTE_F4, -8, NOTE_F4, 16, NOTE_F4, -8, NOTE_G4, 16, 
NOTE_A4, 4, NOTE_A4, 4, NOTE_G4, 8, NOTE_G4, 4, NOTE_A4, 8, NOTE_E4, 4, NOTE_D4, 4, NOTE_C4, 4, REST, -2,
// Repeat
NOTE_C4, 8, NOTE_C4, 4, NOTE_D4, 8, NOTE_E4, 4, NOTE_C4, 4, NOTE_A3, 8, NOTE_A3, 4, NOTE_B3, 8, NOTE_C4, 4, NOTE_D4, 4, 
NOTE_C4, 8, NOTE_C4, 4, NOTE_D4, 8, NOTE_E4, 4, NOTE_G4, 4, NOTE_D4, 4, NOTE_G4, -8, NOTE_A4, 16, NOTE_G4, -8, NOTE_F4, 16, NOTE_E4, -8, NOTE_D4, 16,
NOTE_C4, 4, NOTE_C4, -8, NOTE_D4, 16, NOTE_E4, -8, NOTE_D4, 16, NOTE_C4, 4, NOTE_F4, -8, NOTE_F4, 16, NOTE_F4, -8, NOTE_G4, 16, 
NOTE_A4, 4, NOTE_A4, 4, NOTE_G4, 8, NOTE_G4, 4, NOTE_A4, 8, NOTE_E4, 4, NOTE_D4, 4, NOTE_C4, 4, REST, -2,

// Krusty Krab Theme (Drunken Sailor)
NOTE_A4, 8, NOTE_A4, 16, NOTE_A4, 16, NOTE_A4, 8, NOTE_A4, 16, NOTE_A4, 16, NOTE_A4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_A4, 8, 
NOTE_G4, 8, NOTE_G4, 16, NOTE_G4, 16, NOTE_G4, 8, NOTE_G4, 16, NOTE_G4, 16, NOTE_G4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_G4, 8, 
NOTE_A4, 8, NOTE_A4, 16, NOTE_A4, 16, NOTE_A4, 8, NOTE_A4, 16, NOTE_A4, 16, NOTE_A4, 8, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 8, 
NOTE_C5, 8, NOTE_A4, 8, NOTE_G4, 8, NOTE_E4, 8, NOTE_D4, 4, NOTE_D4, 4,
// Repeat
NOTE_A4, 8, NOTE_A4, 16, NOTE_A4, 16, NOTE_A4, 8, NOTE_A4, 16, NOTE_A4, 16, NOTE_A4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_A4, 8, 
NOTE_G4, 8, NOTE_G4, 16, NOTE_G4, 16, NOTE_G4, 8, NOTE_G4, 16, NOTE_G4, 16, NOTE_G4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_G4, 8, 
NOTE_A4, 8, NOTE_A4, 16, NOTE_A4, 16, NOTE_A4, 8, NOTE_A4, 16, NOTE_A4, 16, NOTE_A4, 8, NOTE_B4, 8, NOTE_C5, 8, NOTE_D5, 8, 
NOTE_C5, 8, NOTE_A4, 8, NOTE_G4, 8, NOTE_E4, 8, NOTE_D4, 2,

// Smallest Violin Song (Woe is Me)
NOTE_F4, 32, NOTE_AS3, 32, NOTE_DS4, 32, NOTE_F4, 32, REST, -4, NOTE_F5, 32, NOTE_AS4, 32, NOTE_DS5, 32, NOTE_F5, 32, REST, -4, 
NOTE_F6, 32, NOTE_AS5, 32, NOTE_DS6, 32, NOTE_F6, 32, REST, -4, REST, 2, NOTE_G4, 2, NOTE_C4, 2, NOTE_GS4, -4, NOTE_G4, 8, NOTE_F4, 4, NOTE_DS4, 4,
NOTE_D4, 4, NOTE_AS3, 4, NOTE_AS4, 4, NOTE_GS4, 4, NOTE_G4, -4, REST, -4, 
NOTE_DS4, 8, NOTE_D4, -4, NOTE_C4, 2,

// Sweet Victory
NOTE_DS4, -8, NOTE_D4, -8, NOTE_AS3, 8, NOTE_C4, 2, NOTE_DS4, -8, NOTE_D4, -8, NOTE_AS3, 8, NOTE_C4, 2, NOTE_DS4, -8, NOTE_D4, -8, NOTE_AS3, 8, NOTE_C4, 2, 
REST, 2, NOTE_C4, -4, NOTE_G3, 8, NOTE_AS3, 8, NOTE_C4, 4, NOTE_C4, 2, REST, 8, REST, 1, 
REST, 4, NOTE_D4, 8, NOTE_DS4, 8, NOTE_F4, 4, NOTE_DS4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_AS3, 8, NOTE_C4, 4, REST, 8, NOTE_C4, 8, NOTE_DS4, -8, NOTE_F4, 16,
NOTE_F4, 8, NOTE_DS4, 8, NOTE_C4, 4, REST, -1, NOTE_D4, 8, NOTE_DS4, 8, NOTE_F4, 4, NOTE_DS4, 8, NOTE_D4, 8, REST, 8, NOTE_G4, 8, NOTE_G4, 2, REST, 1,
NOTE_G4, -8, NOTE_F4, -8, NOTE_DS4, 8, NOTE_F4, 2, NOTE_F4, -8, NOTE_G4, -8, NOTE_GS4, 8, NOTE_G4, 2, NOTE_G4, -8, NOTE_F4, -8, NOTE_DS4, 8, NOTE_F4, 2, 
NOTE_G4, 2, NOTE_F4, -4, NOTE_DS4, 8, NOTE_DS4, 2, NOTE_DS4, 8, NOTE_F4, -4, NOTE_G4, 1, NOTE_G4, -2, NOTE_G4, 4, NOTE_F4, 4, NOTE_DS4, 8, NOTE_DS4, 2, REST, 8,
REST, 2, NOTE_DS4, 8, NOTE_F4, -4, NOTE_G4, 1, NOTE_G4, -2, NOTE_G4, 4, NOTE_F4, 4, NOTE_DS4, 8, NOTE_DS4, 2, REST, 8, REST, 2,
NOTE_AS3, 6, NOTE_C4, 6, NOTE_DS4, 6, NOTE_G4, 1,

// Campfire Song Song
NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_C4, 8, NOTE_A3, 8, NOTE_C4, 2, 
NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_G4, 8, NOTE_E4, 4,
NOTE_D4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_C4, 16, NOTE_C4, 16, 
NOTE_C4, 8, NOTE_A3, 8, NOTE_C4, 8, REST, 16, NOTE_G3, 16, NOTE_A3, 8, NOTE_G3, 8, NOTE_E4, 8, NOTE_D4, 4, NOTE_C4, 8, NOTE_E4, 8, NOTE_D4, 4, NOTE_C4, 8,
NOTE_D4, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_B3, 4,
// Repeat
NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_C4, 8, NOTE_A3, 8, NOTE_C4, 2, 
NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_G4, 8, NOTE_E4, 4,
NOTE_D4, 8, NOTE_C4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_D4, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_C4, 16, NOTE_C4, 16, 
NOTE_C4, 8, NOTE_A3, 8, NOTE_C4, 8, REST, 16, NOTE_G3, 16, NOTE_A3, 8, NOTE_G3, 8, NOTE_E4, 8, NOTE_D4, 4, NOTE_C4, 8, NOTE_E4, 8, NOTE_D4, 4, NOTE_C4, 8,
NOTE_D4, 4, NOTE_G3, 4, NOTE_A3, 4, NOTE_B3, 4,

// Striped Sweater
NOTE_G4, -2, NOTE_G4, 8, NOTE_G4, 8, NOTE_A3, 8, NOTE_G4, 8, NOTE_F4, 4, NOTE_G4, 8, NOTE_B4, 8, NOTE_G4, 8, NOTE_B4, 8, 
NOTE_C5, 4, NOTE_C5, 8, NOTE_C5, 4, NOTE_C5, 8, NOTE_C5, 4, NOTE_GS4, 4, NOTE_G4, 4, REST, 2, 
NOTE_B3, 8, NOTE_D3, 8, NOTE_E3, 8, NOTE_G3, 8, NOTE_B4, 8, NOTE_D4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_B5, 8, NOTE_D5, 8, NOTE_E5, 8, NOTE_G5, 8, REST, 2,
NOTE_B4, 8, NOTE_C5, 4, NOTE_C5, 4, NOTE_B4, 8, NOTE_C5, 4, NOTE_B4, 8, NOTE_C5, 4, NOTE_C5, 8, NOTE_E4, -4, NOTE_E4, 8, NOTE_G4, 4, NOTE_A4, 8, NOTE_B4, 2, 
REST, 8, NOTE_D4, 4, NOTE_C4, 4, NOTE_B3, 4, 

// Ripped Pants
NOTE_C4, 4, NOTE_C4, 8, NOTE_C4, 8, NOTE_C4, 8, NOTE_C4, 8, NOTE_D4, 8, NOTE_E4, 8, NOTE_D4, 8, NOTE_C4, 4, NOTE_C4, 2, REST, 8, 
NOTE_A3, 4, NOTE_D4, 4, NOTE_E4, 4, NOTE_F4, 4, NOTE_E4, 8, NOTE_D4, 4, NOTE_D4, 4, REST, 8, NOTE_C4, 8, NOTE_B3, 8, NOTE_B3, 4, NOTE_A4, 4, NOTE_G4, 4, NOTE_F4, 8,
NOTE_F4, 8, NOTE_E4, 4, NOTE_D4, 8, NOTE_CS4, 4, NOTE_D4, 8, NOTE_E4, 8, NOTE_F4, 4, NOTE_A3, 4, NOTE_A3, 4, NOTE_D4, 4, 
REST, 4, NOTE_C4, 4, NOTE_B3, 4, NOTE_A3, 4, NOTE_G3, 1,

// Goofy Goober
NOTE_G4, -4, NOTE_G4, 8, NOTE_G4, 4, NOTE_A4, 4, NOTE_G4, 4, NOTE_E4, 4, NOTE_C4, 2, NOTE_C5, 4, NOTE_C5, 4, NOTE_C5, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 4,
NOTE_F4, 2, NOTE_C3, 8, NOTE_F3, 8, NOTE_A3, 8, NOTE_C4, 8, NOTE_B3, 2, 

// Credits Theme
NOTE_B3, -8, NOTE_AS3, 16, NOTE_B3, -8, NOTE_CS4, 16, NOTE_B3, 2, NOTE_E3, 4, NOTE_GS3, 4, NOTE_B3, 4, NOTE_GS3, 4, 
NOTE_B3, -8, NOTE_AS3, 16, NOTE_B3, -8, NOTE_CS4, 16, NOTE_B3, 2, NOTE_E3, 4, NOTE_GS3, 4, NOTE_B3, 4, NOTE_GS3, 4, 
NOTE_E3, 4, NOTE_B3, 4, NOTE_B3, 2, NOTE_FS4, -8, NOTE_GS4, 16, NOTE_FS4, -8, NOTE_GS4, 16, NOTE_FS4, 2, 
NOTE_E3, 4, NOTE_B3, 4, NOTE_B3, 2, NOTE_G4, -8, NOTE_FS4, 16, NOTE_G4, -8, NOTE_FS4, 16, NOTE_G4, 2, 
NOTE_B3, -8, NOTE_AS3, 16, NOTE_B3, -8, NOTE_CS4, 16, NOTE_B3, 2, 
NOTE_B3, -8, NOTE_AS3, 16, NOTE_B3, -8, NOTE_CS4, 16, NOTE_B3, 4, NOTE_DS4, 4,

// Police Theme (The Mob)
NOTE_E3, 8, NOTE_E3, 8, NOTE_G3, 8, NOTE_E3, 8, NOTE_A3, 8, NOTE_G3, 8, NOTE_E3, 4, NOTE_E4, 8,
};

int songPacman[] = {

  // Pacman
  // Score available at https://musescore.com/user/85429/scores/107109
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};

int songNokia[] = {

  // Nokia Ringtone 
  // Score available at https://musescore.com/user/29944637/scores/5266155
  
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2, 
};

const int LED_NUM = 10;
Adafruit_NeoPixel strip(LED_NUM, D3, NEO_GRB + NEO_KHZ800);
int rgbMode = 0;
uint16 red, green, blue;
int animationProgress;
int animationSpeeed = 5; // 1 to 10 speeed
unsigned long previousLEDMillis;

unsigned long lastDriveMessage;

int tempo = 120;
float tempoMultiplier = 1;
int songIndex = 8;
bool repeatSong = true;
unsigned long targetNoteMillis;
int currentNoteLength;
int noteIndex = 0;
int buzzerPin = D2;

IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0); 
const char* ssid     = "Super Cool Rover";
const char* password = "123456789";
//FSInfo fs_info;
//DNSServer dnsServer;
AsyncWebServer server(80);
//char bufferTemp[300];

//RH_ASK rfDriver(2000, 11, 12, 10, false);
//RHReliableDatagram rfManager(rfDriver, 1);
//uint8_t rfBuffer[RH_ASK_MAX_MESSAGE_LEN];

struct SONG {
  int *notes;
  uint numNotes;
};

const SONG songs[] = {
  {songPachabel, NUMELEMENTS(songPachabel)},
  {songMii, NUMELEMENTS(songMii)},
  {songTetris, NUMELEMENTS(songTetris)},
  {songKeyboardCat, NUMELEMENTS(songKeyboardCat)},
  {songTakeOnMe, NUMELEMENTS(songTakeOnMe)},
  {songPanther, NUMELEMENTS(songPanther)},
  {songMario, NUMELEMENTS(songMario)},
  {songDragonBallSuper, NUMELEMENTS(songDragonBallSuper)},
  {songSpongebobMedley, NUMELEMENTS(songSpongebobMedley)},
  {songPacman, NUMELEMENTS(songPacman)},
  {songNokia, NUMELEMENTS(songNokia)}
};

/*void ShowInfo(void){
 
  SPIFFS.info(fs_info);
  sprintf(bufferTemp,  "Total Bytes:     %d", fs_info.totalBytes);
  Serial.println(bufferTemp);
  sprintf(bufferTemp,  "Used Bytes:      %d", fs_info.usedBytes);
  Serial.println(bufferTemp); 
  sprintf(bufferTemp,  "Block Size:      %d", fs_info.blockSize);
  Serial.println(bufferTemp);   
  sprintf(bufferTemp,  "Page Size:       %d", fs_info.pageSize);
  Serial.println(bufferTemp); 
  sprintf(bufferTemp,  "Max open files:  %d", fs_info.maxOpenFiles);
  Serial.println(bufferTemp); 
  sprintf(bufferTemp,  "Max path lenght: %d", fs_info.maxPathLength);
  Serial.println(bufferTemp);
  Serial.println("");
}

void ShowDIR(void){

    Serial.println("\n--------------------------------");
    Serial.println("Directory:");
    Dir dir = SPIFFS.openDir("");
 
  while (dir.next()) {
    Serial.print(dir.fileName());
    File f = dir.openFile("r");
    Serial.print("\t\t");
    Serial.println(f.size());
  }
  Serial.println("--------------------------------");
  Serial.println("");
}*/

void setAllLEDs(uint8 r, uint8 g, uint8 b) {
  for (int i = 0; i < LED_NUM; i++)
    strip.setPixelColor(i, g, r, b);
  strip.show();
}

void setup(){
  pinMode(D4, OUTPUT); 
  pinMode(D6, OUTPUT); 
  pinMode(D7, OUTPUT); 
  pinMode(D8, OUTPUT); 
  // Serial port for debugging purposes
  Serial.begin(115200);
  strip.begin();
  setAllLEDs(0, 0, 0);
  strip.show();

  Serial.print("Setting AP (Access Point)…");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(ssid);
  //WiFi.softAP(ssid, password);

  //dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  //dnsServer.start(53, "*", apIP);
  
  delay(500);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
  SPIFFS.begin(); 
  //ShowInfo();
  //ShowDIR();
  // Route for root / web page
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/get-state", HTTP_GET, [](AsyncWebServerRequest *request){
    String s = String();
    s += rgbMode;
    s += " ";
    s += animationSpeeed;
    s += " ";
    s += red;
    s += " ";
    s += green;
    s += " ";
    s += blue;
    s += " ";
    s += tempo;
    s += " ";
    s += repeatSong;
    s += " ";
    s += songIndex;  
    request->send_P(200, "text/plain", String(s).c_str());
  });

  server.on("/rgb", HTTP_POST, [](AsyncWebServerRequest *request){
    rgbMode = request->getParam(0)->value().toInt();
    animationSpeeed = request->getParam(1)->value().toInt();
    red = request->getParam(2)->value().toInt();
    green = request->getParam(3)->value().toInt();
    blue = request->getParam(4)->value().toInt();
    setAllLEDs(red, green, blue);
    animationProgress = 0;
    request->send_P(200, "text/plain", String("Recieved").c_str());
  });

  server.on("/music", HTTP_POST, [](AsyncWebServerRequest *request){
    tempo = request->getParam(0)->value().toInt();
    repeatSong = request->getParam(1)->value().toInt() == 1;
    songIndex = request->getParam(2)->value().toInt();
    noteIndex = 0;
    tempoMultiplier = 1;
    noTone(buzzerPin);
    pinMode(D2, INPUT);
    request->send_P(200, "text/plain", String("Recieved").c_str());
  });

  server.on("/data", HTTP_POST, [](AsyncWebServerRequest *request){
    lastDriveMessage = millis();
    if (request->getParam(1)->value().toInt() == 0) {
        analogWrite(D7, request->getParam(0)->value().toFloat() / 100.0 * 1023.0);
        analogWrite(D8, 0);
      } else {
        analogWrite(D8, request->getParam(0)->value().toFloat() / 100.0 * 1023.0);
        analogWrite(D7, 0);
      }
      if (request->getParam(3)->value().toInt() == 0) {
        analogWrite(D4, request->getParam(2)->value().toFloat() / 100.0 * 1023.0);
        analogWrite(D6, 0);
      } else {
        analogWrite(D6, request->getParam(2)->value().toFloat() / 100.0 * 1023.0);
        analogWrite(D4, 0);
      }
    request->send_P(200, "text/plain", String("Recieved").c_str());
  });

  /*server.on("/generate_204", HTTP_POST, [](AsyncWebServerRequest *request){
    request->redirect("http://192.168.4.1");
  });
  server.on("/fwlink", HTTP_POST, [](AsyncWebServerRequest *request){
    request->redirect("http://192.168.4.1");
  });*/
  /*server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });*/

 server.onNotFound([](AsyncWebServerRequest *request){
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->redirect("http://192.168.4.1");
    //request->send(404);
  }
});

  // Start server
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();

  /*if (MDNS.begin("rover")) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("MDNS responder started");
    Serial.print("You can now connect to http://");
    Serial.print("rover");
    Serial.println(".local");
  } else {
    Serial.println("MDNS responder failed to start");
  }

  targetNoteMillis = millis();

  if (!rfManager.init())
         Serial.println("Failed to initialize rf manager");
         */
}

int getDuration(int index) {
  int noteDuration = 0;
  int divider = songs[songIndex].notes[index];
  int wholenote = (60000 * 4) / tempo;
  if (divider > 0) {
    // regular note, just proceed
    noteDuration = (wholenote) / divider;
  } else if (divider < 0) {
    // dotted notes are represented with negative durations!!
    noteDuration = (wholenote) / abs(divider);
    noteDuration *= 1.5; // increases the duration in half for dotted notes
  }
  //Serial.println(index);
  //Serial.println(noteDuration);
  return noteDuration;
}

void loop(){
  // RF communication
  /*if (rfManager.available()) {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(rfBuffer);
    uint8_t from;
    if (rfManager.recvfrom(rfBuffer, &len, &from)) {
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)rfBuffer);
      // Send a reply back to the originator client
      //if (!rfManager.sendtoWait(data, sizeof(data), from))
      //  Serial.println("sendtoWait failed");
    }
  }*/
  unsigned long currentMillis = millis();

  // Watchdog
  if (lastDriveMessage + 1500 < currentMillis) {
    analogWrite(D4, 0);
    analogWrite(D6, 0);
    analogWrite(D7, 0);
    analogWrite(D8, 0);
  }
  
  // LEDs
  if (currentMillis > previousLEDMillis + 10 - animationSpeeed) {
    if (rgbMode == 1) { // Rainbow
      animationProgress += 24;
      if (animationProgress >= 3 * 65536)
        animationProgress = 0;
      for (int i = 0; i < LED_NUM; i++) {
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(std::min(animationProgress + i * 2048, 3 * 65536))));
      }
      strip.show();
    }
    previousLEDMillis = currentMillis;
  }

  // Music Todo: 
  if (songIndex != -1) {
    if (noteIndex == 0 || targetNoteMillis < currentMillis ) {
      if (songs[songIndex].notes[noteIndex * 2] == TEMPO) {
        tempoMultiplier = getDuration(noteIndex * 2 + 1) / 100.0;
        noteIndex++;
      }
      int duration = getDuration(noteIndex * 2 + 1);
      tone(buzzerPin, songs[songIndex].notes[noteIndex * 2], duration * 0.9);
      noteIndex++;
      // Todo: prevent from cutting off last note
      if (noteIndex > songs[songIndex].numNotes / 2) {
        tempoMultiplier = 1;
        noteIndex = 0;
        if (!repeatSong) {
          songIndex = -1;
          pinMode(D2, INPUT);
        }
      }
      targetNoteMillis = currentMillis + duration;
    }
  }  

  //dnsServer.processNextRequest();
}