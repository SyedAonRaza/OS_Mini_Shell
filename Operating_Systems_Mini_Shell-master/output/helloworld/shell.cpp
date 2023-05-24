#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TOKENSIZE 100

using namespace std;

void StrTokenizer(char *line, char **argv);
void myExecvp(char **argv);

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

int main()
{
    char input[250];
    char *argv[TOKENSIZE];

    while (true)
    {
        cout << "cwushell-> ";
        cin.getline(input, 250);
        StrTokenizer(input, argv);

        if (strcmp(input, "exit") == 0)
        {
            break;
        }
        else if (strcmp(input, "\n") == 0)
        {
            continue;
        }
        else if (strcmp(argv[0], "ls") == 0)
        {
            lsCommand(argv);
        }
        else if (strcmp(argv[0], "cd") == 0)
        {
            cdCommand(argv);
        }
        else if (strcmp(argv[0], "pwd") == 0)
        {
            pwdCommand();
        }
        else if (strcmp(argv[0], "mkdir") == 0)
        {
            mkdirCommand(argv);
        }
        else if (strcmp(argv[0], "rm") == 0)
        {
            rmCommand(argv);
        }
        else if (strcmp(argv[0], "cp") == 0)
        {
            cpCommand(argv);
        }
        else if (strcmp(argv[0], "mv") == 0)
        {
            mvCommand(argv);
        }
        else if (strcmp(argv[0], "cat") == 0)
        {
            catCommand(argv);
        }
        else if (strcmp(argv[0], "grep") == 0)
        {
            grepCommand(argv);
        }
        else if (strcmp(argv[0], "wc") == 0)
        {
            wcCommand(argv);
        }
        else if (strcmp(argv[0], "echo") == 0)
        {
            echoCommand(argv);
        }
        else
        {
            cout << "Command not found" << endl;
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

// Implementations of command functions

void lsCommand(char **args)
{
    myExecvp(args);
}

void cdCommand(char **args)
{
    myExecvp(args);
}

void pwdCommand()
{
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        cout << "Current working directory: " << cwd << endl;
    }
    else
    {
        cout << "Failed to get current working directory" << endl;
    }
}

void mkdirCommand(char **args)
{
    myExecvp(args);
}

void rmCommand(char **args)
{
    // Add the "-r" option to remove directories recursively
    char *newArgs[] = {args[0], "-r", args[1], NULL};

    myExecvp(newArgs);
}

void cpCommand(char **args)
{
    myExecvp(args);
}

void mvCommand(char **args)
{
    myExecvp(args);
}

void catCommand(char **args)
{
    myExecvp(args);
}

void grepCommand(char **args)
{
    myExecvp(args);
}

void wcCommand(char **args)
{
    myExecvp(args);
}

void echoCommand(char **args)
{
    // Skip the first element (command name) and start from index 1
    args++;

    // Loop through the arguments and print each one
    while (*args != NULL)
    {
        cout << *args << " ";
        args++;
    }
    cout << endl;
}
