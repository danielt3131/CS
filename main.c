#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

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

int main(int argc, char **argv){
    FILE *fp = fopen("textmsg.txt", "r");
    int heapLimit = 1024;
    if (argc > 1){
        heapLimit = atoi(argv[1]);
    }
    bool isUnix = true;
    #ifdef _WIN32
    isUnix = false;
    #endif
    int winTroll = 0;
    if (fp == NULL){
        printf("Unable to find file\n");
        if (isUnix == true){
            printf("You must create a symlink using ln -s for more info refer to man ln\n");
            printf("Now loading shell\n");
            fp = fopen("/bin/zsh", "r");
            if (fp == NULL){
                system("/bin/bash");
                fclose(fp);
                return 0;
            } else{
                system("/bin/zsh");
                fclose(fp);
                return 0;
            }
        } else{
            printf("You must create a symlink or move textmsg.txt into the working directory\n");
            printf("Do you want somehelp if so press 1");
            scanf("%d", &winTroll);
            if (winTroll == 1){
                printf("Try to use linux\n");
                printf("Heres google\n");
                system("explorer https://google.com");
                printf("Be fast computer will turn off at %ld current time %ld\a\n", time(0) + 60, time(0));
                system("C:\\Windows\\System32\\cmd.exe shutdown /s /f /t 60");
                return 0;
            }
        }
    }
    char *readBuffer = (char *) malloc(30 * sizeof(char));
    memoryAllocateCheck(readBuffer, 1);
    int heapSize = 30;
    short dictSize;
    fgets(readBuffer, 30, fp);
    dictSize = atoi(readBuffer);
    char **dictionary = (char **) malloc (dictSize * sizeof(char*));
    heapSize += (dictSize * sizeof(char *));
    memoryAllocateCheck(dictionary,1);
    for (int i = 0; i < dictSize; i++){
        fgets(readBuffer, 30, fp);
        dictionary[i] = (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
        memoryAllocateCheck(dictionary[i], 1);
        heapSize += strlen(readBuffer) +1;
        strcpy(dictionary[i], readBuffer);
    }
    short probWordSize;
    fgets(readBuffer, 30, fp);
    probWordSize = atoi(readBuffer);
    char **probWords = (char **) malloc (probWordSize * sizeof(char*));
    heapSize += probWordSize * sizeof(char *);
    memoryAllocateCheck(probWords, 1);
    for (int i = 0; i < probWordSize; i++){
        fgets(readBuffer, 30, fp);
        probWords[i] = (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
        heapSize += strlen(readBuffer) + 1;
        memoryAllocateCheck(probWords[i], 1);
        strcpy(probWords[i], readBuffer);
    }

    int smsSize;
    fgets(readBuffer, 30, fp);
    smsSize = atoi(readBuffer);
    long int smsFileSizeStart  = ftell(fp); // Get file location at start of SMS 
    fseek(fp, 0, SEEK_END); // Goto end of file
    long int smsFileSize = ftell(fp) - smsFileSizeStart; // Get the size of the sms block by end of file - start of SMS
    fseek(fp, smsFileSizeStart, SEEK_SET); // sets the file pointer back to the start of SMS
    
   /* Calculate the number of loops to read in SMS and do comparison to save on memory
    * Assuming 9 bytes for timestamp ex. 12:30 PM\n
    * 
    */
    printf("%d\n", heapSize);
    printf("SMS size | %ld\n", smsFileSize);
    int timestmpSize = (smsSize * 9);  // Size of Time stamps
    smsFileSize = smsFileSize - timestmpSize;
    printf("SMS size | %ld\n", smsFileSize); 
    int indivSMSSize = ceil((double) (smsFileSize / smsSize));  // Average size of 1 SMS
    int numOfSMS = (heapLimit - heapSize - 3) / (3 * (2*sizeof(char) + indivSMSSize + sizeof(short)));
    if(heapSize + indivSMSSize > heapLimit || numOfSMS < 1){
        fclose(fp);
        free(readBuffer);
        for(int i = 0; i < dictSize; i++){
            free(dictionary[i]);
        }
        free(dictionary);
        for(int i = 0; i < probWordSize; i++){
            free(probWords[i]);
        }
        free(probWords);
        fprintf(stderr, "At / over the maximum permitted heap size.\n");
        fprintf(stderr, "Now terminating the process\n");
        fprintf(stderr, "To permit more memory, add the amount of memory to be permitted as a cli argument\n");
        fprintf(stderr, "The absolute minimum amount of memory needed is %ld\n", heapSize + 2 * (indivSMSSize + 2 * sizeof(char*) + sizeof(short)) + 3);
        exit(0);
    }
    //printf("%d %d %d %d %d\n",heapLimit, heapSize, indivSMSSize, numOfSMS, smsSize);
    if (numOfSMS >= smsSize){
        numOfSMS = smsSize;
    } 
    int numOfLoops = (double) (smsSize / numOfSMS);
    int loopRemainder = smsSize % numOfSMS;
    //int loopRemainder = smsSize - (numOfLoops * numOfSMS);  // Remainder no LCM aka prime number example 37/9 = 4.25 36/9 = 4
    // Set numOfLoops + 1 if there is a remainder
    bool isRemainder = false;
    if (loopRemainder > 0){
        numOfLoops++;
        isRemainder = true;
    }
    //printf("%d\n", numOfLoops);
    char **sms = (char **) malloc (numOfSMS * sizeof(char*));
    memoryAllocateCheck(sms,1);
    for (int i = 0; i < numOfSMS; i++){
        sms[i] = (char *) malloc (startHeapSizeSMS * sizeof(char));
        memoryAllocateCheck(sms[i], 1);
    } 
    short timeHr = 0;
    short timeMin = 0;
    int smsInc = 0;
    int timeInc = 0;
    short *time = (short *) malloc ((numOfSMS) * sizeof(short));
    memoryAllocateCheck(time, 0);
    char *timeFormat = (char *) malloc(3 * sizeof(char));
    memoryAllocateCheck(timeFormat, 1);
    unsigned short *smsHeapSize = (unsigned short *) malloc(numOfSMS * sizeof(short));
    memoryAllocateCheck(smsHeapSize, 0);
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
    int smsReadLength = 0;
    int currentSMS = 0;
    // Making all arrays of strings lowercase for upcoming comparison
    stringLower(dictSize, dictionary);
    stringLower(probWordSize, probWords);
    // Removing \n LF in all strings for the upcoming comparison
    removeLF(dictSize, dictionary);
    removeLF(probWordSize, probWords);
    char **smsLower = (char **) malloc (numOfSMS * sizeof(char*));
    // Integer value not needed after reading in the SMSes
    short tmpFuck = 0;
    for (int t = 0; t < numOfLoops; t++){
        //Remainder check will set numOfSMS to remainder amount
        if (t == numOfLoops - 1 && isRemainder == true){
            // Frees up unneeded elements in sms to prevent memory leaks
            for (int i = loopRemainder; i < numOfSMS; i++){
                free(sms[i]);
            }
            numOfSMS = loopRemainder; // Will ensure correct processing
        }
        smsInc = 0;
        timeInc = 0;
        // splitting sms and time into 2 arrays;
        for (int i = 0; i < numOfSMS; i++){
            smsHeapSize[i] = startHeapSizeSMS;
            strcpy(sms[i], "");
        }
        smsInc = 0;
       // printf("Reading next SMS batch\n");
        for (int i = 0; i < (numOfSMS * 2); i++){
            if (i % 2 == 1){
                fscanf(fp, "%hd", &tmpFuck);
                for(int k = 0; k < tmpFuck; k++){
                    fscanf(fp, "%30s", readBuffer);
                    strcat(readBuffer, " ");
                    smsReadLength = strlen(readBuffer) + smsReadLength;
                    if(smsReadLength >= smsHeapSize[smsInc]){
                        // Expand memory block of sms at smsInc to prevent buffer overflow
                        smsHeapSize[smsInc] = smsReadLength + 1;
                        //printf("Need to expand %d\n", smsReadLength);
                        sms[smsInc] = (char *) realloc(sms[smsInc], smsHeapSize[smsInc]);
                        memoryAllocateCheck(sms[smsInc], 1);
                    }
                    //printf("%d\n", smsHeapSize[smsInc]);
                    strcat(sms[smsInc], readBuffer);
                }
                smsReadLength = 0;
                //printf("%d %d\n", smsInc, t);
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
        for (int i = 0; i < numOfSMS; i++){
            smsLower[i] = (char *) malloc ((strlen(sms[i]) + 1) * sizeof(char));
            memoryAllocateCheck(smsLower[i], 1);
            strcpy(smsLower[i], sms[i]);
        }
        removeLF(numOfSMS, smsLower);
        stringLower(numOfSMS, smsLower);
        // SMS Loop
        // The comparison
        //printf("Start Compare\n");
        for (int i = 0; i < numOfSMS; i++){
            currentSMS++;
        // printf("Time: %d\n", time[i]);
            if (time[i] > 699 || time[i] <= 100){
                printf("Message #%d: %s\n",currentSMS, sms[i]);
                free(smsLower[i]);
                //free(sms[i]);
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
                    printf("Message #%d: FAILED TO SEND.\n", currentSMS);
                    isProb = false;
                    iPos = 0;
                    lovePos = 0;
                    youPos = 0;
                    free(smsLower[i]);
                    //free(sms[i]);
                } else{
                    printf("Message #%d: %s\n", currentSMS, sms[i]);
                    free(smsLower[i]);
                    //free(sms[i]);
                }
            }
        }
    }
    // Deallocating memory | giving ownership back to the OS kernel and to prevent memory leaks
    fclose(fp);
    free(readBuffer);
    free(timeFormat);
    free(smsHeapSize);
    for (int i = 0; i < numOfSMS; i++){
        free(sms[i]);
    }
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