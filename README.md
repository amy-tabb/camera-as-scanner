# camera-as-scanner


**TODO**

Currently

 This code is dependent on the >= OpenCV-3.* libraries and OpenMP library (libgomp). These libraries should be in the include path, or specified in your IDE.

    Compiler flags: we use OpenMP for parallelization and the C++11 standard. Note that Eclipse's indexer marks some items from C++11 as errors (but still compiles).

The flags needed using the gnu compiler, openmp, and the C++11 standard are: -fopenmp -std=gnu++11

though depending on the compiler used, you may need different flags

   
    
# Compiling, Linking, Running
Basic instructions for compilation and linking:

1. This code has been written and tested on Ubuntu 14.04 and Ubuntu 16.04, using Eclipse CDT as the IDE, and is written in C/C++.  


2. This code is dependent on the >= OpenCV-3.* libraries and OpenMP library (libgomp).  These libraries should be in the include path, or specified in your IDE.


3. Compiler flags: we use OpenMP for parallelization and the C++11 standard.  Note that Eclipse's indexer marks some items from C++11 as errors (but still compiles).  

The flags needed using the gnu compiler, openmp, and the C++11 standard are: `-fopenmp  -std=gnu++11`

though depending on the compiler used, you may need different [flags](https://www.dartmouth.edu/~rc/classes/intro_openmp/compile_run.html)
	
4. 	libraries needed for linking are:
- gomp   [OpenMP]
- opencv_core [OpenCV]
- opencv_highgui
- opencv_imgproc
- opencv_imgcodecs
- opencv_aruco

5. **Note before going further -- if in Ubuntu, you need to `apt-get install build-essential` to get C++ compiler(s) that are OpenMP enabled.  If you are on a Mac, you will need something comparable to build-essential.**
	
6. Easy way to build in Ubuntu with [Eclipse CDT](https://www.eclipse.org/cdt/) with git support (Egit): 
- `git clone` into the directory of choice.  
- Create new Managed C++ project, for the directory, select the directory where the `camera-as-scanner` was cloned into.
- The compiler and linker flags for OpenCV 4.0 will be imported, though you may have to alter the include paths.  Build and you're ready to run!

7. Easy way to build without Eclipse CDT:

**TODO**

```
g++ src/*.cpp -o curve_skel -fopenmp -std=gnu++11 -Wall -I/usr/local/include -lgomp -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
```

The executable `curve_skel` is created.  If you used method 6., the location will be in `Debug`.

## Running

Run the program with two arguments: the directory of test infomation, and the write directory.  Consequently, if the name of the executable is `camera-as-scanner-project`, to run:

```
./camera-as-scanner-project read-directory write-directory
```

## Input format

The read directory needs to have a particular format.  An example is included in **TODO**.  It is:
- images, a directory of the images to correct with this method.
- **TODO** , text files.

## Output format