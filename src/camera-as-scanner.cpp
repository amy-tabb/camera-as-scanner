//============================================================================
// Name        : Chessboard-camera-cali-project.cpp
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

int main(int argc, char **argv) {


	string read_directory = "";
	string write_directory = "";
	string image_filename = "";
	double homography_scaling;


		if (argc != 4){
			cout << "The arguments are the programname, the read-directory, the write-directory, and scaling factor (pixels per mm). " << endl;
			cout << "You only provided " << argc-1 << " arguments instead of 4.  Quitting " << endl;
			exit(1);
		}
		read_directory = argv[1];
		write_directory = argv[2];
		homography_scaling = FromString<double>(argv[3]);

		EnsureDirHasTrailingBackslash(read_directory);
		EnsureDirHasTrailingBackslash(write_directory);


		PatternsCreated P_Class(read_directory, write_directory, true);

		string id_directory = read_directory + "images/";

		cout << "Setting up camera calibration class ... " << endl;
		CameraCali* C = new CameraCali(id_directory, &P_Class);

		cout << endl << "Reading EXIF information for all images " << endl;
		C->ReadExifInformationForAllImages(id_directory, read_directory, write_directory);

		cout << endl << "Finding aruco patterns in each image " << endl;
		C->FindCornersArucoGeneral(write_directory);

		cout << endl << "Calibrating each image and warping." << endl;
		C->CalibrateArucoSinglyAndUndistort(write_directory, homography_scaling);


		cout << endl << "The code run is now complete!  Processed images are prefixed with warped. " << endl;
		delete C;

	return 0;
}


