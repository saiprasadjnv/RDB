 
%{

	#include "ParseTree.h" 
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <iostream>

	extern "C" int yylex();
	extern "C" int yyparse();
	extern "C" void yyerror(char *s);
  
	// these data structures hold the result of the parsing
	struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
	struct TableList *tables; // the list of tables and aliases in the query
	struct AndList *boolean; // the predicate in the WHERE clause
	struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
	struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
	int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
	int distinctFunc;  // 1 if there is a DISTINCT in an aggregate query
	struct SchemaAttributes *schemaAttributes; // list of the attributes to create
	char *fileType; //fileType
	char *fileName; // file from which data is loaded in insert command.
	struct SortAttributes *sortAttributes; // attributes to sort on, used for sortedfile
	int operationType; //Operation type like create|select|insert..

%}

// this stores all of the types returned by production rules
%union {
 	struct FuncOperand *myOperand;
	struct FuncOperator *myOperator; 
	struct TableList *myTables;
	struct ComparisonOp *myComparison;
	struct Operand *myBoolOperand;
	struct OrList *myOrList;
	struct AndList *myAndList;
	struct NameList *myNames;
	char *actualChars;
	char whichOne;
	struct SchemaAttributes *myCreateAtts;
	struct SortAttributes *mySortAtts;
}

%token <actualChars> Name
%token <actualChars> Float
%token <actualChars> Int
%token <actualChars> String
%token SELECT
%token GROUP 
%token DISTINCT
%token BY
%token FROM
%token WHERE
%token SUM
%token AS
%token AND
%token OR
%token CREATE
%token TABLE
%token HEAP
%token SORTED
%token ON
%token INTEGER
%token DBL
%token STR
%token INSERT
%token INTO
%token DROP
%token SET
%token OUTPUT
%token STDOUT
%token NONE
%token UPDATE
%token STATISTICS
%token FOR


%type <myOrList> OrList
%type <myAndList> AndList
%type <myOperand> SimpleExp
%type <myOperator> CompoundExp
%type <whichOne> Op 
%type <myComparison> BoolComp
%type <myComparison> Condition
%type <myTables> Tables
%type <myTables> CTables
%type <myBoolOperand> Literal
%type <myNames> Atts
%type <myCreateAtts> CAtts
%type <mySortAtts> sAtts

%start SQL


//******************************************************************************
// SECTION 3
//******************************************************************************
/* This is the PRODUCTION RULES section which defines how to "understand" the 
 * input language and what action to take for each "statment"
 */

%%

SQL: SELECT WhatIWant FROM Tables WHERE AndList
{
	fileType =NULL;
	groupingAtts = NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileName = NULL;
	tables = $4;
	boolean = $6;	
	operationType=1;
}

| SELECT WhatIWant FROM Tables WHERE AndList GROUP BY Atts
{
	fileType =NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileName = NULL;
	tables = $4;
	boolean = $6;	
	groupingAtts = $9;
	operationType=1;
}

| CREATE TABLE CTables WhatCWant  AS fleType 
{
	boolean = NULL;	
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	sortAttributes=NULL;
	attsToSelect=NULL;
	fileName = NULL;
	tables = $3;
	operationType=2;
	fileType= strdup("HEAP");
}

| CREATE TABLE CTables WhatCWant AS SORTED ON sAtts
{
	boolean = NULL;	
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	fileName = NULL;
	tables = $3;
	fileType = (char*) malloc(sizeof ("SORTED"));
	strcpy(fileType,"SORTED");
	sortAttributes=$8;
	operationType=2;
}
|INSERT String INTO CTables';'
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables = $4;
	fileName = $2;
	operationType=3;

}
|INSERT String INTO CTables
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables = $4;
	fileName = $2;
	operationType=3;

}
|DROP TABLE CTables
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	fileName=NULL;
	tables = $3;
	operationType=4;
}
|DROP TABLE CTables';'
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	fileName=NULL;
	tables = $3;
	operationType=4;
}
| SET OUTPUT String
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName = strdup($3);
	operationType=5;
}
|SET OUTPUT String';'
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName = strdup($3);
	operationType=5;
}
|SET OUTPUT STDOUT
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName=strdup("STDOUT");
	operationType=5;
}
|SET OUTPUT STDOUT';'
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName=strdup("STDOUT");
	operationType=5;
}
|SET OUTPUT NONE
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName = strdup("NONE");
	operationType=5;
}
|SET OUTPUT NONE';'
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	tables=NULL;
	fileName = strdup("NONE");
	operationType=5;
}
|UPDATE STATISTICS FOR CTables
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	fileName=NULL;
	tables = $4;
	operationType=6;
}
|UPDATE STATISTICS FOR CTables';'
{
	boolean=NULL;
	groupingAtts = NULL;
	finalFunction=NULL;
	distinctAtts=0;
	distinctFunc=0;
	attsToSelect=NULL;
	sortAttributes=NULL;
	schemaAttributes=NULL;
	fileType=NULL;
	fileName=NULL;
	tables = $4;
	operationType=6;
}

WhatIWant: Function ',' Atts 
{
	attsToSelect = $3;
	distinctAtts = 0;
}

| Function
{
	attsToSelect = NULL;
}

| Atts 
{
	distinctAtts = 0;
	finalFunction = NULL;
	attsToSelect = $1;
}

| DISTINCT Atts
{
	distinctAtts = 1;
	finalFunction = NULL;
	attsToSelect = $2;
	finalFunction = NULL;
};

Function: SUM '(' CompoundExp ')'
{
	distinctFunc = 0;
	finalFunction = $3;
}

| SUM DISTINCT '(' CompoundExp ')'
{
	distinctFunc = 1;
	finalFunction = $4;
};

Atts: Name
{
	$$ = (struct NameList *) malloc (sizeof (struct NameList));
	$$->name = $1;
	$$->next = NULL;
} 

| Atts ',' Name
{
	$$ = (struct NameList *) malloc (sizeof (struct NameList));
	$$->name = $3;
	$$->next = $1;
}

Tables: Name AS Name 
{
	$$ = (struct TableList *) malloc (sizeof (struct TableList));
	$$->tableName = $1;
	$$->aliasAs = $3;
	$$->next = NULL;
}

| Tables ',' Name AS Name
{
	$$ = (struct TableList *) malloc (sizeof (struct TableList));
	$$->tableName = $3;
	$$->aliasAs = $5;
	$$->next = $1;
}



CompoundExp: SimpleExp Op CompoundExp
{
	$$ = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	$$->leftOperator = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));
	$$->leftOperator->leftOperator = NULL;
	$$->leftOperator->leftOperand = $1;
	$$->leftOperator->right = NULL;
	$$->leftOperand = NULL;
	$$->right = $3;
	$$->code = $2;	

}

| '(' CompoundExp ')' Op CompoundExp
{
	$$ = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	$$->leftOperator = $2;
	$$->leftOperand = NULL;
	$$->right = $5;
	$$->code = $4;	

}

| '(' CompoundExp ')'
{
	$$ = $2;

}

| SimpleExp
{
	$$ = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	$$->leftOperator = NULL;
	$$->leftOperand = $1;
	$$->right = NULL;	

}

| '-' CompoundExp
{
	$$ = (struct FuncOperator *) malloc (sizeof (struct FuncOperator));	
	$$->leftOperator = $2;
	$$->leftOperand = NULL;
	$$->right = NULL;	
	$$->code = '-';

}
;

Op: '-'
{
	$$ = '-';
}

| '+'
{
	$$ = '+';
}

| '*'
{
	$$ = '*';
}

| '/'
{
	$$ = '/';
}
;

AndList: '(' OrList ')' AND AndList
{
        // here we need to pre-pend the OrList to the AndList
        // first we allocate space for this node
        $$ = (struct AndList *) malloc (sizeof (struct AndList));

        // hang the OrList off of the left
        $$->left = $2;

        // hang the AndList off of the right
        $$->rightAnd = $5;

}

| '(' OrList ')'
{
        // just return the OrList!
        $$ = (struct AndList *) malloc (sizeof (struct AndList));
        $$->left = $2;
        $$->rightAnd = NULL;
}
;

OrList: Condition OR OrList
{
        // here we have to hang the condition off the left of the OrList
        $$ = (struct OrList *) malloc (sizeof (struct OrList));
        $$->left = $1;
        $$->rightOr = $3;
}

| Condition
{
        // nothing to hang off of the right
        $$ = (struct OrList *) malloc (sizeof (struct OrList));
        $$->left = $1;
        $$->rightOr = NULL;
}
;

Condition: Literal BoolComp Literal
{
        // in this case we have a simple literal/variable comparison
        $$ = $2;
        $$->left = $1;
        $$->right = $3;
}
;

BoolComp: '<'
{
        // construct and send up the comparison
        $$ = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        $$->code = LESS_THAN;
}

| '>'
{
        // construct and send up the comparison
        $$ = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        $$->code = GREATER_THAN;
}

| '='
{
        // construct and send up the comparison
        $$ = (struct ComparisonOp *) malloc (sizeof (struct ComparisonOp));
        $$->code = EQUALS;
}
;

Literal : String
{
        // construct and send up the operand containing the string
        $$ = (struct Operand *) malloc (sizeof (struct Operand));
        $$->code = STRING;
        $$->value = $1;
}

| Float
{
        // construct and send up the operand containing the FP number
        $$ = (struct Operand *) malloc (sizeof (struct Operand));
        $$->code = DOUBLE;
        $$->value = $1;
}

| Int
{
        // construct and send up the operand containing the integer
        $$ = (struct Operand *) malloc (sizeof (struct Operand));
        $$->code = INT;
        $$->value = $1;
}

| Name
{
        // construct and send up the operand containing the name
        $$ = (struct Operand *) malloc (sizeof (struct Operand));
        $$->code = NAME;
        $$->value = $1;
}
;


SimpleExp: 

Float
{
        // construct and send up the operand containing the FP number
        $$ = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        $$->code = DOUBLE;
        $$->value = $1;
} 

| Int
{
        // construct and send up the operand containing the integer
        $$ = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        $$->code = INT;
        $$->value = $1;
} 

| Name
{
        // construct and send up the operand containing the name
        $$ = (struct FuncOperand *) malloc (sizeof (struct FuncOperand));
        $$->code = NAME;
        $$->value = $1;
}
;

WhatCWant: '(' CAtts ')' 
{
	schemaAttributes=$2;
};

CAtts: Name INTEGER
{
	$$ = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	$$->name=$1;
	$$->type=2;
	$$->next=NULL;
}
| Name DBL
{
	$$ = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	$$->name=$1;
	$$->type=1;
	$$->next=NULL;

}
| Name STR
{
	$$ = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	$$->name=$1;
	$$->type=4;
	$$->next=NULL;
}
| Name INTEGER',' CAtts
{
	$$ = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	$$->name=$1;
	$$->type=2;
	$$->next=$4;
}
| Name DBL',' CAtts 
{
	$$ = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	$$->name=$1;
	$$->type=1;
	$$->next=$4;

}
|Name STR',' CAtts
{
	$$ = (struct SchemaAttributes*) malloc(sizeof (struct SchemaAttributes));
	$$->name=$1;
	$$->type=4;
	$$->next=$4;
};

CTables: Name
{
	$$ = (struct TableList *) malloc (sizeof (struct TableList));
	$$->tableName = $1;
	$$->aliasAs=NULL;
	$$->next = NULL;
}

| Name ',' CTables
{
	$$ = (struct TableList *) malloc (sizeof (struct TableList));
	$$->tableName = $1;
	$$->aliasAs=NULL;
	$$->next = $3;
};

sAtts: Name';'
{
	$$ = (struct SortAttributes*) malloc (sizeof (struct SortAttributes));
	$$->name=$1;
	$$->next=NULL;

}
| Name',' sAtts
{
	$$ = (struct SortAttributes*) malloc (sizeof (struct SortAttributes));
	$$->name=$1;
	$$->next=$3;
}
|Name
{
	$$ = (struct SortAttributes*) malloc (sizeof (struct SortAttributes));
	$$->name=$1;
	$$->next=NULL;

};

fleType: HEAP';'
{
	fileType = (char*) malloc(sizeof ("HEAP"));
	strcpy(fileType,"HEAP");
}
| HEAP
{
	fileType = (char*) malloc(sizeof ("HEAP"));
	strcpy(fileType,"HEAP");
};

%%

