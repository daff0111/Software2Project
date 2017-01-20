#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>


struct coordinate{
	int x;
	int y;
};

struct arc{
	int a;
	int b;
	int branch; 
	float dist;
	float danger;
};

//linked list of partitions 
struct arcs_list {
	struct arc *partition_arcs[301];
	int arc_count;
	int included;
	int branch;
	float danger;
	struct arcs_list *next;
};

//Order arcs into heap
int lessThan(const struct arc *a1, const struct arc *a2)
{
	//Check that it doesnt generate a new branch first
	if(a1->branch > a2->branch)
	{
		return 1;
	}
	else if(a1->branch < a2->branch)
	{
		return 0;
	}	
	//Check distance as second measure
	if(a1->dist < a2->dist)
	{
		return 1;
	}
	else if(a1->dist > a2->dist)
	{
		return 0;
	}	
	//Lastly check dangerousness
	if(a1->danger < a2->danger)
	{
		return 1;
	}
	else if(a1->danger >= a2->danger)
	{
		return 0;
	}
	return 0;
};

void partition(struct arcs_list *current, int max_branches);

//Get Pivot for quicksort
int getpivot( struct arc **queue, int l, int r) 
{
	int i, j;
	struct arc *pivot = queue[l];
	struct arc *t;
	i = l; j = r;	
	
	while(1)
	{
		do
		{
			i++;
		}
		while(i <= r && (lessThan(pivot, queue[i]) == 1));
		if(lessThan(queue[j], pivot) == 1)
		{
			do
			{
				j--;
			}while(j >= 0 && lessThan(queue[j], pivot) == 1);
		}
		if( i >= j ) break;
		t = queue[i]; queue[i] = queue[j]; queue[j] = t;
	}
	t = queue[l]; queue[l] = queue[j]; queue[j] = t;
	
	return j;
}


//Order Arcs Heap using quicksort
void quickSort( struct arc **queue, int l, int r)
{
   int j;

   if( l < r ) 
   {
       j = getpivot( queue, l, r);
       quickSort( queue, l, j-1);
       quickSort( queue, j+1, r);
   }	
}

float calculate_danger(struct arc **msp, int arc_count){

	int i;
	float danger = 0;

	for(i=0; i<arc_count; i++)
		danger += msp[i]->danger;

	return danger;
}



int main(int argc, char *argv[])
{
	//Clock time the execution
	clock_t begin = clock();

	char* input_file_name = argv[1];
	int input_file_name_size = strlen(input_file_name) - 1; 
	//Overwrite .sol file
	char* output_file_name = (char *) malloc(input_file_name_size);
	strncpy(output_file_name, input_file_name, input_file_name_size);
	strncpy(&output_file_name[input_file_name_size - 3], ".sol\0", 5);

	//Parameters
	int n = 0; //Number of Nodes
	float alpha = 0.0; //Percentage Limit of shortest path
	struct coordinate coord[301]; //Coordinates of the nodes
	float* d; //Dangerousness of paths
	double* distM; //Distances between the nodes
	
	
	//Read the Input file
	FILE* input_file = fopen (input_file_name, "r");
	if( input_file == NULL )
	{
		printf("%s File Not Found.\n", input_file_name);
		return -1;
	}
	char stream[100];
	//param n :=
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", stream); 
	}
	fscanf (input_file, "%d", &n); //read n
	n++; //Count the School;
	printf("n = %d \n",n);
	
	//; param alpha :=
	fscanf (input_file, "%s", stream);
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", stream); 
	}
	fscanf (input_file, "%f", &alpha); //read alpha
	printf("alpha = %f \n",alpha);
	
	
	//; param coordX [*] :=
	fscanf (input_file, "%s", stream);
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", stream); 
	}
	for(int i = 0; i < n; i++)
	{
		int index;
		fscanf (input_file, "%d", &index);
		fscanf (input_file, "%d", &coord[index].x);			
	}
	//; param coordY [*] :=
	fscanf (input_file, "%s", stream);
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", stream); 
	}
	for(int i = 0; i < n; i++)
	{
		int index;
		fscanf (input_file, "%d", &index);
		fscanf (input_file, "%d", &coord[index].y);
	}
	
	//Allocate the danger matrix
	d = (float*) calloc(n*n, sizeof(float));
	//; param d [*,*]: 
	fscanf (input_file, "%s", stream);
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", stream); 
	}
	for(int i = 0; i < n; i++)
	{
		int index;
		fscanf (input_file, "%d", &index);
		//printf("%d ",index);
		for(int j = 0; j < n; j++)
		{
			fscanf (input_file, "%f", &d[i*n+j]);
			//printf("%f ",d[i*n+j]);
		}
		//printf("\n");
	}
	//END of File Input
	fclose (input_file);
	
	//Calculate Distances
	distM = (double*) calloc(n*n, sizeof(double));
	for(int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			int distx = abs(coord[i].x - coord[j].x);
			int disty = abs(coord[i].y - coord[j].y);
			double d2x = distx;
			d2x = d2x*d2x;
			double d2y = disty;
			d2y = d2y*d2y;
			double d = sqrt(d2x + d2y);
			//set the distance Matrix equal on both nodes
			distM[i*n+j] = d;
			distM[j*n+i] = d;		
		}
	}
	
	//List of visited nodes
	int visited = 0;
	int* inTree = (int*) calloc(n, sizeof(int));
	
	//number of branches
	int branches = 0;
	//know if a node has branched
	int* branched = (int*) calloc(n, sizeof(int));
	//min Distances of nodes
	float* minDist = (float*) calloc(n, sizeof(float));
	
	//All arcs to check
	struct arc *queuedArcs[301*301];
	int queuedArcsCount = 0;
	//Actually selected Arcs
	struct arc *mspArcs[301];
	int mspArcsCount = 0;
	//Check if heap needs ordering
	int order = 0;
	
	//Start path from school
	inTree[0]=1;
	visited = 1;
	//Visit all nodes
	while(visited < n)
	{
		if(queuedArcsCount > 0)
		{
			if(order == 1)
			{
				quickSort(queuedArcs,0,queuedArcsCount - 1);
				order = 0;
			}
			struct arc* poppedArc = queuedArcs[queuedArcsCount-1];
			queuedArcsCount--;
			int visitedNode = poppedArc->b;
			//if the poppedArc does not branch an already branched node
			if(inTree[visitedNode] == 0 && !(branched[poppedArc->a] == 1 && poppedArc->branch == 1))
			{
				//Visit node
				inTree[visitedNode] = 1;
				visited++;
				printf("\b\b\b%d", visited);
				//Visit arc 
				mspArcs[mspArcsCount] = poppedArc;
				mspArcsCount++;
				
				/*printf("Visited Node : %d \n", poppedArc->b);
				printf("Count : %d \n", visited);*/
				if(poppedArc->branch == 0)
				{
					branches++;
					printf("\nLeafs : %d \n", branches);
				}
				else
				{
					branched[poppedArc->a] = 1;
				}
				
				minDist[visitedNode] = poppedArc->dist;
				//printf("Visited Node %d Dist : %f ",visitedNode, poppedArc->dist);
				//printf("Max Node Dist : %f \n", distM[visitedNode]*alpha);
				
				//Add arcs of the visited node
				for(int i = 0; i < n; i++)
				{
					if(inTree[i] == 0)
					{
						//Queue new arc
						struct arc *newArc = (struct arc*) malloc(sizeof(struct arc));
						newArc->a = visitedNode;
						newArc->b = i;
						float nodeDistance = minDist[visitedNode]+ distM[visitedNode*n+i];
						//Do not exceed the max distance with the same branch
						if(nodeDistance > distM[i]*alpha)
						{
							newArc->branch = 0;	
						}
						else
						{
							newArc->branch = 1;
						}
						newArc->dist = nodeDistance;
						newArc->danger = d[visitedNode*n+i];
						queuedArcs[queuedArcsCount] = newArc;
						//quickSort(queuedArcs,0,queuedArcsCount);
						queuedArcsCount++;	
						order = 1;
					}
				}
			}
		}
		else
		{
			//Add arcs of visited nodes
			for(int i = 0; i < n; i++)
			{
				if(inTree[i] == 1)
				{
					for(int j = 0; j < n; j++)
					{
						if(inTree[j] == 0)
						{
							struct arc *newArc = (struct arc*) malloc(sizeof(struct arc));
							newArc->a = i;
							newArc->b = j;
							//All branches are new
							newArc->branch = 0;
							float nodeDistance = minDist[i]+distM[i*n+j];
							newArc->dist = nodeDistance;
							newArc->danger = d[i*n+j];
							queuedArcs[queuedArcsCount] = newArc;
							//quickSort(queuedArcs,0,queuedArcsCount);
							queuedArcsCount++;
							order = 1;
						}						
					}
				}
			}
		}
	}
	
	/* Now we are going to create a list with the resultd found. Then, by creating partitions we are going to run un modified version of the above algorithm that should stop every time the tree gets bigger than the minimum number of branches. We only save the viale solutions, and if the mnimimum danger drops, we sabe the new minimum 
	*/


	//so we know when we found lesser danger path 
	float min_danger = calculate_danger(mspArcs, mspArcsCount);

	// declare our list of partitions  
	struct arcs_list *head;
	head = (struct arcs_list *) malloc(sizeof(struct arcs_list));

	int i;
	// fill with the MSP found 
	for(i=0; i<mspArcsCount; i++){
		head->partition_arcs[i]->a = mspArcs[i]->a;
		head->partition_arcs[i]->b = mspArcs[i]->b;
	}
	head->arc_count = mspArcsCount;
	head->included = 0;
	head->next = NULL;
	
	int max_branches = branches;

	// when the list becomes empty, we did all the possible checkings 
	while(head != NULL){

		// we save the current MSP with least danger 
		if(head->danger < min_danger){
			for(i=0; i < head->arc_count; i++){
				head->partition_arcs[i]->a = mspArcs[i]->a;
				head->partition_arcs[i]->b = mspArcs[i]->b;
			}
			min_danger = head->danger;
		}
		
		partition(head, max_branches);	
	}

	//Write Solution
	FILE *output_file = fopen(output_file_name, "w+");
	printf("\nSolution file: %s", output_file_name);
	for(int i = 0; i < mspArcsCount; i++)
	{
		fprintf(output_file, "%d %d \n", mspArcs[i]->b, mspArcs[i]->a);
	}
	fclose(output_file);
	
	//Calculate time
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	int hr = time_spent/3600;
	float t = time_spent - hr*3600;
	int min = t/60;
	float sec = t - min*60;
	printf("\nExec Time: %d hrs %d mins %f seconds", hr, min, sec);
	
	//Deallocate memory
	free(output_file_name);
	//free(coord); 
	free(distM); 
	free(d);
	free(head);
	return 0;
}

// we are goint to recalculate the MST using the partition, if the number of branches exceeds, we give up the solution, Return null when over

void partition(struct arcs_list *current, int max_branches){

	int i;
	struct arcs_list *p1;
	float excluded;
	

	//p1 is going to be our testing node, needs to be equal to the list element	
	p1 = (struct arcs_list *) malloc(sizeof(struct arcs_list));
/*
	for(i=0; i<current->arc_count; i++){
		p1->partition_arcs[i]->a = current->partition_arcs[i].a;
		p1->partition_arcs[i]->b = current->partition_arcs[i].b;
	}
	
	p1->branch = current->branch;
	// this attibute shows what cannot be changed in this representation 
	p1->included = current->included;
	p1->next = NULL;
	*/

	for(i=0; i<current->included; i++){
			p1->partition_arcs[i]->a = current->partition_arcs[i]->a;
			p1->partition_arcs[i]->b = current->partition_arcs[i]->b;
		}
	p1->arc_count = current->included;
	p1->included = current->included;
	p1->next = NULL;
	

	for(;current->included < current->arc_count; current->included++){
		
		// add one arc to partition	
		p1->partition_arcs[current->included]->a = current->partition_arcs[current->included]->a;
		p1->partition_arcs[current->included]->b = current->partition_arcs[current->included]->b;
		p1->arc_count++;
	
		//exclude arc by assigning infinite distance to it	
		excluded = p1->partition_arcs[current->included]->dist;
		p1->partition_arcs[current->included]->dist = FLT_MAX;
		p1->included++;

		// alt_mst is going to return true if found solution
		if(alt_mst(p1, max_branches, n)){
			// if this solution is connecting all paths 
			if(connected(p1)){
				//calculate the danger
				p1->danger = calculate_danger(p1->partition_arcs, p1->arc_count);	
				
				// and put it on our list 
				struct arcs_list *p2;
				p2 = (struct arcs_list *) malloc(sizeof(struct arcs_list));

				for(i=0; i<current->arc_count; i++){
				p2->partition_arcs[i]->a = p1->partition_arcs[i]->a;
				p2->partition_arcs[i]->b = p1->partition_arcs[i]->b;
				}
	
				p2->arc_count = p1->arc_count;
				// this attibute shows what cannot be changed in this representation 
				p2->included = p1->included;
				p2->danger = p1->danger;
	
				p2->next = current->next;
				current->next = p2;
			}	
		}

		// include the arc that was excluded
		p1->partition_arcs[current->included]->dist = excluded;
	} 	

	// pop used from the list 
	struct arcs_list *new_head = current->next;
	free(current);
	current = new_head;
}

/* adaptation of Prim's Algorithm in order to continue on an already choosen arcs
	it's stops as the current spanning tree passes the number of max_branches or the total danger is higher. 
*/

int alt_mst(struct arcs_list partition, int max_branches, int n, double distM){
	int i;

	//List of visited nodes
	int visited = 0;
	int* inTree = (int*) calloc(n, sizeof(int));

	inTree[0] =1;
	visited++;

	double temporary[n];

	// fill the tree with the included arcs 
	for(i=0; i < partition->arc_count; i++) {
		if(partition->partition_arcs[i]->dist != FLT_MAX){  
			inTree[partition->partition_arcs[i]->b] = 1;
			visited++;
		}
		else{
			// save the previous valeìue
			temporary[i] = distM[(partition->partition_arcs[i]->a)*n+(partition->partition_arcs[i]->b)]
			// temporarily take this arc out of the equation! (if implementing thread, take care here!)
			distM[(partition->partition_arcs[i]->a)*n+(partition->partition_arcs[i]->b)] = DBL_MAX;
		} 
	}	


	//number of branches
	// need to calculate this!!! 
	int branches = 0;

	//know if a node has branched
	// we need to fill this!!!!!!!
	int* branched = (int*) calloc(n, sizeof(int)); // 2 ITEMS FROM THIS NODE 
	
	//min Distances of nodes
	float* minDist = (float*) calloc(n, sizeof(float));
	
	//All arcs to check
	struct arc *queuedArcs[301*301];
	int queuedArcsCount = 0;
	/*

	//Actually selected Arcs
	struct arc *mspArcs[301];
	int mspArcsCount = 0;
	
	*/
	
	//Check if heap needs ordering
	int order = 1;
	
	// visited all nodes 
	while(visited < n)
	{
		if(queuedArcsCount > 0)
		{
			if(order == 1)
			{
				quickSort(queuedArcs,0,queuedArcsCount - 1);
				order = 0;
			}
			struct arc* poppedArc = queuedArcs[queuedArcsCount-1];
			queuedArcsCount--;
			int visitedNode = poppedArc->b;
			//if the poppedArc does not branch an already branched node
			if(inTree[visitedNode] == 0 && !(branched[poppedArc->a] == 1 && poppedArc->branch == 1))
			{
				//Visit node
				inTree[visitedNode] = 1;
				visited++;
				printf("\b\b\b%d", visited);
				//Visit arc 
				partition->partition_arcs[partition->arc_count] = poppedArc;
				partition->arc_count++;
				
				/*printf("Visited Node : %d \n", poppedArc->b);
				printf("Count : %d \n", visited);*/
				if(poppedArc->branch == 0)
				{
					branches++;
					printf("\nLeafs : %d \n", branches);
				}
				else
				{
					branched[poppedArc->a] = 1;
				}
				
				minDist[visitedNode] = poppedArc->dist;
				//printf("Visited Node %d Dist : %f ",visitedNode, poppedArc->dist);
				//printf("Max Node Dist : %f \n", distM[visitedNode]*alpha);
				
				//Add arcs of the visited node
				for(int i = 0; i < n; i++)
				{
					if((inTree[i] == 0) && (distM[visitedNode*n+i] < DBL_MAX) && no_cycle(partition->partition_arcs, j,n))
					{
						//Queue new arc
						struct arc *newArc = (struct arc*) malloc(sizeof(struct arc));
						newArc->a = visitedNode;
						newArc->b = i;
						float nodeDistance = minDist[visitedNode]+ distM[visitedNode*n+i];
						//Do not exceed the max distance with the same branch
						if(nodeDistance > distM[i]*alpha)
						{
							newArc->branch = 0;	
						}
						else
						{
							newArc->branch = 1;
						}
						newArc->dist = nodeDistance;
						newArc->danger = d[visitedNode*n+i];
						queuedArcs[queuedArcsCount] = newArc;
						//quickSort(queuedArcs,0,queuedArcsCount);
						queuedArcsCount++;	
						order = 1;
					}
				}
			}
		}
		else
		{
			//Add arcs of visited nodes
			for(int i = 0; i < n; i++)
			{
				if(inTree[i] == 1)
				{
					for(int j = 0; j < n; j++)
					{
						/* before putting on the queue we need to check if
							the it's not an exluded arc, and adding it won't make a cycle
						 */ 
						if((inTree[j] == 0) && (distM[i*n+j] < DBL_MAX) && no_cycle(partition->partition_arcs, j,n))
						{
							struct arc *newArc = (struct arc*) malloc(sizeof(struct arc));
							newArc->a = i;
							newArc->b = j;
							//All branches are new
							newArc->branch = 0;
							float nodeDistance = minDist[i]+distM[i*n+j];
							newArc->dist = nodeDistance;
							newArc->danger = d[i*n+j];
							queuedArcs[queuedArcsCount] = newArc;
							//quickSort(queuedArcs,0,queuedArcsCount);
							queuedArcsCount++;
							order = 1;
						}						
					}
				}
			}
		}
	}

	// return the original value of distM
	for(i=0; i < partition->included; i++) {
		if (temporary[i] != 0){
			// return the original value 
			distM[(partition->partition_arcs[i]->a)*n+(partition->partition_arcs[i]->b)] = temporary[i];
		} 
	}	

// add distM
// fill minDist
//d vector

	free(inTree);
	free(branched);
	free(minDist);
	
	// found a valid SPN
	return 1;
}

// not checked
// Returns true if the graph contains a cycle, else false.
int no_cycle(struct arc **graph, int j, int n){

	int visited[n] ;
    visited[0] = 1;
    for (int i = 1; i < n; i++)
    	visited[i] = 0;
 
	for(i=0; i< n; i++){
		if(!visited[graph[i]->b])
			visited[graph[i]->b] = 1;
		else
			return 0;
	}

	if(visited[j])
		return 0;
    
    return 1;
}