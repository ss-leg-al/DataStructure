#include <stdio.h>
int main(void)
{
    char* data="hello";
    printf("%s",data);
    return 0;
}


static NODE *balancing(NODE *root)
{
	int bFactor = balanceFactor(root);
  
  // LL Case
  if (bFactor > 1 && (strcmp(root->data,root->left->data)<0))
    return rotateRight(root);
  // RR Case
  if (bFactor < -1 && (strcmp(root->data,root->right->data)>0))
    return rotateLeft(root);
  // LR Case
  if (bFactor > 1 && (strcmp(root->data,root->left->data)>0)){
    root->left = rotateLeft(root->left);
    return rotateRight(root);
  }
  // RL Case
  if (bFactor < -1 && (strcmp(root->data,root->right->data)<0)){
    root->right = rotateRight(root->right);
    return rotateLeft(root);
  }
	return root;
}
static NODE *rotateLeft( NODE *root)
{
	NODE* tmp;
	tmp=root->right;
	root->right=tmp->left;
	tmp->left=root;
	root->height=max(getHeight(root->left),getHeight(root->right))+1;
	tmp->height=max(getHeight(tmp->left),getHeight(tmp->right))+1;
	return tmp;
}