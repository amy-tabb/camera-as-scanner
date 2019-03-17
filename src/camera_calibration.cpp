/*
 * camera_calibration.cpp
 *
 *  Created on: Jun 26, 2018
 *      Author: atabb
 */

#include "camera_calibration.hpp"
#include "DirectoryFunctions.hpp"




//class PatternsCreated{
//public:
//	vector<vector<int> > double_to_single;
//	vector<vector< cv::Point3f> > three_d_points_per;
//
//	PatternsCreated(string read_dir);
//
//};

string CreatePaddedNumberString(int number, int length){

	string temp_string = ToString<int>(number);

	int n = length - temp_string.size();

	for (int i = 0; i < n; i++){
		temp_string = "0" + temp_string;
	}

	return temp_string;
}



int CreateStrawberryImages(vector<Mat>& images, int squaresX, int squaresY, int squareLength, int markerLength, int margins, int id_start_number){
	/// mainCreate creates the backstop, some in as well as the individual markers.

	/// Print out a file the registers the markers on the board
	/// everything is written to the current directory ... probably not the best design.
	std::ofstream out;
	string filename = "specification_file.txt";
	out.open(filename.c_str());

	Ptr<aruco::Dictionary> dictionary =
			aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));

	Size imageSize;
	//	int squaresX = 5;
	//	int squaresY = 10;
	//	int squareLength = 200;
	//	int markerLength = 100;
	//	int margins = squareLength - markerLength;

	//	int id_start_number  = squaresX*squaresY + 100;
	//	out << "squaresX " << squaresX << endl;
	//	out << "squaresY " << squaresY << endl;
	//	out << "squareLength " << squareLength << endl;
	//	out << "markerLength " << markerLength << endl;
	//	out << "margins " << margins << endl;
	//	out << "id_start_number " << id_start_number << endl;
	//	out.close();
	imageSize.width = squaresX * squareLength;
	imageSize.height = squaresY * squareLength;


	Mat markerImg;
	Mat boardImage = Mat::zeros(imageSize.height, imageSize.width, CV_8UC1);
	boardImage.setTo(255);
	int x0, y0;

	for (int x = 0, count = 0; x < squaresX; x++){
		for (int y = 0; y < squaresY; y++, count++){
			aruco::drawMarker(dictionary, count, markerLength, markerImg, 1);

			/// where to place?
			x0 = x*squareLength + margins/2;
			y0 = y*squareLength + margins/2;

			Rect R = Rect(x0, y0, markerLength, markerLength);
			//cout << "x0, y0 " << x0 << ", " << y0 << endl;

			markerImg.copyTo(boardImage(R));
		}
		images.push_back(boardImage);
	}


	//filename = "backstop" + ToString<int>(squaresX) + "by" + ToString<int>(squaresY) + ".png";
	//cv::imwrite(filename, boardImage);

	Mat markerImageLarge = Mat::zeros(squareLength, squareLength, CV_8UC1);
	markerImageLarge.setTo(255);
	for (int id = id_start_number, count  = 0; count < 8; count++, id++){

		aruco::drawMarker(dictionary, id, markerLength, markerImg, 1);
		Rect R = Rect(margins/2, margins/2, markerLength, markerLength);
		markerImg.copyTo(markerImageLarge(R));
		filename = "markerID" + ToString<int>(id) + ".png";
		//cv::imwrite(filename, markerImageLarge);
		images.push_back(markerImageLarge);
	}

	return 0;
}

int CreateStrawberryImagesCharuco(vector<Mat>& images, int squaresX, int squaresY, int squareLength, int markerLength,
		int margins, int id_start_number){

	///seriously considering a charuco board for the background too....

	//https://docs.opencv.org/3.4.1/df/d4a/tutorial_charuco_detection.html
	/// mainCreate creates the backstop, some in as well as the individual markers.

	/// Print out a file the registers the markers on the board
	/// everything is written to the current directory ... probably not the best design.
	//	std::ofstream out;
	//	string filename = write_directory + "specification_file.txt";
	//	out.open(filename.c_str());

	/// 5x5 bits, 250 markers in this dictionary.,
	Ptr<aruco::Dictionary> dictionary =
			aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));

	Size imageSize;
	//	int squaresX = 7;
	//	int squaresY = 9;
	//	int squareLength = 48;
	//	int markerLength = 24;
	//	int margins = squareLength - markerLength;
	int number_markers = (squaresX/2)*((squaresY + 1)/2) + ((squaresX + 1)/2)*(squaresY/2);
	int number_boards = 3;


	imageSize.width = squaresX * squareLength;
	imageSize.height = squaresY * squareLength;


	//////////// this creates the aruco board.
	Mat markerImg;
	Mat boardImage = Mat::zeros(imageSize.height, imageSize.width, CV_8UC1);
	boardImage.setTo(255);
	int x0, y0;

	for (int x = 0, count = 0; x < squaresX; x++){
		for (int y = 0; y < squaresY; y++, count++){
			aruco::drawMarker(dictionary, count, markerLength, markerImg, 1);

			/// where to place?
			x0 = x*squareLength + margins/2;
			y0 = y*squareLength + margins/2;

			Rect R = Rect(x0, y0, markerLength, markerLength);
			//cout << "x0, y0 " << x0 << ", " << y0 << endl;

			markerImg.copyTo(boardImage(R));
		}

	}
	images.push_back(boardImage);





	// the problem is here.  I want the square length in pixels.  How to ensure this?  detect, and then write.

	//cv::aruco::CharucoBoard board = cv::aruco::CharucoBoard::create(squaresX, squaresY, 0.04, 0.02, dictionary);
	//cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(squaresX, squaresY, squareLength, markerLength, dictionary);

	markerLength = squareLength*.9;
	int ss = 3;
	cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(ss, ss, 0.04, 0.9*0.04, dictionary);

	vector<cv::Ptr<cv::aruco::CharucoBoard> > boards;
	boards.push_back(cv::aruco::CharucoBoard::create(ss, ss, 0.04, 0.02, dictionary));
	//Mat markerImg;
	imageSize.width = 1 * squareLength;
	imageSize.height = 1 * squareLength;
	boardImage = Mat::zeros(imageSize.height, imageSize.width, CV_8UC1);
	boardImage.setTo(255);
	//cv::Mat boardImage;
	boards[0]->ids[0] = id_start_number;
	board->ids[0] = id_start_number;
	board->draw( imageSize, boardImage, 10, 1 );

	images.push_back(boardImage.clone());



	for (int j  = 1; j < 8; j++){

		boards.push_back(cv::aruco::CharucoBoard::create(ss, ss, 0.04, 0.02, dictionary));
		boards[j]->ids[0] = id_start_number + j;

		board->ids[0]++;
		board->draw( imageSize, boardImage, 10, 1 );

		images.push_back(boardImage.clone());

	}


	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();

	// detectorParams->ccornerRefinementMethod = aruco::CORNER_REFINE_SUBPIX; // do corner refinement in markers
	bool readOk = readDetectorParameters(string("../src/detector_params.yml"), detectorParams);
	if(!readOk) {
		cout << "Invalid detector parameters file" << endl;
		return 0;
	}

	int board_selected;

	for (int i = 1, in = images.size(); i < in; i++){
		Mat image, imageCopy;

		image = images[i].clone();

		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;
		vector< Vec3d > rvecs, tvecs;

		// detect markers and estimate pose
		aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);

		// draw results
		image.copyTo(imageCopy);
		if(ids.size() > 0) {
			cout << ids[0] << endl;
			aruco::drawDetectedMarkers(imageCopy, corners, ids);

			std::vector<cv::Point2f> charucoCorners;
			std::vector<int> charucoIds;
			/// can choose which board to use here ...
			board_selected = ids[0]/number_markers;
			cv::aruco::interpolateCornersCharuco(corners, ids, image, boards[board_selected], charucoCorners, charucoIds);
			// if at least one charuco corner detected
			if(charucoIds.size() > 0)
			{
				/// corners (2d) are linked to the Ids
				cv::aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners, charucoIds, cv::Scalar(255, 0, 0));
			}

			cout << "board " << board_selected << endl;

			images.push_back(imageCopy.clone());

			//filename = write_dir + "/" + image_names[i];
			//cv::imwrite(filename, imageCopy);
		}
	}

	return 0;
}


int CreateTemplateArucoImages(vector<Mat>& images, int squaresX, int squaresY, int squareLength, int markerLength,
		int margins, int arc_code){

	Ptr<aruco::Dictionary> dictionary =
			aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(arc_code));

	Size imageSize;

	imageSize.width = squaresX * squareLength;
	imageSize.height = squaresY * squareLength;


	//////////// this creates the aruco board.
	Mat markerImg;
	Mat boardImage = Mat::zeros(imageSize.height, imageSize.width, CV_8UC1);
	boardImage.setTo(255);
	int x0, y0;

	for (int x = 0, count = 0; x < squaresX; x++){
		for (int y = 0; y < squaresY; y++, count++){
			aruco::drawMarker(dictionary, count, markerLength, markerImg, 1);

			/// where to place?
			x0 = x*squareLength + margins/2;
			y0 = y*squareLength + margins/2;

			Rect R = Rect(x0, y0, markerLength, markerLength);
			//cout << "x0, y0 " << x0 << ", " << y0 << endl;

			markerImg.copyTo(boardImage(R));
		}

	}
	images.push_back(boardImage);

	return 0;
}





int CreateStrawberryImagesCharucoExp(vector<Mat>& images, int squaresX, int squaresY, int squareLength, int markerLength,
		int margins, int id_start_number){

	/// write the spec file?

	///seriously considering a charuco board for the background too....

	//https://docs.opencv.org/3.4.1/df/d4a/tutorial_charuco_detection.html
	/// mainCreate creates the backstop, some in as well as the individual markers.

	/// Print out a file the registers the markers on the board
	/// everything is written to the current directory ... probably not the best design.
	//	std::ofstream out;
	//	string filename = write_directory + "specification_file.txt";
	//	out.open(filename.c_str());

	/// 5x5 bits, 250 markers in this dictionary.,
	// squares are 2cm x 2cm, 28 x 28 background.  so we need a board with 18x18 markers.

	Ptr<aruco::Dictionary> dictionary =
			aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));

	Size imageSize;
	//	int squaresX = 7;
	//	int squaresY = 9;
	//	int squareLength = 48;
	//	int markerLength = 24;
	//	int margins = squareLength - markerLength;
	int number_markers = (squaresX/2)*((squaresY + 1)/2) + ((squaresX + 1)/2)*(squaresY/2);
	int number_boards = 3;


	imageSize.width = squaresX * squareLength;
	imageSize.height = squaresY * squareLength;


	//////////// this creates the aruco board.
	Mat markerImg;
	Mat boardImage = Mat::zeros(imageSize.height, imageSize.width, CV_8UC1);
	boardImage.setTo(255);
	int x0, y0;

	int count = 0;
	for (int x = 0; x < squaresX; x++){
		for (int y = 0; y < squaresY; y++, count++){
			aruco::drawMarker(dictionary, count, markerLength, markerImg, 1);

			/// where to place?
			x0 = x*squareLength + margins/2;
			y0 = y*squareLength + margins/2;

			Rect R = Rect(x0, y0, markerLength, markerLength);
			//cout << "x0, y0 " << x0 << ", " << y0 << endl;

			markerImg.copyTo(boardImage(R));
		}

	}
	images.push_back(boardImage);





	// the problem is here.  I want the square length in pixels.  How to ensure this?  detect, and then write.

	//cv::aruco::CharucoBoard board = cv::aruco::CharucoBoard::create(squaresX, squaresY, 0.04, 0.02, dictionary);
	//cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(squaresX, squaresY, squareLength, markerLength, dictionary);

	markerLength = squareLength*.9;
	int ss = 3;
	cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(ss, ss, 0.04, 0.03, dictionary);

	vector<cv::Ptr<cv::aruco::CharucoBoard> > boards;
	//boards.push_back(cv::aruco::CharucoBoard::create(ss, ss, 0.04, 0.02, dictionary));
	//Mat markerImg;
	imageSize.width = 1 * squareLength;
	imageSize.height = 1 * squareLength;
	boardImage = Mat::zeros(imageSize.height, imageSize.width, CV_8UC1);
	boardImage.setTo(255);
	//cv::Mat boardImage;
	//	boards[0]->ids[0] = id_start_number;
	//	board->ids[0] = id_start_number;
	board->draw( imageSize, boardImage, 10, 1 );

	//images.push_back(boardImage.clone());

	Mat boardImageSmall = Mat::zeros(squareLength + margins, squareLength + margins, CV_8UC1);
	boardImageSmall.setTo(255);


	/// do the singles.
	int start_count = count;
	for (int j  = 0; j < 8; j++, count++){
		for (int x = 0; x < 1; x++){
			for (int y = 0; y < 1; y++){
				cout << "count " << count << endl;
				aruco::drawMarker(dictionary, count, markerLength, markerImg, 1);

				/// where to place?
				x0 = x*squareLength + margins/2;
				y0 = y*squareLength + margins/2;

				Rect R = Rect(x0, y0, markerLength, markerLength);
				//cout << "x0, y0 " << x0 << ", " << y0 << endl;

				markerImg.copyTo(boardImageSmall(R));
			}

		}
		images.push_back(boardImageSmall.clone());
	}




	for (int j  = 0; j < 8; j++){

		boards.push_back(cv::aruco::CharucoBoard::create(ss, ss, 0.04, 0.03, dictionary));

		for (int i = 0; i < 4; i++, start_count++){

			boards[j]->ids[i] = start_count;
			cout << "Start count " << start_count << endl;

			//board->ids[i]++;
		}
		boards[j]->draw( imageSize, boardImage, 10, 1 );

		images.push_back(boardImage.clone());

	}


	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();

	// detectorParams->ccornerRefinementMethod = aruco::CORNER_REFINE_SUBPIX; // do corner refinement in markers
	bool readOk = readDetectorParameters(string("../src/detector_params.yml"), detectorParams);
	if(!readOk) {
		cout << "Invalid detector parameters file" << endl;
		return 0;
	}

	int board_selected;

	for (int i = 0, in = images.size(); i < in; i++){
		Mat image, imageCopy;

		image = images[i].clone();

		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;
		vector< Vec3d > rvecs, tvecs;

		// detect markers and estimate pose
		aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);

		// draw results
		image.copyTo(imageCopy);
		if(ids.size() > 0) {
			cout << ids[0] << endl;
			aruco::drawDetectedMarkers(imageCopy, corners, ids, Scalar(125, 125, 125));

			std::vector<cv::Point2f> charucoCorners;
			std::vector<int> charucoIds;
			/// can choose which board to use here ...

			if (i >= 9){
				board_selected = i-9;
				cv::aruco::interpolateCornersCharuco(corners, ids, image, boards[board_selected], charucoCorners, charucoIds);
				// if at least one charuco corner detected
				if(charucoIds.size() > 0)
				{
					/// corners (2d) are linked to the Ids
					cv::aruco::drawDetectedCornersCharuco(imageCopy, charucoCorners, charucoIds, cv::Scalar(125, 125, 125));
				}

				cout << "board " << board_selected << endl;
			}

			//images.push_back(imageCopy.clone());

			string filename = "temp_file" + ToString<int>(i) + ".jpg";
			cv::imwrite(filename, imageCopy);
		}
	}

	return 0;
}



//
//PatternsCreated::PatternsCreated(string read_dir){
//
//	vector<int> squaresX;
//	vector<int> squaresY;
//	vector<int> pixelSquareLength;
//	vector<int> pixelMarkerLength;
//	vector<double> squareLength;
//	vector<int> number_markers;
//	int number_boards;
//
//	/// 5x5 bits, 250 markers in this dictionary.,
//	dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));
//
//	/// read spec file,
//	string filename = read_dir + "pattern_spec_id.txt";;
//	ifstream in;
//
//	in.open(filename.c_str());
//	if (!in.good()){
//		cout << "Input file is bad -- abort." << filename << endl;
//		exit(1);
//	}
//
//	//
//	string temp;
//	in >> temp >> temp;
//	number_boards = FromString<int>(temp);
//	for (int i = 0; i < number_boards; i++){
//		in >> temp >> temp; squaresX.push_back(FromString<int>(temp));
//		in >> temp >> temp; squaresY.push_back(FromString<int>(temp));
//		in >> temp >> temp; pixelSquareLength.push_back(FromString<int>(temp));
//		in >> temp >> temp; pixelMarkerLength.push_back(FromString<int>(temp));
//		in >> temp >> temp; number_markers.push_back(FromString<int>(temp));
//	}
//	in.close();
//
//
//	for (int i = 0, m = 0, s = 0; i < number_boards; i++){
//		/// the dimensions of the board are linked in here ....
//		boards.push_back(cv::aruco::CharucoBoard::create(squaresX[i], squaresY[i], pixelSquareLength[i], pixelMarkerLength[i], dictionary));
//
//		min_max_id_pattern.push_back(pair<int, int>(m, m));
//
//		for (int j = 0; j < number_markers[i]; j++, m++){
//			boards[i]->ids[j] = m;
//		}
//
//		min_max_id_pattern[i].second = min_max_id_pattern[i].first + number_markers[i] - 1;
//		min_max_id_squares.push_back(pair<int, int>(s, s));
//		s = s + (squaresX[i] - 1)*(squaresY[i] - 1) - 1;
//		min_max_id_squares[i].second = s;
//		s = s + 1;
//	}
//
//	// this is all inane, but want to avoid re-computing it and making a mistake later.
//	number_total_squares = min_max_id_squares[number_boards - 1].second  + 1;
//	int_number_markers = min_max_id_pattern[number_boards - 1].second + 1;
//	//number_markers = 0;
//
//
//	// then open up all of the physical measurement files, add to square length.
//	for (int i = 0; i < number_boards; i++){
//		filename = read_dir + "pattern_square_mm" + ToString<int>(i) + ".txt";
//		in.open(filename.c_str());
//		if (!in.good()){
//			cout << "Input file is bad for physical measurements of the patterns -- abort." << filename << endl;
//			exit(1);
//		}
//
//		in >> temp >> temp;
//		squareLength.push_back(FromString<double>(temp));
//
//		in.close();
//
//	}
//
//	// convert everything to the class members.
//	//////////////////////// double to single.//////////////////////////////
//	double mm;
//	three_d_points = vector< cv::Point3f >(number_total_squares, cv::Point3f());
//	int sm = 0;
//	for (int i = 0, sc = 0; i < number_boards; i++){
//		mm = squareLength[i];
//
//		vector<int> current_index(number_markers[i], 0);
//
//		for (int m = 0; m < number_markers[i]; m++, sc++){
//			current_index[m] = sc;
//		}
//
//		for (int r = 0; r < squaresY[i] - 1; r++){
//			for (int c = 0; c < squaresX[i] - 1; c++, sm++){
//				Point3f p(mm*float(c), float(r)*mm, 0);
//				three_d_points[sm] = p;
//			}
//		}
//
//		double_to_single.push_back(current_index);
//	}
//
//
//	vector<int> color0(3, 0);
//	vector<int> color1(3, 0);
//	vector<int> color2(3, 0);
//
//	color0[2] = 255;
//	color1[1] = 255;
//	color2[0] = 255;
//	display_colors.push_back(color0);
//	display_colors.push_back(color1);
//	display_colors.push_back(color2);
//	color0[1] = 255;
//	display_colors.push_back(color0);
//	color0[1] = 0;
//	color0[1] = 255;
//	display_colors.push_back(color0);
//	color2[1] = 255;
//	display_colors.push_back(color2);
//
//	number_patterns = number_boards;
//
//
//}


//
//PatternsCreated::PatternsCreated(string read_dir, bool strawberry_class){
//
//	vector<int> squaresX;
//	vector<int> squaresY;
//	vector<int> pixelSquareLength;
//	vector<int> pixelMarkerLength;
//	vector<double> squareLength;
//	vector<int> number_markers;
//	int number_boards;
//	single_aruco_markers = strawberry_class;
//
//	if (strawberry_class == false){
//		max_internal_patterns = 0;
//		internalx = 0;  internaly = 0;
//
//		/// 5x5 bits, 250 markers in this dictionary.,
//		dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));
//
//		/// read spec file,
//		string filename = read_dir + "pattern_spec_id.txt";;
//		ifstream in;
//
//		in.open(filename.c_str());
//		if (!in.good()){
//			cout << "Input file is bad -- abort." << filename << endl;
//			exit(1);
//		}
//
//		//
//		string temp;
//		in >> temp >> temp;
//		number_boards = FromString<int>(temp);
//		for (int i = 0; i < number_boards; i++){
//			in >> temp >> temp; squaresX.push_back(FromString<int>(temp));
//			in >> temp >> temp; squaresY.push_back(FromString<int>(temp));
//			in >> temp >> temp; pixelSquareLength.push_back(FromString<int>(temp));
//			in >> temp >> temp; pixelMarkerLength.push_back(FromString<int>(temp));
//			in >> temp >> temp; number_markers.push_back(FromString<int>(temp));
//		}
//		in.close();
//
//
//		for (int i = 0, m = 0, s = 0; i < number_boards; i++){
//			/// the dimensions of the board are linked in here ....
//			boards.push_back(cv::aruco::CharucoBoard::create(squaresX[i], squaresY[i], pixelSquareLength[i], pixelMarkerLength[i], dictionary));
//
//			min_max_id_pattern.push_back(pair<int, int>(m, m));
//
//			for (int j = 0; j < number_markers[i]; j++, m++){
//				boards[i]->ids[j] = m;
//			}
//
//			min_max_id_pattern[i].second = min_max_id_pattern[i].first + number_markers[i] - 1;
//			min_max_id_squares.push_back(pair<int, int>(s, s));
//			s = s + (squaresX[i] - 1)*(squaresY[i] - 1) - 1;
//			min_max_id_squares[i].second = s;
//			s = s + 1;
//		}
//
//		// this is all inane, but want to avoid re-computing it and making a mistake later.
//		number_total_squares = min_max_id_squares[number_boards - 1].second  + 1;
//		int_number_markers = min_max_id_pattern[number_boards - 1].second + 1;
//		//number_markers = 0;
//
//
//		// then open up all of the physical measurement files, add to square length.
//		for (int i = 0; i < number_boards; i++){
//			filename = read_dir + "pattern_square_mm" + ToString<int>(i) + ".txt";
//			in.open(filename.c_str());
//			if (!in.good()){
//				cout << "Input file is bad for physical measurements of the patterns -- abort." << filename << endl;
//				exit(1);
//			}
//
//			in >> temp >> temp;
//			squareLength.push_back(FromString<double>(temp));
//
//			in.close();
//
//		}
//
//		// convert everything to the class members.
//		//////////////////////// double to single.//////////////////////////////
//		double mm;
//		three_d_points = vector< cv::Point3f >(number_total_squares, cv::Point3f());
//		int sm = 0;
//		for (int i = 0, sc = 0; i < number_boards; i++){
//			mm = squareLength[i];
//
//			vector<int> current_index(number_markers[i], 0);
//
//			for (int m = 0; m < number_markers[i]; m++, sc++){
//				current_index[m] = sc;
//			}
//
//			for (int r = 0; r < squaresY[i] - 1; r++){
//				for (int c = 0; c < squaresX[i] - 1; c++, sm++){
//					Point3f p(mm*float(c), float(r)*mm, 0);
//					three_d_points[sm] = p;
//				}
//			}
//
//			double_to_single.push_back(current_index);
//		}
//
//
//		vector<int> color0(3, 50);
//		vector<int> color1(3, 50);
//		vector<int> color2(3, 50);
//
//		color0[2] = 255;
//		color1[1] = 255;
//		color2[0] = 255;
//		display_colors.push_back(color0);
//		display_colors.push_back(color1);
//		display_colors.push_back(color2);
//		color0[1] = 255;
//		display_colors.push_back(color0);
//		color0[1] = 150;
//		color0[2] = 150;
//		display_colors.push_back(color0);
//		color2[1] = 255;
//		display_colors.push_back(color2);
//
//		number_patterns = number_boards;
//	}	else {
//
//		cout << "Got to the strawberry case ... " << endl;
//		/// FOR THE STRAWBERRY CASE
//
//		/// 5x5 bits, 250 markers in this dictionary.,
//		dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));
//
//		/// read spec file,
//		string filename = read_dir + "specification_file.txt";;
//		ifstream in;
//
//		in.open(filename.c_str());
//		if (!in.good()){
//			cout << "Input file is bad -- abort." << filename << endl;
//			exit(1);
//		}
//
//
//		cout << "After open" << endl;
//		//
//		string temp;
//		//		in >> temp >> temp;
//		//		number_boards = FromString<int>(temp);
//		//		for (int i = 0; i < number_boards; i++){
//		int sX, sY, sL, mL, margin, id_start, id_end;
//
//
//		in >> temp >> sX; cout << temp << endl;
//		in >> temp >> sY; cout << temp << endl;
//		in >> temp >> sL; cout << temp << endl;
//		in >> temp >> mL; cout << temp << endl;
//		in >> temp >> margin; cout << temp << endl;
//		in >> temp >> id_start; cout << temp << endl;
//		in >> temp >> id_end; cout << temp << endl;
//		internalx = sX;
//		internaly = sY;
//		/// now, read in mm for external
//		// now, read in mm for internal
//
//		cout << "temp last " << temp << endl;
//
//		number_boards = id_end - id_start + 1;
//		cout << "After read " << number_boards << endl;
//		//}
//		in.close();
//
//
//		for (int i = 0, m = id_start, s = 0; i < number_boards; i++){
//			/// the dimensions of the board are linked in here ....
//			//boards.push_back(cv::aruco::CharucoBoard::create(squaresX[i], squaresY[i], pixelSquareLength[i], pixelMarkerLength[i], dictionary));
//
//			min_max_id_pattern.push_back(pair<int, int>(m, m));
//
//			single_aruco_ids.push_back(m); m++;
//
//
//			min_max_id_pattern[i].second = min_max_id_pattern[i].first;
//			min_max_id_squares.push_back(pair<int, int>(s, s));
//			s = s + 4 - 1;
//			min_max_id_squares[i].second = s;
//			s = s + 1;
//		}
//
//		cout << "After squares " << endl;
//
//		// this is all inane, but want to avoid re-computing it and making a mistake later.
//		number_total_squares = min_max_id_squares[number_boards - 1].second  + 1;
//		int_number_markers = min_max_id_pattern[number_boards - 1].second + 1;
//		//number_markers = 0;
//		cout << "Number boards " << number_boards << endl;
//		cout << "Number total squares " << number_total_squares << endl;
//
//
//		// then open up all of the physical measurement files, add to square length.
//		//
//		filename = read_dir + "pattern_square_mm_external.txt";
//		in.open(filename.c_str());
//		if (!in.good()){
//			cout << "Input file is bad for physical measurements of the patterns -- abort." << filename << endl;
//			exit(1);
//		}
//
//		//double external_mm = 0;
//		in >> temp >> temp;
//		for (int i = 0; i < number_boards; i++){
//			squareLength.push_back(FromString<double>(temp));
//		}
//		in.close();
//
//		//}
//
//		cout << "Before 3d points " << endl;
//		// convert everything to the class members.
//		//////////////////////// double to single.//////////////////////////////
//		double mm;
//		three_d_points = vector< cv::Point3f >(number_total_squares, cv::Point3f());
//		int sm = 0;
//
//		/// external markers
//		for (int i = 0, sc = 0; i < number_boards; i++){
//			mm = squareLength[i];
//
//			vector<int> current_index(1, 0);
//
//			for (int m = 0; m < 1; m++, sc++){
//				current_index[m] = sc;
//			}
//
//			//			for (int r = 0; r < 2; r++){
//			//				for (int c = 0; c < 2; c++, sm++){
//			//					Point3f p(mm*float(c), float(r)*mm, 0);
//			//					three_d_points[sm] = p;
//			//				}
//			//			}
//
//			three_d_points[sm] = Point3f(mm*float(0), float(0)*mm, 0); sm++;
//			three_d_points[sm] = Point3f(mm*float(1), float(0)*mm, 0); sm++;
//			three_d_points[sm] = Point3f(mm*float(1), float(1)*mm, 0); sm++;
//			three_d_points[sm] = Point3f(mm*float(0), float(1)*mm, 0); sm++;
//
//			//
//
//
//
//			//			for (int r = 0; r < 2squaresY[i] - 1; r++){
//			//				for (int c = 0; c < squaresX[i] - 1; c++, sm++){
//			//					Point3f p(mm*float(c), float(r)*mm, 0);
//			//					three_d_points[sm] = p;
//			//				}
//			//			}
//
//			double_to_single.push_back(current_index);
//		}
//		cout << "After 3d points " << endl;
//
//
//		/// internal markers.
//		filename = read_dir + "pattern_square_mm_internal.txt";
//		in.open(filename.c_str());
//		if (!in.good()){
//			cout << "Input file is bad for internal physical measurements of the patterns -- abort." << filename << endl;
//			exit(1);
//		}
//
//		double internal_mm = 0;
//		in >> temp >> temp;
//		internal_mm =  FromString<double>(temp);
//
//		in.close();
//
//		cout << "Before create images " << endl;
//		// detect pattern locations on the image..
//		vector<Mat> images;
//		//CreateStrawberryImages(images, sX, sY, sL, mL, margin, id_start);
//
//		CreateStrawberryImagesCharuco(images, sX, sY, sL, mL, margin, id_start);
//		filename = read_dir + "created_backstop.png";
//		imwrite(filename.c_str(), images[0]);
//		for (int j = 1; j < 9; j++){
//			filename = read_dir + "Marker" + ToString<int>(j-1) + ".png";
//			imwrite(filename.c_str(), images[j]);
//		}
//		cout << "After create images " << endl;
//
//		/// detect the locations of the tags.  Come up with a way to map the tag locations to the 3D locations -- will copy and paste this regime into the cali regime.
//
//		// for each tag, generate where it should be in 3D space.  so indices are tag0 corner0-3 tag1 corner0-3 etc., .etc.  not on a grid necessarily.
//		Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();
//
//		cout << "Before create detector " << endl;
//		//
//		//	// detectorParams->ccornerRefinementMethod = aruco::CORNER_REFINE_SUBPIX; // do corner refinement in markers
//		bool readOk = readDetectorParameters(string("../src/detector_params.yml"), detectorParams);
//		if(!readOk) {
//			cout << "Invalid detector parameters file, quitting" << endl;
//			exit(1);
//		}
//		// already defined appropriately
//		//
//		//	Ptr<aruco::Dictionary> dictionary =
//		//			aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(cv::aruco::DICT_6X6_250));
//		//
//		//
//
//		cout << "Line 478 " << endl;
//		//int internal_image = 0;
//
//		vector< int > ids;
//		vector< vector< Point2f > > corners, rejected;
//		aruco::detectMarkers(images[0], dictionary, corners, ids, detectorParams, rejected);
//
//		//int number_internal_ids = sX*sY;
//
//		Mat imageCopy;
//
//		//images[0].copyTo(imageCopy);
//
//		cvtColor(images[0], imageCopy, cv::COLOR_GRAY2RGB);
//
//		if(ids.size() > 0) {
//			aruco::drawDetectedMarkers(imageCopy, corners, ids, Scalar(255, 255, 0));
//			filename = read_dir + "internal_with_labels.png";
//			cv::imwrite(filename, imageCopy);
//		}
//
//		//images[0].copyTo(imageCopy);
//
//
//		//imageCopy.setTo(255);
//
//		// internal_mm is the measurement per square edge.
//		// how far apart are the squares?
//		if (ids.size() > 0 && int(ids.size()) == sX*sY){
//			vector<int> mapping_to_id(sX*sY, 0);
//			// are the ids in order?
//			for (int i = 0, in = ids.size(); i < in; i++){
//				mapping_to_id[ids[i]] = i;
//			}
//
//			//			cout << "sX, sY " << sX << ", " << sY << endl;
//			//			for (int i = 0, in = ids.size(); i < in; i++){
//			//				cout << mapping_to_id[ids[i]] << endl;
//			//			}
//
//			// indices go Y first, X second. /// inverse of the way they are usually stored, but okay.
//			// markerLength is the length of the square in pixels.
//			// then squarelength is the distance from one corner to the other.
//
//			// now -- just do with pixel coordinates.
//
//			//	internal_mm = 100;
//
//			double distance_between_squares = internal_mm*double(sL)/double(mL);
//
//			cout << "Current settings, square length, distance bewteen squares froms start to finish " << internal_mm << ", " << distance_between_squares << endl;
//
//			//double x_coord;
//			//double y_coord;
//
//			int current_index;
//			int x_value, y_value;
//			vector< Point2f > current_corners;
//
//			int internal_squares = 4*sX*sY;
//			three_d_points_internal = vector< cv::Point3f >(internal_squares, cv::Point3f());
//			max_internal_patterns = sX*sY;
//
//			//			for (int i = 0, in = ids.size(); i < in; i++){
//			//				current_index = mapping_to_id[ids[i]];
//			//				current_corners = corners[current_index];
//			//
//			//				x_value = sX - current_index%sX - 1;
//			//				y_value = current_index/sX;
//			//
//			//				//string coords = ToString<int>(x_value) + ", " + ToString<int>(y_value);
//			//				//putText(imageCopy, coords, Point(current_corners[0].x,current_corners[0].y), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
//			//
//			//
//			//				// assign the value -- clockwise
//			//				Point3f p0(distance_between_squares*double(x_value), distance_between_squares*double(y_value) + internal_mm, 0);
//			//				Point3f p1(distance_between_squares*double(x_value) + internal_mm, distance_between_squares*double(y_value) + internal_mm, 0);
//			//				Point3f p2(distance_between_squares*double(x_value) + internal_mm, distance_between_squares*double(y_value), 0);
//			//				Point3f p3(distance_between_squares*double(x_value), distance_between_squares*double(y_value), 0);
//			//
//			//				//Point3f p(mm*float(c), float(r)*mm, 0);
//			//				three_d_points_internal[4*current_index] = p0; // would correspond to current corners[0]
//			//				three_d_points_internal[4*current_index + 1] = p1;
//			//				three_d_points_internal[4*current_index + 2] = p2;
//			//				three_d_points_internal[4*current_index + 3] = p3;
//			//
//			//				for (int j = 0; j < 4; j++){
//			//					string coords = ToString<float>(three_d_points_internal[4*current_index + j].x) + ", " + ToString<float>(three_d_points_internal[4*current_index + j].y);
//			//					putText(imageCopy, coords, Point(current_corners[j].x,current_corners[j].y), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1);
//			//
//			//				}
//			//
//			//			}
//
//
//			for (int i = 0, in = ids.size(); i < in; i++){
//				cout << "Pattern creation, in this order: " << ids[i] << endl;
//				current_index = ids[i];
//				current_corners = corners[i];
//
//				//x_value = sX - current_index%sX - 1;
//				//y_value = current_index/sX;
//
//				x_value = current_index/sY;
//				y_value = sY - current_index%sY - 1;
//
//				//string coords = ToString<int>(x_value) + ", " + ToString<int>(y_value);
//				//putText(imageCopy, coords, Point(current_corners[0].x,current_corners[0].y), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
//
//
//				// assign the value -- clockwise
//				Point3f p0(distance_between_squares*double(x_value), distance_between_squares*double(y_value) + internal_mm, 0);
//				Point3f p1(distance_between_squares*double(x_value) + internal_mm, distance_between_squares*double(y_value) + internal_mm, 0);
//				Point3f p2(distance_between_squares*double(x_value) + internal_mm, distance_between_squares*double(y_value), 0);
//				Point3f p3(distance_between_squares*double(x_value), distance_between_squares*double(y_value), 0);
//
//				//Point3f p(mm*float(c), float(r)*mm, 0);
//				three_d_points_internal[4*current_index] = p0; // would correspond to current corners[0]
//				three_d_points_internal[4*current_index + 1] = p1;
//				three_d_points_internal[4*current_index + 2] = p2;
//				three_d_points_internal[4*current_index + 3] = p3;
//
//
//				string coords = ToString<float>(x_value) + ", " + ToString<float>(y_value);
//				putText(imageCopy, coords, Point(current_corners[0].x,current_corners[0].y), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1);
//				for (int j = 1; j < 4; j++){
//					string coords = ToString<float>(three_d_points_internal[4*current_index + j].x) + ", " + ToString<float>(three_d_points_internal[4*current_index + j].y);
//					putText(imageCopy, coords, Point(current_corners[j].x,current_corners[j].y), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1);
//
//				}
//
//			}
//
//
//			filename = read_dir + "internal_with_morelabels.png";
//			cv::imwrite(filename, imageCopy);
//
//
//
//
//			cout << "A test ... what is the value of id 23 after everyone goes through? " << endl;
//			current_index = 23;
//			for (int j = 0; j < 4; j++){
//				string coords = ToString<float>(three_d_points_internal[4*current_index + j].x) + ", " + ToString<float>(three_d_points_internal[4*current_index + j].y);
//				cout << coords << endl;
//			}
//
//
//
//		}	else {
//			cout << "not finding all of the elements .... squareX, Y is wrong?  Double check specification file" << endl;
//			exit(1);
//		}
//
//
//
//		//putText(image, "opencv-hub", Point(5,100), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
//
//		//cout << "Quitting while ahead here .... "<< endl;
//		//exit(1);
//
//		// TODO need to write three_d_points_internal;
//
//
//		//
//		//	for (int i = 0, in = image_names.size(); i < in; i++){
//		//		Mat image, imageCopy;
//		//		string filename = read_dir + image_names[i];
//		//		cout << "filename " << filename << endl;
//		//		image = imread(filename.c_str());
//		//
//
//		//		vector< Vec3d > rvecs, tvecs;
//		//
//		//		// detect markers and estimate pose
//		//
//		//
//		//		// draw results
//		//		image.copyTo(imageCopy);
//		//		if(ids.size() > 0) {
//		//			aruco::drawDetectedMarkers(imageCopy, corners, ids);
//		//			filename = write_dir + image_names[i];
//		//			cv::imwrite(filename, imageCopy);
//		//		}
//		//	}
//
//
//
//
//
//
//		//			for (int r = 0; r < 2squaresY[i] - 1; r++){
//		//				for (int c = 0; c < squaresX[i] - 1; c++, sm++){
//		//					Point3f p(mm*float(c), float(r)*mm, 0);
//		//					three_d_points[sm] = p;
//		//				}
//		//			}
//
//
//
//		vector<int> color0(3, 0);
//		vector<int> color1(3, 0);
//		vector<int> color2(3, 0);
//
//		color0[2] = 255;
//		color1[1] = 255;
//		color2[0] = 255;
//		display_colors.push_back(color0);
//		display_colors.push_back(color1);
//		display_colors.push_back(color2);
//		color0[1] = 255;
//		display_colors.push_back(color0);
//		color0[1] = 0;
//		color0[1] = 255;
//		display_colors.push_back(color0);
//		color2[1] = 255;
//		display_colors.push_back(color2);
//
//
//		vector<int> color3(3, 0);
//		vector<int> color4(3, 150);
//		display_colors.push_back(color3);
//		display_colors.push_back(color4);
//
//
//		number_patterns = number_boards;
//
//
//	}
//}


string FindValueOfFieldInFile(string filename, string fieldTag, bool seperator){

	/// reopen file each time, in case things get switched around.  Assume that these are very small files, not the most efficient.

	ifstream in(filename.c_str());

	if (!in.good()){
		cout << "Filename to find " << fieldTag << " is bad " << filename << " quitting !" << endl;
		exit(1);
	}

	string cmp_str;
	string read_str;



	int line_counter = 0;
	vector<string> tokens;
	string token;
	string return_str = "";
	bool found = false;


	while (in  && found == false){

		in >> token;

		//cout << "token "  << token << endl;

		if (token.compare(fieldTag) == 0){
			found = true;

			if (seperator == true && in){
				in >> token;
			}

			if (in){
				in >> return_str;
			}

		}


		line_counter++;
	}


	cout << "Found! " << found << " field " << fieldTag << " and result " << return_str << endl;
	in.close();

	return return_str;

}

//
//int line_with_camera = 200;
//int line_with_focal_length = 200;
//string str1 = "Camera Model Name";
//string str2 = "Focal Length";
//string str3 = "ILCE-6000";
//string cmp_str;
//
//
//int line_counter = 0;
//while (in){
//	getline(in, read_str);
//
//	if (line_with_camera == 200){
//		std::size_t found = read_str.find(str1);
//		if (found!=std::string::npos){
//			line_with_camera = line_counter;
//
//			std::size_t found = read_str.find(str3);
//
//			if (found==std::string::npos){
//				cout << "Camera manufacturer and model not ILCE-6000! " << endl;
//				cout<< "read str  " << read_str << endl;
//				exit(1);
//			}
//		}
//	}
//
//	if (line_with_focal_length == 200){
//		std::size_t found = read_str.find(str2);
//		if (found!=std::string::npos){
//			line_with_focal_length = line_counter;
//			in.close();
//		}
//
//	}
//
//	line_counter++;
//
//}




PatternsCreated::PatternsCreated(string read_dir, string write_directory, bool aruco_markers){

	/// just read assuming that it will be aruco, one pattern.

	vector<int> squaresX;
	vector<int> squaresY;
	vector<int> pixelSquareLength;
	vector<int> pixelMarkerLength;
	vector<double> squareLength;
	vector<int> number_markers;
	vector<int> aruco_code;
	int number_boards;

	single_aruco_markers = aruco_markers;


	/// read spec file,
	string filename = read_dir + "specification_file.txt";;
	ifstream in;

	in.open(filename.c_str());
	if (!in.good()){
		cout << "Input file is bad -- abort." << filename << endl;
		exit(1);
	}
	in.close();


	cout << "After open" << endl;

	// margin is not needed, neither is the id for this one.
	int sX, sY, sL, mL, margin, id_start, id_end, ac;

	/// find everything we need ....
	string return_string;
	string fieldString;

	//// squaresX
	fieldString = "squaresX";
	return_string = FindValueOfFieldInFile(filename, fieldString, false);

	sX = FromString<int>(return_string);

	fieldString = "squaresY";
	return_string = FindValueOfFieldInFile(filename, fieldString, false);

	sY = FromString<int>(return_string);

	fieldString = "squareLength";
	return_string = FindValueOfFieldInFile(filename, fieldString, false);

	sL = FromString<int>(return_string);

	fieldString = "markerLength";
	return_string = FindValueOfFieldInFile(filename, fieldString, false);

	mL = FromString<int>(return_string);

	fieldString = "margins";
	return_string = FindValueOfFieldInFile(filename, fieldString, false);

	margin = FromString<int>(return_string);

	fieldString = "arc_code";
	return_string = FindValueOfFieldInFile(filename, fieldString, false);

	ac = FromString<int>(return_string);

	internalx = sX;
	internaly = sY;


	dictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(ac));
	/// now, read in mm for external
	// now, read in mm for internal


	//historical, from other code.
	number_boards = 1;
	cout << "After read " << number_boards << endl;
	//}
	in.close();

	cout << "After squares " << endl;

	// this is all inane, but want to avoid re-computing it and making a mistake later.
	//number_total_squares = min_max_id_squares[number_boards - 1].second  + 1;
	int_number_markers = 0;
	//number_markers = 0;
	//cout << "Number boards " << number_boards << endl;
	number_total_squares = sX*sY;
	cout << "Number total squares " << number_total_squares << endl;
	//

	// then open up all of the physical measurement files, add to square length.
	//
	filename = read_dir + "calibration_object_info.txt";
	in.open(filename.c_str());
	if (!in.good()){
		cout << "Input file is bad for physical measurements of the patterns -- abort." << filename << endl;
		exit(1);
	}


	// square length
	fieldString = "squarelength";
	return_string = FindValueOfFieldInFile(filename, fieldString, false);

	double mm_square = FromString<double>(return_string);

	for (int i = 0; i < number_boards; i++){
		squareLength.push_back(mm_square);
	}
	in.close();


	cout << "Before 3d points " << endl;
	// convert everything to the class members.
	//////////////////////// double to single.//////////////////////////////
	double mm;
	three_d_points = vector< cv::Point3f >(number_total_squares, cv::Point3f());
	int sm = 0;

	/// external markers
	for (int i = 0, sc = 0; i < number_boards; i++){
		mm = squareLength[i];

		vector<int> current_index(1, 0);

		for (int m = 0; m < 1; m++, sc++){
			current_index[m] = sc;
		}

		three_d_points[sm] = Point3f(mm*float(0), float(0)*mm, 0); sm++;
		three_d_points[sm] = Point3f(mm*float(1), float(0)*mm, 0); sm++;
		three_d_points[sm] = Point3f(mm*float(1), float(1)*mm, 0); sm++;
		three_d_points[sm] = Point3f(mm*float(0), float(1)*mm, 0); sm++;

		double_to_single.push_back(current_index);
	}
	cout << "After 3d points " << endl;

	double internal_mm =  squareLength[0];


	cout << "Before create images " << endl;
	// detect pattern locations on the image..
	vector<Mat> images;
	//CreateStrawberryImages(images, sX, sY, sL, mL, margin, id_start);

	/// maybe need this???
	CreateTemplateArucoImages(images, sX, sY, sL, mL, margin, ac);

	filename = read_dir + "created_template.png";
	imwrite(filename.c_str(), images[0]);

	cout << "After create image " << endl;

	/// detect the locations of the tags.  Come up with a way to map the tag locations to the 3D locations -- will copy and paste this regime into the cali regime.

	// for each tag, generate where it should be in 3D space.  so indices are tag0 corner0-3 tag1 corner0-3 etc., .etc.  not on a grid necessarily.
	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();

	cout << "Before create detector " << endl;
	//
	//	// detectorParams->ccornerRefinementMethod = aruco::CORNER_REFINE_SUBPIX; // do corner refinement in markers
	bool readOk = readDetectorParameters(string("../src/detector_params.yml"), detectorParams);
	if(!readOk) {
		cout << "Invalid detector parameters file, quitting" << endl;
		exit(1);
	}


	cout << "Line 478 " << endl;
	//int internal_image = 0;

	vector< int > ids;
	vector< vector< Point2f > > corners, rejected;
	aruco::detectMarkers(images[0], dictionary, corners, ids, detectorParams, rejected);

	//int number_internal_ids = sX*sY;

	Mat imageCopy;


	cvtColor(images[0], imageCopy, cv::COLOR_GRAY2RGB);

	if(ids.size() > 0) {
		aruco::drawDetectedMarkers(imageCopy, corners, ids, Scalar(255, 255, 0));
		filename = write_directory + "internal_with_labels.png";
		cv::imwrite(filename, imageCopy);
	}

	// internal_mm is the measurement per square edge.
	// how far apart are the squares?
	if (ids.size() > 0 && int(ids.size()) == sX*sY){
		vector<int> mapping_to_id(sX*sY, 0);
		// are the ids in order?
		for (int i = 0, in = ids.size(); i < in; i++){
			mapping_to_id[ids[i]] = i;
		}


		double distance_between_squares = internal_mm*double(sL)/double(mL);

		cout << "Current settings, square length, distance bewteen squares froms start to finish " << internal_mm << ", " << distance_between_squares << endl;



		int current_index;
		int x_value, y_value;
		vector< Point2f > current_corners;

		int internal_squares = 4*sX*sY;
		three_d_points_internal = vector< cv::Point3f >(internal_squares, cv::Point3f());
		max_internal_patterns = sX*sY;


		for (int i = 0, in = ids.size(); i < in; i++){
			cout << "Pattern creation, in this order: " << ids[i] << endl;
			current_index = ids[i];
			current_corners = corners[i];


			x_value = current_index/sY;
			y_value = sY - current_index%sY - 1;


			// assign the value -- clockwise
			Point3f p0(distance_between_squares*double(x_value), distance_between_squares*double(y_value) + internal_mm, 0);
			Point3f p1(distance_between_squares*double(x_value) + internal_mm, distance_between_squares*double(y_value) + internal_mm, 0);
			Point3f p2(distance_between_squares*double(x_value) + internal_mm, distance_between_squares*double(y_value), 0);
			Point3f p3(distance_between_squares*double(x_value), distance_between_squares*double(y_value), 0);

			//Point3f p(mm*float(c), float(r)*mm, 0);
			three_d_points_internal[4*current_index] = p0; // would correspond to current corners[0]
			three_d_points_internal[4*current_index + 1] = p1;
			three_d_points_internal[4*current_index + 2] = p2;
			three_d_points_internal[4*current_index + 3] = p3;


			string coords = ToString<float>(x_value) + ", " + ToString<float>(y_value);
			putText(imageCopy, coords, Point(current_corners[0].x,current_corners[0].y), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1);
			for (int j = 1; j < 4; j++){
				string coords = ToString<float>(three_d_points_internal[4*current_index + j].x) + ", " + ToString<float>(three_d_points_internal[4*current_index + j].y);
				putText(imageCopy, coords, Point(current_corners[j].x,current_corners[j].y), FONT_HERSHEY_DUPLEX, 0.5, Scalar(255, 0, 0), 1);

			}

		}


		filename = write_directory + "internal_with_morelabels.png";
		cv::imwrite(filename, imageCopy);



		cout << "A test ... what is the value of id 23 after everyone goes through? " << endl;
		current_index = 23;
		for (int j = 0; j < 4; j++){
			string coords = ToString<float>(three_d_points_internal[4*current_index + j].x) + ", " + ToString<float>(three_d_points_internal[4*current_index + j].y);
			cout << coords << endl;
		}


	}	else {
		cout << "not finding all of the elements .... squareX, Y is wrong?  Double check specification file" << endl;
		exit(1);
	}

	vector<int> color0(3, 0);
	vector<int> color1(3, 0);
	vector<int> color2(3, 0);

	color0[2] = 255;
	color1[1] = 255;
	color2[0] = 255;
	display_colors.push_back(color0);
	display_colors.push_back(color1);
	display_colors.push_back(color2);
	color0[1] = 255;
	display_colors.push_back(color0);
	color0[1] = 0;
	color0[1] = 255;
	display_colors.push_back(color0);
	color2[1] = 255;
	display_colors.push_back(color2);


	vector<int> color3(3, 0);
	vector<int> color4(3, 150);
	display_colors.push_back(color3);
	display_colors.push_back(color4);


	number_patterns = number_boards;
}



int PatternsCreated::NumberMarkers(){
	return int_number_markers;
}

int PatternsCreated::NumberPatterns(){
	return number_patterns;
}

int PatternsCreated::NumberSquares(){
	return number_total_squares;
}

//vector<vector<bool> > points_present;
//
//	vector<Mat> images; // the calibration images in the set;
//
//	Matrix3d internal_parameters;
//	VectorXd distortion;
//	vector<Matrix4d> external_parameters;
//
//	//?? how should do this, with gaps or not, how best to organize this so that it makes sense?
//	// probably with gaps, so that it matches points present for each image.  To calibrate, just line up with a fresh copy, no gaps.
//	// vector< vector< cv::Point2f> > internal_points;
//
//	CameraCali(string read_dir);
//static bool readDetectorParameters(string filename, Ptr<aruco::DetectorParameters> &params) {
//	FileStorage fs(filename, FileStorage::READ);
//	if(!fs.isOpened())
//		return false;
//	fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
//	fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
//	fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
//	fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
//	fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
//	fs["maxMarkerPerimeterRate"] >> params->maxMarkerPerimeterRate;
//	fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
//	fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
//	fs["minDistanceToBorder"] >> params->minDistanceToBorder;
//	fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
//	//fs["doCornerRefinement"] >> params->doCornerRefinement;
//	fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
//	fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
//	fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
//	fs["markerBorderBits"] >> params->markerBorderBits;
//	fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
//	fs["perspectiveRemoveIgnoredMarginPerCell"] >> params->perspectiveRemoveIgnoredMarginPerCell;
//	fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
//	fs["minOtsuStdDev"] >> params->minOtsuStdDev;
//	fs["errorCorrectionRate"] >> params->errorCorrectionRate;
//	return true;
//}

// this is the only function changed.
CameraCali::CameraCali(string read_dir, PatternsCreated* P){
	pixel_width = 0;
	count_internal_ids_present = 0;
	sensor_width  = 0;
	//straw_pattern_x = 0;
	//straw_pattern_y = 0;

	P_class = P; // have a ptr to all the full information about the patterns.

	// load the images ...

	//vector<string> im_names;
	string filename;
	string txt_ext = "txt";
	Mat im;
	//int count;

	string ext_dir = read_dir + "/external/";
	cout << "Ext dir! " << ext_dir << endl;

	// TODO -- sub in code from aruco read ...
	bool dir_exists = IsDirectory(ext_dir);
	if (dir_exists){
		vector<string> new_names;
		ReadDirectory(ext_dir, im_names);
		number_external_images_max = im_names.size();

		for (int i = 0, in = im_names.size(); i < in; i++){
			filename = ext_dir + im_names[i];
			cout << "Image name " << filename << ", " << im_names[i] << endl;
			if (filename.size() > 3 && filename.substr(filename.size() - 3, filename.size()) != txt_ext){
				im = imread(filename.c_str(), IMREAD_COLOR);
				rows = im.rows;
				cols = im.cols;
				images.push_back(im.clone());
				new_names.push_back(ext_dir + im_names[i]);
			}
		}

		new_names.swap(im_names);
	}	else {
		vector<string> new_names;
		ReadDirectory(read_dir, im_names);

		number_external_images_max = im_names.size();
		//number_external_images_max = 5;


		//for (int i = 0, in = im_names.size(); i < in; i++){
		for (int i = 0; i < number_external_images_max; i++){
			filename = read_dir + "/" + im_names[i];
			//cout << "Image name " << filename << ", " << im_names[i] << endl;
			if (filename.size() > 3 && filename.substr(filename.size() - 3, filename.size()) != txt_ext){
				new_names.push_back(read_dir + "/" + im_names[i]);
			}
		}

		if (new_names.size() > 0){
			im = imread(new_names[0].c_str(), IMREAD_COLOR);
			rows = im.rows;
			cols = im.cols;
			images.resize(new_names.size());
		}

		// this now works great!
#pragma omp parallel for
		for (int i = 0; i < number_external_images_max; i++){

			images[i] = imread(new_names[i].c_str(), IMREAD_COLOR);

		}

		new_names.swap(im_names);
	}



	/// read the internal dir.
	string int_dir = read_dir + "/internal/";
	dir_exists = IsDirectory(int_dir);
	//im_names.clear();
	vector<string> aux_names;
	if (dir_exists){
		ReadDirectory(int_dir, aux_names);
	}

	for (int i = 0, in = aux_names.size(); i < in; i++){
		filename = int_dir + aux_names[i];
		cout << "Image name " << filename << ", " << aux_names[i] << endl;
		if (filename.size() > 3 && filename.substr(filename.size() - 3, filename.size()) != txt_ext){
			im = imread(filename.c_str(), IMREAD_COLOR);
			images.push_back(im.clone());
		}
	}

	cout << "Number max images " << number_external_images_max << endl;

	//char gh; cin >> gh;

}


void CameraCali::ReadExifInformationStrawberry(string read_dir){

	vector<string> im_names;
	string filename;
	ReadDirectory(read_dir, im_names);


	filename = read_dir + "/" + im_names[0];

	string command;

	command = "exiftool " + filename + " > ../ex.txt";
	system(command.c_str());

	double SONY_sensor_width = 23.5;
	double SONY_sensor_height = 15.6;
	double focal_length = 16.0;


	string read_str;

	ifstream in;
	in.open("../ex.txt");

	//// redo --- read through and find
	/// Camera Model Name tag
	/// Focal Length tag
	int line_with_camera = 200;
	int line_with_focal_length = 200;
	string str1 = "Camera Model Name";
	string str2 = "Focal Length";
	string str3 = "ILCE-6000";
	string cmp_str;


	int line_counter = 0;
	while (in){
		getline(in, read_str);

		if (line_with_camera == 200){
			std::size_t found = read_str.find(str1);
			if (found!=std::string::npos){
				line_with_camera = line_counter;

				std::size_t found = read_str.find(str3);

				if (found==std::string::npos){
					cout << "Camera manufacturer and model not ILCE-6000! " << endl;
					cout<< "read str  " << read_str << endl;
					exit(1);
				}
			}
		}

		if (line_with_focal_length == 200){
			std::size_t found = read_str.find(str2);
			if (found!=std::string::npos){
				line_with_focal_length = line_counter;
				in.close();
			}

		}

		line_counter++;

	}

	in.open("ex.txt");
	cout << "line counter " << line_counter << endl;
	for (int m = 0; m < line_counter - 2; m++){
		getline(in, read_str);
	}
	//		getline(in, read_str);
	cout << "result " << read_str << endl;

	for (int m = 0; m < 3; m++){
		in >> read_str;

	}
	in >> focal_length;
	cout << "focal length " << focal_length << endl;
	in.close();


	// b/c these are tilted..
	pixel_width = double(images[0].rows)*SONY_sensor_width/focal_length;
	cout << "pixel width " << pixel_width << endl;

}


void CameraCali::ReadExifInformationForAllImages(string image_read_dir, string parent_dir){

	vector<string> im_names;
	string filename;
	ReadDirectory(image_read_dir, im_names);

	filename = parent_dir + "sensor_size.txt";


	string return_string;
	string fieldString = "sensor_width";

	fieldString = "sensor_width";
	return_string = FindValueOfFieldInFile(filename, fieldString, false);

	sensor_width = FromString<double>(return_string);


	string command;
	double focal_length = 0;

	for (int i = 0, n = im_names.size(); i < n; i++){

		filename = image_read_dir + im_names[i];

		command = "exiftool " + filename + " > ../ex.txt";
		system(command.c_str());

		//	double SONY_sensor_width = 23.5;
		//	double SONY_sensor_height = 15.6;
		//	double focal_length = 16.0;

		filename = "../ex.txt";

		// find focalLength
		fieldString = "Length";
		return_string = FindValueOfFieldInFile(filename, fieldString, true);

		focal_length = FromString<double>(return_string);

		pixel_width = double(images[i].cols)*focal_length/sensor_width;
		cout << "pixel width " << pixel_width << endl;

		focal_lengths.push_back(focal_length);
		pix_widths.push_back(pixel_width);

	}

}

void PatternsCreated::DetermineBoardsPresentFromMarkerList(vector<int>& markers, vector<bool>& boards_seen){

	int np = NumberPatterns();

	//	// initialize
	//	if (int(boards_seen.size()) != np){
	//		boards_seen.resize(np, false);
	//	}	else {
	//
	//		for (int j = 0; j < np; j++){
	//			boards_seen[j] = false;
	//		}
	//	}


	for (int i = 0, in = markers.size(); i < in; i++){
		for (int j = 0;  j < np; j++){ // don't walk this one if it is already occupied.
			if (markers[i] >= min_max_id_pattern[j].first && markers[i] <= min_max_id_pattern[j].second){
				boards_seen[j] = true;
			}
		}
	}
}

int PatternsCreated::MappingArucoIDToPatternNumber(int id){
	int np = NumberPatterns();

	//	// initialize
	//	if (int(boards_seen.size()) != np){
	//		boards_seen.resize(np, false);
	//	}	else {
	//
	//		for (int j = 0; j < np; j++){
	//			boards_seen[j] = false;
	//		}
	//	}


	for (int j = 0;  j < np; j++){ // don't walk this one if it is already occupied.
		if (id >= min_max_id_pattern[j].first && id <= min_max_id_pattern[j].second){
			return j;
		}
	}

	return -1;
}


//int PatternsCreated::SelectPattern(int CharucoID){
//
//	bool found = false;
//	int id = number_patterns;
//
//	for (int i = 0; i < number_patterns && found == false; i++){
//		if (CharucoID >= min_max_id_pattern[i].first && CharucoID <= min_max_id_pattern[i].second){
//			id = i;
//			found  = true;
//		}
//	}
//
//	return id;
//
//}

Scalar PatternsCreated::Color(int index){

	if (index >= display_colors.size() ){
		index = display_colors.size() % index;
	}
	return Scalar(display_colors[index][0], display_colors[index][1],display_colors[index][2]);
}


float euclideanDist(Point2f& p, Point2f& q) {
	Point2f diff = p - q;
	return sqrt(diff.x*diff.x + diff.y*diff.y);
}

void ComputeStats(vector<double>& ds, double& mu, double& var){

	mu = 0;
	double n = ds.size();
	var = 0;

	for (int i = 0, nn = ds.size(); i < nn; i++){
		mu += ds[i];
	}

	mu /= n;

	for (int i = 0, nn = ds.size(); i < nn; i++){
		var += pow(mu - ds[i], 2);
	}

	var /= n;

	cout << "mu is " << mu << ", variance is " << var << endl;
}

double ComputePDF(double mu, double var, double x){

	double pdf = (1.0/sqrt(2.0*3.14*var))*exp(-0.5*(pow(x - mu, 2)/(var)));

	return pdf;
}

void FilterLargerDuplicates( vector< int >& ids, vector< vector< Point2f > >& corners, int max_internal){

	int n = ids.size();
	vector<bool> keep(n, false);

	vector< Point2f > current_cornersi;
	vector< Point2f > current_corners0;

	vector<int> ids_new;
	vector< vector< Point2f > > corners_new;

	// don't keep until verify that is 1) unique number or 2) is smallest.
	bool found;
	int other_index;

	for (int i = 0; i < n; i++){

		if (keep[i] == false){
			found = false; other_index = -1;

			for (int j  = i + 1; j < n && found == false; j++){
				if (ids[j] == ids[i]){
					other_index = j;  found = true;
				}
			}

			if (found == false){
				keep[i] = true;
			}	else {
				// need to decide which one to keep.
				current_cornersi = corners[i];
				current_corners0 = corners[other_index];

				/// diagonal
				int di = euclideanDist(current_cornersi[0], current_cornersi[2]);
				int d0 = euclideanDist(current_corners0[0], current_corners0[2]);

				if (di < d0){
					keep[i] = true;
				}	else {
					keep[other_index] = true;
				}

			}
		}
	}

	/// now create new list ....
	for (int i = 0; i < n; i++){
		if (keep[i] == true){
			ids_new.push_back(ids[i]);
			corners_new.push_back(corners[i]);
		}
	}

	corners_new.swap(corners);
	ids_new.swap(ids);

	corners_new.clear();
	ids_new.clear();

	vector<double> distances;
	double mu, var;
	double pdf;


	/// find out the size of the internals ....
	for (int i = 0; i < n; i++){
		if (ids[i] < max_internal){
			current_cornersi = corners[i];
			int di = euclideanDist(current_cornersi[0], current_cornersi[2]);
			distances.push_back(di);
		}
	}

	ComputeStats( distances, mu, var);
	double std = sqrt(var);
	for (int i = 0; i < n; i++){
		if (ids[i] < max_internal){
			current_cornersi = corners[i];
			int di = euclideanDist(current_cornersi[0], current_cornersi[2]);
			cout << "Distance " << di  << endl;

			if (fabs(mu - di) < 0.20*mu ){
				ids_new.push_back(ids[i]);
				corners_new.push_back(corners[i]);
			}

		}	else {
			ids_new.push_back(ids[i]);
			corners_new.push_back(corners[i]);
		}
	}



	corners_new.swap(corners);
	ids_new.swap(ids);

	//char ch; cin >> ch;

}


void CameraCali::FindCornersAruco(string write_dir){

	/// first, need to find the patterns.
	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();

	bool readOk = readDetectorParameters(string("../src/detector_params.yml"), detectorParams);
	if(!readOk) {
		cout << "Cannot read in Calibrate -- Invalid detector parameters file" << endl;
		exit(1);
	}else {
		cout << "Read the detector parameters ..." << endl;
	}

	int number_patterns = P_class->NumberPatterns();
	int number_markers = P_class->NumberMarkers();
	int number_squares = P_class->NumberSquares();

	string filename;
	int number_images = images.size();
	Mat imageCopy;
	Scalar b_color;
	MatrixXd twod(number_squares, 2);
	twod.setConstant(0);
	int global_index;

	boards_detected.resize(number_images, vector<bool>(P_class->NumberPatterns(), false));


	std::ofstream out;
	filename = write_dir + "points.txt";
	out.open(filename.c_str());

	// we have external 0, 1, 2, 3, 4, 5, 6, 7
	// only get internal squares for one image .. the rest will be the same.

	/// also, read exif.
	int internal_pattern_max_squares = P_class->max_internal_patterns*4;
	MatrixXd twod_internal(internal_pattern_max_squares, 2);


	/// seg faul about access -- double check this
	//	./Box-Calibration-home-project /home/atabb/DemoData/Strawberry/Strawberry/dataset1/ /home/atabb/DemoData/Strawberry/Strawberry/dataset1results/ strawcali
	//
	//	P_class->three_d_points_internal[4*current_index + j].x)
	//cvtColor(src, bwsrc, cv::COLOR_RGB2GRAY);

	if (images.size() == 0){
		cout << "the number of images is 0, quitting " << endl;
		exit(1);
	}	else {
		cout << "Number of images " << images.size() << endl;
	}


	//	points_present.push_back(vector<bool>(number_squares, false));
	//			points_per_board.push_back(vector<int>(number_patterns, 0));
	//			two_d_point_coordinates_dense.push_back(twod);

	// cjharuco ... need to do something similar for this context.  internal and external will be handled differently.
	//	for (int i = 0; i < number_images; i++){
	//
	//			out << "image " << i << endl;
	//			// initialize per image.
	//			points_present.push_back(vector<bool>(number_squares, false));
	//			points_per_board.push_back(vector<int>(number_patterns, 0));
	//			two_d_point_coordinates_dense.push_back(twod);

	int current_index;
	int x_value;
	int y_value;



	//Mat gray;
	for (int i = 0; i < 1; i++){
		imageCopy = images[i].clone();

		/// already know the number of points per board.
		internal_two_d_point_coordinates_dense.push_back(twod_internal);
		//internal_ids_present.push_back(vector<bool>(P_class->max_internal_patterns, false));



		count_internal_ids_present = 0;
		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;
		//vector< Vec3d > rvecs, tvecs;

		//cout << "Before detect " << P_class->max_internal_patterns << endl;
		id_bool.resize(P_class->max_internal_patterns, false);

		// detect markers and estimate pose
		aruco::detectMarkers(images[i], P_class->dictionary, corners, ids, detectorParams, rejected);
		cout << "After detect " << endl;
		// draw results -- dealing with the possibility of more than one board per image.
		if(ids.size() > 0) {
			//P_class->DetermineBoardsPresentFromMarkerList(ids, boards_detected[i]); /// fills this in for all the ids, external patterns.
			//
			// need to make sure that the list is unique ....can take out later.
			FilterLargerDuplicates(ids, corners, P_class->max_internal_patterns);

			aruco::drawDetectedMarkers(imageCopy, corners, ids, Scalar(0, 255, 255));
			cout << "Id size " << ids.size() << endl;
			/// walk through the ids -- only process those that are in the range we're interested in.
			for (int id_count = 0, idn = ids.size(); id_count < idn; id_count++){

				cout << "Current id " << ids[id_count] << endl;
				// if this particular arUco pattern is within the internal range ...
				if (ids[id_count] < P_class->max_internal_patterns){



					vector< Point2f > current_corners;
					current_index = ids[id_count];

					count_internal_ids_present++;
					id_bool[current_index] = true;


					/// not all corners were found, so use id_count for this index.
					current_corners = corners[id_count];
					x_value = P_class->internalx - current_index%P_class->internalx - 1;
					y_value = current_index/P_class->internalx;


					for (int j = 0; j < 3; j++){
						line(imageCopy, Point(current_corners[j].x,current_corners[j].y), Point(current_corners[j+1].x,current_corners[j+1].y), Scalar(255, 255, 0), 10);
					}

					string coords = ToString<int>(current_index);
					putText(imageCopy, coords, Point(current_corners[0].x,current_corners[0].y), FONT_HERSHEY_DUPLEX, 2, Scalar(255, 0, 0), 2);

					for (int j = 0; j < 4; j++){
						internal_two_d_point_coordinates_dense[i](4*current_index + j, 0) = current_corners[j].x;
						internal_two_d_point_coordinates_dense[i](4*current_index + j, 1) = current_corners[j].y;

						string coords = ToString<float>(P_class->three_d_points_internal[4*current_index + j].x) + ", " + ToString<float>(P_class->three_d_points_internal[4*current_index + j].y);
						putText(imageCopy, coords, Point(current_corners[j].x,current_corners[j].y), FONT_HERSHEY_DUPLEX, 2, Scalar(255, 0, 0), 2);
					}
				}	else {

				}
			}
		}

		filename = write_dir + "internal_initial_detect" + ToString<int>(i) + ".jpg";
		imwrite(filename.c_str(), imageCopy);

		//out << endl << "end " << i << endl;
	}




	/// THEN READ > internal aruco number for all of the external files.  We only need one internal image per camera.
	for (int i = 0; i < number_images; i++){
		out << "image " << i << endl;
		imageCopy = images[i].clone();

		patterns_present.push_back(vector<bool>(number_patterns, false));
		points_per_board.push_back(vector<int>(number_patterns, 0));
		points_present.push_back(vector<bool>(number_squares, false));
		two_d_point_coordinates_dense.push_back(twod);

		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;


		// detect markers and estimate pose
		aruco::detectMarkers(images[i], P_class->dictionary, corners, ids, detectorParams, rejected);
		cout << "After detect " << endl;
		// draw results -- dealing with the possibility of more than one board per image.
		if(ids.size() > 0) {



			aruco::drawDetectedMarkers(imageCopy, corners, ids, Scalar(0, 255, 255));
			cout << "Id size " << ids.size() << endl;
			/// walk through the ids -- only process those that are in the range we're interested in.
			for (int id_count = 0, idn = ids.size(); id_count < idn; id_count++){



				cout << "Current id " << ids[id_count] << endl;
				// if this particular arUco pattern is within the internal range ...
				if (ids[id_count] >= P_class->max_internal_patterns){

					// need to record where these external patterns are ....
					//single_aruco_ids
					// TODO something else.
					// for now, figure out how to calibrate for internal characteristics.

					/// which pattern is it?
					// P_class single_arUco_ids tells us which pattern goes with which id.
					int pattern_index = P_class->MappingArucoIDToPatternNumber(ids[id_count]);

					cout << "This is pattern " << pattern_index << endl;
					b_color = P_class->Color(pattern_index);

					vector< Point2f > current_corners;
					current_corners = corners[id_count];
					current_index = ids[id_count];

					for (int j = 0; j < 3; j++){
						line(imageCopy, Point(current_corners[j].x,current_corners[j].y), Point(current_corners[j+1].x,current_corners[j+1].y), b_color, 10);
					}


					string coords = ToString<int>(pattern_index);
					putText(imageCopy, coords, Point(current_corners[0].x,current_corners[0].y), FONT_HERSHEY_DUPLEX, 2, b_color, 2);

					patterns_present[i][pattern_index] = true;
					points_per_board[i][pattern_index] = 4;


					for (int j = 0; j < 4; j++){

						points_present[i][4*pattern_index + j] = true;
						//	cout << "Writing to index " << 4*pattern_index + j << endl;
						two_d_point_coordinates_dense[i](4*pattern_index + j, 0) = current_corners[j].x;
						two_d_point_coordinates_dense[i](4*pattern_index + j, 1) = current_corners[j].y;

						global_index = 4*pattern_index + j;
						out << global_index << " " << std::setprecision(9) << current_corners[j].x << " " << std::setprecision(9) << current_corners[j].y << " ";

						if (j > 0){
							string coords = ToString<float>(P_class->three_d_points[4*pattern_index + j].x) + ", " + ToString<float>(P_class->three_d_points[4*pattern_index + j].y);
							putText(imageCopy, coords, Point(current_corners[j].x,current_corners[j].y), FONT_HERSHEY_DUPLEX, 1, b_color, 2);
						}
					}
				}
			}
		}

		out << endl << "end " << i << endl;

		filename = write_dir + "external_initial_detect" + ToString<int>(i) + ".jpg";
		//imwrite(filename.c_str(), imageCopy);



		Mat im_small;
		Size si;

		si.width = imageCopy.cols/8;
		si.height = imageCopy.rows/8;

		resize(imageCopy, im_small, si);

		imwrite(filename.c_str(), im_small);

	}



	// make it easy for self ... fill in everything computed in this function.
	out << "Image-board truth table " << endl;
	for (int i = 0; i < number_images; i++){
		out << "Image " << i << " : ";
		for (int p = 0; p < number_patterns; p++){
			out << 	points_per_board[i][p] << " " ;
		}
		out << endl;
	}


	out.close();

	//char ch; cin >> ch;
}


void CameraCali::FindCornersArucoGeneral(string write_dir){

	/// first, need to find the patterns.
	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();

	bool readOk = readDetectorParameters(string("../src/detector_params.yml"), detectorParams);
	if(!readOk) {
		cout << "Cannot read in Calibrate -- Invalid detector parameters file" << endl;
		exit(1);
	}else {
		cout << "Read the detector parameters ..." << endl;
	}

	int number_patterns = P_class->NumberPatterns();
	int number_markers = P_class->NumberMarkers();
	int number_squares = P_class->NumberSquares();

	string filename;
	int number_images = images.size();
	Mat imageCopy;
	Scalar b_color;
	MatrixXd twod(number_squares, 2);
	twod.setConstant(0);
	int global_index;

	// TODO is this necessary?? Probably not.
	boards_detected.resize(number_images, vector<bool>(P_class->NumberPatterns(), false));


	std::ofstream out;
	filename = write_dir + "points.txt";
	out.open(filename.c_str());


	/// also, read exif.
	int internal_pattern_max_squares = P_class->max_internal_patterns*4;
	MatrixXd twod_internal(internal_pattern_max_squares, 2);


	/// seg faul about access -- double check this
	//	./Box-Calibration-home-project /home/atabb/DemoData/Strawberry/Strawberry/dataset1/ /home/atabb/DemoData/Strawberry/Strawberry/dataset1results/ strawcali
	//
	//	P_class->three_d_points_internal[4*current_index + j].x)
	//cvtColor(src, bwsrc, cv::COLOR_RGB2GRAY);

	if (images.size() == 0){
		cout << "the number of images is 0, quitting " << endl;
		exit(1);
	}	else {
		cout << "Number of images " << images.size() << endl;
	}


	//	points_present.push_back(vector<bool>(number_squares, false));
	//			points_per_board.push_back(vector<int>(number_patterns, 0));
	//			two_d_point_coordinates_dense.push_back(twod);

	// cjharuco ... need to do something similar for this context.  internal and external will be handled differently.
	//	for (int i = 0; i < number_images; i++){
	//
	//			out << "image " << i << endl;
	//			// initialize per image.
	//			points_present.push_back(vector<bool>(number_squares, false));
	//			points_per_board.push_back(vector<int>(number_patterns, 0));
	//			two_d_point_coordinates_dense.push_back(twod);

	int current_index;
	int x_value;
	int y_value;

	//Mat gray;
	for (int i = 0, nimages = images.size(); i < nimages; i++){
		imageCopy = images[i].clone();

		/// already know the number of points per board.
		internal_two_d_point_coordinates_dense.push_back(twod_internal);
		//internal_ids_present.push_back(vector<bool>(P_class->max_internal_patterns, false));

		count_internal_ids_present = 0;
		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;
		//vector< Vec3d > rvecs, tvecs;

		//cout << "Before detect " << P_class->max_internal_patterns << endl;
		patterns_present.push_back(vector<bool>(P_class->max_internal_patterns, false));
		//id_bool.resize(P_class->max_internal_patterns, false);

		// detect markers and estimate pose
		aruco::detectMarkers(images[i], P_class->dictionary, corners, ids, detectorParams, rejected);
		cout << "After detect " << endl;
		// draw results -- dealing with the possibility of more than one board per image.
		if(ids.size() > 0) {
			//P_class->DetermineBoardsPresentFromMarkerList(ids, boards_detected[i]); /// fills this in for all the ids, external patterns.
			//
			// need to make sure that the list is unique ....can take out later.
			FilterLargerDuplicates(ids, corners, P_class->max_internal_patterns);

			aruco::drawDetectedMarkers(imageCopy, corners, ids, Scalar(0, 255, 255));
			cout << "Id size " << ids.size() << endl;
			/// walk through the ids -- only process those that are in the range we're interested in.
			for (int id_count = 0, idn = ids.size(); id_count < idn; id_count++){

				cout << "Current id " << ids[id_count] << endl;
				// if this particular arUco pattern is within the internal range ...
				if (ids[id_count] < P_class->max_internal_patterns){

					vector< Point2f > current_corners;
					current_index = ids[id_count];

					count_internal_ids_present++;
					patterns_present[i][current_index] = true;


					/// not all corners were found, so use id_count for this index.
					current_corners = corners[id_count];
					x_value = P_class->internalx - current_index%P_class->internalx - 1;
					y_value = current_index/P_class->internalx;


					for (int j = 0; j < 3; j++){
						line(imageCopy, Point(current_corners[j].x,current_corners[j].y), Point(current_corners[j+1].x,current_corners[j+1].y), Scalar(255, 255, 0), 10);
					}

					string coords = ToString<int>(current_index);
					putText(imageCopy, coords, Point(current_corners[0].x,current_corners[0].y), FONT_HERSHEY_DUPLEX, 2, Scalar(255, 0, 0), 2);

					for (int j = 0; j < 4; j++){
						internal_two_d_point_coordinates_dense[i](4*current_index + j, 0) = current_corners[j].x;
						internal_two_d_point_coordinates_dense[i](4*current_index + j, 1) = current_corners[j].y;

						string coords = ToString<float>(P_class->three_d_points_internal[4*current_index + j].x) + ", " + ToString<float>(P_class->three_d_points_internal[4*current_index + j].y);
						putText(imageCopy, coords, Point(current_corners[j].x,current_corners[j].y), FONT_HERSHEY_DUPLEX, 2, Scalar(255, 0, 0), 2);
					}
				}	else {

				}
			}
		}

		filename = write_dir + "internal_initial_detect" + ToString<int>(i) + ".jpg";
		imwrite(filename.c_str(), imageCopy);

		//out << endl << "end " << i << endl;
	}


	//
	//
	//	/// THEN READ > internal aruco number for all of the external files.  We only need one internal image per camera.
	//	for (int i = 0; i < number_images; i++){
	//		out << "image " << i << endl;
	//		imageCopy = images[i].clone();
	//
	//		patterns_present.push_back(vector<bool>(number_patterns, false));
	//		points_per_board.push_back(vector<int>(number_patterns, 0));
	//		points_present.push_back(vector<bool>(number_squares, false));
	//		two_d_point_coordinates_dense.push_back(twod);
	//
	//		vector< int > ids;
	//		vector< vector< Point2f > > corners, rejected;
	//
	//
	//		// detect markers and estimate pose
	//		aruco::detectMarkers(images[i], P_class->dictionary, corners, ids, detectorParams, rejected);
	//		cout << "After detect " << endl;
	//		// draw results -- dealing with the possibility of more than one board per image.
	//		if(ids.size() > 0) {
	//
	//
	//
	//			aruco::drawDetectedMarkers(imageCopy, corners, ids, Scalar(0, 255, 255));
	//			cout << "Id size " << ids.size() << endl;
	//			/// walk through the ids -- only process those that are in the range we're interested in.
	//			for (int id_count = 0, idn = ids.size(); id_count < idn; id_count++){
	//
	//
	//
	//				cout << "Current id " << ids[id_count] << endl;
	//				// if this particular arUco pattern is within the internal range ...
	//				if (ids[id_count] >= P_class->max_internal_patterns){
	//
	//					// need to record where these external patterns are ....
	//					//single_aruco_ids
	//					// TODO something else.
	//					// for now, figure out how to calibrate for internal characteristics.
	//
	//					/// which pattern is it?
	//					// P_class single_arUco_ids tells us which pattern goes with which id.
	//					int pattern_index = P_class->MappingArucoIDToPatternNumber(ids[id_count]);
	//
	//					cout << "This is pattern " << pattern_index << endl;
	//					b_color = P_class->Color(pattern_index);
	//
	//					vector< Point2f > current_corners;
	//					current_corners = corners[id_count];
	//					current_index = ids[id_count];
	//
	//					for (int j = 0; j < 3; j++){
	//						line(imageCopy, Point(current_corners[j].x,current_corners[j].y), Point(current_corners[j+1].x,current_corners[j+1].y), b_color, 10);
	//					}
	//
	//
	//					string coords = ToString<int>(pattern_index);
	//					putText(imageCopy, coords, Point(current_corners[0].x,current_corners[0].y), FONT_HERSHEY_DUPLEX, 2, b_color, 2);
	//
	//					patterns_present[i][pattern_index] = true;
	//					points_per_board[i][pattern_index] = 4;
	//
	//
	//					for (int j = 0; j < 4; j++){
	//
	//						points_present[i][4*pattern_index + j] = true;
	//						//	cout << "Writing to index " << 4*pattern_index + j << endl;
	//						two_d_point_coordinates_dense[i](4*pattern_index + j, 0) = current_corners[j].x;
	//						two_d_point_coordinates_dense[i](4*pattern_index + j, 1) = current_corners[j].y;
	//
	//						global_index = 4*pattern_index + j;
	//						out << global_index << " " << std::setprecision(9) << current_corners[j].x << " " << std::setprecision(9) << current_corners[j].y << " ";
	//
	//						if (j > 0){
	//							string coords = ToString<float>(P_class->three_d_points[4*pattern_index + j].x) + ", " + ToString<float>(P_class->three_d_points[4*pattern_index + j].y);
	//							putText(imageCopy, coords, Point(current_corners[j].x,current_corners[j].y), FONT_HERSHEY_DUPLEX, 1, b_color, 2);
	//						}
	//					}
	//				}
	//			}
	//		}
	//
	//		out << endl << "end " << i << endl;
	//
	//		filename = write_dir + "external_initial_detect" + ToString<int>(i) + ".jpg";
	//		//imwrite(filename.c_str(), imageCopy);
	//
	//
	//
	//		Mat im_small;
	//		Size si;
	//
	//		si.width = imageCopy.cols/8;
	//		si.height = imageCopy.rows/8;
	//
	//		resize(imageCopy, im_small, si);
	//
	//		imwrite(filename.c_str(), im_small);
	//
	//	}



	//	// make it easy for self ... fill in everything computed in this function.
	//	out << "Image-board truth table " << endl;
	//	for (int i = 0; i < number_images; i++){
	//		out << "Image " << i << " : ";
	//		for (int p = 0; p < number_patterns; p++){
	//			out << 	points_per_board[i][p] << " " ;
	//		}
	//		out << endl;
	//	}


	out.close();

	//char ch; cin >> ch;
}

void CameraCali::CalibrateArucoSinglyAndUndistort(string write_dir){


	/// want to recover pose after calibration ...need a map.
	vector<int> mapping_from_limited_to_full_images;
	vector<int> mapping_from_limited_to_full_patterns;
	// create a collection of the points for each image -- hopefully this will work. map -- .


	//



	int number_images = images.size();
	int number_squares = P_class->NumberSquares();
	int number_patterns = P_class->max_internal_patterns;

	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
	//int s;
	//	//	number_images = 2;
	cv::Size image_size;
	//	int last_added;
	//	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
	//

	//rows = images[0].rows;
	//cols = images[0].cols;
	string filename = write_dir + "results.txt";
	std::ofstream out;
	out.open(filename.c_str());

	/// start a text file with all of the information --- internal, distortion, external.

	//cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F);


	double homography_scaling  = 10;

	int max_x = 0;
	int max_y = 0;
	int cx, cy;
	for (int p = 0; p < number_patterns; p++){

		for (int j = 0; j < 4; j++){
			cx = P_class->three_d_points_internal[p*4 + j].x;
			cy = P_class->three_d_points_internal[p*4 + j].y;

			cx > max_x ? max_x = cx : 0;
			cy > max_y ? max_y = cy : 0;

		}
	}

	cout << "max_x, max_y " << max_x <<", " << max_y << endl;

	Mat img1_warp = cv::Mat::zeros(homography_scaling*max_y, homography_scaling*max_x, CV_8UC3);

	for (int i = 0; i < number_images; i++){

		/// each image is its own world... independent of the others in the set b/c of focal length changes.
		vector< vector< cv::Point2f> > twod_points_wo_blanks;
		vector< vector< cv::Point3f> > threed_points_wo_blanks;
		vector< cv::Point2f> caliObjectPointsPlanar;

		out << "image " << i << endl;
		out << "focal length  " << focal_lengths[i] << endl;


		cout << "i "  << i << endl;
		rows = images[i].rows;
		cols = images[i].cols;
		int last_added = 0;
		int s = 0;
		int total_needed = 0;;
		for (int p = 0; p < number_patterns; p++){
			if (patterns_present[i][p] == true){
				total_needed++;
			}
		}

		twod_points_wo_blanks.push_back(vector< cv::Point2f>(total_needed*4));
		threed_points_wo_blanks.push_back(vector< cv::Point3f>(total_needed*4));



		for (int p = 0; p < number_patterns; p++){
			if (patterns_present[i][p] == true){ /// 4 points per board.  Will this be enough?  We'll see.
				{
					//cout << "Pattern " << p << " last_added " << last_added << endl;
					mapping_from_limited_to_full_images.push_back(i);
					mapping_from_limited_to_full_patterns.push_back(p);

					/// each board is a new observation, as is each image.  Get to finer resolution-figuring or transformations later if there are two on the same image.


					/// then, walk through all of the possibles ONLY AT THIS PATTERN/BOARD.
					//s = 0;
					//cout << "LINE 430: Size of current two d " << twod_points_wo_blanks[last_added].size() << " Total samples " << last_added << endl;
					for (int j = 0; j < 4; j++){
						//cout << "Pattern indices in calibrate " << p*4 + j << endl;
						twod_points_wo_blanks[0][s].x = internal_two_d_point_coordinates_dense[i](p*4 + j, 0);    /// twod points w/o blanks is NOT per image to make internal cali work.
						twod_points_wo_blanks[0][s].y = internal_two_d_point_coordinates_dense[i](p*4 + j, 1);

						threed_points_wo_blanks[0][s].x = P_class->three_d_points_internal[p*4 + j].x;
						threed_points_wo_blanks[0][s].y = P_class->three_d_points_internal[p*4 + j].y;
						threed_points_wo_blanks[0][s].z = P_class->three_d_points_internal[p*4 + j].z;

						s++;
					}
				}
			}
		}

		caliObjectPointsPlanar.resize(total_needed*4, cv::Point2f());

		for (int s0 = 0, sn = threed_points_wo_blanks[0].size(); s0 < sn; s0++){

			caliObjectPointsPlanar[s0].x = threed_points_wo_blanks[0][s0].x*homography_scaling;
			caliObjectPointsPlanar[s0].y = (max_y - threed_points_wo_blanks[0][s0].y)*homography_scaling;

		}

		/// have loaded everything ... now get ready to calibrate ...


		image_size = Size(cols, rows);


		cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);

		cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);

		cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F);


		vector<cv::Mat> rvecs, tvecs;

		vector<cv::Mat> rvecs_internal, tvecs_internal;

		cameraMatrix.at<double>(0, 0) = pixel_width;
		cameraMatrix.at<double>(1, 1) = pixel_width;;

		cameraMatrix.at<double>(0, 2) = cols/2;
		cameraMatrix.at<double>(1, 2) = rows/2;

		//	cout << "initial camera matrix " << endl;
		//
		//	for (int i = 0; i < 3; i++){
		//		for (int j = 0; j < 3; j++){
		//			cout << cameraMatrix.at<double>(i, j) << " ";
		//		}
		//		cout << endl;
		//	}

		cout << "Running calibration " << endl;
		//cout << "Number of dist coefficients  = " << distCoeffs.rows << endl;

		double rms = 0;
		char ch;

		rms = calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs_internal, tvecs_internal,
				CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_PRINCIPAL_POINT | CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
				CALIB_FIX_FOCAL_LENGTH);

		cout << "rms " << rms << endl;


		cv::Mat rv; cv::Mat tv;
		solvePnP(threed_points_wo_blanks[0], twod_points_wo_blanks[0], cameraMatrix, distCoeffs, rv, tv, false);

		cout << "After external" << endl;


		rvecs.push_back(rv);
		tvecs.push_back(tv);

		Matrix4d Ext; Ext.setIdentity();

		cv::Rodrigues(rv, rotMatrix);


		for (int r = 0; r < 3; r++){
			for (int c = 0; c < 3; c++){
				Ext(r, c) = rotMatrix.at<double>(r, c);
			}

			Ext(r, 3) = tv.at<double>(r);
		}

		for (int r = 0; r < 3; r++){
			for (int c = 0; c < 3; c++){
				internal_parameters(r, c) = cameraMatrix.at<double>(r, c);
			}
		}

		distortion.resize(distCoeffs.rows);
		for (int r = 0; r < distCoeffs.rows; r++){
			distortion(r) = distCoeffs.at<double>(r, 0);

		}

		out << "rms " << rms << endl;
		out << "K" << endl << internal_parameters << endl;
		out << "dist " << endl << distortion << endl;
		out << "external" << Ext << endl;

		// undistort
		cv::Mat view, rview, map1, map2;
		Mat imageCopy = images[i].clone();
		//	cv::Mat gray;
		string filename;
		cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
				cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0),
				image_size, CV_16SC2, map1, map2);


		cv::remap(imageCopy, rview, map1, map2, cv::INTER_LINEAR);
		/// here -- also need to undistort the points -- then homography.  In which space to do this, since the image is so big??

		filename  = write_dir + "/undistorted" + ToString<int>(i) + ".jpg";
		cv::imwrite(filename.c_str(), rview);

		// homography /////////////////////

		//cv::Mat newCameraMatrix = cv::Mat::eye(3, 3, CV_64F);
		vector< cv::Point2f> undistorted_points;
		Mat newCameraMatrix = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0);


		undistortPoints(twod_points_wo_blanks[0], undistorted_points, cameraMatrix, distCoeffs, cv::noArray(), newCameraMatrix);

//		for (int m = 0; m < int(undistorted_points.size() ); m++){
//
//			drawMarker(rview, undistorted_points[m], Scalar(255, 0,0), 0, 20, 20, 0);
//
//		}



		// H will find the homography AKA transformation onto the plane where pixels == some representation in mm.
		cout << "matches " << endl;
		for (int m = 0; m < 4; m++){
			cout << "i" << i << " " << undistorted_points[m].x << "," << undistorted_points[m].y << " 3d ";
			cout << caliObjectPointsPlanar[m].x << ", " << caliObjectPointsPlanar[m].y << endl;


		//			drawMarker(rview, undistorted_points[m], Scalar(255, 0,0), 0, 20, 20, 0);

		}

		Mat H = findHomography(undistorted_points, caliObjectPointsPlanar);
		out << "H:" << endl << H << endl;

//		// Normalization to ensure that ||c1|| = 1
//		double norm = sqrt(H.at<double>(0,0)*H.at<double>(0,0) +
//		H.at<double>(1,0)*H.at<double>(1,0) +
//		H.at<double>(2,0)*H.at<double>(2,0));
//		H /= norm;

		/// this image needs to be as big as the 3d space... -- in other words, the largest coordinate in the 3d coords.

		warpPerspective(rview, img1_warp, H, img1_warp.size());
		filename  = write_dir + "/warped" + ToString<int>(i) + ".jpg";
		cv::imwrite(filename.c_str(), img1_warp);
		// undistortPoints(IP[0], undistorted_points, cameraMatrix, distCoeffs, cv::noArray(), cameraMatrix);



	}

out.close();

//	vector<vector< cv::Point2f> > twod_points_wo_blanks_internal;
//	vector< vector< cv::Point3f> > threed_points_wo_blanks_internal;
//
//
//
//	twod_points_wo_blanks_internal.push_back(vector< cv::Point2f>(count_internal_ids_present*4));
//	threed_points_wo_blanks_internal.push_back(vector< cv::Point3f>(count_internal_ids_present*4));
//	// then, we can also do the rest of the external items too, and grab all of that good data.  For now, just internals.
//	cout << "Filled in ... " << count_internal_ids_present*4 << endl;
//
//	/// the first one will be from the internal pattern ....
//
//	last_added = twod_points_wo_blanks.size();
//	last_added--;
//
//	last_added = 0;
//	s = 0;
//	for (int i = 0; i < P_class->max_internal_patterns; i++){
//		if (id_bool[i] == true){
//			for (int j =0 ; j < 4; j++){
//				//twod_points_wo_blanks[last_added][i].x;
//				twod_points_wo_blanks_internal[last_added][s].x = internal_two_d_point_coordinates_dense[0](i*4 + j, 0);
//				twod_points_wo_blanks_internal[last_added][s].y = internal_two_d_point_coordinates_dense[0](i*4 + j, 1);
//
//				threed_points_wo_blanks_internal[last_added][s].x = P_class->three_d_points_internal[i*4 + j].x;
//				threed_points_wo_blanks_internal[last_added][s].y = P_class->three_d_points_internal[i*4 + j].y;
//				threed_points_wo_blanks_internal[last_added][s].z = P_class->three_d_points_internal[i*4 + j].z;
//				s++;
//			}
//		}
//	}
//
//	//twod_points_wo_blanks_internal.push_back(twod_points_wo_blanks[twod_points_wo_blanks.size() - 1]);
//	//threed_points_wo_blanks_internal.push_back(threed_points_wo_blanks[twod_points_wo_blanks.size() - 1]);
//
//
//
//
//	image_size = Size(cols, rows);
//
//
//	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
//
//	cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F);
//
//
//	vector<cv::Mat> rvecs, tvecs;
//
//	vector<cv::Mat> rvecs_internal, tvecs_internal;
//
//	cameraMatrix.at<double>(0, 0) = pixel_width;
//	cameraMatrix.at<double>(1, 1) = pixel_width;;
//
//	cameraMatrix.at<double>(0, 2) = cols/2;
//	cameraMatrix.at<double>(1, 2) = rows/2;
//
//	//	cout << "initial camera matrix " << endl;
//	//
//	//	for (int i = 0; i < 3; i++){
//	//		for (int j = 0; j < 3; j++){
//	//			cout << cameraMatrix.at<double>(i, j) << " ";
//	//		}
//	//		cout << endl;
//	//	}
//
//	cout << "Running calibration " << endl;
//	//cout << "Number of dist coefficients  = " << distCoeffs.rows << endl;
//
//	double rms = 0;
//	char ch;
//	//	cout << "Before first " << endl; cin >> ch;
//	//	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//							CV_CALIB_RATIONAL_MODEL);
//
//
//	//if (text_file.size() == 0){
//
//	// experiment -- write input
//	//rms = cv::calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);
//
//	//cout << "rms " << rms << endl;
//
//
//	//rms = calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//		CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_PRINCIPAL_POINT | CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
//	//		CALIB_FIX_FOCAL_LENGTH);
//
//	rms = calibrateCamera(threed_points_wo_blanks_internal, twod_points_wo_blanks_internal, image_size, cameraMatrix, distCoeffs, rvecs_internal, tvecs_internal,
//			CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_PRINCIPAL_POINT | CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
//			CALIB_FIX_FOCAL_LENGTH);
//
//	//rms = calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//					CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
//	//					CALIB_FIX_FOCAL_LENGTH);
//
//	cout << "rms " << rms << endl;
//
//	// want to reproject and see how bad it is ... this is not really giving us that satisfaction.
//	int number_from_external = twod_points_wo_blanks.size();
//
//	cout << "Before external" << endl;
//	for (int i = 0; i < number_from_external; i++){
//		cv::Mat rv; cv::Mat tv;
//		solvePnP(threed_points_wo_blanks[i], twod_points_wo_blanks[i], cameraMatrix, distCoeffs, rv, tv, false);
//
//		rvecs.push_back(rv);
//		tvecs.push_back(tv);
//
//	}
//
//	cout << "After external" << endl;
//	rvecs.push_back(rvecs_internal[0]);
//	tvecs.push_back(tvecs_internal[0]);
//
//
//	//bool solvePnP(InputArray objectPoints, InputArray imagePoints, InputArray cameraMatrix, InputArray distCoeffs, OutputArray rvec, OutputArray tvec, bool useExtrinsicGuess=false, int flags=SOLVEPNP_ITERATIVE )
//	//SOLVEPNP_ITERATIVE Iterative method is based on Levenberg-Marquardt optimization. In this case the function finds such a pose that minimizes reprojection error, that is the sum of squared distances between the observed projections imagePoints and the projected (using projectPoints() ) objectPoints .
//	//SOLVEPNP_P3P Method is based on the paper of X.S. Gao, X.-R. Hou, J. Tang, H.-F. Chang “Complete Solution Classification for the Perspective-Three-Point Problem”. In this case the function requires exactly four object and image points.
//
//
//
//
//	/// write calibration details now.  Also, transfer to the Eigen format.
//
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			internal_parameters(i, j) = cameraMatrix.at<double>(i, j);
//		}
//	}
//
//	distortion.resize(distCoeffs.rows);
//	for (int i = 0; i < distCoeffs.rows; i++){
//		distortion(i) = distCoeffs.at<double>(i, 0);
//
//	}
//
//	cout << "distortion " << distortion << endl;
//	cout << "Camera matrix " << endl << cameraMatrix << endl;
//
//	cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);
//	vector< vector <double> > tempRt(3, vector<double>(4, 0));
//
//
//	double reproj_error = 0;
//	vector<cv::Point2f> imagePoints2;
//	double err;
//	int correct_image;
//	int correct_pattern;
//
//	for (int i = 0; i < number_images; i++){
//		reproject_cam_cali_images.push_back(images[i].clone());
//	}
//
//	//vector< vector<cv::Point2f> > reproj_image_points;
//
//	// TODO keep these repo errors per image/per pattern ... then do not need to redo.
//	// These will form a basis of which hypotheses to use first when we start tightening things up.
//	// ESPECIALLY when there are two or more patterns per frame.
//	//
//
//	/// internal only
//	int number_cali_items = twod_points_wo_blanks.size();
//	for (int m = number_cali_items - 1; m < number_cali_items; m++){
//
//		cv::projectPoints( cv::Mat(threed_points_wo_blanks[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
//				distCoeffs, imagePoints2);
//		err = cv::norm(cv::Mat(twod_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
//		reproj_error        += err*err;
//
//		//reproj_image_points.push_back(imagePoints2);
//		//		correct_image = mapping_from_limited_to_full_images[m];
//		//		correct_pattern = mapping_from_limited_to_full_patterns[m];
//		//		reproj_error_per_board[correct_image][correct_pattern] = err*err;
//
//		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
//			line(reproject_cam_cali_images[0], twod_points_wo_blanks[m][j],imagePoints2[j], Scalar(255, 0, 255), 6 );
//		}
//
//	}
//
//
//
//	vector<double> temp_repro(number_patterns, 0);
//	reproj_error_per_board.resize(number_images, temp_repro);
//
//	for (int m = 0; m < number_cali_items - 1; m++){
//
//		cv::projectPoints( cv::Mat(threed_points_wo_blanks[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
//				distCoeffs, imagePoints2);
//		err = cv::norm(cv::Mat(twod_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
//		reproj_error        += err*err;
//
//		//reproj_image_points.push_back(imagePoints2);
//		correct_image = mapping_from_limited_to_full_images[m];
//		correct_pattern = mapping_from_limited_to_full_patterns[m];
//		reproj_error_per_board[correct_image][correct_pattern] = err*err;
//
//		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
//			line(reproject_cam_cali_images[correct_image], twod_points_wo_blanks[m][j],imagePoints2[j], Scalar(255, 0, 255), 10 );
//		}
//
//	}
//
//
//
//	////////////////////// External -- write into class variables ///////////////////////////////////////////
//	/// need to prep the matrix of rotations ...
//	Matrix4d I;  I.setIdentity();
//
//	// initialize
//	vector<Matrix4d> patterns_base(number_patterns, I);
//	// whether or not the board is present tells us whether to look at the value there.
//	external_parameters.resize(number_images, patterns_base);
//
//	int image_index;
//	int pattern_index;
//
//
//
//	/// convert from the calibration to saved values.
//	/// So the information is going to be with respect treating each observation as if it is from individual images.
//	// Here, we are going to rearrange.
//	for (int stre = 0, stre_total = number_cali_items - 1; stre < stre_total; stre++){
//
//		cv::Rodrigues(rvecs[stre], rotMatrix);
//
//		image_index = mapping_from_limited_to_full_images[stre];
//		pattern_index = mapping_from_limited_to_full_patterns[stre];
//
//
//		for (int i = 0; i < 3; i++){
//			for (int j = 0; j < 3; j++){
//				external_parameters[image_index][pattern_index](i, j) = rotMatrix.at<double>(i, j);
//			}
//
//			external_parameters[image_index][pattern_index](i, 3) = tvecs[stre].at<double>(i);
//		}
//	}
//
//
//	//	/////////////////////////////// UNDISTORT, WRITE REPROJECTION ////////////////////////////////////
//	cv::Mat view, rview, map1, map2;
//	//	cv::Mat gray;
//	string filename;
//	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
//			cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0),
//			image_size, CV_16SC2, map1, map2);
//
//	for (int i = 0; i < number_images; i++){
//		cout << "Writing external " << i << endl;
//		cv::remap(reproject_cam_cali_images[i], rview, map1, map2, cv::INTER_LINEAR);
//
//		if (i > 1){
//			filename  = write_dir + "/ext" + ToString<int>(i) + ".jpg";
//			cv::imwrite(filename.c_str(), rview);
//
//			Mat im_small;
//			Size si;
//
//			si.width = rview.cols/8;
//			si.height = rview.rows/8;
//
//			resize(rview, im_small, si);
//
//			imwrite(filename.c_str(), im_small);
//		}
//	}
//
//
//	cout << "internal parameters in function : " << internal_parameters << endl;
//	///////////////////////////////// WRITE CALIBRATION INFORMATION ///////////////////////////
//	std::ofstream out;
//	filename = write_dir + "cali_results.txt";
//	out.open(filename.c_str());
//
//	out << "Number_patterns " << number_cali_items - 1 << endl;
//	out << "rms " << rms << endl;
//	out << "internal_matrix " << endl;
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			out << internal_parameters(i, j) << " ";
//		}
//		out << endl;
//	}
//
//
//	//<< internal_parameters << endl;
//	out << "distortion_size " << distortion.rows() << endl;
//	out << "distortion_vector " << endl << distortion << endl;
//
//
//	for (int stre = 0, stre_total = number_cali_items - 1; stre < stre_total; stre++){
//		image_index = mapping_from_limited_to_full_images[stre];
//		pattern_index = mapping_from_limited_to_full_patterns[stre];
//
//		out << "EXTERNAL image, pattern, reproj " << image_index << " " << pattern_index << " " << reproj_error_per_board[image_index][pattern_index] << endl;
//		out << external_parameters[image_index][pattern_index] << endl;
//	}
//
//
//	//has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
//	out << "has-calibration-estimate " << endl;
//	for (int i = 0; i < number_images; i++){
//		for (int j = 0; j < number_patterns; j++){
//			out << has_calibration_estimate[i][j] << " ";
//		}
//		out << endl;
//	}
//	out.close();
//
//	filename = write_dir + "two_d_data.txt";
//	out.open(filename.c_str());
//	for (int m = 0; m < number_cali_items - 1; m++){
//		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
//		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
//			out << twod_points_wo_blanks[m][s].x << " " << twod_points_wo_blanks[m][s].y  << endl;
//		}
//	}
//	out.close();
//
//	filename = write_dir + "three_d_data.txt";
//	out.open(filename.c_str());
//	for (int m = 0; m < number_cali_items - 1; m++){
//		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
//		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
//			out << threed_points_wo_blanks[m][s].x << " " << threed_points_wo_blanks[m][s].y  << " " << threed_points_wo_blanks[m][s].z << endl;
//		}
//	}
//	out.close();
//
//
//	filename = write_dir + "image_points_first_image.txt";
//	out.open(filename.c_str());
//	/// external points.
//	for (int i = 0; i < 1; i++){
//		cout << "i "  << i << endl;
//		for (int p = 0; p < number_patterns; p++){
//
//			if (patterns_present[i][p] == true){ /// 4 points per board.  Will this be enough?  We'll see.
//				{
//
//					cout << "Image, Pattern " << i << ", " << p << endl;
//
//					out << "EXTERNAL " << endl;
//
//					//cout << "LINE 430: Size of current two d " << twod_points_wo_blanks[last_added].size() << " Total samples " << last_added << endl;
//					for (int j = 0; j < 4; j++){
//						out << two_d_point_coordinates_dense[i](p*4 + j, 0) << " " << two_d_point_coordinates_dense[i](p*4 + j, 1) << endl;
//					}
//				}
//			}
//		}
//	}
//
//
//
//
//	for (int i = 0; i < P_class->max_internal_patterns; i++){
//		if (id_bool[i] == true){
//			out << "INTERNAL " << i << endl;
//			for (int j =0 ; j < 4; j++){
//				//twod_points_wo_blanks[last_added][i].x;
//				out << internal_two_d_point_coordinates_dense[0](i*4 + j, 0) << " " << internal_two_d_point_coordinates_dense[0](i*4 + j, 1) << endl;
//			}
//		}
//	}
//
//	out << -1 << endl;
//	out.close();
//
//
//
//
//	images.clear();
}


void CameraCali::FindCornersCharuco(string write_dir){

	/// first, need to find the patterns.
	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();

	bool readOk = readDetectorParameters(string("../src/detector_params.yml"), detectorParams);
	if(!readOk) {
		cout << "Cannot read in Calibrate -- Invalid detector parameters file" << endl;
		exit(1);
	}

	int number_patterns = P_class->NumberPatterns();
	int number_markers = P_class->NumberMarkers();
	int number_squares = P_class->NumberSquares();

	string filename;
	int number_images = images.size();
	Mat imageCopy;
	Scalar b_color;
	MatrixXd twod(number_squares, 2);
	twod.setConstant(0);
	int global_index;

	boards_detected.resize(number_images, vector<bool>(P_class->NumberPatterns(), false));


	std::ofstream out;
	filename = write_dir + "points.txt";
	out.open(filename.c_str());

	// output something about the images channel.
	for (int i = 0; i < number_images; i++){

		out << "image " << i << endl;
		// initialize per image.
		points_present.push_back(vector<bool>(number_squares, false));
		points_per_board.push_back(vector<int>(number_patterns, 0));
		two_d_point_coordinates_dense.push_back(twod);

		//	imageCopy = images[i].clone();
		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;
		vector< Vec3d > rvecs, tvecs;

		// detect markers and estimate pose
		aruco::detectMarkers(images[i], P_class->dictionary, corners, ids, detectorParams, rejected);

		// draw results -- dealing with the possibility of more than one board per image.
		if(ids.size() > 0) {
			//we id the board by the aruco tag.
			// TODO - could be more than one board, select more than one.
			P_class->DetermineBoardsPresentFromMarkerList(ids, boards_detected[i]); // this is not really working.  edit, now it is

			// per board refinement and localization of the corners.
			for (int p = 0; p < number_patterns; p++){
				if (boards_detected[i][p] == true){

					b_color = P_class->Color(p);

					// optional, can draw the tags on.
					//aruco::drawDetectedMarkers(images[i], corners, ids, b_color);

					// want this to be fresh each iter.
					std::vector<cv::Point2f> charucoCorners;
					std::vector<int> charucoIds;

					cv::aruco::interpolateCornersCharuco(corners, ids, images[i], P_class->boards[p], charucoCorners, charucoIds);
					// if at least one charuco corner detected
					if(charucoIds.size() > 0)
					{
						//out << "Board " << p << " number " << charucoIds.size() << endl;
						points_per_board[i][p] = charucoIds.size();

						/// corners (2d) are linked to the Ids
						cv::aruco::drawDetectedCornersCharuco(images[i], charucoCorners, charucoIds, b_color);

						//cout << "Image, Board number " << i << ", " << p << endl;
						//cout << "Number of squares " << number_squares << endl;
						for (int j  = 0, jn = charucoIds.size(); j < jn; j++){
							//cout << "charucoID " << charucoIds[j] << endl;
							//cout << "Interpreted in the system .... plus min board number " << P_class->min_max_id_pattern[board_selected].first + charucoIds[j] << endl;

							global_index = P_class->min_max_id_squares[p].first + charucoIds[j];

							points_present[i][global_index] = true;
							two_d_point_coordinates_dense[i](global_index, 0) = charucoCorners[j].x;
							two_d_point_coordinates_dense[i](global_index, 1) = charucoCorners[j].y;

							out << global_index << " " << std::setprecision(9) << charucoCorners[j].x << " " << std::setprecision(9) << charucoCorners[j].y << " ";

							/// this is wrong for camera 2, i = 25, number per is somehow off -- CHANGED TO SQUARES -- probably was it.
						}
						//out << endl;
						//cout << "two d map " << endl << two_d_point_coordinates_dense[i] << endl;

						//char ch; cin >> ch;
					}	else {
						boards_detected[i][p] = false;  // sometimes there's a marker detected, but we can't grab good corners.
					}
				}
			}

			//			cout << "Boards detected bool ";
			//			for (int p = 0; p < number_patterns; p++){
			//				cout << boards_detected[i][p] << " " ;
			//			}
			//
			//			cout << "marker ids ";
			//			for (int p = 0; p < ids.size(); p++){
			//				cout << ids[p] << " " ;
			//			}
			//			cout << endl;
			//
			//			cout << "min maxes pattern ids ";
			//			for (int p = 0; p < number_patterns; p++){
			//				cout << P_class->min_max_id_pattern[p].first << " " << P_class->min_max_id_pattern[p].second << endl ;
			//			}
			//
			//			char ch; cin >> ch;
		}
		filename = write_dir + "initial_detect" + ToString<int>(i) + ".png";
		imwrite(filename.c_str(), images[i]);

		out << endl << "end " << i << endl;
	}

	// make it easy for self ... fill in everything computed in this function.
	out << "Image-board truth table " << endl;
	for (int i = 0; i < number_images; i++){
		out << "Image " << i << " : ";
		for (int p = 0; p < number_patterns; p++){
			out << 	points_per_board[i][p] << " " ;
		}
		out << endl;
	}


	out.close();

	//char ch; cin >> ch;
}


void CameraCali::ReadCorners(string read_dir){
	cout << "Starting read " << endl;
	string filename = read_dir + "points.txt";

	string filename2 = read_dir + "points_confirm.txt";
	ifstream in;
	in.open(filename.c_str());


	std::ofstream out;
	out.open(filename2.c_str());

	if(!in.good()) {
		cout << "Cannot read in camera calibration file " << filename << endl;
		cout << "And the argument specified read camera calibration parameters. Exiting " << endl;
		exit(1);
	}

	int number_patterns = P_class->NumberPatterns();
	int number_markers = P_class->NumberMarkers();
	int number_squares = P_class->NumberSquares();


	int number_images = images.size();
	Mat imageCopy;
	Scalar b_color;
	MatrixXd twod(number_squares, 2);
	twod.setConstant(0);
	int global_index;
	float x, y;

	boards_detected.resize(number_images, vector<bool>(P_class->NumberPatterns(), false));

	string temp_string0;
	string temp_string1;
	string temp_string2;

	string s_end = "end";

	bool in_image = false;

	// output something about the images channel.
	for (int i = 0; i < number_images; i++){

		out << "image " << i << endl;
		in >> temp_string0 >> temp_string1;

		// initialize per image.
		points_present.push_back(vector<bool>(number_squares, false));
		points_per_board.push_back(vector<int>(number_patterns, 0));
		two_d_point_coordinates_dense.push_back(twod);

		// while we don't have 'end' as the first of a triple ....
		in_image = true;
		do {
			//out << global_index << " " << charucoCorners[j].x << " " << charucoCorners[j].y << " ";
			in >> temp_string0;

			// is this end?

			if (temp_string0.compare(s_end) == 0){
				in_image = false;
			}	else {
				//in >> temp_string1 >> temp_string2;
				in >> x >> y;

				// convert to int, double, double.
				global_index = FromString<int>(temp_string0);
				//x = FromString<double>(temp_string1);
				//y = FromString<double>(temp_string2);
				points_present[i][global_index] = true;
				two_d_point_coordinates_dense[i](global_index, 0) = x;
				two_d_point_coordinates_dense[i](global_index, 1) = y;
				out << global_index << " " << x << " " << y << " ";
			}
		} while (in_image == true);


		in >> temp_string1; // reads in the number of image.

		out << endl << "end " << temp_string1 << endl;
	}

	// make it easy for self ... fill in everything computed in this function.
	//out << "Image-board truth table " << endl;
	in >> temp_string0 >> temp_string1 >> temp_string2;
	//cout << "Where we are in the file .... " << temp_string0 << endl;
	out << temp_string0 << " " << temp_string1 << " " << temp_string2 << endl;
	for (int i = 0; i < number_images; i++){
		//out << "Image " << i << " : ";
		in >> temp_string0 >> temp_string1 >> temp_string2;
		out << temp_string0 << " " << temp_string1 << " " << temp_string2;
		cout << temp_string0 << " " << temp_string1 << temp_string2 << endl;
		for (int p = 0; p < number_patterns; p++){
			//out << 	boards_detected[i][p] << " " ;
			in >> temp_string0;

			// this is wrong somehow.
			points_per_board[i][p] = FromString<int>(temp_string0);

			boards_detected[i][p] = points_per_board[i][p] > 0;

			//cout << points_per_board[i][p] << endl;
			out << " " << points_per_board[i][p];

		}
		out << endl;
	}



	out.close();
	in.close();

	//char ch; cin >> ch;
}

int ReturnOption(string entry, vector<string>& comps){

	for (int i = 0, in = comps.size(); i < in; i++){
		if (entry.compare(comps[i]) == 0){
			return i;
		}
	}

	return -1;
}


void CameraCali::ReadStrawberryInternalCorners(string read_dir){

	//vector<MatrixXd> two_d_point_coordinates_segmentation_external;
	//vector<MatrixXd> two_d_point_coordinates_segmentation_internal;
	cout << "Starting read " << endl;
	string filename = read_dir + "image_points_first_image.txt";

	string filename2 = read_dir + "image_points_first_image_confirm.txt";
	ifstream in;
	in.open(filename.c_str());


	//std::ofstream out;
	//out.open(filename2.c_str());

	if(!in.good()) {
		cout << "Cannot read in image points from first image " << filename << endl;
		cout << "Exiting " << endl;
		exit(1);
	}


	MatrixXd twod(4, 2);

	/// while can read in, read in.

	string EXT = "EXTERNAL";
	string INT = "INTERNAL";
	string neg1 = "-1";

	vector<string> options;  options.push_back(EXT);  options.push_back(INT); options.push_back(neg1);

	string temp;

	in >> temp;
	double x, y;
	int val;
	int internal_index;

	while (ReturnOption(temp, options) != 2){
		// read in the entries ....
		val = ReturnOption(temp, options);

		if (val == 1){
			in >> internal_index;
			internal_pattern_indices.push_back(internal_index);
		}
		for (int i = 0; i < 4; i++){
			in >> x >> y;
			twod(i, 0) = x; twod(i, 1) = y;
		}

		if (val == 0){
			two_d_point_coordinates_segmentation_external.push_back(twod);
		}	else {
			two_d_point_coordinates_segmentation_internal.push_back(twod);
		}

		in >> temp;
	}



	in.close();

	//char ch; cin >> ch;
}

void CameraCali::ReadCalibration(string read_dir){

	// need to read:: has_calibration_estimate[i][p] = true;

	//	cout << "distortion " << distortion << endl;
	//		cout << "Camera matrix " << endl << cameraMatrix << endl;
	string filename = read_dir + "cali_results.txt";
	std::ifstream in;
	in.open(filename.c_str());


	int number_images = images.size();
	int number_squares = P_class->NumberSquares();
	int number_patterns = P_class->NumberPatterns();
	// TODO
	/// need to prep the matrix of rotations ...
	Matrix4d I;  I.setIdentity();

	// initialize
	vector<Matrix4d> patterns_base(number_patterns, I);
	// whether or not the board is present tells us whether to look at the value there.
	external_parameters.resize(number_images, patterns_base);

	// TODO
	vector<double> temp_repro(number_patterns, 0);
	reproj_error_per_board.resize(number_images, temp_repro);



	string temp_string0, temp_string1, temp_string2;
	int number_points_wo_blanks;
	int dist_rows;
	int image_index, pattern_index;
	in >> temp_string0 >> number_points_wo_blanks;

	in >> temp_string0 >> temp_string1;

	in >> temp_string0;

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			in >> internal_parameters(i, j);
		}
	}

	in >> temp_string0 >> dist_rows;
	in >> temp_string0;
	distortion.resize(dist_rows);

	for (int i = 0; i < dist_rows; i++){
		in >> distortion(i);
	}

	//EXTERNAL image, pattern, reproj 0 0 4.53632
	for (int i = 0; i < number_points_wo_blanks; i++){
		//		for (int stre = 0, stre_total = int(twod_points_wo_blanks.size()); stre < stre_total; stre++){
		//			image_index = mapping_from_limited_to_full_images[stre];
		//			pattern_index = mapping_from_limited_to_full_patterns[stre];
		//
		//			out << "EXTERNAL image, pattern, reproj " << image_index << " " << pattern_index << " " << reproj_error_per_board[image_index][pattern_index] << endl;
		//			out << external_parameters[image_index][pattern_index] << endl;
		//		}
		in >> temp_string0 >> temp_string1 >> temp_string2 >> temp_string0 >> image_index >> pattern_index >> reproj_error_per_board[image_index][pattern_index];

		// read matrix.
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++){
				in >> external_parameters[image_index][pattern_index](i, j);
			}
		}


	}

	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
	in >> temp_string0;

	int read_int;
	for (int i = 0; i < number_images; i++){
		for (int j = 0; j < number_patterns; j++){

			in >> read_int;
			has_calibration_estimate[i][j] = read_int;
		}
	}

	in.close();

	cout << "Read in the calibration file -- Success!" << endl << internal_parameters << endl;
}

//void CameraCali::CalibrateBasic(string write_dir){
//
//
//	/// want to recover pose after calibration ...need a map.
//	vector<int> mapping_from_limited_to_full_images;
//	vector<int> mapping_from_limited_to_full_patterns;
//	// create a collection of the points for each image -- hopefully this will work. map -- .
//	vector< vector< cv::Point2f> > twod_points_wo_blanks;
//	vector< vector< cv::Point3f> > threed_points_wo_blanks;
//
//
//
//	int number_images = images.size();
//	int number_squares = P_class->NumberSquares();
//	int number_patterns = P_class->NumberPatterns();
//	int s;
//	//	number_images = 2;
//	cv::Size image_size;
//	int last_added;
//	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
//
//	int rows = 0; int cols;
//	for (int i = 0; i < number_images; i++){
//		cout << "i "  << i << endl;
//		rows = images[i].rows;
//		cols = images[i].cols;
//		for (int p = 0; p < number_patterns; p++){
//
//			if (points_per_board[i][p] > 10){ // opencv calibration, may change for own calibration later.  For this stage, only use high-quality poses.
//				//if (boards_detected[i][p] == true)
//				{   // why both?
//					has_calibration_estimate[i][p] = true;
//					cout << "Image, Pattern " << i << ", " << p << endl;
//					cout << "Points per " << points_per_board[i][p] << endl;
//					//cout << "Board number " << p << " detected" << endl;
//					mapping_from_limited_to_full_images.push_back(i);
//					mapping_from_limited_to_full_patterns.push_back(p);
//
//					/// each board is a new observation, as is each image.  Get to finer resolution-figuring or transformations later if there are two on the same image.
//					twod_points_wo_blanks.push_back(vector< cv::Point2f>(points_per_board[i][p]));
//					threed_points_wo_blanks.push_back(vector< cv::Point3f>(points_per_board[i][p]));
//
//					/// then, walk through all of the possibles ONLY AT THIS PATTERN/BOARD.
//					s = 0;
//					last_added = twod_points_wo_blanks.size();
//					last_added--;
//					cout << "LINE 430: Size of current two d " << twod_points_wo_blanks[last_added].size() << endl;
//					for (int j = P_class->min_max_id_squares[p].first; j <= P_class->min_max_id_squares[p].second; j++){
//						if (points_present[i].at(j) == true){
//							//cout << "s, j " << s << ", " << j << endl;
//							/// invalid write here .   Check it out.
//							twod_points_wo_blanks[last_added][s].x = two_d_point_coordinates_dense[i](j, 0);    /// twod points w/o blanks is NOT per image to make internal cali work.
//							twod_points_wo_blanks[last_added][s].y = two_d_point_coordinates_dense[i](j, 1);
//
//							threed_points_wo_blanks[last_added][s].x = P_class->three_d_points[j].x;
//							threed_points_wo_blanks[last_added][s].y = P_class->three_d_points[j].y;
//							threed_points_wo_blanks[last_added][s].z = P_class->three_d_points[j].z;
//
//							s++;
//						}
//					}
//				}
//			}
//		}
//	}
//
//	// keep around for debugging.
//	//twod_points_wo_blanks_class = twod_points_wo_blanks;
//	//threed_points_wo_blanks_class = threed_points_wo_blanks;
//
//	image_size = Size(cols, rows);
//
//
//	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
//
//	cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F);
//
//
//	vector<cv::Mat> rvecs, tvecs;
//
//	cameraMatrix.at<double>(0, 0) = 1000;
//	cameraMatrix.at<double>(1, 1) = 1000;
//
//	cameraMatrix.at<double>(0, 2) = cols/2;
//	cameraMatrix.at<double>(1, 2) = rows/2;
//
//	//	cout << "initial camera matrix " << endl;
//	//
//	//	for (int i = 0; i < 3; i++){
//	//		for (int j = 0; j < 3; j++){
//	//			cout << cameraMatrix.at<double>(i, j) << " ";
//	//		}
//	//		cout << endl;
//	//	}
//
//	cout << "Running calibration " << endl;
//	//cout << "Number of dist coefficients  = " << distCoeffs.rows << endl;
//
//	double rms = 0;
//	char ch;
//	//	cout << "Before first " << endl; cin >> ch;
//	//	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//							CV_CALIB_RATIONAL_MODEL);
//
//
//	//if (text_file.size() == 0){
//
//
//	rms = cv::calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);
//
//	cout << "rms " << rms << endl;
//
//	// option to read in the calibration information ....
//	//	}	else {
//	//		ifstream in(text_file.c_str());
//	//		string temp;
//	//		in >> cameraMatrix.at<double>(0, 0);
//	//		in >> temp;
//	//		in >> cameraMatrix.at<double>(0, 2);
//	//		in >> temp;
//	//		in >> cameraMatrix.at<double>(1, 1);
//	//		in >> cameraMatrix.at<double>(1, 2);
//	//		in >> temp >> temp >> temp;
//	//		in.close();
//	//
//	//		cout << "initial camera matrix " << endl;
//	//
//	//		for (int i = 0; i < 3; i++){
//	//			for (int j = 0; j < 3; j++){
//	//				cout << cameraMatrix.at<double>(i, j) << " ";
//	//			}
//	//			cout << endl;
//	//		}
//	//
//	//		//		cout << "Before first " << endl; cin >> ch;
//	//		//		rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//		//						CV_CALIB_RATIONAL_MODEL);
//	//
//	//		//cout << "Before second " << endl; cin >> ch;
//	//		// submitted Transactions paper has rational model
//	//		//rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//		//		CV_CALIB_RATIONAL_MODEL| CV_CALIB_USE_INTRINSIC_GUESS);
//	//
//	//		//	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//		//					CV_CALIB_USE_INTRINSIC_GUESS);
//	//		// OpenCV versions
//	//		rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//				CALIB_USE_INTRINSIC_GUESS);
//	//	}
//
//
//	/// write calibration details now.  Also, transfer to the Eigen format.
//
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			internal_parameters(i, j) = cameraMatrix.at<double>(i, j);
//		}
//	}
//
//	distortion.resize(distCoeffs.rows);
//	for (int i = 0; i < distCoeffs.rows; i++){
//		distortion(i) = distCoeffs.at<double>(i, 0);
//
//	}
//
//	cout << "distortion " << distortion << endl;
//	cout << "Camera matrix " << endl << cameraMatrix << endl;
//
//	cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);
//	vector< vector <double> > tempRt(3, vector<double>(4, 0));
//
//
//	double reproj_error = 0;
//	vector<cv::Point2f> imagePoints2;
//	double err;
//	int correct_image;
//	int correct_pattern;
//
//	for (int i = 0; i < number_images; i++){
//		reproject_cam_cali_images.push_back(images[i].clone());
//	}
//
//	//vector< vector<cv::Point2f> > reproj_image_points;
//
//	// TODO keep these repo errors per image/per pattern ... then do not need to redo.
//	// These will form a basis of which hypotheses to use first when we start tightening things up.
//	// ESPECIALLY when there are two or more patterns per frame.
//	vector<double> temp_repro(number_patterns, 0);
//	reproj_error_per_board.resize(number_images, temp_repro);
//
//	for (int m = 0; m < int(twod_points_wo_blanks.size()); m++){
//
//		cv::projectPoints( cv::Mat(threed_points_wo_blanks[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
//				distCoeffs, imagePoints2);
//		err = cv::norm(cv::Mat(twod_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
//		reproj_error        += err*err;
//
//		//reproj_image_points.push_back(imagePoints2);
//		correct_image = mapping_from_limited_to_full_images[m];
//		correct_pattern = mapping_from_limited_to_full_patterns[m];
//		reproj_error_per_board[correct_image][correct_pattern] = err*err;
//
//		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
//			line(reproject_cam_cali_images[correct_image], twod_points_wo_blanks[m][j],imagePoints2[j], Scalar(255, 0, 255), 2 );
//		}
//
//	}
//
//
//
//	////////////////////// External -- write into class variables ///////////////////////////////////////////
//	/// need to prep the matrix of rotations ...
//	Matrix4d I;  I.setIdentity();
//
//	// initialize
//	vector<Matrix4d> patterns_base(number_patterns, I);
//	// whether or not the board is present tells us whether to look at the value there.
//	external_parameters.resize(number_images, patterns_base);
//
//	int image_index;
//	int pattern_index;
//
//
//
//	/// convert from the calibration to saved values.
//	/// So the information is going to be with respect treating each observation as if it is from individual images.
//	// Here, we are going to rearrange.
//	for (int stre = 0, stre_total = int(twod_points_wo_blanks.size()); stre < stre_total; stre++){
//
//		cv::Rodrigues(rvecs[stre], rotMatrix);
//
//		image_index = mapping_from_limited_to_full_images[stre];
//		pattern_index = mapping_from_limited_to_full_patterns[stre];
//
//
//		for (int i = 0; i < 3; i++){
//			for (int j = 0; j < 3; j++){
//				external_parameters[image_index][pattern_index](i, j) = rotMatrix.at<double>(i, j);
//			}
//
//			external_parameters[image_index][pattern_index](i, 3) = tvecs[stre].at<double>(i);
//		}
//	}
//
//
//	/////////////////////////////// UNDISTORT, WRITE REPROJECTION ////////////////////////////////////
//	cv::Mat view, rview, map1, map2;
//	//	cv::Mat gray;
//	string filename;
//	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
//			cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0),
//			image_size, CV_16SC2, map1, map2);
//
//	for (int i = 0; i < number_images; i++){
//		cout << "Writing external " << i << endl;
//		cv::remap(reproject_cam_cali_images[i], rview, map1, map2, cv::INTER_LINEAR);
//
//		filename  = write_dir + "/ext" + ToString<int>(i) + ".png";
//		cv::imwrite(filename.c_str(), rview);
//	}
//
//
//	cout << "international parameters in function : " << internal_parameters << endl;
//	///////////////////////////////// WRITE CALIBRATION INFORMATION ///////////////////////////
//	std::ofstream out;
//	filename = write_dir + "cali_results.txt";
//	out.open(filename.c_str());
//
//	out << "Number_patterns " << twod_points_wo_blanks.size() << endl;
//	out << "rms " << rms << endl;
//	out << "internal_matrix " << endl;
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			out << internal_parameters(i, j) << " ";
//		}
//		out << endl;
//	}
//
//
//	//<< internal_parameters << endl;
//	out << "distortion_size " << distortion.rows() << endl;
//	out << "distortion_vector " << endl << distortion << endl;
//
//
//	for (int stre = 0, stre_total = int(twod_points_wo_blanks.size()); stre < stre_total; stre++){
//		image_index = mapping_from_limited_to_full_images[stre];
//		pattern_index = mapping_from_limited_to_full_patterns[stre];
//
//		/// we only have to write until the imageindex of max is
//
//		//if (image_index < number_external_images_max){
//		out << "EXTERNAL image, pattern, reproj " << image_index << " " << pattern_index << " " << reproj_error_per_board[image_index][pattern_index] << endl;
//		out << external_parameters[image_index][pattern_index] << endl;
//		//}
//	}
//
//
//	//has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
//	out << "has-calibration-estimate " << endl;
//	for (int i = 0; i < number_images; i++){
//	//for (int i = 0; i < number_external_images_max; i++){
//		for (int j = 0; j < number_patterns; j++){
//			out << has_calibration_estimate[i][j] << " ";
//		}
//		out << endl;
//	}
//	out.close();
//
//	filename = write_dir + "two_d_data.txt";
//	out.open(filename.c_str());
//	for (int m = 0; m < int(twod_points_wo_blanks.size()); m++){
//		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
//		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
//			out << twod_points_wo_blanks[m][s].x << " " << twod_points_wo_blanks[m][s].y  << endl;
//		}
//	}
//	out.close();
//
//	filename = write_dir + "three_d_data.txt";
//	out.open(filename.c_str());
//	for (int m = 0; m < int(twod_points_wo_blanks.size()); m++){
//		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
//		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
//			out << threed_points_wo_blanks[m][s].x << " " << threed_points_wo_blanks[m][s].y  << " " << threed_points_wo_blanks[m][s].z << endl;
//		}
//	}
//	out.close();
//
//}


void CameraCali::CalibrateBasic(string write_dir){


	/// want to recover pose after calibration ...need a map.
	vector<int> mapping_from_limited_to_full_images;
	vector<int> mapping_from_limited_to_full_patterns;
	// create a collection of the points for each image -- hopefully this will work. map -- .
	vector< vector< cv::Point2f> > twod_points_wo_blanks;
	vector< vector< cv::Point3f> > threed_points_wo_blanks;



	int number_images = images.size();
	int number_squares = P_class->NumberSquares();
	int number_patterns = P_class->NumberPatterns();
	int s;
	//	number_images = 2;
	cv::Size image_size;
	int last_added;
	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));

	int rows = 0; int cols;
	for (int i = 0; i < number_images; i++){
		cout << "i "  << i << endl;
		rows = images[i].rows;
		cols = images[i].cols;
		for (int p = 0; p < number_patterns; p++){

			if (points_per_board[i][p] > 10){ // opencv calibration, may change for own calibration later.  For this stage, only use high-quality poses.
				//if (boards_detected[i][p] == true)
				{   // why both?
					has_calibration_estimate[i][p] = true;
					//cout << "Image, Pattern " << i << ", " << p << endl;
					cout << "Points per " << points_per_board[i][p] << endl;
					//cout << "Board number " << p << " detected" << endl;
					mapping_from_limited_to_full_images.push_back(i);
					mapping_from_limited_to_full_patterns.push_back(p);

					/// each board is a new observation, as is each image.  Get to finer resolution-figuring or transformations later if there are two on the same image.
					twod_points_wo_blanks.push_back(vector< cv::Point2f>(points_per_board[i][p]));
					threed_points_wo_blanks.push_back(vector< cv::Point3f>(points_per_board[i][p]));

					/// then, walk through all of the possibles ONLY AT THIS PATTERN/BOARD.
					s = 0;
					last_added = twod_points_wo_blanks.size();
					last_added--;
					//	cout << "LINE 430: Size of current two d " << twod_points_wo_blanks[last_added].size() << endl;
					for (int j = P_class->min_max_id_squares[p].first; j <= P_class->min_max_id_squares[p].second; j++){
						if (points_present[i].at(j) == true){
							//cout << "s, j " << s << ", " << j << endl;
							/// invalid write here .   Check it out.
							twod_points_wo_blanks[last_added][s].x = two_d_point_coordinates_dense[i](j, 0);    /// twod points w/o blanks is NOT per image to make internal cali work.
							twod_points_wo_blanks[last_added][s].y = two_d_point_coordinates_dense[i](j, 1);

							threed_points_wo_blanks[last_added][s].x = P_class->three_d_points[j].x;
							threed_points_wo_blanks[last_added][s].y = P_class->three_d_points[j].y;
							threed_points_wo_blanks[last_added][s].z = P_class->three_d_points[j].z;

							s++;
						}
					}
				}
			}
		}
	}

	// keep around for debugging.
	//twod_points_wo_blanks_class = twod_points_wo_blanks;
	//threed_points_wo_blanks_class = threed_points_wo_blanks;

	image_size = Size(cols, rows);


	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);

	cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F);


	vector<cv::Mat> rvecs, tvecs;

	cameraMatrix.at<double>(0, 0) = 1000;
	cameraMatrix.at<double>(1, 1) = 1000;

	cameraMatrix.at<double>(0, 2) = cols/2;
	cameraMatrix.at<double>(1, 2) = rows/2;

	//	cout << "initial camera matrix " << endl;
	//
	//	for (int i = 0; i < 3; i++){
	//		for (int j = 0; j < 3; j++){
	//			cout << cameraMatrix.at<double>(i, j) << " ";
	//		}
	//		cout << endl;
	//	}

	cout << "Running calibration " << endl;
	//cout << "Number of dist coefficients  = " << distCoeffs.rows << endl;

	double rms = 0;
	char ch;
	//	cout << "Before first " << endl; cin >> ch;
	//	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
	//							CV_CALIB_RATIONAL_MODEL);


	//if (text_file.size() == 0){


	rms = cv::calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);

	cout << "rms " << rms << endl;

	// option to read in the calibration information ....
	//	}	else {
	//		ifstream in(text_file.c_str());
	//		string temp;
	//		in >> cameraMatrix.at<double>(0, 0);
	//		in >> temp;
	//		in >> cameraMatrix.at<double>(0, 2);
	//		in >> temp;
	//		in >> cameraMatrix.at<double>(1, 1);
	//		in >> cameraMatrix.at<double>(1, 2);
	//		in >> temp >> temp >> temp;
	//		in.close();
	//
	//		cout << "initial camera matrix " << endl;
	//
	//		for (int i = 0; i < 3; i++){
	//			for (int j = 0; j < 3; j++){
	//				cout << cameraMatrix.at<double>(i, j) << " ";
	//			}
	//			cout << endl;
	//		}
	//
	//		//		cout << "Before first " << endl; cin >> ch;
	//		//		rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
	//		//						CV_CALIB_RATIONAL_MODEL);
	//
	//		//cout << "Before second " << endl; cin >> ch;
	//		// submitted Transactions paper has rational model
	//		//rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
	//		//		CV_CALIB_RATIONAL_MODEL| CV_CALIB_USE_INTRINSIC_GUESS);
	//
	//		//	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
	//		//					CV_CALIB_USE_INTRINSIC_GUESS);
	//		// OpenCV versions
	//		rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
	//				CALIB_USE_INTRINSIC_GUESS);
	//	}


	/// write calibration details now.  Also, transfer to the Eigen format.

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			internal_parameters(i, j) = cameraMatrix.at<double>(i, j);
		}
	}

	distortion.resize(distCoeffs.rows);
	for (int i = 0; i < distCoeffs.rows; i++){
		distortion(i) = distCoeffs.at<double>(i, 0);

	}

	cout << "distortion " << distortion << endl;
	cout << "Camera matrix " << endl << cameraMatrix << endl;

	cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);
	vector< vector <double> > tempRt(3, vector<double>(4, 0));


	double reproj_error = 0;
	vector<cv::Point2f> imagePoints2;
	double err;
	int correct_image;
	int correct_pattern;

	for (int i = 0; i < number_images; i++){
		reproject_cam_cali_images.push_back(images[i].clone());
	}

	//vector< vector<cv::Point2f> > reproj_image_points;

	// TODO keep these repo errors per image/per pattern ... then do not need to redo.
	// These will form a basis of which hypotheses to use first when we start tightening things up.
	// ESPECIALLY when there are two or more patterns per frame.
	vector<double> temp_repro(number_patterns, 0);
	reproj_error_per_board.resize(number_images, temp_repro);

	for (int m = 0; m < int(twod_points_wo_blanks.size()); m++){

		cv::projectPoints( cv::Mat(threed_points_wo_blanks[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
				distCoeffs, imagePoints2);
		err = cv::norm(cv::Mat(twod_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
		reproj_error        += err*err;

		//reproj_image_points.push_back(imagePoints2);
		correct_image = mapping_from_limited_to_full_images[m];
		correct_pattern = mapping_from_limited_to_full_patterns[m];
		reproj_error_per_board[correct_image][correct_pattern] = err*err;

		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
			line(reproject_cam_cali_images[correct_image], twod_points_wo_blanks[m][j],imagePoints2[j], Scalar(255, 0, 255), 2 );
		}

	}



	////////////////////// External -- write into class variables ///////////////////////////////////////////
	/// need to prep the matrix of rotations ...
	Matrix4d I;  I.setIdentity();

	// initialize
	vector<Matrix4d> patterns_base(number_patterns, I);
	// whether or not the board is present tells us whether to look at the value there.
	external_parameters.resize(number_images, patterns_base);

	int image_index;
	int pattern_index;



	/// convert from the calibration to saved values.
	/// So the information is going to be with respect treating each observation as if it is from individual images.
	// Here, we are going to rearrange.
	for (int stre = 0, stre_total = int(twod_points_wo_blanks.size()); stre < stre_total; stre++){

		cv::Rodrigues(rvecs[stre], rotMatrix);

		image_index = mapping_from_limited_to_full_images[stre];
		pattern_index = mapping_from_limited_to_full_patterns[stre];


		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				external_parameters[image_index][pattern_index](i, j) = rotMatrix.at<double>(i, j);
			}

			external_parameters[image_index][pattern_index](i, 3) = tvecs[stre].at<double>(i);
		}
	}


	/////////////////////////////// UNDISTORT, WRITE REPROJECTION ////////////////////////////////////
	cv::Mat view, rview, map1, map2;
	//	cv::Mat gray;
	string filename;
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
			cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0),
			image_size, CV_16SC2, map1, map2);

	for (int i = 0; i < number_images; i++){
		cout << "Writing external " << i << endl;
		cv::remap(reproject_cam_cali_images[i], rview, map1, map2, cv::INTER_LINEAR);

		filename  = write_dir + "/ext" + ToString<int>(i) + ".png";
		cv::imwrite(filename.c_str(), rview);
	}


	cout << "internal parameters in function : " << internal_parameters << endl;
	///////////////////////////////// WRITE CALIBRATION INFORMATION ///////////////////////////
	std::ofstream out;
	filename = write_dir + "cali_results.txt";
	out.open(filename.c_str());

	out << "Number_patterns " << twod_points_wo_blanks.size() << endl;
	out << "rms " << rms << endl;
	out << "internal_matrix " << endl;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			out << internal_parameters(i, j) << " ";
		}
		out << endl;
	}


	//<< internal_parameters << endl;
	out << "distortion_size " << distortion.rows() << endl;
	out << "distortion_vector " << endl << distortion << endl;


	for (int stre = 0, stre_total = int(twod_points_wo_blanks.size()); stre < stre_total; stre++){
		image_index = mapping_from_limited_to_full_images[stre];
		pattern_index = mapping_from_limited_to_full_patterns[stre];

		out << "EXTERNAL image, pattern, reproj " << image_index << " " << pattern_index << " " << reproj_error_per_board[image_index][pattern_index] << endl;
		out << external_parameters[image_index][pattern_index] << endl;
	}


	//has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
	out << "has-calibration-estimate " << endl;
	for (int i = 0; i < number_images; i++){
		for (int j = 0; j < number_patterns; j++){
			out << has_calibration_estimate[i][j] << " ";
		}
		out << endl;
	}
	out.close();

	filename = write_dir + "two_d_data.txt";
	out.open(filename.c_str());
	for (int m = 0; m < int(twod_points_wo_blanks.size()); m++){
		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
			out << twod_points_wo_blanks[m][s].x << " " << twod_points_wo_blanks[m][s].y  << endl;
		}
	}
	out.close();

	filename = write_dir + "three_d_data.txt";
	out.open(filename.c_str());
	for (int m = 0; m < int(twod_points_wo_blanks.size()); m++){
		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
			out << threed_points_wo_blanks[m][s].x << " " << threed_points_wo_blanks[m][s].y  << " " << threed_points_wo_blanks[m][s].z << endl;
		}
	}
	out.close();


	images.clear(); /// we don't need to keep this hanging about .... too much memory.
}


//void CameraCali::CalibrateArucoGeneralID(string write_dir){
//
//
//	/// want to recover pose after calibration ...need a map.
//	vector<int> mapping_from_limited_to_full_images;
//	vector<int> mapping_from_limited_to_full_patterns;
//	// create a collection of the points for each image -- hopefully this will work. map -- .
//	vector< vector< cv::Point2f> > twod_points_wo_blanks;
//	vector< vector< cv::Point3f> > threed_points_wo_blanks;
//
//
//
//	int last_added = 0;
//	int s = 0;
//	//	for (int i = 0; i < P_class->max_internal_patterns; i++){
//	//		if (id_bool[i] == true){
//	//			for (int j =0 ; j < 4; j++){
//	//				//twod_points_wo_blanks[last_added][i].x;
//	//				twod_points_wo_blanks[last_added][s].x = internal_two_d_point_coordinates_dense[0](i*4 + j, 0);
//	//				twod_points_wo_blanks[last_added][s].y = internal_two_d_point_coordinates_dense[0](i*4 + j, 1);
//	//
//	//				threed_points_wo_blanks[last_added][s].x = P_class->three_d_points_internal[i*4 + j].x;
//	//				threed_points_wo_blanks[last_added][s].y = P_class->three_d_points_internal[i*4 + j].y;
//	//				threed_points_wo_blanks[last_added][s].z = P_class->three_d_points_internal[i*4 + j].z;
//	//				s++;
//	//			}
//	//		}
//	//	}
//
//
//	int number_images = images.size();
//	int number_squares = P_class->NumberSquares();
//	int number_patterns = P_class->NumberPatterns();
//
//	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
//	//int s;
//	//	//	number_images = 2;
//	cv::Size image_size;
//	//	int last_added;
//	//	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
//	//
//
//	//rows = images[0].rows;
//	//cols = images[0].cols;
//
//
//	//	/// external points.
//	//	for (int i = 0; i < number_images; i++){
//	//		cout << "i "  << i << endl;
//	//		rows = images[i].rows;
//	//		cols = images[i].cols;
//	//		for (int p = 0; p < number_patterns; p++){
//	//
//	//			if (patterns_present[i][p] == true){ /// 4 points per board.  Will this be enough?  We'll see.
//	//				//if (boards_detected[i][p] == true)
//	//				{   // why both?
//	//					has_calibration_estimate[i][p] = true;
//	//					cout << "Image, Pattern " << i << ", " << p << endl;
//	//					//cout << "Points per " << points_per_board[i][p] << endl;
//	//					//cout << "Board number " << p << " detected" << endl;
//	//					mapping_from_limited_to_full_images.push_back(i);
//	//					mapping_from_limited_to_full_patterns.push_back(p);
//	//
//	//					/// each board is a new observation, as is each image.  Get to finer resolution-figuring or transformations later if there are two on the same image.
//	//					twod_points_wo_blanks.push_back(vector< cv::Point2f>(4));
//	//					threed_points_wo_blanks.push_back(vector< cv::Point3f>(4));
//	//
//	//					/// then, walk through all of the possibles ONLY AT THIS PATTERN/BOARD.
//	//					s = 0;
//	//					last_added = twod_points_wo_blanks.size();
//	//					last_added--;
//	//					//cout << "LINE 430: Size of current two d " << twod_points_wo_blanks[last_added].size() << " Total samples " << last_added << endl;
//	//					for (int j = 0; j < 4; j++){
//	//						//cout << "Pattern indices in calibrate " << p*4 + j << endl;
//	//						twod_points_wo_blanks[last_added][s].x = two_d_point_coordinates_dense[i](p*4 + j, 0);    /// twod points w/o blanks is NOT per image to make internal cali work.
//	//						twod_points_wo_blanks[last_added][s].y = two_d_point_coordinates_dense[i](p*4 + j, 1);
//	//
//	//						threed_points_wo_blanks[last_added][s].x = P_class->three_d_points[p*4 + j].x;
//	//						threed_points_wo_blanks[last_added][s].y = P_class->three_d_points[p*4 + j].y;
//	//						threed_points_wo_blanks[last_added][s].z = P_class->three_d_points[p*4 + j].z;
//	//
//	//						s++;
//	//					}
//	//				}
//	//			}
//	//		}
//	//	}
//
//
//	vector<vector< cv::Point2f> > twod_points_wo_blanks_internal;
//	vector< vector< cv::Point3f> > threed_points_wo_blanks_internal;
//
//
//
//	twod_points_wo_blanks_internal.push_back(vector< cv::Point2f>(count_internal_ids_present*4));
//	threed_points_wo_blanks_internal.push_back(vector< cv::Point3f>(count_internal_ids_present*4));
//	// then, we can also do the rest of the external items too, and grab all of that good data.  For now, just internals.
//	cout << "Filled in ... " << count_internal_ids_present*4 << endl;
//
//	/// the first one will be from the internal pattern ....
//
//	last_added = twod_points_wo_blanks.size();
//	last_added--;
//
//	last_added = 0;
//	s = 0;
//	for (int i = 0; i < P_class->max_internal_patterns; i++){
//		if (id_bool[i] == true){
//			for (int j =0 ; j < 4; j++){
//				//twod_points_wo_blanks[last_added][i].x;
//				twod_points_wo_blanks_internal[last_added][s].x = internal_two_d_point_coordinates_dense[0](i*4 + j, 0);
//				twod_points_wo_blanks_internal[last_added][s].y = internal_two_d_point_coordinates_dense[0](i*4 + j, 1);
//
//				threed_points_wo_blanks_internal[last_added][s].x = P_class->three_d_points[i*4 + j].x;
//				threed_points_wo_blanks_internal[last_added][s].y = P_class->three_d_points[i*4 + j].y;
//				threed_points_wo_blanks_internal[last_added][s].z = P_class->three_d_points[i*4 + j].z;
//				s++;
//			}
//		}
//	}
//
//	//twod_points_wo_blanks_internal.push_back(twod_points_wo_blanks[twod_points_wo_blanks.size() - 1]);
//	//threed_points_wo_blanks_internal.push_back(threed_points_wo_blanks[twod_points_wo_blanks.size() - 1]);
//
//
//
//
//	image_size = Size(cols, rows);
//
//
//	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
//
//	cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F);
//
//
//	vector<cv::Mat> rvecs, tvecs;
//
//	vector<cv::Mat> rvecs_internal, tvecs_internal;
//
//	cameraMatrix.at<double>(0, 0) = pixel_width;
//	cameraMatrix.at<double>(1, 1) = pixel_width;;
//
//	cameraMatrix.at<double>(0, 2) = cols/2;
//	cameraMatrix.at<double>(1, 2) = rows/2;
//
//	//	cout << "initial camera matrix " << endl;
//	//
//	//	for (int i = 0; i < 3; i++){
//	//		for (int j = 0; j < 3; j++){
//	//			cout << cameraMatrix.at<double>(i, j) << " ";
//	//		}
//	//		cout << endl;
//	//	}
//
//	cout << "Running calibration " << endl;
//	//cout << "Number of dist coefficients  = " << distCoeffs.rows << endl;
//
//	double rms = 0;
//	char ch;
//	//	cout << "Before first " << endl; cin >> ch;
//	//	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//							CV_CALIB_RATIONAL_MODEL);
//
//
//	//if (text_file.size() == 0){
//
//	// experiment -- write input
//	//rms = cv::calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);
//
//	//cout << "rms " << rms << endl;
//
//
//	//rms = calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//		CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_PRINCIPAL_POINT | CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
//	//		CALIB_FIX_FOCAL_LENGTH);
//
//	rms = calibrateCamera(threed_points_wo_blanks_internal, twod_points_wo_blanks_internal, image_size, cameraMatrix, distCoeffs, rvecs_internal, tvecs_internal,
//			CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_PRINCIPAL_POINT | CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
//			CALIB_FIX_FOCAL_LENGTH);
//
//	//rms = calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//					CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
//	//					CALIB_FIX_FOCAL_LENGTH);
//
//	cout << "rms " << rms << endl;
//
//	// want to reproject and see how bad it is ... this is not really giving us that satisfaction.
//	int number_from_external = twod_points_wo_blanks.size();
//
//	cout << "Before external" << endl;
//	for (int i = 0; i < number_from_external; i++){
//		cv::Mat rv; cv::Mat tv;
//		solvePnP(threed_points_wo_blanks[i], twod_points_wo_blanks[i], cameraMatrix, distCoeffs, rv, tv, false);
//
//		rvecs.push_back(rv);
//		tvecs.push_back(tv);
//
//	}
//
//	cout << "After external" << endl;
//	rvecs.push_back(rvecs_internal[0]);
//	tvecs.push_back(tvecs_internal[0]);
//
//
//	//bool solvePnP(InputArray objectPoints, InputArray imagePoints, InputArray cameraMatrix, InputArray distCoeffs, OutputArray rvec, OutputArray tvec, bool useExtrinsicGuess=false, int flags=SOLVEPNP_ITERATIVE )
//	//SOLVEPNP_ITERATIVE Iterative method is based on Levenberg-Marquardt optimization. In this case the function finds such a pose that minimizes reprojection error, that is the sum of squared distances between the observed projections imagePoints and the projected (using projectPoints() ) objectPoints .
//	//SOLVEPNP_P3P Method is based on the paper of X.S. Gao, X.-R. Hou, J. Tang, H.-F. Chang “Complete Solution Classification for the Perspective-Three-Point Problem”. In this case the function requires exactly four object and image points.
//
//
//
//
//	/// write calibration details now.  Also, transfer to the Eigen format.
//
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			internal_parameters(i, j) = cameraMatrix.at<double>(i, j);
//		}
//	}
//
//	distortion.resize(distCoeffs.rows);
//	for (int i = 0; i < distCoeffs.rows; i++){
//		distortion(i) = distCoeffs.at<double>(i, 0);
//
//	}
//
//	cout << "distortion " << distortion << endl;
//	cout << "Camera matrix " << endl << cameraMatrix << endl;
//
//	cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);
//	vector< vector <double> > tempRt(3, vector<double>(4, 0));
//
//
//	double reproj_error = 0;
//	vector<cv::Point2f> imagePoints2;
//	double err;
//	int correct_image;
//	int correct_pattern;
//
//	for (int i = 0; i < number_images; i++){
//		reproject_cam_cali_images.push_back(images[i].clone());
//	}
//
//	//vector< vector<cv::Point2f> > reproj_image_points;
//
//	// TODO keep these repo errors per image/per pattern ... then do not need to redo.
//	// These will form a basis of which hypotheses to use first when we start tightening things up.
//	// ESPECIALLY when there are two or more patterns per frame.
//	//
//
//	/// internal only
//	int number_cali_items = twod_points_wo_blanks.size();
//	for (int m = number_cali_items - 1; m < number_cali_items; m++){
//
//		cv::projectPoints( cv::Mat(threed_points_wo_blanks[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
//				distCoeffs, imagePoints2);
//		err = cv::norm(cv::Mat(twod_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
//		reproj_error        += err*err;
//
//		//reproj_image_points.push_back(imagePoints2);
//		//		correct_image = mapping_from_limited_to_full_images[m];
//		//		correct_pattern = mapping_from_limited_to_full_patterns[m];
//		//		reproj_error_per_board[correct_image][correct_pattern] = err*err;
//
//		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
//			line(reproject_cam_cali_images[0], twod_points_wo_blanks[m][j],imagePoints2[j], Scalar(255, 0, 255), 6 );
//		}
//
//	}
//
//
//
//	vector<double> temp_repro(number_patterns, 0);
//	reproj_error_per_board.resize(number_images, temp_repro);
//
//	for (int m = 0; m < number_cali_items - 1; m++){
//
//		cv::projectPoints( cv::Mat(threed_points_wo_blanks[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
//				distCoeffs, imagePoints2);
//		err = cv::norm(cv::Mat(twod_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
//		reproj_error        += err*err;
//
//		//reproj_image_points.push_back(imagePoints2);
//		correct_image = mapping_from_limited_to_full_images[m];
//		correct_pattern = mapping_from_limited_to_full_patterns[m];
//		reproj_error_per_board[correct_image][correct_pattern] = err*err;
//
//		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
//			line(reproject_cam_cali_images[correct_image], twod_points_wo_blanks[m][j],imagePoints2[j], Scalar(255, 0, 255), 10 );
//		}
//
//	}
//
//
//
//	////////////////////// External -- write into class variables ///////////////////////////////////////////
//	/// need to prep the matrix of rotations ...
//	Matrix4d I;  I.setIdentity();
//
//	// initialize
//	vector<Matrix4d> patterns_base(number_patterns, I);
//	// whether or not the board is present tells us whether to look at the value there.
//	external_parameters.resize(number_images, patterns_base);
//
//	int image_index;
//	int pattern_index;
//
//
//
//	/// convert from the calibration to saved values.
//	/// So the information is going to be with respect treating each observation as if it is from individual images.
//	// Here, we are going to rearrange.
//	for (int stre = 0, stre_total = number_cali_items - 1; stre < stre_total; stre++){
//
//		cv::Rodrigues(rvecs[stre], rotMatrix);
//
//		image_index = mapping_from_limited_to_full_images[stre];
//		pattern_index = mapping_from_limited_to_full_patterns[stre];
//
//
//		for (int i = 0; i < 3; i++){
//			for (int j = 0; j < 3; j++){
//				external_parameters[image_index][pattern_index](i, j) = rotMatrix.at<double>(i, j);
//			}
//
//			external_parameters[image_index][pattern_index](i, 3) = tvecs[stre].at<double>(i);
//		}
//	}
//
//
//	//	/////////////////////////////// UNDISTORT, WRITE REPROJECTION ////////////////////////////////////
//	cv::Mat view, rview, map1, map2;
//	//	cv::Mat gray;
//	string filename;
//	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
//			cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0),
//			image_size, CV_16SC2, map1, map2);
//
//	for (int i = 0; i < number_images; i++){
//		cout << "Writing external " << i << endl;
//		cv::remap(reproject_cam_cali_images[i], rview, map1, map2, cv::INTER_LINEAR);
//
//		if (i > 1){
//			filename  = write_dir + "/ext" + ToString<int>(i) + ".jpg";
//			cv::imwrite(filename.c_str(), rview);
//
//			Mat im_small;
//			Size si;
//
//			si.width = rview.cols/8;
//			si.height = rview.rows/8;
//
//			resize(rview, im_small, si);
//
//			imwrite(filename.c_str(), im_small);
//		}
//	}
//
//
//	cout << "internal parameters in function : " << internal_parameters << endl;
//	///////////////////////////////// WRITE CALIBRATION INFORMATION ///////////////////////////
//	std::ofstream out;
//	filename = write_dir + "cali_results.txt";
//	out.open(filename.c_str());
//
//	out << "Number_patterns " << number_cali_items - 1 << endl;
//	out << "rms " << rms << endl;
//	out << "internal_matrix " << endl;
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			out << internal_parameters(i, j) << " ";
//		}
//		out << endl;
//	}
//
//
//	//<< internal_parameters << endl;
//	out << "distortion_size " << distortion.rows() << endl;
//	out << "distortion_vector " << endl << distortion << endl;
//
//
//	for (int stre = 0, stre_total = number_cali_items - 1; stre < stre_total; stre++){
//		image_index = mapping_from_limited_to_full_images[stre];
//		pattern_index = mapping_from_limited_to_full_patterns[stre];
//
//		out << "EXTERNAL image, pattern, reproj " << image_index << " " << pattern_index << " " << reproj_error_per_board[image_index][pattern_index] << endl;
//		out << external_parameters[image_index][pattern_index] << endl;
//	}
//
//
//	//has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
//	out << "has-calibration-estimate " << endl;
//	for (int i = 0; i < number_images; i++){
//		for (int j = 0; j < number_patterns; j++){
//			out << has_calibration_estimate[i][j] << " ";
//		}
//		out << endl;
//	}
//	out.close();
//
//	filename = write_dir + "two_d_data.txt";
//	out.open(filename.c_str());
//	for (int m = 0; m < number_cali_items - 1; m++){
//		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
//		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
//			out << twod_points_wo_blanks[m][s].x << " " << twod_points_wo_blanks[m][s].y  << endl;
//		}
//	}
//	out.close();
//
//	filename = write_dir + "three_d_data.txt";
//	out.open(filename.c_str());
//	for (int m = 0; m < number_cali_items - 1; m++){
//		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
//		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
//			out << threed_points_wo_blanks[m][s].x << " " << threed_points_wo_blanks[m][s].y  << " " << threed_points_wo_blanks[m][s].z << endl;
//		}
//	}
//	out.close();
//
//
//	filename = write_dir + "image_points_first_image.txt";
//	out.open(filename.c_str());
//	/// external points.
//	for (int i = 0; i < 1; i++){
//		cout << "i "  << i << endl;
//		for (int p = 0; p < number_patterns; p++){
//
//			if (patterns_present[i][p] == true){ /// 4 points per board.  Will this be enough?  We'll see.
//				{
//
//					cout << "Image, Pattern " << i << ", " << p << endl;
//
//					out << "EXTERNAL " << endl;
//
//					//cout << "LINE 430: Size of current two d " << twod_points_wo_blanks[last_added].size() << " Total samples " << last_added << endl;
//					for (int j = 0; j < 4; j++){
//						out << two_d_point_coordinates_dense[i](p*4 + j, 0) << " " << two_d_point_coordinates_dense[i](p*4 + j, 1) << endl;
//					}
//				}
//			}
//		}
//	}
//
//
//
//
//	for (int i = 0; i < P_class->max_internal_patterns; i++){
//		if (id_bool[i] == true){
//			out << "INTERNAL " << i << endl;
//			for (int j =0 ; j < 4; j++){
//				//twod_points_wo_blanks[last_added][i].x;
//				out << internal_two_d_point_coordinates_dense[0](i*4 + j, 0) << " " << internal_two_d_point_coordinates_dense[0](i*4 + j, 1) << endl;
//			}
//		}
//	}
//
//	out << -1 << endl;
//	out.close();
//
//
//
//
//	images.clear();
//}



void CameraCali::CalibrateStrawberrySet(string write_dir){


	/// want to recover pose after calibration ...need a map.
	vector<int> mapping_from_limited_to_full_images;
	vector<int> mapping_from_limited_to_full_patterns;
	// create a collection of the points for each image -- hopefully this will work. map -- .
	vector< vector< cv::Point2f> > twod_points_wo_blanks;
	vector< vector< cv::Point3f> > threed_points_wo_blanks;



	// work on this!  Add the patterns as well and find a way to write.

	/// take all of the points we have for the one image, with the estimate of the internal parameters, and see what we get!
	// class members to use here
	//vector<MatrixXd> internal_two_d_point_coordinates_dense;
	//vector<vector<bool> > internal_ids_present;
	//double pixel_width;
	//P_class->three_d_points_internal[4*current_index + j].x

	/// need to run valgrind to find out what is going on here ....


	//	twod_points_wo_blanks.push_back(vector< cv::Point2f>(count_internal_ids_present*4));
	//	threed_points_wo_blanks.push_back(vector< cv::Point3f>(count_internal_ids_present*4));
	//	// then, we can also do the rest of the external items too, and grab all of that good data.  For now, just internals.
	//	cout << "Filled in ... " << count_internal_ids_present*4 << endl;
	//
	//	/// the first one will be from the internal pattern ....
	//
	int last_added = 0;
	int s = 0;
	//	for (int i = 0; i < P_class->max_internal_patterns; i++){
	//		if (id_bool[i] == true){
	//			for (int j =0 ; j < 4; j++){
	//				//twod_points_wo_blanks[last_added][i].x;
	//				twod_points_wo_blanks[last_added][s].x = internal_two_d_point_coordinates_dense[0](i*4 + j, 0);
	//				twod_points_wo_blanks[last_added][s].y = internal_two_d_point_coordinates_dense[0](i*4 + j, 1);
	//
	//				threed_points_wo_blanks[last_added][s].x = P_class->three_d_points_internal[i*4 + j].x;
	//				threed_points_wo_blanks[last_added][s].y = P_class->three_d_points_internal[i*4 + j].y;
	//				threed_points_wo_blanks[last_added][s].z = P_class->three_d_points_internal[i*4 + j].z;
	//				s++;
	//			}
	//		}
	//	}


	int number_images = images.size();
	int number_squares = P_class->NumberSquares();
	int number_patterns = P_class->NumberPatterns();

	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
	//int s;
	//	//	number_images = 2;
	cv::Size image_size;
	//	int last_added;
	//	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
	//

	//rows = images[0].rows;
	//cols = images[0].cols;


	/// external points.
	for (int i = 0; i < number_images; i++){
		cout << "i "  << i << endl;
		rows = images[i].rows;
		cols = images[i].cols;
		for (int p = 0; p < number_patterns; p++){

			if (patterns_present[i][p] == true){ /// 4 points per board.  Will this be enough?  We'll see.
				//if (boards_detected[i][p] == true)
				{   // why both?
					has_calibration_estimate[i][p] = true;
					cout << "Image, Pattern " << i << ", " << p << endl;
					//cout << "Points per " << points_per_board[i][p] << endl;
					//cout << "Board number " << p << " detected" << endl;
					mapping_from_limited_to_full_images.push_back(i);
					mapping_from_limited_to_full_patterns.push_back(p);

					/// each board is a new observation, as is each image.  Get to finer resolution-figuring or transformations later if there are two on the same image.
					twod_points_wo_blanks.push_back(vector< cv::Point2f>(4));
					threed_points_wo_blanks.push_back(vector< cv::Point3f>(4));

					/// then, walk through all of the possibles ONLY AT THIS PATTERN/BOARD.
					s = 0;
					last_added = twod_points_wo_blanks.size();
					last_added--;
					//cout << "LINE 430: Size of current two d " << twod_points_wo_blanks[last_added].size() << " Total samples " << last_added << endl;
					for (int j = 0; j < 4; j++){
						//cout << "Pattern indices in calibrate " << p*4 + j << endl;
						twod_points_wo_blanks[last_added][s].x = two_d_point_coordinates_dense[i](p*4 + j, 0);    /// twod points w/o blanks is NOT per image to make internal cali work.
						twod_points_wo_blanks[last_added][s].y = two_d_point_coordinates_dense[i](p*4 + j, 1);

						threed_points_wo_blanks[last_added][s].x = P_class->three_d_points[p*4 + j].x;
						threed_points_wo_blanks[last_added][s].y = P_class->three_d_points[p*4 + j].y;
						threed_points_wo_blanks[last_added][s].z = P_class->three_d_points[p*4 + j].z;

						s++;
					}
				}
			}
		}
	}


	vector<vector< cv::Point2f> > twod_points_wo_blanks_internal;
	vector< vector< cv::Point3f> > threed_points_wo_blanks_internal;



	twod_points_wo_blanks_internal.push_back(vector< cv::Point2f>(count_internal_ids_present*4));
	threed_points_wo_blanks_internal.push_back(vector< cv::Point3f>(count_internal_ids_present*4));
	// then, we can also do the rest of the external items too, and grab all of that good data.  For now, just internals.
	cout << "Filled in ... " << count_internal_ids_present*4 << endl;

	/// the first one will be from the internal pattern ....

	last_added = twod_points_wo_blanks.size();
	last_added--;

	last_added = 0;
	s = 0;
	for (int i = 0; i < P_class->max_internal_patterns; i++){
		if (id_bool[i] == true){
			for (int j =0 ; j < 4; j++){
				//twod_points_wo_blanks[last_added][i].x;
				twod_points_wo_blanks_internal[last_added][s].x = internal_two_d_point_coordinates_dense[0](i*4 + j, 0);
				twod_points_wo_blanks_internal[last_added][s].y = internal_two_d_point_coordinates_dense[0](i*4 + j, 1);

				threed_points_wo_blanks_internal[last_added][s].x = P_class->three_d_points_internal[i*4 + j].x;
				threed_points_wo_blanks_internal[last_added][s].y = P_class->three_d_points_internal[i*4 + j].y;
				threed_points_wo_blanks_internal[last_added][s].z = P_class->three_d_points_internal[i*4 + j].z;
				s++;
			}
		}
	}

	//twod_points_wo_blanks_internal.push_back(twod_points_wo_blanks[twod_points_wo_blanks.size() - 1]);
	//threed_points_wo_blanks_internal.push_back(threed_points_wo_blanks[twod_points_wo_blanks.size() - 1]);




	image_size = Size(cols, rows);


	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);

	cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F);


	vector<cv::Mat> rvecs, tvecs;

	vector<cv::Mat> rvecs_internal, tvecs_internal;

	cameraMatrix.at<double>(0, 0) = pixel_width;
	cameraMatrix.at<double>(1, 1) = pixel_width;;

	cameraMatrix.at<double>(0, 2) = cols/2;
	cameraMatrix.at<double>(1, 2) = rows/2;

	//	cout << "initial camera matrix " << endl;
	//
	//	for (int i = 0; i < 3; i++){
	//		for (int j = 0; j < 3; j++){
	//			cout << cameraMatrix.at<double>(i, j) << " ";
	//		}
	//		cout << endl;
	//	}

	cout << "Running calibration " << endl;
	//cout << "Number of dist coefficients  = " << distCoeffs.rows << endl;

	double rms = 0;
	char ch;
	//	cout << "Before first " << endl; cin >> ch;
	//	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
	//							CV_CALIB_RATIONAL_MODEL);


	//if (text_file.size() == 0){

	// experiment -- write input
	//rms = cv::calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);

	//cout << "rms " << rms << endl;


	//rms = calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
	//		CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_PRINCIPAL_POINT | CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
	//		CALIB_FIX_FOCAL_LENGTH);

	rms = calibrateCamera(threed_points_wo_blanks_internal, twod_points_wo_blanks_internal, image_size, cameraMatrix, distCoeffs, rvecs_internal, tvecs_internal,
			CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_PRINCIPAL_POINT | CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
			CALIB_FIX_FOCAL_LENGTH);

	//rms = calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
	//					CALIB_USE_INTRINSIC_GUESS| CALIB_ZERO_TANGENT_DIST| CALIB_FIX_K3 | CALIB_FIX_ASPECT_RATIO |
	//					CALIB_FIX_FOCAL_LENGTH);

	cout << "rms " << rms << endl;

	// want to reproject and see how bad it is ... this is not really giving us that satisfaction.
	int number_from_external = twod_points_wo_blanks.size();

	cout << "Before external" << endl;
	for (int i = 0; i < number_from_external; i++){
		cv::Mat rv; cv::Mat tv;
		solvePnP(threed_points_wo_blanks[i], twod_points_wo_blanks[i], cameraMatrix, distCoeffs, rv, tv, false);

		rvecs.push_back(rv);
		tvecs.push_back(tv);

	}

	cout << "After external" << endl;
	rvecs.push_back(rvecs_internal[0]);
	tvecs.push_back(tvecs_internal[0]);


	//bool solvePnP(InputArray objectPoints, InputArray imagePoints, InputArray cameraMatrix, InputArray distCoeffs, OutputArray rvec, OutputArray tvec, bool useExtrinsicGuess=false, int flags=SOLVEPNP_ITERATIVE )
	//SOLVEPNP_ITERATIVE Iterative method is based on Levenberg-Marquardt optimization. In this case the function finds such a pose that minimizes reprojection error, that is the sum of squared distances between the observed projections imagePoints and the projected (using projectPoints() ) objectPoints .
	//SOLVEPNP_P3P Method is based on the paper of X.S. Gao, X.-R. Hou, J. Tang, H.-F. Chang “Complete Solution Classification for the Perspective-Three-Point Problem”. In this case the function requires exactly four object and image points.




	/// write calibration details now.  Also, transfer to the Eigen format.

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			internal_parameters(i, j) = cameraMatrix.at<double>(i, j);
		}
	}

	distortion.resize(distCoeffs.rows);
	for (int i = 0; i < distCoeffs.rows; i++){
		distortion(i) = distCoeffs.at<double>(i, 0);

	}

	cout << "distortion " << distortion << endl;
	cout << "Camera matrix " << endl << cameraMatrix << endl;

	cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);
	vector< vector <double> > tempRt(3, vector<double>(4, 0));


	double reproj_error = 0;
	vector<cv::Point2f> imagePoints2;
	double err;
	int correct_image;
	int correct_pattern;

	for (int i = 0; i < number_images; i++){
		reproject_cam_cali_images.push_back(images[i].clone());
	}

	//vector< vector<cv::Point2f> > reproj_image_points;

	// TODO keep these repo errors per image/per pattern ... then do not need to redo.
	// These will form a basis of which hypotheses to use first when we start tightening things up.
	// ESPECIALLY when there are two or more patterns per frame.
	//

	/// internal only
	int number_cali_items = twod_points_wo_blanks.size();
	for (int m = number_cali_items - 1; m < number_cali_items; m++){

		cv::projectPoints( cv::Mat(threed_points_wo_blanks[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
				distCoeffs, imagePoints2);
		err = cv::norm(cv::Mat(twod_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
		reproj_error        += err*err;

		//reproj_image_points.push_back(imagePoints2);
		//		correct_image = mapping_from_limited_to_full_images[m];
		//		correct_pattern = mapping_from_limited_to_full_patterns[m];
		//		reproj_error_per_board[correct_image][correct_pattern] = err*err;

		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
			line(reproject_cam_cali_images[0], twod_points_wo_blanks[m][j],imagePoints2[j], Scalar(255, 0, 255), 6 );
		}

	}



	vector<double> temp_repro(number_patterns, 0);
	reproj_error_per_board.resize(number_images, temp_repro);

	for (int m = 0; m < number_cali_items - 1; m++){

		cv::projectPoints( cv::Mat(threed_points_wo_blanks[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
				distCoeffs, imagePoints2);
		err = cv::norm(cv::Mat(twod_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
		reproj_error        += err*err;

		//reproj_image_points.push_back(imagePoints2);
		correct_image = mapping_from_limited_to_full_images[m];
		correct_pattern = mapping_from_limited_to_full_patterns[m];
		reproj_error_per_board[correct_image][correct_pattern] = err*err;

		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
			line(reproject_cam_cali_images[correct_image], twod_points_wo_blanks[m][j],imagePoints2[j], Scalar(255, 0, 255), 10 );
		}

	}



	////////////////////// External -- write into class variables ///////////////////////////////////////////
	/// need to prep the matrix of rotations ...
	Matrix4d I;  I.setIdentity();

	// initialize
	vector<Matrix4d> patterns_base(number_patterns, I);
	// whether or not the board is present tells us whether to look at the value there.
	external_parameters.resize(number_images, patterns_base);

	int image_index;
	int pattern_index;



	/// convert from the calibration to saved values.
	/// So the information is going to be with respect treating each observation as if it is from individual images.
	// Here, we are going to rearrange.
	for (int stre = 0, stre_total = number_cali_items - 1; stre < stre_total; stre++){

		cv::Rodrigues(rvecs[stre], rotMatrix);

		image_index = mapping_from_limited_to_full_images[stre];
		pattern_index = mapping_from_limited_to_full_patterns[stre];


		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				external_parameters[image_index][pattern_index](i, j) = rotMatrix.at<double>(i, j);
			}

			external_parameters[image_index][pattern_index](i, 3) = tvecs[stre].at<double>(i);
		}
	}


	//	/////////////////////////////// UNDISTORT, WRITE REPROJECTION ////////////////////////////////////
	cv::Mat view, rview, map1, map2;
	//	cv::Mat gray;
	string filename;
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
			cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0),
			image_size, CV_16SC2, map1, map2);

	for (int i = 0; i < number_images; i++){
		cout << "Writing external " << i << endl;
		cv::remap(reproject_cam_cali_images[i], rview, map1, map2, cv::INTER_LINEAR);

		if (i > 1){
			filename  = write_dir + "/ext" + ToString<int>(i) + ".jpg";
			cv::imwrite(filename.c_str(), rview);

			Mat im_small;
			Size si;

			si.width = rview.cols/8;
			si.height = rview.rows/8;

			resize(rview, im_small, si);

			imwrite(filename.c_str(), im_small);
		}
	}


	cout << "internal parameters in function : " << internal_parameters << endl;
	///////////////////////////////// WRITE CALIBRATION INFORMATION ///////////////////////////
	std::ofstream out;
	filename = write_dir + "cali_results.txt";
	out.open(filename.c_str());

	out << "Number_patterns " << number_cali_items - 1 << endl;
	out << "rms " << rms << endl;
	out << "internal_matrix " << endl;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			out << internal_parameters(i, j) << " ";
		}
		out << endl;
	}


	//<< internal_parameters << endl;
	out << "distortion_size " << distortion.rows() << endl;
	out << "distortion_vector " << endl << distortion << endl;


	for (int stre = 0, stre_total = number_cali_items - 1; stre < stre_total; stre++){
		image_index = mapping_from_limited_to_full_images[stre];
		pattern_index = mapping_from_limited_to_full_patterns[stre];

		out << "EXTERNAL image, pattern, reproj " << image_index << " " << pattern_index << " " << reproj_error_per_board[image_index][pattern_index] << endl;
		out << external_parameters[image_index][pattern_index] << endl;
	}


	//has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));
	out << "has-calibration-estimate " << endl;
	for (int i = 0; i < number_images; i++){
		for (int j = 0; j < number_patterns; j++){
			out << has_calibration_estimate[i][j] << " ";
		}
		out << endl;
	}
	out.close();

	filename = write_dir + "two_d_data.txt";
	out.open(filename.c_str());
	for (int m = 0; m < number_cali_items - 1; m++){
		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
			out << twod_points_wo_blanks[m][s].x << " " << twod_points_wo_blanks[m][s].y  << endl;
		}
	}
	out.close();

	filename = write_dir + "three_d_data.txt";
	out.open(filename.c_str());
	for (int m = 0; m < number_cali_items - 1; m++){
		out << "New-board " << twod_points_wo_blanks[m].size() << endl;
		for (int s = 0; s < int(twod_points_wo_blanks[m].size()); s++){
			out << threed_points_wo_blanks[m][s].x << " " << threed_points_wo_blanks[m][s].y  << " " << threed_points_wo_blanks[m][s].z << endl;
		}
	}
	out.close();


	filename = write_dir + "image_points_first_image.txt";
	out.open(filename.c_str());
	/// external points.
	for (int i = 0; i < 1; i++){
		cout << "i "  << i << endl;
		for (int p = 0; p < number_patterns; p++){

			if (patterns_present[i][p] == true){ /// 4 points per board.  Will this be enough?  We'll see.
				{

					cout << "Image, Pattern " << i << ", " << p << endl;

					out << "EXTERNAL " << endl;

					//cout << "LINE 430: Size of current two d " << twod_points_wo_blanks[last_added].size() << " Total samples " << last_added << endl;
					for (int j = 0; j < 4; j++){
						out << two_d_point_coordinates_dense[i](p*4 + j, 0) << " " << two_d_point_coordinates_dense[i](p*4 + j, 1) << endl;
					}
				}
			}
		}
	}




	for (int i = 0; i < P_class->max_internal_patterns; i++){
		if (id_bool[i] == true){
			out << "INTERNAL " << i << endl;
			for (int j =0 ; j < 4; j++){
				//twod_points_wo_blanks[last_added][i].x;
				out << internal_two_d_point_coordinates_dense[0](i*4 + j, 0) << " " << internal_two_d_point_coordinates_dense[0](i*4 + j, 1) << endl;
			}
		}
	}

	out << -1 << endl;
	out.close();




	images.clear();
}

void CameraCali::CopyToMats(Mat& CameraMatrix, Mat& dis){

	for (int r = 0; r < 3; r++){
		for (int c = 0; c < 3; c++){
			CameraMatrix.at<double>(r, c) = internal_parameters(r, c);
		}
	}




	for (int i = 0; i < distortion.rows(); i++){
		dis.at<double>(i, 0) = distortion(i, 0);

	}
}


double CameraCali::ComputeReprojectionErrorOneImagePattern(Matrix4d& ExtParameters, int camera_number, int image_number,
		int pattern_number,
		int type, string write_directory, bool write, int equation_number, bool is_strawberry, Matrix3d* IntParameters){

	// something is wonky here .... double check.  eliminate write.
	// will have to read from the dense points ... there's a known number of points per board.  Start here .  Once this works,
	// try to calibrate with this function.
	cout << "Aprime " << endl << ExtParameters << endl;

	int number_points = points_per_board[image_number][pattern_number];
	cout << "Number points! " << number_points << endl;
	if (number_points == 0){
		cout << "Number points is zero! " << number_points << endl;
		exit(1);
	}

	//	// TODO keep these repo errors per image/per pattern ... then do not need to redo.
	vector< cv::Point2f>  twod_points_wo_blanks(number_points);
	vector< cv::Point3f>  threed_points_wo_blanks(number_points);
	vector<cv::Point2f> imagePoints2;
	double err;

	int s = 0;

	for (int j = P_class->min_max_id_squares[pattern_number].first; j <= P_class->min_max_id_squares[pattern_number].second; j++){
		if (points_present[image_number].at(j) == true){
			//cout << "s, j " << s << ", " << j << endl;
			/// invalid write here .   Check it out.
			twod_points_wo_blanks[s].x = two_d_point_coordinates_dense[image_number](j, 0);    /// twod points w/o blanks is NOT per image to make internal cali work.
			twod_points_wo_blanks[s].y = two_d_point_coordinates_dense[image_number](j, 1);

			threed_points_wo_blanks[s].x = P_class->three_d_points[j].x;
			threed_points_wo_blanks[s].y = P_class->three_d_points[j].y;
			threed_points_wo_blanks[s].z = P_class->three_d_points[j].z;

			s++;
		}
	}




	cout << "first 2d point " << twod_points_wo_blanks[0] << endl;
	cout << "first 3d point " << threed_points_wo_blanks[0] << endl;

	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);

	int n_dist = distortion.rows();
	cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat rvec;
	cv::Mat tvec = cv::Mat(3, 1, CV_64F);
	cv::Mat distCoeffs = cv::Mat::zeros(n_dist, 1, CV_64F);


	// convert this matrix to the OpenCV format ....

	if (IntParameters == 0){
		for (int r = 0; r < 3; r++){
			for (int c = 0; c < 3; c++){
				rotMatrix.at<double>(r, c) = ExtParameters(r, c);
				cameraMatrix.at<double>(r, c) = internal_parameters(r, c);
			}
			tvec.at<double>(r, 0) = ExtParameters(r, 3);
		}

	}	else {
		Matrix3d IP = *IntParameters;

		for (int r = 0; r < 3; r++){
			for (int c = 0; c < 3; c++){
				rotMatrix.at<double>(r, c) = ExtParameters(r, c);
				cameraMatrix.at<double>(r, c) = IP(r, c);
			}
			tvec.at<double>(r, 0) = ExtParameters(r, 3);
		}

		//cout << "Used IP! " << endl << IP << endl;
		//char jn; cin >> jn;

	}
	cout << "internal matrix " << endl << cameraMatrix << endl;
	cout << "rotation matrix " << endl << rotMatrix << endl;
	cout << "translation    " << endl << tvec << endl;

	cv::Rodrigues(rotMatrix, rvec);

	for (int i = 0; i < n_dist; i++){
		distCoeffs.at<double>(i, 0) = distortion(i);
	}


	double reproj_error = 0;


	cv::projectPoints( cv::Mat(threed_points_wo_blanks), rvec, tvec, cameraMatrix,  // project
			distCoeffs, imagePoints2);
	err = cv::norm(cv::Mat(twod_points_wo_blanks), cv::Mat(imagePoints2), CV_L2);              // difference
	reproj_error        += err*err;

	// scale by the number of points ....
	reproj_error = reproj_error / double(number_points);


	if (write){
		// different colors for the different types.
		Scalar line_color(0, 0, 0);

		line_color = P_class->Color(pattern_number);

		//		switch (type){
		//		case 0: {
		//			line_color = Scalar(255, 0, 0);
		//		} break;
		//		case 1: {
		//			line_color = Scalar(0, 255, 0);
		//		} break;
		//		case 2: {
		//			line_color = Scalar(0, 0, 255);
		//		} break;
		//		default: {
		//			line_color = Scalar(255, 0, 255);
		//		}
		//
		//		}


		//Mat im_copy = images[image_number].clone();
		cout << "Size of im names " << im_names.size() << endl;
		Mat im_copy = imread(im_names[image_number].c_str(), IMREAD_COLOR);
		int number_p = imagePoints2.size();

		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
			line(im_copy, twod_points_wo_blanks[j],imagePoints2[j], line_color, 2 );
		}

		if (is_strawberry){
			for (int j = 0, jn = imagePoints2.size() - 1; j < jn; j++){
				line(im_copy, imagePoints2[j],imagePoints2[j + 1], line_color, 10 );
			}

			line(im_copy, imagePoints2[0],imagePoints2[number_p - 1], line_color, 10 );
		}


		string filename;

		filename = write_directory + "Equation" + ToString<int>(equation_number);

		if (is_strawberry){
			filename = filename + ".jpg";

			// resize
			Mat im_small;
			Size si;

			si.width = im_copy.cols/8;
			si.height = im_copy.rows/8;

			resize(im_copy, im_small, si);

			imwrite(filename.c_str(), im_small);

		}	else {
			filename = filename + ".png";
			imwrite(filename.c_str(), im_copy);
		}


	}

	return reproj_error;

}




//double CameraCali::ComputeReprojectionErrorOneImagePattern(Matrix3d& IntParameters, Matrix4d& ExtParameters, int camera_number, int image_number, int pattern_number,
//		int type, string write_directory, bool write, int equation_number){
//
//	// something is wonky here .... double check.  eliminate write.
//	// will have to read from the dense points ... there's a known number of points per board.  Start here .  Once this works,
//	// try to calibrate with this function.
//	cout << "Aprime " << endl << ExtParameters << endl;
//
//	int number_points = points_per_board[image_number][pattern_number];
//	cout << "Number points! " << number_points << endl;
//	if (number_points == 0){
//		cout << "Number points is zero! " << number_points << endl;
//		exit(1);
//	}
//
////	// TODO keep these repo errors per image/per pattern ... then do not need to redo.
//	vector< cv::Point2f>  twod_points_wo_blanks(number_points);
//	vector< cv::Point3f>  threed_points_wo_blanks(number_points);
//	vector<cv::Point2f> imagePoints2;
//		double err;
//
//	int s = 0;
//
//	for (int j = P_class->min_max_id_squares[pattern_number].first; j <= P_class->min_max_id_squares[pattern_number].second; j++){
//		if (points_present[image_number].at(j) == true){
//			//cout << "s, j " << s << ", " << j << endl;
//			/// invalid write here .   Check it out.
//			twod_points_wo_blanks[s].x = two_d_point_coordinates_dense[image_number](j, 0);    /// twod points w/o blanks is NOT per image to make internal cali work.
//			twod_points_wo_blanks[s].y = two_d_point_coordinates_dense[image_number](j, 1);
//
//			threed_points_wo_blanks[s].x = P_class->three_d_points[j].x;
//			threed_points_wo_blanks[s].y = P_class->three_d_points[j].y;
//			threed_points_wo_blanks[s].z = P_class->three_d_points[j].z;
//
//			s++;
//		}
//	}
//
//
//	cout << "first 2d point " << twod_points_wo_blanks[0] << endl;
//	cout << "first 3d point " << threed_points_wo_blanks[0] << endl;
//
//	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
//
//	int n_dist = distortion.rows();
//	cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);
//	cv::Mat rvec;
//	cv::Mat tvec = cv::Mat(3, 1, CV_64F);
//	cv::Mat distCoeffs = cv::Mat::zeros(n_dist, 1, CV_64F);
//
//
//	// convert this matrix to the OpenCV format ....
//	for (int r = 0; r < 3; r++){
//		for (int c = 0; c < 3; c++){
//			rotMatrix.at<double>(r, c) = ExtParameters(r, c);
//			cameraMatrix.at<double>(r, c) = internal_pa(r, c);
//		}
//		tvec.at<double>(r, 0) = ExtParameters(r, 3);
//	}
//	cout << "internal matrix " << endl << cameraMatrix << endl;
//	cout << "rotation matrix " << endl << rotMatrix << endl;
//	cout << "translation    " << endl << tvec << endl;
//
//	cv::Rodrigues(rotMatrix, rvec);
//
//	for (int i = 0; i < n_dist; i++){
//		distCoeffs.at<double>(i, 0) = distortion(i);
//	}
//
//
//	double reproj_error = 0;
//
//
//	cv::projectPoints( cv::Mat(threed_points_wo_blanks), rvec, tvec, cameraMatrix,  // project
//			distCoeffs, imagePoints2);
//	err = cv::norm(cv::Mat(twod_points_wo_blanks), cv::Mat(imagePoints2), CV_L2);              // difference
//	reproj_error        += err*err;
//
//	// scale by the number of points ....
//	reproj_error = reproj_error / double(number_points);
//
//
//	if (write){
//		// different colors for the different types.
//		Scalar line_color(0, 0, 0);
//
//		switch (type){
//		case 0: {
//			line_color = Scalar(255, 0, 0);
//		} break;
//		case 1: {
//			line_color = Scalar(0, 255, 0);
//		} break;
//		case 2: {
//			line_color = Scalar(0, 0, 255);
//		} break;
//
//		}
//
//
//		Mat im_copy = images[image_number].clone();
//
//		for (int j = 0, jn = imagePoints2.size(); j < jn; j++){
//			line(im_copy, twod_points_wo_blanks[j],imagePoints2[j], line_color, 2 );
//		}
//
//		string filename = write_directory + "Equation" + ToString<int>(equation_number) + ".png";
//
//		imwrite(filename.c_str(), im_copy);
//	}
//
//	return reproj_error;
//
//}


// X vector<vector<bool> > points_present;
//	X vector<MatrixXd> two_d_point_coordinates_dense; /// this is initialized to 0, 0.  points present tell us whether a point is there or not.
//	X vector<vector<int> > points_per_board; // for the internal calibration, these need to be set up per board.  Later, we will combine per image.
//	vector<vector<bool> > boards_detected; /// theoretically, more than one visible in each frame.  Deal with this possibility NOW -- worst case first.
//	vector< vector<double> > reproj_error_per_board;
//
//	vector<vector<bool> > has_calibration_estimate;



//void CameraCali::Compare(CameraCali* C){
//
//	bool difference_found = false;
//	double diff;
//	double diff_tol = 0.000001;
//
//	cout << "Scanning two_d_point_coordinates_dense + points_present " << endl;
//
//	// want to see how these are different....
//	if (points_present.size() != C->points_present.size()){
//		cout << "Difference in top-level points-present size " << points_present.size() << ", " << C->points_present.size() << endl;
//	}	else {
//		for (int j = 0; j < int(points_present.size()); j++){
//			if (points_present[j].size() != C->points_present[j].size()){
//				cout << "Difference in next-level points-present size " << j << ": " <<  points_present[j].size() << ", " << C->points_present[j].size() << endl;
//			}	{
//				for (int k = 0; k < int(points_present[j].size()); k++){
//					if (points_present[j][k] != C->points_present[j][k]){
//						cout << "points present indexing error at " << ", " << j << ", " << k << endl;
//
//						//two_d_point_coordinates_dense[i](global_index, 0) = charucoCorners[j].x;
//						//							two_d_point_coordinates_dense[i](global_index, 1)
//					}
//
//
//
//
//					diff = pow(two_d_point_coordinates_dense[j](k, 0) - C->two_d_point_coordinates_dense[j](k, 0),2) +
//							pow(two_d_point_coordinates_dense[j](k, 1) - C->two_d_point_coordinates_dense[j](k, 1),2);
//
//					if (sqrt(diff) > diff_tol){
//						cout << "diff between dense two at " << ", " << j << ", " << k << " " << sqrt(diff) << endl;
//					}
//				}
//			}
//		}
//	}
//
//	cout << "Scanning points per board " << endl;
//	if (points_per_board.size() != C->points_per_board.size()){
//		cout << "Mismatch top-level points per board size " << points_per_board.size() << ", " << C->points_per_board.size() << endl;
//	}	else {
//		for (int i = 0; i < int(points_per_board.size()); i++){
//			if (points_per_board[i].size() != C->points_per_board.size()){
//				cout << "Mismatch second-level points per poard size " << points_per_board[i].size() << ", " << C->points_per_board.size() << endl;
//			}	else {
//
//				for (int p = 0; p < int(points_per_board[i].size()); p++){
//					if (points_per_board[i][p] != C->points_per_board[i][p]){
//						cout << "points per board " << i << ", " << p << " are different " << points_per_board[i][p]  << ", " << C->points_per_board[i][p] << endl;
//					}
//				}
//			}
//		}
//	}
//
//	cout << "Scanning has cali estimate " << endl;
//	if (has_calibration_estimate.size() != C->has_calibration_estimate.size()){
//			cout << "Mismatch top-level has cali estimate " << has_calibration_estimate.size() << ", " << C->has_calibration_estimate.size() << endl;
//		}	else {
//			for (int i = 0; i < int(has_calibration_estimate.size()); i++){
//				if (has_calibration_estimate[i].size() != C->has_calibration_estimate.size()){
//					cout << "Mismatch second-level has_calibration_estimate size " << has_calibration_estimate[i].size() << ", " << C->has_calibration_estimate.size() << endl;
//				}	else {
//
//					for (int p = 0; p < int(has_calibration_estimate[i].size()); p++){
//						if (has_calibration_estimate[i][p] != C->has_calibration_estimate[i][p]){
//							cout << "has_calibration_estimate " << i << ", " << p << " are different " << has_calibration_estimate[i][p]  << ", " << C->has_calibration_estimate[i][p] << endl;
//						}
//					}
//				}
//			}
//		}
//
//	cout << "Scanning two d + three d dense " << endl;
//	//vector< vector< cv::Point2f> >
//	if ((twod_points_wo_blanks_class.size() != C->twod_points_wo_blanks_class.size()) || (threed_points_wo_blanks_class.size() != C->threed_points_wo_blanks_class.size())){
//		cout << "Difference in top-level twod wo blanks size size " << twod_points_wo_blanks_class.size() << ", " << C->twod_points_wo_blanks_class.size() << endl;
//	}	else {
//
//		for (int i  = 0; i < int(twod_points_wo_blanks_class.size()); i++){
//			if ((twod_points_wo_blanks_class[i].size() != C->twod_points_wo_blanks_class[i].size()) ||(threed_points_wo_blanks_class[i].size() != C->threed_points_wo_blanks_class[i].size()) ){
//				cout << "Difference in next level twod size " << i << endl;
//
//			}	else {
//				for (int j=  0; j < int(twod_points_wo_blanks_class[i].size()); j++){
//
////					if (twod_points_wo_blanks_class[i][j].x != C->twod_points_wo_blanks_class[i][j].x){
////						cout << "2d points diff " << twod_points_wo_blanks_class[i][j].x << ", " << C->twod_points_wo_blanks_class[i][j].x << " diff " << abs() << endl;
////					}
//					diff = pow(twod_points_wo_blanks_class[i][j].x - C->twod_points_wo_blanks_class[i][j].x, 2) +
//							pow(twod_points_wo_blanks_class[i][j].y - C->twod_points_wo_blanks_class[i][j].y, 2);
//
//					if (sqrt(diff) > diff_tol){
//						cout << "diff between 2d wo blanks at " << ", " << i << ", " << j << endl;
//					}
//
//					diff = pow(threed_points_wo_blanks_class[i][j].x - C->threed_points_wo_blanks_class[i][j].x, 2) +
//							pow(threed_points_wo_blanks_class[i][j].y - C->threed_points_wo_blanks_class[i][j].y, 2) +
//							pow(threed_points_wo_blanks_class[i][j].z - C->threed_points_wo_blanks_class[i][j].z, 2);
//
//					if (sqrt(diff) > diff_tol){
//						cout << "diff between 3d wo blanks at " << ", " << i << ", " << j << endl;
//					}
//				}
//			}
//		}
//	}
//
//	cout << "Scanning internal " << endl;
//	Matrix3d diff_mat = internal_parameters-C->internal_parameters;
//	diff = diff_mat.norm();
//
//	cout << "internal constraints diff " << diff << endl;
//
//	/// try to calibrate, here.  What's the difference?
//
//	if (C->images.size() == 0 || images.size() == 0){
//		cout << "error!  No images in C " << endl;
//	}	else {
//
//		cv::Size image_size = Size(images[0].cols, images[0].rows);
//
//
//		cv::Mat cameraMatrix0 = cv::Mat::eye(3, 3, CV_64F);
//
//		cv::Mat distCoeffs0 = cv::Mat::zeros(4, 1, CV_64F);
//
//
//		vector<cv::Mat> rvecs0, tvecs0;
//
//		cameraMatrix0.at<double>(0, 0) = 1000;
//		cameraMatrix0.at<double>(1, 1) = 1000;
//
//		cameraMatrix0.at<double>(0, 2) = images[0].cols/2;
//		cameraMatrix0.at<double>(1, 2) = images[0].rows/2;
//
//
//		cv::Mat cameraMatrix1 = cv::Mat::eye(3, 3, CV_64F);
//
//		cv::Mat distCoeffs1 = cv::Mat::zeros(4, 1, CV_64F);
//
//
//		vector<cv::Mat> rvecs1, tvecs1;
//
//		cameraMatrix1.at<double>(0, 0) = 1000;
//		cameraMatrix1.at<double>(1, 1) = 1000;
//
//		cameraMatrix1.at<double>(0, 2) = images[0].cols/2;
//		cameraMatrix1.at<double>(1, 2) = images[0].rows/2;
//
////		double rms1 = cv::calibrateCamera(C->threed_points_wo_blanks_class, C->twod_points_wo_blanks_class, image_size, cameraMatrix1,
////								distCoeffs1, rvecs1, tvecs1, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);
////
////						cout << "C rms " << rms1 << endl;
////						cout << "C matrix " << endl << cameraMatrix1 << endl;
//
//		double rms1 = cv::calibrateCamera(threed_points_wo_blanks_class, C->twod_points_wo_blanks_class, image_size, cameraMatrix1,
//				distCoeffs1, rvecs1, tvecs1, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);
//
//		cout << "rms " << rms1 << endl;
//		cout << "matrix " << endl << cameraMatrix1 << endl;
//
//
//		double rms0 = cv::calibrateCamera(threed_points_wo_blanks_class, twod_points_wo_blanks_class, image_size, cameraMatrix0,
//				distCoeffs0, rvecs0, tvecs0, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);
//
//		cout << "rms " << rms0 << endl;
//		cout << "CCV matrix " << endl << cameraMatrix0 << endl;
//
//
//
//
//	}
//
//	// DO this
////	if (points_per_board[j][k] != C->points_per_board[j][k]){
////							cout << "points per board indexing error at " << ", " << j << ", " << k << endl;
////							cout << points_per_board[j][k] << ", " << C->points_per_board[j][k] << endl;
////						}
//
//
//	char ch;  cin >> ch;
//}

//
//
//void CaliObjectOpenCV2::CalibrateFlexibleExternal(std::ofstream& out, string write_directory){
//
//	// need to make the points 3D
//
//	vector< cv::Point3f> corners(chess_h*chess_w);
//	string filename;
//
//
//	cout << "Calibrating using " << all_points.size() << " patterns " << endl;
//
//	int counter = 0;
//	for( int i = 0; i < chess_h; ++i ){
//		for( int j = 0; j < chess_w; ++j, counter++ ){
//			corners[counter] = (cv::Point3f(float( j*mm_width ), float( i*mm_height ), 0));
//		}
//	}
//
//	// b/c all of the positions are the same ....
//	// create map ....
//	vector<bool> pattern_detected;
//	vector<int> mapping_from_limited_to_full;
//	vector< vector< cv::Point2f> > all_points_wo_blanks;
//
//	for (int i = 0; i < int(all_points.size()); i++){
//		if (all_points[i].size() > 0){
//			mapping_from_limited_to_full.push_back(i);
//			all_points_wo_blanks.push_back(all_points[i]);
//			all_3d_corners.push_back(corners);
//			pattern_detected.push_back(true);
//		}	else {
//			pattern_detected.push_back(false);
//		}
//
//	}
//
//	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
//
//	cv::Mat distCoeffs = cv::Mat::zeros(4, 1, CV_64F);
//	//cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
//
//	vector<cv::Mat> rvecs, tvecs;
//
//	if (image_size.width > 640){
//		cameraMatrix.at<double>(0, 0) = 2500;
//		cameraMatrix.at<double>(1, 1) = 2500;
//	}	else {
//		cameraMatrix.at<double>(0, 0) = 1800;
//		cameraMatrix.at<double>(1, 1) = 1800;
//		cameraMatrix.at<double>(0, 0) = 1000;
//		cameraMatrix.at<double>(1, 1) = 1000;
//
//		cameraMatrix.at<double>(0, 0) = 600;
//		cameraMatrix.at<double>(1, 1) = 600;
//	}
//	cameraMatrix.at<double>(0, 2) = image_size.width/2;
//	cameraMatrix.at<double>(1, 2) = image_size.height/2;
//
//	cout << "initial camera matrix " << endl;
//
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			cout << cameraMatrix.at<double>(i, j) << " ";
//		}
//		cout << endl;
//	}
//
//	cout << "Running calibration " << endl;
//	cout << "Number of dist coefficients  = " << distCoeffs.rows << endl;
//	//LevMarCameraCaliNoDistortion(all_points, all_3d_corners, out);
//	//double rms = cv::calibrateCamera(all_3d_corners, all_points, image_size, cameraMatrix, distCoeffs, rvecs, tvecs, CV_CALIB_USE_INTRINSIC_GUESS,
//	//		cv::TermCriteria( cv::TermCriteria::COUNT, 2, DBL_EPSILON) );  //, CV_CALIB_RATIONAL_MODEL );
//	//CV_CALIB_FIX_PRINCIPAL_POINT |
//	//double rms = cv::calibrateCamera(all_3d_corners, all_points, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//		CV_CALIB_USE_INTRINSIC_GUESS| CV_CALIB_RATIONAL_MODEL);
//
//
//	// this one does not use the initial guess
//	//double rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//		CV_CALIB_RATIONAL_MODEL);
//
//	double rms = 0;
//	char ch;
////	cout << "Before first " << endl; cin >> ch;
////	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
////							CV_CALIB_RATIONAL_MODEL);
//
//
//	if (text_file.size() == 0){
//		// submitted Transactions paper has rational model
//		//rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//		//		CV_CALIB_RATIONAL_MODEL);
//
//		rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs);
//
//	}	else {
//		ifstream in(text_file.c_str());
//		string temp;
//		in >> cameraMatrix.at<double>(0, 0);
//		in >> temp;
//		in >> cameraMatrix.at<double>(0, 2);
//		in >> temp;
//		in >> cameraMatrix.at<double>(1, 1);
//		in >> cameraMatrix.at<double>(1, 2);
//		in >> temp >> temp >> temp;
//		in.close();
//
//		cout << "initial camera matrix " << endl;
//
//		for (int i = 0; i < 3; i++){
//			for (int j = 0; j < 3; j++){
//				cout << cameraMatrix.at<double>(i, j) << " ";
//			}
//			cout << endl;
//		}
//
////		cout << "Before first " << endl; cin >> ch;
////		rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
////						CV_CALIB_RATIONAL_MODEL);
//
//		//cout << "Before second " << endl; cin >> ch;
//		// submitted Transactions paper has rational model
//		//rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//		//		CV_CALIB_RATIONAL_MODEL| CV_CALIB_USE_INTRINSIC_GUESS);
//
//	//	rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//	//					CV_CALIB_USE_INTRINSIC_GUESS);
//		// OpenCV versions
//		rms = cv::calibrateCamera(all_3d_corners, all_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs,
//								CALIB_USE_INTRINSIC_GUESS);
//	}
//
//
//	cout << "rms " << rms << endl;
//	cout << "camera matrix " << endl;
//
//
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			cout << cameraMatrix.at<double>(i, j) << " ";
//		}
//		cout << endl;
//	}
//
//	cout << "Distortion " << endl;
//	for (int i = 0; i < distCoeffs.rows; i++){
//		cout << distCoeffs.at<double>(i, 0) << " ";
//	}
//	cout << endl;
//
//
//	out << "Internal images used: " << number_internal_images_written << endl;
//	out << "External images used: " << external_images.size() << endl;
//	out << "rms " << rms << endl;
//	out << "camera matrix " << endl;
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			out << cameraMatrix.at<double>(i, j) << " ";
//		}
//		out << endl;
//	}
//
//	out << "Distortion " << endl;
//	for (int i = 0; i < distCoeffs.rows; i++){
//		out << distCoeffs.at<double>(i, 0) << " ";
//	}
//	out << endl;
//
//	cv::Mat rotMatrix = cv::Mat::eye(3, 3, CV_64F);
//	vector< vector <double> > tempRt(3, vector<double>(4, 0));
//
//	A.resize(3, vector<double>(3, 0));
//	k.resize(8, 0);
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			A[i][j] = cameraMatrix.at<double>(i, j);
//		}
//	}
//
//	for (int i = 0; i < distCoeffs.rows; i++){
//		k[i] = distCoeffs.at<double>(i, 0);
//	}
//	cout << "Line 13378 " << endl;
//	//char ch; cin >> ch;
//
//	double reproj_error = 0;
//	vector<cv::Point2f> imagePoints2;
//	double err;
//
//	for (int m = number_internal_images_written; m < int(all_points_wo_blanks.size()); m++){
//
//		cv::projectPoints( cv::Mat(all_3d_corners[m]), rvecs[m], tvecs[m], cameraMatrix,  // project
//				distCoeffs, imagePoints2);
//		err = cv::norm(cv::Mat(all_points_wo_blanks[m]), cv::Mat(imagePoints2), CV_L2);              // difference
//		reproj_error        += err*err;
//
//
//		// su
//	}
//
//	out << endl << "Summed reproj error " << reproj_error << endl << endl;
//
//	//Matrix temp;
//	for (int m = number_internal_images_written; m < int(all_points.size()); m++){
//		Rts.push_back(vector<vector <double> >());
//	}
//
////	cout << "Line 1364 " << endl;
////	cout << "number internal images written " << number_internal_images_written << endl;
////	cout << "Number of total pattterns " << all_points_wo_blanks.size() << endl;
////	cout << "Number rvecs " << rvecs.size() << endl;
////	cout << "Number Rts " << Rts.size() << endl;
////	cin >> ch;
//	// we only want these for the external images ....
//	for (int m = number_internal_images_written; m < int(all_points_wo_blanks.size()); m++){
//
//
//		cv::Rodrigues(rvecs[m], rotMatrix);
//		//cout << "after conversion " << endl; cin >> ch;
//
//		for (int i = 0; i < 3; i++){
//			for (int j = 0; j < 3; j++){
//				tempRt[i][j] = rotMatrix.at<double>(i, j);
//			}
//
//			tempRt[i][3] = tvecs[m].at<double>(i);
//		}
////		cout << "after tempRT " << endl; cin >> ch;
////
////		cout << "mapping " << mapping_from_limited_to_full.at(m) << endl; cin >> ch;
//
//		//Rts.push_back(tempRt);
//		Rts[mapping_from_limited_to_full.at(m) - number_internal_images_written] = tempRt;
//		//cout << "RTs " << endl; cin >> ch;
//
//		out << "Rt for cam " << mapping_from_limited_to_full[m] - number_internal_images_written << endl;
//		for (int i = 0; i < 3; i++){
//			for (int j = 0; j < 4; j++){
//				out << tempRt[i][j] << " ";
//			}
//			out << endl;
//		}
//
//		out << endl;
//	}
//
//	cout << "Finish with cali ... " << endl;
//	//cin >> ch;
//
//	cv::Mat view, rview, map1, map2;
//	cv::Mat gray;
//	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
//			cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0),
//			image_size, CV_16SC2, map1, map2);
//
//	for (int i = 0; i < int(external_images.size()); i++){
//		cout << "Writing external " << i << endl;
//		cv::remap(external_images[i], rview, map1, map2, cv::INTER_LINEAR);
//
//		//cv::cvtColor(rview, gray, CV_BGR2GRAY);
//		//cv::cvtColor(gray, rview, CV_GRAY2BGR);
//		filename  = write_directory + "/ext" + ToString<int>(i) + ".png";
//		cv::imwrite(filename.c_str(), rview);
//	}
//}
//

void FindChessboardCorners(){
	//
	//im = external_images[i];
	//	cv::cvtColor(im, gimage, CV_BGR2GRAY);
	//
	//	//corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf,  CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FILTER_QUADS);
	//	// OpenCV version differences
	//	corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf,  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK + CALIB_CB_FILTER_QUADS);
	//
	//	if (!corner_found){
	//		cout << "Trying default option " << endl;
	//		corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf);
	//	}
	//
	//	if (!corner_found){
	//		cout << "Trying  option one" << endl;
	//		//corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf, CV_CALIB_CB_NORMALIZE_IMAGE);
	//		corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf,  CALIB_CB_NORMALIZE_IMAGE);
	//	}
	//
	//	if (!corner_found){
	//		cout << "Trying  option two" << endl;
	//		//corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf, CV_CALIB_CB_FILTER_QUADS);
	//		corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf,  CALIB_CB_FILTER_QUADS);
	//	}
	//
	//	if (!corner_found){
	//		cout << "Trying  option three" << endl;
	//		//corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH);
	//		corner_found = cv::findChessboardCorners(gimage, boardsize, pointBuf,  CALIB_CB_ADAPTIVE_THRESH);
	//	}
	//
	//	if (corner_found) {
	//
	//		// need to flip the orientation, possibly ...
	//		first_point = pointBuf[0];
	//		last_point = pointBuf[chess_w*chess_h - 1];
	//
	//		if (first_point.y < last_point.y){
	//			//if (first_point.x > last_point.x){
	//			cout << "WRONG ORIENTATION! " << endl;
	//
	//
	//			for (int k=0; k<corner_count; k++) {
	//
	//				flipped_points[k] = pointBuf[chess_w*chess_h - 1 - k];
	//
	//			}
	//
	//			pointBuf.swap(flipped_points);
	//
	//		}
	//
	//		some_found = true;
	//		// refine the corner positions
	//		cv::cornerSubPix( gimage, pointBuf, cv::Size(11,11),
	//				cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
	//
	//
	//		if (draw_corners){
	//			// draw detected corner points in the current frame
	//			cv::drawChessboardCorners(external_images[i], boardsize, cv::Mat(pointBuf), true);
	//		}
	//
	//		all_points.push_back(pointBuf);
	//
	//
	//		cout << "Number of patterns " << all_points.size() << endl;
	//	}	else {
	//		all_points.push_back(vector<cv::Point2f>());
	//	}
	//}
	//
}
