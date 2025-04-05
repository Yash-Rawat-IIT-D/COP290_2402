#include <stdio.h>
#include <stdlib.h>

int main(void) 
{
    /* 
     * STEP 1: Write out a test input file that we will feed into ./sheet.
     *         These commands are just examples to demonstrate usage:
     */
    FILE *fp = fopen("test_input.txt", "w");
    if (!fp)
    {
        perror("Could not create test_input.txt");
        return 1;
    }

    /*
     * Here you can insert any commands you like.
     * For example:
     */
    fprintf(fp, "A1=2\n");
    fprintf(fp, "B1=A1+1\n");
    fprintf(fp, "A2=MAX(A1:B1)\n");
    fprintf(fp, "B2=SLEEP(B1)\n");
    fprintf(fp, "A1=1\n");
    fprintf(fp, "B1=1\n");
    fprintf(fp, "A1=5\n");
    fprintf(fp, "disable_output\n");
    fprintf(fp, "A1=2\n");
    fprintf(fp, "enable_output\n");
    fprintf(fp, "q\n");

    fclose(fp);

    /*
     * STEP 2: Construct the system command that pipes our test input
     *         into the sheet program. If your sheet executable has a 
     *         different name/path, adjust accordingly.
     */
    /* 
     * Example: cat test_input.txt | ./sheet 2 2
     */
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 2 2");

    /*
     * STEP 3: Run that command and simply show the output.
     *         You can capture the output in C by using popen 
     *         if you want to parse it. But for a straightforward 
     *         approach, using system() just prints it to stdout.
     */
    int ret = system(cmd);

    if (ret == -1)
    {
        perror("system() failed");
        return 1;
    }

    /*
     * Optionally, you can remove the temporary file if you don't need it 
     * anymore by uncommenting the following line:
     */
    // remove("test_input.txt");

    return 0;
}
