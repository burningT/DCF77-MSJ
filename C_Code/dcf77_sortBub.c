/* sortBub.c
BubbleSort Sortieralgorithmus. Die Elemente werden aufsteigend sortiert.
Parameter:
float arr[] : Array mit den Zahlen die sortiert werden sollen.
short n : Anzahl der Elemente im Array.
*/

#include "dcf77_includes.h"

void dcf77_sortBub(float arr[], short n){
	short i,j;
	float tmp;
	for(i=n-1;i>0 ;i--){
		for(j=0;j<i ;j++){				
			if(arr[j] > arr[j+1]){
				tmp= arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=tmp;
			}
		}
	}
}
