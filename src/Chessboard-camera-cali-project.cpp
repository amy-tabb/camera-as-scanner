//============================================================================
// Name        : Chessboard-camera-cali-project.cpp
// Author      : Amy Tabb
// Version     :
// Copyright   : MIT
// Description :
//============================================================================


#include "Includes.hpp"
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

	/// Two cases:
	/// case A: standard chessboard calibration situation
	///    directory of images, internal and external (optional)
	/// case B: aruco tags, same pattern.
	//      image filename, directory with the cali object specs, as well as the sensor specs, write directory

	string scasea = "a";
	string scaseb = "b";
	bool caseA = false;  bool caseB = false; // potentially confusing;
	if (argc > 2){
		if (scasea.compare(string(argv[1])) == 0){
			caseA = true;
		}	else {
			if (scaseb.compare(string(argv[1])) == 0){
				caseB = true;
			}
		}
	}

	if (!caseA && !caseB){
		cout << "Bad case entered.  Quitting." << endl;
		exit(1);
		// TODO -- example of good argument strings.
	}


	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}

//
//string read_directory = argv[1];
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
