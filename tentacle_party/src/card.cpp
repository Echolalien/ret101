#include "card.h"

card::card(){}

void card::setup(float xp, float yp, float d, float spd){
    x=xp;
    y=yp;
    toBlack=255;
    difficulty=d;
    speed=spd;
}

//-------------------------

void card::update(){

    if(touch==true && lock==false){
        if(reveal==false){
            if(progress<8000){
                progress+=speed/(difficulty/5);
            }
            else if(toBlack>0){
                toBlack-=speed/(difficulty*2);
            }
            else{
                reveal=true;
            }
        }
        else if(toBlack<255){
            toBlack+=speed/(difficulty*2);
        }
        else{
            lock=true;
        }
    }
}

//-------------------------

void card::draw(){

}
