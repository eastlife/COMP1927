// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include "stack.h"
#include "set.h"
#include "graph.h"
#include "queue.h"
#include "html.h"
#include "url_file.h"

#define BUFSIZE 1024

void setFirstURL(char *, char *);
void normalise(char *, char *, char *, char *, int);

int main(int argc, char **argv)
{
	URL_FILE *handle;
	URL_FILE *html;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int  maxURLs;

	if (argc > 2) {
		strcpy(baseURL,argv[1]);
		setFirstURL(baseURL,firstURL);
		maxURLs = atoi(argv[2]);
		if (maxURLs < 40) maxURLs = 40;
	}
	else {
		fprintf(stderr, "Usage: %s BaseURL MaxURLs\n",argv[0]);
		exit(1);
	}
		
	// You need to modify the code below to implement:
	//
	// add firstURL to the ToDo list
	// initialise Graph to hold up to maxURLs
	// initialise set of Seen URLs
	// while (ToDo list not empty and Graph not filled) {
	//    grab Next URL from ToDo list
	//    if (not allowed) continue
	//    foreach line in the opened URL {
	//       foreach URL on that line {
	//          if (Graph not filled or both URLs in Graph)
	//             add an edge from Next to this URL
	//          if (this URL not Seen already) {
	//             add it to the Seen set
	//             add it to the ToDo list
	//          }
	//       }
    //    }
	//    close the opened URL
	//    sleep(1)
	// }
	if (!(handle = url_fopen(firstURL, "r"))) {
		fprintf(stderr,"Couldn't open %s\n", next);
		exit(1);
	}

	// add firstURL to the ToDo list
	Queue ToDo = newQueue();
	enterQueue(ToDo, firstURL);

	//initialise Graph to hold up to maxURLs
	Graph g = newGraph(maxURLs);

	// initialise set of Seen URLs
	Set Seen = newSet();

	char currURL[BUFSIZE];
	
	// while (ToDo list not empty and Graph not filled)
	while(!emptyQueue(ToDo) && nVertices(g) < maxURLs) {

		//grab Next URL from ToDo list
		strcpy(currURL, leaveQueue(ToDo) );

		//if (not allowed) continue
		if (!(html = url_fopen(currURL, "r"))) {
			fprintf(stderr,"Couldn't open %s\n", next);
			continue;
		}

		//foreach line in the opened URL {
		while (!url_feof(html) && nVertices(g) < maxURLs){

			url_fgets(buffer,sizeof(buffer),html);
			//fputs(buffer,stdout);	

			int pos = 0;
			char result[BUFSIZE];
			memset(result,0,BUFSIZE);

			//foreach URL on that line {
			while ((pos = GetNextURL(buffer, currURL, result, pos)) > 0 && nVertices(g) < maxURLs) {		//cannot find the next pos, pos goes to -1 instead.

				printf("Found: '%s'\n",result);
				//if (Graph not filled or both URLs in Graph)
				//add an edge from Next to this URL
				if (isConnected(g, currURL, result)) continue;
				addEdge(g, currURL, result);

				//if (this URL not Seen already) {
				//add it to the Seen set
				//add it to the ToDo list
				if (!isElem(Seen, result)){
						insertInto(Seen, result);
						enterQueue(ToDo, result);
				}
				memset(result,0,BUFSIZE);
			}
		}

		url_fclose(html);
		sleep(1);
	}

	url_fclose(handle);
	sleep(1);


	showGraph(g, 0);
	showGraph(g, 1);

	disposeSet(Seen);
	disposeQueue(ToDo);
	disposeGraph(g);

	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
void setFirstURL(char *base, char *first)
{
	char *c;
	if ((c = strstr(base, "/index.html")) != NULL) {
		strcpy(first,base);
		*c = '\0';
	}
	else if (base[strlen(base)-1] == '/') {
		strcpy(first,base);
		strcat(first,"index.html");
		base[strlen(base)-1] = '\0';
	}
	else {
		strcpy(first,base);
		strcat(first,"/index.html");
	}
}
