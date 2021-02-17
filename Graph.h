/**********************************************
* Program:  Graph.h                           *
* Author:   Theodore Gouskos 				  *
* Date: 12/13                				  *
*                                             *
* Abstract: Graph class for CPTR247 PA8 and   *
*           Final Exam on Graphs              *
**********************************************/
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <fstream>
#include <stack>
#include <set>
#include <string>
#include <cstring>
#include <iterator>



using namespace std;

struct SP_vw{				// Structure needed for Shortest Path Algorithm
	int vertex;
	double weight_sum;
};

bool operator > (SP_vw X, SP_vw Y)	// So that the priority queue will be sorted lowest to highest
{
	return (X.weight_sum > Y.weight_sum);
}

class Graph {
	friend ostream & operator << (ostream &, Graph);
	public:
		Graph();
		bool build(string filename);
		vector<char> breadthFirst(char first);
		vector<char> depthFirst(char first);
		vector< pair<char, char> > minimumSpanningTree();
		vector<char> shortestPath(char first, char last);
		bool isEulerian();
		bool IsConnected();
		vector< pair<char, char> > eulerCircuit();
		int NumberOfEdges();
	private:
		int size;
		char type;
		double edge [26][26];
};

Graph::Graph()
{
//pre:
//post: Initialize private and matrices
	type = 'i';
	size = 0;

	for(int i = 0; i <= 25; i++) //create a matrices filled with negative 1's
	{
		for(int k = 0; k <= 25; k++)
		{
			edge[i][k] = -1;
		}
	}
}

bool Graph::build(string filename)
{
//Pre:
//Post: Checks file and loads it
	char userInput[80];
	char vertex1;
	char vertex2;
	double lineweight;

	ifstream infile;

	system("clear");
	infile.open(filename.c_str());

	if (infile.fail()) //Checks to see if file fails to load
	{
		cout << "File was not found" << endl;
		return 0;
	}
	//infile.getline(userInput,80);	// get a line of input

	infile >> type;
	infile >> size;

	infile >> vertex1;
	infile >> vertex2;
	infile >> lineweight;

	while(!infile.eof())
	{
		vertex1 = vertex1 - 'a';
		vertex2 = vertex2 - 'a';
		edge[vertex1][vertex2] = lineweight;

		if(type == 'U') //Checks for Undirected
		{
			edge[vertex2][vertex1] = lineweight;
		}

		infile >> vertex1;
		infile >> vertex2;
		infile >> lineweight;
	}
	return true;
}

vector<char> Graph::breadthFirst(char first)
{
//Pre:
//Post: Return BFS from valid staring point

	vector<char> bf; //vector of chars
	queue<char> bfqueue; //queue of chars
	set<char> visited;
	set<char>::iterator itr;					//iterator for the visited set

	pair <set<char>::iterator, bool> result;

	if ((first - 'a') >= size || (first - 'a') < 0) //checks to see if Valid input
	{
		cerr << "The starting position was invalid" << '\n';
		return bf;		//if invalid, returns an empty vector
		}
		else //if the user given character was a vertex in the graph
		{

		bfqueue.push(first);	//begin by pushing the first node into queue
		visited.insert(first);

		while (!bfqueue.empty())				//loop continues until queue is empty
		{
			bf.push_back(bfqueue.front());	//pushes the first node from the queue onto
			int i = bfqueue.front() - 'a';		//finds the row of the matrix that the first
			bfqueue.pop();

			for (int j = 0; j < size; j++)
				if (edge[i][j] != -1)			//if any position has an associated weight
				{
					result = visited.insert ((char) (j + 'a'));	//checks to see if the node has been
					if (result.second)						//if it has not been visited and pushes node onto vector
					{
						bfqueue.push((char) (j + 'a'));
					}
				}
		}
	}
	return bf;
}

vector<char> Graph::depthFirst(char first)
{
//Pre:
//Post:
	vector<char> df; //vector of chars
	stack<char> dfstack; //queue of chars
	set<char> visited;
	set<char>::iterator itr;					//iterator for the visited set

	if ((first - 'a') >= size || (first - 'a') < 0) //checks to see if Valid input
	{
		cerr << "The starting position was invalid" << '\n'; //if invalid, returns an empty vector
		return df;
		}
		else //if the user given character was a vertex in the graph
		{

		dfstack.push(first);	//begin by pushing the first node into queue


		while (!dfstack.empty())				//loop continues until queue is empty
		{
			int i = dfstack.top() - 'a';		//finds the row of the matrix that the first
			char temp = dfstack.top();
			dfstack.pop();

			if(visited.find(temp) == visited.end()) //If
			{
			visited.insert(temp);
			df.push_back(temp);
			}

			for (int j = size - 1; j >= 0; j--)
			{
				if (edge[i][j] != -1)			//if any position has an associated weight
				{

					itr = visited.find((char) (j + 'a'));	//checks to see if the node has been
					if (itr == visited.end())						//if it has not been visited and pushes node onto vector
					{
						dfstack.push((char) (j + 'a'));
					}
				}
			}
		}
	}
	return df;

}

int Graph::NumberOfEdges()
{
//Pre:
//Post:Count the Number of edges
	int numedges = 0;

	for (int i = 0; i < size; i++) //Will count edges, but twice -- careful
	{
		for (int j = 0; j < size; j++)
		{
			if (edge[i][j] != -1) //If not empty, increment number of edges, else keep looping.
			{
				numedges++;
			}
		}
	}

	if(type == 'U')
	{
		numedges = numedges / 2;
	}

	return numedges;
}

struct MSP{				// Structure needed for MSP - Minimum Spanning Tree
	char x;
	char y;
	double weight;
};

bool operator > (MSP X, MSP Y)	// So that the priority queue will be sorted lowest to highest
{
	return (X.weight > Y.weight);
}

vector< pair<char, char> > Graph:: minimumSpanningTree()
{
//Pre: Use Prim's Algorithm to find the minimum spanning tree of a graph
//Post: Return it in form of vector of char pairs

	set<char> visited;		//Keeps track of Visited
	set<char>::iterator itr;
	priority_queue<MSP, vector<MSP>, greater<MSP> > priority; //PriorityQueue
	vector<pair<char, char> > edges; //Vector of Pairs
	int x;
	int y;

	visited.insert('a'); //Insert first node

	for (int i = 0; i < size; i++) // Find Neighbors
			{
				if (edge[0][i] != -1) //add neighbors
				{
					MSP newnode;
					newnode.x = 'a';
					newnode.y = i + 'a';
					newnode.weight = edge[0][i];
					priority.push(newnode);
				}
			}
	while(!priority.empty() && visited.size() < size) //While Priority Queue is not empty and all nodes not visited
	{
		MSP node2;
		node2 = priority.top(); //create new initialization
		priority.pop();//pops first

		if(visited.find(node2.y) == visited.end()) //If everything has been visited
		{
			edges.push_back(make_pair(node2.x, node2.y));
			visited.insert(node2.y);

			for (int i = 0; i < size; i++) // Find Neighbors
			{
				if (edge[node2.y - 'a'][i] && visited.find(i + 'a') == visited.end())
				{
					MSP newnode3;
					newnode3.x = node2.y;
					newnode3.y = i + 'a';
					newnode3.weight = edge[newnode3.y - 'a'][i];
					priority.push(newnode3);
				}
			}
		}
	}
	return edges;
}

vector<char> Graph::shortestPath(char first, char last)
{
	double weight_sum[size];
	int predecessor[size];
	priority_queue<SP_vw, vector<SP_vw>, greater<SP_vw> > pq;
	vector<char> SP;
	int v1 = first - 'a';
	int v2 = last - 'a';
	bool found = false;

	// check for invalid vertices
	if (v1 < 0 || v1 >= size || v1 < 0 || v2 >= size)
	{
		cerr << "\nERROR: Either " << first << " or " << last << " is not a vertex in the graph" << endl;
		return SP;
	}

	// was it a silly request?
	if (v1 == v2)
		return SP;

	// initialize data structures
	for (int i = 0; i < size; i++)
		weight_sum[i] = 10000.0;
	weight_sum[v1] = 0;
	for (int i = 0; i < size; i++)
		predecessor[i] = -1;

	// start with v1's neighbors
	for (int i = 0; i < size; i++)
	{
		if (edge[v1][i] != -1)
		{
			weight_sum[i] = edge[v1][i];
			predecessor[i] = v1;
			SP_vw newOne;
			newOne.vertex = i;
			newOne.weight_sum = edge[v1][i];
			pq.push(newOne);
		}
	}


	while (!pq.empty())					// there's still hope of reaching v2
	{
		SP_vw current = pq.top();
		pq.pop();
		if (current.vertex == v2)		// have we made it to v2?
		{
			found = true;
			break;
		}
		for (int i = 0; i < size; i++)	// add current's neighbors
		{
			if (edge[current.vertex][i] != -1)	// i is a neighbor of current
			{
				if ((current.weight_sum + edge[current.vertex][i])
					< weight_sum[i])			// and it is a shorter path
				{
					weight_sum[i] = current.weight_sum + edge[current.vertex][i];
					SP_vw newOne;
					newOne.vertex = i;
					newOne.weight_sum = weight_sum[i];
					pq.push(newOne);
					predecessor[i] = current.vertex;
				}
			}
		}
	}

	if (!found)				// no path to v2 was found
		return SP;

	int current = v2;
	deque<int> buildSP;
	while (current != v1)	// trace back through the predecessor array
	{
		buildSP.push_front(current);
		current = predecessor[current];
	}
	buildSP.push_front(v1);

	for (int i = 0; i < buildSP.size(); i++)
	{
		SP.push_back(buildSP[i] + 'a');
	}
	return SP;
}

bool Graph::isEulerian()
{
	int degree;

	for(int i = 0; i > 0; i++) //Iterate through the matrices
	{
		for(int j = 0; j > 0; j++)
		{
			if (edge[i][j] != -1)
			{
				degree++;
			}
		}
		if (degree % 2 != 0)
		{
			return false;
		}
		degree = 0; //reset to 0
	}
	return true;
}

vector< pair<char, char> > Graph::eulerCircuit()
{
//Pre: Assumed Graph is connected
//Post: Use Fleury's Algorithm
	vector<pair<char, char> > euler; //Holds Euler
	vector<pair<char, char> >::iterator euleritr; //Euler itr
	vector<pair<char, char> >eulertemp; //Copy of Euler

	vector<deque<int> > neighbors; //Vector of deques for Neighbors
	vector<deque<int> >::iterator neighborsitr; //vector for Neighbors Itr

	deque<int>::iterator dequeitr; // itr for deque

	set<char> visited; //set for visited nodes
	set<char>::iterator visiteditr; //itr for visited

	if (!isEulerian()) //if not Eulerian
	{
		cerr << "This Graph is not a Euler Circuit" << endl;
		return euler;
	}

	int numedges = 0;

	for (int i = 0; i < size; i++) //Will count edges, but twice -- careful
	{
		for (int j = 0; j < size; j++)
		{
			if (edge[i][j] != -1) //If not empty, increment number of edges, else keep looping.
			{
				numedges++;
			}
		}
	}

	if(type == 'U')
	{
		numedges = numedges / 2; //Number of edges were counted twice, divide it by 2
	}

	for (int i = 0; i < size; i++)  //creates a temp and pushes back into neighbor
	{
		deque<int> temp;
		for(int j = 0; j < size; j++)
		{
			if( edge[i][j] != -1)
			{
				temp.push_back(j);
			}
		}
		neighbors.push_back(temp);
	}

	char v, w, remembered; //chars to hold different vertices

	while (euler.size() < numedges)  //edges have not been in circuit
	{
		if (euler.size() == 0) //if first time through loop
		{
			v = 'a';
			remembered = v;
			visited.insert(v);
		}
		else {
					for(visiteditr = visited.begin(); visiteditr != visited.end(); visiteditr++)  //Iterates through Visited
					{
						if(!neighbors[int(*visiteditr -'a')].empty()) //Checks for unvisited nodes in visited nodes
						{
						v = *visiteditr;
						remembered = v;
						break;
						}
					}
			}

		bool found = false;					//boolean to determine if remembered was visited

		while (found == false)				//while the remembered node has not been visited
		{
			w = char(neighbors[int(v - 'a')].front() + 'a');	//w is the first unvisited neighbor of v

			neighbors[int(v - 'a')].pop_front();	//pop it from the deque so we do not visit edge

			for (dequeitr = neighbors[int(w - 'a')].begin(); dequeitr != neighbors[int(w - 'a')].end(); dequeitr++) //iterates through neighbors of w
				{
					if (*dequeitr == int(v - 'a'))	//checks for the same edge, if edges are reversed.
						{
							neighbors[int(w - 'a')].erase(dequeitr);	//erases if true
							break;
						}
				}
			visited.insert(w);				//marks w as visited
			eulertemp.push_back(make_pair(v, w));	//pushes the pair v-w onto the circuit so far
			v = w;							//assigns v to w to progress
			if (v == remembered)			//if the circuit is back to the original vertex
			{
				found = true;				//sets found as true, breaks out of loop
			}
		}

		if (euler.size() == 0)				//if this is the first (or only) circuit
		{
			euler = eulertemp;				//euler is assigned eulertemp
			eulertemp.clear();				//clears eulertemp for next iteration of loop
		}
		else
			{
				for (euleritr = euler.begin(); euleritr != euler.end(); euleritr++) //iterates through euler vector to find correct place to insert circuit so far
				{
					if (eulertemp.front().first == (*euleritr).second) //finds where circuit so far picks up from the euler circuit
					{
						euler.insert(euleritr + 1, eulertemp.begin(), eulertemp.end());
						eulertemp.clear();	//inserts circuit so far and clears eulertemp
						break;
					}
				}
			}
	}
	return euler;
}


ostream & operator << (ostream & os, Graph theGraph)
{
//Pre:
//Post: Displays the graph object
	if(theGraph.type == 'D') //Displays if Graph is Directed or Undirected
	{
		os << "Directed" << endl;
	} else {
			 os << "Undirected" << endl;
			}
	os << fixed << setprecision(1) << setw(5); //Format

	for (int i = 0; i < theGraph.size; i++) //Iterates through matrix and displays if there is something to display
	{
		for (int j = 0; j < theGraph.size; j++)
		{
			if (theGraph.edge[i][j] != -1) //If the matrix has info
				os << theGraph.edge[i][j] << setw(5);
			else
				os << "---" << setw(5);
		}
		os << endl;
	}

	return os;

}

bool Graph::IsConnected()
{
	if (type == 'U')
	{
		vector<char> temp;
		breadthFirst('a');
		temp = breadthFirst('a');

		if ( temp.size() == size)
		{
			return true;
		} else {
				return false;
			   }
	}

	if (type == 'D')
	{
		for (int i = 0; i < size; i ++)
		{
			vector<char> temp;
			breadthFirst(i);
			temp = breadthFirst(i);
			if (temp.size() == size)
			{
				return true;
			} else {
					return false;
				   }
		}
	}
}
