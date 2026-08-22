#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

FILE *inf;
FILE *outf;

void writeRecord(char *line)
{
    fwrite(line,80,1,outf);
}

void makeString(char *line)
{
    int i;

    for (i = 74; i < 80; i++)
        line??(i??) = ' ';

    line??(80??) = 0;
}

int check_key_words(char a??(81??))
{
    int return_flag = 0;

    char *keys??(50??) =
    {
        "ACCEPT",
        "ADD",
        "CALL",
        "CANCEL",
        "CLOSE",
        "COMPUTE",
        "CONTINUE",
        "DELETE",
        "DISPLAY",
        "DIVIDE",
        "EVALUATE",
        "EXEC",
        "EXIT",
        "GO",
        "GOBACK",
        "IF",
        "INITIALIZE",
        "INSPECT",
        "MERGE",
        "MOVE",
        "MULTIPLY",
        "OPEN",
        "PERFORM",
        "READ",
        "REWRITE",
        "SEARCH",
        "SET",
        "SORT",
        "START",
        "STOP",
        "STRING",
        "SUBTRACT",
        "UNSTRING",
        "WRITE",
        "END-IF",
        "END-PERFORM",
        "END-READ",
        "END-WRITE",
        "FUNCTION",
        "END-EVALUATE",
        "SPACE",
        "SPACES",
        "ZERO",
        "ZEROS",
        "HIGH-VALUES",
        "LOW-VALUES",
        "MOVE 0"
    };

    int i;

    for (i = 0; i < 47; i++)
    {
        if (strstr(a,keys??(i??)) != NULL)
        {
            return_flag = 1;
            break;
        }
    }

    return return_flag;
}

int main()
{
    char other??(81??);
    char disp??(81??);
    char var??(77??);
    char pending??(81??);

    char *toPos;

    int inProcedure;
    int hasPending;
    int done_to_log;

    int i;
    int j;

    puts("start");

    inProcedure = 0;
    hasPending = 0;

    inf = fopen("DD:INPUTF","rb");

    if (inf == NULL)
    {
        printf("INPUTF OPEN ERROR\n");
        exit(12);
    }

    outf = fopen("DD:OUTPUTF","wb");

    if (outf == NULL)
    {
        printf("OUTPUTF OPEN ERROR\n");

        fclose(inf);

        exit(12);
    }

    puts("before while");

    while (1)
    {
        /*
         * If we already read the next COBOL statement
         * while looking ahead, use it now.
         */
        if (hasPending)
        {
            for (j = 0; j < 81; j++)
                other??(j??) = pending??(j??);

            hasPending = 0;
        }
        else
        {
            if (fread(other,80,1,inf) != 1)
                break;

            makeString(other);
        }

        /*
         * Wait until PROCEDURE DIVISION.
         */
        if (!inProcedure)
        {
            writeRecord(other);

            if (strstr(other,"PROCEDURE") != NULL)
                inProcedure = 1;

            continue;
        }

        /*
         * Comment line.
         */
        if (other??(6??) == '*')
        {
            writeRecord(other);
            continue;
        }

        /*
         * Not a MOVE.
         */
        if (strstr(other,"MOVE ") == NULL)
        {
            writeRecord(other);
            continue;
        }

        /*
         * Do not process these MOVE statements.
         */
        if (strstr(other," SPACE ") != NULL ||
            strstr(other," SPACES ") != NULL ||
            strstr(other," ZERO ") != NULL ||
            strstr(other," ZEROS ") != NULL ||
            strstr(other," HIGH-VALUE ") != NULL ||
            strstr(other," HIGH-VALUES ") != NULL ||
            strstr(other," LOW-VALUE ") != NULL ||
            strstr(other," LOW-VALUES ") != NULL ||
            strstr(other,"MOVE 0") != NULL)
        {
            writeRecord(other);
            continue;
        }

        /*
         * Ignore MOVE statements containing strings.
         */
        if (strchr(other,'"') != NULL)
        {
            writeRecord(other);
            continue;
        }

        /*
         * Find TO.
         */
        toPos = strstr(other," TO ");

        if (toPos == NULL)
        {
            writeRecord(other);
            continue;
        }

        /*
         * Start with the first destination.
         */
        toPos = toPos + 4;

        while (*toPos == ' ')
            toPos++;

        i = 0;

        while (*toPos != ' ' &&
               *toPos != '.' &&
               *toPos != 0 &&
               i < 70)
        {
            var??(i??) = *toPos;

            toPos++;
            i++;
        }

        var??(i??) = 0;

        /*
         * Write the original MOVE.
         */
        writeRecord(other);

        /*
         * If MOVE already ends with a period,
         * there are no continuation lines.
         */
        done_to_log = 0;

        if (strchr(other,'.') != NULL)
            done_to_log = 1;

        /*
         * Look ahead for continuation lines.
         */
        while (!done_to_log)
        {
            /*
             * Read next record.
             */
            if (fread(other,80,1,inf) != 1)
            {
                done_to_log = 1;
                break;
            }

            makeString(other);

            /*
             * Comment after MOVE.
             *
             * We write it and finish the MOVE.
            
