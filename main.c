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

void drawTree(node *root, int x, int y, int horizontalSpacing)
{
    if (root == NULL)
        return;

    drawNode(x, y, root->data, root->color);

    if (root->left)
    {
        drawArrow((Vector2){x + 25, y + 50}, (Vector2){x - horizontalSpacing + 25, y + 100}, DARKBROWN);
        drawTree(root->left, x - horizontalSpacing, y + 100, horizontalSpacing / 1.75);
    }

    if (root->right)
    {
        drawArrow((Vector2){x + 25, y + 50}, (Vector2){x + horizontalSpacing + 25, y + 100}, DARKBROWN);
        drawTree(root->right, x + horizontalSpacing, y + 100, horizontalSpacing / 1.75);
    }
}

bool isMouseOverButton(int x, int y, int width, int height)
{
    return (GetMousePosition().x >= x && GetMousePosition().x <= x + width && GetMousePosition().y >= y && GetMousePosition().y <= y + height);
}

void drawTitle(Rectangle titleBox, char title[])
{
    DrawRectangleRounded(titleBox, 0.25, 32, DARKBROWN);
    DrawText(title, (GetScreenWidth() / 2) - (MeasureText(title, 30) / 2), 40, 30, WHITE);
}

Color adjustBrightness(Color baseColor, float factor)
{
    return (Color){baseColor.r * factor, baseColor.g * factor, baseColor.b * factor, baseColor.a};
}

bool drawButton(Rectangle button, Color buttonColor, char buttonText[])
{
    DrawRectangleRounded(button, 0.25, 32, buttonColor);
    DrawText(buttonText, button.x + 75 - (MeasureText(buttonText, 20) / 2), button.y + 10, 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), button))
    {
        DrawRectangleRounded(button, 0.25, 32, adjustBrightness(buttonColor, 1.2));
        DrawText(buttonText, button.x + 75 - (MeasureText(buttonText, 20) / 2), button.y + 10, 20, WHITE);
    }

    if (CheckCollisionPointRec(GetMousePosition(), button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        DrawRectangleRounded(button, 0.25, 32, adjustBrightness(buttonColor, 0.8));
        DrawText(buttonText, button.x + 75 - (MeasureText(buttonText, 20) / 2), button.y + 10, 20, WHITE);

        return true;
    }

    return false;
}

void drawTextBox(Rectangle textBox, char inputText[], Color textColor)
{
    DrawRectangleRounded(textBox, 0.25, 32, LIGHTGRAY);
    DrawText(inputText, textBox.x + 10, textBox.y + 10, 20, textColor);

    if (((int)(GetTime() / 0.5f)) % 2 == 0)
    {
        DrawText("|", textBox.x + 11 + MeasureText(inputText, 20), textBox.y + 10, 20, DARKGRAY);
    }

    int key = GetKeyPressed();

    switch (key)
    {
    case KEY_BACKSPACE:
        if (strlen(inputText) > 1)
        {
            inputText[strlen(inputText) - 1] = '\0';
        }
        else
        {
            inputText[0] = '0';
        }
        break;
    case '-':
        if (inputText[0] == '0')
        {
            inputText[0] = '-';
        }
        break;
    default:
        if ((key >= '0' && key <= '9') && strlen(inputText) < 10)
        {
            if (inputText[0] == '0')
            {
                inputText[0] = key;
            }
            else
            {
                inputText[strlen(inputText)] = key;
            }
        }
        break;
    }
}