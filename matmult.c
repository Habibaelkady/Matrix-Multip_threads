#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>

#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]
typedef struct element{
int *a;//matrix A
int *b;//matrix B
int *c;//matrix C
int ri; //row index 
int ci; //column index
int size;//colA == rowB
//int ca; //row a
///int cb; //row b
int cc; //columb b
} t;
 
/*i j
a[3*2] b[2*2] item (a,i,j,2) a[0+2*2]
[1 3
 4 1
 5 5]
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int* A, int* B, int* C, int l, int m, int n) //already implemented
{
    
    for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
                //A[k+i*2] B[j+k*2]
            Item(C, i, j, n) = sum;
        }
}

//                sum += Item(A, ri, k, m) * Item(B, k, j, n);
//                                          *B[1+1*2]

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 * [1 2 1] 2*3  3*2 [1 4]
 * [2 5 6]          [3 3]
 *                  [5 6]
 */
 void *element(void *args){
    t *x= (t*)(args); //casting back to struct type
    int size = x->size;
    int sum = 0;
    for (int i= 0 ; i< size;i++) { //looping on the number of rows of matrix B = number of columns of matrix A "inner dimension of multiplication" for calculation of a single element
        sum += Item(x->a, x->ri, i, x->size) * Item(x->b, i, x->ci, x->cc); //calculation of a single element in output matrix -- done by a single thread
    }
    Item(x->c, x->ri, x->ci, x->cc) = sum; //setting the value of the element in the matrix C according to the indices of the thread == indices of the element

}
void matmult_v1(int* A, int* B, int* C, int l, int m, int n) //assign thread for each element 
{
  //number of threads =  number of each element in output matrix =l*n in total
    pthread_t t1[l][n];
    t arr[l][n]; //array of struct  each struct is assigned to a thread
    for (int i=0;i<l ;i++ ){ 
        for (int j=0;j<n;j++){
            arr[i][j].ri=i; //assignig row index for the thread to know which element of the matrix the thread is calculating 
            arr[i][j].ci=j; //assigning column index for the thread to help in calculation
            arr[i][j].size=m; //initializing elements of the struct
            arr[i][j].a=A;
            arr[i][j].b=B;
            arr[i][j].c=C;
            arr[i][j].cc=n;
            
            //arr[][]
            pthread_create(&t1[i][j],NULL,element,&arr[i][j]); //creation of the thread
            
        }
        }
    for(int i=0;i<l;i++){
        for(int j=0;j<n;j++){
        pthread_join(t1[i][j],NULL); //joining for each thread so that the main process can't terminate unless all its threads have terminated
        }
    }

}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols 3*4
 *     B has m rows and n cols 4*5
 *     C has l rows and n cols 3*5
 * 
 */

void *row(void *args)
{
    t *y= (t*)(args); //casting back to struct type
    int size = y->size;//size= number of rows of B = number of columns of A=m
    for(int j=0 ;j<y->cc;j++)//looping on all columns to access all element of a single row
    {
    int sum = 0;
    for (int i= 0 ; i< size;i++) { //looping on the number of rows of matrix B = number of columns of matrix A "inner dimension of multiplication" for calculation of a single element
        sum += Item(y->a, y->ri, i, y->size) * Item(y->b, i, j, y->cc); //calculation of a single element in the output matrix 'row * column'
    }
        Item(y->c, y->ri, j, y->cc) = sum; // setting all the values of the row element by element
    }
}
void matmult_v2(int* A, int* B, int* C, int l, int m, int n)//assign thread for each row in output matrix
{
    pthread_t t1[l]; //array of threads with number of rows of output matrix
    t arr[l]; //array of struct with the number of threads  each arr[k] is assigned to thread k] 
    for (int i=0;i<l ;i++ ){ 
            arr[i].size=m;//initializing elements of struct
            arr[i].a=A;
            arr[i].b=B;
            arr[i].c=C;
            arr[i].ri=i;//each thread is assigned to a different row so each thread should have a row index to help in calculations of matrix
            arr[i].cc=n;
            pthread_create(&t1[i],NULL,row,&arr[i]);//creation of thread 
            
        
        }
     for (int i=0;i<l ;i++ ){ 
        pthread_join(t1[i],NULL);//joining for each thread so that the process can't terminate unless all its threads have terminated
     }
}
