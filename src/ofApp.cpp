#include "ofApp.h"
using namespace ofx::MediaPipe;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    ofSetVerticalSync(false);
    ofSetWindowTitle("DeepFace Detector");
    ofBackground(30, 30, 35);
    
    cam.setDeviceID(0);
    cam.setDesiredFrameRate(30);
    cam.setup(1280, 720);
    
    auto runMode = Tracker::MODE_LIVE_STREAM;
    
    // --- Setup the Hand Tracker ---
    handTracker = make_shared<HandTracker>();
    HandTracker::HandSettings hsettings;
    hsettings.runningMode = runMode;
    hsettings.maxNum = 6;
    handTracker->setup(hsettings);
    
    // --- Setup the Face Tracker ---
    faceTracker = make_shared<FaceTracker>();
    FaceTracker::FaceSettings fsettings;
    fsettings.maxNum = 6;
    fsettings.minDetectionConfidence = 0.25f;
    fsettings.minPresenceConfidence = 0.25f;
    fsettings.minTrackingConfidence = 0.25f;
    fsettings.runningMode = runMode;
    fsettings.outputFaceBlendshapes = true;
    faceTracker->setup(fsettings);
    
    // --- Setup the Pose Tracker ---
    poseTracker = make_shared<PoseTracker>();
    PoseTracker::PoseSettings psettings;
    psettings.maxNum = 6;
    psettings.runningMode = runMode;
    poseTracker->setup(psettings);
}

//--------------------------------------------------------------
void ofApp::update(){
    bool hasNewPixels = false;
    cam.update(); 

    if(currentMode == MODE_CAMERA) {
        if(cam.isInitialized() && cam.isFrameNew()) {
            mVideoPixels = cam.getPixels();
            mVideoPixels.setImageType(OF_IMAGE_COLOR); 
            mVideoPixels.mirror(false, true); 
            hasNewPixels = true;
        }
    } 
    else if(currentMode == MODE_VIDEO) {
        videoPlayer.update();
        if(videoPlayer.isInitialized() && videoPlayer.isFrameNew()) {
            mVideoPixels = videoPlayer.getPixels();
            mVideoPixels.setImageType(OF_IMAGE_COLOR); 
            hasNewPixels = true;
        }
    } 
    else if(currentMode == MODE_IMAGE) {
        if(loadedImage.isAllocated()) {
            mVideoPixels = loadedImage.getPixels();
            hasNewPixels = true; 
        }
    }

    if(hasNewPixels) {
        mVideoFps.newFrame();
        
        if(mVideoTexture.getWidth() != mVideoPixels.getWidth() || mVideoTexture.getHeight() != mVideoPixels.getHeight()){
            mVideoTexture.allocate(mVideoPixels);
        }
        mVideoTexture.loadData(mVideoPixels);

        if(ofGetFrameNum() % 30 == 0) { // Print once per second to avoid spam
            std::cout << "--- AI INPUT DATA ---" << std::endl;
            std::cout << "Current Mode: " << currentMode << std::endl;
            std::cout << "Width: " << mVideoPixels.getWidth() << " | Height: " << mVideoPixels.getHeight() << std::endl;
            std::cout << "Channels: " << mVideoPixels.getNumChannels() << " (Should be 3 for RGB)" << std::endl;
        }

        faceTracker->process(mVideoPixels);
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
        ss << "Pose Tracker FPS: " << poseTracker->getFps() << std::endl;
        ss << "Faces Detected: " << faceTracker->getFaces().size() << std::endl;

        ofSetColor(100);
		ofDrawBitmapStringHighlight(ss.str(), 24, 24 );

    }
}

void ofApp::resetAI() {
    // destroy competely and rebuild tracker
    faceTracker = make_shared<ofx::MediaPipe::FaceTracker>();
    
    ofx::MediaPipe::FaceTracker::FaceSettings fsettings;
    fsettings.maxNum = 10;
    fsettings.minDetectionConfidence = 0.25f;
    fsettings.minPresenceConfidence = 0.25f;
    fsettings.minTrackingConfidence = 0.25f;
    fsettings.runningMode = ofx::MediaPipe::Tracker::MODE_LIVE_STREAM;
    fsettings.outputFaceBlendshapes = true;
    
    faceTracker->setup(fsettings);
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


void ofApp::keyPressed(int key){
    if(key == 'c' || key == 'C') {
        currentMode = MODE_CAMERA;
        if(videoPlayer.isLoaded()) {
            videoPlayer.stop(); 
        }
        resetAI();
    }
    
    // Press 'U' to Upload Media
    if(key == 'u' || key == 'U') {
        ofFileDialogResult result = ofSystemLoadDialog("Select Video or Image");
        
        if(result.bSuccess) {
            string filePath = result.getPath();
            string extension = ofToLower(ofFilePath::getFileExt(filePath));
            
            if(extension == "mp4" || extension == "mov" || extension == "avi"){
                videoPlayer.load(filePath);
                videoPlayer.setLoopState(OF_LOOP_NORMAL);
                videoPlayer.play();
                currentMode = MODE_VIDEO; 
                resetAI();
            }
            else if(extension == "jpg" || extension == "png" || extension == "jpeg"){
                loadedImage.load(filePath);
                loadedImage.setImageType(OF_IMAGE_COLOR);
                currentMode = MODE_IMAGE; 
                resetAI();
            }
        }
    }
}