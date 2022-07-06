#define SHOW_STEP 0 // 제출시 0
#define BALANCING 1 // 제출시 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void);

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree);
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data);

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE *_makeNode( char *data);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);
static void _traverse( NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root);
int balanceFactor(NODE *root);
static NODE *balancing( NODE *root, NODE *newPtr);


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	} 

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{

		key = AVL_Retrieve( tree, str);
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);

		
		fprintf( stdout, "Query: ");
	}
	
	// destroy tree
	AVL_Destroy( tree);

	return 0;
	
}

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void)
{
	AVL_TREE* avl;
	avl=(AVL_TREE*)malloc(sizeof(AVL_TREE));
	if(avl)
	{
		avl->root=NULL;
		avl->count=0;
		return avl;
	}
	else
	{
		return NULL;
	}
	

}

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree)
{
	if(pTree)
	{
		_destroy(pTree->root);
	}
	free(pTree);
}
static void _destroy( NODE *root)
{
	if(root)
	{
		_destroy(root->left);
		free(root->data);
		_destroy(root->right);
		free(root);
	}
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data)
{
	NODE* newNode;
	newNode=_makeNode(data);
	if(newNode)
	{
		pTree->root=_insert(pTree->root,newNode);
		(pTree->count)++;
		return 1;

	}
	else
	{
		return 0;
	}
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr)
{
	
	if(!root)
	{
		root=newPtr;
		return root;
	}
	if(strcmp(newPtr->data,root->data)<0)
	{
		root->left=_insert(root->left,newPtr);
	}
	else
	{
		root->right=_insert(root->right,newPtr);
	}
	
	root->height=max(getHeight(root->left),getHeight(root->right))+1;
	
	
	
	if(balanceFactor(root)>1||balanceFactor(root)<-1)
	{
		root=balancing(root,newPtr);	
	}
	
	
	
	return root;
}

static NODE *_makeNode( char *data)
{
	NODE* newNode;
	newNode=(NODE*)malloc(sizeof(NODE));
	if(newNode)
	{
		newNode->data=strdup(data);
		newNode->right=NULL;
		newNode->left=NULL;
		newNode->height=1;
		return newNode;
	}
	else
	{
		return NULL;
	}
	
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key)
{
	NODE* a;
	a=_retrieve(pTree->root,key);
	if(pTree->root)
	{	
		if(a)
		{
			return a->data;
		}
		else
		{
			return NULL;
		}
	}
	else 
	{
		return NULL;
	}
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key)
{
	if(root)
	{
		if(strcmp(key,root->data)>0)
		{
			return _retrieve(root->right,key);
		}
		else if(strcmp(key,root->data)<0)
		{
			return _retrieve(root->left,key);
		}
		else
		{
			return root;
		}
	}
	else
	{
		return NULL;
	}
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree)
{
	_traverse(pTree->root);
	return;
}
static void _traverse( NODE *root)
{
	if(root)
	{
		_traverse(root->left);
		printf("%s ",root->data);
		_traverse(root->right);
	}
	else
	{
		return;
	}
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree)
{
	_infix_print(pTree->root,0);
}
/* internal traversal function
*/
static void _infix_print( NODE *root, int level)
{
	if(root==NULL)
	{	
		return;
	}
	level++;
	_infix_print(root->right,level);
	
	
	for(int i=1;i<level;++i)
	{
		printf("\t");
	}
	
	printf("%s",root->data);
	printf("\n");
	_infix_print(root->left,level);
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root)
{
	if(!root)
	{
		return 0;
	}
	return root->height;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root)
{	
	
	NODE* a;
	NODE* b;
	a=root->left;
	b=a->right;
	a->right=root;
	root->left=b;
	root->height=max(getHeight(root->left),getHeight(root->right))+1;
	a->height=max(getHeight(a->left),getHeight(a->right))+1;
	return a;
	
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root)
{	
	
	NODE* a;
	NODE* b;
	a=root->right;
	b=a->left;
	a->left=root;
	root->right=b;
	root->height=max(getHeight(root->left),getHeight(root->right))+1;
	a->height=max(getHeight(a->left),getHeight(a->right))+1;
	return a;
	
}
int balanceFactor(NODE *root)
{
	if(!root)
	{
		return 0;
	}
	return getHeight(root->left)-getHeight(root->right);
}

static NODE *balancing( NODE *root, NODE *newPtr)
{
	int b=balanceFactor(root);
	
	/*
	if((b>1)&&(strcmp(newPtr->data,root->left->data)<0))
	{
		return rotateRight(root);
	}
	else if((b<-1)&&(strcmp(newPtr->data,root->left->data)>0))
	{
		return rotateLeft(root);
	}
	else if((b>1)&&(strcmp(newPtr->data,root->left->data)>0))
	{
		root->left=rotateLeft(root->left);
		return rotateRight(root);
	}
	else if((b<-1)&&(strcmp(newPtr->data,root->left->data)<0))
	{
		root->right=rotateRight(root->right);
		return rotateLeft(root);
	}
	*/

	if((b>1)&&((newPtr->data)<(root->left->data)))
	{
		return rotateRight(root);
	}
	else if((b<-1)&&((newPtr->data)>(root->right->data)))
	{
		return rotateLeft(root);
	}
	else if((b>1)&&((newPtr->data)>(root->left->data)))
	{
		root->left=rotateLeft(root->left);
		return rotateRight(root);
	}
	else if((b<-1)&&((newPtr->data)<(root->right->data)))
	{
		root->right=rotateRight(root->right);
		return rotateLeft(root);
	}
	
	
	return root;
}
