//
// Created by orerez on 08/06/2017.
//

#include "mtm_ex3.h"

#ifndef EX3ADT_ROOM_H
#define EX3ADT_ROOM_H

typedef struct escRoom* Room;


Room CreateRoom(char* email,TechnionFaculty faculty , int id, int price, int num_ppl, int difficulty, int start_time,
                int close_time,MtmErrorCode* Error);

MtmErrorCode DestroyRoom(Room room);

int GetRoomID(Room room);

int GetRoomPrice(Room room);

int GetNumPeople(Room room);

int GetRoomDifficulty(Room room);

int GetRoomStartTime(Room room);

int GetRoomCloseTime(Room room);

Room CopyRoom(Room room);


#endif //EX3ADT_ROOM_H
