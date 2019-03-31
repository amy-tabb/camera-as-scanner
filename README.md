# camera-as-scanner


Comments/Bugs/Problems: amy.tabb@ars.usda.gov

Code for taking measurements from images of an object on top of a calibration pattern.

~March 2019.  


# Underlying ideas; how and when to cite this work

Computer vision explanation: This code takes an image of an object on top of an aruco calibration pattern, calibrates the camera using the detected aruco information as well as EXIF tag information, and undistorts and computes the homography from the current location of the aruco calibration pattern in the image to its location in physical space.  Then the image is warped to match the coordinate system of the aruco coordinate system, scaled by a user-selected parameter. 

This README file is to accompany code produced by Amy Tabb as a companion to a paper.  The paper provides a full protocol to use the code:

To be announced.  Email for a draft.


Code release citation:

To be announced.


If you use this code in project that results in a publication, please cite at a minimum the paper above.  Otherwise, there are no restrictions in your use of this code.  However, no guarantees are expressed or implied.

# Related repositories

[amy-tabb/camera-as-scanner-data](https://github.com/amy-tabb/camera-as-scanner-data)

[amy-tabb/aruco-pattern-write](https://github.com/amy-tabb/aruco-pattern-write)


## Dependencies

This code uses the OpenCV 4.0, OpenCV 4.0 extra modules, Eigen and is written in C++. It also uses the `exiftool` executable.

### Tested operating system

This code has been tested on Ubuntu 16.04 and Ubuntu 18.04.  You are welcome to convert it to Windows, but I have not.  While OpenCV is available from distribution repositories, my long experience with it is has always been to build from the source to get the best results.

### OpenCV 4.0

To get the OpenCV4.0 extra modules to build, our experience is that you need to build *both* OpenCV and the extra modules together from source.  Instructions are here:

[OpenCV contributed modules on Github](https://github.com/opencv/opencv_contrib)

### Exiftools

On Ubuntu, install `exiftool` as follows:

```
sudo apt-get install exiftool
```

or

```
libimage-exiftool-perl
```

To test, make sure that the `exiftool` can be run from directory where you are setting up the `camera-as-scanner` project.



**Is getting all of this to work on your system too much of a pain and you are interested in a Docker release?  Let me know!  The squeaky wheel gets the grease.  Email above.**

## Building 


 1. Clone the git repository.  `cd` to the desired directory, then from the command line  -- ` git clone ` the repository name.
 
 2. If you use the [Eclipse CDT IDE](https://www.eclipse.org/cdt/), you can import the project directly after cloning.  You may have to alter the include directory for opencv4.  

2. **Note the OpenCV 4.x requires a >C++11 compiler!  I enable the GNU version with a flag : `-std=gnu++11.`  There other ways of doing this, as well as other choices of compiler.**

3.  Check the include directory for [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page), Eigen is a header-only library. 

4. Required libraries are: opencv_core, opencv_highgui, opencv_imgproc, opencv_imgcodecs, opencv_calib3d, opencv_aruco.

5. Build the project.


## Running

The program takes three arguments: 
- the read directory
- the write directory
- the scaling factor (number of pixels per millimeter)
- and optionally, a Boolean variable (0 for no, 1 for yes) indicating whether or not intermediate image files are written.

Our manuscript explains these items in detail.  If you're in a hurry, use 10 for the scaling factor.

Example valid run commands are:

```
./camera-as-scanner-project /home/your-name/read_directory /home/your-name/write_directory 10 
```
or


```
./camera-as-scanner-project /home/your-name/read_directory /home/your-name/write_directory 10 1 
```


## Read directory format

Examples of read and write directories are given within the [amy-tabb/camera-as-scanner-data](https://github.com/amy-tabb/camera-as-scanner-data) repository.  A read and write directory is `iphone` and `iphone_results`, respectively.  A calibration pattern that can be used for printing is `iphone/created_template.png`, or one can use companion repository [amy-tabb/aruco-pattern-write](https://github.com/amy-tabb/aruco-pattern-write).  

- `calibration_object_info.txt` contains one line: `squarelength 25.5 mm` in the examples.  To find the correct vaue for squarelength, measure one square on the printed calibration pattern.  Edit the file appropriately.
- `sensor_size.txt` contains two lines: `sensor_width 4.80 mm`, `sensor_height 3.60 mm`.  Values for your camera can be found from the manufacturer's website.  Note that EXIFtag information is usually not accurate.
- `specification_file.txt` contains the information generated from companion repository [amy-tabb/aruco-pattern-write](https://github.com/amy-tabb/aruco-pattern-write):

	```
	squaresX 12
	squaresY 15
	squareLength 200
	markerLength 100
	margins 100
	arc_code 11
	```

Note that if you use the pattern from the examples provided in this repository, you can copy the `specification_file.txt`.
-  `images` is the directory of image files.

## Write directory format

For each image in the `images` directory, the program will produce the following file, where `FILENAME` is the original image filename:

- `aruco_detectFILENAME`: image with aruco tag detections overlaid (only if the optional intermediate write image file variable is true).
- `undistortedFILENAME`: image after undistortion opertion.  This is useful to detect; if the calibration quality is poor, straight lines in reality will *not* be straight. (only if the optional intermediate write image file variable is true.)
- `warpedFILENAME`: image after applying transformation such that the image's coordinate system represents the calibration pattern's coordinate system, up to a scaling factor (as selected by the user -- last parameter). 

Text file `results.txt` records the scaling factor selected by the user.  From this, one can take two-dimensional measurements from the image (under the assumptions of planarity).  Say the scaling factor was 5, and a section is 50 pixels wide.  Assuming the units were millimeters and the object planar, the object is 10 mm wide.

`results.txt` also lists the calibration information for each image.

