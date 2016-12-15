set -e

# go somewhere safe
cd /tmp

# get the source to base APR 1.4.6
curl -L -O http://archive.apache.org/dist/apr/apr-1.4.6.tar.gz

# extract it and go into the source
tar -xzvf apr-1.4.6.tar.gz
cd apr-1.4.6

# configure, make, and make install
make
sudo make install

# rest and cleanup
cd /tmp
rm -rf apr-1.4.6 apr-1.4.6.tar.gz

# do the same with apr-util
curl -L -O http://archive.apache.org/dist/apr/apr-util-1.4.1.tar.gz

# extract
tar -xzvf apr-util-1.4.1.tar.gz
cd apr-util-1.4.1

# configure, make, and make install
./configure --with-apr=/usr/local/apr

# cleanup
cd /tmp
rm -rf apr-util-1.4.1* apr-1.4.6*
