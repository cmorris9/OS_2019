void printString(char*);
void printChar(char*);
void readString(char*);
void readSector(char*, int);
void readFile(char*, char*)
void makeInterrupt21(int,int,int,int);
void readFile(char*);
void ternimate();

void main() 
{
	int startVidMem = 0xb800;
	int vidMemOffset = 0x0;
	int white = 0x7;
	char* letters = "Hello World\0";
	char* line[80];
	char* buffer[512];
	char* fileName[6];
	
	int sectorsRead;
	char* bufferC[13312];
	

	while(*letters != 0x0) 
	{
		putInMemory(startVidMem, vidMemOffset, *letters);
		vidMemOffset++;
		putInMemory(startVidMem, vidMemOffset, white);
		vidMemOffset++;
		letters++;
	}
		
		//printString("Enter a line: ");
		//readString(line);
		//printString(line);

		
		readSector(buffer, 30);
		printString(buffer);

		//makeInterrupt21();
		//interrupt(0x21,1,line,0,0);
		//interrupt(0x21,0,line,0,0);
		//while(1);

		makeInterrupt21();
		interrupt(0x21,3,"messag",bufferC, &sectorsRead);
		if(sectorsRead>0)
		{
			interrupt(0x21,0,bufferC,0,0); 
		}
		else 
		{
			interrupt(0x21,0, "messag not found\r\n",0,0); 
		}
			
		while(1);
}

// adding printChar function for debugging
void printChar(char* c)
{
	char character = c;
	interrupt(0x10, 0xe*256+character, 0, 0, 0);
}



void printString(char* chars)
{ 
	int i = 0;
	while(chars[i] != 0x0)
	{
		char al = chars[i];
		char ah = 0xe;
		int ax = ah*256+al;
		interrupt(0x10,ax,0,0,0);
		i++;
	}
	i = 0;

}


void readString(char* lineLocal)
{ 
	int i = 0;
	 

	char x,enter,backspace;
	backspace = 0x8;
	enter = 0xd;

		
	while(1) 
	{
		x = interrupt(0x16,0,0,0,0);
		
		if(x == enter) 
		{
			interrupt(0x10, 0xE*256+0xa,0,0,0);
			interrupt(0x10, 0xE*256+0xd,0,0,0);
			
			lineLocal[i] = 0x0;
			break;
		}
		else if(x == backspace && i !=0) 
		{
			interrupt(0x10, 0xE*256+x,0,0,0);
			interrupt(0x10, 0xE*256+0x20,0,0,0);
			interrupt(0x10, 0xE*256+x,0,0,0);
			i--;  
		}

		else 
		{
			interrupt(0x10, 0xE*256+x,0,0,0);
			lineLocal[i++] = x;
		}

		
	}	
}
		

void readSector(char* buffer, int sector) 
{
		
	char al = 1;
	char ah = 2;
	char ch = 0;
	char cl = sector + 1;
	char dh = 0;
	char dl = 0x80;
	int ax = ah*256+al;
	int cx = ch*256+cl;
	int dx = dh*256+dl;
	interrupt(0x13,ax,buffer,cx,dx);

}
	
	


void handleInterrupt21(int ax, char* bx, int cx, int dx)  
{
		
	if(ax == 0) 
	{

		printString(bx);
   	}
	else if(ax == 1){
		readString(bx); 
	}
	else if(ax == 2) 
	{
		readSector(bx,cx);
	}			}
	else if(ax > 2) 
	{
		printString("error: ax is wrong");
	}
	else 
	{ 
		printString("error");
	}
}

	

void readFile(char* fileName, char* file) 
{
		
	int fileEntry;
	int sectorsRead = 0;
	int i = 0;
	int j;
	char* fileBuffer[512];
	char* bufferC[13312];
	readSector(fileBuffer, 2);
		

	for(fileBuffer[i]; fileBuffer[480]; i = i + 32) 
	{
		
		
		if(fileBuffer[i] == fileName[0] && fileBuffer[i+1] == fileName[1] && fileBuffer[i+2] == fileName[2] && fileBuffer[i+3] == fileName[3] && fileBuffer[i+4] == fileName[4] && fileBuffer[i+5] == fileName[5]) 
		{
			fileEntry = i;
			break;
		}
		
		else 
		{
			sectorsRead = 0;
		}
		sectorsRead+=1;

		for(j = 6; j <32; j++)
		{
			if(fileBuffer[fileEntry+j] = 0) 
			{
				break;
			}
			readSector(bufferC, fileBuffer[fileEntry+j]);
			&bufferC+=512;    
		}



	}		
		
}

//Sean Readfile from his commit
void readFile(char* filename, char* file)
{

 	char dir[512];

 	int i, entry,matchFound = -1;

 	readSector(dir,2);

 	for(entry = 0; entry < 512; entry += 32)
	{

 		if(strcomp(dir,filename,entry))
		{

 			matchFound = entry + 6;

 			break;

 		}

 	}


 	if(matchFound > -1)
	{



 		while(dir[matchFound] != 0)
		{

 			readSector(file,dir[matchFound]);

 			break;

 		}

 	}



}



void terminate()
{

	char shell[6];

	shell[0] = 's';

	shell[1] = 'h';

	shell[2] = 'e';

	shell[3] = 'l';

	shell[4] = 'l';

	shell[5] = '\0';

	executeProgram(shell,2000);

	// while(1);

}
