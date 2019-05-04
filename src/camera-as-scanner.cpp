//============================================================================
// Name        : camera-as-scanner.cpp
// Author      : Amy Tabb
// Version     :
// Copyright   : MIT
// Description :
//============================================================================


#include "Includes.hpp"
#include "camera_calibration.hpp"
#include <iostream>
using namespace std;




void EnsureDirHasTrailingBackslash(string& write_directory){
	int n_letters = write_directory.size();
	bool eval =  (write_directory[n_letters - 1] == '/');
	cout << "Last character compare " << write_directory << " " <<  eval << endl;
	if (eval == false){
		write_directory = write_directory + "/";
	}

}

bool CheckExistenceOfDirectory(string write_directory){

	bool exists= true;
	struct stat info;
	if( stat( write_directory.c_str(), &info ) != 0 ){
		cout << "Path to directory is wrong and/or cannot access " << write_directory << endl;
		exists = false;
	}

	return exists;

}

int main(int argc, char **argv) {


	string read_directory = "";
	string write_directory = "";
	string image_filename = "";
	double homography_scaling;

	bool write_intermediate = false;

	if (argc < 4 ||  argc > 5){
		cout << "The arguments are the programname, the read-directory, the write-directory, scaling factor (pixels per mm), ";
		cout << " and optionally, a flag to indicate whether intermediate results are written" << endl;
		cout << "You only provided " << argc << " arguments instead of 4 or 5.  Quitting " << endl;
		exit(1);
	}
	read_directory = argv[1];
	write_directory = argv[2];
	homography_scaling = FromString<double>(argv[3]);

	/// check that both directories exist first.
	if (!CheckExistenceOfDirectory(read_directory)){
		exit(1);
	}

	if (!CheckExistenceOfDirectory(write_directory)){
		exit(1);
	}


	if (argc == 5){
		// some basic checking on arguments.
		string s = string(argv[4]);
		if (s.size() != 1){
			cout << "Error on last argument.  Boolean value should be 0 or 1.  You entered " << argv[4] << endl;
			exit(1);
		}

		int temp_write = FromString<int>(argv[4]);

		if (temp_write != 0 || temp_write != 1){
			cout << "Error on last argument.  Boolean value should be 0 or 1.  You entered " << argv[4] << endl;
			exit(1);
		}

		write_intermediate = FromString<bool>(argv[4]);
	}

	EnsureDirHasTrailingBackslash(read_directory);
	EnsureDirHasTrailingBackslash(write_directory);


	PatternsCreated P_Class(read_directory, write_directory, true);

	string id_directory = read_directory + "images/";

	cout << "Setting up camera calibration class ... " << endl;
	CameraCali* C = new CameraCali(id_directory, &P_Class);

	cout << endl << "Reading EXIF information for all images " << endl;
	C->ReadExifInformationForAllImages(id_directory, read_directory, write_directory);

	cout << endl << "Finding aruco patterns in each image " << endl;
	C->FindCornersArucoGeneral(write_directory, write_intermediate);

	cout << endl << "Calibrating each image and warping." << endl;
	C->CalibrateArucoSinglyAndUndistort(write_directory, homography_scaling, write_intermediate);


	cout << endl << "The code run is now complete!  Processed images are prefixed with warped. " << endl;
	delete C;

	return 0;
}


