#include <stdio.h>





int add(int a,int b){
    return a+b;
}



int main(){
    printf("hello\n");
    int a = 0;
    int b = 9;
    int sum = add(a,b);
    printf("sum : %d\n",sum);
}