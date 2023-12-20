/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

int a, b;
int p = 1021;

int hashtable_SIZE(int max_users,int primes_g[]){
        int i;
        for(i=0;i<170;i++){
            if(primes_g[i]>max_users){
                return primes_g[i];
            }
        }
        return -1;
 }
void init_hash_table(int hash_table_size){
     hashtable_size=hashtable_SIZE(max_users,primes_g);
     user_hashtable_p=(user_t**)calloc(hashtable_size, sizeof(user_t*));
     if(user_hashtable_p==NULL){
         printf("Error allocating memory for hashtable\n");
         exit(1);
     }
 }
void initialize_hash_function() {
    srand(time(NULL));
    a = rand() % (p-1) + 1; //range 1 to p-1
    b = rand() % (p-1); //range 0 to p-1
}
int hash_function(int userID){
        //return ((a*userID+b)%p)%hashtable_size;
        return userID%3;
    }
/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

void print_R(int userID){
    printf("R %d\n",userID);
    int index=hash_function(userID);
    user_t* temp=user_hashtable_p[index];
    printf("Chain %d of Users:\n",index);
    while(temp!=NULL){
        printf("    %d\n",temp->userID);
        temp=temp->next;
    }
    printf("DONE\n");
}

 int register_user(int userID){
     static int counter=0;
        if(counter==max_users){//check if max users reached
            printf("Max users reached\n");
            return 0;
        }
     if(userID>max_id){//check if userID is greater than max_id
         printf("User ID is greater than max_users\n");
         return 0;
     }
     if(user_hashtable_p==NULL){//first time we register so initialize the hashtable
         init_hash_table(hashtable_size);
         initialize_hash_function();
     }
     int index=hash_function(userID);

     if(user_hashtable_p[index]==NULL){//first user in this index
         user_hashtable_p[index]=(user_t*)malloc(sizeof(user_t));
         if(user_hashtable_p[index]==NULL){
             printf("Error allocating memory for user\n");
             exit(1);
         }
         user_hashtable_p[index]->userID=userID;
         user_hashtable_p[index]->history=NULL;
         user_hashtable_p[index]->next=NULL;
     }
     else{//there are already users in this index
         user_t* temp=user_hashtable_p[index];
         user_t* prev=NULL;
         while(temp!=NULL){
             if(temp->userID==userID){
                 printf("User already exists\n");
                 return 0;
             }
             prev=temp;
             temp=temp->next;
         }
         prev->next=(user_t*)malloc(sizeof(user_t));
         if(prev->next==NULL){
             printf("Error allocating memory for user\n");
             exit(1);
         }
         prev->next->userID=userID;
         prev->next->history=NULL;
         prev->next->next=NULL;
     }
     counter++;
     return 1;
 }
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */
 void print_U(int userID){
     printf("U %d\n",userID);
    int index=hash_function(userID);
    user_t* temp=user_hashtable_p[index];
    printf("Chain %d of Users:\n",index);
    while(temp!=NULL){
        printf("    %d\n",temp->userID);
        temp=temp->next;
    }
     printf("DONE\n");
 }

void delete_history_tree(userMovie_t *pMovie) {
    if (pMovie != NULL) {
        delete_history_tree(pMovie->lc);
        delete_history_tree(pMovie->rc);
        pMovie=NULL;
    }
}

int unregister_user(int userID){
        if(user_hashtable_p==NULL){//no users registered
            printf("No users registered\n");
            return 0;
        }
        int index=hash_function(userID);
        if(user_hashtable_p[index]==NULL){//no users in this index
            printf("No user with this userID\n");
            return 0;
        }
        user_t* temp=user_hashtable_p[index];
        user_t* prev=NULL;
        while(temp!=NULL){
            if(temp->userID==userID){
                if(prev==NULL){//first user in this index
                    delete_history_tree(temp->history);
                    user_hashtable_p[index]=temp->next;
                }
                else{
                    delete_history_tree(temp->history);
                    prev->next=temp->next;
                }
                return 1;
            }
            prev=temp;
            temp=temp->next;
        }
        printf("No user with this userID\n");
        return 0;
 }

void insert_new_movie(new_movie_t *new_releases, int id, int category, int year) {
     if(id<new_releases->movieID){
         if(new_releases->lc==NULL){
             new_releases->lc=(new_movie_t*)malloc(sizeof(new_movie_t));
             if(new_releases->lc==NULL){
                 printf("Error allocating memory for new movie\n");
                 exit(1);
             }
             new_releases->lc->movieID=id;
             new_releases->lc->category=category;
             new_releases->lc->year=year;
             new_releases->lc->watchedCounter=0;
             new_releases->lc->sumScore=0;
             new_releases->lc->lc=NULL;
             new_releases->lc->rc=NULL;
         }
         else{
             insert_new_movie(new_releases->lc,id,category,year);
         }
     }
     else if(id>new_releases->movieID){
         if(new_releases->rc==NULL){
             new_releases->rc=(new_movie_t*)malloc(sizeof(new_movie_t));
             if(new_releases->rc==NULL){
                 printf("Error allocating memory for new movie\n");
                 exit(1);
             }
             new_releases->rc->movieID=id;
             new_releases->rc->category=category;
             new_releases->rc->year=year;
             new_releases->rc->watchedCounter=0;
             new_releases->rc->sumScore=0;
             new_releases->rc->lc=NULL;
             new_releases->rc->rc=NULL;
         }
         else{
             insert_new_movie(new_releases->rc,id,category,year);
         }
     }
     else{
         printf("Movie already exists\n");
     }
}
void print_A(new_movie_t *new_releases) {
    if (new_releases != NULL) {
        print_A(new_releases->lc);
        if(new_releases->rc!=NULL)
            printf("%d,", new_releases->movieID);
        print_A(new_releases->rc);
    }
}

/**
 * @brief Add new movie to new release binary tree.
 * Create a node movie and insert it in 'new release' binary tree.
 *
 * @param movieID The new movie identifier
 * @param category The category of the movie
 * @param year The year movie released
 *
 * @return 1 on success
 *         0 on failure
 */

 int add_new_movie(int movieID, int category, int year){
    if(new_releases==NULL){//first movie
         new_releases=(new_movie_t*)malloc(sizeof(new_movie_t));
         if(new_releases==NULL){
             printf("Error allocating memory for new movie\n");
             exit(1);
         }
         new_releases->movieID=movieID;
         new_releases->category=category;
         new_releases->year=year;
         new_releases->watchedCounter=0;
         new_releases->sumScore=0;
         new_releases->lc=NULL;
         new_releases->rc=NULL;
    }
    else{
     insert_new_movie(new_releases,movieID,category,year);
    }
     return 1;
 }

movie_t *Sentinel;



movie_t* insert_into_category_tree(movie_t array[], int start, int end){
    //inorder insert from array to tree
    if(start>end)
        return NULL;
    int mid=(start+end)/2;
    movie_t* root=(movie_t*)malloc(sizeof(movie_t));
    if(root==NULL){
        printf("Error allocating memory for movie\n");
        exit(1);
    }
    root->movieID=array[mid].movieID;
    root->year=array[mid].year;
    root->watchedCounter=array[mid].watchedCounter;
    root->sumScore=array[mid].sumScore;
    root->lc = insert_into_category_tree(array, start, mid-1);
    root->rc = insert_into_category_tree(array, mid+1, end);
    return root;
}

movie_t *array;
int k ;
movie_t* create_category_array(new_movie_t *temp, int i) {
    static movie_t* array = NULL;
    if (temp == NULL) {
        return array;
    }
    create_category_array(temp->lc, i);

    if (temp->category == i) {
        array = (movie_t *) realloc(array, (k + 1) * sizeof(movie_t));
        array[k].movieID = temp->movieID;
        array[k].year = temp->year;
        array[k].watchedCounter = temp->watchedCounter;
        array[k].sumScore = temp->sumScore;
        k++;
    }
    create_category_array(temp->rc, i);
    return array;
}

/**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 1 on success
 *         0 on failure
 */


 int distribute_movies(void){
    //split new_releases tree to categories array of trees
    if(new_releases==NULL){
        printf("No movies to distribute\n");
        return 0;
    }
    for(int i=0;i<6;i++){
        k=0;
        array = create_category_array(new_releases,i);
        categoryArray[i]=(movieCategory_t*)malloc(sizeof(movieCategory_t));
        categoryArray[i]->movie=(movie_t*)malloc(sizeof(movie_t));
        categoryArray[i]->movie=insert_into_category_tree(array,0, k-1);
        int x=0;
    }
 }

void print_category_tree(movie_t *pMovie) {
    if (pMovie != NULL) {
        print_category_tree(pMovie->lc);
        printf("%d,", pMovie->movieID);
        print_category_tree(pMovie->rc);
    }

}

void print_D(movieCategory_t *categoryArray[]) {
    int i;
    for (i = 0; i < 6; i++) {
        printf("Category %d:\n", i);
        print_category_tree(categoryArray[i]->movie);
        printf("\n");
    }
    printf("DONE\n");
 }
 
 /**
 * @brief User rates the movie with identification movieID with score
 *
 * @param userID The identifier of the user
 * @param category The Category of the movie
 * @param movieID The identifier of the movie
 * @param score The score that user rates the movie with id movieID
 *
 * @return 1 on success
 *         0 on failure
 */

 int watch_movie(int userID,int category, int movieID, int score){
	 return 1;
 }
 
/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int filter_movies(int userID, int score){
	 return 1;
 }
 
/**
 * @brief Find movies from categories withn median_score >= score t
 *
 * @param userID The identifier of the user
 * @param category Array with the categories to search.
 * @param score The minimum score the movies we want to have
 *
 * @return 1 on success
 *         0 on failure
 */

 int user_stats(int userID){
	 return 1;
 }
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

 int search_movie(int movieID, int category){
	 return 1;
 }
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void){
	 return 1;
 }
 
  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void){
     //print hashtable
     for(int i=0;i<hashtable_size;i++){
         if(user_hashtable_p[i]!=NULL){
             printf("Chain %d of Users:\n",i);
             user_t* temp=user_hashtable_p[i];
             while(temp!=NULL){
                 printf("    %d\n",temp->userID);
                 printf("    History Tree:\n");
                 //print_history_tree(temp->history);
                 temp=temp->next;
             }
         }
     }
     printf("DONE\n");
 }
 
