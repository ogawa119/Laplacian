#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void testmesh();
	void rotate(double& x, double& y, double theta);
	void meshSpiralStaircase(double r, double h, unsigned n, double theta);
	void getOnering(ofMesh mesh);
	void displayPointIndex(ofMesh mesh);
	ofMesh mesh;
	ofEasyCam cam;
};
