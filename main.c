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

int main(int argc, char *argv[]){
    FILE *fp = fopen("textmsg.txt", "r");
    if (fp == NULL){
        printf("Unable to find file\n");
        return 0;
    }
    char *sizeBuffer = (char *) malloc(50 * sizeof(char));
    char *readBuffer = (char *) malloc(150 * sizeof(char));
    int dictSize;
    fgets(sizeBuffer, 49, fp);
    dictSize = atoi(sizeBuffer);
    char **dictionary = (char **) malloc (dictSize * sizeof(char*));
    for (int i = 0; i < dictSize; i++){
        fgets(readBuffer, 149, fp);
      //  printf("%s", readBuffer);
        dictionary[i] = (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
        strcpy(dictionary[i], readBuffer);
        //printf("%s", dictionary[i]);
    }
    int probWordSize;
    fgets(sizeBuffer, 49, fp);
    probWordSize = atoi(sizeBuffer);
    char **probWords = (char **) malloc (probWordSize * sizeof(char*));
    for (int i = 0; i < probWordSize; i++){
        fgets(readBuffer, 149, fp);
        probWords[i] = (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
        strcpy(probWords[i], readBuffer);
    }
    int smsSize;
    fgets(sizeBuffer, 49, fp);
    smsSize = atoi(sizeBuffer);
    char **sms = (char **) malloc (smsSize * sizeof(char*));
    for (int i = 0; i < smsSize; i++){
        sms[i] = (char *) malloc (50 * sizeof(char));
    }
    //char tempBuffer [100];
    //fgets(tempBuffer, 99, fp);
    //printf("%s", tempBuffer);
    //char sms[50][50];
    //printf("%s",dictionary[0]);
   // printf("Now reading in SMS and Timestamps\n");
    int timeHr = 0;
    int timeMin = 0;
    int smsInc = 0;
    int timeInc = 0;
    int *time = (int *) malloc ((smsSize) * sizeof(int));
    char timeFormat[8];
    //printf("%d messages to read in\n", smsSize);
    int tmpFuck = 0;
    // splitting sms and time into 2 arrays;
    for (int i = 0; i < (smsSize * 2); i++){
        if (i % 2 == 1){
            //printf("Reading in SMS %d\n", smsInc + 1);
            //fgets(sms[smsInc], 50, fp);
            //fgets(readBuffer, 149, fp);
            //sms[smsInc] =  (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
            //strcpy(sms[smsInc], readBuffer);
            fscanf(fp, "%d", &tmpFuck);
            for(int k = 0; k < tmpFuck; k++){
                fscanf(fp, "%25s", readBuffer);
                strcat(readBuffer, " ");
                strcat(sms[smsInc], readBuffer);
            }
            for (int u = 0; u < 50; u++){
                //printf("%c %d", sms[smsInc][u], sms[smsInc][u]);
                if (sms[smsInc][u] == '\0'){
                    sms[smsInc][u] = '\n';
                    sms[smsInc][u + 1] = '\0';
                    break;
                  //  printf("Added NB\n");
                }
            }
            //printf("SMS : %s\n", sms[smsInc]);
           // printf("SMS Inc: %d\n", smsInc);
            smsInc++;
        }   // Convert 12 HR time string to 24 HR int to array
            else {
            fscanf(fp, "%d:%d %5s\n", &timeHr, &timeMin, timeFormat);
            //printf("Time 1: %d %d %s\n", timeHr , timeMin, timeFormat);
            timeHr = timeHr * 100;
            //printf("%s\n", timeFormat);
            if (strcmp(timeFormat,"PM\0") == 0 && timeHr != 1200){
                timeHr += 1200;
            }
            time[timeInc] = timeHr + timeMin;
            //printf("Time %d\n", time[timeInc]);
            timeInc++;
        }
        //printf("%d\n", i);
    }
    //printf("First SMS %s\n", sms[0]);
    //printf("Second SMS %s\n", sms[1]);
    char **smsLower = (char **) malloc (smsSize * sizeof(char*));
    for (int i = 0; i < smsSize; i++){
        smsLower[i] = (char *) malloc ((strlen(sms[i]) + 1) * sizeof(char));
        strcpy(smsLower[i], sms[i]);
        //printf("%s\n", smsLower[i]);
    }
    //char smsLower[50][50];
    //printf("Making comparisons case insenstive and removing LF\n");
   // printf("%s\n", dictionary[0]);
    stringLower(dictSize, dictionary);
    stringLower(smsSize, smsLower);
    stringLower(probWordSize, probWords);
    //printf("Now removing LF\n");
    removeLF(dictSize, dictionary);
    removeLF(probWordSize, probWords);
    removeLF(smsSize, smsLower);
    //printf("Testing\n");
    int numMispelled = 0;
    int consecTest = 1;
    int misplelledTotal = 0;
    int probTotal = 0;
    //int *probSms = (int *) malloc (smsSize * sizeof(int));
    char *indivSMS;
    int mispelledResult = 0;
    int probResult = 0;
    int indivSMSPosition = 0;
    int consecTestPos = 0;
    bool isConsec = false;
    bool isProb = false;
    int iPos = 0;
    int lovePos = 0;
    int youPos = 0;
    // SMS Loop

    // 32 = space 
    printf("Start Compare\n");
    for (int i = 0; i < smsSize; i++){
        printf("Time: %d\n", time[i]);
        if (time[i] > 699 || time[i] <= 100){
            printf("Message %d: %s", i + 1, sms[i]);
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
                    //printf("Message #%d: FAILED TO SEND.\n", i + 1);
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
                //printf("Token | %s\n", indivSMS);
                indivSMS = strtok(NULL, " ");
                indivSMSPosition++;
            }
            if(isProb == true || ((iPos + 1 == lovePos) && (lovePos + 1 == youPos))){
                printf("Message #%d: FAILED TO SEND.\n", i + 1);
                isProb = false;
                iPos = 0;
                lovePos = 0;
                youPos = 0; 
            } else{
                printf("Message %d: %s", i + 1, sms[i]);
            }
                /* if (consecTest == 0){
                // Reset indivSMS
                indivSMS = strtok(smsLower[i], " ");
                for (int l = 0; l < consecTestPos; l++){
                    // Set indivSMS to I to start I love you test
                    indivSMS = strtok(NULL, " ");
                    printf("Current Token | %s", indivSMS);
                }
                if (strcmp(indivSMS, "i") == 0){
                    printf("ILV Test | %s\n", indivSMS);
                    indivSMS = strtok(NULL, " ");
                    printf("Second Test %s\n", indivSMS);
                    if (strcmp(indivSMS, "love") == 0){
                        indivSMS = strtok(NULL, " ");
                        if (strcmp(indivSMS, "you") == 0){
                            //printf("Message #%d: FAILED TO SEND.\n", i + 1);
                            isConsec = true;
                        } else{
                            consecTest = 1;
                        }
                    } else{
                        consecTest = 1;
                    }
                } else{
                    consecTest = 1;
                }
             } */
             if (isConsec == true || isProb == true){
                printf("Message #%d: FAILED TO SEND.\n", i + 1);
                isConsec = false;
                isProb = false;
            }
        }
    }
    for (int i = 0; i < smsSize; i++){
        free(sms[i]);
        free(smsLower[i]);
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
    free(readBuffer);
    free(sizeBuffer);
    // Determine time ie convert 3:30 PM to 1530
    /*int time[smsSize / 2];
    char tempTime[10];
    int timeInc = 0;
    for (int i = 0; i < smsSize; i++){
        if (i % 2 == 0){
            for (int j = 0; j < 10; j++){
                if(sms[i][j] != ':' && (j < 2)){
                    tempTime[j] = sms[i][j];
                    timeInc++;
                    if (j == timeInc){
                    time[i / 2] = atoi(tempTime) * 100;
                    strcpy(tempTime,"\0\0\0\0\0\0");
                    }
                } else if(sms[i][j] != 32 && (j > 2) && j < 5){
                    tempTime[j] = sms[i][j];
                    timeInc++;
                    if (j == timeInc){
                        time[i / 2] = atoi(tempTime) + time[i / 2];
                        strcpy(tempTime,"\0\0\0\0\0\0");
                    }
                } else {
                    if (sms[i][j] == '\n'){
                        tempTime[j] = '\0';
                    } else{
                        tempTime[j] = sms[i][j];
                    }
                }
            }
            if (strcmp(tempTime, "PM") == 0){
                time[i / 2] = time[i / 2] + 1200;
            }
        }  
    }

    for (int i = 0; i < smsSize; i++){
        if (i % 2 == 1){
            for (int j = 0; j <= strlen(sms[i]); j++){
                
            }
        }
    }
    */

    return 0;
}