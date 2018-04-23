# VTK-NVidia-StrangePixel

Description
-----------

**This bug only seem to happen on Linux (tested on Ubuntu 16.04) with an NVidia graphics
card (tested on Quadro M1000M). It was reproduced on a different computer with a different
NVidia graphics card (unknown) also on Ubuntu 16.04. It happens with several drivers (maybe all),
such as 384.130, 387,390, and 396. It does not appear on the same machine if the NVidia graphics
card is turned off and the Intel graphics card is used. It also does not appear on Windows
or MacOS with NVidia graphics cards.**

This repository contains a small example to reproduce a strange behavior: The center pixel
has a slightly different color than all the other pixels of the image, even though one
would expect the generated image to be uniform.

This can be mitigated by changing the window size to a size that does not generate this problem.
Strangely enough, there does not seem to be any consistency with this: A certain window size
might create the problem, whereas one that is just one pixel different in height or width might
not have this problem.

Changing the color of the object does not seem to make any difference: The final image will
still have its center pixel with a slightly different color.

This program outputs the coordinates and RGB values or the reference pixel and of the pixels
that have a different color. It also saves the generated image under the given file name. The
different color of the center pixel is barely noticeable because it is very subtle.

Usage
-----

./main output_image.png

Requirements
------------

See Installation steps below. This example requires CMake (>=2.8) and the Visualization ToolKit
(VTK >=8.1). VTK 8.1 and above is only required because the API has slightly changed. The bug
seems to still be there with earlier versions such as 8.0.

Installation
------------

Download and install [CMake](https://cmake.org/download/).

Build VTK:
cd ~/
git clone https://github.com/Kitware/VTK.git
mkdir VTK-build
cd VTK-build
cmake ../VTK && make

Compile this tool:
cd ~/
git clone https://github.com/fbudin69500/VTK-NVidia-StrangePixel.git
mkdir VTK-NVidia-StrangePixel-build
cd VTK-NVidia-StrangePixel-build
cmake -DVTK_DIR:PATH=~/VTK-build ../VTK-NVidia-StrangePixel
make

Notes
-----

* Bug does not appear with VTK 6.2 (Ubuntu 16.04 system VTK).
* If VTK 6.2 is compiled manually (default options, BUILD_TESTING=OFF).
**  => With VTK_RENDERING_BACKEND: OpenGL -> works (no bug)
**  => VTK_RENDERING_BACKEND: OpenGL2 -> fails (bug)
* Bug appears with VTK 7.0.0 and above, independently from the OpenGL backend used.
