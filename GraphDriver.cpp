/**********************************************
* Program:  GraphDriver.cpp                   *
* Author:   Eileen Peluso                     *
*                                             *
* Abstract: Driver for CPTR247 PA8 and        *
*           Final Exam on Graphs              *
**********************************************/
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Graph.h"

using namespace std;

int Menu(){
  // pre:  none
  // post: the function returns a number between 0 and 6 inclusive
  //       indicating the user's choice of action to be taken
	int option;
	system("clear");		// clear screen
						// and set up menu
	cout << endl << endl << endl << endl;
	cout << setw(50) << "Test Out Your Graph Algorithms   " << endl;
	cout << endl << endl;
	cout << setw(50) << "Options                           " << endl;
	cout << setw(50) << "   0      End                     " << endl;
	cout << setw(50) << "   1      Load a new graph        " << endl;
	cout << setw(50) << "   2      Breadth first traversal " << endl;
	cout << setw(50) << "   3      Depth first traversal   " << endl;
	cout << setw(50) << "   4      Minimum spanning tree   " << endl;
	cout << setw(50) << "   5      Shortest path           " << endl;
	cout << setw(50) << "   6      Is it Eulerian?         " << endl;
	cout << setw(50) << "   7      Euler circuit           " << endl;
	cout << setw(50) << "   8      Display graph           " << endl;
	cout << setw(50) << "   9      Number of Edges         " << endl;
	cout << setw(50) << "   10     Connectedness           " << endl;
	cout << endl << endl;
	cout << "Select your option: ";
	cin >> option;			// get option from user
							// did they enter a good value
	while (cin.fail() || option > 10  ||  option < 0){
		cin.clear();
		cin.ignore(80, '\n');	        // skip bad data
		cerr << "Invalid option.  Please select again:";
		cin >> option;		        // get new value
	}
	return option;			// send option back
}


void displayVector(vector<char> v){
  // pre:   The parameter v has been filled with the sequence of vertices
  // post:  The vertices are displayed on one line with a space inbetween
	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << " ";
    }
}

void toContinue(){
  // pre:  The user has entered some data at some point in the program
  // post: The processing will stall until the user hits the enter key
	cin.ignore(80, '\n');
	cout << "Hit the enter key to continue: ";
	char junk = cin.get();
}

int main() {
	Graph myGraph;                 	// the graph object
	int option;                    	// the menu option selected by the user
	string filename;               	// the name of the file entered by the user when
									//    asked for the name of the file containing
									//    the graph inforamtion
	bool success = false;          	// becomes true once the graph has been built

	option = Menu();               	// primal action - what does the user want to do?
	while (option != 0)            	// selection 0 - ends processing
	{
		switch (option)
		{
			case 1:                 // selection 1 - build the graph
				if (success)		// allow this option only 1 time
				{
					cout << "You cannot rebuild the graph." << endl;
					toContinue();
					break;
				}
				cout << "Enter the name of the file that contains the graph: ";
				cin >> filename;
				if (!myGraph.build(filename))
				{
					cout << filename << " is not a good file" << endl;
					cout << "Build of the graph was UNSUCCESSFUL" << endl;
					cout << "Program aborted" << endl;
					exit(0);
				}
				else
				{
					cout << "Build of the graph was successful" << endl;
					toContinue();
				}
				success = true;
				break;
			case 2:                 // selection 2 - depth first traversal
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				vector<char> BFtraversal;
				char start;
				cout << "The graph:" << endl;
				cout << myGraph << endl;
				cout << "At which node do you wish to start? ";
				cin >> start;
				BFtraversal = myGraph.breadthFirst(start);
				cout << "\nBreadth First Traversal: ";
				displayVector(BFtraversal);
				cout << endl;
				toContinue();
				break;
			}
			case 3:                 // selection 3 - depth first traversal
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				vector<char> DFtraversal;
				char start;
				cout << "The graph:" << endl;
				cout << myGraph << endl;
				cout << "At which node do you wish to start? ";
				cin >> start;
				DFtraversal = myGraph.depthFirst(start);
				cout << "\nDepth First Traversal: ";
				displayVector(DFtraversal);
				cout << endl;
				toContinue();
				break;
			}
			case 4:                 // selection - minimum spanning tree
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				vector< pair<char, char> > MST;
				vector< pair<char, char> >::iterator itr;
				MST = myGraph.minimumSpanningTree();
				cout << "\nMinimum spanning tree: " << endl;
				for (itr = MST.begin(); itr != MST.end(); itr++)
				{
					cout << itr->first << "..." << itr->second << endl;
				}
				toContinue();
				break;
			}
			case 5:                 // selection - shortest path
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				vector<char> shortestPath;
				char start;
				char finish;
				cout << "The graph:" << endl;
				cout << myGraph << endl;
				cout << "At which node do you wish to start? ";
				cin >> start;
				cout << "At which node do you wish to finish? ";
				cin >> finish;
				shortestPath = myGraph.shortestPath(start, finish);
				cout << "\nShortest path: ";
				displayVector(shortestPath);
				cout << endl;
				toContinue();
				break;
			}
			case 6:                 // selection - is it Eulerian
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				if (myGraph.isEulerian())
					cout << "This graph has an Euler circuit." << endl;
				else
					cout << "This graph does NOT have an Euler circuit." << endl;
				toContinue();
				break;
			}
			case 7:                 // selection - Euler circuit
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				vector< pair<char, char> > EC;
				vector< pair<char, char> >::iterator itr;
				EC = myGraph.eulerCircuit();
				cout << "\nEuler Circuit: " << endl;
				for (itr = EC.begin(); itr != EC.end(); itr++)
				{
					cout << itr->first << "..." << itr->second << endl;
				}
				toContinue();
				break;
			}
			case 8:                 // selection 8 - display the graph
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				cout << "The graph:" << endl;
				cout << myGraph << endl;
				toContinue();
				break;
			}
			case 9:
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				int numedge = myGraph.NumberOfEdges();
				cout << "Here are the Number of Edges: " << numedge << endl;
				toContinue();
				break;
			}
			case 10:
			{
				if (!success)
				{
					cout << "You must build a graph first." << endl;
					toContinue();
					break;
				}
				if(myGraph.IsConnected() == false)
				{
					cout << "Graph is Not Connected" << endl;
				} else {
					cout << "Graph is Connected" << endl;
				}
				toContinue();
				break;
			}
		}
		option = Menu();
	}

	return 0;
}
