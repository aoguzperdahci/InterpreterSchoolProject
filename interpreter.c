#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//constant declarations
#define MAX_FILE_NAME_LEN 64
#define MAX_VARIABLE_NUMBER 256
#define MAX_WORD_LEN 256
#define MAX_VARIABLE_NAME_LEN 20
#define MAX_INT_CONST_LEN 101 //with sign

//function declarations
void openFileForRead(char fileName[]);
int isDelimiter();
int isKeyword();
int isValid();
int isVariable(char name[]);
int isConstant(char c);
void readWordFromFile();
void waitForLine();
void waitForDeclaration();
void waitForAssignment();
void waitForAddition();
void waitForSubtraction();
void waitForOutput();
void waitForLoop();
char* waitForNewVariable();
char* waitForExistingVariable();
char* waitForIntValue();
char* waitForIntConst();
void waitForKeywordTo();
void waitForKeywordFrom();
void waitForKeywordTimes();
void waitForKeywordNewlineOrExistingVariable();
void waitForCodeBlock();
void waitForComment();
void waitForStringConst();
void waitForEndOfLine();
void waitForOutList();
void waitForOutElement();
void waitForLineOrBlock();
int checkNewLine();
void displayErorAndExit();
void initializeVariable(char name[]);
char* getValueOfVariable(char name[]);
void assign(char destination[], char value[]);
char* cloneInt(char value[]);
void reverseInt(char value[]);
char* add(char number1[], char number2[]);
char* subtract(char number1[], char number2[]);
int isGreater(char number1[], char number2[]);
void printInt(char value[]);

//global variables declarations
struct Variable
{
	char name[MAX_VARIABLE_NAME_LEN + 1];
    char value[MAX_INT_CONST_LEN + 1];
};
struct Variable variables[MAX_VARIABLE_NUMBER];
int variableCounter = 0;
FILE *fileRead;
char c;
char word[MAX_WORD_LEN];
int wordSize=0;
int currentLine = 1;
char errorMessage[256];

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
    printf("\n");

    while ((c=fgetc(fileRead))!= EOF)
    {   
        fseek(fileRead, -1, SEEK_CUR);
        waitForLine();
    }

    fclose(fileRead);

    printf("\nProcess completed successfully\nPress enter to exit...");
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

int isKeyword()
{
    static char KEYWORDS[10][8] = {"int","move","to","add","sub","from","out","loop","times","newline"};
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
    static char VALIDCHAR[] = {'-','0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' ',',','.','"','[',']','{','}','_','\n'};
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
    static char DELIMITER[] = {' ',',','.','"','[',']','{','}','\n'};
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

int isVariable(char name[])
{
    for (int i = 0; i < variableCounter; i++)
    {
        if (!strcoll(name, variables[i].name))
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

void readWordFromFile()
{
    word[0] = '\0';
    wordSize = 0;
    while ((c=fgetc(fileRead))!= EOF) 
    {

        if(isValid() == 0)
        {
            sprintf(errorMessage,"EROR line %d! Unrecognized character is detected (%c)", currentLine, c);
            displayErorAndExit();
        }
        
        if(isDelimiter()){

            if(wordSize==0){

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
                }
                else
                {
                    sprintf(errorMessage,"EROR line %d! Invalid syntax (%c)", currentLine, c);
                    displayErorAndExit();
                }

            }else
            {
                fseek(fileRead, -1, SEEK_CUR);
                return;
            }
            
        }else
        {
            word[wordSize++] = c;
            word[wordSize] = '\0';
        }
    }
    
    sprintf(errorMessage,"EROR line %d! Invalid syntax (%s)", currentLine, word);
    displayErorAndExit();
}

void waitForLine()
{
    readWordFromFile();

    if (isKeyword())
    {
        if (!strcoll(word,"int"))
        {
            waitForDeclaration();
            return;
        }
        else if (!strcoll(word,"move"))
        {
            waitForAssignment();
            return;
        }
        else if (!strcoll(word,"add"))
        {
            waitForAddition();
            return;
        }
        else if (!strcoll(word,"sub"))
        {
            waitForSubtraction();
            return;

        }
        else if (!strcoll(word,"out"))
        {
            waitForOutput();
            return;
        }
        else if (!strcoll(word,"loop"))
        {
            waitForLoop();
            return;
        }
        else
        {
            sprintf(errorMessage,"EROR line %d! Invalid syntax (%s). Wrong keyword (leading keyword was expected)", currentLine, word);
            displayErorAndExit();
        }

    }else
    {
        sprintf(errorMessage,"EROR line %d! Invalid syntax (%s). Keyword was expected", currentLine, word);
        displayErorAndExit();
    }
}

void waitForDeclaration()
{
    char *value;
    value = waitForNewVariable();
    initializeVariable(value);
    free(value);
    waitForEndOfLine();
}

void waitForAssignment()
{
    char *source;
    source = waitForIntValue();
    waitForKeywordTo();
    char *destination;
    destination = waitForExistingVariable();
    assign(destination, source);//assign the value to variable 
    free(source);
    free(destination);
    waitForEndOfLine();
}

void waitForAddition()
{
    char *number1;
    number1 = waitForIntValue();
    waitForKeywordTo();
    char *number2;
    number2 = waitForExistingVariable();
    char *result;

    int len1 = strlen(number1);
    int len2 = strlen(number2);
    if (number1[len1 - 1] == '-' && number2[len2 - 1] == '-')
    {//if both numbers are negative then (number2 + number1) = -((-number2) + (-number1))
        number1[len1 - 1] = '\0';
        number2[len2 - 1] = '\0';
        result = add(number1, number2);
        int lenResult = strlen(result);
        result[lenResult] = '-';
        result[lenResult + 1] = '\0';
        number1[len1 - 1] = '-';
        number2[len2 - 1] = '-';

    }else if (number1[len1 - 1] == '-')
    {//if number1 is negative and number2 is positive then (number2 + number1) = (number2 - (-number1))
        number1[len1 - 1] = '\0';

        if (isGreater(number2, number1))
        {//if number2 is greater then result is positive or zero
            result = subtract(number2, number1);
            
        }else
        {//if number1 is greater then result is negative
            result = subtract(number1, number2);
            int lenResult = strlen(result);
            result[lenResult] = '-';
            result[lenResult + 1] = '\0';
        }

        number1[len1 - 1] = '-';

    }else if (number2[len2 - 1] == '-')
    {//if number2 is negative and number1 is positive then (number2 + number1) = (number1 - (-number2))
        number2[len2 - 1] = '\0';

        if (isGreater(number1, number2))
        {//if number1 is greater then result is positive or zero
            result = subtract(number1, number2);

        }else
        {//if number2 is greater then result is negative
            result = subtract(number2, number1);
            int lenResult = strlen(result);
            result[lenResult] = '-';
            result[lenResult + 1] = '\0';
        }
        number2[len2 - 1] = '-';

    }else
    {//if both numbers are positive
        result = add(number1, number2);
    }

    assign(number2,result);
    free(number1);
    free(number2);
    free(result);

    waitForEndOfLine();
}

void waitForSubtraction()
{
    char *number1;
    number1 = waitForIntValue();
    waitForKeywordFrom();
    char *number2;
    number2 = waitForExistingVariable();
    char *result;

    int len1 = strlen(number1);
    int len2 = strlen(number2);
    if (number1[len1 - 1] == '-' && number2[len2 - 1] == '-')
    {//if both numbers are negative then (number2 - number1) = -/+ ((-number2) - (-number1))
        number1[len1 - 1] = '\0';
        number2[len2 - 1] = '\0';

        if (isGreater(number1, number2))
        {//if number1 is greater then result is positive or zero
            result = subtract(number1, number2);
            
        }else
        {//if number2 is greater then result is negative
            result = subtract(number2, number1);
            int lenResult = strlen(result);
            result[lenResult] = '-';
            result[lenResult + 1] = '\0';
        }

        number1[len1 - 1] = '-';
        number2[len2 - 1] = '-';

    }else if (number1[len1 - 1] == '-')
    {//if number1 is negative and number2 is positive then (number2 - number1) = (number2 + (-number1))
        number1[len1 - 1] = '\0';
        result = add(number1, number2);
        number1[len1 - 1] = '-';

    }else if (number2[len2 - 1] == '-')
    {//if number2 is negative and number1 is positive then (number2 - number1) = -(number1 + (-number2))
        number2[len2 - 1] = '\0';
        result = add(number1, number2);
        int lenResult = strlen(result);
        result[lenResult] = '-';
        result[lenResult + 1] = '\0';
        number2[len2 - 1] = '-';

    }else
    {//if both numbers are positive
        if (isGreater(number2, number1))
        {//if number2 is greater then result is positive or zero
            result = subtract(number2, number1);

        }else
        {//if number1 is greater then result is negative
            result = subtract(number1, number2);
            int lenResult = strlen(result);
            result[lenResult] = '-';
            result[lenResult + 1] = '\0';
        }
    }

    assign(number2,result);
    free(number1);
    free(number2);
    free(result);

    waitForEndOfLine();
}

void waitForOutput()
{
    waitForOutList();
    waitForEndOfLine();
}

void waitForLoop()
{
    char *value;
    value = waitForIntValue();
    int len = strlen(value);
    if (value[len - 1] == '-')
    {
        sprintf(errorMessage,"EROR line %d! Invalid value for loop statement (%s). Positive value was expected", currentLine, value);
        displayErorAndExit();
    }
    
    waitForKeywordTimes();
    int temp = currentLine;
    int save = ftell(fileRead);
    assign(value, subtract(value, "0"));//Clear leading zeros if any
    if (value[len - 1] == '-')
    {
        sprintf(errorMessage,"EROR line %d! Invalid value for loop statement (%s). Positive value was expected", currentLine, value);
        displayErorAndExit();
    }else if(len == 0)
    {//if loop controller value is 0 skip the loop
        free(value);
        return;
    }

    while (1)
    {//Until loop controller value reaches 0
        waitForLineOrBlock();

        assign(value, subtract(value, "1"));//After executing the line or block decrease the loop controller value 1
        len = strlen(value);
        if (value[len - 1] == '-')
        {
            sprintf(errorMessage,"EROR line %d! Invalid value for loop statement (%s). Positive value was expected", currentLine, value);
            displayErorAndExit();
        }

        if (len > 0)
        {//if loop controller value > 0 reset file pointer and reexecute the line or block
            fseek(fileRead, save, SEEK_SET);
            currentLine = temp;
        }else
        {//when loop controller value reaches 0 exit the loop
            break;
        }
    }
    
    free(value);
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

char* waitForIntValue()
{
    word[0] = '\0';
    wordSize = 0;
    static char *value;
    while ((c=fgetc(fileRead))!= EOF)
    {   
        if(isValid() == 0)
        {
            sprintf(errorMessage,"EROR line %d! Unrecognized character is detected (%c)", currentLine, c);
            displayErorAndExit();
        }
        if(isDelimiter())
        {
            if (checkNewLine())
            {
                continue;
            }
            if(c == ' ')
            {
                continue;

            }else if(c == '{')
            {
                waitForComment();
                continue;

            }else
            {
                sprintf(errorMessage,"EROR line %d! Invalid syntax (%c)", currentLine, c);
                displayErorAndExit();
            }

        }else{
            if (c == '_')
            {
                sprintf(errorMessage,"EROR line %d! Invalid syntax (%c)", currentLine, c);
                displayErorAndExit();
            }

            fseek(fileRead, -1, SEEK_CUR);
            if(isConstant(c))
            {
                value = waitForIntConst();

            }else
            {
                value = waitForExistingVariable();
            }

            return value;
        }
    }
    sprintf(errorMessage,"EROR line %d! Invalid syntax (%s)", currentLine, word);
    displayErorAndExit();
}

void waitForEndOfLine()
{
    int temp = currentLine;
    word[0] = '\0';
    wordSize = 0;

    while ((c=fgetc(fileRead))!= EOF)
    {   
        if (checkNewLine())
        {
            continue;
        }
        if(isValid() == 0)
        {
            sprintf(errorMessage,"EROR line %d! Unrecognized character is detected (%c)", currentLine, c);           
            displayErorAndExit();
        }

        if(c == ' ')
        {
            continue;

        }else if (c == '{')
        {
            waitForComment();
            continue;

        }else if (c == '.')
        {
            return;

        }else{
            sprintf(errorMessage,"EROR line %d! Missing end of line", temp);
            displayErorAndExit();
        }
    }
    sprintf(errorMessage,"EROR line %d! Missing end of line", temp);
    displayErorAndExit();
}

char* waitForNewVariable()
{
    readWordFromFile();
    if(isKeyword())
    {
        sprintf(errorMessage,"EROR line %d! Keywords cannot be used as variable name (%s)", currentLine, word);
        displayErorAndExit();

    }else if (isVariable(word))
    {
        sprintf(errorMessage,"EROR line %d! This variable name is already taken (%s)", currentLine, word);
        displayErorAndExit();

    }else
    {
        if (wordSize > MAX_VARIABLE_NAME_LEN)
        {
            sprintf(errorMessage,"EROR line %d! Variable names must have a maximum length of 20 characters (%s)", currentLine, word);
            displayErorAndExit();

        }else if (isConstant(word[0]) || word[0] == '_')
        {
            sprintf(errorMessage,"EROR line %d! Variable names must start with a letter (%s)", currentLine, word);
            displayErorAndExit();
        }
    }

    return word;
}

char* waitForIntConst()
{
    static char *value;
    readWordFromFile();
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
            sprintf(errorMessage,"EROR line %d! An integer can be as big as 100 decimal digits", currentLine);
            displayErorAndExit();

        }else if (wordSize == MAX_INT_CONST_LEN && word[0] != '-')
        {
            sprintf(errorMessage,"EROR line %d! An integer can be as big as 100 decimal digits", currentLine);
            displayErorAndExit();

        }else
        {
            value = cloneInt(word);
            reverseInt(value);
            return value;
        }

    }else if(flag == 1)
    {
        sprintf(errorMessage,"EROR line %d! Invalid syntax (%s). Int constant was expected", currentLine, word);
        displayErorAndExit();

    }else
    {
        sprintf(errorMessage,"EROR line %d! Variable names must start with a letter (%s)", currentLine, word);
        displayErorAndExit();
    }
}

char* waitForExistingVariable()
{
    static char *value;
    readWordFromFile();
    if(isKeyword())
    {
        sprintf(errorMessage,"EROR line %d! Keywords cannot be used as variable name (%s)", currentLine, word);
        displayErorAndExit();

    }else if (isVariable(word))
    {
        value = getValueOfVariable(word);
        return value;

    }else
    {
        sprintf(errorMessage,"EROR line %d! Variables must be declared before they are used (%s)", currentLine, word);
        displayErorAndExit();
    }
}

void waitForKeywordTo()
{
    readWordFromFile();
    if(!strcoll(word,"to"))
    {
        return;
    }else
    {
        sprintf(errorMessage,"EROR line %d! Invalid syntax (%s). Keyword \"to\" was expected", currentLine, word);
        displayErorAndExit();
    }
}

void waitForKeywordFrom()
{
    readWordFromFile();
    if(!strcoll(word,"from"))
    {
        return;

    }else
    {
        sprintf(errorMessage,"EROR line %d! Invalid syntax (%s). Keyword \"from\" was expected", currentLine, word);
        displayErorAndExit();
    }
}

void waitForOutList()
{
    waitForOutElement();

    while ((c=fgetc(fileRead))!= EOF)
    {
        if (checkNewLine())
        {
            continue;
        }
        if(c == ' ')
        {
            continue;

        }else if(c == '{')
        {
            waitForComment();
            continue;

        }else if(c == ',')
        {
            waitForOutElement();
            continue;

        }else if(c == '.') 
        {
            fseek(fileRead, -1, SEEK_CUR);
            return;
        }else
        {
            sprintf(errorMessage,"EROR line %d! Invalid syntax. There must be a separator between output elements", currentLine);
            displayErorAndExit();
        }
    }
}

void waitForOutElement()
{   
    while ((c=fgetc(fileRead))!= EOF)
    {   
        if (checkNewLine())
        {
            continue;
        }
        if(isValid() == 0)
        {
            sprintf(errorMessage,"EROR line %d! Unrecognized character is detected (%c)", currentLine, c);
            displayErorAndExit();
        }

        if (c == ' ')
        {
            continue;

        }else if (c == '{')
        {
            waitForComment();
            continue;

        }else if (c == '"')
        {
            waitForStringConst();
            printf("%s", word);
            return;

        }else if (c == 'n')
        {
            fseek(fileRead, -1, SEEK_CUR);
            waitForKeywordNewlineOrExistingVariable();
            return;

        }else
        {
            fseek(fileRead, -1, SEEK_CUR);
            char *value;
            value = waitForIntValue();
            printInt(value);
            free(value);
            return;
        }
    }

    sprintf(errorMessage,"EROR line %d! Invalid syntax (%s)", currentLine, word);
    displayErorAndExit();
}

void waitForStringConst()
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

        if(c == '"' )
        {
            return;

        }else
        {
            word[wordSize++] = c;
            word[wordSize] = '\0';
        }
    }

    sprintf(errorMessage,"EROR line %d! String is not terminated", temp);
    displayErorAndExit();
}

void waitForKeywordNewlineOrExistingVariable()
{
    readWordFromFile();
    if(!strcoll(word,"newline"))
    {
        printf("\n");
        return;
    }else
    {
        fseek(fileRead, -wordSize , SEEK_CUR);
        char *value;
        value = waitForExistingVariable();
        printInt(value);
        free(value);
        return;
    }
}

void waitForKeywordTimes()
{
    readWordFromFile();
    if(!strcoll(word,"times"))
    {
        return;

    }else
    {
        sprintf(errorMessage,"EROR line %d! Invalid syntax (%s). Keyword \"times\" was expected", currentLine, word);
        displayErorAndExit();
    }
}

void waitForLineOrBlock()
{
    word[0] = '\0';
    wordSize = 0;
    while ((c=fgetc(fileRead))!= EOF)
    {   
        if(isValid() == 0)
        {
            sprintf(errorMessage,"EROR line %d! Unrecognized character is detected (%c)", currentLine, c);
            displayErorAndExit();
        }
        if (isDelimiter())
        {
            if(wordSize == 0)
            {
                if (checkNewLine())
                {
                    continue;
                }
                if(c == ' ')
                {
                    continue;

                }else if(c == '{')
                {
                    waitForComment();
                    continue;

                }else if (c == '[')
                {
                    waitForCodeBlock();
                    return;
                }else
                {
                    sprintf(errorMessage,"EROR line %d! Invalid syntax (%c)", currentLine, c);
                    displayErorAndExit();
                }
            }else
            {
                fseek(fileRead, -wordSize-1, SEEK_CUR);
                waitForLine();
                return;
            }

        }else
        {
            word[wordSize++] = c;
            word[wordSize] = '\0';
        }
        
    }
    
    sprintf(errorMessage,"EROR line %d! Invalid syntax (%s)", currentLine, word);
    displayErorAndExit();
}

void waitForCodeBlock()
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
        if(isValid() == 0)
        {
            sprintf(errorMessage,"EROR line %d! Unrecognized character is detected (%c)", currentLine, c);
            displayErorAndExit();
        }

        if(c == ' ')
        {
            continue;

        }else if(c == '{')
        {
            waitForComment();
            continue;

        }else if (c == ']')
        {
            return;

        }else
        {
            fseek(fileRead, -wordSize-1, SEEK_CUR);
            waitForLine();
            continue;  
        }
    }

    sprintf(errorMessage,"EROR line %d! Code block is left open", temp);
    displayErorAndExit();
}

void initializeVariable(char name[])
{
    int len = strlen(name);
    for (int i = 0; i < len; i++)
    {
        variables[variableCounter].name[i] = name[i];
    }
    variables[variableCounter].name[len] = '\0';
    
    variables[variableCounter].value[0] = '0';
    variables[variableCounter].value[1] = '\0';
    variableCounter++;
}

void assign(char destination[], char value[])
{
    int len = strlen(value);
    for (int i = 0; i < len; i++)
    {
        destination[i] = value[i];
    }
    destination[len] = '\0';
}

char* getValueOfVariable(char name[])
{
    for (int i = 0; i < variableCounter; i++)
    {
        if (!strcoll(name, variables[i].name))
        {
            return variables[i].value;
        }
    }
}

char* cloneInt(char value[])
{
    static char *clone;
    clone = (char *) calloc(sizeof(char), MAX_INT_CONST_LEN + 1);
    int len =strlen(value);
    for (int i = 0; i < len; i++)
    {
        clone[i] = value[i];
    }

    return clone;
}

void reverseInt(char value[])
{
    char *newValue = (char *) calloc(sizeof(char), MAX_INT_CONST_LEN + 1);
    int len = strlen(value);
    for (int i = 0; i < len; i++)
    {
        newValue[i] = value[len - i -1];
    }
    assign(value,newValue);
    free(newValue);
}

//Adds unsigned number1 to number2 then returns the result
char* add(char number1[], char number2[])
{
    int len1 = strlen(number1);
    int len2 = strlen(number2);
    
    int len = (len1 > len2) ? len1 : len2;
    static char result[101] = "";
    char c = '0';
    char n;

    for (int i = 0; i < len; i++)
    {
        n = c ;

        if (len1 > i)
        {
            n = n + number1[i] - 48;
        }
        if (len2 > i)
        {
            n = n + number2[i] - 48;
        }
        
        if (n > '9')
        {
            c = '1';
            n = n - 10;
        }else
        {
            c = '0';
        }

        result[i] = n;
        result[i + 1] = '\0';
    }

    if (c == '1')
    {
        if (len < MAX_INT_CONST_LEN - 1)//Max digit check without sign 
        {
            result[len] = '1';
            result[len + 1] = '\0';
        }else
        {
            sprintf(errorMessage,"EROR line %d! An integer can be as big as 100 decimal digits. Result exceeds int size", currentLine);
            displayErorAndExit();
        }
    }

    return result;
}

//Subtracts unsigned number1 than number2 then returns the result
//number1 must be greater or equal then number2
char* subtract(char number1[], char number2[])
{
    int len1 = strlen(number1);
    int len2 = strlen(number2);
    
    int len = (len1 > len2) ? len1 : len2;
    static char result[101] = "";
    char c = '0';
    char n;

    for (int i = 0; i < len; i++)
    {
        n = c ;

        if (len1 > i)
        {
            n = n + number1[i] - 48;
        }
        if (len2 > i)
        {
            n = n - number2[i] + 48;
        }
        
        if (n < '0')
        {
            c = '/';
            n = n + 10;
        }else
        {
            c = '0';
        }

        result[i] = n;
        result[i + 1] = '\0';
    }

    int lenTotal = strlen(result);

    int i = 1;
    while (result[lenTotal - i] == '0')//Clears the leding zeros
    {
        result[lenTotal - i] = '\0';
        i++;
    }

    return result;
}

//If number 1 is greater than number 2 returns 1, if not returns 0
int isGreater(char number1[], char number2[])
{
    int len1 = strlen(number1);
    int len2 = strlen(number2);

    if (len1 > len2)
    {
        return 1;

    }else if (len2 > len1)
    {
        return 0;

    }else
    {
        for (int i = 0; i < len1; i++)
        {
            if (number1[i] > number2[i])
            {
                return 1;

            }else if (number2[i] > number1[i])
            {
                return 0;
            }
        }
        return 0;
    }
}

void printInt(char value[])
{
    int len = strlen(value);
    if (len == 0 || (len == 1 && value[0] == '-'))
    {
        printf("0");
        return;
    }
    
    for (int i = len - 1; i >= 0; i--)
    {
        printf("%c", value[i]);
    }
    
}