#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

typedef struct node
{
    int data;
    struct node *left, *right;
    Color color;
} node;

node *newNode(int data)
{
    node *tmp = (node *)malloc(sizeof(node));

    tmp->data = data;
    tmp->left = tmp->right = NULL;
    tmp->color = WHITE;

    return tmp;
}

node *insert(node *root, int data)
{
    if (root == NULL)
        return newNode(data);

    if (data < root->data)
        root->left = insert(root->left, data);

    else if (data > root->data)
        root->right = insert(root->right, data);

    return root;
}

node *minNode(node *root)
{
    while (root->left != NULL)
        root = root->left;

    return root;
}

node* delete(node* root, int data) {
    if (root == NULL)
        return NULL;

    if (data < root->data)
        root->left = delete(root->left, data);

    else if (data > root->data)
        root->right = delete(root->right, data);

    else {
        if (root->left == NULL) {
            node* tmp = root->right;
            free(root);
            return tmp;

        } else if (root->right == NULL) {
            node* tmp = root->left;
            free(root);
            return tmp;
        }

        node* tmp = minNode(root->right);
        root->data = tmp->data;
        root->right = delete(root->right, tmp->data);
    }

    return root;
}

node *search(node *root, int data)
{
    if (root == NULL)
        return NULL;

    if (root->data == data)
    {
        root->color = RED;
        return root;
    }

    if (root->data > data)
        return search(root->left, data);

    return search(root->right, data);
}

void resetColors(node *root)
{
    if (root == NULL)
        return;

    root->color = WHITE;
    resetColors(root->left);
    resetColors(root->right);
}

void drawNode(int x, int y, int data, Color color)
{
    DrawRectangleRounded((Rectangle){x, y, 50, 50}, 0.25, 32, DARKBROWN);
    DrawText(TextFormat("%d", data), x + 10, y + 15, 20, color);
}

void drawArrow(Vector2 start, Vector2 end, Color color)
{
    DrawLineV(start, end, color);
    DrawTriangle(end, (Vector2){end.x - 10, end.y - 5}, (Vector2){end.x - 10, end.y + 5}, color);
}

