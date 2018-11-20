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

	/// One case:
	/// case B: aruco tags, same pattern.
	//      image filename, directory with the cali object specs, as well as the sensor specs, write directory


	/// arguments: executable_name type read_directory write_directory image_name(caseB only)

	string scasea = "a";
	string scaseb = "b";
	//bool caseA = false;  bool caseB = false;
	string read_directory = "";
	string write_directory = "";
	string image_filename = "";


//	if (argc > 2){
//		if (scasea.compare(string(argv[1])) == 0){
//			caseA = true;
//		}	else {
//			if (scaseb.compare(string(argv[1])) == 0){
//				caseB = true;
//			}
//		}
//	}
//
//	if (!caseA && !caseB){
//		cout << "Bad case entered.  Quitting." << argv[1] << endl;
//		exit(1);
//		// TODO -- example of good argument strings.
//	}

	/// do caseB first.
	{
		if (argc != 3){
			cout << "For case B, the arguments are executable_name caseb read_directory write_directory image_name" << endl;
			cout << "You only provided " << argc-1 << " arguments instead of 4.  Quitting " << endl;
			exit(1);
		}
		read_directory = argv[1];
		write_directory = argv[2];

		EnsureDirHasTrailingBackslash(read_directory);
		EnsureDirHasTrailingBackslash(write_directory);


		PatternsCreated P_Class(read_directory, write_directory, true);

		/// can we read the dictionary as well?  Yes, this is enumerated, so write that in the specification file.
		string id_directory = read_directory + "images/";

		CameraCali* C = new CameraCali(id_directory, &P_Class);
//		/// read exif information.
//		if (is_strawberry){
//			C->ReadExifInformationStrawberry(id_camera_dir);
//		}

		C->ReadExifInformationForAllImages(id_directory, read_directory);

		C->FindCornersAruco(id_directory);
		//C->ReadExifInformationGeneric(id_camera_dir);


		delete C;
	}


//	if (caseA){
//
//
//
//	}




	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}

//
// string read_directory = argv[1];
//	string write_directory = argv[2];
////
////
////string filename = read_directory + "number_cameras.txt";
////	in.open(filename.c_str());
////	if (!in.good()){
////		cout << "Input file is bad for number cameras -- abort." << filename << endl;
////		exit(1);
////	}
//
//
//
///// eventually, if we are reading, write to a different directory.
//if (!cam_cali_read){
//	command = "rm -r " + write_directory;
//	system(command.c_str());
//
//	command = "mkdir " + write_directory;
//	system(command.c_str());
//}
//
//
//command = "mkdir " + id_camera_dir;
//					system(command.c_str());
//					CCV[i]->FindCornersAruco(id_camera_dir);
//					CCV[i]->CalibrateStrawberrySet(id_camera_dir);
//
//
//					CameraCali* C = new CameraCali(id_camera_dir, &P_Class);
//
//
//					PatternsCreated P_Class(read_directory, is_strawberry);
//
//
//					string command;
//
//						EnsureDirHasTrailingBackslash(read_directory);
