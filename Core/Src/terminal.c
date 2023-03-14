#include "terminal.h"


//points to the begin of the list
terminalList* terminalHead = NULL;

//private function prototypes
void bufIntoTokens(char* buf);
void lookForCommand(char ** tokensArr);
void terminalFuncsInit();
void terminalCommandNames(char ** tokensArr);

void add(char ** tokensArr);



//check the availability of data
void terminalCheckCommand(void)
{
    static char buf[MAX_BUF_SIZE];
    static uint8_t counter = 0;
    
    int8_t ch = getchar();
    
    //if queue is empty -> return
    if (ch == EOF)
    {
        return;
    }
    //'r' means that receiving is completed
    else if (ch == '\r')
    {
        buf[counter++] = '\0';
        counter = 0;
        
        printf("command = %s\n", buf);
        bufIntoTokens(buf);
    }
    //write reveiced character in the buf
    else if (isprint(ch))
    {
        buf[counter++] = ch;
    }
}

//pars the buf into tokens
void bufIntoTokens(char* buf)
{
    static char* tokensArr[MAX_BUF_SIZE]; 
    uint8_t counter = 0;
    char* token;

    //separate in places where is SPACE
    token = strtok(buf, " ");
    
    //add found token to the arr of bufs and find next token
    while (token != NULL)
    {
        tokensArr[counter++] = token;
        token = strtok(NULL, " ");
    }
    
    //last element of arr = NULL to stop in loops
    tokensArr[counter] = NULL;
    
    //check found command
    lookForCommand(tokensArr);
}

//look for a command among available commands
void lookForCommand(char ** tokensArr)
{
    //fill in the list (only on first start)
    terminalFuncsInit();
    
    terminalList* current = terminalHead;

    //look for a command and call func
    while (current != NULL)
    {
        if(!strcmp(tokensArr[0], current->name))
        {            
            (*current->funcPtr)(tokensArr);
            return;
        }
        else
        {
            current = current->next;
        }
    }
    
    //if the command was not found
    printf("command not found = %s\n", tokensArr[0]);
}


void terminalFuncsInit()
{
    //check for functions
    if (terminalHead != NULL)
    {
        return;
    }
    
    
    //item from other files
    ledTerminalInit();
    adxlTerminalInit();
    oledTerminalInit();
    
    
    //add other item
    static terminalList addItem =
    {
        .name = "add",
        .desc = "adds two one-figure numbers",
        .funcPtr = add,
        .next = NULL
    };
    terminalListAddItem(&addItem);
    
    static terminalList helpItem =
    {
        .name = "help",
        .desc = "prints all of availabe commands",
        .funcPtr = terminalCommandNames,
        .next = NULL
    };
    terminalListAddItem(&helpItem);
}


void terminalListAddItem(terminalList* newItem)
{
    //add first element
    if (terminalHead == NULL)
    {    
        terminalHead = newItem;
        return;
    }
    
    terminalList* current = terminalHead;
    
    //search the end of the list
    while (current->next != NULL)
    {
        current = current->next;
    }
    
    //add new item at the end of the list
    current->next = newItem;
}



//test function
void add(char ** tokensArr)
{
    printf("add = %d\n", atoi(tokensArr[1]) + atoi(tokensArr[2]));
}

//print all of commands and their descriptions
void terminalCommandNames(char ** tokensArr)
{
    terminalList* current = terminalHead;
    
    while (current != NULL)
    {
        //print the function's name with its description 
        printf("%s - %s\n", current->name, current->desc);
        current = current->next;
    }
}






