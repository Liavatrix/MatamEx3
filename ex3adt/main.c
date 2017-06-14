#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtm_ex3.h"
#include "EscapeTechnion.h"

#define BUFFER_SIZE 251
#define MAX_PARAMETERS 12

MtmErrorCode readOutputParameter(char*** argv, char** output,int argc);

MtmErrorCode readInputParameter(char*** argv, char** input,int argc);

MtmErrorCode getLine(FILE* input,System system,FILE* output);

MtmErrorCode matchCommand(char* line,System system,FILE* output);

int main(int argc, char** argv) {
    MtmErrorCode err;
    FILE* output=stdout;
    FILE* input=stdin;
    for(int i=1; i<argc-1; i+=2) {
        if(strcmp(argv[i], "-i") == 0) {
            if(input != stdin) {
                fclose(input);
                mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
                return 0;
            }
            input = fopen(argv[i+1], "r");
            if(!input) {
                mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
                return 0;
            }
        }
        else if(strcmp(argv[i], "-o") == 0) {
            if(output != stdout) {
                fclose(output);
                mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
                return 0;
            }
            output = fopen(argv[i+1], "w");
            if(!output) {
                mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
                return false;
            }
        }
        else {
            fclose(output);
            fclose(input);
            mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
            return 0;
        }
    }
    System system=InitiateSystem();
    err=getLine(input,system,output);
    if(err!=MTM_SUCCESS) {
        mtmPrintErrorMessage(stderr, err);
        if(err==MTM_OUT_OF_MEMORY||err==MTM_INVALID_COMMAND_LINE_PARAMETERS) {
            matchCommand("SystemDestroy",system,output);
            if (input != stdin)
                fclose(input);
            if (output != stdout)
                fclose(output);
            exit(0);
        }
    }
    if(output!=stdout)
        fclose(output);
    if(input!=stdin)
        fclose(input);
    matchCommand("SystemDestroy",system,output);
    return MTM_SUCCESS;
}

/**
 * count the numbers of command line parameters
 * if the number is not in {0,2,4} return invalid error
 * @param argv - address to the command line parameter array
 * @param input - an address where the return value should be stored
 * @return MTM_SUCCESS if no error occured, or the name of the error occured.
 */
//MtmErrorCode checkArgumentNum(char*** argv){
//    int i;
//    for (i = 1; argv[i]!=NULL ; ++i) {
//    }
//    if(i==0||i==2||i==4)
//        return MTM_SUCCESS;
//    return MTM_INVALID_COMMAND_LINE_PARAMETERS;
//}

/**
 * This function check if the program got "-o" flag.
 * in case there is not such a flag, the return value will be NULL
 * In case the format is invalid (no file name after "-o"), the return
 * value will be the relevant error name.
 *
 * @param argv - an address to an array of char* which holds the arguments of the main
 * @param output - an address to to the string which should hold the output return value
 *
 */
//MtmErrorCode readOutputParameter(char*** argv,char** output,int argc){
//    *output=NULL;
//    for(int i=1;i<argc;i++)
//    {
//        if(strcmp(*argv[i],"-o")==0)
//        {
//            if(argv[i+1]==NULL||strcmp(*argv[i+1],"-i")==0)
//                return MTM_INVALID_COMMAND_LINE_PARAMETERS;
//
//            *output=malloc(strlen(*argv[i+1])+1);
//
//            if(*output==NULL)
//                return (MTM_OUT_OF_MEMORY);
//
//            strcpy(*output,*argv[i+1]);
//            return MTM_SUCCESS;
//        }
//    }
//    return MTM_SUCCESS;
//}

/**
 * This function check if the program got "-i" flag.
 * in case there is not such a flag, the return value will be NULL
 * In case the format is invalid (no file name after "-i"), the return
 * value will be the relevant error name.
 *
 * @param argv - an address to an array of char* which holds the arguments of the main
 * @param input - an address to to the string which should hold the input return value
 *
 */
//MtmErrorCode readInputParameter(char*** argv, char** input,int argc){
//    *input=NULL;
//    for(int i=1;i<argc;i++)
//    {
//        if(strcmp(*argv[i],"-i")==0)
//        {
//            if(argv[i+1]==NULL||strcmp(*argv[i+1],"-o")==0)
//                return MTM_INVALID_COMMAND_LINE_PARAMETERS;
//
//            *input=malloc(strlen(*argv[i+1])+1);
//
//            if(*input==NULL)
//                return (MTM_OUT_OF_MEMORY);
//
//            strcpy(*input,*argv[i+1]);
//            return MTM_SUCCESS;
//        }
//    }
//    return MTM_SUCCESS;
//}

MtmErrorCode getLine(FILE* input,System system,FILE* output){
    MtmErrorCode err;
    char line[BUFFER_SIZE];
    char* command_and_arguments;
    while(fgets(line,BUFFER_SIZE,input)!=NULL) {
        int letters_copied = 0;
        int flag=0;
        command_and_arguments=malloc(strlen(line)+1);
        if(command_and_arguments==NULL)
            return MTM_OUT_OF_MEMORY;
        for (int i = 0; i < strlen(line); i++) {
            if(line[i]=='\t'||line[i]=='\n'){
                continue;
            }
            if (line[i] == '#' && letters_copied == 0) {
                flag=0;
                i = (int) strlen(line);
                continue;
            }
            if (line[i] != ' ' && line[i]){
                flag=1;
                command_and_arguments[letters_copied++] = line[i];
            }
            else {
                if (letters_copied != 0 && line[i - 1] != ' '){
                    flag=1;
                    command_and_arguments[letters_copied++] = line[i];
                }
            }
        }
        if(flag==1) {
            command_and_arguments[letters_copied] = '\0';
            err = matchCommand(command_and_arguments, system, output);
            if (err != MTM_SUCCESS) {
                free(command_and_arguments);
                return err;
            }
        }
        free(command_and_arguments);
    }
    return MTM_SUCCESS;
}

MtmErrorCode matchCommand(char* line,System system,FILE* output) {
    if(strcmp(line,"SystemDestroy")==0) {
        DestroySystem(system);
        return MTM_SUCCESS;
    }
    char* parameters[MAX_PARAMETERS];
    int parameters_inserted = 0;
    MtmErrorCode err=MTM_INVALID_COMMAND_LINE_PARAMETERS;
    char* token=line;
    token = strtok(token, " -");
    while (token != NULL) {
        parameters[parameters_inserted] = malloc(strlen(token)+1);
        strcpy(parameters[parameters_inserted],token);
        parameters_inserted++;
        token = strtok(NULL, " -\t");
    }
    if (parameters_inserted < 2)
        err= MTM_INVALID_COMMAND_LINE_PARAMETERS;
    if (strcmp(parameters[0], "company") == 0 &&
        strcmp(parameters[1], "add") == 0) {
        if(parameters_inserted < 2)
            err=MTM_INVALID_COMMAND_LINE_PARAMETERS;
        else
            err= (CompanyAdd(system,parameters[2], (TechnionFaculty)atoi(parameters[3])));
    }
    if (strcmp(parameters[0], "company") == 0 &&
        strcmp(parameters[1], "remove") == 0) {
        if(parameters_inserted < 1)
            err=MTM_INVALID_COMMAND_LINE_PARAMETERS;
        else
            err= CompanyRemove(system,parameters[2]);
    }
    if (strcmp(parameters[0], "room") == 0 &&
        strcmp(parameters[1], "add") == 0 ) {
        if(parameters_inserted < 7)
            err=MTM_INVALID_COMMAND_LINE_PARAMETERS;
        else
            err= RoomAdd(system,parameters[2], atoi(parameters[3]), atoi(parameters[4]),
                         atoi(parameters[5]), atoi(parameters[6]),
                         atoi(parameters[7]), atoi(parameters[8]));
    }
    if (strcmp(parameters[0], "room") == 0 &&
        strcmp(parameters[1], "remove") == 0 ) {
        if(parameters_inserted < 2)
            err=MTM_INVALID_COMMAND_LINE_PARAMETERS;
        else
            err= RoomRemove(system,(TechnionFaculty)atoi(parameters[2]), atoi(parameters[3]));
    }
    if (strcmp(parameters[0], "escaper") == 0 &&
        strcmp(parameters[1], "add") == 0) {
        if(parameters_inserted < 3)
            err=MTM_INVALID_COMMAND_LINE_PARAMETERS;
        else
            err= EscaperAdd(system,parameters[2], (TechnionFaculty)atoi(parameters[3]), atoi(parameters[4]));
    }
    if (strcmp(parameters[0], "escaper") == 0 &&
        strcmp(parameters[1], "remove") == 0) {
        if(parameters_inserted < 1)
            err=MTM_INVALID_COMMAND_LINE_PARAMETERS;
        else
            err= EscaperRemove(system,parameters[2]);
    }
    if (strcmp(parameters[0], "escaper") == 0 &&
        strcmp(parameters[1], "order") == 0) {
        if(parameters_inserted < 6)
            err=MTM_INVALID_COMMAND_LINE_PARAMETERS;
        else
            err= EscaperOrder(system,parameters[2], (TechnionFaculty)atoi(parameters[3]),
                              atoi(parameters[4]), atoi(parameters[5]), atoi(parameters[6]),
                              atoi(parameters[7]));
    }
    if(strcmp(parameters[0],"escaper")==0 &&
       strcmp(parameters[1],"recommend")==0){
        if(parameters_inserted<3) {
            err = MTM_INVALID_COMMAND_LINE_PARAMETERS;
        }
        else {
            err = EscaperRecommend(system, parameters[2], atoi(parameters[3]));
        }
    }
    if (strcmp(parameters[0], "report") == 0 &&
        strcmp(parameters[1], "day") == 0) {
        err= ReportDay(system,output);
    }
    if (strcmp(parameters[0], "report") == 0 &&
        strcmp(parameters[1], "best") == 0) {
        err= ReportBest(system,output);
    }
    for(int i=0;i<parameters_inserted;i++){
        free(parameters[i]);
    }
    return err;
}
