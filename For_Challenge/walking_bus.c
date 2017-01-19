//  Walking bus challenge:
//  given a  school, a set of bus stops, and a constraint alpha that specifies the max dist that a can be walked from that stop, the goal is to minimize the number of lines that covers all the stops. 
// when there are multiple options, we will always choose the one with the least risk. 

#include <stdio.h> 
#include <limits.h>
#include <stdlib.h>

// after the node is added to the tree, it checks if any node got closer to the tree, respecting the maximum walking distance.
void updateDistances(int target, int *partial, int n, int **weight, int *d, int alpha, int *whoTo) {

	int i;

	for (i = 0; i < n; ++i)

		if ((weight[target][i] != 0) && (d[i] > weight[target][i])) {
			// check if the max walk constraint is fulfilled
			if(partial[target] + weight[target][i] <= alpha * weight[0][i]) {

				d[i] = weight[target][i];
				whoTo[i] = target;
			
			}
		}
}

int main(int argc, char *argv[]) {

	int i, j;
	int n; // number of nodes in graph
	float alpha; // max walk factor 
	int *partial; // distance from the school to this point USING the bus line
	int **weight; // weight[i][j] is the distance between node i and node j;
	int *d; // distance from node i to the current spanning tree; its an optimization factor
	int *inTree; // inTree[i] is 1 if node i is in the tree, 0 otherwhise
	int *whoTo; // whoTo[i] saves the element to link node i to get the minimum distance to the tree 


	FILE *f = fopen(argv[1], "r");

//	FILE *f = fopen("teste.dat", "r");

	fscanf(f, "%d", &n);

	// DYNAMIC ALLOCATION OF VARS
	partial = (int *) malloc(n* sizeof(int)); 
	partial[0] = 0;
  	
  	d = (int *) malloc(n* sizeof(int)); 
  	inTree = (int *) malloc(n* sizeof(int));
  	whoTo = (int *) malloc(n* sizeof(int));

  	weight = (int **) malloc (n*sizeof(int*));
  	for(i = 0; i < n; i++){
  		weight[i] = (int *) malloc (n* sizeof (int));
  	}
  	// END OF DYNAMIC ALLOCATION 

	fscanf(f, "%f", &alpha);

	// weight won't be given ready, needs to be calculate by given points!
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)

			fscanf(f, "%d", &weight[i][j]);

	fclose(f);

	/* Initialise d with infinity */
	for (i = 0; i < n; ++i)
		d[i] = INT_MAX;

	/* Mark all nodes as NOT beeing in the minimum spanning tree */
	for (i = 0; i < n; ++i)
		inTree[i] = 0;

	/* Add the school as starting point */
	printf("Adding node %c\n", 0 + 'A');
	inTree[0] = 1;

	updateDistances(0, partial, n, weight, d, alpha, whoTo);
	
	int treeSize;

	//last node added 		
	int last = 0;

	for (treeSize = 1; treeSize < n; ++treeSize) {

		/* Find the node with the smallest distance to the same bus line */
		int min = -1;

		for (i = 0; i < n; ++i){
		
			// if node isn't already on the graph
			if(!inTree[i]){

				// choose closest
				if((min == -1) || (weight[last][i] < weight[last][min]))
					min = i;
			}
		}

		//check for the alpha constraint 
		if (partial[last] + weight[last][min] > alpha * weight[0][min]){
			
//			printf("PARTIAL: %d WEIGHT: %d\n", partial[last]+weight[last][min], alpha*weight[0][min] );
			
			min = -1;
			// if it wasn't satisfied, open a new bus on any point of the three
			for (i = 0; i < n; ++i)	
				if (!inTree[i])
						if ((min == -1) || (d[min] > d[i]))
							min = i; 
		
			partial[min] = partial[whoTo[min]] + d[min];
			printf("Adding edge %c-%c\n", whoTo[min] + 'A', min + 'A');
		
		} else{
			partial[min] = partial[last] + weight[last][min];	
			printf("Adding edge %c-%c\n", last + 'A', min + 'A');	
		}
	
		/* And add it */
		inTree[min] = 1;
		last = min; 
		// may be move it from here 
		updateDistances(min, partial, n, weight, d, alpha, whoTo);

	}

	printf("END\n" );

	free(partial);

  	for(i = 0; i < n; i++){
  		free(weight[i]);
  	}
  	free(weight);
  	free(d);
  	free(inTree);
  	free(whoTo);

	return 0;

}