#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    commands = false;
    plotHSens = 3;
    plotRSens = 0;
    paintGrow = 1000;
    xRot = 60;
    zRot = 0;
    zRotation = 0;
    initialRadius = 1.0;
    dropWidth = 0.301;
    dropThresh = 0.5;
    bufferSize = 32768;
    
    
    
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);
    // To use FFTW, try:
    //fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);
    
    drawBuffer.resize(bufferSize);
    middleBuffer.resize(bufferSize);
    audioBuffer.resize(bufferSize);
    
    nBins = fft->getBinSize();
    
    binWidth = (float) 22050 / nBins;
    binAmps = new float[nBins];
    binCumAmps = new float[nBins];
    
    drawBins.resize(fft->getBinSize());
    middleBins.resize(fft->getBinSize());
    audioBins.resize(fft->getBinSize());
    
    // 0 output channels,
    // 1 input channel
    // 44100 samples per second
    // [bins] samples per buffer
    // 4 num buffers (latency)
    
    ofSoundStreamSetup(0, 1, this, 44100, bufferSize, 4);
    
    mode = MIC;
    appWidth = ofGetWidth();
    appHeight = ofGetHeight();
    
    
    ofEnableDepthTest();
    
    ofBackground(0, 0, 0);
    

    //============
    
    PolarPoint binPoint;
    binPoint.polarFromTheta(4*pi, initialRadius);
    ofColor c = ofColor::fromHsb(0, 255, 255);
    c.a = 100;
    dropMesh1.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
    dropMesh1.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
    dropMesh2.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
    dropMesh2.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
    dropMesh3.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
    dropMesh3.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
    dropMesh4.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
    dropMesh4.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));

    dropMesh1.addColor(c);
    dropMesh1.addColor(c);
    dropMesh2.addColor(c);
    dropMesh2.addColor(c);
    dropMesh3.addColor(c);
    dropMesh3.addColor(c);
    dropMesh4.addColor(c);
    dropMesh4.addColor(c);
    

    
    int dropCount = 1;
    int k = 16;
    for(int j = 1;j<k-1; j++) {
        for (int i=1+(j-1)*(nBins/k); i < (j*nBins/k)+1; i+=pow(2,(j-1))) {
            
            float thetaMake = 2*pi*log2((float)i/10*binWidth) + 4*pi;
            
            binPoint.polarFromTheta(thetaMake, initialRadius);
            float newHue = fmodf(435.0-binPoint.theta/(2*pi)*255, 256.0);
            
            c.setHue(newHue);
            
            dropMesh1.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
            dropMesh1.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
            dropMesh2.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
            dropMesh2.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
            dropMesh3.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
            dropMesh3.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
            dropMesh4.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
            dropMesh4.addVertex(ofVec3f(binPoint.x, binPoint.y, 0));
            dropMesh1.addColor(c);
            dropMesh1.addColor(c);
            dropMesh2.addColor(c);
            dropMesh2.addColor(c);
            dropMesh3.addColor(c);
            dropMesh3.addColor(c);
            dropMesh4.addColor(c);
            dropMesh4.addColor(c);
            dropMesh1.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            dropMesh2.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            dropMesh3.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
            dropMesh4.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);            
        }
    }
    ofEnableBlendMode(OF_BLENDMODE_ADD);
   
    //==============
    
}


//--------------------------------------------------------------
void ofApp::update(){
    zRotation += zRot;
    plotHeight = plotHSens * bufferSize;
    plotRadius = plotRSens * bufferSize;
    PolarPoint binPoint;
    binPoint.polarFromTheta(4*pi, initialRadius);
    dropMesh1.setVertex(0, ofVec3f(binPoint.x,binPoint.y));
    dropMesh1.setVertex(1, ofVec3f((binPoint.x + dropWidth * paintGrow * cos(binPoint.theta) * binCumAmps[0]*initialRadius), (binPoint.y - dropWidth * paintGrow * sin(binPoint.theta) * binCumAmps[0]*initialRadius), -0.7 * paintGrow * binCumAmps[0]*initialRadius));
    dropMesh2.setVertex(0, ofVec3f((binPoint.x + dropWidth * paintGrow * cos(binPoint.theta) * binCumAmps[0]*initialRadius), (binPoint.y - dropWidth * paintGrow * sin(binPoint.theta) * binCumAmps[0]*initialRadius), -0.7 * paintGrow * binCumAmps[0]*initialRadius));
    dropMesh2.setVertex(1, ofVec3f(binPoint.x, binPoint.y, - paintGrow * binCumAmps[0]*initialRadius));
    dropMesh3.setVertex(0, ofVec3f(binPoint.x, binPoint.y, - paintGrow * binCumAmps[0]*initialRadius));
    dropMesh3.setVertex(1, ofVec3f(binPoint.x - dropWidth * paintGrow * cos(binPoint.theta) * binCumAmps[0]*initialRadius, binPoint.y + dropWidth * paintGrow * sin(binPoint.theta) * binCumAmps[0]*initialRadius, -0.7 * paintGrow * binCumAmps[0]*initialRadius));
    dropMesh4.setVertex(0, ofVec3f(binPoint.x - dropWidth * paintGrow * cos(binPoint.theta) * binCumAmps[0]*initialRadius, binPoint.y + dropWidth * paintGrow * sin(binPoint.theta) * binCumAmps[0]*initialRadius, -0.7 * paintGrow * binCumAmps[0]*initialRadius));
    dropMesh4.setVertex(1, ofVec3f(binPoint.x,binPoint.y));
    int k = 16;
    int l = 1;
    for(int j = 1;j<k-1; j++) {
        for (int i=1+(j-1)*(nBins/k); i < (j*nBins/k)+1; i+=pow(2,(j-1))) {
            float thetaMake = 2*pi*log2((float)i/10*binWidth) + 4*pi;
            
            binPoint.polarFromTheta(thetaMake, initialRadius);
            float newHue = fmodf(435.0-binPoint.theta/(2*pi)*255, 256.0);
            dropMesh1.setVertex(l*2, ofVec3f(binPoint.x, binPoint.y,0));
            dropMesh1.setVertex(l*2+1, ofVec3f((binPoint.x + dropWidth * paintGrow * cos(binPoint.theta) * binCumAmps[i]*initialRadius), (binPoint.y - dropWidth * paintGrow * sin(binPoint.theta) * binCumAmps[i]*initialRadius), -0.7 * paintGrow * binCumAmps[i]*initialRadius));
            
            dropMesh2.setVertex(l*2, ofVec3f((binPoint.x + dropWidth * paintGrow * cos(binPoint.theta) * binCumAmps[i]*initialRadius), (binPoint.y - dropWidth * paintGrow * sin(binPoint.theta) * binCumAmps[i]*initialRadius), -0.7 * paintGrow * binCumAmps[i]*initialRadius));
            dropMesh2.setVertex(l*2+1, ofVec3f(binPoint.x, binPoint.y, - paintGrow * binCumAmps[i]*initialRadius));
            
            dropMesh3.setVertex(l*2, ofVec3f(binPoint.x, binPoint.y, - paintGrow * binCumAmps[i]*initialRadius));
            dropMesh3.setVertex(l*2+1, ofVec3f((binPoint.x - dropWidth * paintGrow * cos(binPoint.theta) * binCumAmps[i]*initialRadius), (binPoint.y + dropWidth * paintGrow * sin(binPoint.theta) * binCumAmps[i]*initialRadius), -0.7 * paintGrow * binCumAmps[i]*initialRadius));
            
            dropMesh4.setVertex(l*2, ofVec3f((binPoint.x - dropWidth * paintGrow * cos(binPoint.theta) * binCumAmps[i]*initialRadius), (binPoint.y + dropWidth * paintGrow * sin(binPoint.theta) * binCumAmps[i]*initialRadius), -0.7 * paintGrow * binCumAmps[i]*initialRadius));
            dropMesh4.setVertex(l*2+1, ofVec3f(binPoint.x, binPoint.y,0));
            if (binCumAmps[i] > dropThresh)  { //  || click on mouse at binPoint.x, binPoint.y, 0-binCumAmps[i] * paintGrow*initialRadius
                Drop newDrop;
                newDrop.makeDrop(dropMesh1.getColor(l*2), binCumAmps[i]*initialRadius*(binPoint.radius+2800)/10*0.35);
                newDrop.polarPoint = binPoint;
                newDrop.polarPoint.z = - paintGrow * binCumAmps[i]*initialRadius/2;
                newDrop.setPosition(newDrop.polarPoint.x*initialRadius, newDrop.polarPoint.y*initialRadius, newDrop.polarPoint.z);
                vDrops.push_back(newDrop);
                binCumAmps[i] *= 0.35;
            }
            l++;
            
        }
    }
    for(int i = 0 ; i < vDrops.size(); i++) {
        vDrops[i].polarPoint.z -= 18*initialRadius;
        vDrops[i].setPosition(vDrops[i].polarPoint.x+cos(vDrops[i].polarPoint.theta)*initialRadius, vDrops[i].polarPoint.y-sin(vDrops[i].polarPoint.theta)*initialRadius, vDrops[i].polarPoint.z);
        vDrops[i].dropColor();
        if (vDrops[i].getPosition().z  < -4000*initialRadius*dropThresh) {
            vDrops[i] = vDrops[vDrops.size()-1];
            vDrops.pop_back();
            i--;
        }
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(appWidth/2, appHeight/2);
    soundMutex.lock();
    drawBuffer = middleBuffer;
    drawBins = middleBins;
    soundMutex.unlock();
    ofRotateX(xRot);
    ofRotateZ(zRotation);
    PolarPoint oldPoint;
    oldPoint.polarFromTheta(0, initialRadius);
    ofSetLineWidth(1.0);
    for (int i=1; i < 807; i++) {
        PolarPoint newPoint;
        newPoint.polarFromTheta((float)i/10, initialRadius);
        ofDrawLine(oldPoint.x, oldPoint.y, newPoint.x, newPoint.y);
        oldPoint = newPoint;
    }

    PolarPoint oldBinPoint;
    oldBinPoint.polarFromTheta(4*pi, 1);
    ofColor c = ofColor::fromHsb(0, 255, 255);
    
    ofSetLineWidth(4.);
    int k = 16;
    for(int j = 1;j<k-1; j++) {
        for (int i=1+(j-1)*(nBins/k); i < (j*nBins/k)+1; i+=pow(2,(j-1))) {
            PolarPoint newBinPoint;
            float thetaMake = 2*pi*log2((float)i/10*binWidth) + 4*pi;
            
            newBinPoint.polarFromTheta(thetaMake, initialRadius);
            float newHue = fmodf(435.0-oldBinPoint.theta/(2*pi)*255, 256.0);
            c.setHue(newHue);
            ofSetColor(c);
            ofDrawLine(oldBinPoint.x+cos(oldBinPoint.theta)*plotRadius*binAmps[i-(int)pow(2,(j-1))], oldBinPoint.y-sin(oldBinPoint.theta)*plotRadius*binAmps[i-(int)pow(2,(j-1))], binAmps[i-(int)pow(2,(j-1))]*plotHeight*initialRadius,newBinPoint.x+cos(newBinPoint.theta)*plotRadius*binAmps[i], newBinPoint.y-sin(newBinPoint.theta)*plotRadius*binAmps[i],binAmps[i]*plotHeight*initialRadius);
            oldBinPoint = newBinPoint;
        }
    
    }

    
    dropMesh1.draw();
    dropMesh2.draw();
    dropMesh3.draw();
    dropMesh4.draw();
    for(int i = 0; i < vDrops.size(); i++) {
        vDrops[i].draw();  
    }
    
    ofPopMatrix();
    if(ofGetElapsedTimef() < 10 && !commands) {
        string info = "Spacebar to show / hide commands";
        ofDrawBitmapString(info, 250, appHeight - 20);
    }
//    string msg = ofToString((int) ofGetFrameRate()) + " fps";
//    ofDrawBitmapString(msg, appWidth - 80, appHeight - 20);
    if(commands) {
        string com = "W / S : Wave Height \t\t\tA / D : Wave Spread \nR / F : Drop Mesh Height \t\tQ / E : Drop Mesh Width \nT / G : Threshold to Drop (Careful!)\t0 (the number) : Zero Drop Mesh \nZ / X : Zoom\t\t\t\t- / + : Z Angle\n< / > : Rotation Speed\t\t\tP : Reset Position \nM : Mic Input (default)\t\t\tN : Noise Input\nB : Sine Input, Mouse X Position is Frequency \nAll commands do not require shift key";
        ofDrawBitmapString(com, 250, appHeight - 115);
    }
    
}

void ofApp::drawBaseSpiral() {
    ofSetColor(255);
    ofPushMatrix();
    ofTranslate(appWidth/2, appHeight/2);
    ofRotateX(0);
    PolarPoint oldPoint;
    oldPoint.polarFromTheta(0, initialRadius);
    ofSetLineWidth(1.0);
    for (int i=1; i < 824; i++) {
        PolarPoint newPoint;
        newPoint.polarFromTheta((float)i/10, initialRadius);
        ofDrawLine(oldPoint.x, oldPoint.y, newPoint.x, newPoint.y);
        oldPoint = newPoint;
    }
    ofPopMatrix();
    
}

float powFreq(float i) {
    return powf(i, 3);
}

void ofApp::plot(vector<float>& buffer, float scale, float offset) {
    ofNoFill();
    int n = buffer.size();
    ofDrawRectangle(0, 0, n, plotHeight);
    glPushMatrix();
    glTranslatef(0, plotHeight / 2 + offset, 0);
    ofBeginShape();
    for (int i = 0; i < n; i++) {
        ofVertex(i, buffer[i] * scale);
    }
    ofEndShape();
    glPopMatrix();
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels) {
    if (mode == MIC) {
        // store input in audioInput buffer
        memcpy(&audioBuffer[0], input, sizeof(float) * bufferSize);
        
    } else if (mode == NOISE) {
        for (int i = 0; i < bufferSize; i++)
            audioBuffer[i] = ofRandom(-1, 1);
    } else if (mode == SINE) {
        for (int i = 0; i < bufferSize; i++)
            audioBuffer[i] = sinf(PI * i * mouseX / appWidth);
    }
    
    fft->setSignal(&audioBuffer[0]);
    
    float* curFft = fft->getAmplitude();
    for (int i =0; i < nBins; i++) {
        binAmps[i] = fft->getAmplitudeAtBin((float)i)*((float)i+32)/(fft->getBinSize()/32);
        binCumAmps[i] += 2 * (pow((1+binAmps[i]),3)-1);
        if(binCumAmps[i] > 0.0)
            binCumAmps[i] *= 0.993;
         //   binCumAmps[i] -= 0.0002;
    //    binCumAmps[i] += binAmps[i];
    }
    memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());
    soundMutex.lock();
    middleBuffer = audioBuffer;
    middleBins = audioBins;
    soundMutex.unlock();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            commands = !commands;
            break;
        case 'm':
            mode = MIC;
            break;
        case 'n':
            mode = NOISE;
            break;
        case 'b':
            mode = SINE;
            break;
        case '=':
            xRot += 5;
            break;
        case '-':
            xRot -= 5;
            break;
        case ',':
            zRot -= 0.1;
            break;
        case '.':
            zRot += 0.1;
            break;
        case 's':
            plotHSens -= 0.25;
            break;
        case 'w':
            plotHSens += 0.25;
            break;
        case 'd':
            plotRSens -= 0.25;
            break;
        case 'a':
            plotRSens += 0.25;
            break;
        case 'q':
            dropWidth -= 0.02;
            break;
        case 'e':
            dropWidth += 0.02;
            break;
        case 'r':
            paintGrow *= 0.98;
            break;
        case 'f':
            paintGrow *= 1.02;
            break;
        case 'z':
            initialRadius *= 0.97;
            for(int i = 0; i < vDrops.size(); i++) {
                vDrops[i].resizeDrop(vDrops[i].radius*0.97);
            }
            break;
        case 'x':
            initialRadius *= 1.03;
            for(int i = 0; i < vDrops.size(); i++) {
                vDrops[i].resizeDrop(vDrops[i].radius*1.03);
            }
            break;
        case 't':
            dropThresh *= 0.98;
            break;
        case 'g':
            dropThresh *= 1.02;
            break;
        case '0':
            for (int i = 0; i < (nBins); i++)
                binCumAmps[i] = 0;
            break;
        case 'p':
            plotHSens = 3;
            plotRSens = 0;
            paintGrow = 1000;
            xRot = 60;
            zRot = 0;
            initialRadius = 1.0;
            dropWidth = 0.3;
            dropThresh = 0.5;
            break;
            
            
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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

void ofApp::exit(){
    delete [] binAmps;
    delete [] binCumAmps;
    
}
