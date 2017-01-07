#include "graph.hpp"

int inf = 1 << 20;
int t   = 1; 		// to use recursion when putting timestamps

// TODO: initialize all vars here
Graph::Graph(Matrix adj_matrix){ this->adj_matrix = adj_matrix; }

Graph::Graph(string filename, bool weighted){
	ifstream in;
	try{
		in.open(filename);
	}
	catch (std::ios_base::failure& e){
		std::cerr << e.what() << '\n';
	}

	int n_vertices, n_edges;
	in >> n_vertices >> n_edges;

	if (!weighted)
		this->adj_matrix.resize(n_vertices+1, vector<int>(n_vertices+1, 0));
	else
		this->adj_matrix.resize(n_vertices+1, vector<int>(n_vertices+1, 1 << 20));

	int v, u, w;
	for (int i=0; i<n_edges; ++i){
		if (!weighted) {
			in >> u >> v;
			this->adj_matrix[u][v] = 1;
		}
		else{
			in >> u >> v >> w;
			adj_matrix[u][v] = adj_matrix[v][u] = w;
		}
	}
	in.close();

	for (int i=0; i<=n_vertices; ++i){ // There is one redudant vertex
		this->vertices.push_back(Vertex(i));
	}
}

Graph::~Graph() {};

void Graph::print_adj_matrix(int print_transposed){
	cout << "Adjacency matrix:\n";
	for(int i=1; i<this->adj_matrix.size(); ++i){
		for(int j=1; j<this->adj_matrix.size(); ++j)
			if (this->adj_matrix[i][j] != inf)
				cout << this->adj_matrix[i][j] << " ";
			else
				cout << '.' << " " ;
		cout << '\n';
	}
	if (print_transposed){
		cout << "Transposed adjacency matrix:\n";
		for(int i=1; i<this->adj_matrix.size(); ++i){
			for(int j=1; j<this->adj_matrix.size(); ++j)
				if (this->adj_matrix[j][i] != inf)
					cout << this->adj_matrix[j][i] << " ";
				else
					cout << '.' << " " ;
			cout << '\n';
		}
	}
}


int Graph::add_vertex(int key, int value){
	vertices.push_back(Vertex(key, value));
	return 0;
}


int Graph::add_vertex(Vertex v){
	vertices.push_back(v);
	return 0;
}


void Graph::print_vertices(){
	vector<Vertex>::iterator it;
	for(it = vertices.begin(); it != vertices.end(); ++it)
		it->print();
}


void Graph::add_edge(Edge e){ this->edges.push_back(e); }


void Graph::print_edges(){
	// Prints edges in a way: (v1, v2)
	for(int v1 = 0; v1 < this->adj_matrix.size(); ++v1){
		cout << "Vertex: " << v1;
		cout << "\n";
		for(int v2=0; v2 < this->adj_matrix[0].size(); ++v2){
			if(this->adj_matrix[v1][v2] < inf)
				printf(" (%d, %d)", v1, v2);
		}
		cout << "\n";
	}
}


int Graph::get_next_unvisited(int from, bool transposed){
	for(int i = 1; i < this->adj_matrix[from].size(); ++i)
		if (!transposed){
			if(adj_matrix[from][i]==1 && !vertices[i].is_visited()){ // not exactly equal to 1
				// vertices[i].mark_visited(); // FIX IT
				return i;
			}
		} else {
			if(adj_matrix[i][from] && !vertices[i].is_visited()){
				// vertices[i].mark_visited();
				return i;
			}
		}


	return -1;
}


std::vector<int> Graph::dfs_path(int value, int from){
	stack<int> st;
	st.push(from);

	std::vector<int> result;

	while (!st.empty()){
		// get next vertex for search
		int current = st.top();
		st.pop();
		vertices[current].mark_visited();

		result.push_back(current);

		int next = get_next_unvisited(current);
		while (next != -1){
			vertices[next].mark_visited(); // vertex is not visited now - bad naming
			st.push(next);
			next = get_next_unvisited(current);
		}
	}

	this->clean();
	return result;
}


/* Same with dfs will only show a path in a graph */
std::vector<int> Graph::bfs_path(int value, int from){
	// Breadth first search
	queue<int> q;
	q.push(from);

	std::vector<int> result;

	while(!q.empty()){
		// take index of current element
		int current = q.front();
		q.pop();

		result.push_back(current);

		// for the real bfSEARCH function
		// if(this->vertices[current].get_value() == value){
		// 	this->clean();
		// 	return result;
		// }

		// mark vertex as visited
		this->vertices[current].mark_visited();

		// add all neighbours to current to queue
		int next = this->get_next_unvisited(current);
		while(next != -1){
			q.push(next);					// TODO:
			vertices[next].mark_visited();  // mark_seen would be correct form
											// or timestamp -1 -> 0
			next = this->get_next_unvisited(current);
		}
	}
	// to be able to use algorithm one more time
	this->clean();
	return result;
}


void Graph::clean(){
	for(int i = 0; i < this->vertices.size(); ++i)
		vertices[i].clean_mark();
	t = 1;
}


std::vector<int> Graph::topological_sort(){
	std::vector<int> result;

	if (this->scc().size() < adj_matrix.size() - 1){
		cout << "Adjacency matrix defines NOT a DAG.\n";
		this->clean();
		return {};
	}
	
	// mark all vertices with time_stamps
	this->depth_first_timestamps();

	int next = 0, i = 0;
	vector<pair<int, int> > tmp;

	for (i=1; i < vertices.size(); ++i){
		// create pair <time_stamp, vertex's index>
		pair<int, int> pair = make_pair(vertices[i].get_timeStamp(), i);
		// add pair to vector
		tmp.push_back(pair);
		next++;
	}

	// sort pairs by time stamps
	for(i=0; i<tmp.size(); ++i){
		for(int j=0; j<tmp.size(); ++j){
			if(tmp[i].first < tmp[j].first)
				swap(tmp[i], tmp[j]);
		}
	}

	for(i=0; i<tmp.size(); ++i) 
		result.push_back(tmp[i].second);

	return result;
}


/* Dijkstra */
std::vector<int> Graph::dijk(int from){
	/* Finding shoortest path to every possible vertice in undirected graph */
	int n = this->adj_matrix.size();

	// create visit vector, fill it with false
	vector<bool> vis(n, false);
	vector<int> dist(n, 1 << 20); // TODO: use limit int here


	// distance from starting point to itself = 0
	dist[from] = 0;

	for(int i=1; i<n; ++i){
		// pick the nearest vertex to the current one
		int cur = -1;
		for(int j=1; j<n; ++j){
			if (vis[j]) continue;
			if (cur == -1 || dist[j] < dist[cur])
				cur = j;
		}

		for(int j=1; j<n; ++j){
			int path = dist[cur] + adj_matrix[cur][j];
			if(path < dist[j])
				dist[j] = path;
		}
		vis[cur] = true;
	}
	std::vector<int> result(dist.begin()+1, dist.end());
	return result;
}


bool Graph::bellman_ford(int source){
	int N = this->adj_matrix.size();
	vector<int> pred(N, -1);
	vector<int> d(N, 1 << 20);

 	pred[source] = 0;
	d[source] = 0;

	for (int i=source; i<N; ++i){
		// release all adjacent verticies
		for (int j=0; j<N; ++j){
			if (adj_matrix[i][j] < (1 << 20) &&
					d[i] + adj_matrix[i][j] < d[j]){
				d[j] = d[i] + adj_matrix[i][j];
				if (pred[j] == -1) pred[j] = i;
				else return false;
			}
		}
	}
	cout << d[2] << "\n";
	for (int i=source; i<N; ++i)
		if (i != pred[i] && pred[i]!=-1 && d[i] > adj_matrix[pred[i]][i] + d[pred[i]]){
			return false;
		}
	return true;
}


Matrix Graph::get_transpose(Matrix adj){
	Matrix transposed;
	transposed.resize(adj.size(), vector<int>(adj.size(), inf));
	for (int i=0; i<adj.size(); i++){
		for (int j=0; j<adj[i].size(); ++j){
			transposed[j][i]=adj[i][j];
			adj[i][j]=inf;
		}
	}
	return transposed;
}


Matrix Graph::scc(){
	numerator.resize(this->adj_matrix.size(), 0);
	denominator.resize(this->adj_matrix.size(), 0);

	this->depth_first_timestamps();
	clean();
	stack<int> seq;
	int c = 1;
	while (seq.size() < vertices.size() && c < vertices.size()-1){
		for (int i=1; i < vertices.size(); ++i)
			if (vertices[i].get_timeStamp() == c) {
				seq.push(i);
				c++;
			}
	}

	// mark all grouped vertices
	Matrix result = group_components(seq);
	for (auto group : result){
		for (auto c : group){
			vertices[c].mark_visited();
		}
	}

	// if there are unvisited vertices with rings
	// add them as single element
	for (int i=1; i<vertices.size(); ++i){
		if (!vertices[i].is_visited() && adj_matrix[i][i])
			result.push_back({i});
	}

	// sort all groups
	for (int i=0; i < result.size(); ++i){
		std::sort(result[i].begin(), result[i].end());
	}
	
	this->clean();
	return result;
}


void Graph::depth_first_timestamps(int x){
	/* fills denominator and nominator vectors with */
	if (vertices[x].is_visited()) return;
	vertices[x].mark_visited();

	for (int i=1; i<adj_matrix[x].size(); i++)
		// if the path exist 
		if (adj_matrix[x][i] && adj_matrix[x][i] < inf){
			depth_first_timestamps(i);
		}
	vertices[x].set_timeStamp(t++);
}


Matrix Graph::group_components(stack<int> seq){
	/* defines strongly connected components */
	int next = 0;
	std::vector<std::vector<int> > result;
	std::vector<int> cv; // connected vertices

	while(!seq.empty()){

		int current;

		if (next) {
			if (std::find(cv.begin(), cv.end(), next) == cv.end()){
				cv.push_back(next);
			}

			current = next; 
			next = 0; 
		}
		else { 
			current = seq.top(); 
			seq.pop(); 
			cv = { current };
		}

		vertices[current].mark_visited();

		for (int i=1; i < adj_matrix.size(); ++i){
			// if there is vertex to go - remember it in next
			if (adj_matrix[i][current] && !vertices[i].is_visited()){
				next = i;
				if ( std::find(cv.begin(), cv.end(), i)!=cv.end())
					cv.push_back(i);
			}
		}
		// add groups of 2 and more nodes to result
		if (!next && cv.size() > 1){
			result.push_back(cv);
		}
	}
	this->clean();
	return result;
}
