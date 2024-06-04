#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node {
    char number[10];
    char *name;
    struct node * left;
    struct node * right;
};


struct node * rootNumber,*rootName,*ptr,*temp;
int foundName,foundContact;

// create node
struct node * createNode(char num[], char *nam){


    temp = (struct node *)malloc(sizeof(struct node));
    temp ->left =NULL;
    temp->right =NULL;

    temp->name = (char *)malloc(sizeof(nam));

    strcpy(temp->name,nam);
    strcpy(temp->number,num);

    //printf("NOde details : %s,%s",temp->name,temp->number);
    return temp;
}

// addNode to the tree
void addNodeNumber(struct node *N){
    if(rootNumber == NULL)
        rootNumber = N;
    else{
        //printf("\n%s : %s",N->name,N->number);

        ptr = rootNumber;
        while(ptr!=NULL){
            if( strcmp(N->number,ptr->number) < 0 && ptr->left!=NULL)
                ptr = ptr->left;
            else{
                if(strcmp(N->number,ptr->number)<0){
                    ptr->left = N;
                    return;
                }
                else{
                    if(ptr->right == NULL && strcmp(N->number,ptr->number) > 0){
                        ptr->right = N;
                        return;
                    }
                    else
                        ptr = ptr->right;
                }
            }
        }
    }
}

// addNode to the tree
void addNodeName(struct node *N){
    if(rootName == NULL)
        rootName = N;
    else{

        ptr = rootName;
        while(ptr!=NULL){
            if( strcmp(N->name,ptr->name) < 0 && ptr->left!=NULL)
                ptr = ptr->left;
            else{
                if(strcmp(N->name,ptr->name)<0){
                    ptr->left = N;
                    return;
                }
                else{
                    if(ptr->right == NULL){
                        ptr->right = N;
                        return;
                    }
                    else
                        ptr = ptr->right;
                }
            }
        }
    }
}


// displayDirectory
void displayDirectory(struct node *dis){
    if(dis!=NULL){
        displayDirectory(dis->left);
        printf("\n%s : %s",dis->name,dis->number);
        displayDirectory(dis->right);
    }
}


//create tree
struct node * createNumberTree(FILE * directory){
    if(directory == NULL){
        printf("\nError opening the file.");
        return NULL;
    }

    char line[65];
    int lineNumber = 1;

    while(fgets(line, sizeof(line), directory) != NULL){
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");

        // extracting name
        char *name;
        name = token;

        // extracting number
        token = strtok(NULL, ",");
        if(token==NULL){
            printf("\nWarning: The data at line number %d of the file is incomplete.",lineNumber);
        }
        else{
            char *number = token;
            addNodeNumber(createNode(number,name));
        }
        lineNumber++;
    }

    return rootNumber;

}

//create tree
struct node * createNameTree(FILE * directory){
    if(directory == NULL){
        printf("\nError opening the file.");
        return NULL;
    }

    char line[65];
    int lineNumber = 1;

    while(fgets(line, sizeof(line), directory) != NULL){
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");

        // extracting name
        char *name;
        name = token;

        // extracting number
        token = strtok(NULL, ",");
        if(token==NULL){
            printf("\nWarning: The data at line number %d of the file is incomplete.",lineNumber);
        }
        else{
            char *number = token;
            addNodeName(createNode(number,name));
        }
        lineNumber++;
    }

    return rootName;

}

// searching by number
void search_by_number(struct node* root, char n[10]) {
    if (root == NULL) {
        printf("Contact does not exist.\n");
        return;
    }
    if(strcmp(n,root->number)>0){
        search_by_number(root->right, n);
    }else if(strcmp(n,root->number)<0){
        search_by_number(root->left, n);
    }else{
        foundContact=1;
        printf("The contact exists.\n Name: %s\n", root->name);
    }
}


// searching by name
// search by name

void searchByName(struct node* root, char *name){
    

    if (root == NULL){
        return;
    }

    int result = strcmp(name, root->name);

    if(result == 0){
        printf("\n%s : %s", root->name,root->number);
        foundName=1;
        searchByName(root->right, name);
    }
    else if(result<0){
        searchByName(root->left, name);
    }else{
        searchByName(root->right, name);
    }
}




// main
void main(){


    char directoryName[50];
    printf("Enter the name of the directory: ");
    scanf("%s",directoryName);

    // creation of directory by number
    FILE * directoryByNumber = fopen(directoryName,"r");
    printf("\nCreating the directory file by number.");
    rootNumber = createNumberTree(directoryByNumber);

    // creation of directory by name
    FILE * directoryByName = fopen(directoryName,"r");
    printf("\n\nCreating the directory file by name.");
    rootName = createNameTree(directoryByName);

    if(rootName!=NULL && rootNumber!=NULL){
        char *name;
        char number[10];
        int ch;

        int more=1;
        while(more==1){
            int choice;
            printf("\nEnter the choice number: \nSearch by name(1)\nSearch by number(2)\nDisplay directory by name(3)\nDisplay directory by number(4)\nexit(5):   ");
            scanf("%d",&choice);

            switch(choice){
                case 1:
                    printf("\nEnter the name to be searched: ");
                    scanf("%s",name);
                    foundName=0;
                    searchByName(rootName,name);
                    if(foundName==0)
                        printf("Contact name for %s not found.",name);
                    break;
                case 2:
                    printf("\nEnter the number to be searched: ");
                    scanf("%s",number);

                    number[strcspn(number, "\n")] = '\0';

                    foundContact=0;
                    search_by_number(rootNumber,number);
                    if(foundContact==0)
                        continue;
                    
                    break;

                case 3: if(rootName!=NULL){
                        printf("\nThe directory by name is as follows: ");
                        displayDirectory(rootName);
                        }
                    else
                        printf("\nThe directory by name is empty.");
                    break;

                case 4: if(rootNumber!=NULL){
                        printf("\nThe directory by number is as follows: ");
                        displayDirectory(rootNumber);
                        }
                    else
                        printf("\nThe directory by number is empty.");
                    break;

                case 5:
                        printf("\nExiting from the program!");
                        goto leave;
                default:
                    printf("\nInput valid choice number");
            }
            printf("\nDo you want to continue: y(1) / n(0): ");
            scanf("%d",&more);
        }
        leave:
        printf("\n-------THANK YOU FOR VISITING----------");
        fclose(directoryByName);
        fclose(directoryByNumber);
    }

    else{
        printf("\nDirectory not found. Please input the correct directory name.");
    }
    
}