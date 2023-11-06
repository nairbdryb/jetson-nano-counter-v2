# Utopia Capstone Trail Counter

## installation
flash most recent version of jetpack onto your jetson nano
update and upgrade
build this from source: https://github.com/dusty-nv/jetson-inference/blob/master/docs/building-repo-2.md (jetson-inference)
download this repo and run the counter.py program using python3


## installation of jetson-inference from source:
```bash
sudo apt-get update
sudo apt-get install git cmake libpython3-dev python3-numpy
git clone --recursive --depth=1 https://github.com/dusty-nv/jetson-inference
cd jetson-inference
mkdir build
cd build
cmake ../
make -j$(nproc)
sudo make install
sudo ldconfig
```