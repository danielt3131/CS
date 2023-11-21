#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void stringLower(int collumSize, char **string){
    for (int i = 0; i < collumSize; i++){
        //printf("Current String %s\n", string[i]);
        for (int j = 0; j <= strlen(string[i]); j++){
            string[i][j] = tolower(string[i][j]);
           // printf("%c\n", string[i][j]);
        }
    }
}

void removeLF(int collumSize, char **string){
    for (int i = 0; i < collumSize; i++){
        for (int j = 0; j <= strlen(string[i]); j++){
            //printf("%c", string[i][j]);
            if (string[i][j] == '\n'){
                string[i][j] = '\0';
                //printf("Removed LF\n");
            }
        }
    }
}

// 1 = char 0 = int
void memoryAllocateCheck(void *array, int datatype){
    if(datatype == 1){
        if (array == NULL){
            fprintf(stderr, "Unable to allocate memory now terminating\n");
            exit(0);
        }
    } else if(datatype == 0){
         if (array == NULL){
            fprintf(stderr, "Unable to allocate memory now terminating\n");
            exit(0);
        }
    }
}

const int startHeapSizeSMS = 29;

int main(int argc, char *argv[]){
    FILE *fp = fopen("textmsg.txt", "r");
    if (fp == NULL){
        printf("Unable to find file\n");
        return 0;
    }
    char *readBuffer = (char *) malloc(30 * sizeof(char));
    memoryAllocateCheck(readBuffer, 1);
    short dictSize;
    fgets(readBuffer, 29, fp);
    dictSize = atoi(readBuffer);
    char **dictionary = (char **) malloc (dictSize * sizeof(char*));
    memoryAllocateCheck(dictionary,1);
    short f = 0;
    for (f = 0; f < dictSize; f++){
        fgets(readBuffer, 29, fp);
        dictionary[f] = (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
        memoryAllocateCheck(dictionary[f], 1);
        strcpy(dictionary[f], readBuffer);
    }
    short probWordSize;
    fgets(readBuffer, 29, fp);
    probWordSize = atoi(readBuffer);
    char **probWords = (char **) malloc (probWordSize * sizeof(char*));
    memoryAllocateCheck(probWords, 1);
    for (f = 0; f < probWordSize; f++){
        fgets(readBuffer, 29, fp);
        probWords[f] = (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
        memoryAllocateCheck(probWords[f], 1);
        strcpy(probWords[f], readBuffer);
    }
    int smsSize;
    fgets(readBuffer, 29, fp);
    smsSize = atoi(readBuffer);
    char **sms = (char **) malloc (smsSize * sizeof(char*));
    memoryAllocateCheck(sms,1);
    for (int i = 0; i < smsSize; i++){
        sms[i] = (char *) malloc (startHeapSizeSMS * sizeof(char));
        memoryAllocateCheck(sms[i], 1);
    }
    short timeHr = 0;
    short timeMin = 0;
    int smsInc = 0;
    int timeInc = 0;
    short *time = (short *) malloc ((smsSize) * sizeof(short));
    memoryAllocateCheck(time, 0);
    char *timeFormat = (char *) malloc(4 * sizeof(char));
    memoryAllocateCheck(timeFormat, 1);
    int *smsHeapSize = (int *) malloc(smsSize * sizeof(int));
    memoryAllocateCheck(smsHeapSize, 0);
    for (int i = 0; i < smsSize; i++){
        smsHeapSize[i] = startHeapSizeSMS;
    }
    int smsReadLength = 0;
    // Integer value not needed after reading in the SMSes
    short tmpFuck = 0;
    // splitting sms and time into 2 arrays;
    for (int i = 0; i < (smsSize * 2); i++){
        if (i % 2 == 1){
            fscanf(fp, "%hd", &tmpFuck);
            for(int k = 0; k < tmpFuck; k++){
                fscanf(fp, "%29s", readBuffer);
                strcat(readBuffer, " ");
                smsReadLength = strlen(readBuffer) + smsReadLength;
                if(smsReadLength >= smsHeapSize[smsInc]){
                    // Expand memory block of sms at smsInc to prevent buffer overflow
                    smsHeapSize[smsInc] = smsReadLength;
                    sms[smsInc] = (char *) realloc(sms[smsInc], smsHeapSize[smsInc]);
                    memoryAllocateCheck(sms[smsInc], 1);
                }
                strcat(sms[smsInc], readBuffer);

            }
            smsReadLength = 0;
            smsInc++;
        }   // Convert 12 HR time string to 24 HR int to array
            else {
            fscanf(fp, "%hd:%hd %3s\n", &timeHr, &timeMin, timeFormat);
            timeHr = timeHr * 100;
            if (strcmp(timeFormat,"PM\0") == 0 && timeHr != 1200){
                timeHr += 1200;
            }
            time[timeInc] = timeHr + timeMin;
            timeInc++;
        }
    }
    fclose(fp);
    free(readBuffer);
    free(timeFormat);
    free(smsHeapSize);
    char **smsLower = (char **) malloc (smsSize * sizeof(char*));
    for (int i = 0; i < smsSize; i++){
        smsLower[i] = (char *) malloc ((strlen(sms[i]) + 1) * sizeof(char));
        strcpy(smsLower[i], sms[i]);
    }
    // Making all arrays of strings lowercase for upcoming comparison
    stringLower(dictSize, dictionary);
    stringLower(smsSize, smsLower);
    stringLower(probWordSize, probWords);
    // Removing \n LF in all strings for the upcoming comparison
    removeLF(dictSize, dictionary);
    removeLF(probWordSize, probWords);
    removeLF(smsSize, smsLower);
    short numMispelled = 0;
    short consecTest = 1;
    int misplelledTotal = 0;
    int probTotal = 0;
    char *indivSMS = NULL;
    short mispelledResult = 0;
    int probResult = 0;
    int indivSMSPosition = 0;
    bool isProb = false;
    int iPos = 0;
    int lovePos = 0;
    int youPos = 0;
    // SMS Loop
    // The comparison
    printf("Start Compare\n");
    for (int i = 0; i < smsSize; i++){
       // printf("Time: %d\n", time[i]);
        if (time[i] > 699 || time[i] <= 100){
            printf("Message #%d: %s\n", i + 1, sms[i]);
            free(sms[i]);
            free(smsLower[i]);
        } else{
            // Assign token from smsLower[i]
            indivSMS = strtok(smsLower[i], " ");
            // Loop until all words in string are compared
            while (indivSMS != NULL){
                misplelledTotal = 0;
                probTotal = 0;
                for (int j = 0; j < dictSize; j++){
                    mispelledResult = strcmp(indivSMS, dictionary[j]);
                    if (mispelledResult > 0 || mispelledResult < 0){
                        mispelledResult = 1; 
                        misplelledTotal += mispelledResult;
                    }
                }
                for (int k = 0; k < probWordSize; k++){
                    probResult = strcmp(indivSMS, probWords[k]);
                    if (probResult > 0 || probResult < 0){
                        probResult = 1;
                        probTotal += probTotal;
                    }
                }
                if (misplelledTotal == dictSize){
                    numMispelled++;
                    misplelledTotal = 0;
                }
                if (numMispelled >= 3 || (probTotal >= probWordSize)){
                    isProb = true;
                    numMispelled = 0;
                    probTotal = 0;
                }
                // Determine if I love you test needs to be done
                consecTest = strcmp(indivSMS, "i");
                if (consecTest == 0){
                    iPos = indivSMSPosition;
                    consecTest = 1;
                }
                consecTest = strcmp(indivSMS, "love");
                if(consecTest == 0){
                    lovePos = indivSMSPosition;
                    consecTest = 1;
                }
                consecTest = strcmp(indivSMS, "you");
                if(consecTest == 0){
                    youPos = indivSMSPosition;
                    consecTest = 1;
                }
                // Get next word in string to compare
                indivSMS = strtok(NULL, " ");
                // Increment indivSMSPosition used for I love you test 
                indivSMSPosition++;
            }
            if(isProb == true || ((iPos + 1 == lovePos) && (lovePos + 1 == youPos))){
                printf("Message #%d: FAILED TO SEND.\n", i + 1);
                isProb = false;
                iPos = 0;
                lovePos = 0;
                youPos = 0;
                free(smsLower[i]);
                free(sms[i]);
            } else{
                printf("Message #%d: %s\n", i + 1, sms[i]);
                free(sms[i]);
                free(smsLower[i]);
            }
        }
    }
    // Deallocating memory | giving ownership back to the OS kernel and to prevent memory leaks
    free(sms);
    free(smsLower);
    for (int i = 0; i < dictSize; i++){
        free(dictionary[i]);
    }
    free(dictionary);
    for (int i = 0; i < probWordSize; i++){
        free(probWords[i]);
    }
    free(probWords);
    free(time);
    free(indivSMS);
    return 0;
}