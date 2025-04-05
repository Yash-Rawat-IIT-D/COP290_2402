// // // // // // // // // // // // // #include <stdio.h>
// // // // // // // // // // // // // #include <stdlib.h>

// // // // // // // // // // // // // int main(void) 
// // // // // // // // // // // // // {
// // // // // // // // // // // // //     /* 
// // // // // // // // // // // // //      * STEP 1: Write out a test input file that we will feed into ./sheet.
// // // // // // // // // // // // //      *         These commands are just examples to demonstrate usage:
// // // // // // // // // // // // //      */
// // // // // // // // // // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // // // // // // // // // //     if (!fp)
// // // // // // // // // // // // //     {
// // // // // // // // // // // // //         perror("Could not create test_input.txt");
// // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // //     }

// // // // // // // // // // // // //     /*
// // // // // // // // // // // // //      * Here you can insert any commands you like.
// // // // // // // // // // // // //      * For example:
// // // // // // // // // // // // //      */
// // // // // // // // // // // // //     fprintf(fp, "A1=2\n");
// // // // // // // // // // // // //     fprintf(fp, "B1=A1+1\n");
// // // // // // // // // // // // //     fprintf(fp, "A2=MAX(A1:B1)\n");
// // // // // // // // // // // // //     fprintf(fp, "B2=SLEEP(B1)\n");
// // // // // // // // // // // // //     fprintf(fp, "A1=1\n");
// // // // // // // // // // // // //     fprintf(fp, "B1=1\n");
// // // // // // // // // // // // //     fprintf(fp, "A1=5\n");
// // // // // // // // // // // // //     fprintf(fp, "disable_output\n");
// // // // // // // // // // // // //     fprintf(fp, "A1=2\n");
// // // // // // // // // // // // //     fprintf(fp, "enable_output\n");
// // // // // // // // // // // // //     fprintf(fp, "q\n");

// // // // // // // // // // // // //     fclose(fp);

// // // // // // // // // // // // //     /*
// // // // // // // // // // // // //      * STEP 2: Construct the system command that pipes our test input
// // // // // // // // // // // // //      *         into the sheet program. If your sheet executable has a 
// // // // // // // // // // // // //      *         different name/path, adjust accordingly.
// // // // // // // // // // // // //      */
// // // // // // // // // // // // //     /* 
// // // // // // // // // // // // //      * Example: cat test_input.txt | ./sheet 2 2
// // // // // // // // // // // // //      */
// // // // // // // // // // // // //     char cmd[256];
// // // // // // // // // // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");

// // // // // // // // // // // // //     /*
// // // // // // // // // // // // //      * STEP 3: Run that command and simply show the output.
// // // // // // // // // // // // //      *         You can capture the output in C by using popen 
// // // // // // // // // // // // //      *         if you want to parse it. But for a straightforward 
// // // // // // // // // // // // //      *         approach, using system() just prints it to stdout.
// // // // // // // // // // // // //      */
// // // // // // // // // // // // //     int ret = system(cmd);

// // // // // // // // // // // // //     if (ret == -1)
// // // // // // // // // // // // //     {
// // // // // // // // // // // // //         perror("system() failed");
// // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // //     }

// // // // // // // // // // // // //     /*
// // // // // // // // // // // // //      * Optionally, you can remove the temporary file if you don't need it 
// // // // // // // // // // // // //      * anymore by uncommenting the following line:
// // // // // // // // // // // // //      */
// // // // // // // // // // // // //     // remove("test_input.txt");

// // // // // // // // // // // // //     return 0;
// // // // // // // // // // // // // }


// // // // // // // // // // // // #include <stdio.h>
// // // // // // // // // // // // #include <stdlib.h>
// // // // // // // // // // // // #include <string.h>

// // // // // // // // // // // // int main(void)
// // // // // // // // // // // // {
// // // // // // // // // // // //     /* STEP 1: Write out a test input file to feed into ./sheet */
// // // // // // // // // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // // // // // // // // //     if (!fp)
// // // // // // // // // // // //     {
// // // // // // // // // // // //         perror("Could not create test_input.txt");
// // // // // // // // // // // //         return 1;
// // // // // // // // // // // //     }
// // // // // // // // // // // //     /* Sample commands */
// // // // // // // // // // // //     fprintf(fp, "A1=2\n");
// // // // // // // // // // // //     fprintf(fp, "B1=A1+1\n");
// // // // // // // // // // // //     fprintf(fp, "A2=MAX(A1:B1)\n");
// // // // // // // // // // // //     fprintf(fp, "B2=SLEEP(B1)\n");
// // // // // // // // // // // //     fprintf(fp, "A1=1\n");
// // // // // // // // // // // //     fprintf(fp, "B1=1\n");
// // // // // // // // // // // //     fprintf(fp, "A1=5\n");
// // // // // // // // // // // //     fprintf(fp, "disable_output\n");
// // // // // // // // // // // //     fprintf(fp, "A1=2\n");
// // // // // // // // // // // //     fprintf(fp, "enable_output\n");
// // // // // // // // // // // //     fprintf(fp, "q\n");
// // // // // // // // // // // //     fclose(fp);

// // // // // // // // // // // //     /* STEP 2: Construct the command that pipes test_input.txt into sheet. */
// // // // // // // // // // // //     char cmd[256];
// // // // // // // // // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");

// // // // // // // // // // // //     /* STEP 3: Use popen() to capture the output of the command */
// // // // // // // // // // // //     FILE *pipe = popen(cmd, "r");
// // // // // // // // // // // //     if (!pipe)
// // // // // // // // // // // //     {
// // // // // // // // // // // //         perror("popen() failed");
// // // // // // // // // // // //         return 1;
// // // // // // // // // // // //     }

// // // // // // // // // // // //     /* STEP 4: Open the output file where we will write the aligned output */
// // // // // // // // // // // //     FILE *out_fp = fopen("aligned_output.txt", "w");
// // // // // // // // // // // //     if (!out_fp)
// // // // // // // // // // // //     {
// // // // // // // // // // // //         perror("Could not open aligned_output.txt for writing");
// // // // // // // // // // // //         pclose(pipe);
// // // // // // // // // // // //         return 1;
// // // // // // // // // // // //     }

// // // // // // // // // // // //     const char *prompt = "[0.0] (ok)>";
// // // // // // // // // // // //     size_t promptLen = strlen(prompt);
// // // // // // // // // // // //     char buffer[1024];

// // // // // // // // // // // //     /*
// // // // // // // // // // // //      * Read the output line by line. For each line, if the prompt is found,
// // // // // // // // // // // //      * insert a newline right after it.
// // // // // // // // // // // //      */
// // // // // // // // // // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // // // // // // // // // //     {
// // // // // // // // // // // //         char *start = buffer;
// // // // // // // // // // // //         char *found = NULL;
// // // // // // // // // // // //         while ((found = strstr(start, prompt)) != NULL)
// // // // // // // // // // // //         {
// // // // // // // // // // // //             /* Write everything up to the prompt */
// // // // // // // // // // // //             fwrite(start, 1, found - start, out_fp);
// // // // // // // // // // // //             /* Write the prompt itself */
// // // // // // // // // // // //             fwrite(prompt, 1, promptLen, out_fp);
// // // // // // // // // // // //             /* Insert a newline so that subsequent text starts on a new line */
// // // // // // // // // // // //             fputc('\n', out_fp);
// // // // // // // // // // // //             /* Advance the pointer past the prompt */
// // // // // // // // // // // //             start = found + promptLen;
// // // // // // // // // // // //         }
// // // // // // // // // // // //         /* Write any remaining text from the line */
// // // // // // // // // // // //         fputs(start, out_fp);
// // // // // // // // // // // //     }

// // // // // // // // // // // //     pclose(pipe);
// // // // // // // // // // // //     fclose(out_fp);

// // // // // // // // // // // //     /* Optionally, remove the temporary file */
// // // // // // // // // // // //     // remove("test_input.txt");

// // // // // // // // // // // //     return 0;
// // // // // // // // // // // // }



// // // // // // // // // // // #include <stdio.h>
// // // // // // // // // // // #include <stdlib.h>
// // // // // // // // // // // #include <string.h>

// // // // // // // // // // // #define BUFFER_SIZE 1024

// // // // // // // // // // // // Helper function: Process a buffer by inserting a newline after every occurrence of (ok)>
// // // // // // // // // // // void process_line(const char *input, FILE *out_fp) {
// // // // // // // // // // //     const char *prompt = "(ok)>";
// // // // // // // // // // //     size_t promptLen = strlen(prompt);
// // // // // // // // // // //     const char *start = input;
// // // // // // // // // // //     const char *found = NULL;

// // // // // // // // // // //     while ((found = strstr(start, prompt)) != NULL) {
// // // // // // // // // // //         // Write text up to the prompt.
// // // // // // // // // // //         fwrite(start, 1, found - start, out_fp);
// // // // // // // // // // //         // Write the prompt.
// // // // // // // // // // //         fwrite(prompt, 1, promptLen, out_fp);
// // // // // // // // // // //         // Insert a newline.
// // // // // // // // // // //         fputc('\n', out_fp);
// // // // // // // // // // //         // Move pointer beyond the prompt.
// // // // // // // // // // //         start = found + promptLen;
// // // // // // // // // // //     }
// // // // // // // // // // //     // Write the remaining text.
// // // // // // // // // // //     fputs(start, out_fp);
// // // // // // // // // // // }

// // // // // // // // // // // int main(void)
// // // // // // // // // // // {
// // // // // // // // // // //     /* --- STEP 1: Write the test input file --- */
// // // // // // // // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // // // // // // // //     if (!fp)
// // // // // // // // // // //     {
// // // // // // // // // // //         perror("Could not create test_input.txt");
// // // // // // // // // // //         return 1;
// // // // // // // // // // //     }

// // // // // // // // // // //     // Sample test commands (customize as needed)
// // // // // // // // // // //     fprintf(fp, "A1=2\n");
// // // // // // // // // // //     fprintf(fp, "B1=A1+1\n");
// // // // // // // // // // //     fprintf(fp, "A2=MAX(A1:B1)\n");
// // // // // // // // // // //     fprintf(fp, "B2=SLEEP(B1)\n");
// // // // // // // // // // //     fprintf(fp, "A1=1\n");
// // // // // // // // // // //     fprintf(fp, "B1=1\n");
// // // // // // // // // // //     fprintf(fp, "A1=5\n");
// // // // // // // // // // //     fprintf(fp, "disable_output\n");
// // // // // // // // // // //     fprintf(fp, "A1=2\n");
// // // // // // // // // // //     fprintf(fp, "enable_output\n");
// // // // // // // // // // //     fprintf(fp, "q\n");
// // // // // // // // // // //     fclose(fp);

// // // // // // // // // // //     /* --- STEP 2: Run the sheet program and write processed output to aligned_output.txt --- */
// // // // // // // // // // //     char cmd[256];
// // // // // // // // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");

// // // // // // // // // // //     FILE *pipe = popen(cmd, "r");
// // // // // // // // // // //     if (!pipe)
// // // // // // // // // // //     {
// // // // // // // // // // //         perror("popen() failed");
// // // // // // // // // // //         return 1;
// // // // // // // // // // //     }

// // // // // // // // // // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // // // // // // // // // //     if (!aligned_fp)
// // // // // // // // // // //     {
// // // // // // // // // // //         perror("Could not open aligned_output.txt for writing");
// // // // // // // // // // //         pclose(pipe);
// // // // // // // // // // //         return 1;
// // // // // // // // // // //     }

// // // // // // // // // // //     char buffer[BUFFER_SIZE];
// // // // // // // // // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // // // // // // // // //     {
// // // // // // // // // // //         // Process each line: if (ok)> appears, insert a newline immediately after it.
// // // // // // // // // // //         process_line(buffer, aligned_fp);
// // // // // // // // // // //     }
// // // // // // // // // // //     pclose(pipe);
// // // // // // // // // // //     fclose(aligned_fp);

// // // // // // // // // // //     /* --- STEP 3: Read from aligned_output.txt and write final output to output.txt --- */
// // // // // // // // // // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // // // // // // // // // //     if (!aligned_in)
// // // // // // // // // // //     {
// // // // // // // // // // //         perror("Could not open aligned_output.txt for reading");
// // // // // // // // // // //         return 1;
// // // // // // // // // // //     }

// // // // // // // // // // //     FILE *final_fp = fopen("output.txt", "w");
// // // // // // // // // // //     if (!final_fp)
// // // // // // // // // // //     {
// // // // // // // // // // //         perror("Could not open output.txt for writing");
// // // // // // // // // // //         fclose(aligned_in);
// // // // // // // // // // //         return 1;
// // // // // // // // // // //     }

// // // // // // // // // // //     // Read each line from the aligned file and process it one more time for safety.
// // // // // // // // // // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // // // // // // // // // //     {
// // // // // // // // // // //         process_line(buffer, final_fp);
// // // // // // // // // // //     }
// // // // // // // // // // //     fclose(aligned_in);
// // // // // // // // // // //     fclose(final_fp);

// // // // // // // // // // //     printf("Final output is written to output.txt\n");
// // // // // // // // // // //     return 0;
// // // // // // // // // // // }


// // // // // // // // // // #include <stdio.h>
// // // // // // // // // // #include <stdlib.h>
// // // // // // // // // // #include <string.h>

// // // // // // // // // // #define BUFFER_SIZE 1024

// // // // // // // // // // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // // // // // // // // // void process_line(const char *input, FILE *out_fp) {
// // // // // // // // // //     const char *prompt = "(ok)>";
// // // // // // // // // //     size_t promptLen = strlen(prompt);
// // // // // // // // // //     const char *start = input;
// // // // // // // // // //     const char *found;
// // // // // // // // // //     while ((found = strstr(start, prompt)) != NULL) {
// // // // // // // // // //         // Write text up to the prompt.
// // // // // // // // // //         fwrite(start, 1, found - start, out_fp);
// // // // // // // // // //         // Write the prompt.
// // // // // // // // // //         fwrite(prompt, 1, promptLen, out_fp);
// // // // // // // // // //         // Insert a newline.
// // // // // // // // // //         fputc('\n', out_fp);
// // // // // // // // // //         // Advance the pointer past the prompt.
// // // // // // // // // //         start = found + promptLen;
// // // // // // // // // //     }
// // // // // // // // // //     // Write any remaining text.
// // // // // // // // // //     fputs(start, out_fp);
// // // // // // // // // // }

// // // // // // // // // // int main(void)
// // // // // // // // // // {
// // // // // // // // // //     /* === STEP 1: Write Test Case 1 input === */
// // // // // // // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // // // // // // //     if (!fp)
// // // // // // // // // //     {
// // // // // // // // // //         perror("Could not create test_input.txt");
// // // // // // // // // //         return 1;
// // // // // // // // // //     }
// // // // // // // // // //     // Test Case 1:
// // // // // // // // // //     // Scrolling behaviour check including end-of-file scrolling cases and
// // // // // // // // // //     // disable/enable output when already disabled/enabled.
// // // // // // // // // //     fprintf(fp, "A1=1\n");
// // // // // // // // // //     // Generate several rows to force scrolling.
// // // // // // // // // //     for (int i = 2; i <= 30; i++) {
// // // // // // // // // //         fprintf(fp, "A%d=%d\n", i, i);
// // // // // // // // // //     }
// // // // // // // // // //     // Disable output twice then re-enable twice.
// // // // // // // // // //     fprintf(fp, "disable_output\n");
// // // // // // // // // //     fprintf(fp, "disable_output\n"); // Already disabled.
// // // // // // // // // //     fprintf(fp, "enable_output\n");
// // // // // // // // // //     fprintf(fp, "enable_output\n");  // Already enabled.
// // // // // // // // // //     fprintf(fp, "q\n");
// // // // // // // // // //     fclose(fp);

// // // // // // // // // //     /* === STEP 2: Pipe the test input into your sheet program === */
// // // // // // // // // //     char cmd[256];
// // // // // // // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");

// // // // // // // // // //     FILE *pipe = popen(cmd, "r");
// // // // // // // // // //     if (!pipe)
// // // // // // // // // //     {
// // // // // // // // // //         perror("popen() failed");
// // // // // // // // // //         return 1;
// // // // // // // // // //     }

// // // // // // // // // //     /* === STEP 3: Process output and write to aligned_output.txt === */
// // // // // // // // // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // // // // // // // // //     if (!aligned_fp)
// // // // // // // // // //     {
// // // // // // // // // //         perror("Could not open aligned_output.txt for writing");
// // // // // // // // // //         pclose(pipe);
// // // // // // // // // //         return 1;
// // // // // // // // // //     }
// // // // // // // // // //     char buffer[BUFFER_SIZE];
// // // // // // // // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // // // // // // // //     {
// // // // // // // // // //         process_line(buffer, aligned_fp);
// // // // // // // // // //     }
// // // // // // // // // //     pclose(pipe);
// // // // // // // // // //     fclose(aligned_fp);

// // // // // // // // // //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// // // // // // // // // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // // // // // // // // //     if (!aligned_in)
// // // // // // // // // //     {
// // // // // // // // // //         perror("Could not open aligned_output.txt for reading");
// // // // // // // // // //         return 1;
// // // // // // // // // //     }
// // // // // // // // // //     FILE *final_fp = fopen("output.txt", "w");
// // // // // // // // // //     if (!final_fp)
// // // // // // // // // //     {
// // // // // // // // // //         perror("Could not open output.txt for writing");
// // // // // // // // // //         fclose(aligned_in);
// // // // // // // // // //         return 1;
// // // // // // // // // //     }
// // // // // // // // // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // // // // // // // // //     {
// // // // // // // // // //         process_line(buffer, final_fp);
// // // // // // // // // //     }
// // // // // // // // // //     fclose(aligned_in);
// // // // // // // // // //     fclose(final_fp);

// // // // // // // // // //     printf("Test Case 1 executed. Final output is in output.txt\n");
// // // // // // // // // //     return 0;
// // // // // // // // // // }


// // // // // // // // // #include <stdio.h>
// // // // // // // // // #include <stdlib.h>
// // // // // // // // // #include <string.h>
// // // // // // // // // #include <limits.h>

// // // // // // // // // #define BUFFER_SIZE 1024

// // // // // // // // // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // // // // // // // // void process_line(const char *input, FILE *out_fp) {
// // // // // // // // //     const char *prompt = "(ok)>";
// // // // // // // // //     size_t promptLen = strlen(prompt);
// // // // // // // // //     const char *start = input;
// // // // // // // // //     const char *found;
// // // // // // // // //     while ((found = strstr(start, prompt)) != NULL) {
// // // // // // // // //         // Write text up to the prompt.
// // // // // // // // //         fwrite(start, 1, found - start, out_fp);
// // // // // // // // //         // Write the prompt.
// // // // // // // // //         fwrite(prompt, 1, promptLen, out_fp);
// // // // // // // // //         // Insert a newline.
// // // // // // // // //         fputc('\n', out_fp);
// // // // // // // // //         // Advance the pointer past the prompt.
// // // // // // // // //         start = found + promptLen;
// // // // // // // // //     }
// // // // // // // // //     // Write any remaining text.
// // // // // // // // //     fputs(start, out_fp);
// // // // // // // // // }

// // // // // // // // // int main(void)
// // // // // // // // // {
// // // // // // // // //     /* === STEP 1: Write Test Case 2 input === */
// // // // // // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // // // // // //     if (!fp)
// // // // // // // // //     {
// // // // // // // // //         perror("Could not create test_input.txt");
// // // // // // // // //         return 1;
// // // // // // // // //     }
    
// // // // // // // // //     // Test Case 2:
// // // // // // // // //     // - Different value inputs, arithmetic operations, parsing of unary operators,
// // // // // // // // //     // - Assignment check for INT_MAX and INT_MIN.
// // // // // // // // //     fprintf(fp, "A1=%d\n", INT_MAX);  // Test assignment for INT_MAX.
// // // // // // // // //     fprintf(fp, "B1=%d\n", INT_MIN);  // Test assignment for INT_MIN.
// // // // // // // // //     fprintf(fp, "C1=+100\n");         // Test parsing of unary plus.
// // // // // // // // //     fprintf(fp, "D1=-50\n");          // Test parsing of unary minus.
// // // // // // // // //     fprintf(fp, "E1=A1+B1\n");        // Arithmetic operation.
// // // // // // // // //     fprintf(fp, "q\n");
// // // // // // // // //     fclose(fp);

// // // // // // // // //     /* === STEP 2: Pipe the test input into your sheet program === */
// // // // // // // // //     char cmd[256];
// // // // // // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");
// // // // // // // // //     FILE *pipe = popen(cmd, "r");
// // // // // // // // //     if (!pipe)
// // // // // // // // //     {
// // // // // // // // //         perror("popen() failed");
// // // // // // // // //         return 1;
// // // // // // // // //     }

// // // // // // // // //     /* === STEP 3: Process output and write to aligned_output.txt === */
// // // // // // // // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // // // // // // // //     if (!aligned_fp)
// // // // // // // // //     {
// // // // // // // // //         perror("Could not open aligned_output.txt for writing");
// // // // // // // // //         pclose(pipe);
// // // // // // // // //         return 1;
// // // // // // // // //     }
// // // // // // // // //     char buffer[BUFFER_SIZE];
// // // // // // // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // // // // // // //     {
// // // // // // // // //         process_line(buffer, aligned_fp);
// // // // // // // // //     }
// // // // // // // // //     pclose(pipe);
// // // // // // // // //     fclose(aligned_fp);

// // // // // // // // //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// // // // // // // // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // // // // // // // //     if (!aligned_in)
// // // // // // // // //     {
// // // // // // // // //         perror("Could not open aligned_output.txt for reading");
// // // // // // // // //         return 1;
// // // // // // // // //     }
// // // // // // // // //     FILE *final_fp = fopen("output.txt", "w");
// // // // // // // // //     if (!final_fp)
// // // // // // // // //     {
// // // // // // // // //         perror("Could not open output.txt for writing");
// // // // // // // // //         fclose(aligned_in);
// // // // // // // // //         return 1;
// // // // // // // // //     }
// // // // // // // // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // // // // // // // //     {
// // // // // // // // //         process_line(buffer, final_fp);
// // // // // // // // //     }
// // // // // // // // //     fclose(aligned_in);
// // // // // // // // //     fclose(final_fp);

// // // // // // // // //     printf("Test Case 2 executed. Final output is in output.txt\n");
// // // // // // // // //     return 0;
// // // // // // // // // }



// // // // // // // // #include <stdio.h>
// // // // // // // // #include <stdlib.h>
// // // // // // // // #include <string.h>

// // // // // // // // #define BUFFER_SIZE 1024

// // // // // // // // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // // // // // // // void process_line(const char *input, FILE *out_fp) {
// // // // // // // //     const char *prompt = "(ok)>";
// // // // // // // //     size_t promptLen = strlen(prompt);
// // // // // // // //     const char *start = input;
// // // // // // // //     const char *found;
    
// // // // // // // //     while ((found = strstr(start, prompt)) != NULL) {
// // // // // // // //         // Write text up to the prompt.
// // // // // // // //         fwrite(start, 1, found - start, out_fp);
// // // // // // // //         // Write the prompt.
// // // // // // // //         fwrite(prompt, 1, promptLen, out_fp);
// // // // // // // //         // Insert a newline.
// // // // // // // //         fputc('\n', out_fp);
// // // // // // // //         // Advance the pointer past the prompt.
// // // // // // // //         start = found + promptLen;
// // // // // // // //     }
// // // // // // // //     // Write any remaining text.
// // // // // // // //     fputs(start, out_fp);
// // // // // // // // }

// // // // // // // // int main(void)
// // // // // // // // {
// // // // // // // //     /* === STEP 1: Write Test Case 3 input === */
// // // // // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // // // // //     if (!fp)
// // // // // // // //     {
// // // // // // // //         perror("Could not create test_input.txt");
// // // // // // // //         return 1;
// // // // // // // //     }
// // // // // // // //     // Test Case 3:
// // // // // // // //     // Checking working behaviour of all functions and automatic updates.
// // // // // // // //     // Initial assignments and function evaluations.
// // // // // // // //     fprintf(fp, "A1=2\n");
// // // // // // // //     fprintf(fp, "B1=A1+3\n");
// // // // // // // //     fprintf(fp, "C1=MAX(A1:B1)\n");
// // // // // // // //     fprintf(fp, "D1=MIN(A1:B1)\n");
// // // // // // // //     fprintf(fp, "E1=AVG(A1:B1)\n");
// // // // // // // //     // Update a cell to trigger automatic recalculations.
// // // // // // // //     fprintf(fp, "A1=5\n");
// // // // // // // //     // Exit command.
// // // // // // // //     fprintf(fp, "q\n");
// // // // // // // //     fclose(fp);

// // // // // // // //     /* === STEP 2: Pipe the test input into your sheet program === */
// // // // // // // //     char cmd[256];
// // // // // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");
    
// // // // // // // //     FILE *pipe = popen(cmd, "r");
// // // // // // // //     if (!pipe)
// // // // // // // //     {
// // // // // // // //         perror("popen() failed");
// // // // // // // //         return 1;
// // // // // // // //     }

// // // // // // // //     /* === STEP 3: Process output and write to aligned_output.txt === */
// // // // // // // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // // // // // // //     if (!aligned_fp)
// // // // // // // //     {
// // // // // // // //         perror("Could not open aligned_output.txt for writing");
// // // // // // // //         pclose(pipe);
// // // // // // // //         return 1;
// // // // // // // //     }
    
// // // // // // // //     char buffer[BUFFER_SIZE];
// // // // // // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // // // // // //     {
// // // // // // // //         process_line(buffer, aligned_fp);
// // // // // // // //     }
// // // // // // // //     pclose(pipe);
// // // // // // // //     fclose(aligned_fp);

// // // // // // // //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// // // // // // // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // // // // // // //     if (!aligned_in)
// // // // // // // //     {
// // // // // // // //         perror("Could not open aligned_output.txt for reading");
// // // // // // // //         return 1;
// // // // // // // //     }
    
// // // // // // // //     FILE *final_fp = fopen("output.txt", "w");
// // // // // // // //     if (!final_fp)
// // // // // // // //     {
// // // // // // // //         perror("Could not open output.txt for writing");
// // // // // // // //         fclose(aligned_in);
// // // // // // // //         return 1;
// // // // // // // //     }
    
// // // // // // // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // // // // // // //     {
// // // // // // // //         process_line(buffer, final_fp);
// // // // // // // //     }
    
// // // // // // // //     fclose(aligned_in);
// // // // // // // //     fclose(final_fp);

// // // // // // // //     printf("Test Case 3 executed. Final output is in output.txt\n");
// // // // // // // //     return 0;
// // // // // // // // }


// // // // // // // #include <stdio.h>
// // // // // // // #include <stdlib.h>
// // // // // // // #include <string.h>

// // // // // // // #define BUFFER_SIZE 1024

// // // // // // // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // // // // // // void process_line(const char *input, FILE *out_fp) {
// // // // // // //     const char *prompt = "(ok)>";
// // // // // // //     size_t promptLen = strlen(prompt);
// // // // // // //     const char *start = input;
// // // // // // //     const char *found;
    
// // // // // // //     while ((found = strstr(start, prompt)) != NULL) {
// // // // // // //         // Write text up to the prompt.
// // // // // // //         fwrite(start, 1, found - start, out_fp);
// // // // // // //         // Write the prompt.
// // // // // // //         fwrite(prompt, 1, promptLen, out_fp);
// // // // // // //         // Insert a newline.
// // // // // // //         fputc('\n', out_fp);
// // // // // // //         // Advance the pointer past the prompt.
// // // // // // //         start = found + promptLen;
// // // // // // //     }
// // // // // // //     // Write any remaining text.
// // // // // // //     fputs(start, out_fp);
// // // // // // // }

// // // // // // // int main(void)
// // // // // // // {
// // // // // // //     /* === STEP 1: Write Test Case 4 input === */
// // // // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // // // //     if (!fp)
// // // // // // //     {
// // // // // // //         perror("Could not create test_input.txt");
// // // // // // //         return 1;
// // // // // // //     }
// // // // // // //     // Test Case 4: SLEEP logic testing.
// // // // // // //     // A1 uses SLEEP(1), B1 depends on A1, and C1 uses SLEEP on the result of B1.
// // // // // // //     // The system should stall for recalculations in cells that are directly recalculated,
// // // // // // //     // but not stall for cells that simply use a stored SLEEP value.
// // // // // // //     fprintf(fp, "A1=SLEEP(1)\n");
// // // // // // //     fprintf(fp, "B1=A1+2\n");
// // // // // // //     fprintf(fp, "C1=SLEEP(B1)\n");
// // // // // // //     fprintf(fp, "q\n");
// // // // // // //     fclose(fp);

// // // // // // //     /* === STEP 2: Pipe the test input into your sheet program === */
// // // // // // //     char cmd[256];
// // // // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");
    
// // // // // // //     FILE *pipe = popen(cmd, "r");
// // // // // // //     if (!pipe)
// // // // // // //     {
// // // // // // //         perror("popen() failed");
// // // // // // //         return 1;
// // // // // // //     }

// // // // // // //     /* === STEP 3: Process output and write to aligned_output.txt === */
// // // // // // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // // // // // //     if (!aligned_fp)
// // // // // // //     {
// // // // // // //         perror("Could not open aligned_output.txt for writing");
// // // // // // //         pclose(pipe);
// // // // // // //         return 1;
// // // // // // //     }
    
// // // // // // //     char buffer[BUFFER_SIZE];
// // // // // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // // // // //     {
// // // // // // //         process_line(buffer, aligned_fp);
// // // // // // //     }
// // // // // // //     pclose(pipe);
// // // // // // //     fclose(aligned_fp);

// // // // // // //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// // // // // // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // // // // // //     if (!aligned_in)
// // // // // // //     {
// // // // // // //         perror("Could not open aligned_output.txt for reading");
// // // // // // //         return 1;
// // // // // // //     }
    
// // // // // // //     FILE *final_fp = fopen("output.txt", "w");
// // // // // // //     if (!final_fp)
// // // // // // //     {
// // // // // // //         perror("Could not open output.txt for writing");
// // // // // // //         fclose(aligned_in);
// // // // // // //         return 1;
// // // // // // //     }
    
// // // // // // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // // // // // //     {
// // // // // // //         process_line(buffer, final_fp);
// // // // // // //     }
    
// // // // // // //     fclose(aligned_in);
// // // // // // //     fclose(final_fp);

// // // // // // //     printf("Test Case 4 executed. Final output is in output.txt\n");
// // // // // // //     return 0;
// // // // // // // }


// // // // // // #include <stdio.h>
// // // // // // #include <stdlib.h>
// // // // // // #include <string.h>

// // // // // // #define BUFFER_SIZE 1024

// // // // // // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // // // // // void process_line(const char *input, FILE *out_fp) {
// // // // // //     const char *prompt = "(ok)>";
// // // // // //     size_t promptLen = strlen(prompt);
// // // // // //     const char *start = input;
// // // // // //     const char *found;
    
// // // // // //     while ((found = strstr(start, prompt)) != NULL) {
// // // // // //         // Write text up to the prompt.
// // // // // //         fwrite(start, 1, found - start, out_fp);
// // // // // //         // Write the prompt.
// // // // // //         fwrite(prompt, 1, promptLen, out_fp);
// // // // // //         // Insert a newline.
// // // // // //         fputc('\n', out_fp);
// // // // // //         // Advance the pointer past the prompt.
// // // // // //         start = found + promptLen;
// // // // // //     }
// // // // // //     // Write any remaining text.
// // // // // //     fputs(start, out_fp);
// // // // // // }

// // // // // // int main(void)
// // // // // // {
// // // // // //     /* === STEP 1: Write Test Case 5 input === */
// // // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // // //     if (!fp)
// // // // // //     {
// // // // // //         perror("Could not create test_input.txt");
// // // // // //         return 1;
// // // // // //     }
// // // // // //     // Test Case 5:
// // // // // //     // Checking correct range parsing.
// // // // // //     // Assign values to cells A1, B1, C1, D1 and then calculate the maximum value with MAX(A1:D1) in E1.
// // // // // //     fprintf(fp, "A1=2\n");
// // // // // //     fprintf(fp, "B1=4\n");
// // // // // //     fprintf(fp, "C1=6\n");
// // // // // //     fprintf(fp, "D1=8\n");
// // // // // //     fprintf(fp, "E1=MAX(A1:D1)\n");
// // // // // //     fprintf(fp, "q\n");
// // // // // //     fclose(fp);

// // // // // //     /* === STEP 2: Pipe the test input into your sheet program === */
// // // // // //     char cmd[256];
// // // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");
    
// // // // // //     FILE *pipe = popen(cmd, "r");
// // // // // //     if (!pipe)
// // // // // //     {
// // // // // //         perror("popen() failed");
// // // // // //         return 1;
// // // // // //     }

// // // // // //     /* === STEP 3: Process output and write to aligned_output.txt === */
// // // // // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // // // // //     if (!aligned_fp)
// // // // // //     {
// // // // // //         perror("Could not open aligned_output.txt for writing");
// // // // // //         pclose(pipe);
// // // // // //         return 1;
// // // // // //     }
    
// // // // // //     char buffer[BUFFER_SIZE];
// // // // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // // // //     {
// // // // // //         process_line(buffer, aligned_fp);
// // // // // //     }
// // // // // //     pclose(pipe);
// // // // // //     fclose(aligned_fp);

// // // // // //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// // // // // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // // // // //     if (!aligned_in)
// // // // // //     {
// // // // // //         perror("Could not open aligned_output.txt for reading");
// // // // // //         return 1;
// // // // // //     }
    
// // // // // //     FILE *final_fp = fopen("output.txt", "w");
// // // // // //     if (!final_fp)
// // // // // //     {
// // // // // //         perror("Could not open output.txt for writing");
// // // // // //         fclose(aligned_in);
// // // // // //         return 1;
// // // // // //     }
    
// // // // // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // // // // //     {
// // // // // //         process_line(buffer, final_fp);
// // // // // //     }
    
// // // // // //     fclose(aligned_in);
// // // // // //     fclose(final_fp);

// // // // // //     printf("Test Case 5 executed. Final output is in output.txt\n");
// // // // // //     return 0;
// // // // // // }


// // // // // #include <stdio.h>
// // // // // #include <stdlib.h>
// // // // // #include <string.h>

// // // // // #define BUFFER_SIZE 1024

// // // // // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // // // // void process_line(const char *input, FILE *out_fp) {
// // // // //     const char *prompt = "(ok)>";
// // // // //     size_t promptLen = strlen(prompt);
// // // // //     const char *start = input;
// // // // //     const char *found;
    
// // // // //     while ((found = strstr(start, prompt)) != NULL) {
// // // // //         // Write text up to the prompt.
// // // // //         fwrite(start, 1, found - start, out_fp);
// // // // //         // Write the prompt.
// // // // //         fwrite(prompt, 1, promptLen, out_fp);
// // // // //         // Insert a newline.
// // // // //         fputc('\n', out_fp);
// // // // //         // Advance the pointer past the prompt.
// // // // //         start = found + promptLen;
// // // // //     }
// // // // //     // Write any remaining text.
// // // // //     fputs(start, out_fp);
// // // // // }

// // // // // int main(void)
// // // // // {
// // // // //     /* === STEP 1: Write Test Case 6 input === */
// // // // //     FILE *fp = fopen("test_input.txt", "w");
// // // // //     if (!fp)
// // // // //     {
// // // // //         perror("Could not create test_input.txt");
// // // // //         return 1;
// // // // //     }
    
// // // // //     // Test Case 6:
// // // // //     // Checking recalculations, reassignments, and cyclic dependency checks in a complex dependency tree.
// // // // //     // Initial dependency chain:
// // // // //     //   A1=1, B1=A1+1, C1=B1+1, D1=C1+1.
// // // // //     // Updating A1 should trigger propagation through B1, C1, and D1.
// // // // //     fprintf(fp, "A1=1\n");
// // // // //     fprintf(fp, "B1=A1+1\n");
// // // // //     fprintf(fp, "C1=B1+1\n");
// // // // //     fprintf(fp, "D1=C1+1\n");
// // // // //     // Update A1 to trigger recalculations.
// // // // //     fprintf(fp, "A1=10\n");
// // // // //     // Introduce a cyclic dependency between E1 and F1.
// // // // //     fprintf(fp, "E1=F1+2\n");
// // // // //     fprintf(fp, "F1=E1+2\n");
// // // // //     // End of test input.
// // // // //     fprintf(fp, "q\n");
// // // // //     fclose(fp);

// // // // //     /* === STEP 2: Pipe the test input into your sheet program === */
// // // // //     char cmd[256];
// // // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");
    
// // // // //     FILE *pipe = popen(cmd, "r");
// // // // //     if (!pipe)
// // // // //     {
// // // // //         perror("popen() failed");
// // // // //         return 1;
// // // // //     }

// // // // //     /* === STEP 3: Process output and write to aligned_output.txt === */
// // // // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // // // //     if (!aligned_fp)
// // // // //     {
// // // // //         perror("Could not open aligned_output.txt for writing");
// // // // //         pclose(pipe);
// // // // //         return 1;
// // // // //     }
    
// // // // //     char buffer[BUFFER_SIZE];
// // // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // // //     {
// // // // //         process_line(buffer, aligned_fp);
// // // // //     }
// // // // //     pclose(pipe);
// // // // //     fclose(aligned_fp);

// // // // //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// // // // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // // // //     if (!aligned_in)
// // // // //     {
// // // // //         perror("Could not open aligned_output.txt for reading");
// // // // //         return 1;
// // // // //     }
    
// // // // //     FILE *final_fp = fopen("output.txt", "w");
// // // // //     if (!final_fp)
// // // // //     {
// // // // //         perror("Could not open output.txt for writing");
// // // // //         fclose(aligned_in);
// // // // //         return 1;
// // // // //     }
    
// // // // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // // // //     {
// // // // //         process_line(buffer, final_fp);
// // // // //     }
    
// // // // //     fclose(aligned_in);
// // // // //     fclose(final_fp);

// // // // //     printf("Test Case 6 executed. Final output is in output.txt\n");
// // // // //     return 0;
// // // // // }


// // // // #include <stdio.h>
// // // // #include <stdlib.h>
// // // // #include <string.h>

// // // // #define BUFFER_SIZE 1024

// // // // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // // // void process_line(const char *input, FILE *out_fp) {
// // // //     const char *prompt = "(ok)>";
// // // //     size_t promptLen = strlen(prompt);
// // // //     const char *start = input;
// // // //     const char *found;
    
// // // //     while ((found = strstr(start, prompt)) != NULL) {
// // // //         // Write text up to the prompt.
// // // //         fwrite(start, 1, found - start, out_fp);
// // // //         // Write the prompt.
// // // //         fwrite(prompt, 1, promptLen, out_fp);
// // // //         // Insert a newline.
// // // //         fputc('\n', out_fp);
// // // //         // Advance the pointer past the prompt.
// // // //         start = found + promptLen;
// // // //     }
// // // //     // Write any remaining text.
// // // //     fputs(start, out_fp);
// // // // }

// // // // int main(void)
// // // // {
// // // //     /* === STEP 1: Write Test Case 7 input === */
// // // //     FILE *fp = fopen("test_input.txt", "w");
// // // //     if (!fp)
// // // //     {
// // // //         perror("Could not create test_input.txt");
// // // //         return 1;
// // // //     }
    
// // // //     // Test Case 7: Cyclic dependency checks.
// // // //     // Create a cyclic dependency: A1 depends on B1 and B1 depends on A1.
// // // //     fprintf(fp, "A1=B1+1\n");
// // // //     fprintf(fp, "B1=A1+1\n");
// // // //     fprintf(fp, "q\n");
// // // //     fclose(fp);

// // // //     /* === STEP 2: Pipe the test input into your sheet program === */
// // // //     char cmd[256];
// // // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");
    
// // // //     FILE *pipe = popen(cmd, "r");
// // // //     if (!pipe)
// // // //     {
// // // //         perror("popen() failed");
// // // //         return 1;
// // // //     }

// // // //     /* === STEP 3: Process output and write to aligned_output.txt === */
// // // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // // //     if (!aligned_fp)
// // // //     {
// // // //         perror("Could not open aligned_output.txt for writing");
// // // //         pclose(pipe);
// // // //         return 1;
// // // //     }
    
// // // //     char buffer[BUFFER_SIZE];
// // // //     while (fgets(buffer, sizeof(buffer), pipe))
// // // //     {
// // // //         process_line(buffer, aligned_fp);
// // // //     }
// // // //     pclose(pipe);
// // // //     fclose(aligned_fp);

// // // //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// // // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // // //     if (!aligned_in)
// // // //     {
// // // //         perror("Could not open aligned_output.txt for reading");
// // // //         return 1;
// // // //     }
    
// // // //     FILE *final_fp = fopen("output.txt", "w");
// // // //     if (!final_fp)
// // // //     {
// // // //         perror("Could not open output.txt for writing");
// // // //         fclose(aligned_in);
// // // //         return 1;
// // // //     }
    
// // // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // // //     {
// // // //         process_line(buffer, final_fp);
// // // //     }
    
// // // //     fclose(aligned_in);
// // // //     fclose(final_fp);

// // // //     printf("Test Case 7 executed. Final output is in output.txt\n");
// // // //     return 0;
// // // // }


// // // #include <stdio.h>
// // // #include <stdlib.h>
// // // #include <string.h>

// // // #define BUFFER_SIZE 1024

// // // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // // void process_line(const char *input, FILE *out_fp) {
// // //     const char *prompt = "(ok)>";
// // //     size_t promptLen = strlen(prompt);
// // //     const char *start = input;
// // //     const char *found;
    
// // //     while ((found = strstr(start, prompt)) != NULL) {
// // //         // Write text up to the prompt.
// // //         fwrite(start, 1, found - start, out_fp);
// // //         // Write the prompt.
// // //         fwrite(prompt, 1, promptLen, out_fp);
// // //         // Insert a newline.
// // //         fputc('\n', out_fp);
// // //         // Advance the pointer past the prompt.
// // //         start = found + promptLen;
// // //     }
// // //     // Write any remaining text.
// // //     fputs(start, out_fp);
// // // }

// // // int main(void)
// // // {
// // //     /* === STEP 1: Write Test Case 8 input === */
// // //     FILE *fp = fopen("test_input.txt", "w");
// // //     if (!fp)
// // //     {
// // //         perror("Could not create test_input.txt");
// // //         return 1;
// // //     }
// // //     // Test Case 8:
// // //     // Division by zero error checking.
// // //     // Set A1 to 10 and then attempt to divide A1 by 0 in cell B1.
// // //     fprintf(fp, "A1=10\n");
// // //     fprintf(fp, "B1=A1/0\n");
// // //     fprintf(fp, "q\n");
// // //     fclose(fp);

// // //     /* === STEP 2: Pipe the test input into your sheet program === */
// // //     char cmd[256];
// // //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");
    
// // //     FILE *pipe = popen(cmd, "r");
// // //     if (!pipe)
// // //     {
// // //         perror("popen() failed");
// // //         return 1;
// // //     }

// // //     /* === STEP 3: Process output and write to aligned_output.txt === */
// // //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// // //     if (!aligned_fp)
// // //     {
// // //         perror("Could not open aligned_output.txt for writing");
// // //         pclose(pipe);
// // //         return 1;
// // //     }
    
// // //     char buffer[BUFFER_SIZE];
// // //     while (fgets(buffer, sizeof(buffer), pipe))
// // //     {
// // //         process_line(buffer, aligned_fp);
// // //     }
// // //     pclose(pipe);
// // //     fclose(aligned_fp);

// // //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// // //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// // //     if (!aligned_in)
// // //     {
// // //         perror("Could not open aligned_output.txt for reading");
// // //         return 1;
// // //     }
    
// // //     FILE *final_fp = fopen("output.txt", "w");
// // //     if (!final_fp)
// // //     {
// // //         perror("Could not open output.txt for writing");
// // //         fclose(aligned_in);
// // //         return 1;
// // //     }
    
// // //     while (fgets(buffer, sizeof(buffer), aligned_in))
// // //     {
// // //         process_line(buffer, final_fp);
// // //     }
    
// // //     fclose(aligned_in);
// // //     fclose(final_fp);

// // //     printf("Test Case 8 executed. Final output is in output.txt\n");
// // //     return 0;
// // // }



// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>

// // #define BUFFER_SIZE 1024
// // #define ROWS 50
// // #define COLS 50

// // // Helper function to convert a column number (1-indexed) into an Excel-style column label.
// // void colLabel(int col, char *label) {
// //     int index = 0;
// //     while (col > 0) {
// //         int rem = (col - 1) % 26;
// //         label[index++] = 'A' + rem;
// //         col = (col - 1) / 26;
// //     }
// //     label[index] = '\0';
// //     // Reverse the label to get the correct order.
// //     for (int i = 0, j = index - 1; i < j; i++, j--) {
// //         char temp = label[i];
// //         label[i] = label[j];
// //         label[j] = temp;
// //     }
// // }

// // // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// // void process_line(const char *input, FILE *out_fp) {
// //     const char *prompt = "(ok)>";
// //     size_t promptLen = strlen(prompt);
// //     const char *start = input;
// //     const char *found;
    
// //     while ((found = strstr(start, prompt)) != NULL) {
// //         // Write text up to the prompt.
// //         fwrite(start, 1, found - start, out_fp);
// //         // Write the prompt.
// //         fwrite(prompt, 1, promptLen, out_fp);
// //         // Insert a newline.
// //         fputc('\n', out_fp);
// //         // Advance the pointer past the prompt.
// //         start = found + promptLen;
// //     }
// //     // Write any remaining text.
// //     fputs(start, out_fp);
// // }

// // int main(void)
// // {
// //     /* === STEP 1: Write Test Case 9 input === */
// //     FILE *fp = fopen("test_input.txt", "w");
// //     if (!fp) {
// //         perror("Could not create test_input.txt");
// //         return 1;
// //     }

// //     // Start with setting A1 to 1.
// //     fprintf(fp, "A1=1\n");

// //     // For each cell in a 50x50 grid (excluding A1),
// //     // assign a formula that depends on A1.
// //     // Each cell is given a formula: <Cell>=A1 + (row * col)
// //     // (This is just an example to generate a large number of dependent cells.)
// //     char colLabelStr[10];
// //     char cellName[20];

// //     for (int r = 1; r <= ROWS; r++) {
// //         for (int c = 1; c <= COLS; c++) {
// //             // Skip cell A1 (row=1, col=1) because it is already assigned.
// //             if (r == 1 && c == 1)
// //                 continue;
// //             colLabel(c, colLabelStr);
// //             snprintf(cellName, sizeof(cellName), "%s%d", colLabelStr, r);
// //             fprintf(fp, "%s=A1+%d\n", cellName, r * c);
// //         }
// //     }

// //     // Update A1 to trigger automatic updates across all dependent cells.
// //     fprintf(fp, "A1=100\n");
// //     // Exit command.
// //     fprintf(fp, "q\n");
// //     fclose(fp);

// //     /* === STEP 2: Pipe the test input into your sheet program === */
// //     // Note: Adjust the dimensions "50 50" if needed.
// //     char cmd[256];
// //     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 50 50");

// //     FILE *pipe = popen(cmd, "r");
// //     if (!pipe) {
// //         perror("popen() failed");
// //         return 1;
// //     }

// //     /* === STEP 3: Process output and write to aligned_output.txt === */
// //     FILE *aligned_fp = fopen("aligned_output.txt", "w");
// //     if (!aligned_fp) {
// //         perror("Could not open aligned_output.txt for writing");
// //         pclose(pipe);
// //         return 1;
// //     }
    
// //     char buffer[BUFFER_SIZE];
// //     while (fgets(buffer, sizeof(buffer), pipe)) {
// //         process_line(buffer, aligned_fp);
// //     }
// //     pclose(pipe);
// //     fclose(aligned_fp);

// //     /* === STEP 4: Read from aligned_output.txt and write final output to output.txt === */
// //     FILE *aligned_in = fopen("aligned_output.txt", "r");
// //     if (!aligned_in) {
// //         perror("Could not open aligned_output.txt for reading");
// //         return 1;
// //     }
    
// //     FILE *final_fp = fopen("output.txt", "w");
// //     if (!final_fp) {
// //         perror("Could not open output.txt for writing");
// //         fclose(aligned_in);
// //         return 1;
// //     }
    
// //     while (fgets(buffer, sizeof(buffer), aligned_in)) {
// //         process_line(buffer, final_fp);
// //     }
    
// //     fclose(aligned_in);
// //     fclose(final_fp);

// //     printf("Test Case 9 executed. Final output is in output.txt\n");
// //     return 0;
// // }


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define ROWS 999
// #define COLS 18278
// #define BUFFER_SIZE 100000

// // Convert a 1-indexed column number to an Excel-style column label.
// void colLabel(int col, char *label) {
//     int index = 0;
//     while (col > 0) {
//         int rem = (col - 1) % 26;
//         label[index++] = 'A' + rem;
//         col = (col - 1) / 26;
//     }
//     label[index] = '\0';
//     // Reverse the string to get the correct order.
//     for (int i = 0, j = index - 1; i < j; i++, j--) {
//         char temp = label[i];
//         label[i] = label[j];
//         label[j] = temp;
//     }
// }

// // Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
// void process_line(const char *input, FILE *out_fp) {
//     const char *pattern = "(ok)>";
//     size_t patternLen = strlen(pattern);
//     const char *start = input;
//     const char *match;
    
//     while ((match = strstr(start, pattern)) != NULL) {
//         // Write text up to the match.
//         fwrite(start, 1, match - start, out_fp);
//         // Write the matched pattern.
//         fwrite(pattern, 1, patternLen, out_fp);
//         // Insert a newline.
//         fputc('\n', out_fp);
//         // Continue after the matched pattern.
//         start = match + patternLen;
//     }
//     // Write any remaining text.
//     fputs(start, out_fp);
// }

// int main(void) {
//     FILE *fp = fopen("test_input.txt", "w");
//     if (!fp) {
//         perror("Could not create test_input.txt");
//         return 1;
//     }
    
//     // Write the initial value for A1.
//     fprintf(fp, "A1=1\n");
    
//     // Stress test: For every cell in a 999x18278 spreadsheet (except A1),
//     // assign a formula that references A1.
//     // For demonstration, each cell is set to: <Cell> = A1 + 0
//     // (You can adjust the formula as needed.)
//     char colLabelStr[16];
//     char cellName[32];
    
//     for (int row = 1; row <= ROWS; row++) {
//         for (int col = 1; col <= COLS; col++) {
//             // Skip cell A1 (row 1, col 1) because it's already defined.
//             if (row == 1 && col == 1)
//                 continue;
//             colLabel(col, colLabelStr);
//             snprintf(cellName, sizeof(cellName), "%s%d", colLabelStr, row);
//             fprintf(fp, "%s=A1+0\n", cellName);
//         }
//     }
    
//     // Update A1 to trigger recalculation in all dependent cells.
//     fprintf(fp, "A1=100\n");
    
//     // Quit command.
//     fprintf(fp, "q\n");
//     fclose(fp);
    
//     // Pipe the test input into the sheet executable.
//     // Adjust the dimensions as per the test case (here, 999 18278).
//     char cmd[256];
//     snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 999 18278");
    
//     FILE *pipe = popen(cmd, "r");
//     if (!pipe) {
//         perror("popen() failed");
//         return 1;
//     }
    
//     // Process the output into an intermediate file (aligned_output.txt).
//     FILE *aligned_fp = fopen("aligned_output.txt", "w");
//     if (!aligned_fp) {
//         perror("Could not open aligned_output.txt for writing");
//         pclose(pipe);
//         return 1;
//     }
    
//     char buffer[BUFFER_SIZE];
//     while (fgets(buffer, sizeof(buffer), pipe)) {
//         process_line(buffer, aligned_fp);
//     }
//     pclose(pipe);
//     fclose(aligned_fp);
    
//     // Read from aligned_output.txt and write the final output to output.txt.
//     FILE *aligned_in = fopen("aligned_output.txt", "r");
//     if (!aligned_in) {
//         perror("Could not open aligned_output.txt for reading");
//         return 1;
//     }
//     FILE *final_fp = fopen("output.txt", "w");
//     if (!final_fp) {
//         perror("Could not open output.txt for writing");
//         fclose(aligned_in);
//         return 1;
//     }
    
//     while (fgets(buffer, sizeof(buffer), aligned_in)) {
//         process_line(buffer, final_fp);
//     }
    
//     fclose(aligned_in);
//     fclose(final_fp);
    
//     printf("Test Case 10 executed. Final output is in output.txt\n");
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100000

// Helper function: Inserts a newline immediately after every occurrence of "(ok)>" in the input line.
void process_line(const char *input, FILE *out_fp) {
    const char *pattern = "(ok)>";
    size_t patternLen = strlen(pattern);
    const char *start = input;
    const char *match;
    
    while ((match = strstr(start, pattern)) != NULL) {
        // Write everything up to the match.
        fwrite(start, 1, match - start, out_fp);
        // Write the matched pattern.
        fwrite(pattern, 1, patternLen, out_fp);
        // Insert a newline.
        fputc('\n', out_fp);
        // Continue after the matched pattern.
        start = match + patternLen;
    }
    // Write any remaining text.
    fputs(start, out_fp);
}

int main(void) {
    // === STEP 1: Write Test Case 11 input ===
    FILE *fp = fopen("test_input.txt", "w");
    if (!fp) {
        perror("Could not create test_input.txt");
        return 1;
    }
    
    // Create 100 cells B1 through B100.
    for (int i = 1; i <= 100; i++) {
        fprintf(fp, "B%d=%d\n", i, i);  // e.g., B1=1, B2=2, ..., B100=100
    }
    
    // Create a formula in C1 that sums all cells B1 to B100.
    // The formula will be: C1=SUM(B1,B2,...,B100)
    fprintf(fp, "C1=SUM(");
    for (int i = 1; i <= 100; i++) {
        fprintf(fp, "B%d", i);
        if (i != 100) {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, ")\n");
    
    // Optionally, update one of the dependencies to trigger recalculation.
    // For example, update B1.
    fprintf(fp, "B1=200\n");
    
    // Exit command.
    fprintf(fp, "q\n");
    fclose(fp);
    
    // === STEP 2: Pipe the test input into your sheet program ===
    // Adjust the command if your executable or dimensions differ.
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "cat test_input.txt | ./sheet 10 10");
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        perror("popen() failed");
        return 1;
    }
    
    // === STEP 3: Process output and write to aligned_output.txt ===
    FILE *aligned_fp = fopen("aligned_output.txt", "w");
    if (!aligned_fp) {
        perror("Could not open aligned_output.txt for writing");
        pclose(pipe);
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        process_line(buffer, aligned_fp);
    }
    pclose(pipe);
    fclose(aligned_fp);
    
    // === STEP 4: Read from aligned_output.txt and write final output to output.txt ===
    FILE *aligned_in = fopen("aligned_output.txt", "r");
    if (!aligned_in) {
        perror("Could not open aligned_output.txt for reading");
        return 1;
    }
    FILE *final_fp = fopen("output.txt", "w");
    if (!final_fp) {
        perror("Could not open output.txt for writing");
        fclose(aligned_in);
        return 1;
    }
    while (fgets(buffer, sizeof(buffer), aligned_in)) {
        process_line(buffer, final_fp);
    }
    fclose(aligned_in);
    fclose(final_fp);
    
    printf("Test Case 11 executed. Final output is in output.txt\n");
    return 0;
}
