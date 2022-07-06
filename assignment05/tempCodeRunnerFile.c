#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT	1
#define FILE_INPUT		2

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree);

/* internal function (not mandatory)
*/
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static NODE* _insert( NODE *root, NODE *newPtr);

NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level);

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int mode; // input mode
	TREE *tree;
	int data;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
		return 1;
	}
	
	FILE *fp;
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		mode = RANDOM_INPUT;
	}
	else mode = FILE_INPUT;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	
	if (mode == RANDOM_INPUT)
	{
		int numbers;
		numbers = atoi(argv[1]);
		assert( numbers > 0);

		fprintf( stdout, "Inserting: ");
		
		srand( time(NULL));
		for (int i = 0; i < numbers; i++)
		{
			data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
			
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
	}
	
	else if (mode == FILE_INPUT)
	{
		fprintf( stdout, "Inserting: ");
		
		while (fscanf( fp, "%d", &data) != EOF)
		{
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
		fclose( fp);
	}
	
	fprintf( stdout, "\n");

	if (BST_Empty( tree))
	{
		fprintf( stdout, "Empty tree!\n");
		BST_Destroy( tree);
		return 0;
	}	

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left inorder traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		int ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left inorder traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void)
{
	TREE* tree;
	tree=(TREE*)malloc(sizeof(TREE));
	if(tree)
	{
		tree->root=NULL;
		return tree;
	}
	else
	{
		return NULL;
	}
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree)
{
	if(pTree)
	{
		_destroy(pTree->root);
	}
	free(pTree);
}

/* internal function (not mandatory)
*/
static void _destroy( NODE *root)
{
	if(root)
	{
		_destroy(root->left);
		_destroy(root->right);
		free(root);
	}
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data)
{
	NODE* newNode=_makeNode(data);
	if(BST_Empty(pTree))
	{
		pTree->root=newNode;
	}
	else	
	{	
		_insert(pTree->root,newNode);
	}
	return 1;

}

/* internal function (not mandatory)
*/
static NODE* _insert( NODE *root, NODE *newPtr)
{ 
	if(root==NULL)
	{	
		return newPtr;
	}
	else if((root->data)>(newPtr->data))
	{
		(root->left)=_insert(root->left,newPtr);
		return root;
	}
	else
	{
		(root->right)=_insert(root->right,newPtr);
		return root;
	}
}

NODE *_makeNode( int data)
{
	NODE* newNode;
	newNode=(NODE*)malloc(sizeof(NODE));
	if(newNode)
	{
		newNode->left=NULL;
		newNode->right=NULL;
		newNode->data=data;
		return newNode;
	}
	else
	{
		return NULL;
	}
	
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey)
{	
	int result;
	NODE* newRoot;
	newRoot=_delete(pTree->root,dltKey,&result);
	if(result)
	{
		pTree->root=newRoot;
	}
	return result;
}

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success)
{	
	NODE* tmpP;
	NODE* d;
	NODE* newRoot;
	int tmp;
	
	if(!root)
	{
		*success=0;
		return NULL;
	}

	if((root->data)>(dltKey))
	{
		root->left=_delete(root->left,dltKey,success);
	}
	else if((root->data)<(dltKey))
	{
		root->right=_delete(root->right,dltKey,success);
	}
	else
	{
		d=root;
		if(root->right==NULL)
		{	
			
			newRoot=root->left;
			free(d);
			*success=1;
			return newRoot;
			
		}
		else
		{
			if(root->left==NULL)
			{
				newRoot=root->right;
				free(d);
				*success=1;
				return newRoot;
			}
			else
			{
				tmpP=root->right;
				while(tmpP->left)
				{
					tmpP=tmpP->left;
				}
				tmp=root->data;
				root->data=tmpP->data;
				tmpP->data=tmp;
				root->right=_delete(root->right,tmpP->data,success);

			}
		}
		
	}
	return root;
}



/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree)
{
	_traverse(pTree->root);
	return;
}
static void _traverse( NODE *root)
{
	if(root)
	{
		_traverse(root->left);
		printf("%d ",root->data);
		_traverse(root->right);
	}
	else
	{
		return;
	}
}

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree)
{
	_inorder_print(pTree->root,0);
}
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level)
{	
	if(root==NULL)
	{	
		return;
	}
	level++;
	_inorder_print(root->right,level);
	
	
	for(int i=1;i<level;++i)
	{
		printf("\t");
	}
	
	printf("%d",root->data);
	printf("\n");
	_inorder_print(root->left,level);
	
	
}


/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree)
{
	if(pTree->root==NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}