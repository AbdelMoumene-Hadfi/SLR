sudo apt-get install cmake
git clone https://github.com/google/googletest.git
cd googletest-master
sudo cmake CMakeLists.txt
make
sudo cp *.a /usr/lib
