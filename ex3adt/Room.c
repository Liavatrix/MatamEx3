//
// Created by orerez on 08/06/2017.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Room.h"


struct escRoom {
    char* email;
    TechnionFaculty faculty;
    int id;
    int price;
    int num_people;
    int difficulty;
    int open_time;
    int close_time;
};


static inline int ValidateTime(int time) {
    return (time > -1 && time < 25);
}

static inline int ValidateDifficulty(int difficulty) {
    return (difficulty > 0 && difficulty < 11);
}

static MtmErrorCode ValidateRoomParameters(char* Email,int id,int price,int num_ppl, int start_time,
                                           int close_time,int difficulty) {
    if(Email==NULL)
        return MTM_NULL_PARAMETER;
    if(id<=0 || num_ppl<0 || price<0 ||price%4!=0)
        return MTM_INVALID_PARAMETER;
    if(!ValidateTime(start_time) || !ValidateTime(close_time))
        return MTM_INVALID_PARAMETER;
    if(start_time==close_time)
        return MTM_INVALID_PARAMETER;
    if(!ValidateDifficulty(difficulty))
        return MTM_INVALID_PARAMETER;

    return MTM_SUCCESS;
}

Room CreateRoom(char* email,TechnionFaculty faculty , int id, int price, int num_ppl, int difficulty, int start_time,
                int close_time, MtmErrorCode* errorCode) {
    *errorCode = ValidateRoomParameters(email,id,price,num_ppl,start_time,close_time,difficulty);
    if(*errorCode!=MTM_SUCCESS)
        return NULL;
    Room room = malloc(sizeof(*(room)));
    char* new_email = malloc(strlen(email)+1);
    if(room==NULL || new_email == NULL) {
        *errorCode=MTM_OUT_OF_MEMORY;
        return NULL;
    }

    strcpy(new_email,email);
    room->email=new_email;
    room->faculty=faculty;
    room->id=id;
    room->price=price;
    room->num_people=num_ppl;
    room->difficulty=difficulty;
    room->open_time=start_time;
    room->close_time=close_time;

    *errorCode = MTM_SUCCESS;
    return room;
}

MtmErrorCode DestroyRoom(Room room) {
    free(room->email);
    free(room);
    return MTM_SUCCESS;
}

int GetRoomID(Room room){
    assert(room!=NULL);
    return room == NULL ? -1 : room->id;
}

int GetRoomPrice(Room room){
    assert(room!=NULL);
    return room == NULL ? -1 : room->price;
}

int GetNumPeople(Room room){
    assert(room!=NULL);
    return room == NULL ? -1 : room->num_people;
}

int GetRoomDifficulty(Room room){
    assert(room!=NULL);
    return room == NULL ? -1 : room->difficulty;
}

int GetRoomStartTime(Room room){
    assert(room!=NULL);
    return room == NULL ? -1 : room->open_time;
}

int GetRoomCloseTime(Room room){
    assert(room!=NULL);
    return room == NULL ? -1 : room->close_time;
}

Room CopyRoom(Room room) {
    assert(room!=NULL);
    Room new_room = malloc(sizeof(*room));
    char* new_email = malloc(strlen(room->email)+1);
    if(new_room==NULL || new_email==NULL)
        return NULL;
    strcpy(new_email,room->email);
    new_room->email=new_email;
    new_room->id=room->id;
    new_room->faculty=room->faculty;
    new_room->price=room->price;
    new_room->num_people=room->num_people;
    new_room->difficulty=room->difficulty;
    new_room->open_time=room->open_time;
    new_room->close_time=room->close_time;

    return new_room;
}

