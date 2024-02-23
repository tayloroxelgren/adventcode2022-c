#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STR_SIZE 10


void resizeFileData(char* fileDataPtr,int32_t usedSize){
    // resizing the memory
    char* tempPtr = (char*)realloc(fileDataPtr, usedSize);
    if (tempPtr == NULL) {
        printf("Failed to reallocate memory.\n");
    } else {
        fileDataPtr = tempPtr; // Use the reallocated memory
    }

}

void resizeElfData(int32_t* ElfDataPtr,int32_t usedSize){
    // resizing the memory
    int32_t* tempPtr = (int32_t*)realloc(ElfDataPtr, usedSize * sizeof(int32_t));
    if (tempPtr == NULL) {
        printf("Failed to reallocate memory.\n");
    } else {
        ElfDataPtr = tempPtr; // Use the reallocated memory
    }

}

// sorts elfData using insertion sort
void sortElfData(int32_t* ElfDataPtr, int32_t sizeElfData) {
    for (int i = 1; i < sizeElfData; i++) { 
        int j = i;
        while (j > 0 && ElfDataPtr[j] > ElfDataPtr[j - 1]) { 
            ElfDataPtr[j-1] = ElfDataPtr[j-1]+ElfDataPtr[j];
            ElfDataPtr[j] = ElfDataPtr[j-1]-ElfDataPtr[j];
            ElfDataPtr[j-1] = ElfDataPtr[j-1]-ElfDataPtr[j];
            j--;
        }
    }
}

// returns the count to know how big elfdata actually needs to be
int gatherElfData(int32_t dataSize,char* fileDataPtr,int32_t* elfData){
    long int result=0;
    long int newresult=0;
    char** endPtr;
    int32_t counter=0;

    for(int i=0;i<dataSize;i++){
        // to hold temporary string
        char tempStr[STR_SIZE + 1];
        strncpy_s(tempStr, sizeof(tempStr), &fileDataPtr[i * STR_SIZE], STR_SIZE);

        if(strcmp(tempStr,"\n")==0){
            elfData[counter]=result;
            result=0;
            counter++;
        }
        else{
            newresult=strtol(tempStr,endPtr,10);
            if(newresult==0){
                break;
            }
            result+=newresult;
        }
    }
    return counter;
}

int main(){

    FILE* file;
    char fileName[]="day1-puzzelin.txt";

    fopen_s(&file,fileName,"r");

    // Error handling
    if (file == NULL) {
        printf("File not found or unable to open the file.\n");
        return 1;
    }

    // allocating memory for fileData
    char* fileData=(char*)malloc(10000*STR_SIZE*sizeof(char));

    // reading the file data
    int32_t counter = 0;
    char* result;
    do{
        result=fgets(&fileData[counter*STR_SIZE],STR_SIZE,file);
        if (result!=NULL){
            counter++;
        }
    }while(result!=NULL);
    const int32_t FILEDATA_SIZE=counter;

    fclose(file);
    
    // resizing memory
    int32_t actualSize = FILEDATA_SIZE * STR_SIZE * sizeof(char);
    resizeFileData(fileData,actualSize);

    // getting elf Data
    int32_t* elfData = malloc(actualSize * sizeof(int32_t));
    int32_t actualElfSize=gatherElfData(actualSize,fileData,elfData);
    resizeElfData(elfData,actualElfSize);
    sortElfData(elfData,actualElfSize);


    // printing answers
    printf("Answer for part 1: %d\n",elfData[0]);

    printf("Answer for part 2: %d",elfData[0]+elfData[1]+elfData[2]);

    // freeing memory
    free(elfData);
    free(fileData);
    return 0;
}

