/*
 * camera_calibration.cpp
 *
 *  Created on: Jun 26, 2018
 *      Author: atabb
 */

#include "camera_calibration.hpp"
#include "DirectoryFunctions.hpp"


string CreatePaddedNumberString(int number, int length){

	string temp_string = ToString<int>(number);

	int n = length - temp_string.size();

	for (int i = 0; i < n; i++){
		temp_string = "0" + temp_string;
	}

	return temp_string;
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
	int sX, sY, sL, mL, margin, ac;

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

	int_number_markers = 0;

	number_total_squares = sX*sY;
	cout << "Number total squares " << number_total_squares << endl;

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

	CreateTemplateArucoImages(images, sX, sY, sL, mL, margin, ac);

	filename = read_dir + "created_template.png";
	imwrite(filename.c_str(), images[0]);

	cout << "After create image " << endl;

	Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();


	bool readOk = readDetectorParameters(string("../src/detector_params.yml"), detectorParams);
	if(!readOk) {
		cout << "Invalid detector parameters file, quitting" << endl;
		exit(1);
	}


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

CameraCali::CameraCali(string read_dir, PatternsCreated* P){
	pixel_width = 0;
	count_internal_ids_present = 0;
	sensor_width  = 0;

	P_class = P; // have a ptr to all the full information about the patterns.

	// load the images ...

	string filename;
	string txt_ext = "txt";
	Mat im;

	string ext_dir = read_dir + "/external/";
	cout << "Ext dir! " << ext_dir << endl;

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

		im_short_names = im_names;
		new_names.swap(im_names);
	}

	/// read the internal dir.
	string int_dir = read_dir + "/internal/";
	dir_exists = IsDirectory(int_dir);
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

	for (int j = 0;  j < np; j++){ // don't walk this one if it is already occupied.
		if (id >= min_max_id_pattern[j].first && id <= min_max_id_pattern[j].second){
			return j;
		}
	}

	return -1;
}


Scalar PatternsCreated::Color(int index){

	if (index >= int(display_colors.size()) ){
		index = display_colors.size() % index;
	}
	return Scalar(display_colors[index][0], display_colors[index][1],display_colors[index][2]);
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

	int number_squares = P_class->NumberSquares();

	string filename;
	int number_images = images.size();
	Mat imageCopy;
	Scalar b_color;
	MatrixXd twod(number_squares, 2);
	twod.setConstant(0);

	boards_detected.resize(number_images, vector<bool>(P_class->NumberPatterns(), false));


	std::ofstream out;
	filename = write_dir + "points.txt";
	out.open(filename.c_str());


	/// also, read exif.
	int internal_pattern_max_squares = P_class->max_internal_patterns*4;
	MatrixXd twod_internal(internal_pattern_max_squares, 2);


	if (images.size() == 0){
		cout << "the number of images is 0, quitting " << endl;
		exit(1);
	}	else {
		cout << "Number of images " << images.size() << endl;
	}


	int current_index;
	int x_value = 0;
	int y_value = 0;

	//Mat gray;
	for (int i = 0, nimages = images.size(); i < nimages; i++){
		imageCopy = images[i].clone();

		/// already know the number of points per board.
		internal_two_d_point_coordinates_dense.push_back(twod_internal);


		count_internal_ids_present = 0;
		vector< int > ids;
		vector< vector< Point2f > > corners, rejected;

		patterns_present.push_back(vector<bool>(P_class->max_internal_patterns, false));

		// detect markers and estimate pose
		aruco::detectMarkers(images[i], P_class->dictionary, corners, ids, detectorParams, rejected);
		cout << "After detect " << endl;

		if(ids.size() > 0) {
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

		//filename = write_dir + "internal_initial_detect" + ToString<int>(i) + ".jpg";
		filename =  write_dir + "aruco_detect" + im_short_names[i];
		imwrite(filename.c_str(), imageCopy);


	}

	out.close();
}

void CameraCali::CalibrateArucoSinglyAndUndistort(string write_dir, double homography_scaling){


	/// want to recover pose after calibration ...need a map.
	vector<int> mapping_from_limited_to_full_images;
	vector<int> mapping_from_limited_to_full_patterns;


	int number_images = images.size();
	int number_patterns = P_class->max_internal_patterns;

	has_calibration_estimate.resize(number_images, vector<bool>(number_patterns, false));

	cv::Size image_size;


	string filename = write_dir + "results.txt";
	std::ofstream out;
	out.open(filename.c_str());

	/// start a text file with all of the information --- internal, distortion, external.
	out << "Scaling " << homography_scaling << endl;

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
		//int last_added = 0;
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


		cout << "Running calibration " << endl;

		double rms = 0;

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

		//filename  = write_dir + "/undistorted" + ToString<int>(i) + ".jpg";
		filename =  write_dir + "undistorted" + im_short_names[i];
		cv::imwrite(filename.c_str(), rview);

		// homography /////////////////////

		//cv::Mat newCameraMatrix = cv::Mat::eye(3, 3, CV_64F);
		vector< cv::Point2f> undistorted_points;
		Mat newCameraMatrix = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, image_size, 1, image_size, 0);


		undistortPoints(twod_points_wo_blanks[0], undistorted_points, cameraMatrix, distCoeffs, cv::noArray(), newCameraMatrix);



		// H will find the homography AKA transformation onto the plane where pixels == some representation in mm.
		cout << "matches " << endl;
		for (int m = 0; m < 4; m++){
			cout << "i" << i << " " << undistorted_points[m].x << "," << undistorted_points[m].y << " 3d ";
			cout << caliObjectPointsPlanar[m].x << ", " << caliObjectPointsPlanar[m].y << endl;


		}

		Mat H = findHomography(undistorted_points, caliObjectPointsPlanar);
		out << "H:" << endl << H << endl;


		/// this image needs to be as big as the 3d space... -- in other words, the largest coordinate in the 3d coords.

		warpPerspective(rview, img1_warp, H, img1_warp.size());
		//filename  = write_dir + "/warped" + ToString<int>(i) + ".jpg";
		filename =  write_dir + "warped" + im_short_names[i];
		cv::imwrite(filename.c_str(), img1_warp);

	}

out.close();

}



void CameraCali::CalibrateBasic(string write_dir){


	/// want to recover pose after calibration ...need a map.
	vector<int> mapping_from_limited_to_full_images;
	vector<int> mapping_from_limited_to_full_patterns;
	// create a collection of the points for each image -- hopefully this will work. map -- .
	vector< vector< cv::Point2f> > twod_points_wo_blanks;
	vector< vector< cv::Point3f> > threed_points_wo_blanks;



	int number_images = images.size();
	//int number_squares = P_class->NumberSquares();
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
					last_added = twod_points_wo_blanks.size() - 1;
					for (int j = P_class->min_max_id_squares[p].first; j <= P_class->min_max_id_squares[p].second; j++){
						if (points_present[i].at(j) == true){
							twod_points_wo_blanks[last_added][s].x = two_d_point_coordinates_dense[i](j, 0);
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

	cout << "Running calibration " << endl;
	//cout << "Number of dist coefficients  = " << distCoeffs.rows << endl;

	double rms = 0;

	rms = cv::calibrateCamera(threed_points_wo_blanks, twod_points_wo_blanks, image_size, cameraMatrix, distCoeffs, rvecs, tvecs, CALIB_FIX_K3 | CALIB_USE_INTRINSIC_GUESS);

	cout << "rms " << rms << endl;


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




