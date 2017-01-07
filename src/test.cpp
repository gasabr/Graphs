#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "graph.hpp"
#include "vertex.hpp"
#include <vector>

std::string DATA_FOLDER = "data/";

typedef vector<vector<int> > Matrix;

SCENARIO("Testing constructor from the file", "[constructor]"){
	// fill the file with some adjacency list
	// create a Graph object from the list of edges
	GIVEN("file graph.txt with E"){
		std::string filename = DATA_FOLDER + "graph.txt";

		WHEN("create an unweighted graph from the file"){
			Graph g = Graph(filename);
			Matrix a = g.get_adj_matrix();

			THEN("check adj matrix size"){
				REQUIRE(a.size() == 9);
				REQUIRE(a[0].size() == 9);
			}
		}
	}

	GIVEN("file weighted-graph.txt"){
		std::string filename = DATA_FOLDER + "weighted-graph.txt";

		WHEN("Create a weighted graph from the file"){
			Graph g = Graph(filename, true);
			Matrix a = g.get_adj_matrix();

			THEN("check adj matrix size"){
				REQUIRE(a.size() == 9);
				REQUIRE(a[0].size() == 9);
			}

			THEN("check adjacency matrix content"){
				std::ifstream in;
				in.open(filename);

				int n_vertices, n_edges;
				in >> n_vertices >> n_edges;

				int v, u, w;
				for (int i=0; i<n_edges; ++i){
					in >> u >> v >> w;
					REQUIRE(a[u][v] == w);
					REQUIRE(a[v][u] == w);
				}
			}
		}
	}
}

SCENARIO("Testing algorithms"){

	GIVEN("graph from 1st test"){
		std::string filename = DATA_FOLDER + "graph.txt";
		Graph g = Graph(filename);

		WHEN("Perform DFS"){
			std::vector<int> dfs_result = g.dfs_path(); // will return way through dfs tree
			std::vector<int> expected_result = {1,2,6,7,8,4,5,3};

			THEN("Check if the path was right"){
				REQUIRE(dfs_result == expected_result);
			}
		}

		WHEN("perform BFS"){
			std::vector<int> bfs_result = g.bfs_path();
			std::vector<int> expected_result = {1,2,3,4,6,5,7,8};

			THEN("Check if the path was right"){
				REQUIRE(bfs_result == expected_result);	
			}
		}

		WHEN("Perform topological sort"){
			std::vector<int> ts_result = g.topological_sort();
			std::vector<int> expected_result = {};

			THEN("Check if the path was right"){
				REQUIRE(ts_result == expected_result);	
			}
		}

		WHEN("find Strongly Connected Components"){
			vector<vector<int> > scc_result = g.scc();
			vector<vector<int> > expected_result = {{1, 2, 3}, {4, 5}, {6, 7}, {8}};

			THEN("Check if the path was right"){
				REQUIRE(scc_result == expected_result);	
			}
		}
	}

	GIVEN("The weighted graph"){
		std::string filename = DATA_FOLDER + "weighted-graph.txt";
		Graph g = Graph(filename, true);

		WHEN("Find shortest paths from every vertex to all others"){
			std::vector<int> sp_from_1   = g.dijk(1);
			std::vector<int> dist_from_1 = {0,2,1,6,11,7,13,19};

			THEN("Check if distance calculated correctly"){
				REQUIRE(sp_from_1 == dist_from_1);
			}
		}
	}
}