#pragma once
#ifndef RGBCODE_H
#define RGBCODE_H

#endif // RGBCODE_H

#include <ws2811/clk.h>
#include <ws2811/gpio.h>
#include <ws2811/dma.h>
#include <ws2811/pwm.h>
#include <ws2811/ws2811.h>

#include <iostream>
#include <unistd.h>
#include<link.h>
#include <math.h>

using namespace std;

namespace COLORS {
    /**
     * @brief FIRST Colors
     */
    constexpr ws2811_led_t	DENIM		= 0x001560BD;
    constexpr ws2811_led_t	FIRST_BLUE	= 0x000066B3;
    constexpr ws2811_led_t	FIRST_RED	= 0x00ED1C24;


    /**
     * @brief Standard Colors
     */
    //these colors are stored in base 16
    constexpr ws2811_led_t	RED			= 0x00FF0000;
    constexpr ws2811_led_t	ORANGE		= 0x00FF8000;
    constexpr ws2811_led_t	YELLOW		= 0x00FFFF00;
    constexpr ws2811_led_t	GREEN		= 0x0000FF00;
    constexpr ws2811_led_t	LIGHT_BLUE	= 0x0000FF40;
    constexpr ws2811_led_t	BLUE		= 0x000000FF;
    constexpr ws2811_led_t	PURPLE		= 0x00800080;
    constexpr ws2811_led_t	PINK		= 0x00FF0080;
    constexpr ws2811_led_t	WHITE		= 0x00FFFFFF;
    constexpr ws2811_led_t	BLACK		= 0x00000000;

    constexpr int numberOfLeds = 10;
}

namespace LEDS{
    constexpr int maxVelocity = 30;
    constexpr int minVelocity = -30;
    constexpr int maxPosition = 255;
    constexpr int minPosition = 0;
}
/*

*/

class RGB
{


private:

    int32_t ledCount = COLORS::numberOfLeds;
    int test = 0;

public:
    RGB();
    ~RGB();
    void SetAll(ws2811_led_t color);
    ws2811_t ledString;

    /**
     * @brief Set the color of an individual LED
     *
     * @param index index of the LED to set
     * @param color ws2811_led_t to set the color to
     */
    void Set(uint8_t index, ws2811_led_t color);

    /**
     * @brief Set the Brightness
     *
     * @param brightness 0 to 255
     */
    void SetBrightness(uint8_t brightness);

    /**
     * @brief Render the changes
     */
    void Render();

    void UniversalClock(int msWait);

    void MoveArray(bool wrapAround, bool forward, int moveNumber); //should it move the leds to the front or back if they are knocked off or delete them, number of moves to make

    void RenderArray();

    void CalcVelAccel();

    void GradualColorChangeSet(int rgbNumber, uint32_t targetColor, int tickInterval);

    void BarMoveInit(int barOneLength, uint32_t colorOne, int barTwoLength, uint32_t colorTwo);

    void TickTracker();

    void MoveWaveSeedArray(bool MoveForward, int moveDistance);

    void WaveTargetToRGBArray();

    void SeedListToSeedArray(int listNumber);

    bool DoTicking = false;
    bool designChanged = true;
    int switcher = 1;
    int switchTracker = 1;
    int eventTickTime = 5;
    int tickingTracker = 0;
    bool doEvent = false;
    bool gradualPremited;


    bool barMoveForward;
    int barMoveJumbDist;
    int barOneLength;
    int barTwoLength;

    struct CRGB
    {
        union{
            struct{
                uint8_t blue;
                uint8_t green;
                uint8_t red;
            };
            uint32_t color;
        };
        union{
            struct{
                uint8_t blueTarget;
                uint8_t greenTarget;
                uint8_t redTarget;
            };
            uint32_t colorTarget;
        };
        double rvel;
        double gvel;
        double bvel;
        double raccel;
        double gaccel;
        double baccel;

        double blueSet;
        double greenSet;
        double redSet;

        bool doingGradual;
    };
    CRGB LEDStore;
    CRGB LEDMap[COLORS::numberOfLeds];


    struct RGBWaveSeed{
        union{
            struct{
                uint8_t targetBlue;
                uint8_t targetGreen;
                 uint8_t targetRed;
            };
            uint32_t colorTarget;
        };
        int tickInterval;
    };

    RGBWaveSeed waveSeedHold[COLORS::numberOfLeds];//tracks what seeds are at what position

    struct RGBWaveList{
        RGBWaveSeed waveSeedInfo;
        int ticksBeforeNextWave;
        int StartingPlace;


    };
  //  int ticksBetweenWaveMove;
    bool waveMoveForward;//wave seeds direction
    int waveMoveDistance;//number of moves made by wave seeds every move event
    int waveADDTickTracker = 0; //tracks of ticks since last wave seed was added
    int waveNumberToBeAdded = 0; //tracks seed number in list to be added next
   RGBWaveList waveSeedList[300];//tracks what designs are to come
};
/*

RGB::RGB()
{
    RGB::ledCount = COLORS::numberOfLeds;

    ledString.freq = WS2811_TARGET_FREQ;
    ledString.dmanum = 10;
    ledString.channel[0].gpionum = 21;
    ledString.channel[0].count = ledCount;
    ledString.channel[0].invert = 0;
    ledString.channel[0].brightness = 255;
    ledString.channel[0].strip_type = WS2811_STRIP_GRB;
    ledString.channel[1].gpionum = 0;
    ledString.channel[1].count = 0;
    ledString.channel[1].invert = 0;
    ledString.channel[1].brightness = 0;

   //ws2811_init(&ledString);
    ws2811_return_t ret = ws2811_init(&ledString);

    if (ret != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        // cout<<"testing"<<endl;
        exit(ret);
    }

}

RGB::~RGB()
{
    for (int x = 0; x < ledCount; x++) {
        ledString.channel[0].leds[x] = COLORS::BLACK;
    }

    // Render the change
    ws2811_render(&ledString);

    // Call lower library function to cleanup DMA
    ws2811_fini(&ledString);
}

void RGB::SetAll(ws2811_led_t color) {
    for (uint8_t x = 0; x < ledCount; x++) {
        ledString.channel[0].leds[x] = color;
    }
    cout<<"Set all"<<endl;
}

void RGB::Set(uint8_t index, ws2811_led_t color) {
    ledString.channel[0].leds[index] = color;
}

void RGB::SetBrightness(uint8_t brightness) {
    ledString.channel[0].brightness = brightness;
}

void RGB::Render() {
    ws2811_return_t ret = ws2811_render(&ledString);
    if (ret != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        exit(ret);
        //cout<<"render error"<<endl;
    }
}

void RGB::UniversalClock(int msWait){
    usleep(msWait*1000);
}

void RGB::RenderArray(){
    for(int i = 0; i<COLORS::numberOfLeds; i++){
        Set(i,LEDMap[i].color);
    }
    Render();
}

void RGB::MoveArray(bool wrapAround, bool forward, int moveNumber){
    //cout<<"MoveArray: a";
    for(int a = 0; a<moveNumber;a++){
        //cout<<"b";
        if(forward){
            //cout<<"c";
            LEDStore = LEDMap[COLORS::numberOfLeds - 1];
            for(int b = COLORS::numberOfLeds - 1;b>0;b--){
                LEDMap[b] = LEDMap[b-1];
                //cout<<b;
        }
        if(wrapAround){
            LEDMap[0] = LEDStore;
        }
        }else{
            //cout<<"d";
            LEDStore = LEDMap[0];
            for(int b = 0;b<COLORS::numberOfLeds;b++){
                LEDMap[b] = LEDMap[b+1];
        }
        if(wrapAround){
            LEDMap[COLORS::numberOfLeds - 1] = LEDStore;
        }
    }
}
//cout<<endl;
}

void RGB::CalcVelAccel(){
    for(int t = 0; t < COLORS::numberOfLeds; t++){

        if(LEDMap[t].doingGradual == false){
        LEDMap[t].redSet = LEDMap[t].red;
        LEDMap[t].greenSet = LEDMap[t].green;
        LEDMap[t].blueSet = LEDMap[t].blue;
        }



        if((abs(LEDMap[t].red - LEDMap[t].redTarget) == 0)){
            //cout<<"red has reached target"<<endl;
            LEDMap[t].rvel= 0;
        }
        if((abs(LEDMap[t].green - LEDMap[t].greenTarget)== 0)){
            //cout<<"Green has reached target"<<endl;
            LEDMap[t].gvel= 0;
        }
        if((abs(LEDMap[t].blue - LEDMap[t].blueTarget) == 0)){
            //cout<<"Blue has reached target"<<endl;
            LEDMap[t].bvel= 0;
        }



        if(LEDMap[t].rvel + LEDMap[t].raccel > LEDS::maxVelocity || LEDMap[t].rvel + LEDMap[t].raccel < LEDS::minVelocity){
            if(LEDMap[t].rvel + LEDMap[t].raccel > LEDS::maxVelocity){
                LEDMap[t].rvel = LEDS::maxVelocity;
            }else{
                LEDMap[t].rvel = LEDS::minVelocity;
            }
            }else{
                 LEDMap[t].rvel += LEDMap[t].raccel;
            }
         if(LEDMap[t].gvel + LEDMap[t].gaccel > LEDS::maxVelocity || LEDMap[t].gvel + LEDMap[t].gaccel < LEDS::minVelocity){
            if(LEDMap[t].gvel + LEDMap[t].raccel > LEDS::maxVelocity){
                LEDMap[t].gvel = LEDS::maxVelocity;
            }else{
                LEDMap[t].gvel = LEDS::minVelocity;
            }
            }else{
                 LEDMap[t].gvel += LEDMap[t].gaccel;
            }
          if(LEDMap[t].bvel + LEDMap[t].baccel > LEDS::maxVelocity || LEDMap[t].bvel + LEDMap[t].baccel < LEDS::minVelocity){
            if(LEDMap[t].bvel + LEDMap[t].baccel > LEDS::maxVelocity){
                LEDMap[t].bvel = LEDS::maxVelocity;
            }else{
                LEDMap[t].bvel = LEDS::minVelocity;
            }
            }else{
                 LEDMap[t].bvel += LEDMap[t].baccel;
            }


         if(LEDMap[t].red + LEDMap[t].rvel > LEDS::maxPosition || LEDMap[t].red + LEDMap[t].rvel < LEDS::minPosition){
            if(LEDMap[t].red + LEDMap[t].rvel > LEDS::maxPosition){
                LEDMap[t].redSet = LEDS::maxPosition;
            }else{
                LEDMap[t].redSet = LEDS::minPosition;
                //cout<<"red min reached "<<LEDMap[t].rvel<<" "<<((int) LEDMap[t].red)<<endl;

            }
            }else{
                //cout<<"rset and rvel: "<<LEDMap[t].redSet<<" "<<LEDMap[t].rvel<<endl;
                 LEDMap[t].redSet += LEDMap[t].rvel;
                 if(t == 9 && LEDMap[t].rvel != 0){
                    test ++;
                //cout<<test<<": tracker 9r"<<endl;
                cout<<LEDMap[t].redSet<<endl;
                }
            }
            //cout<<test<<": "
         if(LEDMap[t].green + LEDMap[t].gvel > LEDS::maxPosition || LEDMap[t].green + LEDMap[t].gvel < LEDS::minPosition){
            if(LEDMap[t].green + LEDMap[t].gvel > LEDS::maxPosition){
                LEDMap[t].greenSet = LEDS::maxPosition;
            }else{
                LEDMap[t].greenSet = LEDS::minPosition;
            }
            }else{
                 LEDMap[t].greenSet += LEDMap[t].gvel;
            }
          if(LEDMap[t].blue + LEDMap[t].bvel > LEDS::maxPosition || LEDMap[t].blue + LEDMap[t].bvel < LEDS::minPosition){
            if(LEDMap[t].blue + LEDMap[t].bvel > LEDS::maxPosition){
                LEDMap[t].blueSet = LEDS::maxPosition;
            }else{
                LEDMap[t].blueSet = LEDS::minPosition;
            }
            }else{
                 LEDMap[t].blueSet += LEDMap[t].bvel;
            }
            LEDMap[t].red = ((uint8_t) lround(LEDMap[t].redSet));
            //LEDMap[t].red = LEDMap[t].redSet;
            LEDMap[t].green = ((uint8_t) lround(LEDMap[t].greenSet));
            //LEDMap[t].green = LEDMap[t].greenSet;
            LEDMap[t].blue = ((uint8_t) lround(LEDMap[t].blueSet));
            //LEDMap[t].blue = LEDMap[t].blueSet;

            //if(LEDMap[t].colorTarget > 0){
                //cout<<"blend stop attempt"<<endl;

                if(((abs(LEDMap[t].red - LEDMap[t].redTarget)<2) && (abs(LEDMap[t].green - LEDMap[t].greenTarget)<2) && (abs(LEDMap[t].blue - LEDMap[t].blueTarget)<2)) || LEDMap[t].color == LEDMap[t].colorTarget){

                    LEDMap[t].rvel= 0;
                    LEDMap[t].gvel= 0;
                    LEDMap[t].bvel= 0;
                    LEDMap[t].raccel= 0;
                    LEDMap[t].gaccel= 0;
                    LEDMap[t].baccel= 0;
                    if(t == 9){

                //	cout<<t<<": stoped blending"<<endl;
                    }
                    LEDMap[t].doingGradual = false;
                }
            //}
        }
}

void RGB::GradualColorChangeSet(int rgbNumber, uint32_t targetColor, int tickInterval){
    if(LEDMap[rgbNumber].rvel > 0){
        //cout<<"rvel is more than 0"<<endl;
    }
    LEDMap[rgbNumber].colorTarget = targetColor;
    LEDMap[rgbNumber].bvel = ((LEDMap[rgbNumber].blueTarget - LEDMap[rgbNumber].blue) / (uint8_t) tickInterval);
    LEDMap[rgbNumber].gvel = ((LEDMap[rgbNumber].greenTarget - LEDMap[rgbNumber].green) / (uint8_t) tickInterval);
    LEDMap[rgbNumber].rvel = (((double) LEDMap[rgbNumber].redTarget - (double) LEDMap[rgbNumber].red) / (double) tickInterval);
    if(LEDMap[rgbNumber].rvel == -12.7){
    //	cout<<"9 rvel is -12.7"<<endl;
    }
        LEDMap[rgbNumber].redSet = LEDMap[rgbNumber].red;
        LEDMap[rgbNumber].greenSet = LEDMap[rgbNumber].green;
        LEDMap[rgbNumber].blueSet = LEDMap[rgbNumber].blue;
        LEDMap[rgbNumber].doingGradual = true;
}




int main(){

while(ending == false){
    switchTracker = switcher;
    switch (switcher)
    {
    case 1:

        break;
    default:
        break;
}
if(switchTracker != switcher){
    designChanged = true;
}else{
    designChanged = false;
}
}


    rgb.LEDMap[8].color = 0xFF0000;
    //rgb.LEDMap[7].red = 255;
    rgb.LEDMap[9].red = 255;
    //rgb.LEDMap[9].rvel =10;
    //rgb.LEDMap[6].bvel = 10;
    //rgb.LEDMap[6].rvel = -10;
    //rgb.LEDMap[6].red = 255;
    rgb.GradualColorChangeSet(9,0x010000,20);
    rgb.RenderArray();
    for(int g = 0;g<80;g++){
    usleep(300000);
    rgb.MoveArray(true,true,1);
    rgb.CalcVelAccel();
    rgb.RenderArray();


    }

    return 0;
}
    */


