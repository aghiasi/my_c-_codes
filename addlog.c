#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *inf;
FILE *outf;

int main()
{
    char other??(81??);
    char disp??(81??);
    char var??(77??);
    char *p;
    int i;
    int j;
    int inProcedure;

    puts("start");

    inProcedure = 0;

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

    while (fread(other,80,1,inf) == 1)
    {
        fwrite(other,80,1,outf);

        /* Make record a C string */
        for (j = 74; j < 80; j++)
            other??(j??) = ' ';
        other??(80??) = 0;

        if (!inProcedure)
        {
            if (strstr(other,"PROCEDURE") != NULL)
                inProcedure = 1;

            continue;
        }

        /* Skip comment lines */
        if (other??(6??) == '*')
            continue;

        if (strstr(other,"MOVE ") == NULL)
            continue;

        if (strstr(other," SPACE ")      != NULL ||
            strstr(other," SPACES ")     != NULL ||
            strstr(other," ZERO ")       != NULL ||
            strstr(other," ZEROS ")      != NULL ||
            strstr(other," HIGH-VALUE ") != NULL ||
            strstr(other," LOW-VALUE ")  != NULL ||
            strstr(other,"MOVE 0")       != NULL)
        {
            continue;
        }

        if (strchr(other,'"') != NULL)
            continue;

        p = strstr(other," TO ");
        if (p == NULL)
            continue;

        p = p + 4;

        while (*p == ' ')
            p++;

        i = 0;

        while (*p != '.' &&
               *p != 0 &&
               i < 70)
        {
            var??(i??) = *p;
            p++;
            i++;
        }

        while (i > 0 && var??(i-1??) == ' ')
            i--;

        var??(i??) = 0;

        memset(disp,' ',80);
        sprintf(disp,
                "           MOVE %s",
                var);
        disp??(strlen(disp)??) = ' ';
        fwrite(disp,80,1,outf);

        memset(disp,' ',80);
        sprintf(disp,
                "                TO WA-MY-TEMP");
        disp??(strlen(disp)??) = ' ';
        fwrite(disp,80,1,outf);

        memset(disp,' ',80);
        sprintf(disp,
                "           DISPLAY \"%s\"",
                var);
        disp??(strlen(disp)??) = ' ';
        fwrite(disp,80,1,outf);

        memset(disp,' ',80);
        sprintf(disp,
                "                   WA-MY-TEMP");
        disp??(strlen(disp)??) = ' ';
        fwrite(disp,80,1,outf);
    }

    fclose(inf);
    fclose(outf);

    return 0;
}
