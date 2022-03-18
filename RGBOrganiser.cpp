#include <iostream>
#include "RGBOrganiser.h"


RGBOrganiser::RGBOrganiser()
{
    rgb = new RGB();
}

RGBOrganiser::~RGBOrganiser()
{
    delete rgb;
}


int main(){

    rgb->SetAll(COLORS::BLUE);



}
