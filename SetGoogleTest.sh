git clone https://github.com/google/googletest.git
cd googletest
ls
sudo -E cmake CMakeLists.txt
ls
make
sudo cp *.a /usr/lib
