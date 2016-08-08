//
//  score.h
//  siftt
//
//  Created by Casey Farina on 2/6/16.
//
//

#ifndef __siftt__score__
#define __siftt__score__
#include "ofMain.h" // we need to include this to have a reference to the openFrameworks framework

#include <iostream>

class scoreDraw {
    
public: // place public functions or variables declarations here
    
    // methods, equivalent to specific functions of your class objects
    void setup(ofVec2f, string,float);	// setup method, use this to setup your object's initial state
    void update();  // update method, used to refresh your objects properties
    void draw();    // draw method, this where you'll do the object's drawing
    void preload();
    bool getDrawMode();
    string getLetter();
    
    // variables
    float x;        // position
    float y;
    float speedY;   // speed and direction
    float speedX;
    int dim;        // size
    int timer;
    int age;
    vector <ofVec2f> path;
    bool drawMode;
    vector <float> randomValues;
    int originalX;
    int pathSizeSet;
    
    
    
    ofColor color;  // color using ofColor type
    
    string letter;
    
    scoreDraw();  // constructor - used to initialize an object, if no properties are passed the program sets them to the default value
private: // place private functions or variables declarations here
}; // don't forget the semicolon!

#endif /* defined(__siftt__score__) */
