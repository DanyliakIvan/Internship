#include "terminal.h"


//head point to the begin of the list
terminalList* head = NULL;

//private function prototypes
void bufIntoTokens(char* buf);
void lookForCommand(char ** tokensArr);
void funcsInit();
void terminalCommandNames(char ** tokensArr);

void add(char ** tokensArr);




//check the availability of data
void terminalCheckCommand(void)
{
    static char buf[MAX_BUF_SIZE];
    static uint8_t counter = 0;
    
    int8_t ch = EOF; //getchar();
    HAL_UART_Receive(&huart2, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
    
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
    funcsInit();
    
    terminalList* current = head;

    //look for a command and call func
    while (current != NULL)
    {
        if(!strcmp(tokensArr[0], current->name))
        {
            (*current->test_ptr)(tokensArr);
            return;
        }
        else
        {
            current = current->next;
        }
    }
    
    //if the command was not found
    printf("command not found\n");
}


void funcsInit()
{
    //check for functions
    if (head != NULL)
    {
        return;
    }
    
    
    //item from other files
    adxlTerminalInit();
    ledTerminalInit();
    
    
    //add other item
    static terminalList addItem =
    {
        .name = "add",
        .desc = "adds two one-figure numbers",
        .test_ptr = add,
        .next = NULL
    };
    terminalListAddItem(&addItem);
    
    static terminalList helpItem =
    {
        .name = "help",
        .desc = "prints all of availabe commands",
        .test_ptr = terminalCommandNames,
        .next = NULL
    };
    terminalListAddItem(&helpItem);
}


void terminalListAddItem(terminalList* newItem)
{
    //add first element
    if (head == NULL)
    {    
        head = newItem;
        return;
    }
    
    terminalList* current = head;
    
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
    terminalList* current = head;
    
    while (current != NULL)
    {
        //print the function's name with its description 
        printf("%s - %s\n", current->name, current->desc);
        current = current->next;
    }
}






