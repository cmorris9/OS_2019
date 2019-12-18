void printString(char*);
void readString(char*);
void readSector(char*, int);
void makeInterrupt21(int,int,int,int);
void readFile(char*, char*, int*);
void executeProgram(char*);
void terminate();
void writeSector(char*, int);
void deleteFile(char*);
void printChar(char*);
void writeFile(char*, char*, int);
void copyFile(char*, char*);
void createFile(char*, char*);
void handleTimerInterrupt(int, int);
void returnFromTimer(int, int);

int processActive[8];
int processStackPointer[8];
int currentProcess;
int dataseg;

void main() {
	int startVidMem = 0xb800;
	int vidMemOffset = 0x0;
	int white = 0x7;
	char* letters = "Sean Shea\0";
	char* line[80];
	
	int sectorsRead;
	char bufferC[13312];

	
	int i;
	
	
	for(i=0;i<8;i++) {
		processActive[i] = 0;	
		processStackPointer[i] = 0xff00;
		currentProcess = -1;	
			}
	
		



	while(*letters != 0x0) {
		putInMemory(startVidMem, vidMemOffset, *letters);
		vidMemOffset++;
		putInMemory(startVidMem, vidMemOffset, white);
		vidMemOffset++;
		letters++;
	}
		
		//printString("Enter a line: ");
		//readString(line);
		//printString(line);

		
		//readSector(buffer, 30);
		//printString(buffer);

		//makeInterrupt21();
		//interrupt(0x21,1,line,0,0);
		//interrupt(0x21,0,line,0,0);
		//while(1);

		//makeInterrupt21();
		//interrupt(0x21,3,"messag",bufferC, &sectorsRead);
		//if(sectorsRead>0){
			//interrupt(0x21,0,bufferC,0,0); }
		//else {
			//interrupt(0x21,0, "messag not found\r\n",0,0); }
			
			//while(1);

		makeInterrupt21();
		//interrupt(0x21,4,"testpr",0,0);
		//interrupt(0x21,4,"tstpr2",0,0);
		interrupt(0x21,8,"this is a test message","testmg",3);
		
		interrupt(0x21,4,"shell",0,0);
		makeTimerInterrupt();
		while(1);
}


void printChar(char* c) {
	char character = c;
	interrupt(0x10, 0xe*256+character, 0, 0, 0);    }

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

		
	while(1) {
		x = interrupt(0x16,0,0,0,0);
		
			if(x == enter) {
			interrupt(0x10, 0xE*256+0xa,0,0,0);
			interrupt(0x10, 0xE*256+0xd,0,0,0);
			
			lineLocal[i] = 0x0;
			break;
			}

		else if(x == backspace && i !=0) {
			interrupt(0x10, 0xE*256+x,0,0,0);
			interrupt(0x10, 0xE*256+0x20,0,0,0);
			interrupt(0x10, 0xE*256+x,0,0,0);
			i--;  }

		else {
			interrupt(0x10, 0xE*256+x,0,0,0);
			lineLocal[i++] = x;
	}

		
		}	}
		

	void readSector(char* buffer, int sector) {
		
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
	
	


	void handleInterrupt21(int ax, char* bx, int cx, int dx)  {
		
		if(ax == 0) {

			printString(bx);
   			 }
		else if(ax == 1){
			readString(bx); 
			}
		else if(ax == 2) {
			readSector(bx,cx);
				}
		else if(ax == 3) {
			readFile(bx,cx,dx);
			}
		else if(ax == 4) {
			executeProgram(bx);
			}
		else if(ax == 5) {
			terminate();
			}
		else if(ax == 6) {
			writeSector(bx,cx);
			}
		else if(ax == 7) {
			deleteFile(bx);
			}
		else if(ax == 69){
			printChar(bx);
			}
		else if(ax == 8) {
			writeFile(bx,cx,dx);
			}
		else if(ax == 9) {
			copyFile(bx,cx,dx);
			}
		else if(ax == 10) {
			createFile(bx);
			}
		else { 
			printString("error");
	}}

	

	void readFile(char* fileName, char* bigBuffer, int *sectorsRead){
		int fileEntry;
		int i = 0;
		int j = 0;
		int bufferSize = 0;
		char fileBuffer[512];
		readSector(fileBuffer, 2);
		

	for(i=0; i<512; i = i + 32) {
		
		
		if(fileBuffer[i] == fileName[0] && fileBuffer[i+1] == fileName[1] && fileBuffer[i+2] == fileName[2] && fileBuffer[i+3] == fileName[3] && fileBuffer[i+4] == fileName[4] && fileBuffer[i+5] == fileName[5]) {
			fileEntry = i;
			break;
				}
		
		else {
			
			 *sectorsRead = 0;}

		}
			

	for(j = 6; j <32; j++){
		if(fileBuffer[fileEntry+j] == 0) {
			break;
				}
	else {
		readSector(bigBuffer, fileBuffer[fileEntry+j]);
		bigBuffer = bigBuffer + 512; 
		*sectorsRead = *sectorsRead+1;
}}
}

	void executeProgram(char* name) {

		char bufferX[13312];
		int sectorsRead;
		int i;
		int segment;
		//int dataseg;
	

		readFile(name, bufferX, &sectorsRead);
		dataseg = setKernelDataSegment();
		for(i=0;i<8;i++) {
			
			if(processActive[i] == 0)  
						
				{
				segment =(i+2)*0x1000;
			//loop to put all
				putInMemory(segment,0x0,bufferX);
				initializeProgram();
				dataseg = setKernelDataSegment();
				processActive[i] = 1;
				processStackPointer[i] = 0xff00;
				
				break;
				}
			}
			restoreDataSegment(dataseg);}

	void terminate()
	{
		char shell[6];
		
		shell[0] = "s";
		shell[1] = "h";
		shell[2] = "e";
		shell[3] = "l";
		shell[4] = "l";
		shell[5] = "\0";
				
		dataseg = setKernelDataSegment();
		processActive[currentProcess] = 0;
		restoreDataSegment(dataseg);
		while(1);
			
		

		//executeProgram(shell);
	}


	void writeSector(char* buffer, int sector) {
		
		char al = 1;
		char ah = 3;
		char ch = 0;
		char cl = sector + 1;
		char dh = 0;
		char dl = 0x80;
		int ax = ah*256+al;
		int cx = ch*256+cl;
		int dx = dh*256+dl;
		interrupt(0x13,ax,buffer,cx,dx);

		}


	void deleteFile(char* fileName){
		
		int i;
		int j;
		char dir[512];
		char map[512];
		int sector;
		readSector(dir,2);
		readSector(map,1);
			printChar('$');
		for(i=0; i<512; i = i + 32) {
			printChar('#');
			if(dir[i] == fileName[0] && dir[i+1] == fileName[1] && dir[i+2] == fileName[2] && dir[i+3] == fileName[3] && dir[i+4] == fileName[4] && dir[i+5] == fileName[5]) {
			printChar('%');
			dir[i] = '\0';
			for(j=6;j<32;j++){
				sector = dir[i+j];		
				map[sector] = 0;} }
			
					//break;		
}
				
			writeSector(dir,2);
			writeSector(map,1);

}


	void writeFile(char* buffer, char* filename, int numberOfSectors) {
		
		int i;
		int j;
		int k;
		int l;
		int start;
		int end;
		int entry;
		int sector;
		int sectorsRead;
		char dir[512];
		char map[512];
		readSector(dir,2);
		readSector(map,1);
	
		for(i=0; i<512; i = i + 32) {
			if(dir[i] == '\0') {
				dir[i] = filename[0];
				dir[i+1] = filename[1];
				dir[i+2] = filename[2];
				dir[i+3] = filename[3];
				dir[i+4] = filename[4];
				dir[i+5] = filename[5];
				entry = i;
				break;
			}
			

		} 

	//for(l=0; l<13312; l+=512)
		for(j=3; j<512; j++) {
			if(map[j] == 0) {
				map[j] = 0xFF;
				sector = j;
				break;

			}
		

			}
		dir[i+6] = sector;
		
	//advasnce by 512 in buffer to get to next sector worth of data
		
		
		writeSector(buffer,sector);
	//***
		start = entry*32+7;
		end = entry*32+32;
	
		for(k = start; k<end; k++) {
			dir[k] = 0;}
		
		writeSector(dir,2);
		writeSector(map,1);
		}


	void copyFile(char* filename, char* filename1) {

		char buffer[13312];
		int sectorsRead;

		
		readFile(filename, buffer, &sectorsRead);
		writeFile(buffer, filename1, 1); 
	}

	void createFile(char* filename, char* buffer) {
		
		
		char* line;
		printChar('e');
		printChar('n');
		printChar('t');
		printChar('e');
		printChar('r');
		printChar(':');
		readString(line);
		writeFile(line, filename, 1);}


	
	void handleTimerInterrupt(int segment, int sp) {

		//printChar('T');
		//printChar('i');
		//printChar('c');
		dataseg = setKernelDataSegment();
		if(currentProcess != -1){
		processStackPointer[currentProcess] = sp;}
	for(;;){
		currentProcess +=1;
		currentProcess = currentProcess % 8;
		if(processActive[currentProcess] == 1) {
			break;}}
		//maybe another break?
		restoreDataSegment(dataseg);
****
		returnFromTimer(segment, sp);
		}

	
	


