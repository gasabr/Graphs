#include "vertex.hpp"

Vertex::Vertex(int key, int value){
	this->key = key;
	this->value = value;
	this->visited = false;
	this->timeStamp = -1;
	this->pred = 0;
	this->path = 1000000000; // move to max int
}
