#include "terminal.h"


typedef struct list
{
	char* name; //command name
	void (*test_ptr)(char **);// pointer to a function
	struct list* next; // poinet to next element of the list
} list_t;


//head point to the begin of the list
list_t* head = NULL;

//prototypes
void bufIntoTokens(char* buf);
void checkForCommand(char ** tokensArr);
void funcsInit();
void listAddItem(list_t* newItem);
void add(char ** tokensArr);
void sub(char ** tokensArr);
void mul(char ** tokensArr);
void divv(char ** tokensArr);
void commandNames(char ** tokensArr);




//wait for data from UART
void checkCommand(void)
{
	static char buf[128];
	static uint8_t counter = 0;
	
	int8_t ch = getchar();
	//HAL_UART_Receive(&huart2, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
	
	//if data is not in queue -> return
	if (ch == EOF)
	{
		return;
	}
	//'r' means that transmit is completed
	else if (ch == '\r')
	{
		buf[counter++] = '\0';
		counter = 0;
		bufIntoTokens(buf);
	}
	//write one character in array
	else if (isprint(ch))
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
	
	//last element of arr = NULL to stop in loops
	tokensArr[counter] = NULL;
	
	checkForCommand(tokensArr);
}

//search a command among available commands
void checkForCommand(char ** tokensArr)
{
	//fill in the list (only in first starting)
	funcsInit();
	
	list_t* current = head;

	//search command and call func
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
	printf("command not found\n");
}


void funcsInit()
{
	//check for functions
	if (head != NULL)
	{
		return;
	}
	
	//add another funcs
	static list_t addItem =
	{
		.name = "add",
		.test_ptr = add,
		.next = NULL
	};
	listAddItem(&addItem);
	
	static list_t subItem =
	{
		.name = "sub",
		.test_ptr = sub,
		.next = NULL
	};
	listAddItem(&subItem);
	
	static list_t mulItem =
	{
		.name = "mul",
		.test_ptr = mul,
		.next = NULL
	};
	listAddItem(&mulItem);
	
	static list_t divItem =
	{
		.name = "div",
		.test_ptr = divv,
		.next = NULL
	};
	listAddItem(&divItem);
	
	static list_t dataFromPotentiometerItem =
	{
		.name = "readFromPot",
		.test_ptr = receiveBrightnessFromPotentiometer,
		.next = NULL
	};
	listAddItem(&dataFromPotentiometerItem);
	
	static list_t dataFromUARTItem =
	{
		.name = "readFromPC",
		.test_ptr = receiveBrightnessFromPC,
		.next = NULL
	};
	listAddItem(&dataFromUARTItem);

	static list_t dataFromADXL345PC =
	{
		.name = "adxlPC",
		.test_ptr = adxlReadAndOutToPC,
		.next = NULL
	};
	listAddItem(&dataFromADXL345PC);

	static list_t dataFromADXL345Display =
	{
		.name = "adxlDisplay",
		.test_ptr = adxlReadAndOutOnDisplay,
		.next = NULL
	};
	listAddItem(&dataFromADXL345Display);
	
	static list_t help =
	{
		.name = "help",
		.test_ptr = commandNames,
		.next = NULL
	};
	listAddItem(&help);

}


void listAddItem(list_t* newItem)
{
	//add first element
	if (head == NULL)
	{	
		head = newItem;
		return;
	}
	
	list_t* current = head;
	
	//search end of the list
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = newItem;
}



//test functions
void add(char ** tokensArr)
{
	printf("add = %d\n", (tokensArr[1][0] - 48) + (tokensArr[2][0] - 48));
}

void sub(char ** tokensArr)
{
	printf("sub = %d\n", (tokensArr[1][0] - 48) - (tokensArr[2][0] - 48));
}

void mul(char ** tokensArr)
{
	printf("mul = %d\n", (tokensArr[1][0] - 48) * (tokensArr[2][0] - 48));
}

void divv(char ** tokensArr)
{
	printf("div = %d\n", (tokensArr[1][0] - 48) / (tokensArr[2][0] - 48));
}



void commandNames(char ** tokensArr)
{
	printf("add * * - add two one-figure numbers\n");
	printf("sub * * - sub two one-figure numbers\n");
	printf("mul * * - mul two one-figure numbers\n");
	printf("div * * - div two one-figure numbers\n");
	printf("readFromPot - read data from potentiometer and set this number on led\n");
	printf("readFromPC - read data from terminal and set this number on led\n");
	printf("adxlPC - read data from ADXL345 and sent this data to terminal \n");
	printf("adxlDisplay *** - read data from ADXL345 and sent this data to led\n");
	printf("help - print all of availabe commands\n\n");
}






