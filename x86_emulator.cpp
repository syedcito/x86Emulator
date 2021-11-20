/*
* Syed Ali
* Prof. Vulis
* 11/10/2021
* CSc 210 limited x86 emulator
*/
#include <iostream>
#include <bitset>
#include <stdio.h>

using namespace std;
#define largest_mem 0x100000
#define byte unsigned char
#define word unsigned short

byte AL = 0x00, AH = 0x00, BL = 0x00, BH = 0x00, CL = 0x00, CH = 0x00, DL = 0x00, DH = 0x00;
word AX = 0x0000, CX = 0x0000, DX = 0x0000, BX = 0x0000, SP = 0x0000, BP = 0x0000, SI = 0x0000, DI = 0x0000, temp_16;
word IP = 0x00;
byte memory[0x100000];
byte opcode = 0x00;
string printAsm;
bool ZF, CF, OF;
word together(byte low, byte hi)
{
    word combine = (hi << 8) | low;
    return combine;
}
void printRegisters(){
    printf("AL = %d \t AH = %d\t AX = %d \n", AL, AH, AX );
    printf("CL = %d \t CH = %d\t CX = %d \n", CL, CH, CX );
    printf("DL = %d \t DH = %d\t DX = %d \n", DL, DH, DX );
    printf("BL = %d \t BH = %d\t BX = %d \n", AL, AH, AX );
    printf("SP = %d \t BP = %d\t SI = %d \t DI = %d \n", SP, BP, SI, DI);
    printf("Zero Flag: %d \t Carry Flag: %D \t Overflow Flag: %d \n", ZF, CF, OF);
}
void updateRegister16() {
    AX = AH + AL;
    BX = BH + BL;
    CX = CH + CL;
    DX = DH + DL;
}
void updateRegister8()
{
    AL = AX & 0xff;
    AH = (AX & 0xff00) >> 8;
    CL = CX & 0xff;
    CH = (CX & 0xff00) >> 8;
    DL = DX & 0xff;
    DH = (DX & 0xff00) >> 8;
    BL = BX & 0xff;
    BH = (BX & 0xff00) >> 8;
}
void addRegis8(byte& reg1, byte& reg2){
    //contents in right register are added to left register
    if (reg1 > 0xFF - reg2){ OF = 1;  CF = 1;}
    reg1 += reg2;
    if (reg1 == 0) ZF = 1;
}
void addRegis16(word& reg1, word reg2, byte& reg1h, byte& reg1l, byte& reg2h, byte& reg2l) {
    if (reg1 > 0xFFFF - reg2) {OF = 1; CF = 1;}
    reg1 += reg2;
    reg1h = reg1 & 0xFF;
    reg1l = reg1 >> 8 & 0xFF;
    if(!reg1) ZF = 1;
}

void addRegis16s(word& reg1, word& reg2){
    if (reg1 > 0xFFFF - reg2) OF = 1; CF = 1;
    reg1 += reg2;
    if (reg1 == 0) ZF = 1;
}

void sub8(byte& reg1, byte& reg2)
{
    if (reg1 > 0xFF - reg2) {OF = 1; CF = 1;};
    reg1  -= reg2;
    if (!reg1) ZF = 1;
}
void sub16(word& reg1, word& reg2) {
    if (reg1 > 0xFFFF - reg2) {OF = 1; CF = 1; };
    reg1 -= reg2;
    if (!reg1) ZF = 1;
}

void SUB16_8(word& a, byte& b)
{
    if (a > 0xFFFF - b) {CF = 1; OF = 1;};
    a -= b;
    if (!a) ZF = 1;
}
void addRegis16_8(word& a, byte& b)
{
    if (a > 0xFFFF - b) { OF = 1; CF = 1; };
    a += b;
    if (!a) ZF = 1;
}

void run(byte opcode) {
    switch (opcode) {
        //moving constants to a 8bit register
        case 0xB0:
            opcode = memory[IP++];
            AL = opcode;
            updateRegister16();
            break;
        case 0xB1:
            opcode = memory[IP++];
            CL = opcode;
            updateRegister16();
            break;
        case 0xB2:
            opcode = memory[IP++];
            DL = opcode;
            updateRegister16();
            break;
        case 0xB3:
            opcode = memory[IP++];
            BL = opcode;
            updateRegister16();
            break;
        case 0xB4:
            opcode = memory[IP++];
            AH = opcode;
            updateRegister16();
            break;
        case 0xB5:
            opcode = memory[IP++];
            CH = opcode;
            updateRegister16();
            break;
        case 0xB6:
            opcode = memory[IP++];
            DH = opcode;
            updateRegister16();
            break;
        case 0xB7:
            opcode = memory[IP++];
            BH = opcode;
            updateRegister16();
            break;
            //moving constants to 16 bit registers
        case 0xB8:
            opcode = memory[IP++];
            AL = opcode;
            opcode = memory[IP++];
            AX = opcode;
            updateRegister16();
            break;
        case 0xB9:
            opcode = memory[IP++];
            CL = opcode;
            opcode = memory[IP++];
            CX = opcode;
            updateRegister16();
            break;
        case 0xBA:
            opcode = memory[IP++];
            DL = opcode;
            opcode = memory[IP++];
            DX = opcode;
            updateRegister16();
            break;
        case 0xBB:
            opcode = memory[IP++];
            BL = opcode;
            opcode = memory[IP++];
            BX = opcode;
            updateRegister16();
            break;
        case 0xBC:
            opcode = memory[IP++];
            break;
        case 0xBD:
            opcode = memory[IP++];
            break;
        case 0xBE:
            opcode = memory[IP++];
            break;
        case 0xBF:
            opcode = memory[IP++];
            break;
            //adding 8 bit registers
        case 0x02:
            opcode = memory[IP++];
            switch (opcode) {
                case 0xC0:
                    //add AL, AL
                    addRegis8(AL, AL);
                    updateRegister16();
                    break;
                case 0xC1:
                    //add AL, CL
                    addRegis8(AL, CL);
                    updateRegister16();
                    break;
                case 0xC2:
                    //add AL, DL
                    addRegis8(AL, DL);
                    updateRegister16();
                    break;
                case 0xC3:
                    //add AL, BL
                    addRegis8(AL, BL);
                    updateRegister16();
                    break;
                case 0xC4:
                    //add AL, AH
                    addRegis8(AL, AH);
                    updateRegister16();
                    break;
                case 0xC5:
                    //add AL, CH
                    addRegis8(AL, CH);
                    updateRegister16();
                    break;
                case 0xC6:
                    //add AL, DH
                    addRegis8(AL, DH);
                    updateRegister16();
                    break;
                case 0xC7:
                    //add AL, BH
                    addRegis8(AL, BH);
                    updateRegister16();
                    break;
                case 0xC8:
                    //add CL, AL
                    addRegis8(CL, AL);
                    updateRegister16();
                    break;
                case 0xC9:
                    //add CL, CL
                    addRegis8(CL, CL);
                    updateRegister16();
                    break;
                case 0xCA:
                    //add CL,DL
                    addRegis8(CL, DL);
                    updateRegister16();
                    break;
                case 0xCB:
                    //add CL, BL
                    addRegis8(CL, BL);
                    updateRegister16();
                    break;
                case 0xCC:
                    //add CL, AH
                    addRegis8(CL, AH);
                    updateRegister16();
                    break;
                case 0xCD:
                    //add CL, CH
                    addRegis8(CL, CH);
                    updateRegister16();
                    break;
                case 0xCE:
                    //add CL, DH
                    addRegis8(CL, DH);
                    updateRegister16();
                    break;
                case 0xCF:
                    //add CL, BH
                    addRegis8(CL, BH);
                    updateRegister16();
                    break;
                case 0xD0:
                    //add DL, AL
                    addRegis8(DL, AL);
                    updateRegister16();
                    break;
                case 0xD1:
                    //add DL, CL
                    addRegis8(DL, CL);
                    updateRegister16();
                    break;
                case 0xD2:
                    //add dl,dl
                    addRegis8(DL, DL);
                    updateRegister16();
                    break;
                case 0xD3:
                    //add DL, BL
                    addRegis8(DL, BL);
                    updateRegister16();
                    break;
                case 0xD4:
                    //add DL, AH
                    addRegis8(DL, AH);
                    updateRegister16();
                    break;
                case 0xD5:
                    //add DL, CH
                    addRegis8(DL, CH);
                    updateRegister16();
                    break;
                case 0xD6:
                    //add DL, DH
                    addRegis8(DL, DH);
                    updateRegister16();
                    break;
                case 0xD7:
                    //add DL, BH
                    addRegis8(DL, BH);
                    updateRegister16();
                    break;
                case 0xD8:
                    //add BL, AL
                    addRegis8(BL, AL);
                    updateRegister16();
                    break;
                case 0xD9:
                    //add BL, CL
                    addRegis8(BL, CL);
                    updateRegister16();
                    break;
                case 0xDA:
                    //add BL, DL
                    addRegis8(BL, DL);
                    updateRegister16();
                    break;
                case 0xDB:
                    //add BL, BL
                    addRegis8(BL, BL);
                    updateRegister16();
                    break;
                case 0xDC:
                    //add BL. AH
                    addRegis8(BL, AH);
                    updateRegister16();
                    break;
                case 0xDD:
                    //add BL, CH
                    addRegis8(BL, CH);
                    updateRegister16();
                    break;
                case 0xDE:
                    //add BL, DH
                    addRegis8(BL, DH);
                    updateRegister16();
                    break;
                case 0xDF:
                    //add BL, BH
                    addRegis8(BL, BH);
                    updateRegister16();
                    break;
                case 0xE0:
                    //add ah,al
                    addRegis8(AH, AL);
                    updateRegister16();
                    break;
                case 0xE1:
                    //add ah,cl
                    addRegis8(AH, CL);
                    updateRegister16();
                    break;
                case 0xE2:
                    //add ah,dl
                    addRegis8(AH, DL);
                    updateRegister16();
                    break;
                case 0xE3:
                    //add ah,bl
                    addRegis8(AH, BL);
                    updateRegister16();
                    break;
                case 0xE4:
                    //add ah,ah
                    addRegis8(AH, AH);
                    updateRegister16();
                    break;
                case 0xE5:
                    //add ah,ch
                    addRegis8(AH, CH);
                    updateRegister16();
                    break;
                case 0xE6:
                    //add ah,dh
                    addRegis8(AH, CH);
                    updateRegister16();
                    break;
                case 0xE7:
                    //add ah,bh
                    addRegis8(AH, BH);
                    updateRegister16();
                    break;
                case 0xE8:
                    //add ch,al
                    addRegis8(CH, AL);
                    updateRegister16();
                    break;
                case 0xE9:
                    //add ch,cl
                    addRegis8(CH, CL);
                    updateRegister16();
                    break;
                case 0xEA:
                    //add ch,dl
                    addRegis8(CH, DL);
                    updateRegister16();
                    break;
                case 0xEB:
                    //add ch,bl
                    addRegis8(CH, BL);
                    updateRegister16();
                    break;
                case 0xEC:
                    //add ch,ah
                    addRegis8(CH, AH);
                    updateRegister16();
                    break;
                case 0xED:
                    //add ch,ch
                    addRegis8(CH, CH);
                    updateRegister16();
                    break;
                case 0xEE:
                    //add ch,dh
                    addRegis8(CH, DH);
                    updateRegister16();
                    break;
                case 0xEF:
                    //add ch,bh
                    addRegis8(CH, BH);
                    updateRegister16();
                    break;
                case 0xF0:
                    //add dh,al
                    addRegis8(DH, AL);
                    updateRegister16();
                    break;
                case 0xF1:
                    //add dh,cl
                    addRegis8(DH, CL);
                    updateRegister16();
                    break;
                case 0xF2:
                    //add dh,dl
                    addRegis8(DH, DL);
                    updateRegister16();
                    break;
                case 0xF3:
                    //add dh,bl
                    addRegis8(DH, BL);
                    updateRegister16();
                    break;
                case 0xF4:
                    //add dh,ah
                    addRegis8(DH, AH);
                    updateRegister16();
                    break;
                case 0xF5:
                    //add dh,ch
                    addRegis8(DH, CH);
                    updateRegister16();
                    break;
                case 0xF6:
                    //add dh,dh
                    addRegis8(DH, AH);
                    updateRegister16();
                    break;
                case 0xF7:
                    //add dh,bh
                    addRegis8(DH, BH);
                    updateRegister16();
                    break;
                case 0xF8:
                    //add bh,al
                    addRegis8(BH, AL);
                    updateRegister16();
                    break;
                case 0xF9:
                    //add bh,cl
                    addRegis8(BH, CL);
                    updateRegister16();
                    break;
                case 0xFA:
                    //add bh,dl
                    addRegis8(BH, DL);
                    updateRegister16();
                    break;
                case 0xFB:
                    //add bh,bl
                    addRegis8(BH, BL);
                    updateRegister16();
                    break;
                case 0xFC:
                    //add bh,ah
                    addRegis8(BH, AH);
                    updateRegister16();
                    break;
                case 0xFD:
                    //add bh,ch
                    addRegis8(BH, CH);
                    updateRegister16();
                    break;
                case 0xFE:
                    //add bh,dh
                    addRegis8(BH, DH);
                    updateRegister16();
                    break;
                case 0xFF:
                    //add bh,bh
                    addRegis8(BH, BH);
                    updateRegister16();
                    break;
            }
            break;
        case 0x03:
            //adding 16 bit registers
            opcode = memory[IP++];
            switch (opcode) {
                case 0xC0:
                    //add ax,ax
                    addRegis16(AX, AX, AL, AH, AL, AH);
                    break;
                case 0xC1:
                    //add ax,cx
                    addRegis16(AX, CX, AL, AH, CL, CH);
                    break;
                case 0xC2:
                    //add ax,dx
                    addRegis16(AX, DX, AL, AH, DL, DH);
                    break;
                case 0xC3:
                    //add ax,bx
                    addRegis16(AX, BX, AL, AH, BL, BH);
                    break;
                case 0xC4:
                    //add ax,sp
                    addRegis16(AX, SP, AL, AH, AL, AH);
                    break;
                case 0xC5:
                    //add ax,bp
                    addRegis16(AX, BP, AL, AH, AL, AH);
                    break;
                case 0xC6:
                    //add ax,si
                    addRegis16(AX, SI, AL, AH, AL, AH);
                    break;
                case 0xC7:
                    //add ax,di
                    addRegis16(AX, DI, AL, AH, AL, AH);
                    break;
                case 0xC8:
                    //add cx,ax
                    addRegis16(CX, AX, CL, CH, AL, AH);
                    break;
                case 0xC9:
                    //add cx,cx
                    addRegis16(CX, CX, CL, CH, CL, CH);
                    break;
                case 0xCA:
                    //add cx,dx
                    addRegis16(CX, DX, CL, CH, DL, DH);
                    break;
                case 0xCB:
                    //add cx,bx
                    addRegis16(CX, BX, CL, CH, BL, BH);
                    break;
                case 0xCC:
                    //add cx,sp
                    addRegis16(CX, SP, CL, CH, CL, CH);
                    break;
                case 0xCD:
                    //add cx,bp
                    addRegis16(CX, BP, CL, CH, CL, CH);
                    break;
                case 0xCE:
                    //add cx,si
                    addRegis16(CX, SI, CL, CH, CL, CH);
                    break;
                case 0xCF:
                    //add cx,di
                    addRegis16(CX, DI, CL, CH, CL, CH);
                    break;
                case 0xD0:
                    //add dx,ax
                    addRegis16(DX, AX, DL, DH, AL, AH);
                    break;
                case 0xD1:
                    //add dx,cx
                    addRegis16(DX, CX, DL, DH, CL, CH);
                    break;
                case 0xD2:
                    //add dx,dx
                    addRegis16(DX, DX, DL, DH, DL, DH);
                    break;
                case 0xD3:
                    //add dx,bx
                    addRegis16(DX, BX, DL, DH, BL, BH);
                    break;
                case 0xD4:
                    //add dx,sp
                    addRegis16(DX, SP, DL, DH, DL, DH);
                    break;
                case 0xD5:
                    //add dx,bp
                    addRegis16(DX, BP, DL, DH, DL, DH);
                    break;
                case 0xD6:
                    //add dx,si
                    addRegis16(DX, SI, DL, DH, DL, DH);
                    break;
                case 0xD7:
                    //add dx,di
                    addRegis16(DX, DI, DL, DH, DL, DH);
                    break;
                case 0xD8:
                    //add bx,ax
                    addRegis16(BX, AX, BL, BH, AL, AH);
                    break;
                case 0xD9:
                    //add bx,cx
                    addRegis16(BX, CX, BL, BH, CL, CH);
                    break;
                case 0xDA:
                    //add bx,dx
                    addRegis16(BX, DX, BL, BH, DL, DH);
                    break;
                case 0xDB:
                    //add bx,bx
                    addRegis16(BX, BX, BL, BH, BL, BH);
                    break;
                case 0xDC:
                    //add bx,sp
                    addRegis16(BX, SP, BL, BH, BL, BH);
                    break;
                case 0xDD:
                    //add bx,bp
                    addRegis16(BX, BP, BL, BH, BL, BH);
                    break;
                case 0xDE:
                    //add bx,si
                    addRegis16(BX, SI, BL, BH, BL, BH);
                    break;
                case 0xDF:
                    //add bx,di
                    addRegis16(BX, DI, BL, BH, BL, BH);
                    break;
                case 0xE0:
                    //add sp,ax
                    addRegis16s(SP, AX);
                    break;
                case 0xE1:
                    //add sp,cx
                    addRegis16s(SP, CX);
                    break;
                case 0xE2:
                    //add sp,dx
                    addRegis16s(SP, DX);
                    break;
                case 0xE3:
                    //add sp,bx
                    addRegis16s(SP, BX);
                    break;
                case 0xE4:
                    //add sp,sp
                    addRegis16s(SP, SP);
                    break;
                case 0xE5:
                    //add sp,bp
                    addRegis16s(SP, BP);
                    break;
                case 0xE6:
                    //add sp,si
                    addRegis16s(SP, SI);
                    break;
                case 0xE7:
                    //add sp,di
                    addRegis16s(SP, DI);
                    break;
                case 0xE8:
                    //add bp,ax
                    addRegis16s(BP, AX);
                    break;
                case 0xE9:
                    //add bp,cx
                    addRegis16s(BP, CX);
                    break;
                case 0xEA:
                    //add bp,dx
                    addRegis16s(BP, DX);
                    break;
                case 0xEB:
                    //add bp,bx
                    addRegis16s(BP, BX);
                    break;
                case 0xEC:
                    //add bp,sp
                    addRegis16s(BP, SP);
                    break;
                case 0xED:
                    //add bp,bp
                    addRegis16s(BP, BP);
                    break;
                case 0xEE:
                    //add bp,si
                    addRegis16s(BP, SI);
                    break;
                case 0xEF:
                    //add bp,di
                    addRegis16s(BP, DI);
                    break;
                case 0xF0:
                    //add si,ax
                    addRegis16s(SI, AX);
                    break;
                case 0xF1:
                    //add si,cx
                    addRegis16s(SI, CX);
                    break;
                case 0xF2:
                    //add si,dx
                    addRegis16s(SI, DX);
                    break;
                case 0xF3:
                    //add si,bx
                    addRegis16s(SI, BX);
                    break;
                case 0xF4:
                    //add si,sp
                    addRegis16s(SI, SP);
                    break;
                case 0xF5:
                    //add si,bp
                    addRegis16s(SI, BP);
                    break;
                case 0xF6:
                    //add si,si
                    addRegis16s(SI, SI);
                    break;
                case 0xF7:
                    //add si,di
                    addRegis16s(SI, DI);
                    break;
                case 0xF8:
                    //add di,ax
                    addRegis16s(DI, AX);
                    break;
                case 0xF9:
                    //add di,cx
                    addRegis16s(DI, CX);
                    break;
                case 0xFA:
                    //add di,dx
                    addRegis16s(DI, DX);
                    break;
                case 0xFB:
                    //add di,bx
                    addRegis16s(DI, BX);
                    break;
                case 0xFC:
                    //add di,sp
                    addRegis16s(DI, SP);
                    break;
                case 0xFD:
                    //add di,bp
                    addRegis16s(DI, BP);
                    break;
                case 0xFE:
                    //add di,si
                    addRegis16s(DI, SI);
                    break;
                case 0xFF:
                    //add di,di
                    addRegis16s(DI, DI);
                    break;
            }

            break;
        case 0x04:
            //add AL, imm8
            opcode = memory[IP++];
            if (AL > 0xFF - opcode) {
                CF = 1;
                OF = 1;
            }
            AL += opcode;
            if (!AL) ZF = 1;
            updateRegister16();
            break;
        case 0x05:
            //add AX, imm16
            opcode = memory[IP++];
            if (AX > 0xFFFF - opcode) {
                CF = 1;
                OF = 1;
            }
            AL += opcode;
            opcode = memory[IP++];
            AH += opcode;
            updateRegister16();
            if (!AX) ZF = 1;
            break;

        case 0x81:
            //add imm16 to 16 bit registers
            opcode = memory[IP++];
            switch (opcode) {
                case 0xC1:
                    //add cx,imm16
                    opcode = memory[IP++];
                    CL = opcode;
                    opcode = memory[IP++];
                    CH = opcode;
                    if (CX > 0xFFFF - ((CH << 8) | CL)) {
                        OF = 1;
                        CF = 1;
                    }
                    CX += (CH << 8) | CL;
                    if (!CX) ZF = 1;
                    break;
                case 0xC2:
                    //add dx,imm16
                    opcode = memory[IP++];
                    DL = opcode;
                    opcode = memory[IP++];
                    DH = opcode;
                    if (DX > 0xFFFF - (DH << 8) | DL) {
                        OF = 1;
                        CF = 1;
                    }
                    DX += (DH << 8) | DL;
                    if (!DX) ZF = 1;
                    break;
                case 0xC3:
                    //add bx,imm16
                    opcode = memory[IP++];
                    BL = opcode;
                    opcode = memory[IP++];
                    BH = opcode;
                    if (BX > 0xFFFF - (BH << 8) | BL) {
                        OF = 1;
                        CF = 1;
                    }
                    BX += (BH << 8) | BL;
                    if (!BX) ZF = 1;
                    break;
                case 0xC4:
                    //add sp,imm16
                    opcode = memory[IP++];
                    if (SP > 0xFFFF - ((opcode << 8) | memory[IP])) {
                        OF = 1;
                        CF = 1;
                    }
                    SP += (opcode << 8) | memory[IP++];
                    if (!SP) ZF = 1;
                    break;
                case 0xC5:
                    //add bp,imm16
                    opcode = memory[IP++];
                    if (BP > 0xFFFF - ((opcode << 8) | memory[IP])) {
                        OF = 1;
                        CF = 1;
                    }
                    BP += (opcode << 8) | memory[IP++];
                    if (!BP) ZF = 1;
                    break;
                case 0xC6:
                    //add si,imm16
                    opcode = memory[IP++];
                    if (SI > 0xFFFF - ((opcode << 8) | memory[IP])) {
                        OF = 1;
                        CF = 1;
                    }
                    SI += (opcode << 8) | memory[IP++];
                    if (!DI) ZF = 1;
                    break;
                case 0xC7:
                    //add di,imm16
                    opcode = memory[IP++];
                    if (DI > 0xFFFF - ((opcode << 8) | memory[IP])) {
                        OF = 1;
                        CF = 1;
                    }
                    DI += (opcode << 8) | memory[IP++];
                    if (!DI) ZF = 1;
                    break;
                    //sub reg16, imm16
                case 0xE9:
                    opcode = memory[IP++];
                    CL = opcode;
                    opcode = memory[IP++];
                    CH = opcode;
                    temp_16 = together(CL, CH);
                    sub16(CX, temp_16);
                    break;
                case 0xEA:
                    opcode = memory[IP++];
                    DL = opcode;
                    opcode = memory[IP++];
                    DH = opcode;
                    temp_16 = together(DL, DH);
                    sub16(DX, temp_16);
                    break;
                case 0xEB:
                    opcode = memory[IP++];
                    BL = opcode;
                    opcode = memory[IP++];
                    BH = opcode;
                    temp_16 = together(BL, BH);
                    sub16(BX, temp_16);
                    break;
                case 0xEC:
                    opcode = memory[IP++];
                    temp_16 = together(opcode, memory[IP]);
                    sub16(SP, temp_16);
                    break;
                case 0xED:
                    opcode = memory[IP++];
                    temp_16 = together(opcode, memory[IP]);
                    sub16(BP, temp_16);
                    break;
                case 0xEE:
                    opcode = memory[IP++];
                    temp_16 = together(opcode, memory[IP]);
                    sub16(SI, temp_16);
                    break;
                case 0xEF:
                    opcode = memory[IP++];
                    temp_16 = together(opcode, memory[IP]);
                    sub16(DI, temp_16);
                    break;
            }
            updateRegister8();
            break;
        case 0x80:
            //adding imm8 to 8 bit registers
            opcode = memory[IP++];
            switch (opcode) {
                // add imm8 to CL
                case 0xC1:
                    opcode = memory[IP++];
                    if (CL > 0xFF - opcode) {
                        OF = 1;
                        CF = 1;
                    }
                    CL += opcode;
                    if (!CL) ZF = 1;
                    updateRegister16();
                    break;
                case 0xC2:
                    opcode = memory[IP++];
                    if (DL > 0xFF - opcode) {
                        OF = 1;
                        CF = 1;
                    }
                    DL += opcode;
                    if (!DL) ZF = 1;
                    updateRegister16();
                    break;
                case 0xC3:
                    opcode = memory[IP++];
                    if (BL > 0xFF - opcode) {
                        OF = 1;
                        CF = 1;
                    }
                    BL += opcode;
                    if (!BL) ZF = 1;
                    updateRegister16();
                    break;
                case 0xC4:
                    opcode = memory[IP++];
                    if (AH > 0xFF - opcode) {
                        OF = 1;
                        CF = 1;
                    }
                    AH += opcode;
                    if (!AH) ZF = 1;
                    updateRegister16();
                    break;
                case 0xC5:
                    opcode = memory[IP++];
                    if (CH > 0xFF - opcode) {
                        OF = 1;
                        CF = 1;
                    }
                    CH += opcode;
                    if (!CH) ZF = 1;
                    updateRegister16();
                    break;
                case 0xC6:
                    opcode = memory[IP++];
                    if (DH > 0xFF - opcode) {
                        OF = 1;
                        CF = 1;
                    }
                    DH += opcode;
                    if (!DH) ZF = 1;
                    updateRegister16();
                    break;
                case 0xC7:
                    opcode = memory[IP++];
                    if (BH > 0xFF - opcode) {
                        OF = 1;
                        CF = 1;
                    }
                    BH += opcode;
                    if (!BH) ZF = 1;
                    updateRegister16();
                    break;
                    //sub reg 8 imm8
                case 0xE9:
                    opcode = memory[IP++];
                    sub8(CL, opcode);
                    CX = together(CL, CH);
                    break;
                case 0xEA:
                    opcode = memory[IP++];
                    sub8(DL, opcode);
                    DX = together(DL, DH);
                    break;
                case 0xEB:
                    opcode = memory[IP++];
                    sub8(BL, opcode);
                    BX = together(BL, BH);
                    break;
                case 0xEC:
                    opcode = memory[IP++];
                    sub8(AH, opcode);
                    AX = together(AL, AH);
                    break;
                case 0xED:
                    opcode = memory[IP++];
                    sub8(CH, opcode);
                    CX = together(CL, CH);
                    break;
                case 0xEE:
                    opcode = memory[IP++];
                    sub8(DH, opcode);
                    DX = together(DL, DH);
                    break;
                case 0xEF:
                    opcode = memory[IP++];
                    sub8(BH, opcode);
                    BX = together(BL, BH);
                    break;
            }
            break;
        case 0xE2:
            if (CX != 0 && ZF == 0 && CF == 0 && OF == 0) {
                signed char loop_count = memory[IP++];
                CX--;
                IP += loop_count;
            } else
                IP++;
            break;
        case 0xFE:
            opcode = memory[IP++];
            switch (opcode) {
                //increment 8 bit registers
                case 0xC0:
                    AL++;
                    AX = together(AL, AH);
                    break;
                case 0xC1:
                    CL++;
                    CX = together(CL, CH);
                    break;
                case 0xC2:
                    DL++;
                    DX = together(DL, DH);
                    break;
                case 0xC3:
                    BL++;
                    BX = together(BL, BH);
                    break;
                case 0xC4:
                    AH++;
                    AX = together(AL, AH);
                    break;
                case 0xC5:
                    CH++;
                    CX = together(CL, CH);
                    break;
                case 0xC6:
                    DH++;
                    DX = together(DL, DH);
                    break;
                case 0xC7:
                    BH++;
                    BX = together(BL, BH);
                    break;
                    //decrement 8 bit registers
                case 0xC8:
                    AL--;
                    AX = together(AL, AH);
                    break;
                case 0xC9:
                    CL--;
                    CX = together(CL, CH);
                    break;
                case 0xCA:
                    DL--;
                    DX = together(DL, DH);
                    break;
                case 0xCB:
                    BL--;
                    BX = together(BL, BH);
                    break;
                case 0xCC:
                    AH--;
                    AX = together(AL, AH);
                    break;
                case 0xCD:
                    CH--;
                    CX = together(CL, CH);
                    break;
                case 0xCE:
                    DH--;
                    DX = together(DL, DH);
                    break;
                case 0xCF:
                    BH--;
                    BX = together(BL, BH);
                    break;
            }
            break;
        case 0x83:
            opcode = memory[IP++];
            switch (opcode) {
                //add reg16, imm8
                case 0xC0:
                    opcode = memory[IP++];
                    addRegis16_8(AX, opcode);
                case 0xC1:
                    opcode = memory[IP++];
                    addRegis16_8(CX, opcode);
                    break;
                case 0xC2:
                    opcode = memory[IP++];
                    addRegis16_8(DX, opcode);
                    break;
                case 0xC3:
                    opcode = memory[IP++];
                    addRegis16_8(BX, opcode);
                    break;
                case 0xC4:
                    opcode = memory[IP++];
                    addRegis16_8(SP, opcode);
                    break;
                case 0xC5:
                    opcode = memory[IP++];
                    addRegis16_8(BP, opcode);
                    break;
                case 0xC6:
                    opcode = memory[IP++];
                    addRegis16_8(SI, opcode);
                    break;
                case 0xC7:
                    opcode = memory[IP++];
                    addRegis16_8(DI, opcode);
                    break;

                    //sub reg16, imm8
                case 0xE8:
                    opcode = memory[IP++];
                    SUB16_8(AX, opcode);
                    break;
                case 0xE9:
                    opcode = memory[IP++];
                    SUB16_8(CX, opcode);
                    break;
                case 0xEA:
                    opcode = memory[IP++];
                    SUB16_8(DX, opcode);
                    break;
                case 0xEB:
                    opcode = memory[IP++];
                    SUB16_8(BX, opcode);
                    break;
                case 0xEC:
                    opcode = memory[IP++];
                    SUB16_8(SP, opcode);
                    break;
                case 0xED:
                    opcode = memory[IP++];
                    SUB16_8(BP, opcode);
                    break;
                case 0xEE:
                    opcode = memory[IP++];
                    SUB16_8(SI, opcode);
                    break;
                case 0xEF:
                    opcode = memory[IP++];
                    SUB16_8(DI, opcode);
                    break;
            }
            updateRegister8();
            break;

            //sub reg8, reg8
        case 0x2A:
            opcode = memory[IP++];
            switch (opcode) {
                //sub AL, reg8
                case 0xC0:
                case 0xC1:
                case 0xC2:
                case 0xC3:
                case 0xC4:
                case 0xC5:
                case 0xC6:
                case 0xC7:
                    if (opcode == 0xC0)
                        sub8(AL, AL);
                    if (opcode == 0xC1)
                        sub8(AL, CL);
                    if (opcode == 0xC2)
                        sub8(AL, DL);
                    if (opcode == 0xC3)
                        sub8(AL, BL);
                    if (opcode == 0xC4)
                        sub8(AL, AH);
                    if (opcode == 0xC5)
                        sub8(AL, CH);
                    if (opcode == 0xC6)
                        sub8(AL, DH);
                    if (opcode == 0xC7)
                        sub8(AL, BH);
                    AX = together(AL, AH);
                    break;
                    //sub CL, reg8
                case 0xC8:
                case 0xC9:
                case 0xCA:
                case 0xCB:
                case 0xCC:
                case 0xCD:
                case 0xCE:
                case 0xCF:
                    if (opcode == 0xC8)
                        sub8(CL, AL);
                    if (opcode == 0xC9)
                        sub8(CL, CL);
                    if (opcode == 0xCA)
                        sub8(CL, DL);
                    if (opcode == 0xCB)
                        sub8(CL, BL);
                    if (opcode == 0xCC)
                        sub8(CL, AH);
                    if (opcode == 0xCD)
                        sub8(CL, CH);
                    if (opcode == 0xCE)
                        sub8(CL, DH);
                    if (opcode == 0xCF)
                        sub8(CL, BH);
                    CX = together(CL, CH);
                    break;
                    //sub DL, reg8
                case 0xD0:
                case 0xD1:
                case 0xD2:
                case 0xD3:
                case 0xD4:
                case 0xD5:
                case 0xD6:
                case 0xD7:
                    if (opcode == 0xD0)
                        sub8(DL, AL);
                    if (opcode == 0xD1)
                        sub8(DL, CL);
                    if (opcode == 0xD2)
                        sub8(DL, DL);
                    if (opcode == 0xD3)
                        sub8(DL, BL);
                    if (opcode == 0xD4)
                        sub8(DL, AH);
                    if (opcode == 0xD5)
                        sub8(DL, CH);
                    if (opcode == 0xD6)
                        sub8(DL, DH);
                    if (opcode == 0xD7)
                        sub8(DL, BH);
                    DX = together(DL, DH);
                    break;
                    //sub BL, reg8
                case 0xD8:
                case 0xD9:
                case 0xDA:
                case 0xDB:
                case 0xDC:
                case 0xDD:
                case 0xDE:
                case 0xDF:
                    if (opcode == 0xD8)
                        sub8(BL, AL);
                    if (opcode == 0xD9)
                        sub8(BL, CL);
                    if (opcode == 0xDA)
                        sub8(BL, DL);
                    if (opcode == 0xDB)
                        sub8(BL, BL);
                    if (opcode == 0xDC)
                        sub8(BL, AH);
                    if (opcode == 0xDD)
                        sub8(BL, CH);
                    if (opcode == 0xDE)
                        sub8(BL, DH);
                    if (opcode == 0xDF)
                        sub8(BL, BH);
                    BX = together(BL, BH);
                    break;
                    //sub AH, reg8
                case 0xE0:
                case 0xE1:
                case 0xE2:
                case 0xE3:
                case 0xE4:
                case 0xE5:
                case 0xE6:
                case 0xE7:
                    if (opcode == 0xE0)
                        sub8(AH, AL);
                    if (opcode == 0xE1)
                        sub8(AH, CL);
                    if (opcode == 0xE2)
                        sub8(AH, DL);
                    if (opcode == 0xE3)
                        sub8(AH, BL);
                    if (opcode == 0xE4)
                        sub8(AH, AH);
                    if (opcode == 0xE5)
                        sub8(AH, CH);
                    if (opcode == 0xE6)
                        sub8(AH, DH);
                    if (opcode == 0xE7)
                        sub8(AH, BH);
                    AX = together(AL, AH);
                    break;
                    //sub CH, reg8
                case 0xE8:
                case 0xE9:
                case 0xEA:
                case 0xEB:
                case 0xEC:
                case 0xED:
                case 0xEE:
                case 0xEF:
                    if (opcode == 0xE8)
                        sub8(CH, AL);
                    if (opcode == 0xE9)
                        sub8(CH, CL);
                    if (opcode == 0xEA)
                        sub8(CH, DL);
                    if (opcode == 0xEB)
                        sub8(CH, BL);
                    if (opcode == 0xEC)
                        sub8(CH, AH);
                    if (opcode == 0xED)
                        sub8(CH, CH);
                    if (opcode == 0xEE)
                        sub8(CH, DH);
                    if (opcode == 0xEF)
                        sub8(CH, BH);
                    CX = together(CL, CH);
                    break;
                    //sub DH, reg8
                case 0xF0:
                case 0xF1:
                case 0xF2:
                case 0xF3:
                case 0xF4:
                case 0xF5:
                case 0xF6:
                case 0xF7:
                    if (opcode == 0xF0)
                        sub8(DH, AL);
                    if (opcode == 0xF1)
                        sub8(DH, CL);
                    if (opcode == 0xF2)
                        sub8(DH, DL);
                    if (opcode == 0xF3)
                        sub8(DH, BL);
                    if (opcode == 0xF4)
                        sub8(DH, AH);
                    if (opcode == 0xF5)
                        sub8(DH, CH);
                    if (opcode == 0xF6)
                        sub8(DH, DH);
                    if (opcode == 0xF7)
                        sub8(DH, BH);
                    DX = together(DL, DH);
                    break;
                    //sub BH, reg8
                case 0xF8:
                case 0xF9:
                case 0xFA:
                case 0xFB:
                case 0xFC:
                case 0xFD:
                case 0xFE:
                case 0xFF:
                    if (opcode == 0xF8)
                        sub8(BH, AL);
                    if (opcode == 0xF9)
                        sub8(BH, CL);
                    if (opcode == 0xFA)
                        sub8(BH, DL);
                    if (opcode == 0xFB)
                        sub8(BH, BL);
                    if (opcode == 0xFC)
                        sub8(BH, AH);
                    if (opcode == 0xFD)
                        sub8(BH, CH);
                    if (opcode == 0xFE)
                        sub8(BH, DH);
                    if (opcode == 0xFF)
                        sub8(BH, BH);
                    BX = together(BL, BH);
                    break;
                case 0x40:
                    AX++;
                    break;
                case 0x41:
                    CX++;
                    break;
                case 0x42:
                    DX++;
                    break;
                case 0x43:
                    BX++;
                    break;
                case 0x44:
                    SP++;
                    break;
                case 0x45:
                    BP++;
                    break;
                case 0x46:
                    SI++;
                    break;
                case 0x47:
                    DI++;
                    break;
                    //decrement 16 bit registers
                case 0x48:
                    AX--;
                    break;
                case 0x49:
                    CX--;
                    break;
                case 0x4A:
                    DX--;
                    break;
                case 0x4B:
                    BX--;
                    break;
                case 0x4C:
                    SP--;
                    break;
                case 0x4D:
                    BP--;
                    break;
                case 0x4E:
                    SI--;
                    break;
                case 0x4F:
                    DI--;
                    break;
            }
            break;
        case 0x8A:
            opcode = memory[IP++];
            switch (opcode)
            {
                case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5: case 0xC6: case 0xC7:
                    if (opcode == 0xC1)
                        AL = CL;
                    if (opcode == 0xC2)
                        AL = DL;
                    if (opcode == 0xC3)
                        AL = BL;
                    if (opcode == 0xC4)
                        AL = AH;
                    if (opcode == 0xC5)
                        AL = CH;
                    if (opcode == 0xC6)
                        AL = DH;
                    if (opcode == 0xC7)
                        AL = BH;
                    AX = together(AL, AH);
                    break;

                    //mov CL, reg8
                case 0xC8: case 0xCA: case 0xCB: case 0xCC: case 0xCD: case 0xCE: case 0xCF:
                    if (opcode == 0xC8)
                        CL = AL;
                    if (opcode == 0xCA)
                        CL = DL;
                    if (opcode == 0xCB)
                        CL = BL;
                    if (opcode == 0xCC)
                        CL = AH;
                    if (opcode == 0xCD)
                        CL = CH;
                    if (opcode == 0xCE)
                        CL = DH;
                    if (opcode == 0xCF)
                        CL = BH;
                    CX = together(CL, CH);
                    break;

                    //mov DL, reg8
                case 0xD0: case 0xD1: case 0xD3: case 0xD4: case 0xD5: case 0xD6: case 0xD7:
                    if (opcode == 0xD0)
                        DL = AL;
                    if (opcode == 0xD1)
                        DL = CL;
                    if (opcode == 0xD3)
                        DL = BL;
                    if (opcode == 0xD4)
                        DL = AH;
                    if (opcode == 0xD5)
                        DL = CH;
                    if (opcode == 0xD6)
                        DL = DH;
                    if (opcode == 0xD7)
                        DL = BH;
                    DX = together(DL, DH);
                    break;

                    //mov BL, reg8
                case 0xD8: case 0xD9: case 0xDA: case 0xDC: case 0xDD: case 0xDE: case 0xDF:
                    if (opcode == 0xD8)
                        BL = AL;
                    if (opcode == 0xD9)
                        BL = CL;
                    if (opcode == 0xDA)
                        BL = DL;
                    if (opcode == 0xDC)
                        BL = AH;
                    if (opcode == 0xDD)
                        BL = CH;
                    if (opcode == 0xDE)
                        BL = DH;
                    if (opcode == 0xDF)
                        BL = BH;
                    BX = together(BL, BH);
                    break;

                    //mov AH, reg8
                case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE5: case 0xE6: case 0xE7:
                    if (opcode == 0xE0)
                        AH = AL;
                    if (opcode == 0xE1)
                        AH = CL;
                    if (opcode == 0xE2)
                        AH = DL;
                    if (opcode == 0xE3)
                        AH = BL;
                    if (opcode == 0xE5)
                        AH = CH;
                    if (opcode == 0xE6)
                        AH = DH;
                    if (opcode == 0xE7)
                        AH = BH;
                    AX = together(AL, AH);
                    break;

                    //mov CH, reg8
                case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xEE: case 0xEF:
                    if (opcode == 0xE8)
                        CH = AL;
                    if (opcode == 0xE9)
                        CH = CL;
                    if (opcode == 0xEA)
                        CH = DL;
                    if (opcode == 0xEB)
                        CH = BL;
                    if (opcode == 0xEC)
                        CH = AH;
                    if (opcode == 0xEE)
                        CH = DH;
                    if (opcode == 0xEF)
                        CH = BH;
                    CX = together(CL, CH);
                    break;

                    //mov DH, reg8
                case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF7:
                    if (opcode == 0xF0)
                        DH = AL;
                    if (opcode == 0xF1)
                        DH = CL;
                    if (opcode == 0xF2)
                        DH = DL;
                    if (opcode == 0xF3)
                        DH = BL;
                    if (opcode == 0xF4)
                        DH = AH;
                    if (opcode == 0xF5)
                        DH = CH;
                    if (opcode == 0xF7)
                        DH = BH;
                    DX = together(DL, DH);
                    break;

                    //mov BH, reg8
                case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFE:
                    if (opcode == 0xF8)
                        BH = AL;
                    if (opcode == 0xF9)
                        BH = CL;
                    if (opcode == 0xFA)
                        BH = DL;
                    if (opcode == 0xFB)
                        BH = BL;
                    if (opcode == 0xFC)
                        BH = AH;
                    if (opcode == 0xFD)
                        BH = CH;
                    if (opcode == 0xFE)
                        BH = DH;
                    BX = together(BL, BH);
                    break;
            }
            updateRegister16();
            break;
        case 0x8B:
            opcode = memory[IP++];
            switch (opcode){
                //mov AX, reg16
                case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5: case 0xC6: case 0xC7:
                    if (opcode == 0xC1)
                        AX = CX;
                    if (opcode == 0xC2)
                        AX = DX;
                    if (opcode == 0xC3)
                        AX = BX;
                    if (opcode == 0xC4)
                        AX = SP;
                    if (opcode == 0xC5)
                        AX = BP;
                    if (opcode == 0xC6)
                        AX = SI;
                    if (opcode == 0xC7)
                        AX = DI;
                    break;

                    //mov CX, reg16
                case 0xC8: case 0xCA: case 0xCB: case 0xCC: case 0xCD: case 0xCE: case 0xCF:
                    if (opcode == 0xC8)
                        CX = AX;
                    if (opcode == 0xCA)
                        CX = DX;
                    if (opcode == 0xCB)
                        CX = BX;
                    if (opcode == 0xCC)
                        CX = SP;
                    if (opcode == 0xCD)
                        CX = BP;
                    if (opcode == 0xCE)
                        CX = SI;
                    if (opcode == 0xCF)
                        CX = DI;
                    break;

                    //mov DX, reg16
                case 0xD0: case 0xD1: case 0xD3: case 0xD4: case 0xD5: case 0xD6: case 0xD7:
                    if (opcode == 0xD0)
                        DX = AX;
                    if (opcode == 0xD1)
                        DX = CX;
                    if (opcode == 0xD3)
                        DX = BX;
                    if (opcode == 0xD4)
                        DX = SP;
                    if (opcode == 0xD5)
                        DX = BP;
                    if (opcode == 0xD6)
                        DX = SI;
                    if (opcode == 0xD7)
                        DX = DI;
                    break;

                    //mov BX, reg16
                case 0xD8: case 0xD9: case 0xDA: case 0xDC: case 0xDD: case 0xDE: case 0xDF:
                    if (opcode == 0xD8)
                        BX = AX;
                    if (opcode == 0xD9)
                        BX = CX;
                    if (opcode == 0xDA)
                        BX = DX;
                    if (opcode == 0xDC)
                        BX = SP;
                    if (opcode == 0xDD)
                        BX = BP;
                    if (opcode == 0xDE)
                        BX = SI;
                    if (opcode == 0xDF)
                        BX = DI;
                    break;

                    //mov SP, reg16
                case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE5: case 0xE6: case 0xE7:
                    if (opcode == 0xE0)
                        SP = AX;
                    if (opcode == 0xE1)
                        SP = CX;
                    if (opcode == 0xE2)
                        SP = DX;
                    if (opcode == 0xE3)
                        SP = BX;
                    if (opcode == 0xE5)
                        SP = BP;
                    if (opcode == 0xE6)
                        SP = SI;
                    if (opcode == 0xE7)
                        SP = DI;
                    break;

                    //mov BP, reg16
                case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xEE: case 0xEF:
                    if (opcode == 0xE8)
                        BP = AX;
                    if (opcode == 0xE9)
                        BP = CX;
                    if (opcode == 0xEA)
                        BP = DX;
                    if (opcode == 0xEB)
                        BP = BX;
                    if (opcode == 0xEC)
                        BP = SP;
                    if (opcode == 0xEE)
                        BP = DH;
                    if (opcode == 0xEF)
                        BP = BH;
                    break;

                    //mov SI, reg16
                case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF7:
                    if (opcode == 0xF0)
                        SI = AX;
                    if (opcode == 0xF1)
                        SI = CX;
                    if (opcode == 0xF2)
                        SI = DX;
                    if (opcode == 0xF3)
                        SI = BX;
                    if (opcode == 0xF4)
                        SI = SP;
                    if (opcode == 0xF5)
                        SI = BP;
                    if (opcode == 0xF7)
                        SI = DI;
                    break;

                    //mov DI, reg16
                case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFE:
                    if (opcode == 0xF8)
                        DI = AX;
                    if (opcode == 0xF9)
                        DI = CX;
                    if (opcode == 0xFA)
                        DI = DX;
                    if (opcode == 0xFB)
                        DI = BX;
                    if (opcode == 0xFC)
                        DI = SP;
                    if (opcode == 0xFD)
                        DI = BP;
                    if (opcode == 0xFE)
                        DI = SI;
                    break;
            }
            updateRegister8();
            break;
            }

    }

    int main() {
        int stack[1000000];
        string sample_file;
        cout << "Enter File Name: \t";
        cin >> sample_file;
        const char *file = sample_file.c_str();
        FILE *run_file = fopen(file, "rb");
        if (!run_file) {
            cerr << "ERROR FILE DOES NOT EXIST";
        } else {
            fread(memory, 1, largest_mem, run_file);
            fclose(run_file);
        }

        while (1) {
            opcode = memory[IP++];
            switch (opcode) {
                case 0xCD:
                    opcode = memory[IP++];
                    printf("Interrupt Call: %d \n", opcode);
                    cout << endl;
                    if (opcode == 0x20) {
                        cout << "OUTPUT:" << endl;
                        for (int i = 0; i < printAsm.size() - 1; ++i) {
                            cout << printAsm[i];
                        }
                        cout << endl;
                        return 0;
                    }
                    if (opcode == 0x21) {
                        AL = DL;
                        AX = AH << 8 | AL;
                        int x = DX;
                        char b = x;
                        printAsm.push_back(b);
                    }
                    break;
                case 0x90:
                    break;
                default:
                    run(opcode);
                    break;
            }
            printRegisters();
        }
        return 0;
    }
