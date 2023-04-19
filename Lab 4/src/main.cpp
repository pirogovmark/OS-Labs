#include "unistd.h"
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include <sys/mman.h>
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "errno.h"
#include "fcntl.h"


int main() {

    int fd = open("data.txt", O_RDONLY);
    struct stat sb;

    if (fstat(fd, &sb) == -1){
        perror("Couldn't get the file size\n");
    }

    int id = fork();

    if (id < 0) {
        perror("An error occurred with fork");
        return -1;
    } else if (id == 0) {   // Сhild
        sleep(15);

        char *out = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0));
        printf("%p Child\n", out);

        int nOfChars = out[0] - '0';
        int nOfDigits = out[nOfChars + 3] - '0';

        char str[nOfChars];

        for (int i = 2; i < 2 + nOfChars; i++) {
            str[i - 2] = out[i];
        }
        float sum = 0;
        int start = 5 + nOfChars;
        for (int j = 0; j < nOfDigits; ++j) {
            int i = 0;
            char a[4];
            while (i < 4) {
                a[i] = out[start];
                ++i;
                ++start;
            }
            sum += atof(a);
            start += 1;
        }

        FILE *fptr2;
        fptr2 = fopen(str, "w");
        fprintf(fptr2,"The answer is: %.2f\n", sum);
        fclose(fptr2);
        close(fd);

    } else {               // Parent
        FILE *fp;
        fp = fopen("data.txt", "w");

        char s[30];
        printf("Enter the filename:\n");
        scanf("%s", s);

        int len = strlen(s);

        int n = rand() % 9 + 1;

        fprintf(fp, "%d\n", len);
        fprintf(fp, "%s\n", s);
        fprintf(fp, "%d\n", n);

        printf("Enter %d different digits\n", n);

        float x;
        for (int i = 0; i < n; ++i) {
            scanf("%f", &x);
            fprintf(fp, "%.2f\n", x);
        }
        fclose(fp);

        char *in = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0));
        printf("%p Parent\n", in);
        close(fd);

        pid_t wpid;
        int status = 0;
        while ((wpid = wait(&status)) > 0);
    }

    return 0;
}
