#include "timing.h"


#define TICKRATE_HZ1    (4000000U)    // ticks per second 


//points to the begin of the list
timingList* timingHead = NULL;


volatile uint32_t usDelayTimer;


void timingInit(void)
{
    SystemCoreClockUpdate();
    
    (void)SysTick_Config(SystemCoreClock / TICKRATE_HZ1);
}

void timingCheckPeriods(void)
{
    timingList* current = timingHead;
    
    while (current != NULL)
    {
        if (current->counter >= current->timeout)
        {
            char* justForArgumentFunc[2] = {"x", "y"};
            (*current->funcPtr)(justForArgumentFunc);
            current->counter = 0;
        }
        current = current->next;
    }
}

void timingDelay(uint32_t delay)
{
    usDelayTimer = delay;
    
    while (usDelayTimer != 0)
    {
        
    }
}

void timingAddItem(timingList* newItem)
{
    //add address of the element of the stucture as an ID  
    newItem->id = (uint32_t)newItem;
    
    //for first element in the list
    if (timingHead == NULL)
    {
        timingHead = newItem;
        return;
    }
    
    //check for availability of the element
    timingDeleteItem(newItem->id);
    
    
    timingList* current = timingHead;

    while (current->next != NULL)
    {
        current = current->next;
    }
    
    current->next = newItem;
}

//deletes the element from the list
void timingDeleteItem(uint32_t id)
{
    timingList *previous = timingHead;
    timingList *current = previous->next;
  
    //check first element
    if (timingHead->id == id)
    {
        timingHead = timingHead->next;
        return;
    }

    //check other elements
    while(current != NULL)
    {
        if(current->id == id)
        {
            previous->next = current->next;
            break;
        }
        previous = current;
        current = current->next;
    }
}

void HAL_IncTick(void)
{ 
    //static uint32_t msCounter = 0;
    
    /*if(usDelayTimer > 0)
    {
        usDelayTimer--;
    }*/
    
    for (timingList* current = timingHead; current != NULL; current = current->next)
    {
        if (current->counter != current->timeout)
        {
            current->counter++;
        }
    }
}









