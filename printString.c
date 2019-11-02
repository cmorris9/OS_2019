#include <stdio.h>

void printString(char*);
void readString(char*);


int main()
{

//This part below has been modified to fit step 2. If you run the command in
// step 1  (printString("Hello World") that works fine. Just need to get
// readString working so that the code below prompts the user for a line
// and then reads in the line and then prints as you type
char line[80];
printString("Enter a line: \0");
readString(line);
printString(line);

return 0;   }

//printString below is all set
void printString(char* chars)
{
        printf("%s\n", chars);
}

void readString(char* chars) 
{

//while (interrupt(0x16,0,0,0,0) != 0) {

	for (int i = 0; i < 80; i++)
	{

		chars[i] = interrupt(0x16,0,0,0,0);
		printf("%d\n", chars[i]);
		// What I'm doing here is trying to read the information from the interrupt 
		//into the chars array I made
	}

}


