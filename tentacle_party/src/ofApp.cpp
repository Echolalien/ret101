#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetBackgroundAuto(true);
    ofSetBackgroundColor(0);
    ofSetCircleResolution(600);

    ofHideCursor();

    //image calibration
    moon.load("moon.jpg");
    sun.load("sun.jpg");
    sword.load("sword.jpg");
    moon.resize(moon.getWidth()*scale, moon.getHeight()*scale);
    sun.resize(sun.getWidth()*scale, sun.getHeight()*scale);
    sword.resize(sword.getWidth()*scale, sword.getHeight()*scale);

    //card calibration
    Moon.setup(ofGetHeight()-moon.getWidth()*2.6, moon.getHeight()*0.4, difficulty, speed);
    Sun.setup(ofGetHeight()/2-sun.getWidth()/2, ofGetHeight()/2-sun.getHeight()/2, difficulty, speed);
    Sword.setup(sword.getWidth()*1.6, ofGetHeight()-sword.getHeight()*1.4, difficulty, speed);

    //line points
    inkWell.set(25,25);
//    dotPos.set(inkWell);
//    stalk.set(Sun.x+sun.getWidth()/2, Sun.y+sun.getHeight()/2);
//    club.set(stalk);

    //myoelectric threshold. my muscles go from 150-660 when activated, 45-90 when cold.
    threshold = 250;

    //find arduino, wait for init then run setup. change connect value to correct USB port
    ard.connect("/dev/ttyACM0", 57600);
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino	= false;

}

//--------------------------------------------------------------
void ofApp::update(){

    updateArduino();

    //readouts
    cout << "myoX = " << myo.x << "|| myoY = " << myo.y << endl;
//    cout << "dot: " << dotPos.x << ", " << dotPos.y << " mouse: " <<mouseX+ofGetHeight()/2-ofGetWidth()/2 << ", " << mouseY << endl;

    //using myo data to control dot (deprecated)
    //dotPos.x=myo.x;
    //dotPos.y=myo.y;

    //mouse controls for point grabbing
//    dotPos.x=mouseX+ofGetHeight()/2-ofGetWidth()/2;
//    dotPos.y=mouseY;

    //directional key test controls, replace with osc analog data (deprecated)
//    if(keyIsDown[356] && dotPos.x>=inkWell.x+25){
//        dotPos.x-=5;
//    }
//    if(keyIsDown[357] && dotPos.y>=inkWell.y+25){
//        dotPos.y-=5;
//    }
//    if(keyIsDown[358] && dotPos.x <= ofGetHeight()-50){
//        dotPos.x+=5;
//    }
//    if(keyIsDown[359] && dotPos.y <= ofGetHeight()-50){
//        dotPos.y+=5;
//    }

    //myoelectric target controls - to test this without any myoelectric sensors, just plug some capacitive wire into A0 and A1 on the arduino and change the threshold value in setup. ensure arduino has correct USB port details.
    if(myo.x>threshold && myo.y<=threshold){
        target=1;
    }
    else if(myo.x<=threshold && myo.y>threshold){
        target=2;
    }
    else if(myo.x>threshold && myo.y>threshold){
        target=3;
    }
    else{
        target=0;
    }

    //heading calculations + progress updates
    if(elapsed>durationMins*60*60){
        timesUp=true;
        shutDown();
    }
    else{
        elapsed++;
    }
    if(timesUp==false){
        if(heading!=target){
            if(progress<=0){
                heading=target;
            }
            else{
                progress-=tentacleSpeed*speed;
            }
        }
        else{
            if(progress<1){
                progress+=tentacleSpeed*speed;
            }
        }
    }

    //card connections
    if(heading==1){
        if(progress>=1){
            Moon.touch=true;
        }
        else{
            Moon.touch=false;
        }
        Sun.touch=false;
        Sword.touch=false;
    }
    if(heading==2){
        if(progress>=1){
            Sword.touch=true;
        }
        else{
            Sword.touch=false;
        }
        Sun.touch=false;
        Moon.touch=false;
    }
    if(heading==3){
        if(progress>=1){
            Sun.touch=true;
        }
        else{
            Sun.touch=false;
        }
        Moon.touch=false;
        Sword.touch=false;
    }

    //pass points to tentacle based on current heading
    if(heading==0){
        tentacle.curveTo(inkWell);
        tentacle.curveTo(inkWell);
        tentacle.curveTo(inkWell);
        tentacle.curveTo(inkWell);
    }
    else if(heading!=0){
        tentacle.curveTo(-25,-60);
        tentacle.curveTo(inkWell);
        tentacle.curveTo(110,70);
        tentacle.curveTo(140, 200);
        tentacle.curveTo(295, 195);
        tentacle.curveTo(350, 360);
    }
    if(heading==1){
        tentacle.curveTo(530, 310);
        tentacle.curveTo(680, 480);
        tentacle.curveTo(835, 475);
        tentacle.curveTo(940,390);
        tentacle.curveTo(970, 285);
        tentacle.curveTo(955,170);
        tentacle.curveTo(880,90);
        tentacle.curveTo(775,60);
        tentacle.curveTo(675,110);
        tentacle.curveTo(665,230);
        tentacle.curveTo(730,290);
        tentacle.curveTo(830,280);
        tentacle.curveTo(860,208);
        tentacle.curveTo(815,159);
        tentacle.curveTo(748,181);
        tentacle.curveTo(761,245);
        tentacle.curveTo(805,230);
        tentacle.curveTo(793,199);
        tentacle.curveTo(Moon.x+moon.getWidth()/2, Moon.y+moon.getHeight()/2);
        tentacle.curveTo(876,255);
    }
    if(heading==2){
        tentacle.curveTo(439,387);
        tentacle.curveTo(323,532);
        tentacle.curveTo(461,585);
        tentacle.curveTo(377,700);
        tentacle.curveTo(413,811);
        tentacle.curveTo(Sword.x+sword.getWidth()/2, Sword.y+sword.getHeight()/2);
        tentacle.curveTo(351,1079);
    }
    if(heading==3){
        tentacle.curveTo(Sun.x+sun.getWidth()/2, Sun.y+sun.getHeight()/2);
        tentacle.curveTo(529,908);
    }

    //curve the tentacle to mouse/keyboard co-ordinates for testing
//  tentacle.curveTo(dotPos);
//  tentacle.curveTo(mouseX+ofGetHeight()/2-ofGetWidth()/2, mouseY);
    tentacle.simplify(0.5);

    Moon.update();
    Sun.update();
    Sword.update();
    
    /* code below deprecated, moves end of tentacle slowly towards an analog grid value. myoelectrics values proved not reliable enough for this method
    
    //need to move tentacle forward and back, move mass one way or other, bend tip one way or other

    //calculate relative position and clamp it to positive #
    relative = dotPos-stalk;
    relativePositive.x = relative.x*(ofSign(relative.x));
    relativePositive.y = relative.y*(ofSign(relative.y));

    //calculate proportional distance for 1 pixel of movement
    percent.x = relativePositive.x/((relativePositive.x)+(relativePositive.y));
    percent.y = relativePositive.y/((relativePositive.x)+(relativePositive.y));

    //feedback
    cout << relative.x << ", " << relative.y << endl << relativePositive.x << ", " << relativePositive.y << endl << percent << endl;


    //unclamp and move stalk to 50px away, club trailing ahead
    if(relativePositive.x>5 || relativePositive.y>5){
            stalk.x+=speed*(percent.x*ofSign(relative.x));
            stalk.y+=speed*(percent.y*ofSign(relative.y));
            club=stalk+25;
    }


    //turn club to face dotPos*/


}

//--------------------------------------------------------------
void ofApp::draw(){
    ofTranslate(ofGetWidth()/2-ofGetHeight()/2,0);
    ofSetColor(255);
    ofDrawRectangle(0,0,ofGetHeight(),ofGetHeight());
    ofSetColor(0);
    ofDrawCircle(inkWell, 10);

    ofPushStyle();
    ofPushMatrix();
        if(timesUp==false){
            ofSetColor(0);
        }
        else{
            ofSetColor(0,0,0,Moon.toBlack);
        }
        ofTranslate(Moon.x, Moon.y);
        ofRotate(-45);
        ofTranslate(-moon.getWidth()/1.3, 10);
        if(Moon.progress>0){
            for(float i; i<Moon.progress; i+=5){
                //draw circles inside bounds of 5px of card
                float xPos = ofNoise(i, 5);
                float yPos = ofNoise(i*10, 5);
                ofDrawCircle(10+(xPos*(moon.getWidth()-20)), 10+(yPos*(moon.getHeight()-20)), ((Moon.progress-i)/8000)*20);
            }
            if(Moon.toBlack<255 && timesUp==false){
                ofSetColor(0,0,0, 255-Moon.toBlack);
                ofDrawRectangle(0,0, moon.getWidth(), moon.getHeight());
            }
        }
        if(Moon.reveal==true || Moon.lock==true){
            ofDrawRectangle(0,0, moon.getWidth(), moon.getHeight());
            ofSetColor(255,255,255,Moon.toBlack);
            moon.draw(0,0);
        }
    ofPopMatrix();
    ofPopStyle();

    ofPushStyle();
    ofPushMatrix();
        if(timesUp==false){
            ofSetColor(0);
        }
        else{
            ofSetColor(0,0,0,Sun.toBlack);
        }
        ofTranslate(Sun.x, Sun.y);
        ofRotate(-45);
        ofTranslate(-sun.getWidth()/1.3, 10);
        if(Sun.progress>0){
            for(float i; i<Sun.progress; i+=5){
                //draw circles inside bounds of 5px of card
                float xPos = ofNoise(i*20, 5);
                float yPos = ofNoise(i*50, 5);
                ofDrawCircle(10+(xPos*(sun.getWidth()-20)), 10+(yPos*(sun.getHeight()-20)), ((Sun.progress-i)/8000)*20);
            }
            if(Sun.toBlack<255 && timesUp==false){
                ofSetColor(0,0,0, 255-Sun.toBlack);
                ofDrawRectangle(0,0, sun.getWidth(), sun.getHeight());
            }
        }
        if(Sun.reveal==true || Sun.lock==true){
            ofDrawRectangle(0,0, sun.getWidth(), sun.getHeight());
            ofSetColor(255,255,255,Sun.toBlack);
            sun.draw(0,0);
        }
    ofPopMatrix();
    ofPopStyle();

    ofPushStyle();
    ofPushMatrix();
        if(timesUp==false){
            ofSetColor(0);
        }
        else{
            ofSetColor(0,0,0,Sword.toBlack);
        }
        ofTranslate(Sword.x, Sword.y);
        ofRotate(-45);
        ofTranslate(-sword.getWidth()/1.3, 10);
        if(Sword.progress>0){
            for(float i; i<Sword.progress; i+=5){
                //draw circles inside bounds of 5px of card
                float xPos = ofNoise(i*20, 5);
                float yPos = ofNoise(i*50, 5);
                ofDrawCircle(10+(xPos*(sword.getWidth()-20)), 10+(yPos*(sword.getHeight()-20)), ((Sword.progress-i)/8000)*20);
            }
            if(Sword.toBlack<255 && timesUp==false){
                ofSetColor(0,0,0, 255-Sword.toBlack);
                ofDrawRectangle(0,0, sword.getWidth(), sword.getHeight());
            }
        }
        if(Sword.reveal==true || Sword.lock==true){
            ofDrawRectangle(0,0, sword.getWidth(), sword.getHeight());
            ofSetColor(255,255,255,Sword.toBlack);
            sword.draw(0,0);
        }
    ofPopMatrix();
    ofPopStyle();

    ofPushStyle();
        ofSetColor(0);
//        tentacle.draw();
            for(float i = 0; i<progress-0.05; i+=0.001){
                ofDrawCircle(tentacle.getPointAtPercent(i),5);
            }
            for(float i = 0; i<5; i+=0.1){
                ofDrawCircle(tentacle.getPointAtPercent(progress-0.05+(i*0.01)),5-i*0.7);
            }
        tentacle.clear();
    ofPopStyle();

    //deprecated test drawings
//    ofNoFill();
//    ofSetColor(255,0,0);
//    ofDrawCircle(stalk, 50);
//    ofSetColor(255,0,0);
//    ofDrawCircle(dotPos, 5);
//    ofFill();
}

//--------------------------------------------------------------
void ofApp::reset(){
    elapsed=0;
    timesUp=false;
    Moon.lock=false;
    Moon.reveal=false;
    Moon.touch=false;
    Moon.progress=0;
    Moon.toBlack=255;
    Sword.lock=false;
    Sword.reveal=false;
    Sword.touch=false;
    Sword.progress=0;
    Sword.toBlack=255;
    Sun.lock=false;
    Sun.reveal=false;
    Sun.touch=false;
    Sun.progress=0;
    Sun.toBlack=255;
    progress=0;
}

//--------------------------------------------------------------
void ofApp::shutDown(){
    if(progress>0){
        progress-=tentacleSpeed*speed;
    }
    if(Moon.toBlack>0){
        Moon.toBlack--;
    }
    if(Sun.toBlack>0){
        Sun.toBlack--;
    }
    if(Sword.toBlack>0){
        Sword.toBlack--;
    }
    if(progress<=0 && Moon.toBlack<=0 && Sun.toBlack<=0 && Sword.toBlack<=0 && heading!=0){
        reset();
    }
}

//--------------------------------------------------------------
void ofApp::setupArduino(const int & version) {

    //code adapted from oF firmata example
    // remove listener because we don't need it anymore
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);

    // it is now safe to send commands to the Arduino
    bSetupArduino = true;

    // print firmware name and version to the console
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();

    // Note: pins A0 - A5 can be used as digital input and output.
    // Refer to them as pins 14 - 19 if using StandardFirmata from Arduino 1.0.
    // If using Arduino 0022 or older, then use 16 - 21.
    // Firmata pin numbering changed in version 2.3 (which is included in Arduino 1.0)

    // set pins A0&1 to analog input to receive EMG data
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    ard.sendAnalogPinReporting(1, ARD_ANALOG);

    // Listen for changes on the analog pins
    ofAddListener(ard.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

//--------------------------------------------------------------
void ofApp::updateArduino(){

    // update the arduino, get any data or messages.
    // the call to ard.update() is required
    ard.update();


    //if you need to test setup at hardware level
//	// do not send anything until the arduino has been set up
//	if (bSetupArduino) {
//        // fade the led connected to pin D11
//		ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
//	}

}

//--------------------------------------------------------------
void ofApp::analogPinChanged(const int & pinNum) {
    // do something with the analog input. here we're simply going to print the pin number and
    // value to the screen each time it changes
    myo.x = ard.getAnalog(0);
    myo.y = ard.getAnalog(1);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

//    //keyboard control
//    keyIsDown[key] = true;

//    cout<<key<<endl;

//    key targeting to test for myos
//    if(key==49){
//        target=1;
//    }
//    if(key==50){
//        target=2;
//    }
//    if(key==51){
//        target=3;
//    }
//    if(key==96){
//        target=0;
//    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    //keyboard control
//    keyIsDown[key] = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
