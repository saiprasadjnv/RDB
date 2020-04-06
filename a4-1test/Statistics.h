#ifndef STATISTICS_
#define STATISTICS_
#include "ParseTree.h"
#include <map>
#include <vector>
#include <string> 
#include "cstring"

using namespace std; 
#define ll long long

class Statistics
{
private: 
	struct cmp_str
	{
		bool operator()(char const *a, char const *b) const
		{
			return std::strcmp(a, b) < 0;
		}
		map <vector <char*>, pair<char*, int> > tempState; 
	};
	map < vector<char*>, map<char*, ll, cmp_str> > StatisticsTable; 
	// map < vector <char*>, map <char*, int, cmp_str> > tempState; 
	vector <pair <char*, ll>> tempState; 
	double getNumOfTuples(vector <vector <char*> > &partitions, double fraction);

public:
	Statistics();
	Statistics(Statistics &copyMe);	 // Performs deep copy
	~Statistics();


	void AddRel(char *relName, int numTuples);
	void AddAtt(char *relName, char *attName, int numDistincts);
	void CopyRel(char *oldName, char *newName);
	
	void Read(char *fromWhere);
	void Write(char *fromWhere);

	void  Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
	
	double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);

	//This method checks if the given relations given are consistent with the existing relation Partitions
	//in the StatisticsTable. Returns 1 if consistent. Exits the program if inconsistent. 
	int CheckifRelsExist(char* relNames[], int numToJoin, vector <vector <char*> > &partitions); 
	
	//This method checks if the given CNF in the parseTree is consistent with the relations in the relNames. 
	//Returns 1 if consistent. Exits the program if inconsistent. 
	int checkAndGetAttVal(vector <vector <char*> > &partitions, char* attName, int &whichPartition);  
	
	//Prints the StatisticsTable. Used for debugging. 
	void PrintStatistics(); 

	int GetFilteredTuples(OrList or1, ll numOfTuples);

	double processOrlist(ll numOfinputTuples, struct OrList* myOrlist, vector <vector <char*> > &partitions); 
};

#endif
