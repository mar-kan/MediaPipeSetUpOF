# Fresh Ubuntu installation of OpenFrameworks

download openFrameworks
cd to openFrameworks location

tar -zxvf of_v0.12.0_linux64gcc6_release.tar.gz
mv of_v0.12.0_linux64gcc6_release openFrameworks

cd openFrameworks/scripts/linux/ubuntu
sudo ./install_dependencies.sh

cd ..
./compileOF.sh -j4
./compilePG.sh

// test example
cd ~/openFrameworks/examples/graphics/polygonExample
make
make run

# Setting up ofxMediaPipe

cd <path of openFrameworks directory>/openFrameworks/addons
git clone https://github.com/design-io/ofxMediaPipePython.git

cd ofxMediaPipePython
./InstallMediaPipe.sh

# download required models
mkdir -p <of path>/openFrameworks/addons/ofxMediaPipe/libs/mediapipe/models
cd <of path>/openFrameworks/addons/ofxMediaPipe/libs/mediapipe/models
wget https://storage.googleapis.com/mediapipe-models/face_landmarker/face_landmarker/float16/1/face_landmarker.task

export LD_LIBRARY_PATH=~/openFrameworks/addons/ofxMediaPipe/libs/mediapipe/lib/linux:$LD_LIBRARY_PATH

# install python 3.11 and prerequisites
sudo apt install python3-pip python3.11-venv python3.11-full libpython3.11-stdlib -y
python3.11 -m pip install mediapipe numpy --user

export LD_LIBRARY_PATH=/home/marianna/anaconda3/envs/mediapipe/lib:$LD_LIBRARY_PATH

# new project
cd <of path>/openFrameworks/projectGenerator
./projectGenerator

![alt text](include/image.png)

# existing project
git clone https://github.com/Mista-Kev/deepfake-detector.git

cd <of path>/openFrameworks/projectGenerator
./projectGenerator

Import the existing project, add the oxfMediaPipePython addon, and configure your platform, and update.

![alt text](include/image-1.png)

# Activate venv and install python packages
conda activate mediapipe
pip install Pillow matplotlib

# Execution

cd <of path>/openFrameworks/apps/myApps/DeepFaceDetector
make -j4
make runRelease