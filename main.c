#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



void stringLower(int collumSize, char **string){
    for (int i = 0; i < collumSize; i++){
        for (int j = 0; j <= strlen(string[i]); j++){
            string[i][j] = tolower(string[i][j]);
        }
    }
}

void removeLF(int collumSize, char **string){
    for (int i = 0; i < collumSize; i++){
        for (int j = 0; j <= strlen(string[i]); j++){
            if (string[i][j] == '\n'){
                string[i][j] = '\0';
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
    char sizeBuffer[50];
    int dictSize;
    fgets(sizeBuffer, 49, fp);
    dictSize = atoi(sizeBuffer);
    char **dictionary = (char **) malloc (dictSize * sizeof(char));
    for (int i = 0; i < dictSize; i++){
        dictionary[i] = (char *) malloc (30 * sizeof(char));
        fgets(dictionary[i], 29, fp);
    }
    int probWordSize;
    fgets(sizeBuffer, 49, fp);
    probWordSize = atoi(sizeBuffer);
    char **probWords = (char **) malloc (probWordSize * sizeof(char));
    for (int i = 0; i < probWordSize; i++){
        probWords[i] = (char *) malloc (100 * sizeof(char));
        fgets(probWords[i], 99, fp);
    }
    int smsSize;
    fgets(sizeBuffer, 49, fp);
    smsSize = atoi(sizeBuffer);
    char **sms = (char **) malloc (smsSize * sizeof(char));
    for (int i = 0; i < smsSize; i++){
        sms[i] = (char *) malloc (80 * sizeof(char));
    }
    /*
    char tempBuffer [100];
    fgets(tempBuffer, 99, fp);
    printf("%s", tempBuffer);
    fgets(tempBuffer, 99, fp);
    printf("%s", tempBuffer);
    */
    //char sms[50][50];
    printf("Now reading in SMS and Timestamps\n");
    int timeHr = 0;
    int timeMin = 0;
    int smsInc = 0;
    int timeInc = 0;
    int *time = (int *) malloc (smsSize * sizeof(int));
    char timeFormat[8];
    printf("%d\n", smsSize);
    int tmpFuck = 0;
    // splitting sms and time into 2 arrays;
    for (int i = 0; i < (smsSize * 2); i++){
        if (i % 2 == 1){
            printf("Reading in SMS %d\n", smsInc + 1);
            fgets(sms[smsInc], 50, fp);
            //fscanf(fp, "%d %79s\n", &tmpFuck, sms[smsInc]);
            printf("SMS : %s\n", sms[smsInc]);
            smsInc++;
        }   // Convert 12 HR time string to 24 HR int to array
            else {
            fscanf(fp, "%d:%d %7s\n", &timeHr, &timeMin, timeFormat);
            printf("Time 1: %d %d %s\n", timeHr , timeMin, timeFormat);
            timeHr = timeHr * 100;
            printf("%s\n", timeFormat);
            if (strcmp(timeFormat,"PM\0") == 0 && timeHr != 1200){
                timeHr += 1200;
            }
            time[timeInc] = timeHr + timeMin;
            printf("Time %d\n", time[timeInc]);
            timeInc++;
        }
        printf("%d\n", i);
    }
    
    /*
    char **smsLower = (char **) malloc (smsSize * sizeof(char));
    for (int i = 0; i < smsSize; i++){
        smsLower[i] = (char *) malloc ((strlen(sms[i]) + 1) * sizeof(char));
        strcpy(smsLower[i], sms[i]);
    }
    */
    char smsLower[50][50];
    printf("Making comparisons case insenstive and removing LF\n");
    stringLower(dictSize, dictionary);
    stringLower(smsSize, smsLower);
    stringLower(probWordSize, probWords);
    removeLF(dictSize, dictionary);
    removeLF(dictSize, probWords);
    removeLF(smsSize, smsLower);
    int numMispelled = 0;
    int consecTest = 1;
    int misplelledTotal = 0;
    int probTotal = 0;
    int *probSms = (int *) malloc (smsSize * sizeof(int));
    char *indivSMS;
    int mispelledResult = 0;
    int probResult = 0;
    int indivSMSPosition = 0;
    int consecTestPos = 0;
    // SMS Loop

    // 32 = space 
    printf("Start Compare\n");
    for (int i = 0; i < smsSize; i++){
        if (time[i] > 699 && time[i] <= 100){
            printf("Message %d: %s", i + 1, sms[i]);
        } else{
            misplelledTotal = 0;
            // Assign token from smsLower[i]
            indivSMS = strtok(smsLower[i], " ");
            // Loop until all words in string are compared
            while (indivSMS != NULL){
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
                        probTotal += probResult;
                    }
                }
                if (misplelledTotal != dictSize){
                    numMispelled++;
                }
                if (numMispelled >= 3 || probTotal != probWordSize){
                    printf("Message #%d: FAILED TO SEND.\n", i + 1);
                    numMispelled = 0;
                    probTotal = 0;
                    break;
                }
                // Determine if I love you test needs to be done
                consecTest = strcmp(indivSMS, "i");
                if (consecTest == 0){
                    consecTestPos = indivSMSPosition;
                }
                // Get next word in string to compare
                indivSMS = strtok(smsLower[i], NULL);
                indivSMSPosition++;
            }
            if (consecTest == 0){
                // Reset indivSMS
                indivSMS = strtok(smsLower[i], " ");
                for (int l = 0; l < consecTestPos; l++){
                    // Set indivSMS to I to start I love you test
                    indivSMS = strtok(smsLower[i], NULL);
                }
                if (strcmp(indivSMS, "i") == 0){
                    indivSMS = strtok(smsLower[i], NULL);
                    if (strcmp(indivSMS, "love") == 0){
                        indivSMS = strtok(smsLower[i], NULL);
                        if (strcmp(indivSMS, "you") == 0){
                            printf("Message #%d: FAILED TO SEND.\n", i +1);
                        }
                    } 
                }
            }
        }
    }
    /*
    for (int i = 0; i < smsSize; i++){
        free(sms[i]);
        free(smsLower[i]);
    }
    free(sms);
    free(smsLower);
    */
    for (int i = 0; i < dictSize; i++){
        free(dictionary[i]);
    }
    free(probWords);
    for (int i = 0; i < probWordSize; i++){
        free(probWords[i]);
    }
    free(dictionary);
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