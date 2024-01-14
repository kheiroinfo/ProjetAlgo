#include <stdio.h>  // Standard input-output functions
#include <stdlib.h> // Standard library functions, including memory allocation
#include <string.h> // String manipulation functions
#include <raylib.h> // Raylib library for graphical functions

// Definition of a structure representing a node in a binary tree.
typedef struct node
{
    int data;                  // Integer data stored in the node
    struct node *left, *right; // Pointers to the left and right child nodes
    Color color;               // Color variable for additional information
} node;

// Function to create a new binary tree node with the given data.
node *newNode(int data)
{
    // Allocate memory for a new node
    node *tmp = (node *)malloc(sizeof(node));

    // Initialize the fields of the new node
    tmp->data = data;              // Set data
    tmp->left = tmp->right = NULL; // Set left and right child pointers to NULL
    tmp->color = WHITE;            // Set color to WHITE

    // Return a pointer to the newly created node
    return tmp;
}

// Function to insert a new node with the given data into a binary search tree.
node *insert(node *root, int data)
{
    // If the tree is empty, create a new node with the given data
    if (root == NULL)
        return newNode(data);

    // If the data is less than the current node's data, insert into the left subtree
    if (data < root->data)
        root->left = insert(root->left, data);

    // If the data is greater than the current node's data, insert into the right subtree
    else if (data > root->data)
        root->right = insert(root->right, data);

    // If the data is equal, do not allow duplicate entries (assuming uniqueness)
    // Return a pointer to the root of the modified tree after insertion
    return root;
}

// Function to find the node with the minimum value in a binary search tree.
node *minNode(node *root)
{
    // Traverse the left subtree until the leftmost node is reached
    while (root->left != NULL)
        root = root->left;

    // Return a pointer to the node with the minimum value
    return root;
}

// Function to delete a node with the given data from a binary search tree.
node *delete(node *root, int data)
{
    // If the tree is empty, return NULL
    if (root == NULL)
        return NULL;

    // If the data is less than the current node's data, delete from the left subtree
    if (data < root->data)
        root->left = delete (root->left, data);

    // If the data is greater than the current node's data, delete from the right subtree
    else if (data > root->data)
        root->right = delete (root->right, data);

    else
    {
        // If the node to be deleted has only one child or no child
        if (root->left == NULL)
        {
            node *tmp = root->right;
            free(root);
            return tmp;
        }
        else if (root->right == NULL)
        {
            node *tmp = root->left;
            free(root);
            return tmp;
        }

        // If the node to be deleted has two children
        node *tmp = minNode(root->right);
        root->data = tmp->data;
        root->right = delete (root->right, tmp->data);
    }

    // Return a pointer to the root of the modified tree after deletion
    return root;
}

// Function to search for a node with the given data in a binary search tree.
node *search(node *root, int data)
{
    // If the tree is empty or the current node is NULL, return NULL (data not found)
    if (root == NULL)
        return NULL;

    // If the data is found in the current node, set its color to RED and return the node
    if (root->data == data)
    {
        root->color = RED;
        return root;
    }

    // If the data is less than the current node's data, search in the left subtree
    if (root->data > data)
        return search(root->left, data);

    // If the data is greater than the current node's data, search in the right subtree
    return search(root->right, data);
}

// Function to reset the color of all nodes in a binary search tree to WHITE.
void resetColors(node *root)
{
    // If the current node is NULL, return (base case for recursion)
    if (root == NULL)
        return;

    // Set the color of the current node to WHITE
    root->color = WHITE;

    // Recursively reset colors for the left and right subtrees
    resetColors(root->left);
    resetColors(root->right);
}

// Function to draw a graphical representation of a node in a binary search tree.
void drawNode(int x, int y, int data, Color color)
{
    // Draw a rounded rectangle representing the node with specified color
    DrawRectangleRounded((Rectangle){x, y, 50, 50}, 0.25, 32, DARKBROWN);

    // Display the integer data inside the node's rectangle using the specified color
    DrawText(TextFormat("%d", data), x + 10, y + 15, 20, color);
}

// Function to draw an arrow connecting two points.
void drawArrow(Vector2 start, Vector2 end, Color color)
{
    // Draw a line connecting the start and end points with the specified color
    DrawLineV(start, end, color);

    // Draw a triangular shape at the end to represent the arrowhead
    DrawTriangle(end, (Vector2){end.x - 10, end.y - 5}, (Vector2){end.x - 10, end.y + 5}, color);
}

// Function to draw a binary search tree on the screen with graphical representation.
void drawTree(node *root, int x, int y, int horizontalSpacing)
{
    // If the current node is NULL, return (base case for recursion)
    if (root == NULL)
        return;

    // Draw the current node
    drawNode(x, y, root->data, root->color);

    // If the left child exists, draw an arrow and recursively draw the left subtree
    if (root->left)
    {
        drawArrow((Vector2){x + 25, y + 50}, (Vector2){x - horizontalSpacing + 25, y + 100}, DARKBROWN);
        drawTree(root->left, x - horizontalSpacing, y + 100, horizontalSpacing / 1.75);
    }

    // If the right child exists, draw an arrow and recursively draw the right subtree
    if (root->right)
    {
        drawArrow((Vector2){x + 25, y + 50}, (Vector2){x + horizontalSpacing + 25, y + 100}, DARKBROWN);
        drawTree(root->right, x + horizontalSpacing, y + 100, horizontalSpacing / 1.75);
    }
}

// Function to draw a title box with text in the center of the screen.
void drawTitle(Rectangle titleBox, char title[])
{
    // Draw a rounded rectangle as the title box with specified dimensions and position
    DrawRectangleRounded(titleBox, 0.25, 32, DARKBROWN);

    // Display the specified text in the center of the title box with a white font color
    DrawText(title, (GetScreenWidth() / 2) - (MeasureText(title, 30) / 2), 40, 30, WHITE);
}

// Function to adjust the brightness of a given color.
Color adjustBrightness(Color baseColor, float factor)
{
    // Adjust the brightness of each component of the color using the specified factor
    return (Color){baseColor.r * factor, baseColor.g * factor, baseColor.b * factor, baseColor.a};
}

// Function to draw a rounded rectangle button with text and handle mouse interactions.
bool drawButton(Rectangle button, Color buttonColor, char buttonText[])
{
    // Draw a rounded rectangle as the button with specified dimensions, position, and color
    DrawRectangleRounded(button, 0.25, 32, buttonColor);

    // Display the specified text on the button in the center
    DrawText(buttonText, button.x + 75 - (MeasureText(buttonText, 20) / 2), button.y + 10, 20, WHITE);

    // If the mouse is over the button, increase its brightness
    if (CheckCollisionPointRec(GetMousePosition(), button))
    {
        DrawRectangleRounded(button, 0.25, 32, adjustBrightness(buttonColor, 1.2));
        DrawText(buttonText, button.x + 75 - (MeasureText(buttonText, 20) / 2), button.y + 10, 20, WHITE);
    }

    // If the mouse is over the button and the left mouse button is pressed, decrease brightness
    if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        DrawRectangleRounded(button, 0.25, 32, adjustBrightness(buttonColor, 0.8));
        DrawText(buttonText, button.x + 75 - (MeasureText(buttonText, 20) / 2), button.y + 10, 20, WHITE);

        // Return true to indicate that the button was clicked
        return true;
    }

    // Return false if the button was not clicked
    return false;
}

// Function to draw a rounded rectangle textbox with text and handle keyboard input.
void drawTextBox(Rectangle textBox, char inputText[], Color textColor)
{
    // Draw a rounded rectangle as the textbox with specified dimensions, position, and color
    DrawRectangleRounded(textBox, 0.25, 32, LIGHTGRAY);

    // Display the specified text inside the textbox
    DrawText(inputText, textBox.x + 10, textBox.y + 10, 20, textColor);

    // Simulate a cursor by alternating the display of a vertical bar
    if (((int)(GetTime() / 0.5f)) % 2 == 0)
    {
        DrawText("|", textBox.x + 11 + MeasureText(inputText, 20), textBox.y + 10, 20, DARKGRAY);
    }

    // Get the key that was pressed
    int key = GetKeyPressed();

    // Handle keyboard input
    switch (key)
    {
    case KEY_BACKSPACE:
        // If there are more than one character, remove the last character
        if (strlen(inputText) > 1)
        {
            inputText[strlen(inputText) - 1] = '\0';
        }
        // If there's only one character, set it to '0'
        else
        {
            inputText[0] = '0';
        }
        break;
    case '-':
        // If the first character is '0', replace it with '-'
        if (inputText[0] == '0')
        {
            inputText[0] = '-';
        }
        break;
    default:
        // If the key is a numeric character and there's room for more characters
        if ((key >= '0' && key <= '9') && strlen(inputText) < 10)
        {
            // If the first character is '0', replace it with the pressed key
            if (inputText[0] == '0')
            {
                inputText[0] = key;
            }
            else if (strlen(inputText) == 1 && inputText[0] == '-' && key == '0')
                break;
            // If the first character is not '0', append the pressed key to the string
            else
            {
                inputText[strlen(inputText)] = key;
            }
        }
        break;
    }
}

// Main function of the program for visualizing a binary search tree.
int main()
{
    // Initialize the Raylib window, enable fullscreen, and set target frames per second
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Visualisation d'une arbre binaire de recherche");
    ToggleFullscreen();
    SetTargetFPS(60);

    // Initialize the root of the binary search tree to NULL
    node *root = NULL;

    // Set up the title box for displaying the program title
    char title[47] = "Visualisation d'une arbre binaire de recherche";
    Rectangle titleBox = {(GetScreenWidth() / 2) - (MeasureText(title, 30) / 2) - 5, 35, MeasureText(title, 30) + 10, 40};

    // Set up buttons for adding, deleting, and searching nodes in the tree
    Rectangle ajouterButton = {(GetScreenWidth() / 2) - 245, GetScreenHeight() - 60, 150, 40};
    Rectangle supprimerButton = {(GetScreenWidth() / 2) - 75, GetScreenHeight() - 60, 150, 40};
    Rectangle rechercherButton = {(GetScreenWidth() / 2) + 95, GetScreenHeight() - 60, 150, 40};

    // Initialize variables to track text box activation status
    int addTextBoxActive = 0;
    int deleteTextBoxActive = 0;
    int searchTextBoxActive = 0;

    // Set up text boxes for entering values to add, delete, or search
    Rectangle addTextBox = {(GetScreenWidth() / 2) - 245, GetScreenHeight() - 110, 150, 40};
    Rectangle deleteTextBox = {(GetScreenWidth() / 2) - 75, GetScreenHeight() - 110, 150, 40};
    Rectangle searchTextBox = {(GetScreenWidth() / 2) + 95, GetScreenHeight() - 110, 150, 40};

    // Initialize input text buffers for text boxes
    char addInputText[10] = "0";
    char deleteInputText[10] = "0";
    char searchInputText[10] = "0";

    // Initialize variables to store numeric values entered in text boxes
    int addValue = 0;
    int deleteValue = 0;
    int searchValue = 0;

    // Main loop for visualizing a binary search tree.
    while (!WindowShouldClose())
    {
        // Begin drawing and clear the background
        BeginDrawing();
        ClearBackground(WHITE);

        // Draw the title box with the program title
        drawTitle(titleBox, title);

        // Handle button clicks and activate/deactivate text boxes accordingly
        if (drawButton(ajouterButton, DARKGREEN, "Ajouter"))
        {
            addTextBoxActive = 1;
            deleteTextBoxActive = 0;
            searchTextBoxActive = 0;
        }

        if (drawButton(supprimerButton, MAROON, "Supprimmer"))
        {
            addTextBoxActive = 0;
            deleteTextBoxActive = 1;
            searchTextBoxActive = 0;
        }

        if (drawButton(rechercherButton, DARKBLUE, "Rechercher"))
        {
            addTextBoxActive = 0;
            deleteTextBoxActive = 0;
            searchTextBoxActive = 1;
        }

        // Handle user input and update the tree for adding nodes
        if (addTextBoxActive)
        {
            // Draw the add text box and handle Enter key press
            drawTextBox(addTextBox, addInputText, DARKGREEN);

            if (IsKeyPressed(KEY_ENTER))
            {
                // Convert input text to integer and insert into the tree
                addValue = atoi(addInputText);
                root = insert(root, addValue);

                // Reset node colors in the tree
                resetColors(root);

                // Reset input text buffer
                addInputText[0] = '0';
                for (int i = 1; i < 10; i++)
                {
                    addInputText[i] = '\0';
                }
            }
        }

        // Handle user input and update the tree for deleting nodes
        if (deleteTextBoxActive)
        {
            // Draw the delete text box and handle Enter key press
            drawTextBox(deleteTextBox, deleteInputText, MAROON);

            if (IsKeyPressed(KEY_ENTER))
            {
                // Convert input text to integer and delete from the tree
                deleteValue = atoi(deleteInputText);
                root = delete (root, deleteValue);

                // Reset node colors in the tree
                resetColors(root);

                // Reset input text buffer
                deleteInputText[0] = '0';
                for (int i = 1; i < 10; i++)
                {
                    deleteInputText[i] = '\0';
                }
            }
        }

        // Handle user input and update the tree for searching nodes
        if (searchTextBoxActive)
        {
            // Draw the search text box and handle Enter key press
            drawTextBox(searchTextBox, searchInputText, DARKBLUE);

            if (IsKeyPressed(KEY_ENTER))
            {
                // Convert input text to integer and search in the tree
                searchValue = atoi(searchInputText);

                // Reset node colors in the tree and highlight the searched node
                resetColors(root);
                search(root, searchValue);

                // Reset input text buffer
                searchInputText[0] = '0';
                for (int i = 1; i < 10; i++)
                {
                    searchInputText[i] = '\0';
                }
            }
        }

        // Draw the binary search tree
        drawTree(root, (GetScreenWidth() / 2) - 25, 100, 250);

        // End drawing
        EndDrawing();
    }

    // Close the window and return
    CloseWindow();
    return 0;
}