-- irrBullet 0.1.6 --
NOTE: Extract bin.7z to see the compiled examples!

For class list and documentation, refer to doc/doxygen/html/index.html

1. Introduction:

Thank you for using irrBullet!

It does include all of the needed Bullet features, plus features
that make it much easier to implement physics into your game or simulation.

It includes interfaces to physics objects, and pointers inside those
interfaces so that you can use the Bullet library to its full extent
if you need access to a function that is not yet included in the interfaces.

If you spot any bugs or would like to leave feadback,
let me know at josiah@skyreignsoftware.net16.net.

This version only comes with one simple example, with source code included.
You can find the compiled version in the bin/ directory.

There is only a release build of irrBullet in libs/release, 
as well as the source code to the wrapper.


2. Starting a project:

To begin using basic irrBullet features in your project, 
you must follow these steps:


1. Add these search directories to your project:
	%irrBullet%/include
	%irrBullet%/include/bheaders/Bullet

2. add "#include <irrBullet.h>" to the top of your project
with the other includes. 

3. add these files to your linker (available in the libs/ folder):
	libirrBullet.a, libbulletdynamics.a, libbulletsoftbody.a
	libGIMPACTUtils.a (if you use GImpact),
	libbulletmath.a, libbulletcollision.a

	Make sure the linker files are in that order for irrBullet,
	or your project will not compile.

3. Extra:
The header files to the Bullet Physics library is included, under
include/bheaders/Bullet. A project file to compile irrBullet is under
the source/ folder.

The media in the bin/ folder is not all mine, some of it is from
the Irrlicht examples folder. The Irrlicht 1.6.1 dll is included
for the examples to run.


Copyright (C) 2009-2010 Josiah H. (Skyreign Software)
