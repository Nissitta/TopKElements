#include <stdio.h>
#include <stdlib.h>
#include "TopKheader.h"

int main(){
    
    int no_words = 16000;   //Approximate no. of words in dataset

    char** Arr = (char**)calloc(no_words,sizeof(char*)); 
    int count_of_elements = data_from_file("Countries.txt",no_words,Arr);

    int k = 5 ; //Required no. of elements to monitor

    TopKHitter(Arr,k, count_of_elements);
    
}