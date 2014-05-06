//
//  people.h
//  Transience
//
//  Created by Yang Wang on 5/4/14.
//
//

#ifndef __Transience__people__
#define __Transience__people__

#include <iostream>
#include "ofMain.h"

class people {
public:
    //constructor
    people(ofVec3f _location);
    
    //methods
    void update();
    void seek(ofVec3f _target);
    
    
    //properties
    ofVec3f location;
    bool newly_born;
    bool just_died;
    ofColor color;
    float new_time;
    ofVec3f velocity;
    ofVec3f acceleration;
    float maxforce;
    float maxspeed;
    string belongsTo;
   };

#endif /* defined(__Transience__people__) */
