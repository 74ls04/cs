// boruvka.cpp
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <fstream>
#include <algorithm>
#include <iterator>

/*
A visual representation of the weighted graph contained in graph.txt

(0)----2------(2)----19-------(4)
 |\            |            / | \
 |  \          |          /   |   9 
 |    \        |        /     |     \
 8     14      25     17      5     (6)
 |       \     |     /        |     /   
 |         \   |   /          |   1
 |           \ | /            | /        
(1)----21-----(3)----13------(5)

*/

struct Vertex
{
	int source, dest, weight;

	// Overload the >> operator to make parsing the input file cleaner
	friend std::istream& operator >>(std::istream& is, Vertex& v)
	{
		char ch;
		is >> v.source >> ch >> v.dest >> ch >> v.weight;
		return is;
	}
};

// Define an adjaceny list using a vector that contains a list
std::vector <std::list<std::pair<int, int>>> adj_list(7);


// Input file. Expects a single comma separated point per line
std::ifstream input("graph.txt");

int main()
{
	// The mighty graph!
	std::vector<Vertex> graph;

	// Read data from graph csv file
	if (!input)
	{
		std::cerr << "Could not open the file!" << std::endl;
		return 1;
	}
	else
	{
		// Read file and store in vector using some lovely STL magic
		std::copy(std::istream_iterator<Vertex>(input),
			std::istream_iterator<Vertex>(),
			std::back_inserter(graph)
		);
		input.close();
	}

	// Build adjecency list
	for (auto const& v : graph)
	{
		adj_list[v.source].push_back(std::make_pair(v.dest, v.weight));
		std::cout << v.source << std::endl;
	}

	std::cin.get();

    return 0;
}

