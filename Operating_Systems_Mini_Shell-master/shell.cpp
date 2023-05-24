#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_ARGS 100
#define MAX_HISTORY_SIZE 10
#define TOKENSIZE 100

using namespace std;

char history[MAX_HISTORY_SIZE][TOKENSIZE];
int historyCount = 0;

void StrTokenizer(char *line, char **argv);
void myExecvp(char **argv);
void executeCommand(char* input);

// Command function prototypes
void lsCommand(char **args);
void cdCommand(char **args);
void pwdCommand();
void mkdirCommand(char **args);
void rmCommand(char **args);
void cpCommand(char **args);
void mvCommand(char **args);
void catCommand(char **args);
void grepCommand(char **args);
void wcCommand(char **args);
void chmodCommand(char **args);
void echoCommand(char **args);
void historyCommand();
void displayCommands();

int main()
{
    char input[250];

    displayCommands(); // Display available commands

    while (true)
    {
        cout << "cwushell-> ";
        cin.getline(input, 250);

        if (strcmp(input, "exit") == 0)
        {
            break;
        }
        else if (strcmp(input, "history") == 0)
        {
            historyCommand();
        }
        else
        {
            executeCommand(input);
        }
    }

    return 0;
}

void myExecvp(char **argv)
{
    pid_t pid;
    int status;
    int childStatus;

    pid = fork();
    if (pid == 0)
    {
        childStatus = execvp(*argv, argv);
        if (childStatus < 0)
        {
            cout << "ERROR: Wrong input" << endl;
        }
        exit(0);
    }
    else if (pid < 0)
    {
        cout << "Something went wrong!" << endl;
    }
    else
    {
        waitpid(pid, &status, 0);
    }
}

void StrTokenizer(char *input, char **argv)
{
    char *stringTokenized;
    stringTokenized = strtok(input, " ");
    while (stringTokenized != NULL)
    {
        *argv++ = stringTokenized;
        stringTokenized = strtok(NULL, " ");
    }

    *argv = NULL;
}

void executeCommand(char* input)
{
    // Skip empty commands
    if (input[0] == '\0') {
        return;
    }

    // Tokenize input
    char* argv[MAX_ARGS];
    StrTokenizer(input, argv);

    // Execute specific commands
    if (strcmp(argv[0], "exit") == 0) {
        exit(0);
    }
    else if (strcmp(argv[0], "ls") == 0) {
        lsCommand(argv);
    }
    else if (strcmp(argv[0], "cd") == 0) {
        cdCommand(argv);
    }
    else if (strcmp(argv[0], "pwd") == 0) {
        pwdCommand();
    }
    else if (strcmp(argv[0], "mkdir") == 0) {
        mkdirCommand(argv);
    }
    else if (strcmp(argv[0], "rm") == 0) {
        rmCommand(argv);
    }
    else if (strcmp(argv[0], "cp") == 0) {
        cpCommand(argv);
    }
    else if (strcmp(argv[0], "mv") == 0) {
        mvCommand(argv);
    }
    else if (strcmp(argv[0], "cat") == 0) {
        catCommand(argv);
    }
    else if (strcmp(argv[0], "grep") == 0) {
        grepCommand(argv);
    }
    else if (strcmp(argv[0], "wc") == 0) {
        wcCommand(argv);
    }
    else if (strcmp(argv[0], "chmod") == 0) {
        chmodCommand(argv);
    }
    else if (strcmp(argv[0], "echo") == 0) {
        echoCommand(argv);
    }
    else {
        cout << "Command not found" << endl;
        return;
    }

    // Add the command to history
    if (historyCount < MAX_HISTORY_SIZE) {
        strcpy(history[historyCount], input);
        historyCount++;
    }
    else {
        strcpy(history[historyCount % MAX_HISTORY_SIZE], input);
        historyCount++;
    }
}

void lsCommand(char **args)
{
    args[0] = (char *)"ls";
    myExecvp(args);
}

void cdCommand(char **args)
{
    if (args[1] == NULL)
    {
        cout << "ERROR: No directory specified" << endl;
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            cout << "ERROR: Directory not found" << endl;
        }
    }
}

void pwdCommand()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        cout << cwd << endl;
    }
    else
    {
        cout << "ERROR: Failed to get current working directory" << endl;
    }
}

void mkdirCommand(char **args)
{
    if (args[1] == NULL)
    {
        cout << "ERROR: No directory name specified" << endl;
    }
    else
    {
        if (mkdir(args[1], 0777) == 0)
        {
            cout << "Directory created successfully" << endl;
        }
        else
        {
            cout << "ERROR: Failed to create directory" << endl;
        }
    }
}

void chmodCommand(char **args)
{
    if (args[1] == NULL || args[2] == NULL)
    {
        cout << "ERROR: Invalid command syntax" << endl;
    }
    else
    {
        if (chmod(args[1], strtol(args[2], 0, 8)) == 0)
        {
            cout << "Permissions changed successfully" << endl;
        }
        else
        {
            cout << "ERROR: Failed to change permissions" << endl;
        }
    }
}
void grepCommand(char** args)
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1)
    {
        cout << "Pipe failed" << endl;
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        // Child process

        // Redirect standard output to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Execute the grep command
        myExecvp(args);
        exit(0);
    }
    else if (pid > 0)
    {
        // Parent process

        // Close the write end of the pipe
        close(pipefd[1]);

        // Read from the read end of the pipe
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0)
        {
            write(STDOUT_FILENO, buffer, bytesRead);
        }

        close(pipefd[0]);

        wait(NULL);
    }
    else
    {
        cout << "Fork failed" << endl;
    }
}
void echoCommand(char **args)
{
    for (int i = 1; args[i] != NULL; i++)
    {
        cout << args[i] << " ";
    }
    cout << endl;
}

void rmCommand(char **args)
{
    // Add the "-r" option to remove directories recursively
    char *newArgs[] = {args[0], "-r", args[1], NULL};

    myExecvp(newArgs);
}


void cpCommand(char **args)
{
    args[0] = (char *)"cp";
    myExecvp(args);
}
void mvCommand(char **args)
{
    args[0] = (char *)"mv";
    myExecvp(args);
}

void catCommand(char **args)
{
    args[0] = (char *)"cat";
    myExecvp(args);
}

void wcCommand(char **args)
{
    args[0] = (char *)"wc";
    myExecvp(args);
}


void historyCommand()
{
    for (int i = 0; i < historyCount; i++)
    {
        cout << i + 1 << ". " << history[i] << endl;
    }
}

void displayCommands()
{
    cout << "Available commands:" << endl;
    cout << "  - ls" << endl;
    cout << "  - cd" << endl;
    cout << "  - pwd" << endl;
    cout << "  - mkdir" << endl;
    cout << "  - rm" << endl;
    cout << "  - cp" << endl;
    cout << "  - mv" << endl;
    cout << "  - cat" << endl;
    cout << "  - grep" << endl;
    cout << "  - wc" << endl;
    cout << "  - chmod" << endl;
    cout << "  - echo" << endl;
    cout << "  - history" << endl;
    cout << "  - exit" << endl;
}

