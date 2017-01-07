#ifndef VERTEX
#define VERTEX

#include <iostream>

using namespace std;

class Vertex
{
	int key;
	int value;
	bool visited;
	int pred;
	int path;
	int timeStamp; // get rid of it

public:

	Vertex(int key, int value);
	Vertex(int key) : key(key), visited(false) { };

	~Vertex(){};

	int get_key()   { return this->key; }
	int get_value() { return this->value; }
	int get_path()  { return this->path; }
	int get_pred()  { return this->pred; }
	void set_pred(int pred){ this->pred = pred; }
	void set_path_length(int path) { this->path=path; }

	void set_timeStamp(int i) { this->timeStamp = i; }
	int  get_timeStamp()      { return this->timeStamp; };

	bool is_visited() { return this->visited; }
	void print() { printf("(%d, %d); children: ", this->key, this->value); };

	void mark_visited() { this->visited = true; }
	void clean_mark() { this->visited = false; }
};

#endif // VERTEX
