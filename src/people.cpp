//
//  people.cpp
//  Transience
//
//  Created by Yang Wang on 5/4/14.
//
//

#include "people.h"
people::people(ofVec3f _location){
    acceleration = ofVec3f(0,0,0);
    velocity = ofVec3f(0,0,0);
    location = _location;
    maxspeed = ofRandom(3,10);
    maxforce = ofRandom(0.1,0.4);
    newly_born = false;
    just_died = false;
    color.r = 0;
    color.g = 0;
    color.b = 0;
    color.a = 80;
    new_time = 0;
    belongsTo = "";
}

void people::update(){
    velocity += acceleration;
    velocity.limit(maxspeed);
    location += velocity;
    acceleration = acceleration * 0;
    
}

void people::seek(ofVec3f target){
    ofVec3f desired = target - location;
    desired.normalize();
    desired *= maxspeed;
    ofVec3f steer = desired - velocity;
    steer.limit(maxforce);
    acceleration += steer;
}


