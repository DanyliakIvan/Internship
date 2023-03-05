#include "terminal.h"


//head point to the begin of the list
list_t* head = NULL;



//wait for data from UART
void checkCommand(void)
{
	static char buf[100];
	static uint8_t counter = 0;
	int8_t ch;// = getchar();
	HAL_UART_Receive(&huart2, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
	//if data is not in queue -> return
	if (ch == EOF || ch == 24)
	{
		return;
	}
	//'r' means that transmit is completed
	else if (ch == '\r')
	{
		bufIntoTokens(buf);
		buf[counter++] = ch;
		counter = 0;
		//printf("%s\n", buf);
	}
	//write one character in array
	else if (ch >= 32)
	{
		buf[counter++] = ch;
	}
}

//pars string into array of tokens
void bufIntoTokens(char* buf)
{
	static char* tokensArr[100]; 
	uint8_t counter = 0;
	char* token;

	//separate in places where is SPACE
	token = strtok(buf, " ");
	
	while (token != NULL)
	{
		tokensArr[counter++] = token;
		token = strtok(NULL, " ");
	}
	
	tokensArr[counter] = NULL;
	
	checkForCommand(tokensArr);
}

//search a command among available commands
void checkForCommand(char ** tokensArr)
{
	//fill in the list (only in first starting)
	createListOfFuncs();
	list_t* current = head;

	//search command and call func
	while (current != NULL)
	{
		if(!strcmp(tokensArr[0], current->name))
		{
			(*current->test_ptr)(tokensArr[1][0], tokensArr[2][0]);
			return;
		}
		else
		{
			current = current->next;
		}
	}
	printf("command not found\n");
}


void createListOfFuncs()
{
	//check for functions
	if (head != NULL)
	{
		return;
	}
	
	//add another funcs
	listAddElement("add", &add);
	
	listAddElement("sub", &sub);
	
	listAddElement("mul", &mul);
	
	listAddElement("div", &divv);
}


void listAddElement(char* name, void (*ptrToFunc)(int8_t, int8_t))
{
	//add first element
	if (head == NULL)
	{	
		head = (list_t*) malloc(sizeof(list_t));
		head->name = name;
		head->test_ptr = ptrToFunc;
		head->next = NULL;
		return;
	}
	
	list_t* current = head;
	
	//search end of the list
	while (current->next != NULL)
	{
		current = current->next;
	}
	
	current->next = (list_t*) malloc(sizeof(list_t));
	current->next->name = name;
	current->next->test_ptr = ptrToFunc;
	current->next->next = NULL;
}


//test functions
void add(int8_t a, int8_t b)
{
	printf("add = %d\n", (a - 48) + (b - 48));
}

void sub(int8_t a, int8_t b)
{
	printf("sub = %d\n", (a - 48) - (b - 48));
}

void mul(int8_t a, int8_t b)
{
	printf("mul = %d\n", (a - 48) * (b - 48));
}

void divv(int8_t a, int8_t b)
{
	printf("div = %d\n", (a - 48) / (b - 48));
}









