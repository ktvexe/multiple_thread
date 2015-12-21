#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
//parameter thread used
struct parameter{
	int* arrrow,*arrcol;
	int size;
};

//function thread used
void* calculate(void *ptr);

int main(int argc,char** argv){
	pthread_t *threadx;
	struct parameter *threadpara;
	FILE* fptr, *fptr_w;
	int	row1,row2,col1,col2;
	int **calrow,**calcol;
	int* reta;
	//input file read
	if(argc < 2){
		perror("Please input file name!");
		return -1;
	}

	fptr=fopen(argv[1],"r");
	if(!fptr){
		perror("Open failure");
		return -1;
	}
	fscanf(fptr,"%d",&row1);
	fscanf(fptr,"%d",&col1);
	printf("row1:%d  col1:%d\n",row1,col1);
	calrow =(int**)malloc(row1*sizeof(int*));
	for(int i =0;i<row1;i++){
		calrow[i]=(int*)malloc(col1*sizeof(int));
	}
	for(int i =0 ;i<row1;i++){
		for(int j=0;j<col1;j++){
			fscanf(fptr,"%d",&calrow[i][j]);
		}
	}

	fscanf(fptr,"%d",&row2);
	fscanf(fptr,"%d",&col2);
	printf("row2:%d  col2:%d\n",row2,col2);
	calcol =(int**)malloc(col2*sizeof(int*));
	for(int i =0;i<col2;i++){
		calcol[i]=(int*)malloc(row2*sizeof(int));
	}
	for(int i =0 ;i<row2;i++){
		for(int j=0;j<col2;j++){
			fscanf(fptr,"%d",&calcol[j][i]);
		}
	}
	
	threadpara =(struct parameter*)malloc(row1*col2*sizeof(struct parameter));
	for(int i =0;i<row1*col2;i++){
		threadpara[i].arrrow =(int*)malloc(col1*sizeof(int));
		threadpara[i].arrcol =(int*)malloc(row2*sizeof(int));
	}
	
	for(int i =0;i<row1;i++){
		for(int j =0;j<col2;j++){
			threadpara[i*col2+j].arrrow=calrow[i];
			threadpara[i*col2+j].arrcol=calcol[j];
			threadpara[i*col2+j].size=col1;
		}
	}

	for(int i =0 ;i<row1;i++){
		for(int j=0;j<col1;j++){
			printf("%d ",threadpara[i*col2].arrrow[j]);
		}
		printf("\n");
	}
	
	for(int i =0 ;i<row2;i++){
		for(int j=0;j<col2;j++){
			printf("%d ",threadpara[j].arrcol[i]);
		}
		printf("\n");
	}
	

	threadx=(pthread_t*)malloc(row1*col2*sizeof(pthread_t));
	reta=(int*)malloc(row1*col2*sizeof(int));
	
	for(int i=0;i<row1*col2;i++){
		pthread_create(&threadx[i],NULL,calculate,&threadpara[i]);
		pthread_join(threadx[i],(void*)&reta[i]);
	}
	
	fptr_w=fopen("output","w");
	fprintf(fptr_w,"%d %d \n",row1,col2);		
	
	for(int i = 0 ;i<row1;i++){
		for(int j =0;j<col2;j++){
			fprintf(fptr_w,"%d ",reta[i*(row1-1)+j]);		
		}
		fprintf(fptr_w,"\n");
	}
	fclose(fptr_w);
	fclose(fptr);
}


void* calculate(void *ptr){
	int result = 0,size;
	struct parameter *threadpara=(struct parameter*)ptr;
	size =threadpara ->size;
	printf("size:%d\n",size);
	for(int i=0;i<size;i++ ){
		result+=threadpara->arrrow[i]*threadpara->arrcol[i];
		
	}
	usleep(10000);
	pthread_exit((void*)(intptr_t)result);
	//pthread_exit(result);

}
