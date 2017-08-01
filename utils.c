#include <stdio.h>

typedef enum ARE = {'a', 'r', 'e'};
typedef enum boolean = {NO, YES};

typedef struct commands
{
    char* opCode;
    int numOfOperands;
    int srcOp;
    int destOp;
    ARE encoding; 
}commands;

typedef struct symbolTable
{
    char* lableName;
    int adress;
    boolean ext;
    boolean act;
}symbolTable;

char* r0, r1, r2, r3, r4, r5, r6, r7;
r0 = "0000";
r1 = "0001";
r2 = "0010";
r3 = "0011";
r4 = "0100";
r5 = "0101";
r6 = "0110";
r7 = "0111";

char* mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop;
mov = "0000";
cmp = "0001";
add = "0010";
sub = "0011";
not = "0100";
clr = "0101";
lea = "0110";
inc = "0111";
dec = "1000";
jmp = "1001";
bne = "1010";
red = "1011";
prn = "1100";
jsr = "1101";
rts = "1110";
stop = "1111";

char* immidiate_adress = "00"; /* operands that starts with # and a real number right after */
char* direct_adress = "01"; /*  the operand is a lable that wad/will be declare in the code*/
char* mat_adress = "10"; /* the operand is a matrix with the registers inside. for instance: M[r2][r5] */
char* direct_reg = "11"; /* for two registers r0 - r7 operands*/

