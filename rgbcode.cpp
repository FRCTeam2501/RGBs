
#include "rgbcode.h"
#include "mainwindow.h"
#include <iostream>
#include <unistd.h>
#include<link.h>
#include <math.h>
#include <QDebug>

using namespace std;

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
    cout<<"LEDS INIT"<<endl;

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
        LEDMap[rgbNumber].bvel = (((double) LEDMap[rgbNumber].blueTarget - (double) LEDMap[rgbNumber].blue) / (double) tickInterval);
        LEDMap[rgbNumber].gvel = (((double) LEDMap[rgbNumber].greenTarget - (double) LEDMap[rgbNumber].green) / (double) tickInterval);
        LEDMap[rgbNumber].rvel = (((double) LEDMap[rgbNumber].redTarget - (double) LEDMap[rgbNumber].red) / (double) tickInterval);
        if(LEDMap[rgbNumber].rvel == -12.7){
        //	cout<<"9 rvel is -12.7"<<endl;
        }
            LEDMap[rgbNumber].redSet = LEDMap[rgbNumber].red;
            LEDMap[rgbNumber].greenSet = LEDMap[rgbNumber].green;
            LEDMap[rgbNumber].blueSet = LEDMap[rgbNumber].blue;
            LEDMap[rgbNumber].doingGradual = true;
}

void RGB::BarMoveInit(int barOneLength, uint32_t colorOne, int barTwoLength, uint32_t colorTwo){
    for(int i = 0; i<COLORS::numberOfLeds;){
        for(int a = 0; a<barOneLength; a++){
            LEDMap[i].color = colorOne;
            i++;
        }
        for(int c = 0;c<barTwoLength;c++){
            LEDMap[i].color = colorTwo;
            i++;
        }
    }
}

void RGB::MoveWaveSeedArray(bool MoveForward, int moveDistance){
    for (int a = 0; a<moveDistance;a++){
    if(MoveForward){
        for(int t = 0; t<COLORS::numberOfLeds - 1; t++){
            waveSeedHold[COLORS::numberOfLeds - 1 - t] = waveSeedHold[COLORS::numberOfLeds - 2 - t];

        }
        waveSeedHold[0].targetRed = NULL;
        waveSeedHold[0].targetGreen = NULL;
        waveSeedHold[0].targetBlue = NULL;
        waveSeedHold[0].tickInterval = NULL;
    }else{
        for(int t = 0; t<COLORS::numberOfLeds - 1; t++){
            waveSeedHold[t] = waveSeedHold[t + 1];
        }
        waveSeedHold[COLORS::numberOfLeds - 1].targetRed = NULL;
        waveSeedHold[COLORS::numberOfLeds - 1].targetGreen = NULL;
        waveSeedHold[COLORS::numberOfLeds - 1].targetBlue = NULL;
        waveSeedHold[COLORS::numberOfLeds - 1].tickInterval = NULL;
    }
    }
}

void RGB::WaveTargetToRGBArray(){
  //  qDebug()<<"WaveToRGBS a";
  //  usleep(400000);
  //  qDebug()<<"waveTesting!!!!!!!!!!!";
    for(int w = 0; w< COLORS::numberOfLeds;w++){
       // LEDMap[w].
     //   qDebug()<<"WaveToRGBS loop";
      //  usleep(400000);
        if(waveSeedHold[w].tickInterval > 0){
          //  qDebug()<<"Trueeeeeeeeeeee";
        //    usleep(400000);
        GradualColorChangeSet(w,waveSeedHold[w].colorTarget,waveSeedHold[w].tickInterval);
        //LEDMap[w].colorTarget = waveSeedHold[w].colorTarget;
    }else{
        //    qDebug()<<"FALSE";
      //  qDebug()<<"WaveToRGBS loop end";
       // usleep(400000);
    }
}
}


void RGB::SeedListToSeedArray(int listNumber){
//qDebug()<<"startinggggggggggggg"<<waveSeedList[listNumber].StartingPlace;
   waveSeedHold[waveSeedList[listNumber].StartingPlace-1].targetRed = waveSeedList[listNumber].waveSeedInfo.targetRed;
   waveSeedHold[waveSeedList[listNumber].StartingPlace-1].targetGreen = waveSeedList[listNumber].waveSeedInfo.targetGreen;
   waveSeedHold[waveSeedList[listNumber].StartingPlace-1].targetBlue = waveSeedList[listNumber].waveSeedInfo.targetBlue;
   waveSeedHold[waveSeedList[listNumber].StartingPlace-1].tickInterval = waveSeedList[listNumber].waveSeedInfo.tickInterval;
   GradualColorChangeSet(waveSeedList[listNumber].StartingPlace-1,waveSeedHold[waveSeedList[listNumber].StartingPlace-1].colorTarget,waveSeedHold[waveSeedList[listNumber].StartingPlace-1].tickInterval);//This line is disgusting but I am roling with it
}

void RGB::TickTracker(){
   // qDebug()<<"TickTracker a";
    //usleep(400000);
    if(DoTicking){
        switchTracker = switcher;
        switch (switcher)
        {
        case 1:
         //   cout<<"b";
           if(doEvent){
               MoveArray(true, barMoveForward, barMoveJumbDist);
          //     cout<<"c";
           }
            break;
        case 2:
         //    qDebug()<<"TickTracker b";
            if(waveSeedList[waveNumberToBeAdded].ticksBeforeNextWave <= waveADDTickTracker){
            //     qDebug()<<"TickTracker cccccccccccccccccccccccccccc";
             //    usleep(400000);
                SeedListToSeedArray(waveNumberToBeAdded);
                waveNumberToBeAdded++;
             //    qDebug()<<"TickTracker d";
                waveADDTickTracker = 0;
                if(waveSeedList[waveNumberToBeAdded].waveSeedInfo.tickInterval == 0){
                    waveNumberToBeAdded = 0;
                //     qDebug()<<"TickTracker e";
                }
            }else{
                waveADDTickTracker++;
             //    qDebug()<<"TickTracker f";
            }
            if(doEvent){
            //     qDebug()<<"TickTracker g";
              //   usleep(400000);
                MoveWaveSeedArray(waveMoveForward, waveMoveDistance);
             //    qDebug()<<"TickTracker h";
             //    usleep(400000);
                WaveTargetToRGBArray();
            //     qDebug()<<"TickTracker i";
              //   usleep(400000);

            }

            if(gradualPremited == true){
            CalcVelAccel();
           }
           //  qDebug()<<"TickTracker l";
            for(int u = 0; u<COLORS::numberOfLeds;u++){
                if(waveSeedHold[u].targetRed>0){
               //     qDebug()<<"Greater";
                }else{
                 //             qDebug()<<"L";
                }
            }
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
    if(tickingTracker >= eventTickTime){
        tickingTracker = 0;
        doEvent = true;
    }else{
        tickingTracker++;
        doEvent = false;
    }
   // cout<<endl;
    RenderArray();
}

/*
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
