#include <iostream>
#include <unistd.h>
#include<link.h>
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


	rgb.LEDMap[7].red = 255;
	rgb.LEDMap[8].blue = 255;
	rgb.LEDMap[9].green = 255;

	rgb.RenderArray();
	for(int g = 0;g<30;g++){
	usleep(200000);
	rgb.MoveArray(true,true,1);
	rgb.RenderArray();
	}


	return 0;
}
