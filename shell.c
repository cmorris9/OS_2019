main(){
    
    char line[80];
    char command [10];
    char buffer[13312];
    int i = 5;
    int j = 0;
    int sectorsRead;

    //Shell in the command line
    syscall(0,"A:>");
    //user input
    syscall(1, line);

        //if the user uses type or exec
        if((line[0]=='t' && line[1] == 'y'&& line[2] == 'p' && line[3] == 'e') ||
        (line[0]=='e' && line[1] == 'x'&& line[2] == 'e'&& line[3] == 'c')){
            while(line[i] !='\0')
            {command[j] = line[i];
            i++;
            j++;
            }
            if(line[0]=='t' && line[1] == 'y'&& line[2] == 'p' && line[3] == 'e'){
                syscall(3,command,buffer,&sectorsRead);
                if(sectorsRead > 0){
                    syscall(0,buffer);
                }
                else{
                    syscall(0, "No file found\n\r");
                }
            }
            if(line[0]=='e' && line[1] == 'x'&& line[2] == 'e' && line[3] == 'c'){
                syscall(4,command);
                    syscall(0,"No file found\n\r");
            }

        }
        else{syscall(0,"Cmd wrong");}


    
}
