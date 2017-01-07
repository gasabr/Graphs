#ifndef GRAPH
#define GRAPH

#include <vector>
#include <iostream>
#include <utility> 	// std::pair
#include <stack> 	// for dfs
#include <queue> 	// for bfs
#include <fstream>

#include "vertex.hpp"

typedef vector<vector<int> > Matrix;
typedef pair<int, int> Edge;

using namespace std;

// int inf = 1 << 20;
// int n = 1; // to be able to use recursion in depth_first_timestamp

class Graph
{
	std::vector<Vertex> vertices;
	std::vector<Edge>   edges;
	Matrix adj_matrix;
	std::vector<int> topological_sorted;
	std::vector<int> numerator; // FIX IT
	std::vector<int> denominator; // FIX IT
	// int n=1; // FIX IT

public:
	Graph(){};
	Graph(Matrix adj_matrix); // need
	Graph(string filename, bool weighted=false);
	~Graph();

	Matrix get_adj_matrix() { return this->adj_matrix; };

	void print_vertices();
	void print_vertices_num(){ cout << vertices.size() << "\n"; }
	void print_edges();

	void add_edge(Edge e);
	int  add_vertex(int key, int value);
	int  add_vertex(Vertex v);

	int    get_next_unvisited(int from, bool transposed=false);
	void   clean(); 	// set all vertices as unvisited and timestamp to default
	void   print_adj_matrix(int print_transposed=0);
	Matrix get_transpose(Matrix adj);

	std::vector<int> dfs_path(int value=0, int from=1);
	std::vector<int> bfs_path(int value=0, int from=1);
	std::vector<int> topological_sort();
	std::vector<int> dijk(int from);
	bool             bellman_ford(int source=0);
	void             depth_first_timestamps(int x=1);
	Matrix           scc();
	Matrix           group_components(stack<int>);
};

#endif // GRAPH
