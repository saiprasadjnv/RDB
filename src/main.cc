
#include <iostream>
#include "ParseTree.h"
#include "Statistics.h"
#include "QueryOptimizer.h" 
#include <stdlib.h>
#include <thread>
#include <stdio.h>
#include "DatabaseController.h"
// #include "y.tab.c"
using namespace std;

extern "C" {
	struct YY_BUFFER_STATE; 
	int yyparse(void);   // defined in y.tab.c
	struct YY_BUFFER_STATE *yy_scan_string(const char*);
	void yy_delete_buffer(YY_BUFFER_STATE buffer);
	// struct yy_buffer_state YY_BUFFER_STATE; 
}

extern struct TableList* tables; 
extern struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
extern struct AndList *boolean; // the predicate in the WHERE clause
extern struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
extern struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
extern int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
extern int distinctFunc;  // 1
extern char *fileType;
extern struct SchemaAttributes *schemaAttributes;
extern struct SortAttributes *sortAttributes;
extern char *fileName;
extern int operationType;

void printFunction(struct FuncOperator *currFunction){
	if(currFunction == NULL){ 
		return; 
	}
	printFunction(currFunction->leftOperator); 
	cout << "code " << finalFunction->code << " ";
	if(finalFunction->leftOperand != NULL){
		cout << " ,left Operand " << finalFunction->leftOperand->code << " : " << finalFunction->leftOperand->value << "\n"; 
	} 
		
	printFunction(currFunction->right); 
}

void printAndList(struct AndList *currAndlist){
	while(currAndlist != NULL){
		struct OrList* currOrlst = currAndlist->left; 
		while(currOrlst != NULL){
			cout << "Left Operand: " << currOrlst->left->left->value << " code: " << currOrlst->left->code << " Right Operand: " << currOrlst->left->right->value << "; ";   
			currOrlst = currOrlst->rightOr; 
		}
		cout << "\n"; 
		currAndlist = currAndlist->rightAnd; 
	}
}


int main () {

	DatabaseController myDBController;

	cout << "Database: \n";
	string cmd; 
	while(1){
		cout << "\nSQL>";
		cmd.clear(); 
		getline(std::cin, cmd);
		YY_BUFFER_STATE *buffer = yy_scan_string(cmd.c_str());
		yyparse(); 
		myDBController.processQuery();
		cmd.clear(); 
		// line.clear();
		// this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}

