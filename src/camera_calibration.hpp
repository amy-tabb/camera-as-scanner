/*
 * camera_calibration.hpp
 *
 *  Created on: Jun 26, 2018
 *      Author: atabb
 */

#ifndef CAMERA_CALIBRATION_HPP_
#define CAMERA_CALIBRATION_HPP_

#include <iostream>


#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <inttypes.h>
#include <parallel/algorithm>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <omp.h>


#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "Includes.hpp"


using namespace Eigen;
using namespace cv;
using std::vector;
using std::ifstream;
using std::string;


using std::set;
using std::max;
using std::min;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::map;
using std::pair;

// double to single mapping -- take 0 9 to number

static bool readDetectorParameters(string filename, Ptr<aruco::DetectorParameters> &params) {
	FileStorage fs(filename, FileStorage::READ);
	if(!fs.isOpened())
		return false;
	fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
	fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
	fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
	fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
	fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
	fs["maxMarkerPerimeterRate"] >> params->maxMarkerPerimeterRate;
	fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
	fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
	fs["minDistanceToBorder"] >> params->minDistanceToBorder;
	fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
	//fs["doCornerRefinement"] >> params->doCornerRefinement;
	fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
	fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
	fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
	fs["markerBorderBits"] >> params->markerBorderBits;
	fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
	fs["perspectiveRemoveIgnoredMarginPerCell"] >> params->perspectiveRemoveIgnoredMarginPerCell;
	fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
	fs["minOtsuStdDev"] >> params->minOtsuStdDev;
	fs["errorCorrectionRate"] >> params->errorCorrectionRate;
	return true;
}


class PatternsCreated{
public:
	vector<vector<int> > double_to_single;
	vector< cv::Point3f> three_d_points;
	vector< cv::Point3f> three_d_points_internal; // for strawberry case-- TODO get rid of
	Ptr<aruco::Dictionary> dictionary;
	vector<cv::Ptr<cv::aruco::CharucoBoard> > boards; /// for refining the estimate of corner locations -- todo get rid of
	vector< vector<int> > display_colors;
	vector<pair<int, int> > min_max_id_pattern;
	vector<pair<int, int> > min_max_id_squares;
	bool single_aruco_markers;
	vector<int> single_aruco_ids;
	int max_internal_patterns;
	int internalx, internaly; /// remove todo

	//PatternsCreated(string read_dir);
	PatternsCreated(string read_dir, string write_directory, bool aruco_markers);


	void DetermineBoardsPresentFromMarkerList(vector<int>& markers, vector<bool>& boards_seen);
	int MappingArucoIDToPatternNumber(int id);
	//int SelectPattern(int CharucoID);

	Scalar Color(int index);

	int NumberMarkers();
	int NumberSquares();
	int NumberPatterns();

	void SetNumberMarkers(int n);
	void SetNumberPatterns(int n);

protected:
	int number_patterns;
	int number_total_squares;
	int int_number_markers;

};


class CameraCali{
public:
	// there are multiple patterns which may or may not be viewable in an image.  The particular pattern is indexed first, then whether the point was discovered or not is indexed second.
	// the default is that all are empty.

	// so the patterns create an ordering of the points ... 0 ... n-1. #1 n to 2n.  We have a map in PatternsCreated in case there is a pattern with an unusual number of squares.
	vector<vector<bool> > points_present; //charuco
	vector<vector<bool> > patterns_present; //aruco
	vector<MatrixXd> two_d_point_coordinates_dense; /// this is initialized to 0, 0.  points present tell us whether a point is there or not.
	vector<vector<int> > points_per_board; // for the internal calibration, these need to be set up per board.  Later, we will combine per image.
	vector<vector<bool> > boards_detected; /// theoretically, more than one visible in each frame.  Deal with this possibility NOW -- worst case first.
	vector< vector<double> > reproj_error_per_board;

	vector<vector<bool> > has_calibration_estimate;
	vector<string> im_names;

	//vector<vector<bool> > internal_points_present;
	vector<MatrixXd> internal_two_d_point_coordinates_dense;
	int count_internal_ids_present;
	vector<bool> id_bool;
	vector<double> focal_lengths;
	vector<double> pix_widths;
	double sensor_width;
	double pixel_width;
	int rows;
	int cols;
	//int straw_pattern_x;
	//int straw_pattern_y;

	//vector< vector< cv::Point2f> > twod_points_wo_blanks_class;
	//vector< vector< cv::Point3f> > threed_points_wo_blanks_class;


	vector<Mat> images; // the calibration images in the set
	int number_external_images_max;
	vector<Mat> reproject_cam_cali_images;

	Matrix3d internal_parameters;
	VectorXd distortion;
	vector<vector< Matrix4d> > external_parameters;

	PatternsCreated* P_class;

	vector<int> internal_pattern_indices;
	vector<MatrixXd> two_d_point_coordinates_segmentation_external;
	vector<MatrixXd> two_d_point_coordinates_segmentation_internal;

	//?? how should do this, with gaps or not, how best to organize this so that it makes sense?
	// probably with gaps, so that it matches points present for each image.  To calibrate, just line up with a fresh copy, no gaps.
	// vector< vector< cv::Point2f> > internal_points;

	CameraCali(string read_dir, PatternsCreated* P);

	void FindCornersCharuco(string write_dir);

	void FindCornersAruco(string write_dir);

	void ReadCorners(string read_dir);

	void ReadCalibration(string read_dir);

	// only for really bad problems
	//void Compare(CameraCali* C);

	void CalibrateBasic(string write_dir);

	void CalibrateStrawberrySet(string write_dir);

	double ComputeReprojectionErrorOneImagePattern(Matrix4d& ExtParameters, int camera_number, int image_number,
			int pattern_number, int type, string write_directory, bool write, int equation_number, bool is_strawberry, Matrix3d* IntParameters = 0);

	void ReadExifInformationStrawberry(string read_dir);

	void ReadExifInformationForAllImages(string image_read_dir, string parent_dir);

	void ReadStrawberryInternalCorners(string read_dir);

	void CopyToMats(Mat& CameraMatrix, Mat& dis);

	void CalibrateArucoGeneralID(string write_dir);

	void FindCornersArucoGeneral(string write_dir);


};

string CreatePaddedNumberString(int number, int length);

int CreateStrawberryImagesCharucoExp(vector<Mat>& images, int squaresX, int squaresY, int squareLength, int markerLength,
		int margins, int id_start_number);



#endif /* CAMERA_CALIBRATION_HPP_ */
