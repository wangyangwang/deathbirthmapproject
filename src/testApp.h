#pragma once

#include "ofMain.h"
#include "people.h"
#include "ofxOsc.h"

#define PORT 4444
#define EVENT_RADIUS 30
class testApp : public ofBaseApp{

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofImage image;
    ofEasyCam cam;
    vector<ofVec3f> offsets;
    vector<people> particles;
    
    bool birth, death;
    
    float distance;
    
    ofxOscReceiver receiver;
    
    
    ofTrueTypeFont referencerLocation;
    
    float rLocationX, rLocationY;
    
    bool manhattanbirth, manhattandeath,
    brooklynbirth, brooklyndeath,
    queensbirth, queensdeath,
    islanddeath, islandbirth,
    bronxbirth, bronxdeath;
 
    
   
};
