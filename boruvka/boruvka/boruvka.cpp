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
#include <set>

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
	// find root and make root as parent of i
	if (sets[i].parent != i) sets[i].parent = ds_find(sets, sets[i].parent);
	return sets[i].parent;
}

void ds_union(std::vector<Set> &sets, int x, int y)
{
	int xroot = ds_find(sets, x);
	int yroot = ds_find(sets, y);

	// Attach smaller rank tree under root of high
	// rank tree (Union by Rank)
	if (sets[xroot].rank < sets[yroot].rank)
		sets[xroot].parent = yroot;
	else if (sets[xroot].rank > sets[yroot].rank)
		sets[yroot].parent = xroot;

	// If ranks are same, then make one as root and
	// increment its rank by one
	else
	{
		sets[yroot].parent = xroot;
		sets[xroot].rank++;
	}
}

// Input file. Expects a single comma separated point per line
std::ifstream input("graph.txt");

void boruvka(std::vector<std::list<std::pair<int, int>>> &adj_list, std::vector<Edge> &edge_list)
{
	int num_v = adj_list.size();
	int num_e = edge_list.size();
	int total_weight = 0;
	int collections = num_v;
	int path_cost = 0;

	std::vector<Set> sets;
	std::vector<int> cheapest_edge;
	Set tmp;


	for (int i = 0; i < num_v; i++)
	{
		tmp.parent = i;
		tmp.rank = 0;
		sets.push_back(tmp);
	}


	while (collections > 1)
	{
		for (int i = 0; i < num_v; i++)
		{

			cheapest_edge.push_back(-1);
		}

		// For each edge in the graph
		for (int i = 0; i < num_e; i++)
		{
			// Identify which set the source and destination vertices belong to
			int src_set = ds_find(sets, edge_list[i].src);
			int dest_set = ds_find(sets, edge_list[i].dest);

			// If the source and destination aren't in the same set
			if (src_set != dest_set) {

				// Populate the cheapest edge list
				if (cheapest_edge[src_set] == -1 || edge_list[cheapest_edge[src_set]].weight
		> edge_list[i].weight)
				{
					cheapest_edge[src_set] = i;
				}

				if (cheapest_edge[dest_set] == -1 || edge_list[cheapest_edge[dest_set]].weight
		> edge_list[i].weight)
				{
					cheapest_edge[dest_set] = i;
				}
			}

		}

		for (int i = 0; i < num_v; i++)
		{
			if (cheapest_edge[i] != -1)
			{
				int src_set = ds_find(sets, edge_list[cheapest_edge[i]].src);
				int dest_set = ds_find(sets, edge_list[cheapest_edge[i]].dest);
				
				if (src_set != dest_set) {
					std::cout << edge_list[cheapest_edge[i]].weight << std::endl;
					path_cost += edge_list[cheapest_edge[i]].weight;
					ds_union(sets, src_set, dest_set);
 					collections--;
				}
			}
		}
		
		/*
		for (int i = 0; i < num_v; i++)
		{
			//auto cheapest = std::min_element(adj_list[i].begin(), adj_list[i].end(), [](auto const& lhs, auto const& rhs) {return lhs.second < rhs.second; });
			
			std::pair<int, int> cheapest;
			int src_set = ds_find(sets, i);

			for (auto const &list : adj_list[i])
			{
				cheapest_edge[src_set] = list.second;
				int dest_set = ds_find(sets, list.first);
				if (src_set != dest_set) {
					//cheapest_edge[src_set] = list.second;
					 cheapest.first = list.first;
					 cheapest.second = list.second;
					 break;	// Since the adjacency list is sorted, the first valid edge is always the cheapest
				}
			}

			
			// Identify which set the source and destination vertices belong to
			//int src_set = ds_find(sets, i);
			int dest_set = ds_find(sets, cheapest.first);

			// If the source and destination aren't in the same set
			if (src_set != dest_set) {
				path_cost += cheapest.second;
				ds_union(sets, src_set, dest_set);
				collections--;
			}
		}
		*/
		cheapest_edge.clear();
	}
	
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
			std::cout << "Added edge to list: " << v.src << " " << v.dest << " " << v.weight << std::endl;
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
				std::cout << "Added edge to list: " << v.src << " " << v.dest << " " << v.weight << std::endl;
			}
		}
	}

	boruvka(adj_list, edge_list);

	std::cin.get();

    return 0;
}

