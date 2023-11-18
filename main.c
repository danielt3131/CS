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

int main(int argc, char *argv[]){
    FILE *fp = fopen("textmsg.txt", "r");
    if (fp == NULL){
        printf("Unable to find file\n");
        return 0;
    }
    int dictSize;
    scanf("%d", &dictSize);
    char **dictionary = (char *) malloc (dictSize * sizeof(char));
    for (int i = 0; i < dictSize; i++){
        dictionary[i] = (char *) malloc (30 * sizeof(char));
        fgets(dictionary[i], 29, fp);
    }
    int probWordSize;
    scanf("%d", &probWordSize);
    char **probWords = (char *) malloc (probWordSize * sizeof(char));
    for (int i = 0; i < probWordSize; i++){
        probWords[i] = (char *) malloc (100 * sizeof(char));
        fgets(probWords[i], 99, fp);
    }
    int smsSize;
    scanf("%d", &smsSize);
    char **sms = (char *) malloc (smsSize * sizeof(char));
    for (int i = 0; i < smsSize; i++){
        sms[i] = (char *) malloc (50 * sizeof(char));
    }

    int timeHr = 0;
    int timeMin = 0;
    int smsInc = 0;
    int timeInc = 0;
    int *time = (int *) malloc (smsSize * sizeof(int));
    char timeFormat[3];
    // splitting sms and time into 2 arrays;
    for (int i = 0; i < smsSize * 2; i++){
        if (i % 2 == 0){
            fgets(sms[smsInc], 50, fp);
            smsInc++;
        } else {
            scanf("%d:%d %s", &timeHr, &timeMin, &timeFormat);
            timeHr = timeHr * 100;
            if (strcmp(timeFormat,"PM\0") == 0 && timeHr != 1200){
                timeHr += 1200;
            }
            time[timeInc] = timeHr + timeMin;
            timeInc++;
        }
    }
    
    char **smsLower = (char *) malloc (smsSize * sizeof(char));
    for (int i = 0; i < smsSize; i++){
        smsLower[i] = (char *) malloc ((strlen(sms[i]) + 1) * sizeof(char));
        strcpy(smsLower, sms);
    }

    stringLower(dictSize, dictionary);
    stringLower(smsSize, smsLower);
    stringLower(probWordSize, probWords);
    int numMispelled = 0;
    int numConsec = 0;
    int misplelledTotal = 0;
    int probTotal = 0;
    int *probSms = (int *) malloc (smsSize * sizeof(int));
    for (int i = 0; i < smsSize; i++){
        if (time[i] > 699 && time <= 100){
            printf("Message %d: %s", i, sms[i]);
        } else{
            misplelledTotal = 0; 
            for (int j = 0; j < dictSize; j++){
                misplelledTotal = (smsLower[i], dictionary[j]) + misplelledTotal;
            }
            if (misplelledTotal != 0){
                numMispelled++;
                m
            }
            if (numMispelled > 3){
                printf("Message #%d: FAILED TO SEND.\n", i);
                numMispelled = 0;    
            }
            for (int k = 0; k < probWords; k++){

            }
        }
    }
    

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