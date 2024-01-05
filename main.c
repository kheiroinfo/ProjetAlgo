#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    Color color; // Color for highlighting nodes
} Node;

Node* newNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->color = WHITE; // Default color
    return node;
}

Node* insert(Node* node, int data) {
    if (node == NULL) return newNode(data);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);

    return node;
}

Node* search(Node* root, int key) {
    if (root == NULL || root->data == key) {
        if (root != NULL)
            root->color = RED; // Highlight the found node
        return root;
    }

    if (key < root->data)
        return search(root->left, key);

    return search(root->right, key);
}

void drawNode(int x, int y, int data, Color color) {
    DrawRectangle(x, y, 50, 50, DARKGRAY);  // Set node color to DARKGRAY
    DrawText(TextFormat("%d", data), x + 15, y + 20, 20, color);
}
void drawArrow(Vector2 start, Vector2 end, Color color) {
    DrawLineV(start, end, color);
    DrawTriangle(end, (Vector2){end.x - 10, end.y - 5}, (Vector2){end.x - 10, end.y + 5}, color);
}

void drawTree(Node* root, int x, int y, int horizontalSpacing) {
    if (root == NULL) return;

    drawNode(x, y, root->data, root->color);

    if (root->left) {
        drawArrow((Vector2){x + 25, y + 50}, (Vector2){x - horizontalSpacing + 25, y + 100}, DARKGRAY);
        drawTree(root->left, x - horizontalSpacing, y + 100, horizontalSpacing / 2);
    }

    if (root->right) {
        drawArrow((Vector2){x + 25, y + 50}, (Vector2){x + horizontalSpacing + 25, y + 100}, DARKGRAY);
        drawTree(root->right, x + horizontalSpacing, y + 100, horizontalSpacing / 2);
    }
}