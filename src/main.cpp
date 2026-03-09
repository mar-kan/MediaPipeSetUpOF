#include "ofMain.h"
#include "ofApp.h"

int main() {
    ofGLWindowSettings settings;
    settings.setSize(1280, 720);
    settings.windowMode = OF_WINDOW;
    settings.setGLVersion(3, 2);
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}
