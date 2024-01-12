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

int isMouseOverButton(int x, int y, int width, int height) {
    Vector2 mouse = GetMousePosition();
    return (mouse.x >= x && mouse.x <= x + width && mouse.y >= y && mouse.y <= y + height);
}

Node* deleteNode(Node* root, int key) {
    if (root == NULL) return root;

    if (key < root->data)
        root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = root->right;
        while (temp->left != NULL)
            temp = temp->left;

        root->data = temp->data; // Update the data instead of copying
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}
int main() {
    InitWindow(1400, 650, "Binary Tree - Animation");

    Node* root = NULL;
    int addValue;
    int deleteValue;
    int searchValue;
    int addTextBoxActive = 0;
    int deleteTextBoxActive = 0;
    int searchTextBoxActive = 0;
    char addInputText[50] = "\0";
    char deleteInputText[50] = "\0";
    char searchInputText[50] = "\0";

    while (!WindowShouldClose()) {
        // Check for key input to add node
        if (IsKeyPressed(KEY_ENTER)) {
            // Check if add text box is active
            if (addTextBoxActive) {
                sscanf(addInputText, "%d", &addValue);
                root = insert(root, addValue);
                // Reset add input text and disable add text box
                addInputText[0] = '\0';
                addTextBoxActive = 0;
            }
        }

        // Check if mouse is over the "ajouter" button and left mouse button is pressed
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isMouseOverButton(0, 0, 100, 50)) {
            // Enable add text box for input
            addTextBoxActive = 1;
            deleteTextBoxActive = 0; // Disable delete text box
            searchTextBoxActive = 0; // Disable search text box
        }

        // Check for key input to delete node
        if (IsKeyPressed(KEY_ENTER)) {
            // Check if delete text box is active
            if (deleteTextBoxActive) {
                sscanf(deleteInputText, "%d", &deleteValue);
                root = deleteNode(root, deleteValue);
                // Reset delete input text and disable delete text box
                deleteInputText[0] = '\0';
                deleteTextBoxActive = 0;
            }
        }

        // Check if mouse is over the "supprimer" button and left mouse button is pressed
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isMouseOverButton(0, 60, 100, 50)) {
            // Enable delete text box for input
            deleteTextBoxActive = 1;
            addTextBoxActive = 0; // Disable add text box
            searchTextBoxActive = 0; // Disable search text box

        }

        // Check for key input to search node
        if (IsKeyPressed(KEY_ENTER)) {
            // Check if search text box is active
            if (searchTextBoxActive) {
                sscanf(searchInputText, "%d", &searchValue);
                search(root, searchValue);
                // Reset search input text and disable search text box
                searchInputText[0] = '\0';
                searchTextBoxActive = 0;
            }
        }

        // Check if mouse is over the "rechercher" button and left mouse button is pressed
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isMouseOverButton(0, 120, 100, 50)) {
            // Enable search text box for input
            searchTextBoxActive = 1;
            addTextBoxActive = 0; // Disable add text box
            deleteTextBoxActive = 0; // Disable delete text box
        }

        // Update the add text box input
        if (addTextBoxActive) {
            int key = GetKeyPressed();
            if ((key >= 32) && (key <= 125) && (strlen(addInputText) < 49)) {
                addInputText[strlen(addInputText)] = (char)key;
                addInputText[strlen(addInputText) + 1] = '\0';
            } else if (key == KEY_BACKSPACE) {
                if (strlen(addInputText) > 0) {
                    addInputText[strlen(addInputText) - 1] = '\0';
                }
            }
        }

        // Update the delete text box input
        if (deleteTextBoxActive) {
            int key = GetKeyPressed();
            if ((key >= 32) && (key <= 125) && (strlen(deleteInputText) < 49)) {
                deleteInputText[strlen(deleteInputText)] = (char)key;
                deleteInputText[strlen(deleteInputText) + 1] = '\0';
            } else if (key == KEY_BACKSPACE) {
                if (strlen(deleteInputText) > 0) {
                    deleteInputText[strlen(deleteInputText) - 1] = '\0';
                }
            }
        }
            
        // Update the search text box input
        if (searchTextBoxActive) {
            int key = GetKeyPressed();
            if ((key >= 32) && (key <= 125) && (strlen(searchInputText) < 49)) {
                searchInputText[strlen(searchInputText)] = (char)key;
                searchInputText[strlen(searchInputText) + 1] = '\0';
            } else if (key == KEY_BACKSPACE) {
                if (strlen(searchInputText) > 0) {
                    searchInputText[strlen(searchInputText) - 1] = '\0';
                }
            }
        }

        // Draw the tree on the window
        BeginDrawing();
        ClearBackground(WHITE);

        // Draw the "ajouter" button
        DrawRectangle(0, 0, 100, 50, LIGHTGRAY);
        DrawText("ajouter", 10, 15, 20, BLACK);

        // Draw the "supprimer" button
        DrawRectangle(0, 60, 100, 50, LIGHTGRAY);
        DrawText("supprimer", 10, 75, 20, BLACK);

        // Draw the "rechercher" button
        DrawRectangle(0, 120, 100, 50, LIGHTGRAY);
        DrawText("rechercher", 10, 135, 20, BLACK);

        // Draw the add text box
        if (addTextBoxActive) {
            DrawRectangle(110, 0, 150, 50, LIGHTGRAY);
            DrawRectangleLines(110, 0, 150, 50, DARKGRAY);
            DrawText(addInputText, 120, 12, 20, MAROON);
            if ((GetTime() - 1) < 0.5f)
                DrawText("_", 120 + MeasureText(addInputText, 20), 12, 20, MAROON);
        }

        // Draw the delete text box
        if (deleteTextBoxActive) {
            DrawRectangle(110, 60, 150, 50, LIGHTGRAY);
            DrawRectangleLines(110, 60, 150, 50, DARKGRAY);
            DrawText(deleteInputText, 120, 72, 20, MAROON);
            if ((GetTime() - 1) < 0.5f)
                DrawText("_", 120 + MeasureText(deleteInputText, 20), 72, 20, MAROON);
        }

        // Draw the search text box
        if (searchTextBoxActive) {
            DrawRectangle(110, 120, 150, 50, LIGHTGRAY);
            DrawRectangleLines(110, 120, 150, 50, DARKGRAY);
            DrawText(searchInputText, 120, 132, 20, MAROON);
            if ((GetTime() - 1) < 0.5f)
                DrawText("_", 120 + MeasureText(searchInputText, 20), 132, 20, MAROON);
        }

        // Draw the tree on the window
        drawTree(root, GetScreenWidth() / 2, 80, 200);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

