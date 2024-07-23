// SAFE-GUARD:
// It is good practice to apply safe-guards to header files
// Safe-guard's ensures only 1 copy of the header file is used in the project build
// The macro name should be mirroring the file name with _ for spaces, dots, etc.

#ifndef CORE_H
#define CORE_H

//////////////////////////////////////
// USER INTERFACE FUNCTIONS
//////////////////////////////////////

// Clear the standard input buffer
void clearInputBuffer(void);

// Wait for user to input the "enter" key to continue
void suspend(void);

//////////////////////////////////////
// USER INPUT FUNCTIONS
//////////////////////////////////////

int inputInt(void);

int inputIntPositive(void);

int inputIntRange(int min, int max);

char inputCharOption(const char *stringList);

void inputCString(char *strValue, int min, int max);

void displayFormattedPhone(const char *number);

#endif // !CORE_H
