#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include "core.h"

//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void)
{
    // Discard all remaining char's from the standard input buffer:
    while (getchar() != '\n')
    {
        ; // do nothing!
    }
}

// Wait for user to input the "enter" key to continue
void suspend(void)
{
    printf("<ENTER> to continue...");
    clearInputBuffer();
    putchar('\n');
}

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

// Get an integer from the user
int inputInt(void)
{
    char newLine = 'x';
    int value = 0;

    while (newLine != '\n')
    {
        scanf("%d%c", &value, &newLine);
        if (newLine != '\n')
        {
            clearInputBuffer();
            printf("Error! Input a whole number: ");
        }
    }

    return value;
}

// Validate and get a positive integer from the user
int inputIntPositive(void)
{
    int value;

    do
    {
        value = inputInt();
        if (value <= 0)
        {
            printf("ERROR! Value must be > 0: ");
        }
    } while (value <= 0);

    return value;
}

// Validate and get an integer within a range from the user
int inputIntRange(int min, int max)
{
    int value = 0;
    do
    {
        value = inputInt();

        if (value < min || value > max)
        {
            printf("ERROR! Value must be between %d and %d inclusive: ", min, max);
        }
    } while (value < min || value > max);

    return value;
}

// Get a character from the user that matches one of the characters in the stringList
char inputCharOption(const char *stringList)
{

    char cValue;
    int i = 0;
    int stopLoop = 0;

    do
    {
        scanf("%c", &cValue);
        clearInputBuffer();

        for (i = 0; stringList[i] != '\0'; i++)
        {

            if (cValue == stringList[i])
            {
                stopLoop = 1;
            }
        }

        if (stopLoop == 0)
        {
            printf("ERROR: Character must be one of [%s]: ", stringList);
            clearInputBuffer();
        }

    } while (stopLoop == 0);

    return cValue;
}

// Get a string from the user
void inputCString(char *strValue, int min, int max)
{
    int stopLoop = 0;
    int strlength;
    int exactlength = 0;
    int i;
    do
    {

        char stringList[100];
        scanf(" %[^\n]", stringList);
        clearInputBuffer();
        strlength = 0;
        //  char c = stringList[strlength];

        while (stringList[strlength] != EOF && stringList[strlength] != '\0')
        {
            strlength++;
        }

        if (min == max)
        {
            exactlength = min;
            if (strlength == exactlength)
            {
                for (i = 0; i <= strlength; i++)
                {
                    strValue[i] = stringList[i];
                }
                stopLoop = 1;
            }

            else
            {
                printf("Invalid %d-digit number! Number: ", exactlength);
            }
        }
        else
        {
            if (strlength >= min && strlength <= max)
            {
                for (i = 0; i <= strlength; i++)
                {
                    strValue[i] = stringList[i];
                }
                stopLoop = 1;
            }
            else if (strlength > max)
            {
                printf("ERROR: String length must be no more than %d chars: ", max);
            }
            else
            {
                printf("ERROR: String length must be between %d and %d chars: ", min, max);
            }
        }
        stringList[strlength] = '\0';

    } while (stopLoop == 0);
}

// Display a phone number in the format (xxx)xxx-xxxx
void displayFormattedPhone(const char *number)
{
    int digit = 0;
    int i = 0;

    if (number == NULL)
    {
        printf("(___)___-____");
        return;
    }

    while (number[i] != '\0')
    {
        if (isdigit(number[i]))
        {
            digit++;
        }
        i++;
    }

    if (digit == 10)
    {

        printf("(%c%c%c)%c%c%c-%c%c%c%c", number[0], number[1], number[2],
               number[3], number[4], number[5], number[6],
               number[7], number[8], number[9]);
    }
    else
    {
        printf("(___)___-____");
    }
}
