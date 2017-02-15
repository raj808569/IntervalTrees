#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
using namespace std;
struct interval{
	int lower_bound,higher_bound;
};
struct node{
	interval *pair;
	int maximum;
	node *left,*right;
};
node *newnode(interval x){
	struct node *temp=new node;
	temp->pair=new interval(x);
	temp->maximum=x.higher_bound;
	temp->left=temp->right=NULL;
	return temp;
}
struct node * minimum_lower_bound(struct node* current_node)
{
    struct node* current = current_node;
    while (current->left != NULL)
        current=current->left;
	return current;
}
int getmaximum(int p,int q,int r){
	return p>q?(p>r?p:r):(q>r?q:r);
}
void inorder(node *root){
	if(root!=NULL){
		inorder(root->left);
		cout<<"["<<root->pair->lower_bound<<","<<root->pair->higher_bound<<"] "<<"maximum = "<<root->maximum<<"\n";
		inorder(root->right);
	}
}

node *insert(node *root,interval x){
	if(root==NULL){
		return newnode(x);
	}
	if(x.lower_bound<(root->pair)->lower_bound){
		root->left=insert(root->left,x);
	}
	else{
		root->right=insert(root->right,x);
	}
	if(root->maximum<x.higher_bound){
		root->maximum=x.higher_bound;
	}
	return root;
}
node *delete_interval(node *root,interval x){
	if(root==NULL){
		return NULL;
	}
	if(x.lower_bound<(root->pair)->lower_bound){
		root->left=delete_interval(root->left,x);
	}
	else if(x.lower_bound>(root->pair)->lower_bound){
		root->right=delete_interval(root->right,x);
	}
	else if(x.lower_bound==(root->pair)->lower_bound ){
		if(x.higher_bound==(root->pair)->higher_bound){
			if(root->left==NULL){
				struct node *temp=root->right;
				free(root);
				return temp;
			}
			else if(root->right==NULL){
				struct node *temp=root->left;
				free(root);
				return temp;
			}
			struct node *temp=minimum_lower_bound(root->right);
			root->pair=temp->pair;
			root->right=delete_interval(root->right,*(temp->pair));
			}
		else{
			root->right=delete_interval(root->right,x);
		}
	}
	root->maximum=getmaximum(root->pair->higher_bound,((root->left!=NULL)?root->left->maximum:INT_MIN),((root->right!=NULL)?root->right->maximum:INT_MIN));
	return root;
}
bool isoverlaptrue(interval first,interval second){
	return (!(first.higher_bound<second.lower_bound || second.higher_bound<first.lower_bound));
}
interval *search_interval(node *root,interval x){
	if(root==NULL){
		return NULL;
	}
	if(isoverlaptrue(*(root->pair),x)){
		return root->pair;
	}
	if(root->left!=NULL && x.lower_bound<=(root->left)->maximum){
		return search_interval(root->left,x);
	}
	return search_interval(root->right,x);
}

int main(){
	interval sample[] = {{15, 20}, {10, 30}, {17, 19},{5, 20}, {12, 15}, {30, 40}};
    int length = sizeof(sample)/sizeof(sample[0]);
    node *root = NULL;
    for (int i = 0; i < length; i++)
        root = insert(root,sample[i]);
	cout << "the tree created is as follows:-\n";
    inorder(root);
	interval test1 = {14,16};
	interval test2 = {21,23};
    interval *fetched_result1 = search_interval(root, test1);
    if (fetched_result1 == NULL)
        cout << "\n no matching interval with {14,16}\n";
    else
        cout << "\n {14,16} overlaps with {" << fetched_result1->lower_bound << "," << fetched_result1->higher_bound << "} \n";
	
	interval *fetched_result2 = search_interval(root, test2);
    if (fetched_result2 == NULL)
        cout << "\n no matching interval with {21,23}\n";
    else
        cout << "\n {21,23} overlaps with {" << fetched_result2->lower_bound << "," << fetched_result2->higher_bound << "} \n";
        
    interval deln1= {30,40};
    cout << "\n deleting interval {30,40}";
    root=delete_interval(root,deln1);
    cout << "\n resulting interval tree is: \n";
    inorder(root);
    
	interval deln2= {17,19};
    cout << "\n deleting interval {17,19}";
    root=delete_interval(root,deln2);
     cout << "\n final tree is \n";
    inorder(root);
    return 0;
}
