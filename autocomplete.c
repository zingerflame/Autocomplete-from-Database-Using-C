#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"
// typedef struct term{
//     char term[200]; // assume terms are not longer than 200
//     double weight;
// } term;

int compare_terms(const void* p_a, const void* p_b){
    struct term* p_a_s = (struct term*)p_a;
    struct term* p_b_s = (struct term*)p_b;

    return strcmp(p_a_s->term, p_b_s->term);
}

//stuff for getting digits
int is_digit(char c){
    return c >= '0' && c <= '9'; // digit 
}

const char* get_numeric_str(const char* str){
    while(!is_digit(*str)){
        str++;
    }
    return str;
}

void read_in_terms(term** terms, int* pnterms, char* filename){
    
    //count number of terms (says at top of text file)
    FILE* fp = fopen(filename, "r");
    char size[200];
    fgets(size, 200, fp);
    *pnterms = atoi(size);
    size[strlen(size) - 1] = '\0';

    *terms = (term*)malloc((*pnterms)*sizeof(term));
    // no need to malloc term.term as it is initialized as an array

    int i = 0;
    //store every line's text part in (*terms+i)'s value's term (see lec 7 for reference)
    char line[200];

    while(fgets(line, 200, fp)){

      // allocate 
        char* words = (char*)malloc(2*(sizeof(line)));
        char* token;
        token = strtok(line, "\t"); //split by tab
        int j = 0;
        while(token != NULL && j < 2){
            strcpy((words+200*j), token);
            token = strtok(NULL, "\t"); // use prev string
            j++;
        }
        // take last element, copy TERM of term into the char array
        (words+200)[strcspn((words+200), "\n")] = 0;
        strcpy((*terms+i)->term, (words+200)); 

        free(words);
        free(token);

        //copy number
        const char* num_str = get_numeric_str(line);
        //converts that number part of string into int and copies it into weight
        (*terms+i)->weight = atof(num_str);

        i++; //updates term (struct) index
    }
    fclose(fp);
    
    // sort the terms
    qsort(*terms , *pnterms, sizeof(term), compare_terms);
    
    
}

int lowest_match(term* terms, int nterms, char* substr){
  int low = 0;
  int high = nterms-1;

  while (low <= high){
    int mid = (high+low)/2;
    // compare 
    //*sizeof(term)
    if (strncmp((terms[mid]).term, substr, strlen(substr)) < 0){
      low = mid+1;
    }
    else{
      high = mid-1;
    }
  }
  return low;
}

int highest_match(term* terms, int nterms, char* substr){
  int low = 0;
  int high = nterms-1;

  while (low <= high){
    int mid = (high+low)/2;
    // compare 
    //*sizeof(term)
    if (strncmp((terms[mid]).term, substr, strlen(substr)) > 0){ //need strncmp otherwise wont work since how strcmp weight works
      high = mid-1;
    }
    else{
      low = mid+1;
    }
  }
  return high;
}

int compare_nums_backwards(const void* p_a, const void* p_b){

    struct term* p_a_s = (struct term*)p_a;
    struct term* p_b_s = (struct term*)p_b;

    double diff = (*p_b_s).weight - (*p_a_s).weight;
    if (diff > 0){
      return 1;
    }
    else{
      return -1;
    }
}

void autocomplete(term** answer, int* n_answer, term* terms, int nterms, char* substr){
  //find lowest and highest matches and copy them to answer+i or smt

  int low_index = lowest_match(terms, nterms, substr);
  int high_index = highest_match(terms, nterms, substr);
  // all matches should be inclusive of these
  *n_answer = high_index-low_index+1;
  *answer = (term*)malloc((*n_answer)*sizeof(term));

  for(int k = low_index; k < (high_index+1); k++){
    // copy all terms to answers
    //should copy the weight and term
    strcpy((*answer+(k-low_index))->term, (terms+k)->term);
    (*answer+(k-low_index))->weight = (terms+k)->weight;
  }

  // sort the terms
  qsort(*answer , *n_answer, sizeof(term), compare_nums_backwards);
}

// int main(){

//   term* term1;
//   int n = 0;
//   read_in_terms(&term1, &n, "wiktionary.txt");

//   // printf("%i\n", lowest_match(term1, n, "Tor"));
//   // printf("%i\n", highest_match(term1, n, "Tor"));
//   // // printf("%i\n", sizeof(term));

//   // printf("%s\n", (term1+82641)->term);
//   // printf("%s\n", (term1+82819)->term);
//   // printf("\n\n\n");

//   term* answer1;
//   int p = 0;
//   autocomplete(&answer1, &p, term1, n, "to");

//   for(int i = 0; i < 15;i++){
//     printf("%f\n", answer1[i].weight);
//   }
// }