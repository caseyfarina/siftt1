#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup(){
    
    start == 0;
    
    //divide the screen into three thirds so it does not need to be computed every time
    firstThird = ofGetWidth()*.33;
    secondThird = ofGetWidth()*.7;
    
    dividerMode2 = ofGetWidth()*.7;
    mappingSwitch = false;
    
    ofBackground(50);
    
    _mapping = new ofxMtlMapping2D();
    _mapping->init(ofGetWidth(), ofGetHeight(), "mapping/xml/shapes.xml", "mapping/controls/mapping.xml");
    
    
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);// set up to receive on a port
    sender.setup(HOST, PORT);
    
    receiverS.setup(PORT1);
    senderS.setup(HOST1, PORT1);
    
    receiverSIFTT.setup(PORTSIFTT);
    senderSIFTT.setup(HOSTSIFTT, PORTSIFTT);
    
	current_msg_string = 0;
    mouseX = 0;
	mouseY = 0;
	mouseButtonState = "";
    
	ofBackground(0, 0, 0);
    
    stringLength = 0;// intialize the variable to store the string length
    
    ofTrueTypeFont::setGlobalDpi(72);// to make sure the fonts are correct size
    
    verdana30.load("Geometos.ttf", 30, true, true);// Initialize font
    verdana30.setLineHeight(60);
    verdana30.setLetterSpacing(1.035);
    
    verdana20.load("Geometos.ttf", 20, true, true);// Initialize font
	verdana20.setLineHeight(60);
	verdana20.setLetterSpacing(1.035);
    
    verdana10.load("Geometos.ttf", 10, true, true);// Initialize font
	verdana10.setLineHeight(60);
	verdana10.setLetterSpacing(1.035);
    
    verdana80.load("Geometos.ttf", 80, true, true);// Initialize font
	verdana80.setLineHeight(60);
	verdana80.setLetterSpacing(1.035);
    
    //functionn where I load all the image sequences
    
    loadImageSequences();
    
    
    //initializing the box2D world
    boundsA.set(firstThird, 0, firstThird, ofGetHeight());
    
    box2d.init();
	box2d.setGravity(0, 3); // set the box2D gravity
	box2d.setFPS(60.0);
    box2d.enableEvents();
	//box2d.registerGrabbing();
    box2d.createBounds(boundsA);
    
    ofAddListener(box2d.contactStartEvents, this, &ofApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &ofApp::contactEnd);

   
    
    //set UP the collision circles for the Siftt tweets
    for(int i = 0;i < 18;i++){
        shared_ptr<CustomParticle> circ = shared_ptr<CustomParticle>(new CustomParticle);
        circ.get()->setup(box2d.getWorld(), 0, 0, 50);
        circ.get()->isFixed();
        circ.get()->setupTheCustomData(" ",1);
        
        siftRandomGen.push_back(ofRandom(10));
        sifttLetters.push_back(circ);
    }
    
    sifttTweets.push_back("I'm tweeting to #siftt");
    sifttTweets.push_back("Play thing thing for me What is going on?");
    sifttTweets.push_back("What is going on here? #siftt What is going on here? What is going on here?");
    sifttTweets.push_back("Are you Kidding me? #siftt No way this is crazy #siftt");
    sifttTweets.push_back("No way this is crazy #siftt");
    sifttTweets.push_back("Is this thing on? #siftt");
    sifttTweets.push_back("Huh #siftt");
    sifttTweets.push_back("THis    IS HAppening #siftt");
    for(int i = 0; i < sifttTweets.size(); i++){
        vector < float > tempFloatVector;
        tempFloatVector.push_back(ofGetWidth()+100);
        
        sifttTweetXPosition.push_back(tempFloatVector);
    }
    ofSetFrameRate(24);
    
    
   
    
    
}

//UPDATE--------------------------------------------------------
void ofApp::update(){
    if(start > 0){

   
    //This is mode one
    if(mainMode == 1){
        
    //"P" turns the system on and "O" turns it off. TODO create a fade in of some kind
        
        //"I" turns on the projection mapping. May be unncessary depending on the scenario
        
        if (mappingSwitch) {
            _mapping->update();

        }
        
    
    
    //receiverSIFTT.
    while(receiverSIFTT.hasWaitingMessages()){
        ofxOscMessage mm;
        receiverSIFTT.getNextMessage(&mm);
        
        if(mm.getAddress() == "/siftt"){
			//get the new message and put it in the string "textStuff
            textStuff = mm.getArgAsString(0);
            cout << textStuff;
        sifttTweets.push_back(textStuff);
        }
    }
    
	// check for waiting messages
    //i had to comment this out because when the number of messages starts to exceed one per frame, everything start to grind to a halt.
	//while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        // grab the /pan information to control the tweet from left to right
        
        if(m.getAddress() == "/pan"){
            
            //get the pan Value
            float tempPan = m.getArgAsFloat(0);
            
            //Map the pan value to tweet placement
            panValue = ofMap(tempPan,0.0,1.0,firstThird,secondThird);
			
		}
        
        // check for New Tweet
		if(m.getAddress() == "/text" || lister.size() < 5){
			//get the new message and put it in the string "textStuff
            textStuff = m.getArgAsString(0);
            
            
            /*
            //search the tweet for SIFTT
            if(ofIsStringInString(textStuff, "mac")){
                sifttTweets.push_back(textStuff);
            }else{
             */
            
            // If the tweet does not contain SIFTT
            lister.push_back(textStuff);
                
            // Make the tweets that are exactly 140 characters much bigger than all others
            float r;
            if (textStuff.size() == 140){
                r = ofRandom(40,60);
            }else{
                r = ofMap(ofClamp(textStuff.size(),0,140), 0, 139, 0, 12);
            }
                
            // create a bubble
            //float r = ofRandom(5, 15);		// a random radius 4px - 20px
            
            shared_ptr<CustomParticle> p = shared_ptr<CustomParticle>(new CustomParticle);
            p.get()->setPhysics(1.0, ofRandom(1.0), 0.3);
            p.get()->setup(box2d.getWorld(), panValue, 2, r); //use the pan value to spread the trending tweets across the top middle of the screen
            p.get()->setVelocity(ofRandom(-5, 5), ofRandom(-50, 5));
            p.get()->setRotation(ofRandom(0,360));
            
            
            p.get()->setupTheCustomData(textStuff,0);
            
            circles.push_back(p);
            
		}
        
        if(m.getAddress() == "/num"){
            //splitter.clear();
			// the single argument is a string
            
			numberInput = m.getArgAsFloat(2);//get the new message and put it in the string "textStuff
            
		}
        
        
	
    // this limits the size of the entire list of tweets to 40
    if( lister.size() > 40){
        lister.erase(lister.begin());
    }
    
    // this limits the size of the entire list of siftt tweets
    if( sifttTweets.size() > 6){
        sifttTweets.erase(sifttTweets.begin());
    }
    
    //update the circles
    for(int i=0; i<circles.size(); i++) {
		circles[i].get()->update();
    }
    
    
    //get Rid of old tweets circles by getting too old or pasing the barrier
    vector <shared_ptr<CustomParticle> > ::iterator iter = circles.begin();
	while (iter != circles.end()) {
        
        //grab the age of the circle
        int ageTemp;
        ageTemp = iter->get()->getAge();
        
        //grab the x position of the circle
        ofVec2f ypos;
        ypos = iter->get()->getPosition();
        
        
        int tempLife = iter->get()->getlifeTime();
        //set the position of the barrier
        int barrierYpos = ofGetHeight()*.4;
        if(ageTemp > (tempLife*lifeMultiplier)){
            iter->get()->destroy();
            circles.erase(iter);
            
        }else if(ypos[1] > barrierYpos) {
            
            //Grab the impact points for the particles that hit the barrier
            barrierImpactVelocities.push_back(iter->get()->getVelocity());
            
            barrierImpactPoints.push_back(iter->get()->getPosition());
            barrierImpactLetters.push_back(iter->get()->getLetter());
            
            
            //DESTROY THE CIRCLES THAT hit the barrier or get too old
            iter->get()->destroy();
            circles.erase(iter);
        }
		else ++iter;
    }
    
    
    // this limits the size of the entire list of siftt tweets
    if( barrierImpactPoints.size() > 10){
        barrierImpactPoints.erase(barrierImpactPoints.begin());
    }

    
    
    for (int i = 0; i < barrierImpactPoints.size(); i++) {
        
        ofxOscMessage m;
        m.setAddress("/boundaryTransition");
        std::string s = barrierImpactLetters[i];
        float tempPitch = ofMap(s[0],0,128,0.0,1.0);
        m.addFloatArg(tempPitch);
        
        senderS.sendMessage(m, false);
        
        ofxOscMessage h;
        h.setAddress("/boundaryPan");
        float tempPan = ofMap(barrierImpactPoints[i].y,0,800,0.0,1.0);
        h.addFloatArg(tempPan);
        
        senderS.sendMessage(h, false);
        
        
        
        shared_ptr<scoreDraw> tempGesture =shared_ptr<scoreDraw>(new scoreDraw);
        senderS.sendMessage(m, false);
        ofVec2f tempVelocity = barrierImpactVelocities[i];
        float tempXspeed =abs(ofClamp(tempVelocity.x,-60,60));
        tempXspeed = ofMap(tempXspeed, 0, 60, 0.3, 5);
        tempGesture.get()->setup(barrierImpactPoints[i],barrierImpactLetters[i],tempXspeed);
        tempGesture.get()->update();
        scoreGestures.push_back(tempGesture);
         
       
        
        
    }
    
    barrierImpactVelocities.clear();
    barrierImpactLetters.clear();
    barrierImpactPoints.clear();

    
    box2d.update();
    
    
    }
    
    else if(mainMode == 2){
        
            
            //"I" turns on the projection mapping. May be unncessary depending on the scenario
            
            if (mappingSwitch) {
                _mapping->update();
                
            }
        
        //setup the strings for mode 2
        mode2strings.clear();
        for (int i = 0; i < mode2stringLocations.size(); i++ ) {
            ofPolyline tempLine;
            tempLine.addVertex(mode2stringLocations[i]);
            tempLine.addVertex(dividerMode2,mode2stringLocations[i][1]);
            mode2strings.push_back(tempLine);
            
        }
            
            
            //receiverSIFTT.
            while(receiverSIFTT.hasWaitingMessages()){
                ofxOscMessage mm;
                receiverSIFTT.getNextMessage(&mm);
                
                if(mm.getAddress() == "/siftt"){
                    //get the new message and put it in the string "textStuff
                    textStuff = mm.getArgAsString(0);
                    cout << textStuff;
                    sifttTweets.push_back(textStuff);
                }
            }
            
            // check for waiting messages
            //i had to comment this out because when the number of messages starts to exceed one per frame, everything start to grind to a halt.
            //while(receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            
            // grab the /pan information to control the tweet from left to right
            
            if(m.getAddress() == "/pan"){
                
                //get the pan Value
                float tempPan = m.getArgAsFloat(0);
                
                //Map the pan value to tweet placement
                panValue = ofMap(tempPan,0.0,1.0,0,dividerMode2);
                
            }
            
            // check for New Tweet
            if(m.getAddress() == "/text" || lister.size() < 5){
                //get the new message and put it in the string "textStuff
                textStuff = m.getArgAsString(0);
                
                // If the tweet does not contain SIFTT
                lister.push_back(textStuff);
                
                // Make the tweets that are exactly 140 characters much bigger than all others
                float r;
                if (textStuff.size() == 140){
                    r = ofRandom(40,60);
                }else{
                    r = ofMap(ofClamp(textStuff.size(),0,140), 0, 139, 0, 12);
                }
                
                shared_ptr<CustomParticle> p = shared_ptr<CustomParticle>(new CustomParticle);
                p.get()->setPhysics(1.0, ofRandom(1.0), 0.3);
                
                //use the pan value to spread the trending tweets across the bottom
                p.get()->setup(box2d.getWorld(), panValue, ofGetHeight()-4, r);
                p.get()->setVelocity(ofRandom(-1, 1), ofRandom(-4, 0));
                p.get()->setRotation(ofRandom(-360,360));
                p.get()->setupTheCustomData(textStuff,0);
                circles.push_back(p);
            }
            
            if(m.getAddress() == "/num"){
                //splitter.clear();
                // the single argument is a string
                
                numberInput = m.getArgAsFloat(2);//get the new message and put it in the string "textStuff
                
            }
            
            
            
            // this limits the size of the entire list of tweets to 40
            if( lister.size() > 40){
                lister.erase(lister.begin());
            }
            
            // this limits the size of the entire list of siftt tweets
            if( sifttTweets.size() > 6){
                sifttTweets.erase(sifttTweets.begin());
            }
            
            //update the circles
            for(int i=0; i<circles.size(); i++) {
                circles[i].get()->update();
            }
            
            
            //get Rid of old tweets circles by getting too old or pasing the barrier
            vector <shared_ptr<CustomParticle> > ::iterator iter = circles.begin();
            while (iter != circles.end()) {
                
                //grab the age of the circle
                int ageTemp;
                ageTemp = iter->get()->getAge();
                
                //grab the x position of the circle
                ofVec2f ypos;
                ypos = iter->get()->getPosition();
                
                
                int tempLife = iter->get()->getlifeTime();
                //set the position of the barrier
                int barrierYpos = 20;
                if(ageTemp > (tempLife*lifeMultiplier)){
                    iter->get()->destroy();
                    circles.erase(iter);
                    
                }else if(ypos[1] < barrierYpos) {
                    
                    //Grab the impact points for the particles that hit the barrier
                    //barrierImpactVelocities.push_back(iter->get()->getVelocity());
                    
                    //barrierImpactPoints.push_back(iter->get()->getPosition());
                    //barrierImpactLetters.push_back(iter->get()->getLetter());
                    
                    
                    //DESTROY THE CIRCLES THAT hit the barrier or get too old
                    iter->get()->destroy();
                    circles.erase(iter);
                }
                else ++iter;
            }
            
            /*
             Aprevious = Acounter;
             
             int AcollisionPerFrame = Acounter - Aprevious;
             
             if(AcollisionPerFrame){
             int AaverageRadius = Acollisionradius/AcollisionPerFrame;
             if(AcollisionPerFrame > 4){
             for(int i = 0;i < 4; i++){
             int velocity = ofMap(AaverageRadius,0,100,0,127);
             note = (int)ofRandom(0,127);
             midiOut.sendNoteOn(1, note,  velocity);
             }
             }else{
             for(int i = 0;i < AcollisionPerFrame; i++){
             int velocity = ofMap(AaverageRadius,0,100,0,127);
             note = (int)ofRandom(0,127);
             midiOut.sendNoteOn(1, note,  velocity);
             
             }
             
             }
             
             }
             
             Acollisionradius = 0;
             */
            
            
            // this limits the size of the entire list of siftt tweets
            if( barrierImpactPoints.size() > 10){
                barrierImpactPoints.erase(barrierImpactPoints.begin());
            }
            
            
            
            for (int i = 0; i < barrierImpactPoints.size(); i++) {
                
                ofxOscMessage m;
                m.setAddress("/boundaryTransition");
                std::string s = barrierImpactLetters[i];
                float tempPitch = ofMap(s[0],0,128,0.0,1.0);
                m.addFloatArg(tempPitch);
                
                senderS.sendMessage(m, false);
                
                ofxOscMessage h;
                h.setAddress("/boundaryPan");
                float tempPan = ofMap(barrierImpactPoints[i].y,0,800,0.0,1.0);
                h.addFloatArg(tempPan);
                
                senderS.sendMessage(h, false);
                
                
                
            }
            
            barrierImpactVelocities.clear();
            barrierImpactLetters.clear();
            barrierImpactPoints.clear();
            
            
            box2d.update();
            
            
        

        
    }
    
    else if(mainMode == 3){
        
    }
    
}
}




//DRAW----------------------------------------------------------
void ofApp::draw(){

    if (mappingSwitch) {
        _mapping->bind();
    }
    
    if(start >0){
    
        
        
        if(mainMode == 1){
            
            ofPushMatrix();
            //Code for zooming in, not implimenting right now
            //ofScale(float(ofGetMouseX())/(ofGetWidth()/2), float(ofGetMouseX())/(ofGetWidth()/2));
            
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofFill();
    ofSetColor(255,200);
        

    
    testRect.draw();
    ofSetColor(255);
    ofSetLineWidth(3);
        
        //Draw some lines to see the Grid
        //ofDrawLine(firstThird, 0, firstThird, ofGetHeight());
        ofDrawLine(secondThird, 0, secondThird, ofGetHeight());
    

    //Draw the Barrier Video
    ofPushMatrix();
    ofTranslate(firstThird, ofGetHeight()*.4);
    //fix the size of the barrier video
    ofScale(.85, 1);
    //ofRotate(-90);
    //barrier[ofGetFrameNum()%barrier.size()].draw(0,0,800,40);
    //barrierVid.draw(0, 0, 800, 40);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(1000, ofGetHeight());
    //ofRotate(-90);
    background[ofGetFrameNum()%background.size()].draw(0,0,800,200);
    //barrierVid.draw(0, 0, 800, 40);
    ofPopMatrix();
        
    
    
  
        
    //DRAW THE GRPHIC SCORE!!!!!_______________________________________
    for(int i = 0; i < scoreGestures.size();i++){
        
        
        scoreGestures[i].get()->update();
        
        //draw letters on impact
        if (scoreGestures[i].get()->timer < 90) {
            ofSetColor(255,255 - (scoreGestures[i].get()->timer*10));
            ofPushMatrix();
            ofTranslate(scoreGestures[i].get()->originalX, scoreGestures[i].get()->y);
            ofScale(scoreGestures[i].get()->timer*.2,scoreGestures[i].get()->timer*.2);
            
            verdana80.drawString(scoreGestures[i].get()->letter,0,0 );
            ofPopMatrix();
            //explosion[scoreGestures[i].get()->timer].draw((scoreGestures[i].get()->originalX)-250, (scoreGestures[i].get()->y)-250                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             ,500,500);
            ofPushStyle();
            ofNoFill();
            //ofSetLineWidth(scoreGestures[i].get()->timer*0.3);
            ofSetCircleResolution(200);
            int tempRad = scoreGestures[i].get()->timer*3;
            ofDrawCircle(scoreGestures[i].get()->originalX, scoreGestures[i].get()->y, tempRad);
            ofPopStyle();
        }
        
        if (scoreGestures[i].get()->getDrawMode()) {
            scoreGestures[i].get()->draw();
        }else{
            string tempLetter = scoreGestures[i].get()->getLetter();
            
            if(tempLetter == "w" || tempLetter =="W"){
                scoreDrawer(i, siftt_score_w);
            }
            else if(tempLetter == "q" || tempLetter =="Q"){
                scoreDrawer(i, siftt_score_q);
            }
            else if(tempLetter == "p" || tempLetter =="P"){
                scoreDrawer(i, siftt_score_p);
            }
            else if(tempLetter == "b" || tempLetter =="B"){
                scoreDrawer(i, siftt_score_b);
            }
            else if(tempLetter == "x" || tempLetter =="X"){
                scoreDrawer(i, siftt_score_x);
            }
            else if(tempLetter == "z" || tempLetter =="Z"){
                scoreDrawer(i, siftt_score_z);
            }
            else if(tempLetter == "l" || tempLetter =="L"){
                scoreDrawer(i, siftt_score_l);
            }
            else if(tempLetter == "k" || tempLetter =="K"){
                scoreDrawer(i, siftt_score_k);
            }else if(tempLetter == "i" || tempLetter =="I"){
                scoreDrawer(i, siftt_score_i);
            }
            
            

        //scoreGestures[i].get()->draw();
        if(scoreGestures[i].get()->timer > 500){
            scoreGestures.erase(scoreGestures.begin()+i);
        }
    }
    }
    
    
    
    
    
    
    // set the color etc.
    ofFill();
    
   
    //verdana80.drawString("hello!", 0,0);
    
    // timing of how often a new word is displayed in the list
    if(ofRandom(10)>9.0){
        currentCount += 1;
    }
    
    /*
    //Draw the trending tweets list in the background of the top
    for(int i = 0; i < lister.size();i++){
        splitter.clear();
        // the single argument is a string
        
        //textStuff = m.getArgAsString(0);//get the new message and put it in the string "textStuff
        if(i%2 ==0){
        ofSetColor(255,50);
        }else{
        ofSetColor(255,80);
        }
        splitter = ofSplitString(lister[i], " ",TRUE);//Spl
        stringLength = splitter.size();
        if(stringLength > 0){
            int currentWord = currentCount%stringLength;
            verdana20.drawString(splitter[currentWord], 10, (i*20));
        }
    }
     */
    
    //DRAW THE SIFTT TWEETS ON THE RIGHT SIDE
    for(int i = 0; i < sifttTweets.size();i++){
        splitter.clear();
        // the single argument is a string
        
        //textStuff = m.getArgAsString(0);//get the new message and put it in the string "textStuff
        if(i%2 ==0){
            ofSetColor(255,100);
        }else{
            ofSetColor(255,130);
        }
        splitter = ofSplitString(sifttTweets[i], " ",TRUE);//Spl
        stringLength = splitter.size();
        
        
        //this is all to move the sifttTweets back and forth
        sifttTweetXPosition[i].push_back(secondThird + (i*80) + 100);
        if (sifttTweetXPosition[i].size() > 30) {
            sifttTweetXPosition[i].erase(sifttTweetXPosition[i].begin());
        }
        float tempAccumulator = 0;
        for (int j = 0; j < sifttTweetXPosition[i].size(); j++) {
            tempAccumulator += sifttTweetXPosition[i][j];
        }
        
        float tempXpositon = tempAccumulator/sifttTweetXPosition[i].size();
        
        
        if(stringLength > 0){
            
            int currentWord = currentCount%stringLength;
            ofPushMatrix();
            int nudgeText = 30; //Compensate for the height of the Font
            ofTranslate(tempXpositon, ofGetHeight()-10);
            ofRotate(-90);
            ofScale(1.1,1.1);
            int scrollerSmall = sifttTweets[i].size()-((ofGetFrameNum()/(sifttTweets[i].size()/3))%(sifttTweets[i].size()));
            verdana80.drawString(sifttTweets[i].substr(scrollerSmall,50),0,0);
            //verdana80.drawString(splitter[currentWord], 0, 0);
            ofPopMatrix();
            
        }
    }
    
        
    if(ofGetFrameNum()%100 == 0 ){
        int randomSize = sifttTweets.size();
        
        currentViewSiftt = sifttTweets[int(ofRandom(0,randomSize))].substr(0,15);
        //randomly shuffle the lift of audience tweets
        std::random_shuffle ( sifttTweets.begin(), sifttTweets.end() );
        std::random_shuffle ( sifttTweetXPosition.begin(), sifttTweetXPosition.end() );

    }
    
    
    //send the new siftt tweets that are onscreen over OSC
    if(previousViewSiftt != currentViewSiftt){
        ofxOscMessage m;
        m.setAddress("/siftt");
        
        //m.addStringArg(currentViewSiftt);
        m.addFloatArg(ofRandom(40,100));
        senderS.sendMessage(m, false);
    }
    
    
    cout << currentViewSiftt;
    //this draws the Siftttweets and creates the circles that will filter the results
    //string testo = "!THISISTHETEST!";
    if(currentViewSiftt.length()>0){
        /*
        string drawingSifttTweets;
        if(ofGetFrameNum()%120 == 0){
            sifttViewCounter++;
            int currentSiftCount = sifttViewCounter%3;
            int stringstart = ofClamp(currentSiftCount*14,0,currentViewSiftt.size());
            int stringend = ofClamp(stringstart+15,0,currentViewSiftt.size());
            
            drawingSifttTweets = currentViewSiftt[stringstart,stringend];
        }
        cout << drawingSifttTweets;
        */
        
        for(int i = 0;i < currentViewSiftt.length()/5;i++){
        for(int j = 0; j < 5;j++){
        ofSetColor(255,200);
        ofPushMatrix();
        float noiseScale = 100;
            
            float xNoise;
            float yNoise;
            float yposition;
            float xposition;
            
        if(siftRandomGen[j]>5){
            xNoise = noiseScale * ofSignedNoise((ofGetElapsedTimeMillis()*0.000057657)+(i*1000)+(j*3000));
            xposition = (j*100)+firstThird+xNoise;
            yposition = ((i*80)+100);
            
        }else{
        
            yNoise = noiseScale * ofSignedNoise((ofGetElapsedTimeMillis()*0.000087657)+(i*9000)+(j*6000));
            xposition = (j*100)+firstThird;
            yposition = ((i*80)+100+yNoise);
        }
        
            
        
        ofTranslate(xposition,yposition);
        //ofRotate(-90);
        string letter = ofToString(currentViewSiftt[(i*5)+(j)]);
        verdana80.drawString(letter,0,0);
        
        
        
        
        ofRectangle rect = verdana80.getStringBoundingBox(letter,xposition,yposition);
        ofVec2f center = rect.getCenter();
        int textWidth = rect.getWidth();
        
        ofPushStyle();
        //ofNoFill();
        ofSetColor(255, 30);
        //ofSetLineWidth(5);
            if(textWidth > 2){
                ofDrawCircle(30, -25, ofGetFrameNum()%((textWidth/2)+1));
            }
        ofPopStyle();
        ofPopMatrix();
        //testRect.setup(box2d.getWorld(), center.x-40,center.y,textWidth/2);
        sifttLetters[(i*5)+(j)].get()->setRadius(textWidth *0.8);
        sifttLetters[(i*5)+(j)].get()->setPosition(center.x, center.y);
        //sifttLetters[(i*5)+(5-j)].get()->
        //sifttLetters[(i*5)+(5-j)].get()->addRepulsionForce(center.x-80, center.y, 500);(i*5)+(j)
        //sifttLetters[(i*5)+(5-j)].get()->setPosition((i*200)+100, (j*(800/6)));
        //sifttLetters[(i*5)+(5-j)].get()->draw();
        }
    }
    }
    previousViewSiftt = currentViewSiftt;
    ofSetColor(255, 0, 0);
    ofPushMatrix();
    
    
    
    
   
    
    
    //ofSort(circles,sortMe);
        
        
    //Main drawing of all box2D tweets
    
    for(int i=0; i<circles.size(); i++) {
		//ofFill();
		//ofSetHexColor(0x90d4e3);
        
        // testing out a repulsion force
        //circles[i].get()->addRepulsionForce(30, ofGetHeight()/2, .1);
        
        ofVec2f position;
        float radiusTemp;
        radiusTemp = circles[i].get()->getRadius();
        
        //find out how close that tweet is to the barrier
        int closeToBarrier = ofMap(circles[i].get()->getPosition().y, 0, ofGetHeight()*.4, 50, 255);
        ofSetColor(closeToBarrier);
        
        
        //ALL THIS DRAWS THE TRENDING TWEETS ON THE LEFT SIDE AND THE LINES FROM THE TWEETS TO THE TWEET PARTICLES
        //Get the vertical position of that component of the list
        int verticalPositionList = i*22;
        
        //if it is not that close to the barrier
        if(closeToBarrier < 130){
            
            int scrollerSmall = circles[i].get()->getTweet().size()-((ofGetFrameNum()/4)%(circles[i].get()->getTweet().size()));
            verdana20.drawString(circles[i].get()->getTweet().substr(scrollerSmall,22),4,verticalPositionList);
            //ofRectangle rect = verdana20.getStringBoundingBox(circles[i].get()->getTweet().substr(scrollerSmall,22), 4, verticalPositionList);
            ofSetColor(200,200);
            //ofDrawRectangle(rect.x, rect.y, rect.width, rect.height*(ofMap(closeToBarrier, 0, 255, 0, 1)));
        }else{
            
            int scroller = (ofGetFrameNum()/2)%(circles[i].get()->getTweet().size());
            string tempStringthing = circles[i].get()->getTweet().substr(scroller,17);
            verdana30.drawString(tempStringthing,4,verticalPositionList);
            
            ofSetColor(closeToBarrier + 30);
            ofDrawCircle(firstThird-60,verticalPositionList-2,5,5);
            
            ofSetLineWidth(2);
            ofSetColor(closeToBarrier);
            ofPolyline thisline;
            thisline.curveTo(firstThird-60,verticalPositionList);
            thisline.curveTo(firstThird-60,verticalPositionList);
            //thisline.curveTo(firstThird-20, verticalPositionList);
            thisline.curveTo(firstThird+(i/(circles.size()+1)*300), circles[i].get()->getPosition().y);
            thisline.curveTo(circles[i].get()->getPosition().x-10, circles[i].get()->getPosition().y);
            thisline.curveTo(circles[i].get()->getPosition().x-10, circles[i].get()->getPosition().y);
            thisline.draw();
            thisline.clear();
            //ofDrawLine(firstThird-10,i*22,circles[i].get()->getPosition().x,circles[i].get()->getPosition().y);
            //verdana30.drawString(circles[i].get()->getTweet().substr(31,60),4,i*12+30);
            //verdana30.drawString(circles[i].get()->getTweet().substr(61,90),4,i*12+60);
            //verdana30.drawString(circles[i].get()->getTweet().substr(90,139),4,i*12+90);
        }
        
		//ofDrawEllipse(circles[i].get()->getPosition(),radiusTemp*2,radiusTemp*2);
        ofPushMatrix();
        ofTranslate(circles[i].get()->getPosition());
        
        
        if(circles[i].get()->getAge() < 5){
            int colorScale = 200 - circles[i].get()->getAge()*30;
            ofSetColor(255,colorScale);
            ofDrawCircle(0, 0, circles[i].get()->getAge()*circles[i].get()->getRadius());
        }
        //scale the text size based on the particle size
        float textSize = radiusTemp/10;
        
        
        //display the tweet word before and roation
        ofSetColor(255,100);
        //ofRotate(-90);
        
        //update which word occasionally
        if(ofRandom(10) > 9.0){
            circles[i].get()->incrementCurrentWord();
        }
        
        //draw the word
        verdana10.drawString(circles[i].get()->getWord(),-20,40);
        ofRotate(90);
        
        // now rotate the particle
        ofRotate(circles[i].get()->getRotation());
        ofSetColor(255,200);
        
        if(ofRandom(10)>6.0){
            circles[i].get()->incrementLetter();
        }
        
        ofSetColor(closeToBarrier);
        verdana20.drawString(circles[i].get()->getLetter(),-8,10);
        
        
        ofScale(2,2);
        //map the y position onto the image display for now
        int currentFrameY = ofClamp(ofMap(circles[i].get()->getPosition().y, 0, ofGetHeight()*.4, 0, 175),0,179);
        images[currentFrameY].draw(-radiusTemp,-radiusTemp,radiusTemp*2,radiusTemp*2);
        
        if(closeToBarrier > 100){
            //map the x position onto the next layer
            ofSetColor(255,100);
            int currentFrameX = ofClamp(ofMap(circles[i].get()->getPosition().x, firstThird, secondThird, 0, 180),0,179);
            images[currentFrameX].draw(-radiusTemp*2,-radiusTemp*2,radiusTemp*4,radiusTemp*4);
        }
        
        if(closeToBarrier>150){
            //map the Age ONTO THE third layer
            ofSetColor(255,80);
            //int clampSize = ofClamp(circles[i].get()->getWord().size(), 0, 50);
            int letterHud = ofMap(circles[i].get()->getAge(),0,circles[i].get()->getlifeTime()*lifeMultiplier,0,180);
            images[letterHud].draw(-radiusTemp*3,-radiusTemp*3,radiusTemp*6,radiusTemp*6);
        
            ofSetColor(255,10);
            ofDrawEllipse(0,0, radiusTemp*2,radiusTemp*2);
        }
        ofPopMatrix();
        
	}
            
    

    ofDisableAlphaBlending();
            ofPopMatrix();
            
    
    }
    
    
        else if(mainMode == 2){
            ofEnableAlphaBlending();
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            ofFill();
            ofSetColor(255,200);
            
            
            //A rough draft of the physical simulation strings
        
            for(int i = 0; i < mode2strings.size();i++){
                mode2strings[i].draw();
            }
            
            //Main drawing of all box2D tweets
            
            for(int i=0; i<circles.size(); i++) {
                //ofFill();
                //ofSetHexColor(0x90d4e3);
                
                // testing out a repulsion force
                //circles[i].get()->addRepulsionForce(30, ofGetHeight()/2, .1);
                
                ofVec2f position;
                float radiusTemp;
                radiusTemp = circles[i].get()->getRadius();
                
                //ofDrawEllipse(circles[i].get()->getPosition(),radiusTemp*2,radiusTemp*2);
                ofPushMatrix();
                ofTranslate(circles[i].get()->getPosition());
                
                //scale the text size based on the particle size
                float textSize = radiusTemp/10;
                
                //display the tweet word before and roation
                ofSetColor(255,100);
                ofRotate(-90);
                
                //update which word occasionally
                if(ofRandom(10) > 9.0){
                    circles[i].get()->incrementCurrentWord();
                }
                
                //draw the word
                verdana10.drawString(circles[i].get()->getWord(),-20,40);
                ofRotate(90);
                
                // now rotate the particle
                ofRotate(circles[i].get()->getRotation());
                ofSetColor(255);
                
                if(ofRandom(10)>6.0){
                    circles[i].get()->incrementLetter();
                }
                
                verdana20.drawString(circles[i].get()->getLetter(),-8,10);
                
                
                ofScale(2,2);
                //map the y position onto the image display for now
                int currentFrameY = ofClamp(ofMap(circles[i].get()->getPosition().y, 0, 800, 0, 175),0,179);
                images[currentFrameY].draw(-radiusTemp,-radiusTemp,radiusTemp*2,radiusTemp*2);
                
                //map the x position onto the next layer
                ofSetColor(255,100);
                int currentFrameX = ofClamp(ofMap(circles[i].get()->getPosition().x, 0, 1280, 0, 180),0,179);
                images[currentFrameX].draw(-radiusTemp*2,-radiusTemp*2,radiusTemp*4,radiusTemp*4);
                
                //map the word size onto the third layer
                ofSetColor(255,40);
                int clampSize = ofClamp(circles[i].get()->getWord().size(), 0, 50);
                int letterHud = ofMap(clampSize,0,50,0,180);
                images[letterHud].draw(-radiusTemp*3,-radiusTemp*3,radiusTemp*6,radiusTemp*6);
                
                ofSetColor(255,10);
                ofDrawEllipse(0,0, radiusTemp*2,radiusTemp*2);
                
                ofPopMatrix();
                
            }
            
            
             //Draw the trending tweets list in the background of the top
             for(int i = 0; i < lister.size();i++){
                 splitter.clear();
                 // the single argument is a string
             
                 //textStuff = m.getArgAsString(0);//get the new message and put it in the string "textStuff
             if(i%2 ==0){
                 ofSetColor(255,50);
             }else{
                 ofSetColor(255,80);
             }
             splitter = ofSplitString(lister[i], " ",TRUE);//Spl
             stringLength = splitter.size();
             if(stringLength > 0){
             int currentWord = currentCount%stringLength;
             verdana20.drawString(splitter[currentWord], dividerMode2, (i*20));
             }
             }
            
            
            //send pluck over osc
            
            for(int i = 0;i < circles.size();i++){
                ofPoint tempPoint = circles[i].get()->getPosition();
                for(int j = 0; j < mode2stringLocations.size();j++){
                    if(tempPoint[1] < mode2stringLocations[j][1]+2 || tempPoint[1] > mode2stringLocations[j][1]-2){
                        ofxOscMessage m;
                        m.setAddress("/mode2string");
                        float tempStringHeight = mode2stringLocations[j][1];
                        m.addFloatArg(tempStringHeight);
                        //cout << tempStringHeight << "stringHeight";
                        
                        float tempStringPluckLocation = tempPoint[0];
                        m.addFloatArg(tempStringPluckLocation);
                        //cout << tempStringPluckLocation << "stringPluckLocation";
                        
                        senderS.sendMessage(m, false);
                        
                        
                    }
                }
                
                

                
            }
            
            
            ofDisableAlphaBlending();
            
            }
    
    
    
        else if(mainMode == 3){
        
        }
    
    }
    if (mappingSwitch) {
        _mapping->unbind();
        //-------- mapping of the towers/shapes
        _mapping->draw();
    }


}

void ofApp::contactStart(ofxBox2dContactArgs &e) {
    //int noteLimiter = 0;
    if(e.a != NULL && e.b != NULL) {
        
        if((e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) /*|| e.a->GetType() == b2Shape::e_edge*/) {
            
            Acounter++;
            /*
            cout << "Hello World!\n";
            ofxOscMessage m;
            m.setAddress("/collide");
            m.addIntArg(1);
            sender.sendMessage(m, false);
             */
            //  SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
			//SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
            
            Data *DataA = (Data*)e.a->GetBody()->GetUserData();
            Data *DataB = (Data*)e.b->GetBody()->GetUserData();
            if(DataA->group != DataB->group){
                //string counter = ofToString(ofGetFrameNum());
                //cout << counter;
                ofxOscMessage m;
                m.setAddress("/collide");
                m.addIntArg(1);
                senderS.sendMessage(m, false);
                
            }

        }
        
        
    }
    
}

void ofApp::contactEnd(ofxBox2dContactArgs &e) {
    if(e.a != NULL && e.b != NULL) {
        Data *DataA = (Data*)e.a->GetBody()->GetUserData();
        Data *DataB = (Data*)e.b->GetBody()->GetUserData();
        
        if(DataA) {
            
            //DataA->final_size-=1;
            //DataA->bHitter = false;
            //DataA->hitHistory.push_back(false);
            //sound[aData->soundID].play();
        }
        
        if(DataB) {
            //DataB->final_size-=1;
            //DataB->bHitter = false;
            //DataB->hitHistory.push_back(false);
            //sound[bData->soundID].play();
        }
    }
    
}

void ofApp::loadImageSequences() {
    ofDirectory dir;
    
    int nFiles = dir.listDir("circle_test");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            images.push_back(ofImage());
            images.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("background");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            background.push_back(ofImage());
            background.back().load(filePath);
            
        }
        
    }
    
    
    
    nFiles = dir.listDir("siftt_score_w");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_w.push_back(ofImage());
            siftt_score_w.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("siftt_score_q");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_q.push_back(ofImage());
            siftt_score_q.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("siftt_score_k");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_k.push_back(ofImage());
            siftt_score_k.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("siftt_score_b");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_b.push_back(ofImage());
            siftt_score_b.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("siftt_score_p");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_p.push_back(ofImage());
            siftt_score_p.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("siftt_score_i");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_i.push_back(ofImage());
            siftt_score_i.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("siftt_score_l");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_l.push_back(ofImage());
            siftt_score_l.back().load(filePath);
            
        }
        
    }
    nFiles = dir.listDir("siftt_score_x");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_x.push_back(ofImage());
            siftt_score_x.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("siftt_score_z");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            siftt_score_z.push_back(ofImage());
            siftt_score_z.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("barrier");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            barrier.push_back(ofImage());
            barrier.back().load(filePath);
            
        }
        
    }
    
    nFiles = dir.listDir("explosion");
    if(nFiles) {
        
        for(int i=0; i<dir.size(); i++) {
            
            // add the image to the vector
            string filePath = dir.getPath(i);
            explosion.push_back(ofImage());
            explosion.back().load(filePath);
            
        }
        
    }
    
    
    
}

void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'a':
            lifeMultiplier+=+0.2;
            cout << lifeMultiplier;break;
        case 'z':
            lifeMultiplier-=0.2;
            cout << lifeMultiplier;break;
        case 'p':
            start++;break;
        case 'o':
            start--;break;
        case 'i':
            mappingSwitch= !mappingSwitch;break;
        case '1':
            mainMode = 1;
            // set the box2D gravity
            boundsA.set(firstThird, 0, firstThird, ofGetHeight());
            box2d.createBounds(boundsA);
            box2d.setGravity(0, 3);
            circles.clear();
            //sifttTweets.clear();
            lister.clear();break;
        case '2':
            mainMode = 2;
            // set the box2D gravity
            boundsA.set(0, -20, dividerMode2, ofGetHeight()+20);
            box2d.createBounds(boundsA);
            box2d.setGravity(0, -6);
            circles.clear();
            sifttTweets.clear();
            lister.clear();
            
            
            //Create five new strings to be plucked
            mode2stringLocations.clear();
            for(int i = 0; i < 5; i++){
                ofPoint tempPoint;
                tempPoint = ofPoint(0,ofRandom(20,ofGetHeight()-20));
                mode2stringLocations.push_back(tempPoint);
            }
            break;
        case '3':
            mainMode = 3;break;
        
    }
    // this is the code to test the score gestures with the keyboard
    
    shared_ptr<scoreDraw> tempGesture =shared_ptr<scoreDraw>(new scoreDraw);
    
    ofVec2f tempVelocity = ofVec2f(1,1);
    float tempXspeed =abs(ofClamp(tempVelocity.x,-60,60));
    tempXspeed = ofMap(tempXspeed, 0, 60, 0.3, 5);
    string tempStr = "";
    tempStr += char(key);
    tempGesture.get()->setup(ofVec2f(500,ofGetHeight()/2),tempStr,tempXspeed);
    tempGesture.get()->update();
    scoreGestures.push_back(tempGesture);
     
    
}

void ofApp::scoreDrawer(int i, vector <ofImage> & sift_score){
    
    ofSetColor(255,200);
    ofPushMatrix();
    
    ofTranslate(scoreGestures[i].get()->originalX,scoreGestures[i].get()->y);
    
    //ofRotate(-90);
    sift_score[scoreGestures[i].get()->timer%sift_score.size()].draw(0,0);
    ofPopMatrix();
    
    
}

bool sortMe(const shared_ptr<CustomParticle> &a, const shared_ptr<CustomParticle> &b){
    
    ofVec2f tempA = a.shared_ptr::get()->getPosition();
    ofVec2f tempB = b.shared_ptr::get()->getPosition();
    if (tempA.y < tempB.y ){
        return true;
    } else {
        return false;
    }
}
