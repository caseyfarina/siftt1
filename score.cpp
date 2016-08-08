//
//  score.cpp
//  siftt
//
//  Created by Casey Farina on 2/6/16.
//
//

#include "score.h"
#include "ofApp.h"

scoreDraw::scoreDraw(){
    
}

void scoreDraw::preload(){
    
    
}

bool scoreDraw::getDrawMode(){
    bool tempDrawMode;
    tempDrawMode = drawMode;
    return tempDrawMode;
}

string scoreDraw::getLetter(){
    string tempLetterString;
    tempLetterString = letter;
    return tempLetterString;
}



void scoreDraw::setup(ofVec2f tempLocation, string tempLetter,float tempXspeed){
    
    x = tempLocation[0];
    originalX = tempLocation[0];
    y = tempLocation[1];
    timer = 0;
    dim = ofRandom(0, 60);
    speedY = tempXspeed;
    letter = tempLetter;
    age = 400;
    pathSizeSet = ofRandom(3,20);
    
    ofSetCircleResolution(200);
    //set up some static random values for each gesture
    for(int i = 0; i < 100; i++){
        randomValues.push_back(ofRandom(0, 1.0));
    }
    
    if(letter == "a" || letter == "A" ){
        drawMode = TRUE;
    }else if(letter == "i" || letter == "I"){
        drawMode = FALSE;
    }else if(letter == "g" || letter == "G"){
        drawMode = TRUE;
    }else if(letter == "r" || letter == "R"){
        drawMode = TRUE;
    }else if(letter == "s" || letter == "S"){
        drawMode = TRUE;
    }else if(letter == "c" || letter == "C"){
        drawMode = TRUE;
    }else if(letter == "u" || letter == "U"){
        drawMode = TRUE;
    }else if(letter == "t" || letter == "T"){
        drawMode = TRUE;
    }else if(letter == "n" || letter == "N"){
        drawMode = TRUE;
    }else if(letter == "v" || letter == "V"){
        drawMode = TRUE;
    }else if(letter == "h" || letter == "H"){
        drawMode = TRUE;
    }else if(letter == "y" || letter == "Y"){
        drawMode = TRUE;
    }else if(letter == "w" || letter == "W"){
        drawMode = FALSE;
    }else if(letter == "f" || letter == "F"){
        drawMode = TRUE;
    }else if(letter == "e" || letter == "E"){
        drawMode = TRUE;
    }else if(letter == "q" || letter == "Q"){
        drawMode = FALSE;
    }else if(letter == "k" || letter == "K"){
        drawMode = FALSE;
    }else if(letter == "x" || letter == "X"){
        drawMode = FALSE;
    }else if(letter == "z" || letter == "Z"){
        drawMode = FALSE;
    }else if(letter == "p" || letter == "P"){
        drawMode = FALSE;
    }else if(letter == "b" || letter == "B"){
        drawMode = FALSE;
    }else if(letter == "l" || letter == "L"){
        drawMode = FALSE;
    }else if(letter == ";" || letter == ":" || letter == "!"){
        drawMode = FALSE;
    }else{
        drawMode = TRUE;
    }
    
    
}

void scoreDraw::update(){
    timer++;
    if(timer > age){
        
    }
    y+=speedY;
    path.push_back(ofVec2f(x,y));
    
                   if(path.size() > pathSizeSet){
                       path.erase(path.begin());
                   }
}

void scoreDraw::draw(){
    
    //ofPushMatrix();
   // ofTranslate(ofGetWidth()*.66, -ofGetHeight()*.4);
   // ofRotate(90,0,0,1);
    
    if(letter == "a" || letter == "A"){
        for(int i = 0;i < path.size();i++){
            ofFill();
            ofSetColor(randomValues[i+4]*255,100);
            ofDrawTriangle(
            path[i].x,
            path[i].y+(100.0*randomValues[i+4]*randomValues[30]),
            path[i].x+(100.0*randomValues[i]),
            path[i].y+(200.0*randomValues[i+1]*randomValues[30]),
            path[i].x,
            path[i].y+(600.0*randomValues[i+2]*randomValues[30]));
            
        }
    //ofDrawEllipse(x, y, dim,dim);
    }
    else if(letter == "r" || letter == "R"){
        float accumTri = 0;
        for(int i = 0;i < path.size();i++){
            ofFill();
            
            
            ofSetColor(randomValues[i]*255,randomValues[i+10]*255);
            float triSize = randomValues[i]*50;
            
            ofDrawTriangle(
                           path[i].x,
                           path[i].y+accumTri,
                           path[i].x+(400.0*randomValues[i]*randomValues[30]),
                           path[i].y+(triSize/2)+accumTri,
                           path[i].x,
                           path[i].y+(triSize)+accumTri);
            ofNoFill();
            ofSetLineWidth(2);
            ofSetColor(255,200);
            ofDrawTriangle(
                           path[i].x,
                           path[i].y+accumTri,
                           path[i].x+(400.0*randomValues[i]*randomValues[30]),
                           path[i].y+(triSize/2)+accumTri,
                           path[i].x,
                           path[i].y+(triSize)+accumTri);
            accumTri+=triSize;
            
        }
        //ofDrawEllipse(x, y, dim,dim);
    }else if(letter == "v" || letter == "V"){
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            float xNoise = randomValues[20]*300 * ofSignedNoise(ofGetFrameNum()*0.005+randomValues[i]*400);
            float yNoise = randomValues[29]*300 * ofSignedNoise(ofGetFrameNum()*0.0037+randomValues[i+10]*400);
            ofDrawEllipse(path[i].x+xNoise,
                          path[i].y+yNoise,
                          randomValues[90-i]*20,
                          randomValues[90-i]*20);
            
        }
    }else if(letter == "t" || letter == "T"){
            for(int i = 0;i < path.size();i++){
                ofSetColor(255,(randomValues[70-i]*200));
                float xNoise = randomValues[20]*300 * ofSignedNoise(ofGetFrameNum()*0.0003+randomValues[i]*400);
                float yNoise = randomValues[29]*300 * ofSignedNoise(ofGetFrameNum()*0.00357+randomValues[i+10]*400);
                ofDrawRectangle(path[i].x+xNoise+(i*100*randomValues[3]),
                              path[i].y+yNoise,
                              randomValues[90-i]*10,
                              randomValues[90-i]*100);
                
            }
    }else if(letter == "s" || letter == "S"){
        ofPushStyle();
        ofSetLineWidth(2);
        ofNoFill();
        ofSetColor(255,200);
        ofBeginShape();
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            float xNoise = randomValues[20]*300 * ofSignedNoise(ofGetFrameNum()*0.0003+randomValues[i]*400);
            float yNoise = randomValues[29]*300 * ofSignedNoise(ofGetFrameNum()*0.00357+randomValues[i+10]*400);
            ofVertex(path[i].x+xNoise,
                            path[i].y+yNoise);
            
        }
        ofEndShape();
        ofPopStyle();
    }else if(letter == "c" || letter == "C"){
        ofPolyline quickU;
        ofPushStyle();
        ofSetLineWidth(2);
        ofNoFill();
        ofSetColor(255,200);
        
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            float xNoise = randomValues[20]*100 * ofSignedNoise(ofGetFrameNum()*0.053+randomValues[i]*400);
            float yNoise = randomValues[29]*100 * ofSignedNoise(ofGetFrameNum()*0.07+randomValues[i+10]*400);
            quickU.curveTo(path[i].x+xNoise,
                           path[i].y+yNoise);
            
        }
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            float xNoise = randomValues[07]*200 * ofSignedNoise(ofGetFrameNum()*0.33+randomValues[i+9]*400);
            float yNoise = randomValues[9]*200 * ofSignedNoise(ofGetFrameNum()*0.51+randomValues[i+15]*400);
            quickU.curveTo(path[i].x+xNoise,
                           path[i].y+yNoise);
            
        }
        
        quickU.draw();
        ofPopStyle();
        quickU.clear();
    }else if(letter == "u" || letter == "U"){
        ofPolyline quickU;
        ofPushStyle();
        ofSetLineWidth(2);
        ofNoFill();
        ofSetColor(255,200);
        
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            float xNoise = randomValues[20]*300 * ofSignedNoise(ofGetFrameNum()*0.0053+randomValues[i]*400);
            float yNoise = randomValues[29]*300 * ofSignedNoise(ofGetFrameNum()*0.007+randomValues[i+10]*400);
            quickU.curveTo(path[i].x+xNoise,
                     path[i].y+yNoise);
            
        }
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            float xNoise = randomValues[20]*500 * ofSignedNoise(ofGetFrameNum()*0.0033+randomValues[i+9]*400);
            float yNoise = randomValues[29]*500 * ofSignedNoise(ofGetFrameNum()*0.0051+randomValues[i+15]*400);
            quickU.curveTo(path[i].x+xNoise,
                           path[i].y+yNoise);
            
        }
        quickU.draw();
        ofPopStyle();
        quickU.clear();
    }else if(letter == "g" || letter == "G"){
        for(int j = 0;j < randomValues[3]*40;j++){
            for(int i = 0;i < path.size();i++){
                ofSetColor(255,(randomValues[70-i]*200));
                ofDrawEllipse(path[i].x+(i*10.0*randomValues[9]),
                              path[i].y+(j*40.0*randomValues[3]),
                              randomValues[90]*20,
                              randomValues[90]*20);
                
            }
        }
        
    }else if(letter == "i" || letter == "i"){
        for(int j = 0;j < randomValues[3]*40;j++){
            for(int i = 0;i < path.size();i++){
                ofSetColor(255,(randomValues[70-i]*200));
                ofDrawEllipse(path[i].x+(i*100.0*randomValues[9]),
                              path[i].y+(j*400.0*randomValues[3]),
                              randomValues[90]*5,
                              randomValues[90]*5);
                
            }
        }
    
    }else if(letter == "o" || letter == "O"){
        
        ofPushStyle();
        ofSetCircleResolution(30);
        for(int i = 0;i < path.size();i++){
            ofNoFill();
            ofSetColor(255,(randomValues[70-i]*200));
            float tempLineWidth = ofRandom(0,5);
            ofSetLineWidth(tempLineWidth);
            float tempRad = ofRandom(0,randomValues[60]*300);
            
            ofDrawCircle(x, y, tempRad);
        }
        ofPopStyle();
    }else if(letter == "h" || letter == "H"){
        for(int j = 0;j < randomValues[3]*10;j++){
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            ofDrawEllipse(path[i].x+(i*40.0*randomValues[9]),
                          path[i].y+(j*10.0*randomValues[3]),
                          randomValues[90]*10,
                          randomValues[90]*10);
            
        }
        }
    }else if(letter == "y" || letter == "Y"){
        ofNoFill();
        //ofSetLineWidth(ofRandom(1,9));
        ofSetColor(255,200);
        ofBeginShape();
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            ofCurveVertex((path[i].x)+ofRandom(-5,5)+(i*50*randomValues[21]),path[i].y+ofRandom(-30*randomValues[20],30*randomValues[6]));
            
        }
        for(int i = 0;i < path.size();i++){
            ofSetColor(255,(randomValues[70-i]*200));
            ofCurveVertex((path[i].x)+ofRandom(-5,5)+(i*20*randomValues[20]),path[i].y+ofRandom(-20*randomValues[9],20*randomValues[7]));
            
        }
        ofEndShape();
    }else if(letter == "n" || letter == "N"){
        for(int i = 0;i < path.size();i++){
            
            ofSetColor(255,(randomValues[70-i]*200));
            ofSetLineWidth(randomValues[i]*4);
            ofDrawLine(path[i].x+ofRandom(-5,5),
                       path[i].y+(i*10*randomValues[5])+ofRandom(-5,5),
                       path[i].x+((randomValues[50-i])*500+100)+ofRandom(-5,5),
                       path[i].y+(i*10*randomValues[5])+ofRandom(-5,5));
            
        }
        
    }else if(letter == "f" || letter == "F"){
        
        for(int i = 0;i < path.size();i++){
            
            ofSetColor(255,(randomValues[70-i]*200));
            ofSetLineWidth(randomValues[i]*4);
            ofDrawLine(path[i].x,
                       path[i].y+(i*10*randomValues[5]),
                       path[i].x+((randomValues[50-i])*500+100),
                       path[i].y+(i*10*randomValues[5]));
            
        }
    }else if(letter == "e" || letter == "E"){
        
        for(int i = 0;i < path.size();i++){
            
            ofSetColor(255,(randomValues[70-i]*200));
            ofSetLineWidth(randomValues[i]*20);
            ofDrawLine(path[i].x,
                       path[i].y+((randomValues[60-i]-0.5)*300),
                       path[i].x+((randomValues[50-i])*500),
                       path[i].y+((randomValues[60-i]-0.5)*300));
            
        }
    }else{
        ofSetColor(randomValues[0]*255,100);
        ofDrawEllipse(x, y, dim,dim);
    }
    //ofPopMatrix();
}