#pragma once

#include "ofMain.h"
#include "card.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void reset();
    void shutDown();

    //key control
//    bool keyIsDown [255];

    float speed =25;
    float tentacleSpeed = 0.0002;
    float difficulty = 0.5;
    float scale = 0.2;
    ofVec2f dotPos, inkWell;
    ofPolyline tentacle;
    ofImage moon, sun, sword;
    card Moon, Sun, Sword;
    ofVec2f myo;
    int target = 0;
    int heading = 0;
    float progress = 0;
    float threshold;
    bool timesUp = false;
    float durationMins = 3;
    int elapsed = 0;

    //firmata code, based on oF firmata example
    ofArduino	ard;
    bool		bSetupArduino;			// flag variable for setting up arduino once

private:

    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
    void updateArduino();

};
