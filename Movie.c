/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"

int a, b;
int p ;

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
int init_p(){
    //p=random in range max_users to 1021
    srand(time(NULL));
    int rand_p = rand() % (1020 - hashtable_size + 1) + hashtable_size;
    for(int i=0;i<170;i++){
        if(primes_g[i]>=rand_p)
            return primes_g[i];
    }
}
void initialize_hash_function() {
    srand(time(NULL));
    a = rand() % (p-1) + 1; //range 1 to p-1
    b = rand() % (p-1); //range 0 to p-1
}
int hash_function(int userID){
    return ((a*userID+b)%p)%hashtable_size;
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
         p=init_p();
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

void delete_history_tree(userMovie_t **root){
    if(*root!=NULL){
        delete_history_tree(&((*root)->lc));
        delete_history_tree(&((*root)->rc));
        free(*root);
        *root=NULL;
    }
}

int unregister_user(int userID){
        if(user_hashtable_p==NULL){//no users registered
            return 0;
        }
        int index=hash_function(userID);
        if(user_hashtable_p[index]==NULL){//no users in this index
            return 0;
        }
        user_t* temp=user_hashtable_p[index];
        user_t* prev=NULL;
        while(temp!=NULL){
            if(temp->userID==userID){
                if(prev==NULL){//first user in this index
                    delete_history_tree(&(temp)->history);
                    user_hashtable_p[index]=temp->next;
                }
                else{
                    delete_history_tree(&(temp)->history);
                    prev->next=temp->next;
                }
                return 1;
            }
            prev=temp;
            temp=temp->next;
        }
        return 0;//user not found
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
        printf("%d, ", new_releases->movieID);
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
    if(new_releases==NULL){//first movie - root
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
    if(start>end)
        return Sentinel;
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

int k ;
movie_t* create_category_array(new_movie_t *temp, int i, movie_t* array) {
    if (temp != NULL) {
        array = create_category_array(temp->lc, i, array);
        if (temp->category == i) {
            array = (movie_t *) realloc(array, (k + 1) * sizeof(movie_t));
            array[k].movieID = temp->movieID;
            array[k].year = temp->year;
            array[k].watchedCounter = temp->watchedCounter;
            array[k].sumScore = temp->sumScore;
            k++;
        }
        array = create_category_array(temp->rc, i, array);
    }
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
    Sentinel=(movie_t*)malloc(sizeof(movie_t));
    if(Sentinel==NULL){
        printf("Error allocating memory for sentinel\n");
        exit(1);
    }
    Sentinel->movieID=-1;
    for(int i=0;i<6;i++){
        k=0;//counter for array
        movie_t *array=NULL;// if array was global
        array = create_category_array(new_releases,i,array);
        categoryArray[i]=(movieCategory_t*)malloc(sizeof(movieCategory_t));
        if(categoryArray[i]==NULL){
            printf("Error allocating memory for category\n");
            exit(1);
        }
        categoryArray[i]->movie=(movie_t*)malloc(sizeof(movie_t));
        if(categoryArray[i]->movie==NULL){
            printf("Error allocating memory for category\n");
            exit(1);
        }
        categoryArray[i]->movie=insert_into_category_tree(array,0, k-1);
    }
    return 1;
 }

void print_category_tree(movie_t *pMovie) {
    if (pMovie != Sentinel){
        print_category_tree(pMovie->lc);
        printf("%d, ", pMovie->movieID);
        print_category_tree(pMovie->rc);
    }
}
void print_D(movieCategory_t *categoryArray[]) {
    int i;
    char *categoryName[] = {"HORROR", "SCIFI", "DRAMA", "ROMANCE", "DOCUMENTARY", "COMEDY"};
    for (i = 0; i < 6; i++) {
        printf("%s : ", categoryName[i]);
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
 movie_t * search_movie_W(movie_t *root, int movieID){
     if(root==Sentinel)
         return NULL;
     movie_t* temp=root;
     while(temp!=Sentinel){
         if(temp->movieID==movieID){
             return temp;
         }
         else if(temp->movieID<movieID){
             temp=temp->rc;
         }
         else{
             temp=temp->lc;
         }
     }
        return NULL;
 }

user_t *search_user (int userID){
    if(user_hashtable_p==NULL)
        return NULL;
    int index=hash_function(userID);
    if(user_hashtable_p[index]==NULL)
        return NULL;
    user_t *temp=user_hashtable_p[index];
    while(temp!=NULL){
        if(temp->userID==userID){
            return temp;
        }
        temp=temp->next;
    }
    return NULL;
}
struct user_movie *copy_node(struct user_movie *root) {
    userMovie_t *temp = (userMovie_t *) malloc(sizeof(userMovie_t));
    if (temp == NULL) {
        printf("Error allocating memory for user movie\n");
        exit(1);
    }
    temp->movieID = root->movieID;
    temp->score = root->score;
    temp->category = root->category;
    temp->parent = root;
    temp->lc = NULL;
    temp->rc = NULL;
    return temp;
}

userMovie_t *insert_node(userMovie_t *movie) {
    userMovie_t *temp = (userMovie_t *) malloc(sizeof(userMovie_t));
    if (temp == NULL) {
        printf("Error allocating memory for user movie\n");
        exit(1);
    }
    temp->movieID = movie->movieID;
    temp->score =movie->score;
    temp->category = movie->category;
    temp->lc = NULL;
    temp->rc = NULL;
    return temp;
}

void insert_watch_history(userMovie_t **root, userMovie_t *info) {
     if(*root == NULL){
         *root = info;
         (*root)->lc=NULL;
         (*root)->rc=NULL;
         (*root)->parent=NULL;
         return;
     }
     userMovie_t *temp = *root;
    while (temp!=NULL){
        if (temp->movieID>info->movieID){
            if (temp->lc==NULL){
                temp->rc=copy_node(temp);
                temp->movieID=info->movieID;
                temp->score=info->score;
                temp->category=info->category;
                temp->lc= insert_node(info);
                temp->lc->parent=temp;
            }
            temp = temp->lc;
        }else if(temp->movieID<info->movieID) {
            if (temp->rc == NULL) {
                temp->lc = copy_node(temp);
                temp->rc = insert_node(info);
                temp->rc->parent = temp;
            }
            temp = temp->rc;
        }
        else{
            return;
        }
    }
}
void print_history_tree(userMovie_t *root){
    if(root!=NULL){
        print_history_tree(root->lc);
        if(root->rc==NULL&&root->lc==NULL)
            printf("        MovieID:%d Score:%d\n",root->movieID,root->score);
        print_history_tree(root->rc);
    }
}

void Print_W(user_t *user) {
    printf("History Tree of user %d:\n", user->userID);
    print_history_tree(user->history);
}

int watch_movie(int userID, int category, int movieID, int score) {
     //search for the movie in category tree
     movie_t *temp_movie = search_movie_W(categoryArray[category]->movie, movieID);
     if (temp_movie == NULL) {
         return 0;
     }
     //search for the user in hashtable
    user_t *temp_user = search_user(userID);
    if (temp_user == NULL) {
        return 0;
    }
    temp_movie->watchedCounter++;
    temp_movie->sumScore += score;
    //insert movie in user's history tree
     userMovie_t *info = (userMovie_t *) malloc(sizeof(userMovie_t));
     if (info == NULL) {
         printf("Error allocating memory for user movie\n");
         exit(1);
     }
     info->movieID = temp_movie->movieID;
     info->category = category;
     info->score = score;
     insert_watch_history(&(temp_user->history),info);

     //Print_W(temp_user);

    return 1;
 }

int count_movies(movie_t *root, int score) {
    if (root == Sentinel)
        return 0;
    if (root->watchedCounter!=0 && root->sumScore / root->watchedCounter >= score)
        return 1 + count_movies(root->lc, score) + count_movies(root->rc, score);
    else
        return count_movies(root->lc, score) + count_movies(root->rc, score);
}

movie_t** create_filter_array(movie_t *root, int score, movie_t **array){
    if(root != Sentinel) {
        if (root->watchedCounter != 0 && root->sumScore / root->watchedCounter > score) {
            array[k++] = root;
        }
        array = create_filter_array(root->lc, score, array);
        array = create_filter_array(root->rc, score, array);
    }
    return array;
}

void swap(movie_t **a, movie_t **b)
{
    movie_t *t = *a;
    *a = *b;
    *b = t;
}

void heapify(movie_t *array[], int n, int i) {
    int largest = i;
    int l = 2*i + 1; // left = 2*i + 1
    int r = 2*i + 2; // right = 2*i + 2

    if (l < n && ((float)array[l]->sumScore / array[l]->watchedCounter) > ((float)array[largest]->sumScore / array[largest]->watchedCounter)) {
        largest = l;
    }
    if (r < n && ((float)array[r]->sumScore / array[r]->watchedCounter) > ((float)array[largest]->sumScore / array[largest]->watchedCounter)) {
        largest = r;
    }
    if (largest != i) {
        swap(&array[i], &array[largest]);
        heapify(array, n, largest);
    }
}

void heapSort(movie_t *array[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) { //n/2-1 is the last node with at least one child
        heapify(array, n, i); //build heap
    }

    for (int i = n - 1; i >= 0; i--) {
        swap(&array[0], &array[i]);
        heapify(array, i, 0);
    }
}


void print_F(int userID,int score , movie_t **array, int numMovies) {
    printf("    ");
    for (int i = 0; i < numMovies; i++) {
        printf("%d  %.3f , ", array[i]->movieID, (float)array[i]->sumScore/array[i]->watchedCounter);
    }
    printf("\nDone\n");

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
	 //search in the category tree for movies with score>=score

    user_t *temp_user = search_user(userID);
    if (temp_user == NULL) {
        return 0;
    }
    int numMovies=0;
    for(int i=0;i<6;i++){
        numMovies+=count_movies(categoryArray[i]->movie,score);
    }
    movie_t **array=(movie_t**)malloc(numMovies*sizeof(movie_t*));
    if(array==NULL){
        printf("Error allocating memory for array\n");
        exit(1);
    }
    k=0;
    for(int i=0;i<6;i++){
       array=create_filter_array(categoryArray[i]->movie,score,array);
    }
    heapSort(array,numMovies);
    //print_F(userID,score,array,numMovies);
    return 1;
 }
 int counter ;
 int Sumscore ;


userMovie_t * fully_left_leaf(userMovie_t *root){
    if(root==NULL)
        return NULL;
    if(root->lc==NULL&&root->rc==NULL)
        return root;
    if(root->lc!=NULL)
        return fully_left_leaf(root->lc);
    else
        return fully_left_leaf(root->rc);

}

userMovie_t *FindNextleaf(userMovie_t *prev_child){
    if(prev_child->parent==NULL)
        return NULL;
    if(prev_child->parent->lc==prev_child){
        if(prev_child->parent->rc!=NULL)
            return fully_left_leaf(prev_child->parent->rc);
        else
            return FindNextleaf(prev_child->parent);
    }
    else{
        return FindNextleaf(prev_child->parent);
    }
}/*
 * if leaf is left child -> if right child is not null -> return fully left leaf of right child
 * else temp_root = leaf->parent
 * */



int total_score(userMovie_t *root){
    userMovie_t *left=fully_left_leaf(root);
    if(left==NULL)
        return -1;
    Sumscore+=left->score;
    counter++;
    while (1){
        userMovie_t *next=FindNextleaf(left);
        if(next==NULL)
            break;
        left=next;
        Sumscore+=next->score;
        counter++;
    }
    return Sumscore/counter;
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
     //search for user in hashtable
     user_t *temp_user = search_user(userID);
     if (temp_user == NULL) {
         return 0;
     }
     counter=0;
     Sumscore=0;
     //add all scores of the leaves of the tree left to right
     int score=total_score(temp_user->history);
        if(score==-1){
            return 0;
        }
        printf("Q %d  %d\n",userID,score);
 }



void print_I(int id, int category, int year) {
    char *categoryName[] = {"HORROR", "SCIFI", "DRAMA", "ROMANCE", "DOCUMENTARY", "COMEDY"};
    printf("I %d %s %d \n", id, categoryName[category], year);
    printf("DONE\n");
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
    if(categoryArray[category]->movie==Sentinel)
        return 0;
    movie_t* temp=categoryArray[category]->movie;
    while(temp!=Sentinel){
        if(temp->movieID==movieID){
            print_I(movieID,category,temp->year);
            return 1;
        }
        else if(temp->movieID<movieID){
            temp=temp->rc;
        }
        else{
            temp=temp->lc;
        }
    }
    return 0;
 }

 /**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

 int print_movies(void){
     print_D(categoryArray);
	 return 1;
 }

  /**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

 int print_users(void){
     if(user_hashtable_p==NULL){
         return 0;
     }
     for(int i=0;i<hashtable_size;i++){
         if(user_hashtable_p[i]!=NULL){
             printf("Chain %d of Users:\n",i);
             user_t* temp=user_hashtable_p[i];
             while(temp!=NULL){
                 printf("    %d\n",temp->userID);
                 printf("    History Tree:\n");
                 print_history_tree(temp->history);
                 temp=temp->next;
             }
         }
     }
     printf("DONE\n");
     return 1;
 }

