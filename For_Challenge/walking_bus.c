//  Walking bus challenge:
//  given a  school, a set of bus stops, and a constraint alpha that specifies the max dist that a can be walked from that stop, the goal is to minimize the number of lines that covers all the stops. 
// when there are multiple options, we will always choose the one with the least risk. 

#include <stdio.h> 
#include <float.h>
#include <stdlib.h>
#include <math.h>

// after the node is added to the tree, it checks if any node got closer to the tree, respecting the maximum walking distance.
void updateDistances(int target, float *partial, int n, float **weight, float *d, int alpha, int *whoTo) {

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

// fill weight array with the relative distances 
void calc_weight(int *x_axis,int *y_axis,float **weight,int n){
	int i, j;

	// EXPENSIVE FUNCTION, NEEDS TO BE OPTIMIZED 
	for(i=0; i < n; i++)
		for(j=0; j < n; j++)
			weight[i][j] = sqrtf((float)((x_axis[i]-x_axis[j])*(x_axis[i]-x_axis[j])+(y_axis[i]-y_axis[j])*(y_axis[i]-y_axis[j])));
}

int main(int argc, char *argv[]) {

	int i;
	int n; // number of nodes in graph
	float alpha; // max walk factor 

	int *x_axis;
	int *y_axis; 

	float *partial; // distance from the school to this point USING the bus line
	float **weight; // weight[i][j] is the distance between node i and node j;
	float *d; // distance from node i to the current spanning tree; its an optimization factor
	int *inTree; // inTree[i] is 1 if node i is in the tree, 0 otherwhise
	int *whoTo; // whoTo[i] saves the element to link node i to get the minimum distance to the tree 

//	FILE *f = fopen(argv[1], "r");

	FILE *f = fopen("teste2.dat", "r");

	// ignore format strings 
	fscanf(f, "%*s %*s %*s %*s %d %*s", &n);
	
	n += 1; // the school plus n points

	// DYNAMIC ALLOCATION OF VARS

	partial = (float *) malloc(n* sizeof(float)); 
	partial[0] = 0;
  	
  	d = (float *) malloc(n* sizeof(float)); 
  	inTree = (int *) malloc(n* sizeof(int));
  	whoTo = (int *) malloc(n* sizeof(int));

  	x_axis = (int *) malloc(n* sizeof(int));
  	y_axis = (int *) malloc(n* sizeof(int));

  	weight = (float **) malloc (n*sizeof(float*));
  	for(i = 0; i < n; i++){
  		weight[i] = (float *) malloc (n* sizeof (float));
  	}
  	// END OF DYNAMIC ALLOCATION 

  	fscanf(f, "%*s %*s %*s %f %*s", &alpha);

	fscanf(f, "%*s %*s %*s %*s");
	for (i = 0; i < n; ++i)
		fscanf(f, "%*s %d", &x_axis[i]);

	fscanf(f, "%*s %*s %*s %*s %*s");
	for (i = 0; i < n; ++i)
		fscanf(f, "%*s %d", &y_axis[i]);
	fscanf(f, "%*s");

	fclose(f);

	// calculate the distance between the given points and fill the weight vector 
	calc_weight(x_axis, y_axis, weight,n);

	/* Initialise d with infinity */
	for (i = 0; i < n; ++i)
		d[i] = FLT_MAX;

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
		if (((float) partial[last] + weight[last][min]) > alpha * weight[0][min]){
			
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

/*  still missing:
	read input in the specified format
	save graph as a hash
	calculate distance between points in the cartesian axis and use this distance as weight
	when in face of two equal distance, choose the safest -> this can be tricky (choose the next only or do compute multiple options?)
	
*/