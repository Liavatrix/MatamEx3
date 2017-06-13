//
// Created by orerez on 08/06/2017.
//

#ifndef EX3ADT_COMPANY_H
#define EX3ADT_COMPANY_H

#include "Room.h"
#include "mtm_ex3.h"
#include "Order.h"


typedef struct escCompany* Company;


/**
* Allocates a new escape room company.
*
* Creates a new empty company, and allocates memory for it's rooms.
* Additionally, it initiates the company's values according to parameters.
*
* @param email The company's email. (Used as unique search key).
 *
* @param faculty An enum reflecting which faculty owns the company.
 *
* @return
* 	MTM_NULL_PARAMETER if one of the parameters is NULL
 * 	MTM_OUT_OF_MEMORY if allocations failed.
 * 	MTM_EMAIL_ALREADY_EXISTS if email is already used by another entity.
* 	A new Company in case of success.
*/

Company CreateCompany(char* email, TechnionFaculty faculty,MtmErrorCode* error_code);

Company CopyCompany(Company company,MtmErrorCode* error_code);

MtmErrorCode DestroyCompany(Company company);

MtmErrorCode CreateInsertRoom(Company company , int id, int price, int num_ppl, int difficulty,
                              int start_time, int end_time);

MtmErrorCode RemoveDestroyRoom(Company company, Room room);

MtmErrorCode ClearCompanyRooms(Company company);

Room GetFirstRoom(Company company);

Room GetNextRoom(Company company);

Room SearchRoom(Company company,int id);

Room RoomOfOrder(Company company, Order order);

int GetCompanyIncome(Company company);

int GetNumRooms(Company company);

MtmErrorCode UpdateCompanyIncome(Company company , Order order, int is_from_faculty);

TechnionFaculty GetCompanyFaculty(Company company);

char* GetCompanyEmail(Company company);

#endif //EX3ADT_COMPANY_H