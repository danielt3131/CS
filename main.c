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
    char *readBuffer = (char *) malloc(50 * sizeof(char));
    int dictSize;
    fgets(readBuffer, 49, fp);
    dictSize = atoi(readBuffer);
    char **dictionary = (char **) malloc (dictSize * sizeof(char*));
    for (int i = 0; i < dictSize; i++){
        fgets(readBuffer, 49, fp);
        dictionary[i] = (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
        strcpy(dictionary[i], readBuffer);
    }
    int probWordSize;
    fgets(readBuffer, 49, fp);
    probWordSize = atoi(readBuffer);
    char **probWords = (char **) malloc (probWordSize * sizeof(char*));
    for (int i = 0; i < probWordSize; i++){
        fgets(readBuffer, 49, fp);
        probWords[i] = (char *) malloc ((strlen(readBuffer) + 1) * sizeof(char));
        strcpy(probWords[i], readBuffer);
    }
    int smsSize;
    fgets(readBuffer, 49, fp);
    smsSize = atoi(readBuffer);
    char **sms = (char **) malloc (smsSize * sizeof(char*));
    for (int i = 0; i < smsSize; i++){
        sms[i] = (char *) malloc (50 * sizeof(char));
    }
    int timeHr = 0;
    int timeMin = 0;
    int smsInc = 0;
    int timeInc = 0;
    int *time = (int *) malloc ((smsSize) * sizeof(int));
    char timeFormat[8];
    // Integer value not needed after reading in the SMSes
    int tmpFuck = 0;
    // splitting sms and time into 2 arrays;
    for (int i = 0; i < (smsSize * 2); i++){
        if (i % 2 == 1){
            fscanf(fp, "%d", &tmpFuck);
            for(int k = 0; k < tmpFuck; k++){
                fscanf(fp, "%25s", readBuffer);
                strcat(readBuffer, " ");
                strcat(sms[smsInc], readBuffer);
            }
            for (int u = 0; u < 50; u++){
                // Making sure that '\0' is added
                if (sms[smsInc][u] == '\0'){
                    sms[smsInc][u] = '\n';
                    sms[smsInc][u + 1] = '\0';
                    break;
                }
            }
            smsInc++;
        }   // Convert 12 HR time string to 24 HR int to array
            else {
            fscanf(fp, "%d:%d %5s\n", &timeHr, &timeMin, timeFormat);
            timeHr = timeHr * 100;
            if (strcmp(timeFormat,"PM\0") == 0 && timeHr != 1200){
                timeHr += 1200;
            }
            time[timeInc] = timeHr + timeMin;
            timeInc++;
        }
    }
    char **smsLower = (char **) malloc (smsSize * sizeof(char*));
    for (int i = 0; i < smsSize; i++){
        smsLower[i] = (char *) malloc ((strlen(sms[i]) + 1) * sizeof(char));
        strcpy(smsLower[i], sms[i]);
    }
    // Making all arrays of strings lowercase for upcoming comparision
    stringLower(dictSize, dictionary);
    stringLower(smsSize, smsLower);
    stringLower(probWordSize, probWords);
    // Removing \n LF in all strings for the upcoming comparison
    removeLF(dictSize, dictionary);
    removeLF(probWordSize, probWords);
    removeLF(smsSize, smsLower);
    int numMispelled = 0;
    int consecTest = 1;
    int misplelledTotal = 0;
    int probTotal = 0;
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
    // The comparision
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
            } else{
                printf("Message %d: %s", i + 1, sms[i]);
            }
             if (isConsec == true || isProb == true){
                printf("Message #%d: FAILED TO SEND.\n", i + 1);
                isConsec = false;
                isProb = false;
            }
        }
    }
    // Deallocating memory | giving ownership back to the OS kernel and to prevent memory leaks
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
    return 0;
}