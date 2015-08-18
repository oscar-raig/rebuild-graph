mkdir install
cd install
echo "Downloading gsl library"
wget http://ftp.gnu.org/gnu/gsl/gsl-1.16.tar.gz
tar xzvf gsl-1.16.tar.gz
mkdir ../gsl-1.16
cd gsl-1.16
./configure --prefix=`pwd`/../../gsl-1.16
make 
make install
cd ..
echo "Downloading boost library"
wget http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz
mkdir ../boost
tar xzvf boost_1_59_0.tar.gz
cd boost_1_59_0
./bootstrap.sh
./b2 cxxflags="-stdlib=libstdc++" linkflags="-stdlib=libstdc++" 
./b2 install --prefix=`pwd`/../../boost
echo "gsl library and boost library installed succesfully"
