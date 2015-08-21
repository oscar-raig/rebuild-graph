mkir install_dir
cd install_dir
wget https://googletest.googlecode.com/files/gtest-1.7.0.zip
tar xzfv gtest-1.7.0.zip
cd gtest-1.7.0
./configure
make 
cp -a include/gtest ../gtest_docker
cp -a lib/.libs/* ../gtest_docker/libs
