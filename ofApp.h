#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOsc.h"
#include "score.h"
#include "ofxMtlMapping2D.h"



// listen on port 12345
#define PORT 3333
#define NUM_MSG_STRINGS 20
#define HOST "169.254.235.79"

#define PORTSIFTT 3339
#define NUM_MSG_STRINGSSIFTT 20
#define HOSTSIFTT "169.254.235.79"

#define PORT1 3335
#define NUM_MSG_STRINGS 20
#define HOST1 "169.254.235.79"




class Data {
public:
    int id;
    int age;
    int wordCount;
    int currentWord;
    int currentLetter;
    string particleTweet;
    vector< string > tweetWords; // used to cycle words per tweet
    int counter;
    int group;
    int lifetime;
    
    //ofImage    *    texturePtr;
    
};

class CustomParticle: public ofxBox2dCircle{
public:
    void setupTheCustomData(string tempTweet,int tempGroup) {
        
        setData(new Data());
        Data *theData = (Data*)getData();
        
        theData->id = ofRandom(0, 100);
        theData->age = 0;
        theData->particleTweet = tempTweet;
        theData->tweetWords = ofSplitString(theData->particleTweet," ",TRUE);
        theData->wordCount = theData->tweetWords.size();
        theData->currentWord = 0;
        theData->group = tempGroup;
        theData->lifetime = tempTweet.size();
    }
    
    void update(){
        Data* theData = (Data*)getData();
        if(theData) {
            theData->age = theData->age + 1;
            
            
        }
        
    }
    
    int getGroup(){
        Data* theData = (Data*)getData();
        int tempGroup;
        tempGroup = theData->group;
        return tempGroup;
        
    }
    
    string getTweet(){
        Data* theData = (Data*)getData();
        string tempTweet;
        tempTweet = theData->particleTweet;
        return tempTweet;
    }
    
    
    void incrementCurrentWord(){
        Data* theData = (Data*)getData();
        int tempWord;
        tempWord = theData->currentWord;
        tempWord++;
        tempWord = tempWord%theData->tweetWords.size();
        theData->currentWord = tempWord;
        
    }
    
    void incrementLetter(){
        Data* theData = (Data*)getData();
        int tempLetter;
        tempLetter = theData->currentLetter;
        tempLetter++;
        tempLetter = tempLetter%theData->particleTweet.size();
        theData->currentLetter = tempLetter;
        
    }
    
    string getLetter(){
        Data* theData = (Data*)getData();
        string tempLetter;
        tempLetter = theData->particleTweet[theData->currentLetter];
        return tempLetter;
    }
    
    string getWord(){
        Data* theData = (Data*)getData();
        string tempWord;
        tempWord = theData->tweetWords[theData->currentWord];
        return tempWord;
        
    }
    /*
    void setTexture(ofImage * texture) {
        Data* theData = (Data*)getData();
        if(theData) {
            theData->texturePtr = texture;
            theData->texturePtr->getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
            theData->texturePtr->getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
            }
    }
     */
    
    int getAge(){
        Data* theData = (Data*)getData();
        if(theData){
            
            return theData->age;;
        }
    }
    
    int getlifeTime(){
        Data* theData = (Data*)getData();
        if(theData){
            
            return theData->lifetime;;
        }
    }
    };

class ofApp : public ofBaseApp {
public:
    
    void setup();
    void update();
    void draw();
    void contactStart(ofxBox2dContactArgs &e);
	void contactEnd(ofxBox2dContactArgs &e);
    void loadImageSequences();
    void keyPressed(int key);
    void scoreDrawer(int i, vector <ofImage> & sift_score);
    
    //The sorting function THIS IS NOT WORKING
    static bool sortMe(CustomParticle::ofxBox2dCircle & a, CustomParticle::ofxBox2dCircle & b);
    
    
    //These are the settins to divide the screen in mainMode 1
    float firstThird;
    float secondThird;
    
    //This is the setting for the divider in mainMode 2
    float dividerMode2;
    
    //All of these are setting up the font settings and OSC ports
    ofTrueTypeFont font;
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    ofxOscReceiver receiverSIFTT;
    ofxOscSender senderSIFTT;
    
    ofxOscReceiver receiverS;
    ofxOscSender senderS;
    
    ofVideoPlayer barrierVid;
    
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    int sifttViewCounter =0;
    
    int mouseX, mouseY;
    string mouseButtonState;
    
    int numberofargs;
    ofImage receivedImage;
    string folderNames;
    string argtype;
    string argname;
    string msg_string;
    
    int start;
    
    int stringLength;
    int currentCount;
    float numberInput;
    
    vector< ofPoint > mode2stringLocations;
    vector< ofPolyline > mode2strings;
    
    float numStuff;// temporary input for reading the osc input
    string textStuff;//the temporary holder for reading the osc input
    vector< string > splitter; // used to cycle words per tweet
    vector< string > lister; //the big list of tweets
    vector< string > sifttTweets;//used to store the siftt Tweets
    vector< vector < float > > sifttTweetXPosition; //used to store the siftt tweets positions
    vector< float > siftRandomGen;//randomly move the collision letters
    
    vector< ofVec2f > barrierImpactPoints;
    vector< string > barrierImpactLetters;
    vector< ofVec2f > barrierImpactVelocities;
    
    
    //stuff for the collsion management system which is not enabled right now
    int Acounter;
    int Aprevious;
    int Acollisionradius;
    
    //get the panValue from node JS
    float panValue;
    
    // how long the tweet particles live
    float lifeMultiplier = 1;
    
    ofTrueTypeFont	verdana10; // font
    ofTrueTypeFont	verdana20; // font
    ofTrueTypeFont	verdana30; // font
    ofTrueTypeFont	verdana80; // font
    
    vector <ofImage> images;
    vector <ofImage> barrier;
    vector <ofImage> explosion;
    vector <ofImage> background;
    vector <ofImage> siftt_score_w;
    vector <ofImage> siftt_score_q;
    vector <ofImage> siftt_score_k;
    vector <ofImage> siftt_score_i;
    vector <ofImage> siftt_score_b;
    vector <ofImage> siftt_score_p;
    vector <ofImage> siftt_score_l;
    vector <ofImage> siftt_score_x;
    vector <ofImage> siftt_score_z;
    
    
    
    ///////////////////////////
    ofxBox2d      box2d;
    ofRectangle boundsA;
    ofPolyline                              groundLine;
	vector < shared_ptr<ofxBox2dEdge> >     grounds;
    ofMesh textMesh;
    
    vector < shared_ptr<CustomParticle> > sifttLetters;
    ofxBox2dPolygon polyEdge;
    ofxBox2dCircle testRect;
    //std::vector  <CustomParticle*> circles;
    vector		<shared_ptr<CustomParticle> >	circles;
    
    vector<ofPolyline> polylines;
    
    vector <shared_ptr<CustomParticle> > particles;
    
    vector <shared_ptr< scoreDraw > > scoreGestures;
    
    string previousViewSiftt;
    string currentViewSiftt;
    bool mappingSwitch;
    
    //this stores the mode of the piece
    int mainMode;
    
private:
    ofxMtlMapping2D* _mapping;

};


