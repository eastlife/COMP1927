// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"

void beforeTest (DLList myList);
void afterTest (DLList myList);
void moveTestPreview (DLList myList, int code); 

typedef struct DLListNode {
	char   *value;  // value of this list item (string)
	struct DLListNode *prev;
	               // pointer previous node in list
	struct DLListNode *next;
	               // pointer to next node in list
} DLListNode;

typedef struct DLListRep {
	int  nitems;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *curr;  // current node in list
	DLListNode *last;  // last node in list
} DLListRep;


int main(int argc, char *argv[]) {

	DLList myList;
	myList = getDLList(stdin);
	putDLList(stdout,myList);


	DLList emptyL = newDLList(); // creates an empty list for testing

    FILE *f1;
    f1 = fopen("test01.c","rt+");
	DLList oneLineL = getDLList(f1);

    FILE *f2;
    f2 = fopen("test02.c","rt+");
	DLList manyLineL = getDLList(f2);

	assert(validDLList(myList)); // checks all test files/copies are legitimate lists
	assert(validDLList(emptyL));
	assert(validDLList(oneLineL));
	assert(validDLList(manyLineL));


	// Testing DLListDelete 

	printf("\n\n*******************************\n");
	printf("Now testing DLListDelete, good luck!\n");
	printf("*******************************\n\n");

	beforeTest(myList); // shows status of text before action
	printf("Initiating DLListDelete [1]...\n");
	DLListDelete(myList);
	afterTest(myList); // shows status of text after action 
	assert(validDLList(myList)); // verifies is still a functioning list 
	printf("Test 1 passed!\n");

	beforeTest(emptyL); 
	printf("Initiating DLListDelete [2]...\n");
	DLListDelete(emptyL);
	afterTest(emptyL);
	assert(validDLList(emptyL));
	printf("Test 2 passed!\n");

	beforeTest(oneLineL); 
	printf("Initiating DLListDelete [3]...\n");
	DLListDelete(oneLineL);
	afterTest(oneLineL);
	assert(validDLList(oneLineL));
	printf("Test 3 passed!\n");

	beforeTest (manyLineL); 
	printf("Initiating DLListDelete - Removing one line [4.1]...\n");
	DLListDelete(manyLineL);
	afterTest(manyLineL);
	assert(validDLList(manyLineL));
	beforeTest (manyLineL); 
	printf("Initiating DLListDelete [4.2] - Removing line 3 lines down...\n");
	manyLineL->curr = manyLineL->curr->next->next->next; 
	DLListDelete(manyLineL);
	afterTest(manyLineL);
	assert(validDLList(manyLineL));
	printf("Test 4 passed! Good job!\n");

	// Testing DLListBefore 

	printf("\n\n*******************************\n");
	printf("Now testing DLListBefore, good luck!\n");
	printf("*******************************\n\n");

	beforeTest (myList); 
	printf("Initiating DLListBefore [1.1]...\n");
	printf("Line being added: 'Testing, testing, 1, 2, 3...'\n\n");
	DLListBefore(myList, "Testing, testing, 1, 2, 3...");
	afterTest (myList);
	assert(validDLList(myList));
	printf("Initiating DLListBefore [1.2]...\n");
	printf("Line being added: 'Header line: Boxian Deng & Michael Italiano'\n\n");
	DLListBefore(myList, "Header line: Boxian Deng & Michael Italiano");
	afterTest (myList);
	assert(validDLList(myList));
	printf("Test 5 passed!\n");

	emptyL = newDLList();

	beforeTest (emptyL); 
	printf("Initiating DLListBefore [2.1]...\n");
	printf("Line being added: 'Adding line before curr to an empty list'\n\n");
	DLListBefore(emptyL, "Adding line before curr to an empty list");
	afterTest (emptyL);
	assert(validDLList(emptyL));
	printf("Initiating DLListBefore [2.2]...\n");
	printf("Line being added: 'adding another line...don't mind me.'\n\n");
	DLListBefore(emptyL, "adding another line...don't mind me.");	
	afterTest (emptyL);
	assert(validDLList(emptyL));
	printf("Test 6 passed!\n");	

	beforeTest (oneLineL); 
	printf("Initiating DLListBefore [3.1]...\n");
	printf("Line being added: 'i'm good, thanks.'\n\n");
	DLListBefore(oneLineL, "i'm good, thanks.");
	afterTest (oneLineL);
	assert(validDLList(oneLineL));
	printf("Initiating DLListBefore [3.2]...\n");
	printf("Line being added: 'how are you?'\n\n");
	DLListBefore(oneLineL, "how are you?");	
	afterTest (oneLineL);
	assert(validDLList(oneLineL));
	printf("Test 7 passed!\n");	

	beforeTest (manyLineL); 
	printf("Initiating DLListBefore [3.1]...\n");
	printf("Line being added: 'dead beef'\n\n");
	DLListBefore(manyLineL, "dead beef");
	afterTest (manyLineL);
	assert(validDLList(manyLineL));
	printf("Initiating DLListBefore [3.2]...\n");
	printf("Line being added: 'canary'\n\n");
	DLListBefore(manyLineL, "canary");	
	afterTest (manyLineL);
	assert(validDLList(manyLineL));
	printf("Let's up the ante...current being moved.\n\n");
	manyLineL->curr = manyLineL->curr->next->next->next; 
	printf("Initiating DLListBefore [3.3]...\n");
	printf("Line being added: '1234211111 week04 111113155122'\n\n");
	DLListBefore(manyLineL, "1234211111 week04 111113155122");	
	afterTest (manyLineL);
	assert(validDLList(manyLineL));
	printf("Test 8 passed! Impressive...\n");	

	// Testing DLListAfter 

	printf("\n\n*******************************\n");
	printf("Now testing DLListAfter, good luck!\n");
	printf("*******************************\n\n");

	beforeTest (myList); 
	printf("Initiating DLListAfter [1.1]...\n");
	printf("Line being added: 'I'm\n\n");
	DLListAfter(myList, "I'm");
	afterTest (myList);
	assert(validDLList(myList));
	printf("Initiating DLListAfter [1.2]...\n");
	printf("Line being added: 'running out of things to say'\n\n");
	DLListAfter(myList, "running out of things to say");
	afterTest (myList);
	assert(validDLList(myList));
	printf("Test 9 passed!\n");

	emptyL = newDLList();

	beforeTest (emptyL); 
	printf("Initiating DLListAfter [2.1]...\n");
	printf("Line being added: 'pow pow pow'\n\n");
	DLListAfter(emptyL, "pow pow pow");
	afterTest (emptyL);
	assert(validDLList(emptyL));
	printf("Initiating DLListAfter [2.2]...\n");
	printf("Line being added: 'pew pew pew'\n\n");
	DLListAfter(emptyL, "pew pew pew");	
	afterTest (emptyL);
	assert(validDLList(emptyL));
	printf("Test 10 passed!\n");	

	beforeTest (oneLineL); 
	printf("Initiating DLListAfter [3.1]...\n");
	printf("Line being added: 'COMP1927 s2 2016'\n\n");
	DLListAfter(oneLineL, "COMP1927 s2 2016");
	afterTest (oneLineL);
	assert(validDLList(oneLineL));
	printf("Initiating DLListAfter [3.2]...\n");
	printf("Line being added: 'Michael & Boxian'\n\n");
	DLListAfter(oneLineL, "Michael & Boxian");	
	afterTest (oneLineL);
	assert(validDLList(oneLineL));
	printf("Test 11 passed!\n");	

	beforeTest (manyLineL); 
	printf("Initiating DLListAfter [4.1]...\n");
	printf("Line being added: 'testing'\n\n");
	DLListAfter(manyLineL, "testing");
	afterTest (manyLineL);
	assert(validDLList(manyLineL));
	printf("Initiating DLListAfter [4.2]...\n");
	printf("Line being added: 'DLListAfter'\n\n");
	DLListAfter(manyLineL, "DLListAfter");	
	afterTest (manyLineL);
	assert(validDLList(manyLineL));
	printf("Let's up the ante...current being moved two lines down.\n\n");
	manyLineL->curr = manyLineL->curr->next->next; 
	printf("Initiating DLListAfter [3.3]...\n");
	printf("Line being added: 'interrupting line'\n\n");
	DLListAfter(manyLineL, "interrupting line");	
	afterTest (manyLineL);
	assert(validDLList(manyLineL));
	printf("Test 12 passed! *phew...\n");	

	// Testing DLListMoveTo

	printf("\n\n*******************************\n");
	printf("Now testing DLListMoveTo, good luck Jas!\n");
	printf("*******************************\n\n");

	moveTestPreview (myList, 0);
	printf("Initiating DLListMoveTo [1]...\n");
	printf("Moving to 2nd node...\n");
	DLListMoveTo(myList, 2);
	moveTestPreview (myList, 1);
	assert(validDLList(myList));
	printf("\nMoving to 1st node...\n");
	DLListMoveTo(myList, 1);
	moveTestPreview (myList, 1);
	assert(validDLList(myList));
	printf("\nTest 13 passed!\n");

	emptyL = newDLList();

	printf("Testing DLListMoveTo with an empty list...\n");
	moveTestPreview (emptyL , 0);
	printf("Initiating DLListMoveTo [2]...\n");
	printf("No move should occur but let's try go to node 100..\n");
	DLListMoveTo(emptyL, 100);
	moveTestPreview (emptyL , 1);
	assert(validDLList(emptyL));
	printf("\nTest 14 passed!\n");	

	moveTestPreview (oneLineL, 0);
	printf("Initiating DLListMoveTo [3.1]...\n");
	printf("Moving to node 2...'\n");
	DLListMoveTo(oneLineL, 2);
	moveTestPreview (oneLineL, 1);
	assert(validDLList(oneLineL));
	printf("Initiating DLListMoveTo [3.2]...\n");
	printf("Moving back to first node...\n");
	DLListMoveTo(oneLineL, 1);	
	moveTestPreview (oneLineL, 1);
	assert(validDLList(oneLineL));
	printf("\nTest 15 passed!\n");	

	moveTestPreview (manyLineL, 0); 
	printf("Initiating DLListMoveTo [4.1]...\n");
	printf("Moving to 5th node\n");
	DLListMoveTo(manyLineL, 5);
	moveTestPreview (manyLineL, 1);
	assert(validDLList(manyLineL));
	printf("Initiating DLListAfter [4.2]...\n");
	printf("Moving to second node...'\n");
	DLListMoveTo(manyLineL, 2);	
	moveTestPreview (manyLineL, 1);
	assert(validDLList(manyLineL));
	printf("\nTest 16 passed! Good work Jas...\n");	

	// Final test using all functions 

	printf("\n\n*******************************\n");
	printf("FINAL TESTING PHASE\n");
	printf("*******************************\n\n");

	emptyL = newDLList();

	printf("Testing many functions on an originally empty list.");
	printf("The functions should successfully populate and manipulate this list.");

	beforeTest(emptyL);

	printf("Adding line to start using DLListBefore - line reads");
	printf("'One test to rule them all!'\n");
	DLListBefore(emptyL, "One test to rule them all!");

	afterTest(emptyL);

	printf("Adding two consecutive lines using DLListAfter twice\n");
	printf("Aside: these lines won't make sense...yet\n");
	DLListAfter(emptyL, "This file must");
	DLListAfter(emptyL, "to thereby be deemed successful.");

	afterTest(emptyL);

	printf("Now moving to 2nd node...\n");
	DLListMoveTo(emptyL, 2);	

	afterTest(emptyL);

	printf("Using DLListAfter again twice.\n");
	DLListAfter(emptyL, "utilise each function");
	DLListAfter(emptyL, "bla bla bla - you're gonna have to delete me - bla bla bla");

	afterTest(emptyL);

	printf("Deleting that nuisance.\n");
	DLListDelete(emptyL);

	afterTest(emptyL);

	printf("Moving to node 1 and then inserting a final header line using DLListBefore\n"); 
	DLListMoveTo(emptyL, 1);
	DLListBefore(emptyL, "COMP1927 week04: Michael & Boxian");

	afterTest(emptyL);

	assert(validDLList(emptyL));

	printf("If the above is now a beautiful text file of 5 lines including the header...\n");
	printf("Congratulations! All tests passed, you're awesome!\n");

	return 0;
}

void beforeTest (DLList myList) {

	printf("\n*** STATE OF FILE BEFORE TEST: *** \n\n");
	printf("Number of lines before action: %d\n", DLListLength(myList));

	struct DLListNode *revertCurr; 
	revertCurr = myList->curr; 

	if (myList->curr != NULL) { 

		printf("Current line: '%s' \n", myList->curr->value);

	} else { 

		printf("Current line: No current line. List is empty.\n");

	}


	printf("\n*** PREVIEWING TEXT...\n\n");

	myList->curr = myList->first;

	if (myList->first != NULL) { 

		while (myList->curr != NULL) { // prints whole file

			printf("%s\n", myList->curr->value);
			myList->curr = myList->curr->next;

		}

	}

	printf("\n...PREVIEW END ***\n\n");

	myList->curr = revertCurr; 

	return;

}


void afterTest (DLList myList) {

	printf("\n*** STATE OF FILE AFTER TEST: *** \n\n");
	printf("Number of lines after action: %d\n", DLListLength(myList));

	struct DLListNode *revertCurr; 
	revertCurr = myList->curr; 

	if (myList->curr != NULL) { 

		printf("Current line: '%s' \n", myList->curr->value);

	} else { 

		printf("Current line: No current line. List is empty.\n");

	}

	printf("\n*** PREVIEWING TEXT... \n\n");

	myList->curr = myList->first;

	if (myList->first != NULL) { 

		myList->curr = myList->first;

		while (myList->curr != NULL) { // prints whole file

			printf("%s\n", myList->curr->value);
			myList->curr = myList->curr->next;

		}

	}

	printf("\n...PREVIEW END ***\n\n");

	myList->curr = revertCurr;

	return;

}

void moveTestPreview (DLList myList, int code) {

	if (myList != NULL) { 

		if (code == 0) { // before 

			struct DLListNode *revertCurr; 
			revertCurr = myList->curr; 

			printf("\n*** STATE OF FILE BEFORE TEST: *** \n\n");
			printf("Number of lines before action: %d\n", DLListLength(myList));

			if (myList->curr != NULL) { 

				printf("Current line: '%s' \n", myList->curr->value);

			} else { 

				printf("Current line: No current line. List is empty.\n");

			}

			myList->curr = myList->first; 

			printf("\n*** PREVIEWING TEXT... \n\n");

			while (myList->curr != NULL) { // prints whole file

			printf("%s\n", myList->curr->value);
			myList->curr = myList->curr->next;

			}

			printf("\n...PREVIEW END ***\n\n");

			myList->curr = revertCurr; 

		} else if (code == 1) { // after

			printf("\n*** STATE OF FILE AFTER TEST: *** \n\n");
			printf("Number of lines after action: %d\n", DLListLength(myList));

			if (myList->curr != NULL) { 

				printf("Current line: '%s' \n", myList->curr->value);

			} else { 

				printf("Current line: No current line. List is empty.\n");

			}

		}

	} 

	return;

}

