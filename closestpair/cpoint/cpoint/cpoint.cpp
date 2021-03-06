﻿/**
* Closest Pair of Points
* Given a set of points, this program returns the closest pair in 2D or 3D space.
* CSCI-7432 - Algorithms and Data Structures
*
* The 2D implementation is complete but the 3D implementation is still in progress. I'm 
* attempting to use the same code for both with the program seamlessly switching between the two.
*
* This program isn't quite fully optimized yet -- the main function has copying of vectors
* which I didn't optimize. There are also a few other places where I should have been
* passing pointers instead of manipulating data.
*
* Paul Bupe Jr
*/

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector> // Holding our 2D arrays
#include <algorithm> // Using C++'s fast quicksort implementation
#include <fstream>
#include <iterator>
#include <math.h>

// Input file. Expects a single comma separated point per line
std::ifstream input("points2d.txt");


// Representation of a point in 2D or 3D space
struct Point
{
	double x, y, z;
	bool is_3d = false;
	// Overload the >> operator to make parsing the input file cleaner
	friend std::istream& operator >>(std::istream& is, Point& p)
	{
		char ch;
		is >> p.x >> ch >> p.y;
		return is;
	}
};


// Predicate used by std::find_if to check if point is in vector
bool point_is_same(Point& p1, Point& p2, bool is_3d = false) {
	return is_3d ? ((p1.x == p2.x) && (p1.y == p2.y) && (p1.z == p2.z)) : ((p1.x == p2.x) && (p1.y == p2.y));
};

// Distance between two points -- the usual formula.
double find_dist(Point& p1, Point& p2, bool is_3d = false)
{
	return is_3d ? sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z)) 
		: sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}


// This function is used in the final stage of the recurssion to return the two closest points 
std::pair<Point, Point> brute_force_pair(std::vector<Point> &points, bool is_3d=false)
{
	// Initialize min to infinity so we don't have to worry about max values for points
	double min = std::numeric_limits<double>::infinity();

	std::pair<Point, Point> closest_pair;
	int n = points.size();

	if (is_3d) {
		for (int i = 0; i < n; ++i) {
			for (int j = i + 1; j < n; ++j) {
				double dist = find_dist(points.at(i), points.at(j), true);
				if (dist < min) {
					min = dist;
					closest_pair = std::pair<Point, Point>(points.at(i), points.at(j));
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < n; ++i) {
			for (int j = i + 1; j < n; ++j) {
				double dist = find_dist(points.at(i), points.at(j));
				if (dist < min) {
					min = dist;
					closest_pair = std::pair<Point, Point>(points.at(i), points.at(j));
				}
			}
		}
	}

	return std::pair<Point, Point> (closest_pair);
}


std::pair<Point, Point> find_closest_pair(std::vector<Point> &points, std::vector<Point> &points_x, 
	std::vector<Point> &points_y, bool is_3d=false) {
	
	// TODO: Finish implementing the 3D equivalent for the recursive operations

	// Get the size of our collection
	const int n = points.size();

	// If there are 3 or less points then let's go ahead and bruteforce it!
	// This is also the exit condition of our recursion 
	if (n < 4) return brute_force_pair(points);

	// Let's divide our sorted X collection in half and also copy into our unordered sets
	std::vector<Point> x_left(points_x.begin(), points_x.begin() + n / 2);
	std::vector<Point> x_right(points_x.begin() + n / 2, points_x.end());

	std::vector<Point> points_left(x_left.begin(), x_left.end());
	std::vector<Point> points_right(x_right.begin(), x_right.end());

	// Time to split the sorted Y by comparing to the two collections we just created above
	// I'm getting really sleepy at this point..
	std::vector<Point> y_left;
	std::vector<Point> y_right;

	for (int i = 0; i < n; ++i) {

		// If we find this value in out left collection...
		//if (std::find(points_left.begin(), points_left.end(), points_y[i]) != points_left.end())
		//std::vector<Point_2d> curr_y = points_y.at(i);
		auto it = std::find_if(points_left.begin(), points_left.end(), [&](const auto& p) {return (p.x == points_y.at(i).x) && (p.y == points_y.at(i).y); });
		//auto it = std::find_if(points_left.begin(), points_left.end(), point_is_same);
		if (it != points_left.end())
		{
			y_left.push_back(points_y[i]);
		}
		else
		{ // It's in the right!
			y_right.push_back(points_y[i]);
		}


	}

	// This is where the recursion occurs
	std::pair<Point, Point> closest_pair_left = find_closest_pair(points_left, x_left, y_left);
	std::pair<Point, Point> closest_pair_right = find_closest_pair(points_right, x_right, y_right);

	// Find which pair is closest
	auto min_dist_left = find_dist(closest_pair_left.first, closest_pair_left.second);
	auto min_dist_right = find_dist(closest_pair_right.first, closest_pair_right.second);

	std::pair<Point, Point> closest_pair = min_dist_left < min_dist_right ? closest_pair_left : closest_pair_right;
	double closest_pair_dist = find_dist(closest_pair.first, closest_pair.second);

	// Time for the funky calculations 
	// The proofs say it works -- I don't understand them.
	// Let's get our center
	double center = (points_x[n / 2 - 1].x + points_x[n / 2].x) / 2.0;
	

	std::vector<Point> center_pairs;
	for (int i = 0; i < n; ++i) {
		if (abs(points_y[i].x - center) <= closest_pair_dist) { center_pairs.push_back(points_y[i]); }
	}

	// Solving the potentially intersecting pairs here
	int cp_n = center_pairs.size();
	for (int i = 0; i < cp_n; ++i) {
		for (int j = i+1; j < cp_n && (center_pairs[j].y - center_pairs[i].y < closest_pair_dist); ++j) {
			double n_dist = find_dist(center_pairs[i], center_pairs[j]);
			if (n_dist < closest_pair_dist) {
				closest_pair = std::pair<Point, Point>(center_pairs[i], center_pairs[j]);
				closest_pair_dist = n_dist;
			}
		}
	}

	return closest_pair;
	// Time to iterate through our 
}

int main()
{
	// Vector to store our points
	std::vector<Point> points;
	std::vector<Point> points_sorted_x;
	std::vector<Point> points_sorted_y;
	std::vector<Point> points_sorted_z;

	// File operations to read our input data
	if (!input) 
	{
		std::cerr << "Could not open the file!" << std::endl;
		return 1;
	}
	else
	{
		// Read file and store in vector using some lovely STL magic
		std::copy(std::istream_iterator<Point>(input),
			std::istream_iterator<Point>(),
			std::back_inserter(points)
		);
		input.close();
	}

	points_sorted_x = points;
	points_sorted_y = points;
	points_sorted_z = points;
	
	// TODO: Add condition to detect and sort 3D points
	
	// Sort by X
	std::sort(points_sorted_x.begin(), points_sorted_x.end(), [](const auto& lhs, const auto& rhs){return lhs.x < rhs.x;});
	//Sort by Y
	std::sort(points_sorted_y.begin(), points_sorted_y.end(), [](const auto& lhs, const auto& rhs) {return lhs.y < rhs.y; });

	auto final_pair = find_closest_pair(points, points_sorted_x, points_sorted_y);

	std::cout << '(' << final_pair.first.x << ',' << final_pair.first.y << ')'
	<< '(' << final_pair.second.x << ',' << final_pair.second.y << ')' << std::endl;

	std::cin.get();

    return 0;
}

