#include "testApp.h"

//evertime a person died, some particle evaporated

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(255);
    cam.enableMouseInput();
    cam.enableMouseMiddleButton();
    image.loadImage("map.jpg");
    int w = image.getWidth();
    int h = image.getHeight();
    int step = 1;
    for(int x=0;x<w;x+=step){
        for(int y=0;y<h;y+=step){
            ofColor c = image.getColor(x, y);
            float intensity = c.getLightness();
            if(intensity <= 180){
                float saturation = c.getLightness();
                float z = ofMap(saturation, 0, 255, 0, 300);
                ofVec3f pos(4*x,4*y,z);
                particles.push_back(pos);
                offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
            }
        }
    }
    death = false;
    birth = false;
    distance = 400;
    ofEnableAlphaBlending();
    
    //osc
    receiver.setup(PORT);
    
    
    //text
    //   referencerLocation.loadFont("Helvetica.dfont", 10);
    //referencer location
    rLocationX = 800;
    rLocationY = 300;
    
    manhattanbirth = false;
    manhattandeath = false;
    bronxbirth = false;
    bronxdeath = false;
    queensbirth = false;
    queensdeath = false;
    brooklynbirth = false;
    brooklyndeath = false;
    islandbirth = false;
    islanddeath = false;
    
}
//--------------------------------------------------------------
void testApp::update(){
    
    for ( int i = 0 ; i < particles.size();i++){
        if (particles[i].location.x > 1215 &&
            particles[i].location.x < 1430 &&
            particles[i].location.y > -420 &&
            particles[i].location.y < 255) {
            particles[i].belongsTo = "manhattan";
        }
        if (particles[i].location.x > 1180 &&
            particles[i].location.x < 1590 &&
            particles[i].location.y > 255 &&
            particles[i].location.y < 720) {
            particles[i].belongsTo = "bronx";
        }
        if (particles[i].location.x > 1690 &&
            particles[i].location.x < 2255 &&
            particles[i].location.y > 365 &&
            particles[i].location.y < -180) {
            particles[i].belongsTo = "queens";
        }
        if (particles[i].location.x > 1385 &&
            particles[i].location.x < 2255 &&
            particles[i].location.y > 55 &&
            particles[i].location.y < -790) {
            particles[i].belongsTo = "brooklyn";
        }
        if (particles[i].location.x > 1005 &&
            particles[i].location.x < 1345 &&
            particles[i].location.y > -705 &&
            particles[i].location.y < -1625) {
            particles[i].belongsTo = "staten_island";
        }
        
        
    }
    
    
    int message;
    //OSC::::
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        if (m.getAddress() == "/manhattanbirth") {
            //            message = m.getArgAsFloat(0);
            //           cout<<message<<endl;
            //           if(message != 0){
            //               death  = true;
            //           }
            manhattanbirth = true;
        }
        if (m.getAddress() == "/manhattandeath") {
            manhattandeath = true;
        }
        if (m.getAddress() == "/brooklynbirth") {
            brooklynbirth = true;
        }
        if (m.getAddress() == "/brooklyndeath") {
            brooklyndeath = true;
        }
        
        if (m.getAddress() == "/queensndeath") {
            queensdeath = true;
        }
        if (m.getAddress() == "/queensbirth") {
            queensbirth = true;
        }
        if (m.getAddress() == "/islandbirth") {
            islandbirth = true;
        }
        if (m.getAddress() == "/islanddeath") {
            islanddeath = true;
        }
        if (m.getAddress() == "/bronxbirth") {
            bronxbirth = true;
        }
        if (m.getAddress() == "/bronxdeath") {
            bronxdeath = true;
        }
        
        
        
    }
    //-----------
    //death
    //-----------
    
    int get = ofRandom(0,particles.size());
    if(death){
        ofVec3f getPosition(particles[get].location);
        for(int i=0;i < 200;i++){
            particles.push_back(getPosition
                                +
                                ofVec3f(ofRandom(-EVENT_RADIUS,EVENT_RADIUS),ofRandom(-EVENT_RADIUS,EVENT_RADIUS),ofRandom(-EVENT_RADIUS,EVENT_RADIUS))
                                );
            particles.back().seek(getPosition + ofVec3f(0, 0, -100));
            particles.back().maxspeed = 2000;
            particles.back().maxforce = 0.5;
            particles.back().just_died = true;
            //cam.lookAt(getPosition);
            
            
        }
        death = false;
    }
    
    //-----------
    //birth
    //-----------
    
    int get2 = ofRandom(0,particles.size());
    if(birth){
        
        ofVec3f getPosition(particles[get2].location);
        for (int i=0;i<200; i++) {
            particles.push_back(getPosition+
                                ofVec3f(0,0,-600)
                                );
            particles.back().seek(getPosition +
                                  ofVec3f(ofRandom(-EVENT_RADIUS,EVENT_RADIUS),ofRandom(-EVENT_RADIUS,EVENT_RADIUS),ofRandom(-EVENT_RADIUS,EVENT_RADIUS))
                                  );
            particles.back().maxspeed = 4000;
            particles.back().maxforce = 10;
            particles.back().newly_born = true;
            particles.back().color.g = 0;
        }
        birth = false;
    }
    
    int numVerts = particles.size();
    for(int i=0;i<particles.size();i++){
        //jitter
        
        
        if(!particles[i].just_died && !particles[i].newly_born){
            ofVec3f vert = particles[i].location;
            float time = ofGetElapsedTimef();
            float timeScale = 5.0;
            float displacementScale = 0.75;
            ofVec3f timeOffsets = offsets[i];
            vert.x+=(ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
            vert.y+=(ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
            vert.z+=(ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
            particles[i].location.set(ofVec3f(vert.x,vert.y,vert.z));
        }
        
        
        //-----------
        //death
        //-----------
        
        if(particles[i].just_died){
            //Fountain
            particles[i].update();
            //Color Chaning
            particles[i].color.a = ofMap(particles[i].new_time, 0, 1400, 255, 0);
            particles[i].color.r = ofMap(particles[i].new_time, 0, 1400, 173, 0);
            particles[i].color.g = ofMap(particles[i].new_time, 0, 1400, 44, 0);
            particles[i].color.b = ofMap(particles[i].new_time, 0, 1400, 44, 0);
            
            particles[i].new_time++;
            if(particles[i].new_time > 1300 ){
                particles[i].just_died = false;
                particles[i].seek(particles[i].location);
                particles[i].color.r = 0;
                particles[i].color.g = 0;
                particles[i].color.b = 0;
                particles[i].new_time = 0;
            }
        }
        
        //-----------
        //birth
        //-----------
        
        if(particles[i].newly_born){
            //Fountain
            particles[i].update();
            //color changing
            //particles[i].color.a = ofMap(particles[i].new_time, 0, 200, 255, 0);
            particles[i].new_time++;
            ofVec3f target;
            int lucknumber = ofRandom(0,particles.size());
            if (particles[lucknumber].newly_born == false && particles[lucknumber].just_died == false) {
                particles[i].color.r = 61;
                particles[i].color.g = 169;
                particles[i].color.b = 190;
                
                target.set(particles[lucknumber].location + ofVec3f(0,0,100));
                if(abs(particles[i].location.z - target.z) < 10){
                    particles[i].newly_born = false;
                    particles[i].color.a = 100;
                    particles[i].color.g = 0;
                    particles[i].color.b = 0;
                    particles[i].new_time = 0;
                }
            }
            
            
            
            
        }
        
    }
    
    
    

    
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    cam.begin();
    
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofEllipse(rLocationX, rLocationY, 10, 10);
    ofPopStyle();
    
    for(int i=0;i<particles.size();i++){
        mesh.addVertex(particles[i].location);
        mesh.addColor(particles[i].color);
    }
    
    
    ofPushMatrix();
    ofScale(-1, 1,-1);
    ofRotateZ(150);
    ofPushStyle();
    mesh.drawFaces();
    ofPopStyle();
    ofPopMatrix();
    
    cam.end();
    
    
    manhattanbirth = false;
    manhattandeath = false;
    bronxbirth = false;
    bronxdeath = false;
    queensbirth = false;
    queensdeath = false;
    brooklynbirth = false;
    brooklyndeath =false;
    islandbirth = false;
    islanddeath = false;
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
    
    switch (key) {
        case 'f':
            ofSetFullscreen(true);
            break;
        case 'n':
            birth = true;
            break;
        case 'm':
            death = true;
            break;
            
        case 'a':
            rLocationX-= 5;
            break;
        case'w':
            rLocationY-= 5;
            break;
        case's':
            rLocationY+= 5;
            break;
        case 'd':
            rLocationX+= 5;
            break;
    }
    cout<<"x is:"<<rLocationX<<" &&& y is: "<<rLocationY<<endl;
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
