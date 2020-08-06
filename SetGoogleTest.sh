sudo apt install libgtest-devcd googletest
cd /usr/src/gtest sudo && cmake CMakeLists.txt && sudo make
sudo cp libgtest.a libgtest_main.a /usr/lib
