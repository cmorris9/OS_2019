// shell

enableInterrupts();
main(){



char commandIn[80];
char fileName[6];
char fileName2[6];
int i;
int k;
char buffer[13312];
int sectorsRead;
char dir[512];
int j;
char* dirFileName;



for(;;){
	enableInterrupts();
	syscall(0,"SHELL>");
	syscall(1,commandIn);
	//syscall(0,commandIn);

	if(commandIn[0] == 't' && commandIn[1] == 'y' && commandIn[2] == 'p' && commandIn[3] == 'e'){
		for(i = 0; i <6; i++) {
			fileName[i] = commandIn[i+5];
				}
		syscall(3,fileName,buffer, &sectorsRead);
			if(sectorsRead > 0){
				syscall(0,buffer);	}
			else{  syscall(0,"file not found");
			}}
			
	else if(commandIn[0] == 'e' && commandIn[1] == 'x' && commandIn[2] == 'e' && commandIn[3] == 'c'){
		for(i = 0; i <6; i++) {
			fileName[i] = commandIn[i+5];
				}
		syscall(4,fileName);}

	else if(commandIn[0] == 'd' && commandIn[1] == 'i' && commandIn[2] == 'r') {
		syscall(2,dir,2);

		for(j=0; j<512; j = j + 32){
			if(dir[j] != '\0'){
			
			
			syscall(69,dir[j]);
			syscall(69,dir[j+1]);
			syscall(69,dir[j+2]);
			syscall(69,dir[j+3]);
			syscall(69,dir[j+4]);
			syscall(69,dir[j+5]);
			syscall(0,"\n\r");
}
		}
		}
	else if(commandIn[0] == 'd' && commandIn[1] == 'e' && commandIn[2] == 'l') {

		for(i = 0; i <6; i++) {
			fileName[i] = commandIn[i+4];	}	

	
		syscall(7,fileName);   }

	else if(commandIn[0] == 'c' && commandIn[1] == 'o' && commandIn[2] == 'p' && commandIn[3] == 'y') {
		
		for(i = 0; i <6; i++) {
			fileName[i] = commandIn[i+5];	}

		for(k = 0; k<6; k++) {
			fileName2[k] = commandIn[k+12]; }

		syscall(9, fileName, fileName2);
		}	
	else if(commandIn[0] == 'c' && commandIn[1] == 'r' && commandIn[2] == 'e' && commandIn[3] == 'a' && commandIn[4] == 't' && commandIn[5] == 'e') {
		
		
		for(i = 0; i <6; i++) {
			fileName[i] = commandIn[i+7];	}

		syscall(10, fileName, buffer);
		}

	else{
		syscall(0,"Bad Command");}
		}
	}

