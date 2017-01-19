#include <stdio.h>
#include <limits.h>
#include <math.h>

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
	if(a1->branch > a2->branch)
	{
		return 1;
	}
	else if(a1->branch < a2->branch)
	{
		return 0;
	}	
	if(a1->dist < a2->dist)
	{
		return 1;
	}
	else if(a1->dist > a2->dist)
	{
		return 0;
	}
	
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

int main(int argc, char *arginTree[])
{
	char* input_file_name = arginTree[1];
	int input_file_name_size = strlen(input_file_name) - 1; 
	//Overwrite .sol file
	char* output_file_name = (char *) malloc(input_file_name_size);
	strncpy(output_file_name, input_file_name, input_file_name_size);
	strncpy(&output_file_name[input_file_name_size - 3], ".sol\0", 5);

	//Parameters
	int n = 0; //Number of Nodes
	float alpha = 0.0; //Percentage Limit of shortest path
	struct coordinate coord[300]; //Coordinates of the nodes
	float* d; //Dangerousness of paths
	float* distM; //Distances between the nodes
	
	
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
	
	//Allocate the coordinate matrix
	//coord = (struct coordinate*) malloc(n+10, sizeof(struct coordinate));
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
		//printf("%d \n",coord[index].y);
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
	distM = (float*) calloc(n*n, sizeof(float));
	for(int i = 0; i < n; i++)
	{
		for(int j = i + 1; j < n; j++)
		{
			int distx = abs(coord[i].x - coord[j].x);
			int disty = abs(coord[i].y - coord[j].y);
			float d2 = sqrt(distx*distx + disty*disty); 
			//set the distance Matrix equal on both nodes
			distM[i*n+j] = d2;
			distM[j*n+i] = d2;
			//printf("%d ",dist2[i*n+j]);
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
	float* mindist = (float*) calloc(n, sizeof(float));
	
	//All arcs to check
	struct arc *queuedArcs[300*300];
	int queuedArcsCount = 0;
	//Actually selected Arcs
	struct arc *mspArcs[300*300];
	int mspArcsCount = 0;
	
	//Start path from school
	inTree[0]=1;
	visited = 1;
	//Visit all nodes
	while(visited < n)
	{
		if(queuedArcsCount > 0)
		{
			struct arc* poppedArc = queuedArcs[queuedArcsCount-1];
			queuedArcsCount--;
			int visitedNode = poppedArc->b;
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
					printf("\n Branches : %d \n", branches);
				}
				else
				{
					branched[poppedArc->a] = 1;
				}
				
				mindist[visitedNode] = poppedArc->dist;
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
						float nodeDistance = mindist[visitedNode]+ distM[visitedNode*n+i];
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
						quickSort(queuedArcs,0,queuedArcsCount);
						queuedArcsCount++;					
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
							float nodeDistance = mindist[i]+distM[i*n+j];
							newArc->dist = nodeDistance;
							newArc->danger = d[i*n+j];
							queuedArcs[queuedArcsCount] = newArc;
							quickSort(queuedArcs,0,queuedArcsCount);
							queuedArcsCount++;
						}						
					}
				}
			}
		}
	}
	
	
	//Write Solution
	FILE *output_file = fopen(output_file_name, "w+");
	printf("\n Solution file: %s", output_file_name);
	for(int i = 0; i < mspArcsCount; i++)
	{
		fprintf(output_file, "%d %d \n", mspArcs[i]->b, mspArcs[i]->a);
	}
	
	fclose(output_file);
	//Deallocate memory
	free(output_file_name);
	//free(coord); 
	free(distM); 
	free(d);
	return 0;
}