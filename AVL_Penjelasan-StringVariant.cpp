//Ini template AVL yang dari ppt + penjelasannya, semangat ngapal :D
//Di template ini aku coba pake string buat jadi pembandingnya, bukan int. Yaa, siapa tau aja masuk wkwk

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//===========================================================
struct Node{ 
    char name[100];
    int num;
    Node * left;
    Node *right;
    int height;
};

/*  
    Tips menghemat waktu!! Kalo kalian pake extension .cpp atau .c++, 
    bisa langsung tulis nama struct dibanding harus nulis struct Node :D
*/
Node *createNode(int key, const char name[]){ 
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode->num = key;
    strcpy(newNode->name, name);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;

    return newNode;
}

//Function yang mau dipake
//===========================================================
int max(int a, int b){
    return (a > b)? a : b;
}

int getHeight(Node *node){
    if(!node) 
        return 0;
    return node->height;
}

int getBalance(Node *node){
    if(!node) 
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

//Rotations
//===========================================================
Node *rightRot(Node *y){
    Node *x = y->left;
    Node *z = x->right;

    x->right = y;
    y->left = z;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return x;
}

Node *leftRot(Node *x){
    Node *y = x->right;
    Node *z = y->left;

    y->left = x;
    x->right = z;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

//Insert
//===========================================================
Node *insertNode(Node *node, int key, const char name[]){
    //Jika current node belom ada isinya, langsung create node 
    if(!node) 
        return createNode(key, name);
    //Kalau lebih kecil dari current node, recursion ke kiri
    if(strcmp(name, node->name) < 0) 
        node->left = insertNode(node->left, key, name);
    //Kalau lebih besar dari current node, recursion ke kanan
    else if(strcmp(name, node->name) > 0) 
        node->right = insertNode(node->right, key, name);
    //Gak boleh double
    else 
        return node;
    
    //Update height dan dapetin balance dari tree
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    int balance = getBalance(node);

    //Left left case
    if(balance > 1 && key < node->left->num)
        return rightRot(node);
    //Right right case
    if(balance < -1 && key > node->right->num)
        return leftRot(node);
    //Left right case
    if(balance > 1 && key > node->right->num){
        node->left = leftRot(node->left);
        return rightRot(node);
    }
    //Right left case
    if(balance < -1 && key < node->left->num){
        node->right = rightRot(node->right);
        return leftRot(node);
    }

    return node;
}

//Print
//===========================================================
void inOrder(Node *node){
    if(node){
        inOrder(node->left);
        printf("%s %d\n", node->name, node->num);
        inOrder(node->right);
    }
}

//Deletion
//===========================================================
//Kita pake predecessor yaa, kalo mau pake successor tinggal ganti right jadi left
Node *minValueNode(Node *node){
    Node *curr = node;
    while(curr->right){
        curr = curr->right;
    }
    return curr;
}

Node *deleteNode(Node *node, const char name[]){
    //Bagian ini sama kayak insert tadi
    if(!node) 
        return node;
    else if(strcmp(name, node->name) < 0)
        node->left = deleteNode(node->left, name);
    else if(strcmp(name, node->name) > 0)
        node->right = deleteNode(node->right, name);
    else{ 
        //Jika tidak ada anak (leaf node)
        if(!node->left && !node->right){ 
            free(node);
            node = NULL;
        } 

        //Anaknya 1
        else if(!node->left || !node->right){ 
            Node *temp = node->left? node->left : node->right; 
            *node = *temp;
            free(temp);
        }

        //Punya 2 anak
        else {
            //Kalo mau pake successor tinggal ganti semua right jadi left
            Node *temp = minValueNode(node->right);
            strcpy(node->name, temp->name);
            node->num = temp->num;
            node->right = deleteNode(node->right, temp->name);
        }
    }

    if(!node) 
        return node;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    int balance = getBalance(node);

    //Left left case
    if(balance > 1 && getBalance(node->left) > 0)
        return rightRot(node);
    //Left right case
    if(balance > 1 && getBalance(node->left) < 0){
        node->left = leftRot(node->left);
        return rightRot(node);
    }
    //Right right case
    if(balance < -1 && getBalance(node->right) < 0)
        return leftRot(node);
    //Right left case
    if(balance < -1 && getBalance(node->right) > 0){
        node->right = rightRot(node->right);
        return leftRot(node);
    }

    return node;
}

int main(){
    Node *root = NULL;

    root = insertNode(root, 5, "abc");
    root = insertNode(root, 10, "baa");
    root = insertNode(root, 20, "fa");
    root = insertNode(root, 4, "aaa");

    inOrder(root);

    root = deleteNode(root, "baa");
    printf("\n\nAfter deleting node:\n");
    inOrder(root);

    return 0;
}