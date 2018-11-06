#include <iostream>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE_ERROR -1
#define STRING_ERROR -2
#define STRING_ARRAY_ERROR -3
#define OUTPUT_FILE_ERROR -4
#define INVALID_N_STRINGS -5
#define UNKNOWN_ERROR -6

/**
    \brief Count length of file

    \param[in] file Input file pointer
    \return Length of file
*/
int FileLength(FILE * file);


/**
    \brief Reads all text from file

    Reads all text from file

    \param[in] file Input file pointer
    \return String (char *) with text from file
*/
char * ReadText(FILE * file);


/**
    \brief Writes text into file

    \param[in] strs array of strings (read char pointers) which should be written to file
    \param[in] N_strings Number of strings in strs
    \param[in] file Output file
    \return 0 if all is normal
    \return OUTPUT_FILE_ERROR if file is NULL
    \return STRING_ARRAY_ERROR if strs is NULL
    \return INVALID_N_STRINGS if number of strings is incorrect (e.g. less 0)
*/
int WriteText(char ** strs, int N_strings, FILE * file);


/**
    \brief Splits string by symbol

    Splits string by symbol 'c', writes strings into array and replace all symbols 'c' to '\0'

    \param[in] text Text to split
    \param[in] c Symbol
    \param[out] strs Array of strings

    \return Number of strings in text
    \return NO_STRING_ERROR if text is NULL
    \return NO_STRING_ARRAY_ERROR if strs if NULL
*/
int StrSplit(char * text, char c, char *** strs);


/**
    \brief Compares 2 strings

    \param[in] p1 const pointer on the 1st string
    \param[in] p2 const pointer on the 2nd string
    \return Number less than 0 if 1st string is less than 2nd
    \return Number greater then 0 if 2nd string is less than 1st
    \return 0 if strings are equal
*/
int CmpBegin(const void * p1, const void * p2);


/**
*/
int CmpEnd(const void * p1, const void * p2);


/*****************************************************************************************************************************************************************************************************/


int main()
{
    int N_strings = -1;
    FILE * finput = fopen("Onegin2.txt", "r");
    FILE * foutput = fopen("NewOnegin.txt", "w");
    char ** strs = NULL;
    char * text = ReadText(finput);
    if (text == NULL)
    {
        fprintf(stderr, "Input file error, see stderr\n");
        return INPUT_FILE_ERROR;
    }

    N_strings = StrSplit(text, '\n', &strs);
    if (N_strings == STRING_ERROR)
    {
        return STRING_ERROR;
    }
    if (N_strings == STRING_ARRAY_ERROR)
    {
        return STRING_ARRAY_ERROR;
    }

    WriteText(strs, N_strings, foutput);
    qsort(strs, N_strings, sizeof(char *), CmpBegin);
    WriteText(strs, N_strings, foutput);
    qsort(strs, N_strings, sizeof(char *), CmpEnd);
    WriteText(strs, N_strings, foutput);
    return 0;
}


/*****************************************************************************************************************************************************************************************************/


int FileLength(FILE * file)
{
    if (file == NULL)
    {
        fprintf(stderr, "Input file error\n");
        return INPUT_FILE_ERROR;
    }

    int a = -1, b = -1;
    if (fseek(file, 0, SEEK_SET) != 0 || (a = ftell(file)) == -1)
    {
        fprintf(stderr, "Failed to move file pointer to the begin\n");
        return UNKNOWN_ERROR;
    }
    if (fseek(file, 0, SEEK_END) != 0 || (b = ftell(file)) == -1)
    {
        fprintf(stderr, "Failed to move file pointer to the end\n");
        return UNKNOWN_ERROR;
    }
    if ((b - a) < 0)
    {
        fprintf(stderr, "Something is wrong here, size of file is less than zero\n");
        return UNKNOWN_ERROR;
    }
    return (b - a);
}


char * ReadText(FILE * file)
{
    if (file == NULL)
    {
        fprintf(stderr, "Input file error\n");
        return NULL;
    }
    int len = FileLength(file);
    if (len == INPUT_FILE_ERROR)
    {
        fprintf(stderr, "Input file error\n");
        return NULL;
    }
    if (len == UNKNOWN_ERROR)
    {
        fprintf(stderr, "Unknown error\n");
        return NULL;
    }
    char * text = (char *)calloc(len + 1, sizeof(char));
    fseek(file, 0, SEEK_SET);
    if (fread(text, len, 1, file) != 1)
    {
        fprintf(stderr, "Failed to read text from file\n");
        return NULL;
    }
    return text;
}


int WriteText(char ** strs, int N_strings, FILE * file)
{
    if (file == NULL)
    {
        fprintf(stderr, "Output file error\n");
        return OUTPUT_FILE_ERROR;
    }
    if (strs == NULL)
    {
        fprintf(stderr, "No strings array\n");
        return STRING_ARRAY_ERROR;
    }
    if (N_strings < 0)
    {
        fprintf(stderr, "There are %d strings in text, it's strange\n", N_strings);
        return INVALID_N_STRINGS;
    }
    for (int i = 0; i < N_strings; i++)
    {
        fprintf(file, "%s\n", strs[i]);
    }
    return 0;
}


int StrSplit(char * text, char c, char *** strs_p)
{
    if (text == NULL)
    {
        fprintf(stderr, "Text nullptr, go by forest\n");
        return STRING_ERROR;
    }

    int N_strings = 0, i = 0, j = 0;
    while (1)
    {
        if (text[i] == '\0')
        {
            N_strings++;
            break;
        }
        if (text[i] == c)
        {
            text[i] = '\0';
            N_strings++;
        }
        i++;
    }
    *strs_p = (char **)calloc(N_strings, sizeof(char *));
    char ** strs = *strs_p;
    if (strs == NULL)
    {
        fprintf(stderr, "Failed allocating mem for strings array\n");
        return STRING_ARRAY_ERROR;
    }
    strs[0] = text;
    for (i = 0; i < N_strings - 1; i++)
    {
        for (j = 0; strs[i][j] != '\0'; j++);
        strs[i + 1] = &(strs[i][j + 1]);
    }
    return N_strings;
}


int CmpBegin(const void * p1, const void * p2)
{
    return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int CmpEnd(const void * p1, const void * p2)
{
    char * s1 = *(char * const *)p1;
    char * s2 = *(char * const *)p2;
    printf("Compare: \"%s\", \"%s\";\n", s1, s2);
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    char * s1_ = (char *)calloc(len1, sizeof(char));
    strncpy(s1_, s1, len1 + 1);
    char * s2_ = (char *)calloc(len2, sizeof(char));
    strncpy(s2_, s2, len2 + 1);
    printf("Copies:  \"%s\", \"%s\";\n", s1_, s2_);

    for (int i = 0; i < len1 + 2; i++)
        printf("%d ", s1[i]);
    printf("\n");
    for (int i = 0; i < len1 + 2; i++)
        printf("%d ", s1_[i]);
    printf("\n");

    char c = 0;
    for (int i = 0; i < len1 / 2; i++)
    {
        printf("[%d, %d] -> ", s1_[i], s1_[len1 - 1 - i]);
        c = s1_[i];
        s1_[i] = s1_[len1 - 1 - i];
        s1_[len1 - 1 - i] = c;
        printf("[%d, %d]\n", s1_[i], s1_[len1 - 1 - i]);
    }
    for (int i = 0; i < len2 / 2; i++)
    {
        c = s2_[i];
        s2_[i] = s2_[len2 - 1 - i];
        s2_[len2 - 1 - i] = c;
    }
    printf("Reverse: \"%s\", \"%s\";\n", s1_, s2_);
    printf("\n");
    return strcmp(s1_, s2_);
}
