/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

int a, b;


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
    a = rand() % (max_users-1) + 1; //range 1 to max_users-1
    b = rand() % (max_users-1); //range 0 to max_users-1
}
int hash_function(int userID){
        return (a*userID+b)%hashtable_size;
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
    /*
     R <userID>
     Chain <j> of Users:
        <userID1>
        <userID2>
        ...
        <userIDn>
DONE*/
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
        if(counter==max_users){
            printf("Max users reached\n");
            return 0;
        }
     if(userID>max_id){
         printf("User ID is greater than max_users\n");
         return 0;
     }
     if(user_hashtable_p==NULL){
         init_hash_table(hashtable_size);
         initialize_hash_function();
     }
     int index=hash_function(userID);

     if(user_hashtable_p[index]==NULL){
         user_hashtable_p[index]=(user_t*)malloc(sizeof(user_t));
         if(user_hashtable_p[index]==NULL){
             printf("Error allocating memory for user\n");
             exit(1);
         }
         user_hashtable_p[index]->userID=userID;
         user_hashtable_p[index]->history=NULL;
         user_hashtable_p[index]->next=NULL;
     }
     else{
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

 int unregister_user(int userID){
	 return 1;
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
	 return 1;
 }
 
 /**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 0 on success
 *         1 on failure
 */

 int distribute_movies(void){
	 return 1;
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
	 return 1;
 }
 
