int split_strcmp(int start1, int end1, char *input1, int start2, int end2, char *input2) {
    int i, j;
    int len1 = strlen(input1);
    int len2 = strlen(input2);
    char buffer1[len1 + 1], buffer2[len2 + 1];

    j = 0;
    for (i = start1; i <= end1; i++) {
        buffer1[j] = input1[i];
        j++;
    }
    buffer1[j] = '\0';

    j = 0;
    for (i = start2; i <= end2; i++) {
        buffer2[j] = input2[i];
        j++;
    }
    buffer2[j] = '\0';

    if (strcmp(buffer1, buffer2) == 0) return 1;
    else return 0;
}

int split_str(int start, int end, char *input, char *output) {

    int len = strlen(input);
    int i, j;
    char buffer[len + 1];

    j = 0;
    for(i=start; i<=end; i++) {
        buffer[j] = input[i];
        j++;
    }
    buffer[j] = '\0';
    strcpy(output, buffer);

    return 0;

}
