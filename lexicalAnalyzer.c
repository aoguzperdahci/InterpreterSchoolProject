#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//constant declarations
#define MAX_FILE_NAME_LEN 64
#define MAX_WORD_LEN 256
#define MAX_VARIABLE_NAME_LEN 20
#define MAX_INT_CONST_LEN 101 //with sign

//function declarations
void openFileForRead(char fileName[]);
void openFileForWrite(char fileName[]);
int isDelimiter();
int isKeyword();
int isValid();
int isConstant(char c);
void waitForWord();
void waitForComment();
void waitForStringConst();
void passInvalidWord();
int checkNewLine();
void writeTokenToFile(char token[]);
void displayErorAndPassWord();
void displayErorAndExit();
void checkWord();


//global variables declarations
char KEYWORDS[10][10] = {"int","move","to","add","sub","from","out","loop","times","newline"};
char DELIMITER[] = {' ',',','.','"','[',']','{','}','\n'};
char VALIDCHAR[] = {'-','0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' ',',','.','"','[',']','{','}','_','\n'};
FILE *fileRead;
FILE *fileWrite;
char c;
char word[MAX_WORD_LEN];
int wordSize=0;
int currentLine = 1;
char errorMessage[256];
int errorCount = 0;

int main (int argc,char *argv[]){

    char fileName[MAX_FILE_NAME_LEN];
    if(argc>1)
    {
        strcpy(fileName, argv[1]);
    }else
    {
        printf("Enter the file name:");
        scanf("%s", fileName);
    }

    openFileForRead(fileName);
    openFileForWrite(fileName);

    printf("\n");

    while ((c=fgetc(fileRead))!= EOF)
    {   
        fseek(fileRead, -1, SEEK_CUR);
        waitForWord();
    }

    fclose(fileRead);
    fclose(fileWrite);

    if (errorCount == 0)
    {
        printf("\nProcess completed successfully\nPress enter to exit...");
    }else if (errorCount == 1)
    {
        printf("\n%d errors detected\nPress enter to exit...", errorCount);
    }else
    {
        printf("\n%d errors detected\nPress enter to exit...", errorCount);
    }

    scanf("%c",&c);
    getchar();

    return 0;
}

void openFileForRead(char fileName[])
{
    char f[MAX_FILE_NAME_LEN+3];
    strcpy(f,fileName);
    strcat(f,".ba");
    
    if ((fileRead = fopen(f,"r")) == NULL){

        sprintf(errorMessage,"Error! %s file failed to open", f);
        displayErorAndExit();
    }
}

void openFileForWrite(char fileName[])
{
    char f[MAX_FILE_NAME_LEN+3];
    strcpy(f,fileName);
    strcat(f,".lx");

    if ((fileWrite = fopen(f,"w")) == NULL){
        sprintf(errorMessage,"Error! %s file failed to open", f);
        displayErorAndExit();
    }

}

void writeTokenToFile(char token[])
{
    //strcat(token,"\n");
    //printf("%s\n",token);
    fprintf(fileWrite, "%s\n", token);
}

void displayErorAndPassWord()
{
    errorCount++;
    printf("%s\n",errorMessage);
    passInvalidWord();
}

void displayErorAndExit()
{
    printf("%s\n",errorMessage);
    printf("\nPress enter to exit...");
    scanf("%c",&c);
    getchar();
    exit(1);
}


int checkNewLine()
{
    if ( c == '\n')
    {
        currentLine++;
        return 1;
    }
    return 0;
}

void waitForWord()
{
    word[0] = '\0';
    wordSize = 0;
    while ((c=fgetc(fileRead))!= EOF) 
    {

        if(isValid() == 0)
        {
            sprintf(errorMessage,"EROR line %d! Unrecognized character is detected (%c)",currentLine, c);
            displayErorAndPassWord();
            return ;
        }
        
        if(isDelimiter()){

            if (wordSize > 0)
            {
                checkWord();
            }

            word[0] = '\0';
            wordSize = 0;

            if (checkNewLine())
            {
                continue;
            }
            
            if (c == ' ')
            {
                continue;
            }else if(c == '{')
            {
                waitForComment();
                continue;

            }else if (c == '.')
            {
               writeTokenToFile("EndOfLine");
               continue;

            }else if (c == '}')
            {
                sprintf(errorMessage,"EROR line %d! Invalid syntax (%c)",currentLine, c);
                displayErorAndPassWord();
                continue;

            }else if (c == '"')
            {
                waitForStringConst();
                continue;

            }else if (c == '[')
            {
                char token[20]="OpenBlock";
                writeTokenToFile(token);
                continue;

            }else if (c == ']')
            {
                char token[20]="CloseBlock";
                writeTokenToFile(token);
                continue;

            }else if (c == ',')
            {
                char token[20]="Seperator";
                writeTokenToFile(token);
                continue;
            }
            
        }else
        {
            word[wordSize++] = c;
            word[wordSize] = '\0';
        }
    }
    
    if (wordSize > 0)
    {
        checkWord();
    }
}

int isKeyword()
{
    int len = sizeof(KEYWORDS)/sizeof(KEYWORDS[0]);
    for (int i = 0; i < len; i++)
    {
        if (!strcoll(KEYWORDS[i],word))
        {
            return 1;
        }
    }
    return 0;
}

int isValid()
{
    int len = strlen(VALIDCHAR);

    for (int i = 0; i < len; i++)
    {
        if (VALIDCHAR[i] == c)
        {
            return 1;
        }
    }
    return 0;
}

int isDelimiter()
{
    int len = strlen(DELIMITER);
    for (int i = 0; i < len; i++)
    {
        if (DELIMITER[i] == c)
        {
            return 1;
        }
    }
    return 0;
}


int isConstant(char c)
{
    char list[] = {'-','0','1','2','3','4','5','6','7','8','9'};
    int len = strlen(list);
    for (int i = 0; i < len; i++)
    {
        if (list[i] == c)
        {
            return 1;
        }
    }
    return 0;
}

void waitForComment()
{
    word[0] = '\0';
    wordSize = 0;
    int temp = currentLine;

    while ((c=fgetc(fileRead))!= EOF)
    {   
        if (checkNewLine())
        {
            continue;
        }
        if(c == '}')
        {
            return;
        }
    }

    sprintf(errorMessage,"EROR line %d! Comment block is left open", temp);
    displayErorAndExit();
}


void waitForStringConst()
{
    word[0] = '"';
    word[1] = '\0';
    wordSize = 1;
    int temp = currentLine;

    while ((c=fgetc(fileRead))!= EOF)
    {   
        word[wordSize++] = c;
        word[wordSize] = '\0';
        if (checkNewLine())
        {
            continue;
        }

        if(c == '"' )
        {
            char token[120]="StringConstant ";
            strcat(token,word);
            writeTokenToFile(token);

            word[0] = '\0';
            wordSize = 0;
            return;
        }
    }

    sprintf(errorMessage,"EROR line %d! String is not terminated", temp);
    displayErorAndExit();
}

void checkWord()
{
    if (isKeyword())
    {
        char token[50]="Keyword ";
        strcat(token,word);
        writeTokenToFile(token);

    }else if (isConstant(word[0]))
    {
        int flag = 0; 
        for (int i = 0; i < wordSize; i++)
        {
            if (isConstant(word[i]))
            {
                if(i != 0 && word[i] == '-')
                {
                    flag = 1;
                    continue;;
                }
            }else
            {
                flag = 2;
                break;
            }
        }
        if (wordSize == 1 && word[0] == '-')
        {
            flag = 1;
        }
        

        if (flag == 0)
        {
            if (wordSize > MAX_INT_CONST_LEN)
            {
                sprintf(errorMessage,"EROR line %d! An integer can be as big as 100 decimal digits",currentLine);
                displayErorAndPassWord();

            }else if (wordSize == MAX_INT_CONST_LEN && word[0] != '-')
            {
                sprintf(errorMessage,"EROR line %d! An integer can be as big as 100 decimal digits",currentLine);
                displayErorAndPassWord();
            }else
            {
                char token[120]="IntConstant ";
                strcat(token,word);
                writeTokenToFile(token);
            }
        }else if (flag == 1)
        {
            sprintf(errorMessage,"EROR line %d! Invalid syntax (%s)",currentLine,word);
            displayErorAndPassWord();
        }else
        {
            sprintf(errorMessage,"EROR line %d! Variable names must start with a letter (%s)",currentLine,word);
            displayErorAndPassWord();
        }

    }else
    {
        if (wordSize > MAX_VARIABLE_NAME_LEN)
        {
            sprintf(errorMessage,"EROR line %d! Variable names must have a maximum length of 20 characters (%s)",currentLine, word);
            displayErorAndPassWord();

        }else if (word[0] == '_')
        {
            sprintf(errorMessage,"EROR line %d! Variable names must start with a letter (%s)",currentLine, word);
            displayErorAndPassWord();

        }else
        {
            char token[30]="Identifier ";
            strcat(token,word);
            writeTokenToFile(token);
        }
    }
}

void passInvalidWord()
{
    int t = 0;
    fseek(fileRead, -1, SEEK_CUR);
    while ((c=fgetc(fileRead))!= EOF)
    {   
        t++;
        if (isValid())
        {
            if (isDelimiter())
            {
                if (t > 1)
                {
                    fseek(fileRead, -1, SEEK_CUR);
                }

                word[0] = '\0';
                wordSize = 0;
                return;
            }
        }else if (t > 1)
        {
            sprintf(errorMessage,"EROR line %d! Unrecognized character is detected (%c)",currentLine, c);
            errorCount++;
            printf("%s\n",errorMessage);
        }
        
    }
}