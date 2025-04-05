#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *in_fp = fopen("output.txt", "r");
    if (!in_fp) {
        perror("Could not open output.txt for reading");
        return 1;
    }

    FILE *out_fp = fopen("output11.txt", "w");
    if (!out_fp) {
        perror("Could not open output11.txt for writing");
        fclose(in_fp);
        return 1;
    }

    int c;
    // Process the file character by character.
    while ((c = fgetc(in_fp)) != EOF) {
        fputc(c, out_fp);
        // Every time a '>' is encountered, insert a newline.
        if (c == '>') {
            fputc('\n', out_fp);
        }
    }

    fclose(in_fp);
    fclose(out_fp);

    printf("Processed output written to output11.txt\n");
    return 0;
}
