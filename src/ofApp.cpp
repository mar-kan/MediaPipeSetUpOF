#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(30);
    ofSetWindowTitle("DeepFace Detector");
    ofBackground(30, 30, 35);

    // explicit device ID 0 — on macOS the first call might just trigger
    // the permission dialog and not actually start the camera yet,
    // so we check isFrameNew() in update before doing anything with pixels
    cam.setDeviceID(0);
    cam.setDesiredFrameRate(30);
    cam.setup(1280, 720);
}

//--------------------------------------------------------------
void ofApp::update() {
    cam.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255);
    cam.draw(0, 0, ofGetWidth(), ofGetHeight());

    ofSetColor(255);
    ofDrawBitmapString("DEEPFACE DETECTOR", 10, 20);
    ofSetColor(150);
    ofDrawBitmapString("Webcam OK — next up: face detection", 10, 34);

    ofSetColor(100);
    ofDrawBitmapString("FPS: " + ofToString((int)ofGetFrameRate()),
                       ofGetWidth() - 70, ofGetHeight() - 10);
}
