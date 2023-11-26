# Compile Converseen on from Source Code

To manually compile **Converseen** on any **GNU/Linux** distribution, you need the **gnu c++ compiler**, **Qt5** or **Qt6 Framework Libraries** and **ImageMagick++** development libraries (preferably ImageMagick version 7 but legacy version 6 is also perfectly compatible).
Download Converseen, extract the archive content, enter directory and build the executable using these commands:

## Install Converseen using Qt5:

> tar -xvf converseen-0.*.tar.bz2
> cd converseen-0.x
> mkdir build cd build
> cmake .. (to install on /usr/local/)
> *or*
> cmake -DCMAKE_INSTALL_PREFIX=/usr  .. (to install on /usr/)
> make
> make install

## Install Converseen using Qt6:
> tar -xvf converseen-0.*.tar.bz2
> cd converseen-0.x
> mkdir build cd build
> cmake .. -DUSE_QT6=yes (to install on /usr/local/)
> *or*
> cmake -DCMAKE_INSTALL_PREFIX=/usr -DUSE_QT6=yes .. (to install on /usr/)
> make
> make install
