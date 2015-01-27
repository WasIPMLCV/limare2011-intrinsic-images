# Retinex PDE Fork

This is a fork of the code provided at [http://www.ipol.im/pub/art/2011/lmps_rpe/](http://www.ipol.im/pub/art/2011/lmps_rpe/). Reference:

    [1] N. Limare, A. B. Petro, C. Sbert, and J.-M. Morel.
        Retinex Poisson Equation: a Model for Color Perception.
        Image Processing On Line, 2011.

Using Ubuntu, the code can be compiled using CMake after installing the required dependecies:

    $ sudo apt-get install cmake
    $ sudo apt-get install fftw3-dev
    $ sudo apt-get install libpng-dev

To compile the code, use CMake:

    $ cd limare2011-intrinsic-images
    $ cmake .
    $ make
    [ 25%] Building C object CMakeFiles/io_png.dir/io_png.c.o
    Linking C static library libio_png.a
    [ 25%] Built target io_png
    [ 50%] Building C object CMakeFiles/retinex_pde.dir/retinex_pde_lib.c.o
    Linking C static library libretinex_pde.a
    [ 50%] Built target retinex_pde
    [ 75%] Building C object CMakeFiles/retinex_pde_cli.dir/retinex_pde.c.o
    [100%] Building C object CMakeFiles/retinex_pde_cli.dir/norm.c.o
    Linking C executable retinex_pde_cli
    [100%] Built target retinex_pde_cli

To suppress the debug information, adapt add the `-NDEBUG` flag.

Use the created command line interface as follows:

    ./retinex_pde_cli T in.png rtnx.png shdng.png

**Tip:** Some basic documentation of io_png can be found on GitHub: [https://github.com/nilx/io_png](https://github.com/nilx/io_png).

Below you find an updated version of the original README.

## Original README

### About

Author: Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>, Jose-Luis Lisani <joseluis.lisani@uib.es>

Copyright: (C) 2009-2011 IPOL Image Processing On Line http://www.ipol.im/

License: GPL v3+, see GPLv3.txt

### Overview

This source code provides an implementation of the Retinex theory by a Poisson equation, as described in IPOL: [http://www.ipol.im/pub/algo/lmps_retinex_poisson_equation/](http://www.ipol.im/pub/algo/lmps_retinex_poisson_equation/).

This program reads a PNG image, computes its laplacian, performs a DFT, then solves the Poisson equation in the Fourier space and performs an inverse DFT. The result is normalized to have the same mean and variance as the input image and written as a PNG image.

### Requirements

The code is written in ANSI C, and should compile on any system with an ANSI C compiler.

The libpng header and libraries are required on the system for compilation and execution. See [http://www.libpng.org/pub/png/libpng.html](http://www.libpng.org/pub/png/libpng.html).

The fftw3 header and libraries are required on the system for compilation and execution. See [http://www.fftw.org/](http://www.fftw.org/).

### Compilation

~~Simply use the provided makefile, with the command `make`.~~ - Use Cmake instead!

Alternatively, you can manually compile

    cc io_png.c norm.c retinex_pde_lib.c retinex_pde.c -lpng -lfftw3f -o retinex_pde

Multi-threading is possible, with the FFTW_NTHREADS parameter:

    cc io_png.c norm.c retinex_pde_lib.c retinex_pde.c -DFFTW_NTHREADS=4 -lpng -lfftw3f -lfftw3f_threads -o retinex_pde

### Usage

This program takes ~~4~~5 parameters: `retinex_pde T in.png rtnx.png shdng.png`

* `T`: retinex threshold [0...1[
* `in.png`: input image
* `rtnx.png`: retinex output image
* `shdng.png`: shading output image

### About this File

Copyright 2009-2011 IPOL Image Processing On Line [http://www.ipol.im/](http://www.ipol.im/);

Author: Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>

Copying and distribution of this file, with or without modification, are permitted in any medium without royalty provided the copyright notice and this notice are preserved.  This file is offered as-is, without any warranty.

# License

See [http://www.ipol.im/pub/art/2011/lmps_rpe/](http://www.ipol.im/pub/art/2011/lmps_rpe/) for license information.

For license information of `Lenna.png` see [http://en.wikipedia.org/wiki/Lenna#mediaviewer/File:Lenna.png](http://en.wikipedia.org/wiki/Lenna#mediaviewer/File:Lenna.png).
