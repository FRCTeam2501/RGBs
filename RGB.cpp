#include <iostream>
#include <unistd.h>
#include<link.h>
#include <math.h>
using namespace std;

#include "RGB.h"

RGB::RGB()
{
	RGB::ledCount = ledCount;

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
RGB rgb;

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
		rgb.Set(i,rgb.LEDMap[i].color);
	}
	Render();
}

void RGB::MoveArray(bool wrapAround, bool forward, int moveNumber){
	//cout<<"MoveArray: a";
	for(int a = 0; a<moveNumber;a++){
		//cout<<"b";
		if(forward){
			//cout<<"c";
			rgb.LEDStore = rgb.LEDMap[COLORS::numberOfLeds - 1];
			for(int b = COLORS::numberOfLeds - 1;b>0;b--){
				rgb.LEDMap[b] = rgb.LEDMap[b-1];
				//cout<<b;
		}
		if(wrapAround){
			rgb.LEDMap[0] = rgb.LEDStore;
		}
		}else{
			//cout<<"d";
			rgb.LEDStore = rgb.LEDMap[0];
			for(int b = 0;b<COLORS::numberOfLeds;b++){
				rgb.LEDMap[b] = rgb.LEDMap[b+1];
		}
		if(wrapAround){
			rgb.LEDMap[COLORS::numberOfLeds - 1] = rgb.LEDStore;
		}
	}
}
//cout<<endl;
}

void RGB::CalcVelAccel(){
	for(int t = 0; t < COLORS::numberOfLeds; t++){
		LEDMap[t].redSet = LEDMap[t].red;
		LEDMap[t].greenSet = LEDMap[t].green;
		LEDMap[t].blueSet = LEDMap[t].blue;

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
				cout<<"red min reached"<<endl;
            }
            }else{
				if(t == 9){
				cout<<"red accounted for"<<endl;}
                 LEDMap[t].redSet += LEDMap[t].rvel;
            }
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
		if((abs(LEDMap[t].red - LEDMap[t].redTarget)<2)){
			cout<<"red has reached target"<<endl;
		}
		if((abs(LEDMap[t].green - LEDMap[t].greenTarget)<2)){
			cout<<"Green has reached target"<<endl;
		}
		if((abs(LEDMap[t].blue - LEDMap[t].blueTarget)<2)){
			cout<<"Blue has reached target"<<endl;
		}
				if(((abs(LEDMap[t].red - LEDMap[t].redTarget)<2) && (abs(LEDMap[t].green - LEDMap[t].greenTarget)<2) && (abs(LEDMap[t].blue - LEDMap[t].blueTarget)<2)) || LEDMap[t].color == LEDMap[t].colorTarget){
					 
					LEDMap[t].rvel= 0;
					LEDMap[t].gvel= 0;
					LEDMap[t].bvel= 0;
					LEDMap[t].raccel= 0;
					LEDMap[t].gaccel= 0;
					LEDMap[t].baccel= 0;
					//cout<<t<<": stoped blending"<<endl;
				}
			//}
        }
}

void RGB::GradualColorChangeSet(int rgbNumber, uint32_t targetColor, int tickInterval){
	if(LEDMap[rgbNumber].rvel > 0){
		cout<<"rvel is more than 0"<<endl;
	}
	LEDMap[rgbNumber].colorTarget = targetColor;
	LEDMap[rgbNumber].bvel = ((LEDMap[rgbNumber].blueTarget - LEDMap[rgbNumber].blue) / (uint8_t) tickInterval);
	LEDMap[rgbNumber].gvel = ((LEDMap[rgbNumber].greenTarget - LEDMap[rgbNumber].green) / (uint8_t) tickInterval);
	LEDMap[rgbNumber].rvel = ((LEDMap[rgbNumber].redTarget - LEDMap[rgbNumber].red) / (uint8_t) tickInterval);
	if(LEDMap[rgbNumber].rvel > 0){
		cout<<"9 rvel is greater than 0"<<endl;
	}
}

int main(){
/*	
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
*/

	//rgb.LEDMap[8].color = 0xFF8800;
	//rgb.LEDMap[7].red = 255;
	rgb.LEDMap[8].red = LEDS::minPosition;
	rgb.LEDMap[9].red = 255;
	//rgb.LEDMap[9].rvel =10;
	//rgb.LEDMap[6].bvel = 10;
	//rgb.LEDMap[6].rvel = -10;
	//rgb.LEDMap[6].red = 255;
	rgb.GradualColorChangeSet(9,0x000000,20);
	rgb.RenderArray();
	for(int g = 0;g<80;g++){
	usleep(300000);
	//rgb.MoveArray(true,true,1);
	if(rgb.LEDMap[9].rvel < 0){
		cout<<"it is less"<<endl;
	}
	rgb.CalcVelAccel();
	rgb.RenderArray();
	//cout<<rgb.LEDMap[9].blue<<endl;
	
	}

	return 0;
}
