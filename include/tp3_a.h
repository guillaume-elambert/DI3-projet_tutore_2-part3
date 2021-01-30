
#ifndef LR_A

#define LR_A 1
#define LR_D 2
#define LR_R 3
#define LR_E -1
 
#define JSON_T_LCB 0     /**< { */
#define JSON_T_RCB 1     /**< } */
#define JSON_T_LB 2      /**< [ */
#define JSON_T_RB 3      /**< ] */
#define JSON_T_COMMA 4   /**< , */
#define JSON_T_COLON 5   /**< : */
#define JSON_T_STRING 6  /**< s (chaine de caractère) */
#define JSON_T_NUMBER 7  /**< n (nombre entier ou réel) */
#define JSON_T_TRUE 8    /**< t (true) */
#define JSON_T_FALSE 9   /**< f (false) */
#define JSON_T_NULL 10   /**< u (null) */
#define JSON_T_ERROR 11  /**< erreur */
#define O 12
#define M 13
#define P 14
#define A 15
#define E 16
#define V 17



const int tableAction[26][12] = {
//     {     }     [     ]     ,     :     s     n     t     f     u     #  
    {LR_D, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//0
    {LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_A},//1
    {LR_E, LR_D, LR_E, LR_E, LR_E, LR_E, LR_D, LR_E, LR_E, LR_E, LR_E, LR_E},//2
    {LR_E, LR_D, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//3
    {LR_E, LR_R, LR_E, LR_E, LR_D, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//4
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_R},//5
    {LR_E, LR_E, LR_E, LR_E, LR_E, LR_D, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//6
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_R},//7
    {LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_D, LR_E, LR_E, LR_E, LR_E, LR_E},//8
    {LR_D, LR_E, LR_D, LR_E, LR_E, LR_E, LR_D, LR_D, LR_D, LR_D, LR_D, LR_E},//9
    {LR_E, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//10
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//11
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//12
    {LR_E, LR_R, LR_E, LR_E, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//13
    {LR_D, LR_E, LR_D, LR_D, LR_E, LR_E, LR_D, LR_D, LR_D, LR_D, LR_D, LR_E},//14
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//15
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//16
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//17
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//18
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//19
    {LR_E, LR_E, LR_E, LR_D, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//20
    {LR_E, LR_E, LR_E, LR_R, LR_D, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//21
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//22
    {LR_E, LR_R, LR_E, LR_R, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E},//23
    {LR_D, LR_E, LR_D, LR_E, LR_E, LR_E, LR_D, LR_D, LR_D, LR_D, LR_D, LR_E},//24
    {LR_E, LR_E, LR_E, LR_R, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E, LR_E} //25

};

//tableau LR_D'analyse

#define err -1

#define r1 0
#define r2 1
#define r3 2
#define r4 3
#define r5 4
#define r6 5
#define r7 6
#define r8 7
#define r9 8
#define r10 9
#define r11 10
#define r12 11
#define r13 12
#define r14 13
#define r15 14
#define r16 15

#define d1 1
#define d2 2
#define d3 3
#define d4 4
#define d5 5
#define d6 6
#define d7 7
#define d8 8
#define d9 9
#define d10 10
#define d11 11
#define d12 12
#define d13 13
#define d14 14
#define d15 15
#define d16 16
#define d17 17 
#define d18 18
#define d19 19
#define d20 20
#define d21 21
#define d22 22
#define d23 23
#define d24 24
#define d25 25


const int tableAnalyse[26][18] = {
//     {     }    [    ]    ,    :    s    n    t    f    u   #    O    M    P    A    E    V
    { d2 , err, err, err, err, err, err, err, err, err, err, err, d1 , err, err, err, err, err },//i0
    { err, err, err, err, err, err, err, err, err, err, err, err, err, err, err, err, err, err },//i1
    { err, d5 , err, err, err, err, d6 , err, err, err, err, err, err, d3 , d4 , err, err, err },//i2
    { err, d7 , err, err, err, err, err, err, err, err, err, err, err, err, err, err, err, err },//i3
    { err, r3 , err, err, d8 , err, err, err, err, err, err, err, err, err, err, err, err, err },//i4
    { err, r1 , err, r1 , r1 , err, err, err, err, err, err, r1 , err, err, err, err, err, err },//i5
    { err, err, err, err, err, d9 , err, err, err, err, err, err, err, err, err, err, err, err },//i6
    { err, r2 , err, r2 , r2 , err, err, err, err, err, err, r2 , err, err, err, err, err, err },//i7
    { err, err, err, err, err, err, d6 , err, err, err, err, err, err, d10, d4 , err, err, err },//i8
    { d2 , err, d14, err, err, err, d15, d16, d17, d18, d19, err, d11, err, err, d12, err, d13 },//i9
    { err, r4 , err, err, err, err, err, err, err, err, err, err, err, err, err, err, err, err },//i10
    { err, r12, err, r12, r12, err, err, err, err, err, err, err, err, err, err, err, err, err },//i11
    { err, r13, err, r13, r13, err, err, err, err, err, err, err, err, err, err, err, err, err },//i12
    { err, r5 , err, err, r5 , err, err, err, err, err, err, err, err, err, err, err, err, err },//i13
    { d2 , err, d14, d22, err, err, d15, d16, d17, d18, d19, err, d11, err, err, d12, d20, d21 },//i14
    { err, r10, err, r10, r10, err, err, err, err, err, err, err, err, err, err, err, err, err },//i15
    { err, r11, err, r11, r11, err, err, err, err, err, err, err, err, err, err, err, err, err },//i16
    { err, r14, err, r14, r14, err, err, err, err, err, err, err, err, err, err, err, err, err },//i17
    { err, r15, err, r15, r15, err, err, err, err, err, err, err, err, err, err, err, err, err },//i18
    { err, r16, err, r16, r16, err, err, err, err, err, err, err, err, err, err, err, err, err },//i19
    { err, err, err, d23, err, err, err, err, err, err, err, err, err, err, err, err, err, err },//i20
    { err, err, err, r8 , d24, err, err, err, err, err, err, err, err, err, err, err, err, err },//i21
    { err, r6 , err, r6 , r6 , err, err, err, err, err, err, err, err, err, err, err, err, err },//i22
    { err, r7 , err, r7 , r7 , err, err, err, err, err, err, err, err, err, err, err, err, err },//i23
    { d2 , err, d14, err, err, err, d15, d16, d17, d18, d19, err, d11, err, err, d12, d25, d21 },//i24
    { err, err, err, r9 , err, err, err, err, err, err, err, err, err, err, err, err, err, err } //i25
};

//taille de la partie droite de la regle
//                                     1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
const int taillePartieDroiteRegle[16]={2,3,1,3,3,2,3,1,3,1, 1, 1, 1, 1, 1, 1};

//Auxiliaire partie gauche
//                               1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 
const int reglePartieGauche[16]={O,O,M,M,P,A,A,E,E,V ,V ,V ,V ,V ,V ,V };

int analyseurLR(TLex *_data);
int traduction(int _lex);

#endif