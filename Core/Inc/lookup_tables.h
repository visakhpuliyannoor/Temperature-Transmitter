/*
 * lookup_tables.h
 *
 *  Created on: 15-Jan-2026
 *      Author: visakh.p
 */

#ifndef INC_LOOKUP_TABLES_H_
#define INC_LOOKUP_TABLES_H_

#include <stdint.h>



// Sensor type definitions (from your code)
typedef enum {
    RTD = 0,
    MA_4_20,
    MA_0_20,
    VOLT_0_1,
    VOLT_0_5,
    VOLT_0_10,
    K_TYPE,
    J_TYPE,
    R_TYPE,
    S_TYPE,
    B_TYPE,
    N_TYPE,
    T_TYPE
} SensorType_t;


// K-Type Tables
extern float const  mvktable[8] ;
extern float const slopktable[8];
extern float const consktable[8];


// J-Type Tables
extern float const mvjtable[8];
extern float const slopjtable[8];
extern float const consjtable[8];

// R-Type Tables
extern float const mvrtable[14] ;
extern float const sloprtable[14];
extern float const consrtable[14];


// S-Type Tables
extern float const mvstable[13];
extern float const slopstable[13];
extern float const consstable[13];


// B-Type Tables
extern float const mvbtable[26] ;
extern float const slopbtable[26];
extern float const consbtable[26];


// T-Type Tables
extern float const mvttable[68];
extern float const slopttable[68];
extern float const consttable[68] ;


// N-Type Tables
extern float const mvntable[21];

extern float const slopntable[21];

extern float const consntable[21];

// RTD Tables
extern float const mvrtdtable[10];
extern float const sloprtdtable[10];
extern float const constrtdtable[10];
extern int const temprtd[10];



#endif /* INC_LOOKUP_TABLES_H_ */
