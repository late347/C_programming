#include <stdio.h>
//lauri solin 1606308

int main (void){
int n=0;
int j=0, k=0, i=0;
int swap=0;	// swap is used as a boolean either 1 or zero
int temp=0;
int input=0;
int integer=0;

printf("give ten integers\n" ); // asks how many elements in array
	//scanf(" %d", &input);

	int arrayA[10];
	int arrayB[10];
	k=0;
	while(k<10){
		scanf("%d",&input);
		arrayA[k]=input;
		arrayB[k]=input;
		k++;
	}

	j=0;

	for (i=0; i<10-1;i++){
		swap=0;
		for(j=0; j<10-1-i; j++){
			if(arrayA[j]>arrayA[j+1]){
			temp=arrayA[j];	
			arrayA[j]=arrayA[j+1];
			arrayA[j+1]=temp;
			swap=1;
			}
			else{

			}
		}
		// jos eka kierros meni läpi ilman swappia,
	// =>implikoi että lista oli oikeassa järjestyksessä valmiiksi
		if(swap==0){
		break;
		}
	


	}

j=0;
printf("original array was \n");
for(j=0;j<10;j++){
	printf("%d ",arrayB[j] );
}
j=0;
printf("\n");
printf("sorted array was \n");
for(j=0;j<10;j++){
	printf("%d ",arrayA[j] );
}
}





