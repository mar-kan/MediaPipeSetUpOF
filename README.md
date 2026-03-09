# Deepfake Detector

Real-time deepfake / authenticity detection using openFrameworks.

Work in progress — right now it just runs the webcam. Face detection and the actual analysis come next.

## Setup (macOS)

### 1. Get openFrameworks

Download **oF 0.12.1** from [openframeworks.cc/download](https://openframeworks.cc/download/) and unzip it somewhere (e.g. `~/Documents/of_v0.12.1_osx_release/`).

### 2. Clone this repo

This needs to live inside the oF folder structure:

```bash
cd ~/Documents/of_v0.12.1_osx_release/apps/myApps/
git clone https://github.com/Mista-Kev/deepfake-detector.git
```

### 3. Build & run

```bash
cd deepfake-detector
make -j4
make RunRelease
```

That's it. You should see a window with your webcam feed.

### Camera gotchas on macOS

This took us a while to figure out so writing it down:

- **Permission dialog**: The first time you run the app, macOS will ask for camera access. The app might not get any frames until you approve it and restart. This is normal.
- **Device ID**: `cam.setDeviceID(0)` grabs the built-in webcam. If you're using a USB camera, try `1` or `2`. There's no great way to list them from oF, just trial and error.
- **`initGrabber` is deprecated** in oF 0.12 — use `cam.setup(width, height)` instead. Old examples and tutorials still use `initGrabber`, it works but throws a warning.
- **Resolution**: We request 1280x720 but the actual resolution depends on your camera. The app scales to fill the window either way.
- If the camera just shows a black screen, check System Settings > Privacy > Camera and make sure your terminal / IDE has access.

### Addons needed (later)

Right now the project doesn't use any addons. Once we add face tracking we'll need:
- `ofxCv`
- `ofxFaceTracker2`

Instructions for setting those up will follow.
