/**
* Minimum Spanning Tree using Boruvka's Algorithm
* CSCI-7432 - Algorithms and Data Structures
*
* This program generates a MST using Boruvka's algorithm.
* It accepts a CSV text file representing a graph. The first
* column is the source vertex, the second is the destination vertex
* and the last is the weight of the edge between the two.
* 
* The graph is stored as an adjaceny list.
*
*
*
* Paul Bupe Jr
*/


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


 The output for the included example is:

  Total path cost is: 43
  MST is: {0,2} {0,1} {3,5} {4,5} {5,6} {0,3}

*/

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <set>


// Input file. Expects a single comma separated weighted edge per line
std::ifstream input("graph.txt");

// Representation of a weighted edge
struct Edge
{
	int src, dest, weight;

	// Overload the >> operator to make parsing the input file cleaner
	friend std::istream& operator >>(std::istream& is, Edge& v)
	{
		char ch;
		is >> v.src >> ch >> v.dest >> ch >> v.weight;
		return is;
	}
};

// Returns a full edge given just the weight of the edge
Edge edge_from_weight(std::vector<Edge> &edge_list, int weight)
{
	Edge empty_edge = { -1,-1,-1 };
	auto edge = std::find_if(edge_list.begin(), edge_list.end(), [&](const auto& e)
	{return (e.weight == weight); });
	return (edge != edge_list.end()) ? (*edge) : empty_edge;
}


// Utilizing disjoint sets to handle the merging of all the sets (collections) we create
// This requires find() and union() methods for identifying which set an edge belongs to 
// and for merging sets.

struct Set
{
	int parent;
	int rank;
};

int ds_find(std::vector<Set> &sets, int i)
{
	// Find root and make root as parent of i
	if (sets[i].parent != i) sets[i].parent = ds_find(sets, sets[i].parent);
	return sets[i].parent;
}

// This function is a third party implementation of union by rank
// Nothing special going on here. 
// https://www.geeksforgeeks.org/union-find-algorithm-set-2-union-by-rank/

void ds_union(std::vector<Set> &sets, int x, int y)
{
	int xroot = ds_find(sets, x);
	int yroot = ds_find(sets, y);

	// Attach smaller rank tree under root of higher rank tree
	if (sets[xroot].rank < sets[yroot].rank)
	{
		sets[xroot].parent = yroot;
	}
	else if (sets[xroot].rank > sets[yroot].rank)
	{
			sets[yroot].parent = xroot;
	}
	// If ranks are same, then make one as root and increment its rank by one
	else
	{
		sets[yroot].parent = xroot;
		sets[xroot].rank++;
	}
}

// The main algorithm
std::vector<std::pair<int, std::list<Edge>>> boruvka(std::vector<std::list<std::pair<int, int>>> &adj_list, std::vector<Edge> &edge_list)
{
	int num_v = adj_list.size();
	int total_weight = 0;
	int collections = num_v;
	int path_cost = 0;

	std::vector<std::pair<int, std::list<Edge>>> mst;
	std::list<Edge> mst_edges;
	std::vector<Set> sets;
	std::vector<int> cheapest_edge;

	// Initialize our sets
	for (int i = 0; i < num_v; i++) { sets.push_back({ i, 0 }); }

	while (collections > 1)
	{
		// Maintain a list of cheapest edges
		for (int i = 0; i < num_v; i++) { cheapest_edge.push_back(-1); }

		// Calculate the minimum outgoing weights from each set
		for (int i = 0; i < num_v; i++)
		{
			int src_set = ds_find(sets, i);
			std::pair<int, int> cheapest = { -1, -1 };
			for (auto const &list : adj_list[i])
			{
				int dest_set = ds_find(sets, list.first);
				if (src_set != dest_set) {
					cheapest.first = list.first;
					cheapest.second = list.second;
					break;	// Since the adjacency list is sorted, the first valid edge is always the cheapest
				}
			}
			cheapest_edge[src_set] = (cheapest_edge[src_set] == -1 || cheapest_edge[src_set] > cheapest.second) ? cheapest.second : cheapest_edge[src_set];
		}

		// Using a disjoint set, merge set and populate the MST
		for (int i = 0; i < num_v; i++)
		{
			if (cheapest_edge[i] != -1)
			{
				Edge cheapest = edge_from_weight(edge_list, cheapest_edge[i]);

				int src_set = ds_find(sets, cheapest.src);
				int dest_set = ds_find(sets, cheapest.dest);

				if (src_set != dest_set) {
					mst_edges.push_back(cheapest);
					path_cost += cheapest.weight;
					ds_union(sets, src_set, dest_set);
					collections--;
				}
			}
		}

		cheapest_edge.clear();
	}

	mst.push_back({ path_cost, mst_edges });

	return mst;
}


int main()
{
	// Define an adjaceny list using a vector that contains a list
	std::vector<std::list<std::pair<int, int>>> adj_list(7);

	// Define an edge list
	std::vector<Edge> edge_list;

	// From our input file
	std::vector<Edge> raw_graph;

	// Read data from graph csv file
	if (!input)
	{
		std::cerr << "Could not open the file!" << std::endl;
		return 1;
	}
	else
	{
		// Read file and store in vector using some lovely STL magic
		std::copy(std::istream_iterator<Edge>(input),
			std::istream_iterator<Edge>(),
			std::back_inserter(raw_graph)
		);
		input.close();
	}

	// Sort the raw graph data by edge weight
	std::sort(raw_graph.begin(), raw_graph.end(), [](const auto& lhs, const auto& rhs) {return lhs.weight < rhs.weight; });

	// Build adjecency and edge lists
	for (auto const& v : raw_graph)
	{
		adj_list[v.src].push_back(std::make_pair(v.dest, v.weight));
		

		if (edge_list.empty())
		{
			edge_list.push_back(v);
			//std::cout << "Added edge to list: " << v.src << " " << v.dest << " " << v.weight << std::endl;
		}
		else
		{
			// Since we're dealing with an undirected graph, (x,y) == (y,x)
			// Using a lambda as the find_if predicate
			auto it = std::find_if(edge_list.begin(), edge_list.end(), [&](const auto& e)
			{return ((e.src == v.src) && (e.dest == v.dest)) ||
				((e.src == v.dest) && (e.dest == v.src)); });
			
			// If this edge is not in the edge_list then add it!
			if (it == edge_list.end())
			{
				edge_list.push_back(v);
				//std::cout << "Added edge to list: " << v.src << " " << v.dest << " " << v.weight << std::endl;
			}
		}
	}

	// Run the algorithm
	std::vector<std::pair<int, std::list<Edge>>> mst = boruvka(adj_list, edge_list);

	
	// Display the results
	std::cout << "Total path cost is: " << mst[0].first << std::endl;
	std::cout << "MST is: ";

	for (auto const &edges : mst[0].second)
	{
		std::cout << '{' << edges.src << ',' << edges.dest << std::string("} ");
	}
	std::cout << std::endl;

	// Pause the command prompt so the results can be seen.
	std::cin.get();

    return 0;
}

