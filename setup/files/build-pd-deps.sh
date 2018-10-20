#!/bin/bash

echo "lolol" >> test.txt
exit 0

export SRCDIR=$HOME/src
cd $SRCDIR
curl -L https://github.com/pure-data/pure-data/archive/0.49-0.tar.gz | tar zxf -
cd pure-data-0.49-0
./autogen.sh
./configure --disable-portaudio --enable-fftw --disable-jack-framework --disable-oss --disable-locales
make -j4
sudo make install

cd $SRCDIR
git clone --depth=1 https://github.com/porres/pd-cyclone.git
cd pd-cyclone
make -j4 pdincludepath=/usr/local/include/pd/
sudo make install

cd $SRCDIR
git clone --depth=1 https://github.com/pd-externals/list-abs.git
rm -rf list-abs/.git
sudo mv list-abs/ /usr/local/lib/pd-externals/

cd $SRCDIR
git clone --depth=1 https://github.com/electrickery/pd-freeverb.git
cd pd-freeverb
make -j4
sudo make install

cd $SRCDIR
git clone --depth=1 --recursive https://git.iem.at/pd/comport.git
cd comport
make -j4
sudo make install

cd $HOME
git clone https://github.com/csaudiodesign/geps-standalone.git
