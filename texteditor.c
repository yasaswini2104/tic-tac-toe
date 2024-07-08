//------------------------------------------TEXT EDITOR---------------------------------------

//-----------------Using doubly linked list------------------
//stacks are used in undo and redo functions

// The standard library header files are used in the following code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{  
    // Structure that represents a node in a doubly linked list
    char data;
    struct node* prev;
    struct node* next;
} node;


// Function to create a new node
node* createNode(char ch)
{
    node* newNode = (node*)malloc(sizeof(node));
    // Memory is allocated dynamically in the heap memory
    newNode->prev = NULL;
    newNode->data = ch;
    newNode->next = NULL;
    return newNode;
}


// Global declaration of undo and redo stack
node* undoStack = NULL;
node* redoStack = NULL;


// Structure definition for a text editor
typedef struct textEditor
{
    // head and current are the pointers to the text editor 
    node* head;
    node* current;
} textEditor;


// Function to create a text editor
textEditor* createTextEditor()
{
    // Allocating memory for the Editor
    textEditor* editor = (textEditor*)malloc(sizeof(textEditor));
    editor->head = NULL;
    editor->current = NULL;
    // Initializing the pointers of the editor to NULL
    return editor;
}


// Function to push the elements into the stack
void push(node** top, char ch)
{
    // Create a new node with the given character
    node* newNode = createNode(ch);
    
    if (*top == NULL)
    {
        // If the stack is empty, make the new node the top of the stack
        (*top) = newNode;
    }
    else
    {
        // If the stack is not empty, make the new node the new top of the stack by updating the pointers
        newNode->next = (*top);
        newNode->next->prev = newNode;
        (*top) = newNode;
    }
}


// Function to insert the elements in the editor
void insertch(textEditor* editor, char ch)
{
    // Push the character onto the undo stack
    push(&undoStack, ch);
    
    if (editor->current == NULL)
    {
        // If the editor is empty, create a new node and make it the head and current node
        node* newNode = createNode(ch);
        editor->head = newNode;
        editor->current = newNode;
    }
    else
    {
        // If the editor is not empty, insert the character at the current position
        node* newNode = createNode(ch);
        newNode->prev = editor->current;
        newNode->next = editor->current->next;
        
        if (editor->current->next != NULL)
        {
            editor->current->next->prev = newNode;
        }
        
        editor->current->next = newNode;
        editor->current = newNode;
    }
}


// Function to open a file and load its content
void open(textEditor* editor, const char* filename)
{
    // Opening a file in read mode
    FILE* file = fopen(filename, "r");
    
    // If the file pointer is not pointing to any filename
    if (file == NULL)
    {
        printf("Error in opening the file!\n");
        return;
    }
    
    // If the file pointer is pointing to the specified filename, then the following iteration takes place
    char ch;
    while ((ch = fgetc(file)) != EOF) // EOF = End Of File
    {
        node* newNode = createNode(ch);
        // Creates a new node to store the given data
        insertch(editor, ch);
    }
    
    fclose(file);
    printf("File opened successfully\n");
}


// Function to save the content of the text editor into a file
void save(textEditor* editor, const char* filename)
{
    // Opening a file in write mode
    FILE* file = fopen(filename, "w");
    
    if (file == NULL)
    {
        printf("Error: Could not save the file.\n");
        return;
    }
    
    // To traverse the data of the editor, we declare and initialize it with the editor head
    node* current = editor->head;
    
    // Writing the content given into the file to save the content
    while (current != NULL)
    {
        fputc(current->data, file);
        current = current->next;
    }
    
    fclose(file);
    printf("File saved successfully.\n");
}


// Function to pop the elements from the stack
char pop(node** top)
{
    // Check if the stack is empty
    if (*top == NULL)
    {
        printf("Stack is Empty!\n");
        return '\0';
    }
    else
    {
        // Retrieve the character value from the top node
        char ch = (*top)->data;
        node* temp = (*top);
        
        // Update the top of the stack to the next node
        (*top) = (*top)->next;
        
        if ((*top) != NULL)
        {
            (*top)->prev = NULL;
        }
        
        // Free the memory of the removed node
        free(temp);
        
        // Return the popped character
        return ch;
    }
}


// Function to delete the character from the editor
void deletech(textEditor* editor)
{
    if (editor->current == NULL)
    {
        printf("There are no characters to delete!\n");
        return;
    }
    
    // Push the character onto the undo stack
    push(&undoStack, editor->current->data);
    
    // Create a temporary node pointer to hold the current node
    node* temp = editor->current;
    
    if (editor->current->prev != NULL)
    {
        editor->current->prev->next = editor->current->next;
    }
    else
    {
        editor->head = editor->current->next;
    }
    
    if (editor->current->next != NULL)
    {
        editor->current->next->prev = editor->current->prev;
    }
    
    editor->current = editor->current->prev;
    
    // Free the memory of the removed node
    free(temp);
}


// Function to insert a sentence at the current position in the text
void writeSentence(textEditor* editor, const char* sentence)
{
    // Finding the length of the sentence by using the string function called strlen
    int length = strlen(sentence);
    int i;
    for ( i = 0; i < length; i++)
    {
        insertch(editor, sentence[i]);
    }
}


// Function to undo the previous action
void undo(textEditor* editor)
{
    if (undoStack == NULL)
    {
        printf("Empty!\n");
        return;
    }
    
    // Pop the character from the undo stack
    char ch = pop(&undoStack);
    
    // Push the character onto the redo stack
    push(&redoStack, ch);
    
    // Delete the character from the editor
    deletech(editor);
}


// Function to restore the previous undo action
void redo(textEditor* editor)
{
    if (redoStack == NULL)
    {
        printf("Redo stack is empty!\n");
        return;
    }
    
    // Pop the character from the redo stack
    char ch = pop(&redoStack);
    
    // Push the character onto the undo stack
    push(&undoStack, ch);
    
    // Insert the character into the editor
    insertch(editor, ch);
}


// Printing all the elements in the editor by traversing
void print(textEditor* editor)
{
    // Check if the editor is empty
    if (editor->head == NULL)
    {
        printf("File is empty!\n");
        return;
    }
    
    // Create a temporary node pointer to traverse the linked list
    node* temp = editor->head;
    
    // Traverse the linked list and print each character
    while (temp != NULL)
    {
        printf("%c", temp->data);
        temp = temp->next;
    }
    
    printf("\n");
}


// Function to clear the text editor
void clear(textEditor* editor)
{
    node* current = editor->head;
    
    while (current != NULL)
    {
        node* temp = current;
        current = current->next;
        free(temp);
    }
    
    editor->head = NULL;
    editor->current = NULL;
}


int main()
{
    int choice;
    char ch, filename[100], sentence[1000];
    textEditor* editor = NULL;
    
    do
    {
    	//the following options will be printed on the screen
        printf("Text Editor Menu:\n"
               "1. Create a new file\n"
               "2. Open a file\n"
               "3. Save to file\n"
               "4. Write a sentence\n"
               "5. Undo\n"
               "6. Redo\n"
               "7. Clear the content\n"
               "8. Print the text in editor\n"
               "0. Exit\n");
               
        //choice to be entered
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice)
        {
            case 1:
                editor = createTextEditor();
                printf("File has been created\n");
                break;
                
            case 2:
                printf("Enter the filename to open: ");
                scanf("%s", filename);
                open(editor, filename);
                break;
                
            case 3:
                printf("Enter the filename to save: ");
                scanf("%s", filename);
                save(editor, filename);
                break;
                
            case 4:
                printf("Enter the sentence: ");
                scanf(" %[^\n]s", sentence);
                writeSentence(editor, sentence);
                break;
                
            case 5:
                undo(editor);
                printf("Undo performed\n");
                break;
                
            case 6:
                redo(editor);
                printf("Redo performed\n");
                break;
                
            case 7:
                clear(editor);
                printf("Content cleared\n");
                break;
                
            case 8:
                print(editor);
                break;
                
            case 0:
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid choice!\n");
                break;
        }
        printf("\n");
    } while (choice!=0);
    return 0;
    
}

