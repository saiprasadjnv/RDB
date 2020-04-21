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
	//Comparison functor for the map. 
	struct cmp_str
	{
		bool operator()(char const *a, char const *b) const
		{
			return std::strcmp(a, b) < 0;
		} 
	};

	struct cmp_str2
	{
		bool operator()(vector<char*> a, vector<char*> b ) const
		{
			for(int i =0; i< min<int>(a.size(),b.size()); i++){
				if(strcmp(a[i],b[i])==0){
					continue; 
				}else{
					return std::strcmp(a[i], b[i])<0;
				}
			} 
			return 0;
		}  
	};


	map < vector<string>, map<string, ll> > StatisticsTable; 
	
	// Each time an estimate is called, this vector holds the possible changes in the Statistics object 
	// The changes are committed to the Statistics object only when the Apply method is called. 
	vector <pair <string, ll>> tempState; 

	map <string, ll > selectTempState;

	//It takes the selectivity factor as input and 
	//returns the total number of output tuples for the given operation. 
	double getNumOfTuples(vector <vector <string> > &partitions, double fraction);

public:
	//Default constructor. 
	Statistics();

	//Takes the copyMe Statistics object and performs deep copy.
	//Initializes the current Object state with the state of copyMe object. 
	Statistics(Statistics &copyMe);	
	~Statistics();

	//Adds the relation to the Statistics object. 
	void AddRel(char *relName, int numTuples);

	//Adds the attribute corresponding to an existing relation in the Statistics object. 
	//Exits the program if relation doesn't exist.
	void AddAtt(char *relName, char *attName, int numDistincts);

	//Produces a copy of the relation given by "oldName" under "newName". 
	void CopyRel(char *oldName, char *newName);
	
	//Initializes the state of the current Statistics object 
	//using the data available in the file "fromWhere". 
	void Read(char *fromWhere);

	//Writes the state of the current Statistics object to the file "fromWhere". 
	void Write(char *fromWhere);

	//Applies the join operation in the current Statistics object. 
	//Merges the partitions which are used in the join operation and updates the 
	//information on Total Number of tuples and the number of distinct values for each attribute. 
	void  Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
	
	//Returns the estimated number of output tuples for the given operation.  
	double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);

	//This method checks if the given relations given are consistent with the existing relation Partitions
	//in the StatisticsTable. Returns 1 if consistent. Exits the program if inconsistent. 
	int CheckifRelsExist(char* relNames[], int numToJoin, vector <vector <string> > &partitions); 
	
	//This method checks if the given CNF in the parseTree is consistent with the relations in the relNames. 
	//Returns 1 if consistent. Exits the program if inconsistent. 
	int checkAndGetAttVal(vector <vector <string> > &partitions, char* attName);  
	
	//Prints the StatisticsTable. Used for debugging. 
	void PrintStatistics(); 

	//Processes the given orList and returns the selectivity factor of the entire orList. 
	double processOrlist(ll numOfinputTuples, struct OrList* myOrlist, vector <vector <string> > &partitions); 
};

#endif
