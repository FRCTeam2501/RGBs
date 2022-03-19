#include <iostream>
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
		cout<<"testing"<<endl;
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

RGB rgb;

int main(){
	cout<<"a";
	rgb.SetAll(COLORS::BLUE);
	cout<<"b"<<endl;
	return 0;
}
