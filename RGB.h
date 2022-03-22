#pragma once

#include "ws2811/clk.h"
#include "ws2811/gpio.h"
#include "ws2811/dma.h"
#include "ws2811/pwm.h"
#include "ws2811/ws2811.h"


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
	constexpr int maxVelocity = 15;
	constexpr int maxPosition = 255;
}

bool ending = false;
bool designChanged = false;
int switcher = 1;
int switchTracker = 1;

class RGB
{

private:
    
    int32_t ledCount = COLORS::numberOfLeds;

	

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
		uint8_t rvel;
		uint8_t gvel;
		uint8_t bvel;
		uint8_t raccel;
		uint8_t gaccel;
		uint8_t baccel;
	};
	CRGB LEDStore;
	CRGB LEDMap[COLORS::numberOfLeds];
};
