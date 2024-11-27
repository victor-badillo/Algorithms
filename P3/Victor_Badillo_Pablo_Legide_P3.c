/* Victor Badillo - victor.badillo@udc.es and Pablo Legide - p.legide@udc.es */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>


double microseconds() {// measures in microseconds
  
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void init_seed() {
  
    srand(time(NULL));
}

void random_init(int v [], int n) {// generates pseudo-random numbers between -n y +n
  
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

struct node {
   int elem;
   int num_repetitions;
   struct node *left, *right;
};
typedef struct node * position;
typedef struct node * tree;


static struct node *createnode(int e) {
   struct node *p = malloc(sizeof(struct node));
   if (p == NULL) {
      printf("out of memory\n"); exit(EXIT_FAILURE);
   }
   p->elem = e;
   p->num_repetitions = 1;
   p->left = NULL;
   p->right = NULL;
   return p;
}


tree insert(int e, tree a){
   if (a == NULL)
      return createnode(e);
   else if (e < a->elem)
      a->left = insert(e, a->left);
   else if (e > a->elem)
      a->right = insert(e, a->right);
   else
      a->num_repetitions++;
   return a;
}


tree createtree(){
   return NULL;
}


int isemptytree(tree a){
   if(a==NULL){
      return 1;
   }
   else return 0;
}


position find(int e, tree a){
   if(a==NULL){
      return NULL;
   }
   else if(e==a->elem){
      return a;
   }
   else if(e<a->elem){
      return find(e, a->left);
   }
   else return find(e,a->right);
}


tree deletetree(tree a){
   if(isemptytree(a)){
      return a;
   }
   else{
      deletetree(a->left);
      deletetree(a->right);
      free(a);
      a=NULL;
      return a;
   }
}


position leftchild(tree a){
   return a->left;
}


position rightchild(tree a){
   return a->right;
}


int element(position b){
   return b->elem;
}


int numberofrepetitions(position b){
   return b->num_repetitions;
}


int heigth(tree a){
   if(isemptytree(a)){
      return -1;
   }
   else{
      if(heigth(leftchild(a))<heigth(rightchild(a))) return 1+heigth(rightchild(a));
      else return 1+heigth(leftchild(a));
   }
}


void visualize(tree a){
   if (isemptytree(a)){
        printf("empty tree: ()\n");
   }
   else{
      if(!isemptytree(leftchild(a))){
         printf("(");
         visualize(leftchild(a));
         printf(")");
      }
        printf(" %d ", element(a));
      if(!isemptytree(rightchild(a))){
         printf("(");
         visualize(rightchild(a));
         printf(")");
      }
   }
}

void printFind(int n,position a){
    if(a == NULL){
        printf("searching for %d and finding nothing\n",n);
    }else{
        printf("searching for %d and finding %d repeated: %d times\n", n,n,numberofrepetitions(find(n,a)));
    }
}



void test(){
   tree a = createtree();
   visualize(a);
   heigth(a);
   printf("tree height: %d\n", heigth(a));
   printf("inserting a 3\n");
   a = insert(3,a);
   printf("inserting a 1\n");
   a = insert(1,a);
   printf("inserting a 2\n");
   a = insert(2,a);
   printf("inserting a 5\n");
   a = insert(5,a);
   printf("inserting a 4\n");
   a = insert(4,a);
   printf("inserting a 5\n");
   a = insert(5,a);
   visualize(a);
    printf("\n");
   printf("tree height: %d\n", heigth(a));
    printFind(1,find(1,a));
    printFind(2,find(2,a));
    printFind(3,find(3,a));
    printFind(4,find(4,a));
    printFind(5,find(5,a));
    printFind(6,find(6,a));
    printf("deleting all nodes, freeing memory: \n");
    a = deletetree(a);
    visualize(a);
    printf("tree height: %d\n", heigth(a));
}


void insertionTimes(){

   double t1, t2, t_ins, x1,x2,x3;
   
   printf("Insertion of n elements\n");
   printf("\t  n\t      t(n)\t  t(n)/f(n)\t  t(n)/g(n)\t t(n)/h(n)\n");
   printf("______________________________________");
   printf("_____________________________________\n");


   for(int n=8000; n<=256000; n*=2){ //We start at 8000 to avoid times less than 500 microseconds

      tree b = createtree();
      int v1[n];
      random_init(v1,n);

      t1=microseconds();
      for(int i=0;i<n;i++){
         b = insert(v1[i],b);
      }

      t2=microseconds();
      t_ins=t2-t1;

      x1=t_ins/pow(n,1.05);
      x2=t_ins/pow(n,1.19); 
      x3=t_ins/pow(n,1.65);

      printf("%12d%15.3f%16.6f%16.6f%15.7f\n", n, t_ins, x1, x2, x3);
      deletetree(b);
   }
}


void searchTimes(){
   double t3,t4,t_sea,y1,y2,y3;

   printf("\nSearch of n elements\n");
   printf("\t  n\t      t(n)\t  t(n)/f(n)\t  t(n)/g(n)\t t(n)/h(n)\n");
   printf("______________________________________");
   printf("_____________________________________\n");
   
   
   for(int n=8000; n<=256000; n*=2){ //We start at 8000 to avoid times less than 500 microseconds
      int v2[n], v3[n];
      tree c = createtree();
      random_init(v2,n);
      random_init(v3,n);

      for(int i=0;i<n;i++){
         c = insert(v2[i],c);
      }
     
      t3=microseconds();
      for(int j=0;j<n;j++){
         find(v3[j],c);
      }
      t4=microseconds();
      t_sea=t4-t3;

      y1=t_sea/pow(n,1.05);
      y2=t_sea/pow(n,1.28); 
      y3=t_sea/pow(n,1.65);

      printf("%12d%15.3f%16.6f%16.6f%15.7f\n", n, t_sea, y1, y2, y3);
      deletetree(c);
   }
}
   

int main(){
   init_seed();
   test();
   insertionTimes();
   searchTimes();
}
