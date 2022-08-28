#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#include <string.h>
#include <stdbool.h>
#include <errno.h>

//Additinol liblaries
//For filesearch
#include <dirent.h>
#include <sys/stat.h>
//For cdh
#include <ctype.h>

#define PSMODULE "Pstraverse.ko"

const char *sysname = "shellfyre";

int filesearch(char dir[], char** args);


//cdh Global variables and functions
char cd_history_file[250];
char totalDirectory[1000];
char finaldirectory[1000];
char DirectoryList[20][1000];
int child_pipe[2];
int read_info;
int closePipes();
int cdhChild();
int user_Selection(int initialLength);
int cdhChildofChild(char* name_of_command );
int cdhistory(char* cd_history_file);
int countDirectory(char* cd_history_file);
int readDirectory(char* cd_history_file);
int formatFilePath(char* cd_history_file);
int openPath();
int correctDirectories(char* cd_history_file, int size);



int takecd(char** args);

int joker();

int copy(char** args);

void pstraverse(int root, char* search_type);




enum return_codes
{
	SUCCESS = 0,
	EXIT = 1,
	UNKNOWN = 2,
};

struct command_t
{
	char *name;
	bool background;
	bool auto_complete;
	int arg_count;
	char **args;
	char *redirects[3];		// in/out redirection
	struct command_t *next; // for piping
};

/**
 * Prints a command struct
 * @param struct command_t *
 */
void print_command(struct command_t *command)
{
	int i = 0;
	printf("Command: <%s>\n", command->name);
	printf("\tIs Background: %s\n", command->background ? "yes" : "no");
	printf("\tNeeds Auto-complete: %s\n", command->auto_complete ? "yes" : "no");
	printf("\tRedirects:\n");
	for (i = 0; i < 3; i++)
		printf("\t\t%d: %s\n", i, command->redirects[i] ? command->redirects[i] : "N/A");
	printf("\tArguments (%d):\n", command->arg_count);
	for (i = 0; i < command->arg_count; ++i)
		printf("\t\tArg %d: %s\n", i, command->args[i]);
	if (command->next)
	{
		printf("\tPiped to:\n");
		print_command(command->next);
	}
}

/**
 * Release allocated memory of a command
 * @param  command [description]
 * @return         [description]
 */
int free_command(struct command_t *command)
{
	if (command->arg_count)
	{
		for (int i = 0; i < command->arg_count; ++i)
			free(command->args[i]);
		free(command->args);
	}
	for (int i = 0; i < 3; ++i)
		if (command->redirects[i])
			free(command->redirects[i]);
	if (command->next)
	{
		free_command(command->next);
		command->next = NULL;
	}
	free(command->name);
	free(command);
	return 0;
}

/**
 * Show the command prompt
 * @return [description]
 */
int show_prompt()
{
	char cwd[1024], hostname[1024];
	gethostname(hostname, sizeof(hostname));
	getcwd(cwd, sizeof(cwd));
	printf("%s@%s:%s %s$ ", getenv("USER"), hostname, cwd, sysname);
	return 0;
}

/**
 * Parse a command string into a command struct
 * @param  buf     [description]
 * @param  command [description]
 * @return         0
 */
int parse_command(char *buf, struct command_t *command)
{
	const char *splitters = " \t"; // split at whitespace
	int index, len;
	len = strlen(buf);
	while (len > 0 && strchr(splitters, buf[0]) != NULL) // trim left whitespace
	{
		buf++;
		len--;
	}
	while (len > 0 && strchr(splitters, buf[len - 1]) != NULL)
		buf[--len] = 0; // trim right whitespace

	if (len > 0 && buf[len - 1] == '?') // auto-complete
		command->auto_complete = true;
	if (len > 0 && buf[len - 1] == '&') // background
		command->background = true;

	char *pch = strtok(buf, splitters);
	
	command->name = (char *)malloc(strlen(pch) + 1);
	if (pch == NULL)
		command->name[0] = 0;
	else
		strcpy(command->name, pch);

	command->args = (char **)malloc(sizeof(char *));

	int redirect_index;
	int arg_index = 0;
	char temp_buf[1024], *arg;

	while (1)
	{
		// tokenize input on splitters
		pch = strtok(NULL, splitters);
		if (!pch)
			break;
		arg = temp_buf;
		strcpy(arg, pch);
		len = strlen(arg);

		if (len == 0)
			continue;										 // empty arg, go for next
		while (len > 0 && strchr(splitters, arg[0]) != NULL) // trim left whitespace
		{
			arg++;
			len--;
		}
		while (len > 0 && strchr(splitters, arg[len - 1]) != NULL)
			arg[--len] = 0; // trim right whitespace
		if (len == 0)
			continue; // empty arg, go for next

		// piping to another command
		if (strcmp(arg, "|") == 0)
		{
			struct command_t *c = malloc(sizeof(struct command_t));
			int l = strlen(pch);
			pch[l] = splitters[0]; // restore strtok termination
			index = 1;
			while (pch[index] == ' ' || pch[index] == '\t')
				index++; // skip whitespaces

			parse_command(pch + index, c);
			pch[l] = 0; // put back strtok termination
			command->next = c;
			continue;
		}

		// background process
		if (strcmp(arg, "&") == 0)
			continue; // handled before

		// handle input redirection
		redirect_index = -1;
		if (arg[0] == '<')
			redirect_index = 0;
		if (arg[0] == '>')
		{
			if (len > 1 && arg[1] == '>')
			{
				redirect_index = 2;
				arg++;
				len--;
			}
			else
				redirect_index = 1;
		}
		if (redirect_index != -1)
		{
			command->redirects[redirect_index] = malloc(len);
			strcpy(command->redirects[redirect_index], arg + 1);
			continue;
		}

		// normal arguments
		if (len > 2 && ((arg[0] == '"' && arg[len - 1] == '"') || (arg[0] == '\'' && arg[len - 1] == '\''))) // quote wrapped arg
		{
			arg[--len] = 0;
			arg++;
		}
		command->args = (char **)realloc(command->args, sizeof(char *) * (arg_index + 1));
		command->args[arg_index] = (char *)malloc(len + 1);
		strcpy(command->args[arg_index++], arg);
	}
	command->arg_count = arg_index;
	return 0;
}

void prompt_backspace()
{
	putchar(8);	  // go back 1
	putchar(' '); // write empty over
	putchar(8);	  // go back 1 again
}

/**
 * Prompt a command from the user
 * @param  buf      [description]
 * @param  buf_size [description]
 * @return          [description]
 */
int prompt(struct command_t *command)
{
	int index = 0;
	char c;
	char buf[4096];
	static char oldbuf[4096];

	// tcgetattr gets the parameters of the current terminal
	// STDIN_FILENO will tell tcgetattr that it should write the settings
	// of stdin to oldt
	static struct termios backup_termios, new_termios;
	tcgetattr(STDIN_FILENO, &backup_termios);
	new_termios = backup_termios;
	// ICANON normally takes care that one line at a time will be processed
	// that means it will return if it sees a "\n" or an EOF or an EOL
	new_termios.c_lflag &= ~(ICANON | ECHO); // Also disable automatic echo. We manually echo each char.
	// Those new settings will be set to STDIN
	// TCSANOW tells tcsetattr to change attributes immediately.
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

	// FIXME: backspace is applied before printing chars
	show_prompt();
	int multicode_state = 0;
	buf[0] = 0;

	while (1)
	{
		c = getchar();
		// printf("Keycode: %u\n", c); // DEBUG: uncomment for debugging

		if (c == 9) // handle tab
		{
			buf[index++] = '?'; // autocomplete
			break;
		}

		if (c == 127) // handle backspace
		{
			if (index > 0)
			{
				prompt_backspace();
				index--;
			}
			continue;
		}
		if (c == 27 && multicode_state == 0) // handle multi-code keys
		{
			multicode_state = 1;
			continue;
		}
		if (c == 91 && multicode_state == 1)
		{
			multicode_state = 2;
			continue;
		}
		if (c == 65 && multicode_state == 2) // up arrow
		{
			int i;
			while (index > 0)
			{
				prompt_backspace();
				index--;
			}
			for (i = 0; oldbuf[i]; ++i)
			{
				putchar(oldbuf[i]);
				buf[i] = oldbuf[i];
			}
			index = i;
			continue;
		}
		else
			multicode_state = 0;

		putchar(c); // echo the character
		buf[index++] = c;
		if (index >= sizeof(buf) - 1)
			break;
		if (c == '\n') // enter key
			break;
		if (c == 4) // Ctrl+D
			return EXIT;
	}
	if (index > 0 && buf[index - 1] == '\n') // trim newline from the end
		index--;
	buf[index++] = 0; // null terminate string

	strcpy(oldbuf, buf);

	parse_command(buf, command);

	//print_command(command); // DEBUG: uncomment for debugging

	// restore the old settings
	tcsetattr(STDIN_FILENO, TCSANOW, &backup_termios);
	return SUCCESS;
}

int process_command(struct command_t *command);

int main()
{
	
	openPath();
	//Real path is a path resolver referances https://www.systutorials.com/docs/linux/man/3p-realpath/
	char *ptr = realpath(totalDirectory, finaldirectory);

	while (1)
	{
		struct command_t *command = malloc(sizeof(struct command_t));
		memset(command, 0, sizeof(struct command_t)); // set all bytes to 0

		int code;
		code = prompt(command);
		if (code == EXIT)
			break;

		code = process_command(command);
		if (code == EXIT)
			break;

		free_command(command);
	}

	printf("\n");
	return 0;
}



int process_command(struct command_t *command)
{
	int r;
	if (strcmp(command->name, "") == 0)
		return SUCCESS;

	if (strcmp(command->name, "exit") == 0)
		return EXIT;

	if (strcmp(command->name, "cd") == 0)
	{
		if (command->arg_count > 0)
		{
			r = chdir(command->args[0]);
			if (r == -1)
				printf("-%s: %s: %s\n", sysname, command->name, strerror(errno));

			cdhistory(finaldirectory);
			return SUCCESS;
		}
	}

	// TODO: Implement your custom commands here
	if (strcmp(command->name, "filesearch") == 0) {
		//For the recursion first takes current dirrectory
		filesearch(".",command->args);
		return SUCCESS;
	}

	if (strcmp(command->name, "take") == 0) {
		//Getting final directory path
		takecd(command->args);
		cdhistory(finaldirectory);
		return SUCCESS;
	}


	 if (strcmp(command->name, "joker") == 0) {
	 	joker();
	 	return SUCCESS;      
	}

	if (strcmp(command->name, "starwars") == 0) {

		system("telnet towel.blinkenlights.nl");
	}
	
	if (strcmp(command->name, "copy") == 0) {
		copy(command->args);
		return SUCCESS;
	}

	if (strcmp(command->name, "pstraverse")==0) {
		if (command->args[1]==NULL){
			printf("The correct format of using pstraverse is pstraverse PID (-d or -b) option\n");
			return SUCCESS;
		}
		int get_pid=atoi(command->args[0]);
		char search_type[250]; 		
		strcpy(search_type,command->args[1]);
		pstraverse(get_pid,search_type);
 	}
		
	if(pipe(child_pipe) < 0){
		printf("Error when creating cdhPipe: %s", strerror(errno));
	}
	
	pid_t pid = fork();

	if (pid == 0){ // child

		//'cdh' command implementation.
		if(strcmp(command->name, "cdh") == 0){
			cdhChild();

		}

		
		// increase args size by 2
		command->args = (char **)realloc(
			command->args, sizeof(char *) * (command->arg_count += 2));

		// shift everything forward by 1
		for (int i = command->arg_count - 2; i > 0; --i)
			command->args[i] = command->args[i - 1];

		// set args[0] as a copy of name
		command->args[0] = strdup(command->name);
		// set args[arg_count-1] (last) to NULL
		command->args[command->arg_count - 1] = NULL;

		/// TODO: do your own exec with path resolving using execv()
		char *paths = getenv("PATH");
		char *token = strtok(paths, ":");
		char *path = malloc(128);
		while(token != NULL){
			strcpy(path, token);
			strcat(path, "/");
			strcat(path, command->name);
			if(execv(path, command->args) == -1){
				token = strtok(NULL, ":");
			}else{
				free(path);
			}
		}
		exit(0);
	}else{
		/// TODO: Wait for child to finish if command is not running in background

		if(command->background == 0){
			wait(NULL);
			if(strcmp(command->name, "cdh") == 0){
				//int cdhChildofChild(command->name);
				char select_information[1000];
				read_info = read(child_pipe[0], select_information, 1000);

				close(child_pipe[0]);

				r = chdir(select_information);
				if (r == -1){
					printf("-%s: %s: path: %s, %s\n", sysname, command->name, select_information, strerror(errno));
				}
			}else{
				int closePipes();
			}
		}else{
			if(strcmp(command->name, "cdh") == 0){
				//int cdhChildofChild(command->name);
				char select_information[1000];
				read_info = read(child_pipe[0], select_information, 1000);

				close(child_pipe[0]);

				r = chdir(select_information);
				if (r == -1){
					printf("-%s: %s: path: %s, %s\n", sysname, command->name, select_information, strerror(errno));
				}
			}else{
				int closePipes();
			}
		}
		return SUCCESS;
	}

	printf("-%s: %s: command not found\n", sysname, command->name);
	return UNKNOWN;
}




int filesearch(char directory_name[], char** args)
{

		//printf("%s\n",args[2]);
		char keyword[250]; 
  	// Other then struct dirent readdir cannot work
  	DIR *dir;
    struct dirent *entry;
  	char path_lines[1000];
  	//char indent=0;
  	char final_adress[1000];


		FILE *fp= popen("ls", "r");
	//User give the only keyword copy it
	if ((args[0] != NULL) && (args[1] == NULL)){
	  if ((args[0] != NULL) && ((strcmp(args[0], "-r") != 0)) && ((strcmp(args[0], "-o") != 0)))
	  {
	    strcpy(keyword, args[0]);
	    	while (fgets(path_lines, 1000, fp) != NULL) {
					if (strstr(path_lines,keyword)) printf("./%s",path_lines);
				}
				pclose(fp);

	  }
	  else{
	  	printf("Please specify the keyword you want to search");
	  }
	}
	else if ((args[1] != NULL) && (args[2] == NULL)) {
		if ((strcmp(args[0], "-r") == 0))
	  {
		    strcpy(keyword, args[1]);
	  		DIR *dir;
	    
	    struct dirent *entry;

	    if (!(dir = opendir(directory_name)))
	    {
	        return 1;
	    }

	    while ((entry = readdir(dir)) != NULL) {
	    	if (strstr(entry->d_name,keyword)){
					printf("%s/%s\n",directory_name,entry->d_name);
				} 
	        if (entry->d_type == DT_DIR) {
	            char path[1024];
	            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
	                continue;
	            snprintf(path, sizeof(path), "%s/%s", directory_name, entry->d_name);
	            filesearch(path, args);

	        } else {

	        }
	    }
	    closedir(dir);



	  }
	  else if ((strcmp(args[0], "-o") == 0))
	  {
	  	strcpy(keyword, args[1]);
	  	char open[1000];
	  	strcpy(open,"xdg-open ./");

	  	char first_directory[250];
			getcwd(first_directory, sizeof(first_directory));
	  	while (fgets(path_lines, 1000, fp) != NULL) {
					if (strstr(path_lines,keyword)){
					printf("./%s",path_lines);
					strcat(open, path_lines);
					system(open);
					strcpy(open,"xdg-open ./");
				}
			}
				pclose(fp);

	  }
	  else{
	  	printf("Expected format of filesearch \"your search option eaither -r -o or both or nothing\" keyword  \n");
	  }



	}
	else if (args[2] != NULL) {
		//printf("%s\n","hello" );

		if ( ((strcmp(args[0], "-r") == 0) || (strcmp(args[0], "-o") == 0)) && ((strcmp(args[1], "-r") == 0) || ((strcmp(args[1], "-o") == 0)) ))
	  {
	  	
	    strcpy(keyword, args[2]);
	  	DIR *dir;	
	  	char print_directory[1000];
	    struct dirent *entry;
	  	char open[1000] = "xdg-open ";



	    if (!(dir = opendir(directory_name)))
	    {
	        return 1;
	    }
	    while ((entry = readdir(dir)) != NULL) {
	    		if (strstr(entry->d_name,keyword)){
	        	strcpy(print_directory,directory_name);
	        	strcat(print_directory,"/");
	        	strcat(print_directory, entry->d_name);
	        	printf("%s\n",print_directory);
	        	strcat(open,print_directory);
						system(open);
						strcpy(open,"xdg-open ");
						} 

	        if (entry->d_type != DT_DIR) {

	        } else {
	          	char path[1024];
	            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
	                continue;
	            snprintf(path, sizeof(path), "%s/%s", directory_name, entry->d_name);
	            filesearch(path, args);
	        }
	    }
	    closedir(dir);

	  }
	  else{
	  	printf("Expected format of filesearch \"your search option eaither -r -o or both or nothing\" keyword  \n");
	  }


	}

	else{
			printf("Your argumnet number is not expected\n");
			printf("Expected format of filesearch command is filesearch \"your search option eaither -r -o or both or nothing\" keyword  \n");
	}
  	


  return 1;
}




int cdhistory(char* cd_history_file)
{
	char after_cd[250];
	getcwd(after_cd, sizeof(after_cd));
	char cd_write_to[250];
	strcpy(cd_write_to,"echo ");
	strcat(cd_write_to, after_cd);
	strcat(cd_write_to, " >> ");
	strcat(cd_write_to, cd_history_file);
	//printf("%s\n",cd_write_to );
	system(cd_write_to);
  return 1;
}

int closePipes(){
	close(child_pipe[1]);
	close(child_pipe[0]);

}

int cdhChild( ){
			
			
			FILE *fd = fopen(finaldirectory, "r");
			if (fd != NULL){

				//First fix the size of the directory memory to 10
				int initialLength = countDirectory(finaldirectory);
				if(initialLength > 10){
					correctDirectories(finaldirectory, initialLength);
				}
				//If user choice letter then removing a give an integer of the index remember that char increases as 1 byte
				char to_let_to_int = 'a';
				int checkLength = readDirectory(finaldirectory);

				//WE DONT NEED Fıle any more we get the lines to DirectoryList array
				fclose(fd);

				while(1){
					
					if(checkLength == 0){
					 //cdhChild job done now cdhchildofchildwork		
					 exit(0);
					}
					checkLength-=1;
					
						//Print of cd histories
						printf("%c %d) ~%s\n", to_let_to_int + checkLength, checkLength + 1, DirectoryList[checkLength]);
					if(checkLength == 0){
						printf("Select directory by letter or number: ");
						user_Selection(initialLength);
						break;
					} 
				}
				
			}
			else{
				printf("There is no data for the history of cd to create new data file visit on directories.\n");
			}
			exit(0);
 return 1;

}

int user_Selection(int initialLength){
	//If user choice letter then removing a give an integer of the index remember that char increases as 1 byte
	char to_let_to_int = 'a';
	int selected_index;
	char selectedchoice[128];

	if(fgets(selectedchoice, 128, stdin) != NULL){


		if(!isdigit(selectedchoice[0])){
			selected_index = selectedchoice[0] - to_let_to_int;
			if(selected_index > 10 || selected_index < 0){
				printf("The size of the history can be between 0-10.\n");
				write(child_pipe[1], " ", 2);
				close(child_pipe[1]);
			}
			else if( selected_index > initialLength - 1){
				printf("There is no enough history as you demand\n");
				write(child_pipe[1], " ", 2);
				close(child_pipe[1]);
			}else{
				write(child_pipe[1], DirectoryList[selected_index], 1000);
				close(child_pipe[1]);
			}

		}else{

			selected_index = atoi(selectedchoice) - 1;
			if(selected_index > 10 || selected_index < 0){
				printf("The size of the history can be between 0-10.\n");
				write(child_pipe[1], " ", 2);
				close(child_pipe[1]);
			}
			else if( selected_index > initialLength - 1){
				printf("There is no enough history as you demand\n");
				write(child_pipe[1], " ", 2);
				close(child_pipe[1]);
			}else{
				write(child_pipe[1], DirectoryList[selected_index], 1000);
				close(child_pipe[1]);
			}

		}
	}else{
		printf("No selection occur");
	}
	return 1;

}

int cdhChildofChild(char* name_of_command){
	int r;
	char select_information[1000];
	read_info = read(child_pipe[0], select_information, 1000);

	close(child_pipe[0]);

	r = chdir(select_information);
	if (r == -1){
		printf("-%s: %s: path: %s, %s\n", sysname, name_of_command, select_information, strerror(errno));
	}
	cdhistory(finaldirectory);


 return 1;

}





//Since each time file openiing and closing helper function to count line is better then writing each time
int countDirectory(char* cd_history_file)
{
        int total_line = 0;
      	//Open file
 	FILE *fp = fopen(cd_history_file, "r");
        //Get lines
	char lines[1000];
	while(fgets(lines, 1024, fp) != NULL){
		total_line+=1;
	}

	 fclose(fp);

 return total_line;
}

int readDirectory(char* cd_history_file)
{
    int total_line = 0;
      	//Open file
    FILE *fp = fopen(finaldirectory, "r");
        //Get lines
	char lines[1000];
	while(fgets(lines, 1000, fp) != NULL){
		//Get line infos
		memcpy(DirectoryList[total_line], lines, sizeof(lines));
		if(DirectoryList[total_line][strlen(lines) - 1] == '\n'){
			DirectoryList[total_line][strlen(lines) - 1] = '\0';
		}

		total_line+=1;
	}

	 fclose(fp);

 return total_line;
}


int openPath()
{

	getcwd(cd_history_file, sizeof(cd_history_file));
	strcat(cd_history_file, "/cd_history.txt");
	memcpy(totalDirectory, cd_history_file, sizeof(cd_history_file));
	char new[1000];
    char* token;

    memset(new, 0, sizeof(new));
    
    char space[2] = " ";
    token = strtok(cd_history_file, space);

    while(token != NULL){
            strcat(new, token);
            token = strtok(NULL, space);
            if(token == NULL) break;
            strcat(new, "\\ ");
    }

    memset(cd_history_file, 0, 1000);
    memcpy(cd_history_file, new, sizeof(new));

	FILE *fl = fopen("cd_history.txt", "r");
	if(fl == NULL){
		system("echo >> cd_history.txt");
	}else{
		fclose(fl);
	}

 return 1;
}

int correctDirectories(char* cd_history_file, int size){

	int listing = 0;
	int total_line = 0;
	char file_listing[size][1000];
	char lines[1000];
	FILE *fp = fopen(cd_history_file, "r");


	while(fgets(lines, 1000, fp) != NULL){

		memcpy(file_listing[listing], lines, sizeof(lines));
		listing+=1;
	}
	fclose(fp);

	fp = fopen(cd_history_file, "w");
	while(total_line < 10){
		fputs(file_listing[listing-1], fp);
		listing-=1;
		total_line+=1;
	}
	fclose(fp);
	


 return 1;
}





int takecd(char** args)
{
	  char currentDirectory[250];
    //Get directory path     
    char *directories = args[0];
    printf("%s",args[0]);
    //Get each directory
    char *each = strtok(directories, "/");
    //Traversing directories
    while (each != NULL) {
	//If no directory create and make exutable writable and readable
			getcwd(currentDirectory, sizeof(currentDirectory));
			strcat(currentDirectory,"/");
			strcat(currentDirectory,each);   
    	mkdir(currentDirectory, 0777);
	//Change directory
        chdir(currentDirectory);
	//Get other path
        each = strtok(NULL, "/");
    }
  return 1;

}

int joker()
{
		char cronJob[1000];
		//strcpy(cronJob,"* * * * * XDG_RUNTIME_DIR=/run/user/1000 notify-send joke: \"$(curl -s https://icanhazdadjoke.com)\" ");
		strcpy(cronJob,"*/15 * * * * XDG_RUNTIME_DIR=/run/user/1000 notify-send joke: \"$(curl -s https://icanhazdadjoke.com)\" ");
		strcat(cronJob,"\n");
		FILE *filePoint;
		filePoint=fopen("cronJob.txt","w");
		fputs(cronJob,filePoint);
		fclose(filePoint);
		char* args2[]={"crontab","cronJob.txt",NULL};
		pid_t pidjoker=fork();
		if (pidjoker==0){
			execlp("crontab","crontab","cronJob.txt",NULL);
		}else
		{
			wait(NULL);
		}
	return SUCCESS;

}

int copy(char** args){

		char filename[1000];
    char file_names[1000];
    char first_directory[1000];
    char check_directory[1000];
    strcpy(filename,args[0]);
		
    //Make sure that it's different than current directory
		getcwd(first_directory, sizeof(first_directory));
		if ((strcmp(args[1], first_directory) == 0)){
			printf("You cannot copy into same directory");
		}

  
    //Getting file pointer to reach memory
    FILE *fptr1 = fopen(filename, "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", filename);
        exit(0);
    }

    //To get each byte from the file char c used.
    char c;
    c = fgetc(fptr1);
  
		//Changing to user demanded direactory	  
    chdir(args[1]);

    //Make sure that given directory is avaliable
    getcwd(check_directory, sizeof(check_directory));
    if ((strcmp(first_directory, check_directory) == 0)){
			printf("The given directory is not avaliable\n");
			fclose(fptr1);
			chdir(first_directory);
			return SUCCESS;	
		}

    //Check the file exist or not
    FILE *checkpointer= popen("ls", "r");
	  while (fgets(file_names, 1000, checkpointer) != NULL) {
					if (strstr(file_names,filename)){

					 printf("Already there is a file with same name you can not copy to file. Change one of the file name\n");
					 pclose(checkpointer);
					 fclose(fptr1);
					 chdir(first_directory);
					 return SUCCESS;
					}
		}
				

    // Creating name with same name both readable and writable
    FILE *fptr2 = fopen(filename, "w");
    if (fptr2 == NULL)
    {
        printf("Cannot open file %s \n", filename);
        chdir(first_directory);
        return SUCCESS;
    }
  
    //Copying each byto new file until end of the file
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
  	
  	//Turn back initial directory
  	chdir(first_directory);
    fclose(fptr1);
    pclose(checkpointer);
    fclose(fptr2);

    printf("File copied directory to %s\n",args[1] );
    return 1;

}

void pstraverse(int root, char* search_type) {
	char rootinfo[1000], search[1000];
	sprintf(rootinfo, "given_pid=%d", root);
	sprintf(search, "search_type=%s", search_type);

	pid_t child = fork();
	if(child == 0){
		char *insArgs[] = {"/usr/bin/sudo","insmod",PSMODULE,rootinfo,search,0};
    	execv(insArgs[0], insArgs);
	}else{
		wait(NULL);
		char *rmArgs[] = {"/usr/bin/sudo","rmmod",PSMODULE,0};
		pid_t child_remove = fork();
		if (child_remove == 0){
    	execv(rmArgs[0], rmArgs);
		}else{
			wait(NULL);
		}

	}
}








