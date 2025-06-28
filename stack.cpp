#include <graphics.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

//constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MAX_ITEMS = 10;
const int ITEM_WIDTH = 300;
const int ITEM_HEIGHT = 40;
const int STACK_X = 250;
const int STACK_Y = 150;

//colors
const int ITEM_COLORS[] = {LIGHTRED, LIGHTGREEN, LIGHTBLUE, YELLOW, LIGHTMAGENTA};

//stack data
int stackValues[10];
int stackCount = 0;

//check if stack is empty or full
bool isStackEmpty() 
{ return stackCount == 0; 
}
bool isStackFull() 
{ return stackCount == MAX_ITEMS; 
}

//basic stack operations
bool push(int value) 
{
    if (!isStackFull()) 
    {
        stackValues[stackCount++] = value;
        return true;
    }
    return false;
}

bool pop() 
{
    if (!isStackEmpty()) 
    {
        stackCount--;
        return true;
    }
    return false;
}

int peek() 
{
    return isStackEmpty() ? -1 : stackValues[stackCount - 1];
}

//func to convert int to string
string intToString(int num) 
{
    char buffer[20];
    sprintf(buffer, "%d", num);
    return string(buffer);
}

//draw text centered in a box
void drawCenteredText(int x, int y, int width, int height, string text) 
{
    char buffer[100];
    strncpy(buffer, text.c_str(), 99);
    buffer[99] = '\0';
    
    int textWidth = textwidth(buffer);
    int textHeight = textheight(buffer);
    int centerX = x + (width - textWidth) / 2;
    int centerY = y + (height - textHeight) / 2;
    
    outtextxy(centerX, centerY, buffer);
}

//to draw a single stack item
void drawStackItem(int x, int y, int value, int colorIndex) 
{
    int itemColor = ITEM_COLORS[colorIndex % 5];
    
    //to draw filled item
    setfillstyle(SOLID_FILL, itemColor);
    bar(x, y, x + ITEM_WIDTH, y + ITEM_HEIGHT);
    
    //to draw border
    setcolor(WHITE);
    rectangle(x, y, x + ITEM_WIDTH, y + ITEM_HEIGHT);
    
    //to draw value
    setcolor(WHITE);
    drawCenteredText(x, y, ITEM_WIDTH, ITEM_HEIGHT, intToString(value));
}

//to draw the entire stack
void drawStack(string message = "") 
{
    cleardevice();
    
    //to draw title
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    drawCenteredText(0, 20, SCREEN_WIDTH, 30, "STACK VISUALIZER");
    
    //to draw operations to be performed
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    outtextxy(50, 70, "Operations:");
    outtextxy(50, 90, "1.Press 'P' to push");
    outtextxy(50, 110, "2.Press 'O' to pop");
    outtextxy(50, 130, "3.Press 'Q' to quit");
    
    // to draw container
    int containerHeight = MAX_ITEMS * ITEM_HEIGHT;
    setcolor(LIGHTGRAY);
    line(STACK_X, STACK_Y, STACK_X, STACK_Y + containerHeight);
    line(STACK_X + ITEM_WIDTH, STACK_Y, STACK_X + ITEM_WIDTH, STACK_Y + containerHeight);
    line(STACK_X, STACK_Y + containerHeight, STACK_X + ITEM_WIDTH, STACK_Y + containerHeight);
    
    //to draw items
    for (int i = 0; i < stackCount; i++) 
    {
        int itemY = STACK_Y + containerHeight - (i + 1) * ITEM_HEIGHT;
        drawStackItem(STACK_X, itemY, stackValues[i], i);
    }
    
    //to draw status
    string status = "Items: " + intToString(stackCount) + "/" + intToString(MAX_ITEMS);
    if (isStackEmpty()) status += " (Empty)";
    if (isStackFull()) status += " (Full)";
    
    setcolor(WHITE);
    outtextxy(50, STACK_Y + containerHeight + 30, (char *)status.c_str());
    
    //for message display if any
    if (!message.empty()) 
    {
        outtextxy(50,  50, (char *) message.c_str());
    }
}

//function to get user input for push operation
int getUserInput() 
{
    string inputPrompt = "Enter value to push (1-999): ";
    string userInput = "";
    int value = 0;
    bool validInput = false;
    
    //draw the stack first without any message
    drawStack();
    
    //draw the operation title prominently
    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    outtextxy(350,50 , "PUSH OPERATION");
    
    //draw input prompt
    setcolor(WHITE);
    outtextxy(250, 75, (char*)inputPrompt.c_str());
    
    //input loop
    while (!validInput) 
    {   
        //display current input
        setcolor(WHITE);
        outtextxy(450,75, (char*)userInput.c_str());
        
        //wait for key press
        char key = getch();
        
        if (key == 13) //enter key
        {
            if (!userInput.empty()) 
            {
                value = atoi(userInput.c_str());
                if (value > 0 && value <= 999) 
                {
                    validInput = true;
                }
            }
        }
        else if (key == 8 || key == 127) //backspace
        {
            if (!userInput.empty()) 
            {
                userInput = userInput.substr(0, userInput.length() - 1);
            }
        }
        else if (key >= '0' && key <= '9' && userInput.length() < 3) //digit
        {
            userInput += key;
        }
        else if (key == 27) //esc key to cancel input
        {
            return -1; //return -1 to indicate cancelled input
        }
    }
    
    return value;
}

//push animation
void showPushAnimation(int value) 
{
    if (isStackFull()) 
    {
        drawStack("Can't push - Stack is full!");
        delay(1500);
        return;
    }
    
    int finalY = STACK_Y + (MAX_ITEMS - stackCount - 1) * ITEM_HEIGHT;
    
    for (int y = 0; y < finalY; y += 15) 
    {
        drawStack();
        drawStackItem(STACK_X, y, value, stackCount);
        delay(10);
    }
    
    push(value);
    drawStack("Pushed: " + intToString(value));
    delay(1000);
}

//pop animation
void showPopAnimation() 
{
    if (isStackEmpty()) 
    {
        drawStack("Can't pop - Stack is empty!");
        delay(1500);
        return;
    }
    
    int value = peek();
    int startY = STACK_Y + (MAX_ITEMS - stackCount) * ITEM_HEIGHT;
    
    pop();
    
    for (int y = startY; y >= -ITEM_HEIGHT; y -= 15) 
    {
        drawStack();
        drawStackItem(STACK_X, y, value, stackCount);
        delay(10);
    }
    
    drawStack("Popped: " + intToString(value));
    delay(1000);
}

int main() 
{
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Stack Visualizer - User Input");
    drawStack("Welcome! Press P to push your first item.");
    bool running = true;
    while (running) 
    {
        if (kbhit()) 
        {
            char key = toupper(getch());
            
            switch (key) 
            {
                case 'P':
                {
                    if (!isStackFull()) 
                    {
                        int userValue = getUserInput();
                        if (userValue != -1) //if input wasn't cancelled
                        {
                            showPushAnimation(userValue);
                        }
                        else 
                        {
                            drawStack("Push operation cancelled.");
                            delay(1000);
                        }
                    }
                    else 
                    {
                        drawStack("Can't push - Stack is full!");
                        delay(1500);
                    }
                    drawStack(); //redraw clean stack
                    break;
                }
                case 'O':
                    showPopAnimation();
                    drawStack(); //redraw clean stack
                    break;
                case 'Q':
                    running = false;
                    break;
            }
        }
        delay(10);
    }

    cleardevice();
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    drawCenteredText(0, SCREEN_HEIGHT/2 - 30, SCREEN_WIDTH, 60, "Thanks for using the Stack Visualizer!");
    delay(1500);
    closegraph();
    return 0;
}