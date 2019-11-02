void printString(char*);
void readString(char*);
void readSector(char*, int);    

void main()
{
	int startVidMem = 0xb800;
	int vidMemOffset = 0x0;
	int white = 0x7;
	char* letters = "Hemmo world\0";
	char* line[80];
	char* buffer[512];
	//void printString(char*);
	//void readString(char*);
	//void readSector(char*, int);	
	char al = 'Q';
	char ah = 0xe;
	int ax = ah*256+al;
	//interrupt(0x10,ax,0,0,0);
	while(*letters != 0x0)
	{
		putInMemory(startVidMem, vidMemOffset, *letters);
		vidMemOffset++;
		putInMemory(startVidMem, vidMemOffset, white);
		vidMemOffset++;
		letters++;
	}

	printString("Enter a line: ");
	readString(line);
	printString(line);

	readSector(buffer, 30);
	printString(buffer);	
	while(1);
}

//print string function takes string (array of char), in this case "hello"
void printString(char* chars)
{
	//start index at 0
	int i =0;
	// while the character does not equel the endline
	while(chars[i] != 0x0)
	{
		
		//set current index to al and pass it through to create parameters
		//pass paremeters to inturrupt and increase index. 
		char al = chars[i];
        	char ah = 0xe;
        	int ax = ah*256+al;
       		interrupt(0x10,ax,0,0,0);
		i++;

	}
	//c is odd so setting to 0 at end just incase it does not want to forget
	i = 0;
}

//print string function takes string (array of char), in this case "hello"
void readString(char* chars)
{
        //start index at 0
        int index =0;
	char* templine[80];
	char x, enter, backspace;
	backspace = 0x8;
	enter = 0xd;
	
        // while the character does not equel the endline
        while(1)
        {

                
                x = interrupt(0x16,0,0,0,0);
		if(x == enter)
		{
			interrupt(0x10, 0xE*256+0xa, 0, 0, 0);
			interrupt(0x10, 0xE*256+0xd, 0, 0, 0);
			chars[index] = 0x0;
			break;
		}
		else if(x == backspace && index != 0x0)
		{
			interrupt(0x10, 0xE*256+x, 0, 0, 0);
                        interrupt(0x10, 0xE*256+0x20, 0, 0, 0);
			interrupt(0x10, 0xE*256+x, 0, 0, 0);
			index--;
                       
		}
		else
		{
			interrupt(0x10, 0xE*256+x, 0, 0, 0);
			chars[index++] = x;
                       
		}
               

        }       
}



void readSector(char* buffer, int sector)
{
        //start index at 0
        int i =0;
        // while the character does not equel the endline
     

                //set current index to al and pass it through to create parameters
                //pass paremeters to inturrupt and increase index.
	char al = 1;
        char ah = 2;
	int ch = 0;
	int dh = 0;
	int cl = 2;
	int dl = 0x80;
	int ax = ah*256+al;
	int cx = ch*256+cl;
	int dx = dh*256+dl;
        interrupt(0x13, ax, buffer, cx, dx);
               

        
        //c is odd so setting to 0 at end just incase it does not want to forget
      
}

