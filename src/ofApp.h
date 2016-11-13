#pragma once

#include "ofMain.h"
#include "ofxFft.h"
#include "of3dPrimitives.h"
#include "ofGraphics.h"
//#include "PolarLocation.h"

enum {SINE, MIC, NOISE};

struct PolarPoint : public ofPoint {
    float radius;
    float theta;
    
    PolarPoint () {}
    ~PolarPoint () {}
    
    
    void polarFromTheta (float thetaMake, float initialRadius) {
        radius = initialRadius * pow(2,thetaMake/(2*pi));
        theta = thetaMake;
        while (theta > (2*pi)) theta -= (2*pi);
        while (theta < 0) theta += (2*pi);
        x = radius * cos(fmod(theta, (2 * pi)));
        y = radius * sin(fmod(theta, (2 * pi)));
        //    point.y = point.y - 768;
        //   float tempTheta = -theta - 90;
        
        
        if ((theta < (pi/2) &&  theta > (-pi/2)) || (theta > (3 * pi / 2) && theta < (5 * pi / 2)))
            
            //        (fmod(theta, (2 * pi)) < 0.25 || fmod(theta, (2 * pi)) > 0.75)
            x = fabs(x);
        else
            x =  - fabs(x);
        
        //    NSLog(@"Theta = %e", theta);
        if ((theta > 0.0 && theta <= pi) || (theta <= -pi && theta > (-2 * pi)) || (theta > (2*pi) && theta <= (3*pi)))
            //(fmod(theta, (2 * pi)) < 0.5)
            
            y = fabs(y);
        else
            y = - fabs(y);
        y = -y;
        z = 0;
    }
    
};


struct Drop : public of3dPrimitive {
    PolarPoint polarPoint;
    float radius;
//    bool dropping;
    Drop () {}
    ~Drop () {}
    
    void makeDrop(ofColor color, float r){
        radius = r;
        float x, y, z, theta, phi;
//        float r = 30;
        for(int i = 0; i<75; i++) {
            phi = 18 * PI* i/75;
          //  **theta = PI*i/150;
            x = r*i/75*std::cos(phi);
            y = r*i/75*std::sin(phi);
            z = 2*r - 2*r*i/75 - r*1.25;
            getMesh().addVertex(ofVec3f(x,y,z));
            getMesh().addColor(color);
        }
        for(int i = 75; i<150; i++) {
            
            phi = 18 * PI* i/150;
            theta = PI*i/150;
            x = r*std::sin(theta)*std::cos(phi);
            y = r*std::sin(theta)*std::sin(phi);
            z = r*std::cos(theta) - r*1.25;
            getMesh().addVertex(ofVec3f(x,y,z));
            getMesh().addColor(color);
        }
        getMesh().setMode(OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY);
 //       dropping = false;
//        cerr << "color: " << << << endl;
        
    }
    
    void dropColor() {
        ofColor c = ofColor::fromHsb(0, 255, 255);
        c.a = 100;
        float newHue = fmodf(435.0-polarPoint.theta/(2*pi)*255, 256.0);
        c.setHue(newHue);
        getMesh().clearColors();
        for (int i = 0; i < getMesh().getVertices().size(); i++) {
            getMesh().addColor(c);
        }
        
        
    }
    
    void resizeDrop(float newRadius) {
        for (int i = 0; i < getMesh().getVertices().size(); i++) {
            getMesh().setVertex(i,ofVec3f(getMesh().getVertex(i).x * newRadius/radius, getMesh().getVertex(i).y * newRadius/radius, getMesh().getVertex(i).z * newRadius/radius));
        }
        radius = newRadius;
    }
    
    // unused alternative function for making drops from cone and sphere mesh objects
    void makeNewDrop (float r, ofColor c) {
//        getMesh().setMode(OF_PRIMITIVE_TRIANGLES);
        ofMesh coneMesh;
        ofMesh sphereMesh;
        coneMesh = ofMesh::cone( r, r*2, 16, 16, 16, OF_PRIMITIVE_TRIANGLES);
        for(int i = 0; i < coneMesh.getVertices().size(); i++) {
            coneMesh.setVertex(i, ofVec3f(coneMesh.getVertex(i).x, -coneMesh.getVertex(i).z, -coneMesh.getVertex(i).y-r));
        }
 //       getMesh().addVertices(coneMesh.getVertices());
        sphereMesh = ofMesh::sphere(1, 16, OF_PRIMITIVE_TRIANGLES);
//        for(int i = 0; i < sphereMesh.getVertices().size()/2; i++) {
//            sphereMesh.setVertex(i, ofVec3f(sphereMesh.getVertex(i).x, -sphereMesh.getVertex(i).z, -sphereMesh.getVertex(i).y));
//  //          getMesh().addVertex(sphereMesh.getVertex(i));
//        }
        getMesh().addVertices(sphereMesh.getVertices());
        
//        getMesh().addVertices(sphereMesh.getVertices()[(0,sphereMesh.getVertices().size()/2));
        for(int i = 0; i < getMesh().getVertices().size(); i++) {
            getMesh().addColor(c);
        }
//        getMesh().addColors(c,getMesh().getVertices().size());
        
        
    }
    
    
    void setRadius(float newRadius) {
        radius = newRadius;
        float x, y, z, theta, phi;
 //       float r = 30;
        for(int i = 0; i<75; i++) {
            phi = 18 * PI* i/75;
            //  theta = PI*i/150;
            x = radius*i/75*std::cos(phi);
            y = radius*i/75*std::sin(phi);
            z = 2*radius - 2*radius*i/75 - radius*1.25;
            getMesh().setVertex(i,ofVec3f(x,y,z));
 //           getMesh().addColor(color);
        }
        for(int i = 75; i<150; i++) {
            
            phi = 18 * PI* i/150;
            theta = PI*i/150;
            x = radius*std::sin(theta)*std::cos(phi);
            y = radius*std::sin(theta)*std::sin(phi);
            z = radius*std::cos(theta) - radius*1.25;
            getMesh().setVertex(i,ofVec3f(x,y,z));
//            getMesh().addColor(color);
        }
  //      polarPoint.z = 0;

    }
    
    
        // unused, unfinished function for canvas painting
    ofMesh crossSlice (float zCross) {
        ofMesh slice;
        float x, y;
        if(zCross < -2 * radius && zCross > -3*radius) {
            for (int i = 0; i < 180; i++) {
                x = ((zCross+3)*PI/2 * cos(i*PI/90));
                y = ((zCross+3)*PI/2 * sin(i*PI/90));
                slice.addVertex(ofVec3f(x,y,0));
            }
        }
            
        // if z = zCross add vertices
        
        
        return slice;
    }
    
};


class ofApp : public ofBaseApp{

	public:
//        ~ofApp () {
//            delete [] binAmps;
//            delete [] binCumAmps;
//            delete [] drops;
//        }
		void setup();
        void plot(vector<float>& buffer, float scale, float offset);
        void audioReceived(float* input, int bufferSize, int nChannels);
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void drawBaseSpiral();
    
        int plotHeight, plotRadius, bufferSize;
        bool commands;
    
        ofxFft* fft;
        int nBins;
        float binWidth, initialRadius, dropWidth, dropHeight, dropThresh, plotHSens, plotRSens;
        float* binAmps;
        float* binCumAmps;
    
        vector<Drop> vDrops;
    
 //       Drop* drops;
        ofMesh dropMesh1, dropMesh2, dropMesh3, dropMesh4;
    
        int spectrogramOffset;
        ofImage spectrogram;
        int mode;
        float xRot, zRot, zRotation, paintGrow;
    
        int appWidth, appHeight;
        
        ofMutex soundMutex;
  //      PolarPoint* spiralPoints;
    
        vector<float> drawBins, middleBins, audioBins;
        vector<float> drawBuffer, middleBuffer, audioBuffer;
        vector<ofMeshFace> triangles;
        ofMaterial material;
    
    
    
};
