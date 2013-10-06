#include <QCoreApplication>
#include <string.h>
#include "gost.h"


uint8_t Gost_Table[_GOST_TABLE_SIZE] = {
    0x04, 0x02, 0x0F, 0x05, 0x09, 0x01, 0x00, 0x08, 0x0E, 0x03, 0x0B, 0x0C, 0x0D, 0x07, 0x0A, 0x06,
    0x0C, 0x09, 0x0F, 0x0E, 0x08, 0x01, 0x03, 0x0A, 0x02, 0x07, 0x04, 0x0D, 0x06, 0x00, 0x0B, 0x05,
    0x0D, 0x08, 0x0E, 0x0C, 0x07, 0x03, 0x09, 0x0A, 0x01, 0x05, 0x02, 0x04, 0x06, 0x0F, 0x00, 0x0B,
    0x0E, 0x09, 0x0B, 0x02, 0x05, 0x0F, 0x07, 0x01, 0x00, 0x0D, 0x0C, 0x06, 0x0A, 0x04, 0x03, 0x08,
    0x03, 0x0E, 0x05, 0x09, 0x06, 0x08, 0x00, 0x0D, 0x0A, 0x0B, 0x07, 0x0C, 0x02, 0x01, 0x0F, 0x04,
    0x08, 0x0F, 0x06, 0x0B, 0x01, 0x09, 0x0C, 0x05, 0x0D, 0x03, 0x07, 0x0A, 0x00, 0x0E, 0x02, 0x04,
    0x09, 0x0B, 0x0C, 0x00, 0x03, 0x06, 0x07, 0x05, 0x04, 0x08, 0x0E, 0x0F, 0x01, 0x0A, 0x02, 0x0D,
    0x0C, 0x06, 0x05, 0x02, 0x0B, 0x00, 0x09, 0x0D, 0x03, 0x0E, 0x07, 0x0A, 0x0F, 0x04, 0x01, 0x08
};
unsigned char GOST_Key_d[32] = {
    0x04, 0x75, 0xF6, 0xE0, 0x50, 0x38, 0xFB, 0xFA, 0xD2, 0xC7, 0xC3, 0x90, 0xED, 0xB3, 0xCA, 0x3D,
    0x15, 0x47, 0x12, 0x42, 0x91, 0xAE, 0x1E, 0x8A, 0x2F, 0x79, 0xCD, 0x9E, 0xD2, 0xBC, 0xEF, 0xBD
};
unsigned char Data_O[16] = {
    0x33, 0x33, 0x33, 0x33, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x33, 0x33, 0x33, 0x33
};
//Data from real etalon cryptor:
//imitta
//unsigned char Imitta_e[_GOST_Imitta_Size] = {
//    0x82, 0x96, 0x5E, 0x19, 0x5A, 0xC9, 0x27, 0x2C
//};
//rotate bytes acording to architecture of CPU
unsigned char Imitta_Et[_GOST_Imitta_Size] ={
    0x19, 0x5E, 0x96, 0x82,  0x2C, 0x27,  0xC9, 0x5A
};

//Simple replacement
//unsigned char Data_C_S_Et[16] = {
//    0x56, 0xF5, 0xD7, 0x7D, 0x40, 0x1E, 0xBE, 0xD9, 0x73, 0xFE, 0x01, 0x18, 0x4E, 0x79, 0x05, 0x03
//};
//rotate bytes acording to architecture of CPU
unsigned char Data_C_S_Et[16] = {
    0x7D, 0xD7, 0xF5, 0x56, 0xD9, 0xBE, 0x1E, 0x40, 0x18, 0x01, 0xFE, 0x73, 0x03,  0x05, 0x79, 0x4E
};
uint32_t GOST_Key[8];
uint8_t  Imitta[_GOST_Imitta_Size];
uint8_t  Data_E[sizeof(Data_O)];
int main(int argc, char *argv[])
{
   // QCoreApplication a(argc, argv);
//Imitta test
    memset(Imitta,_GOST_Def_Byte,_GOST_Imitta_Size);
    GOST_Imitta(Data_O, Imitta, (uint32_t)sizeof(Data_O),Gost_Table,GOST_Key_d);
    if (memcmp(Imitta,Imitta_Et,_GOST_Imitta_Size))
    {
        printf("Imitta test failed\r\n");
        return -1;
    }
    printf("Imitta test passed\r\n");
    memcpy(Data_E,Data_O,sizeof(Data_O));
    GOST_Encrypt_SR(Data_E,sizeof(Data_E),_GOST_Crypt_SR_Encrypt,Gost_Table,GOST_Key_d);
    if (memcmp(Data_C_S_Et,Data_E,sizeof(Data_E)))
    {
        printf("Simple replacement encryption test failed\r\n");
        return -1;
    }
    printf("Simple replacement encryption test passed\r\n");
    GOST_Encrypt_SR(Data_E,sizeof(Data_E),_GOST_Crypt_SR_Decrypt,Gost_Table,GOST_Key_d);
    if (memcmp(Data_O,Data_E,sizeof(Data_E)))
    {
        printf("Simple replacement decryption test failed\r\n");
        return -1;
    }
    printf("Simple decryption test passed\r\n");

    return 0;
}
