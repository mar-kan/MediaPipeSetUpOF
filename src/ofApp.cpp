#include "ofApp.h"

// setting this to avoid typing ofx::MediaPipe::HandTracker
// now we can just use HandTracker
using namespace ofx::MediaPipe;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30); // Kept at 30 for stability with 3 heavy AI trackers
    ofSetVerticalSync(false);
    ofSetWindowTitle("DeepFace Detector");
    ofBackground(30, 30, 35);
    
    // Explicit camera setup from DeepFace snippet
    cam.setDeviceID(0);
    cam.setDesiredFrameRate(30);
    cam.setup(1280, 720);
    
    // Tracker::MODE_LIVE_STREAM uses threads, avoiding Python GIL thread locks.
    auto runMode = Tracker::MODE_LIVE_STREAM;
    
    // --- Setup the Hand Tracker ---
    handTracker = make_shared<HandTracker>();
    HandTracker::HandSettings hsettings;
    hsettings.runningMode = runMode;
    hsettings.maxNum = 2;
    handTracker->setup(hsettings);
    
    // --- Setup the Face Tracker (MERGED) ---
    faceTracker = make_shared<FaceTracker>();
    FaceTracker::FaceSettings fsettings;
    fsettings.maxNum = 2;
    fsettings.minDetectionConfidence = 0.25f;
    fsettings.minPresenceConfidence = 0.25f;
    fsettings.minTrackingConfidence = 0.25f;
    fsettings.runningMode = runMode;
    fsettings.outputFaceBlendshapes = true; // CRITICAL FOR BLINK ANALYSIS
    faceTracker->setup(fsettings);
    
    // --- Setup the Pose Tracker ---
    poseTracker = make_shared<PoseTracker>();
    PoseTracker::PoseSettings psettings;
    psettings.maxNum = 2;
    psettings.runningMode = runMode;
    poseTracker->setup(psettings);
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    
    if(cam.isInitialized() && cam.isFrameNew()) {
        mVideoFps.newFrame();
        mVideoPixels = cam.getPixels();
        mVideoPixels.mirror(false, true); // Flips the webcam horizontally
        
        handTracker->process(mVideoPixels);
        faceTracker->process(mVideoPixels);
        poseTracker->process(mVideoPixels);
        
        mVideoTexture.loadData(mVideoPixels);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(mVideoTexture.getWidth() > 0 && mVideoTexture.getHeight() > 0) {
        
        ofRectangle windowRect(0, 0, ofGetWidth(), ofGetHeight());
        ofRectangle videoRect(0, 0, mVideoTexture.getWidth(), mVideoTexture.getHeight());
        videoRect.scaleTo(windowRect);
        
        ofSetColor(255);
        mVideoTexture.draw(videoRect);
        
        ofPushMatrix(); {
            ofTranslate(videoRect.x, videoRect.y);
            ofScale(videoRect.getWidth() / mVideoTexture.getWidth(), videoRect.getHeight() / mVideoTexture.getHeight());
            
            faceTracker->draw();
            handTracker->draw();
            poseTracker->draw();
        } ofPopMatrix();
        
        ofSetColor(255);
        ofDrawBitmapString("DEEPFACE DETECTOR", 10, 20);
        
        stringstream ss;
        ss << "App FPS: " << (int)ofGetFrameRate() << std::endl;
        ss << "Video FPS: " << mVideoFps.getFps() << std::endl;
        ss << "Hand Tracker FPS: " << handTracker->getFps() << std::endl;
        ss << "Face Tracker FPS: " << faceTracker->getFps() << std::endl;
        ss << "Pose Tracker FPS: " << poseTracker->getFps();
        ss << "Faces Detected: " << faceTracker->getFaces().size();

        ofSetColor(100);
		ofDrawBitmapStringHighlight(ss.str(), 24, 24 );

    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    // remove instances of trackers
    handTracker.reset();
    poseTracker.reset();
    faceTracker.reset();
    
    // clean up the python interpreter to prevent Linux segmentation faults
    Tracker::PyShutdown();
}

// ... [Leave the rest of the empty input functions like keyPressed exactly as they are] ...