
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(int value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

long long treeSum(Node *root) {
    if (root == NULL)
        return 0;

    long long leftSum = 0;
    long long rightSum = 0;

    Node *left = root->left;
    Node *right = root->right;

    #pragma omp task shared(leftSum) firstprivate(left)
    leftSum = treeSum(left);

    #pragma omp task shared(rightSum) firstprivate(right)
    rightSum = treeSum(right);

    #pragma omp taskwait

    return root->value + leftSum + rightSum;
}

void freeTree(Node *root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void) {
    Node *root = createNode(10);
    root->left = createNode(5);
    root->right = createNode(15);
    root->left->left = createNode(2);
    root->left->right = createNode(3);
    root->right->left = createNode(7);
    root->right->right = createNode(8);

    long long total;

    #pragma omp parallel
    {
        #pragma omp single
        total = treeSum(root);
    }

    printf("Sum of all tree nodes = %lld\n", total);

    freeTree(root);
    return 0;
}
