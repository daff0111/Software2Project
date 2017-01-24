#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>

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
		fscanf (input_file, "%s", &stream); 
	}
	fscanf (input_file, "%d", &n); //read n
	n++; //Count the School;
	printf("n = %d \n",n);
	
	//; param alpha :=
	fscanf (input_file, "%s", &stream);
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", &stream); 
	}
	fscanf (input_file, "%f", &alpha); //read alpha
	printf("alpha = %f \n",alpha);
	
	
	//; param coordX [*] :=
	fscanf (input_file, "%s", &stream);
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", &stream); 
	}
	for(int i = 0; i < n; i++)
	{
		int index;
		fscanf (input_file, "%d", &index);
		fscanf (input_file, "%d", &coord[index].x);			
	}
	//; param coordY [*] :=
	fscanf (input_file, "%s", &stream);
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", &stream); 
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
	fscanf (input_file, "%s", &stream);
	while(strcmp(stream, ":=") != 0)
	{
		fscanf (input_file, "%s", &stream); 
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
	printf("Calculating MST... \n");
	//List of visited nodes
	int visited = 0;
	int* inTree = (int*) calloc(n, sizeof(int));
	
	//number of branches
	int branches = 0;
	//know if a node has branched
	int* branched = (int*) calloc(n, sizeof(int));
	//min Distances of nodes
	float* minDist = (float*) calloc(n, sizeof(float));
	
	//Arcs to check
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
			if(inTree[visitedNode] == 0 && !(branched[poppedArc->a] > 0 && poppedArc->branch == 1))
			{
				//Visit node
				inTree[visitedNode] = 1;
				visited++;
				//Print current number of Nodes
				printf("\b\b\b%d", visited);
				//Visit arc 
				mspArcs[mspArcsCount] = poppedArc;
				mspArcsCount++;

				if(poppedArc->branch == 0)
				{
					branches++;
					//printf("\nLeafs : %d \n", branches);
				}
				branched[poppedArc->a]++;
				
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
						newArc->danger = d[i*n+visitedNode];
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
							newArc->danger = d[j*n+i];
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
	
	//Keep the number of Max branches and the MST
	int maxBranches = branches;
	//Current dangerousness
	float minDangerousness = 0;
	for(int i = 0; i < mspArcsCount; i++)
	{
		struct arc* nextArc = mspArcs[i];
		minDangerousness = minDangerousness + d[nextArc->b*n+nextArc->a]; 
	}
	
	//Try to minimize the dangerousness
	printf("\nFirst Pass: Leafs: %d Danger: %f \n", maxBranches, minDangerousness);
	for(int i = 0; i < mspArcsCount; i++)
	{
		struct arc* selectedArc = mspArcs[i];
		float arcdanger = selectedArc->danger;
		int selectedNode = selectedArc->b;
		if(branched[selectedNode] == 0)
		{
			for(int j = 0; j < n; j++)
			{
				//Do the pass just for the end leafs - do not create new braches
				if(branched[j] == 0 && selectedNode != j)
				{
					//control just less dangerous paths
					if(d[selectedNode*n+j] < arcdanger)
					{
						float nodeDistance = minDist[j]+ distM[j*n+selectedNode];
						//Do not exceed the max distance
						if(nodeDistance <= distM[selectedNode]*alpha)
						{
							//A better arc was found, substitute selected arc
							branched[j]++;
							branched[selectedArc->a]--;
							arcdanger = d[selectedNode*n+j];
							//printf("Switch arc : %d %d for %d %d, Danger: %f %f \n", selectedArc->a, selectedNode, j, selectedNode, selectedArc->danger, arcdanger);
							selectedArc->a = j;
							selectedArc->danger = arcdanger;
							selectedArc->dist = nodeDistance;
						}
					}
				}
			}
		}
	}
	
	
	minDangerousness = 0;
	for(int i = 0; i < mspArcsCount; i++)
	{
		struct arc* nextArc = mspArcs[i];
		minDangerousness = minDangerousness + d[nextArc->b*n+nextArc->a]; 
	}
	
	printf("Second Pass: Leafs: %d Danger: %f \n\n", maxBranches, minDangerousness);
	float divideBy = 10;
	if(minDangerousness > 10)
	{
		divideBy = 100;
	}
	if(minDangerousness > 100)
	{
		divideBy = 1000;
	}
	float solutionValue = maxBranches + minDangerousness/divideBy;
	//printf("Solution Value: %f \n", solutionValue);
	
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
	return 0;
}