
#include <stdlib.h>
#include <time.h>


/* 64bit binary, 8 bytes  = 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF */
#define D_BINSTR	 0	

/* 16 asc string = "FFFFFFFFFFFFFFFF" */
#define D_HEXSTR	 1

/* 64 char string = {0x01, 0x01, 0x01, 0x01, 0x01...} * 64 */
#define D_CHARSTR	 2

int des(unsigned char *source,unsigned char * dest,unsigned char * inkey, int flg);
void vTestBit(void *vpValue, int viByteLen, int vMode);
/* ============================================================
Description: DES by fox 20100819
input
	vpData		:	64bit or 16 Ascii HEX
	vpEncData	:	64bit or 16 Ascii HEX
	vpKey			:	64bit or 16 Ascii HEX
	viDesLen	: 8 or 16. 8 = input 64 bit string , 16 = input 16 Ascii HEX string
	viEncMode : 0 or 1. 0 = encypt, 1 = dencypt
============================================================ */
int lBasicDes(unsigned char *vpData, unsigned char *vpEncData, unsigned char *vpKey, int viDesLen, int viEncMode);
void vGenRandomData(void *vpStr, void *vpStrAsc, int viByteLen);


int main(int argc, char ** argv)
{
	unsigned int liX;
	unsigned int liData;
	unsigned long long llData2;
	unsigned char lsData3[16];
	
	unsigned char lsData[16], lsHexStr[32 + 1];;
	unsigned char lsKey[16];
	unsigned char lsEncData[16];

  /*	
	memset(lsHexStr, 0, sizeof(lsHexStr));
	vGenRandomData(lsData, lsHexStr, 8);
	printf("HexString is %s \n", lsHexStr);
	vTestBit(&lsData, 8, 1);
	return ;
	*/
	
	/*
	printf("Len of short = %d, int = %d,  long = %d, longlong = %d, time_t = %d \n", 
		sizeof(short),
		sizeof(int),
		sizeof(long),
		sizeof(long long),
		sizeof(time_t)
		);
	*/
	
	printf("-- Input : \n");
	memcpy(lsKey, "636F6D7075746572", 16);
	memcpy(lsData, "6C6561726E696E67", 16);
	printf("-- Key =  636F6D7075746572 \n");
	printf("-- Data = 6C6561726E696E67 \n");

	liX = lBasicDes(lsData, lsEncData, lsKey, 16, 0);
	//liX = des(lsData, lsEncData, lsKey, 0);
	printf("%s \n", lsEncData);
	//vTestBit(lsEncData, 8, 2);
	
	/*
	liData = 7;
	liX = 0;
	while(liX++ < sizeof(liData)*8){
		vTestBit(&liData, sizeof(liData), 0);
		liData <<= 1;
	}
	printf("\n");
	
	llData2 = 7;
	liX = 0;
	while(liX++ < sizeof(llData2)*8){
		vTestBit(&llData2, sizeof(llData2), 0);
		llData2 <<= 1;
	}
	printf("\n");
	
	memset(lsData3, 0, sizeof(lsData3));
	lsData3[15] = 7;
	liX = 0;
	while(liX++ < sizeof(lsData3)*1){
		vTestBit(&lsData3, sizeof(lsData3), 0);
		lsData3[15] <<= 1;
	}
	printf("\n"); 
	*/
	/* printf("llData2 = [%d]\n", llData2); */
		
}
void vGenRandomData(void *vpStr, void *vpStrAsc, int viByteLen)
{
	unsigned char lcLeft, lcRight;
	static char lsaRndTbl[16] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F 
	};
	static char lsaRndTblAsc[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	time_t t; 

	srand((unsigned) time(&t)); 
	while(--viByteLen >= 0){
		lcLeft = lsaRndTbl[rand() % 16];
		lcRight = lsaRndTbl[rand() % 16];
		((unsigned char *)vpStr)[viByteLen] = 
			(lsaRndTbl[lcLeft] << 4) | lsaRndTbl[lcRight];

		((unsigned char *)vpStrAsc)[2 * viByteLen] = lsaRndTblAsc[lcLeft];
		((unsigned char *)vpStrAsc)[2 * viByteLen + 1] = lsaRndTblAsc[lcRight];
	}
}

/* ============================================================
Description: DES by fox 20100819
input
	vpValue		:	bit value
	viByteLen	:	byte len
	vMode			:	0 = show hex , 1 = show binary, 2 =  show all
============================================================ */
void vTestBit(void *vpValue, int viByteLen, int vMode)
{
	int liX, liY;
	unsigned char *lcp;

	lcp = (unsigned char *)vpValue;
	if(vMode == 1 || vMode  == 2){
		liX = 0, liY = 0;
		printf("Len = [%d] \nHex = ", viByteLen);
		while(liX++ < viByteLen)printf("%02X ", lcp[liX - 1]);
		printf("\n");
	}

	if(vMode == 0 || vMode  == 2){
		liX = 0, liY = 0;
		printf("Bin = ");
		while(liX++ < viByteLen){
			while(liY++ < 8)
				((1 << (8 - liY)) & lcp[liX - 1]) ? printf("1"):	printf("0");
			liY = 0;
			printf(" ");
		}
		printf("\n");
	}
}

void vShowByteAry(void *vpValue, int viByteLen)
{
	int liX, liY;
	unsigned char *lcp;

	liX = -1;
	lcp = (unsigned char *)vpValue;
	while(liX++ < viByteLen){
	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	  printf("%d", lcp[liX]);
	}
  printf("\n");
}

/* ============================================================
Description: Transform data type in  D_BINSTR, D_HEXSTR, D_CHARSTR
para:
	(i)vpInValue	:	string
	(o)vpOutValue	:	string
	(i)viInType		:	D_BINSTR, D_HEXSTR, D_CHARSTR
	(i)viOutType	: D_BINSTR, D_HEXSTR, D_CHARSTR
	(i)viInByteLen: vpInValue byte len 
	(o)Ret =      : <=0: error,   > 0 : Byte len of vpOutValue
exam:
	unsigned char lsIn[64];
	unsigned char lsOut[64];
	memcpy(&lsIn[0], "FFFFFFFFFFFFFFFF", 16;
	liRet = lTD(lsIn, lsOut, 16, D_HEXSTR, D_BINSTR);
	liRet = 8
============================================================ */
int lTD(void *vpInValue, void *vpOutValue, int viByteLen, int viInType, int viOutType)
{

	int liX, liY;
	unsigned char *lcp, *lcp1;
	
	
	lcp = (unsigned char *)vpInValue;
	lcp1 = (unsigned char *)vpOutValue;
	switch(viInType){

		case (D_HEXSTR):
			break;

		case (D_CHARSTR):
			break;

		case (D_BINSTR):

		default:

			break;
	}
	
	/* 64bit binary, 8 bytes  = 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF */
	#define D_BINSTR	 0	
	
	/* 16 asc string = "FFFFFFFFFFFFFFFF" */
	#define D_HEXSTR	 1
	
	/* 64 char string = {0x01, 0x01, 0x01, 0x01, 0x01...} * 64 */
	#define D_CHARSTR	 2


	liX = -1;
	lcp = (unsigned char *)vpInValue;
	while(liX++ < viByteLen){
	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	  printf("%d", lcp[liX]);
	}
  printf("\n");
}


/* ============================================================
Description: DES by fox 20100819
para:
	(i)vpData			:	64bit or 16 Ascii HEX
	(o)vpEncData	:	64bit or 16 Ascii HEX
	(i)vpKey			:	64bit or 16 Ascii HEX
	(i)viDesLen		: 8 or 16. 8 = input 64 bit string , 16 = input 16 Ascii HEX string
	(i)viEncMode 	: 0 or 1. 0 = encypt, 1 = dencypt
============================================================ */
int lBasicDes(unsigned char *vpData, unsigned char *vpEncData, unsigned char *vpKey, int viDesLen, int viEncMode)
{

	/* Key Permute 1x -> 16x */
	/* Permuted Choice 1 (PC-1) （8,16,24,32,40,48,56,64）已经被去掉 */
	static unsigned char kp1[56] = {
		57, 49, 41, 33, 25, 17,  9,  1,  58, 50, 42, 34, 26, 18, 10, 2,   59, 51, 43, 35, 27, 19, 11,  3,  60, 52, 44, 36, 63, 55, 47, 39, 
		31, 23, 15,  7, 62, 54, 46, 38,  30, 22, 14,  6, 61, 53, 45, 37,  29, 21, 13,  5, 28, 20, 12,  4
	};
	
	/* Key left shift step */
	static unsigned char kp2[16] = {
		1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
	};

	/* Key Compress 56 bits to 48 bits */
	/* Permuted Choice 2 (PC-2) compress 56bit key to 48bit */
	static unsigned char kp3_org[48] = {
		14, 17, 11, 24,  1,  5,  3, 28,  15,  6, 21, 10, 23, 19, 12,  4,  26, 8,  16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55, 30, 40,  51, 45, 33, 48, 44, 49, 39, 56,  34, 53, 46, 42, 50, 36, 29, 32
	};
	static unsigned char kp3[48][2] = {
	  1,5,  2,0,  1,2,  2,7,  0,0,  0,4,  0,2,  3,3,    1,6,  0,5,  2,4,  1,1,  2,6,  2,2,  1,3,  0,3,    
  	3,1,  0,7,  1,7,  0,6,  3,2,  2,3,  1,4,  0,1,    5,0,  6,3,  3,6,  4,4,  5,6,  6,6,  3,5,  4,7,    
  	6,2,  5,4,  4,0,  5,7,  5,3,  6,0,  4,6,  6,7,    4,1,  6,4,  5,5,  5,1,  6,1,  4,3,  3,4,  3,7
  };

	/* kp4[KeyPermuteIndex][SrcKeyByteIndex][SrcKeyInByte&Bit] */
	/* 利用输入64bit 密钥和 56bits，48bits 分散算法得到所有 16个子密钥中每个bit对应原始64bits密钥的字节和bit位 
			即，可以通过kp4，直接把原始输入的64bits密钥变换成所有16个子密钥	*/
	static unsigned char kp4[16][48][2] = {
  	/* K48 = 00 */                                                                                      
  	1,1,  6,2,  4,1,  7,3,  6,0,  2,0,  4,0,  7,0,    0,1,  1,0,  2,2,  5,1,  0,2,  4,2,  3,1,  3,0,    
  	5,3,  7,1,  7,2,  0,0,  4,3,  3,2,  2,1,  5,0,    2,5,  3,3,  4,6,  6,5,  4,4,  0,3,  5,6,  3,5,    
  	0,4,  6,4,  2,6,  3,4,  7,4,  2,4,  4,5,  7,6,    1,6,  2,3,  5,4,  1,5,  1,4,  7,5,  6,6,  3,6,    
  	                                                                                                    
  	/* K48 = 01 */                                                                                      
  	0,1,  5,2,  3,1,  6,3,  5,0,  1,0,  3,0,  6,0,    7,2,  0,0,  1,2,  4,1,  7,3,  3,2,  2,1,  2,0,    
  	4,3,  6,1,  6,2,  7,1,  7,0,  2,2,  1,1,  4,0,    1,5,  2,3,  3,6,  5,5,  3,4,  7,6,  4,6,  2,5,    
  	3,3,  5,4,  1,6,  2,4,  6,4,  1,4,  3,5,  6,6,    0,6,  1,3,  4,4,  0,5,  0,4,  6,5,  5,6,  2,6,    
  	                                                                                                    
  	/* K48 = 02 */                                                                                      
  	6,2,  3,2,  1,1,  4,3,  3,0,  7,1,  1,0,  4,0,    5,2,  6,1,  7,3,  2,1,  5,3,  1,2,  0,1,  0,0,    
  	6,0,  4,1,  4,2,  5,1,  5,0,  0,2,  7,2,  2,0,    7,4,  0,3,  1,6,  3,5,  1,4,  5,6,  2,6,  0,5,    
  	1,3,  3,4,  7,5,  0,4,  4,4,  3,3,  1,5,  4,6,    6,5,  7,6,  2,4,  6,4,  2,3,  4,5,  3,6,  0,6,    
  	                                                                                                    
  	/* K48 = 03 */                                                                                      
  	4,2,  1,2,  7,2,  6,0,  1,0,  5,1,  7,1,  2,0,    3,2,  4,1,  5,3,  0,1,  7,0,  7,3,  6,2,  6,1,    
  	4,0,  2,1,  2,2,  3,1,  3,0,  6,3,  5,2,  0,0,    5,4,  6,6,  7,5,  1,5,  3,3,  3,6,  0,6,  6,4,    
  	7,6,  1,4,  5,5,  2,3,  2,4,  1,3,  7,4,  2,6,    4,5,  5,6,  0,4,  4,4,  0,3,  2,5,  1,6,  6,5,    
  	                                                                                                    
  	/* K48 = 04 */                                                                                      
  	2,2,  7,3,  5,2,  4,0,  7,1,  3,1,  5,1,  0,0,    1,2,  2,1,  7,0,  6,2,  5,0,  5,3,  4,2,  4,1,    
  	2,0,  0,1,  0,2,  1,1,  1,0,  4,3,  3,2,  6,1,    3,4,  4,6,  5,5,  7,4,  1,3,  1,6,  6,5,  4,4,    
  	5,6,  3,3,  3,5,  0,3,  0,4,  7,6,  5,4,  0,6,    2,5,  3,6,  2,3,  2,4,  6,6,  0,5,  7,5,  4,5,    
  	                                                                                                    
  	/* K48 = 05 */                                                                                      
  	0,2,  5,3,  3,2,  2,0,  5,1,  1,1,  3,1,  6,1,    7,3,  0,1,  5,0,  4,2,  3,0,  7,0,  2,2,  2,1,    
  	0,0,  6,2,  6,3,  7,2,  7,1,  6,0,  1,2,  4,1,    1,4,  2,6,  3,5,  5,4,  7,6,  7,5,  4,5,  2,4,    
  	3,6,  1,3,  1,5,  6,6,  2,3,  5,6,  3,4,  6,5,    0,5,  1,6,  0,3,  0,4,  4,6,  6,4,  5,5,  2,5,    
  	                                                                                                    
  	/* K48 = 06 */                                                                                      
  	6,3,  7,0,  1,2,  0,0,  3,1,  7,2,  1,1,  4,1,    5,3,  6,2,  3,0,  2,2,  1,0,  5,0,  0,2,  0,1,    
  	6,1,  4,2,  4,3,  5,2,  5,1,  4,0,  7,3,  2,1,    3,3,  0,6,  1,5,  3,4,  5,6,  5,5,  2,5,  0,4,    
  	1,6,  7,6,  7,4,  4,6,  0,3,  3,6,  1,4,  4,5,    6,4,  7,5,  6,6,  2,3,  2,6,  4,4,  3,5,  0,5,    
  	                                                                                                    
  	/* K48 = 07 */                                                                                      
  	4,3,  5,0,  7,3,  6,1,  1,1,  5,2,  7,2,  2,1,    7,0,  4,2,  1,0,  0,2,  7,1,  3,0,  6,3,  6,2,    
  	4,1,  2,2,  6,0,  3,2,  3,1,  2,0,  5,3,  0,1,    1,3,  6,5,  7,4,  1,4,  3,6,  3,5,  0,5,  2,3,    
  	7,5,  5,6,  5,4,  2,6,  6,6,  1,6,  3,3,  2,5,    4,4,  5,5,  4,6,  0,3,  0,6,  2,4,  1,5,  6,4,    
  	                                                                                                    
  	/* K48 = 08 */                                                                                      
  	7,0,  4,0,  6,3,  5,1,  0,1,  4,2,  6,2,  1,1,    6,0,  3,2,  0,0,  7,3,  6,1,  2,0,  5,3,  5,2,    
  	3,1,  1,2,  5,0,  2,2,  2,1,  1,0,  4,3,  7,2,    0,3,  5,5,  6,4,  0,4,  2,6,  2,5,  7,4,  1,3,    
  	6,5,  4,6,  4,4,  1,6,  5,6,  0,6,  2,3,  1,5,    3,4,  4,5,  3,6,  7,6,  7,5,  1,4,  0,5,  5,4,    
  	                                                                                                    
  	/* K48 = 09 */                                                                                      
  	5,0,  2,0,  4,3,  3,1,  6,2,  2,2,  4,2,  7,2,    4,0,  1,2,  6,1,  5,3,  4,1,  0,0,  7,0,  3,2,    
  	1,1,  7,3,  3,0,  0,2,  0,1,  7,1,  6,0,  5,2,    6,6,  3,5,  4,4,  2,3,  0,6,  0,5,  5,4,  7,6,    
  	4,5,  2,6,  2,4,  7,5,  3,6,  6,5,  0,3,  7,4,    1,4,  2,5,  1,6,  5,6,  5,5,  3,3,  6,4,  3,4,    
  	                                                                                                    
  	/* K48 = 10 */                                                                                      
  	3,0,  0,0,  6,0,  1,1,  4,2,  0,2,  2,2,  5,2,    2,0,  7,3,  4,1,  7,0,  2,1,  6,1,  5,0,  1,2,    
  	7,2,  5,3,  1,0,  6,3,  6,2,  5,1,  4,0,  3,2,    4,6,  1,5,  2,4,  0,3,  6,5,  6,4,  3,4,  5,6,    
  	2,5,  0,6,  0,4,  5,5,  1,6,  4,5,  6,6,  5,4,    3,3,  0,5,  7,5,  3,6,  3,5,  1,3,  4,4,  1,4,    
  	                                                                                                    
  	/* K48 = 11 */                                                                                      
  	1,0,  6,1,  4,0,  7,2,  2,2,  6,3,  0,2,  3,2,    0,0,  5,3,  2,1,  5,0,  0,1,  4,1,  3,0,  7,3,    
  	5,2,  7,0,  7,1,  4,3,  4,2,  3,1,  2,0,  1,2,    2,6,  7,4,  0,4,  6,6,  4,5,  4,4,  1,4,  3,6,    
  	0,5,  6,5,  2,3,  3,5,  7,5,  2,5,  4,6,  3,4,    1,3,  6,4,  5,5,  1,6,  1,5,  7,6,  2,4,  3,3,    
  	                                                                                                    
  	/* K48 = 12 */                                                                                      
  	7,1,  4,1,  2,0,  5,2,  0,2,  4,3,  6,3,  1,2,    6,1,  7,0,  0,1,  3,0,  6,2,  2,1,  1,0,  5,3,    
  	3,2,  5,0,  5,1,  6,0,  2,2,  1,1,  0,0,  7,3,    0,6,  5,4,  2,3,  4,6,  2,5,  2,4,  3,3,  1,6,    
  	6,4,  4,5,  0,3,  1,5,  5,5,  0,5,  2,6,  1,4,    7,6,  4,4,  3,5,  7,5,  7,4,  5,6,  0,4,  1,3,    
  	                                                                                                    
  	/* K48 = 13 */                                                                                      
  	5,1,  2,1,  0,0,  3,2,  6,3,  6,0,  4,3,  7,3,    4,1,  5,0,  6,2,  1,0,  4,2,  0,1,  7,1,  7,0,    
  	1,2,  3,0,  3,1,  4,0,  0,2,  7,2,  6,1,  5,3,    6,5,  3,4,  0,3,  2,6,  0,5,  0,4,  1,3,  7,5,    
  	4,4,  2,5,  6,6,  7,4,  3,5,  6,4,  0,6,  3,3,    5,6,  2,4,  1,5,  5,5,  5,4,  3,6,  2,3,  7,6,    
  	                                                                                                    
  	/* K48 = 14 */                                                                                      
  	3,1,  0,1,  6,1,  1,2,  4,3,  4,0,  6,0,  5,3,    2,1,  3,0,  4,2,  7,1,  2,2,  6,2,  5,1,  5,0,    
  	7,3,  1,0,  1,1,  2,0,  6,3,  5,2,  4,1,  7,0,    4,5,  1,4,  6,6,  0,6,  6,4,  2,3,  7,6,  5,5,    
  	2,4,  0,5,  4,6,  5,4,  1,5,  4,4,  6,5,  1,3,    3,6,  0,4,  7,4,  3,5,  3,4,  1,6,  0,3,  5,6,    
  	                                                                                                    
  	/* K48 = 15 */                                                                                      
  	2,1,  7,2,  5,1,  0,2,  7,0,  3,0,  5,0,  4,3,    1,1,  2,0,  3,2,  6,1,  1,2,  5,2,  4,1,  4,0,    
  	6,3,  0,0,  0,1,  1,0,  5,3,  4,2,  3,1,  6,0,    3,5,  0,4,  5,6,  7,5,  5,4,  1,3,  6,6,  4,5,    
  	1,4,  7,4,  3,6,  4,4,  0,5,  3,4,  5,5,  0,3,    2,6,  3,3,  6,4,  2,5,  2,4,  0,6,  7,6,  4,6
	};

	/* pos */
	static unsigned char kp5[64][2] = {
	  /* pos */                                                                                      
	  0,0,  0,1,  0,2,  0,3,  0,4,  0,5,  0,6,  0,7,    1,0,  1,1,  1,2,  1,3,  1,4,  1,5,  1,6,  1,7,
	  2,0,  2,1,  2,2,  2,3,  2,4,  2,5,  2,6,  2,7,    3,0,  3,1,  3,2,  3,3,  3,4,  3,5,  3,6,  3,7,
	  4,0,  4,1,  4,2,  4,3,  4,4,  4,5,  4,6,  4,7,    5,0,  5,1,  5,2,  5,3,  5,4,  5,5,  5,6,  5,7,
	  6,0,  6,1,  6,2,  6,3,  6,4,  6,5,  6,6,  6,7,    7,0,  7,1,  7,2,  7,3,  7,4,  7,5,  7,6,  7,7
	};

	static unsigned char BitMask[8] = {
		0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
	};
	
	/* Data Initial Permutation (IP) */
	static unsigned char dp1_org[64] = {
		58, 50, 42, 34, 26, 18, 10,  2,  60, 52, 44, 36, 28, 20, 12, 4,  62, 54, 46, 38, 30, 22, 14, 6,  64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17,  9,  1,  59, 51, 43, 35, 27, 19, 11, 3,  61, 53, 45, 37, 29, 21, 13, 5,  63, 55, 47, 39, 31, 23, 15, 7
	};
	static unsigned char dp1[64][2] = {
  7,1,  6,1,  5,1,  4,1,  3,1,  2,1,  1,1,  0,1,    7,3,  6,3,  5,3,  4,3,  3,3,  2,3,  1,3,  0,3,
  7,5,  6,5,  5,5,  4,5,  3,5,  2,5,  1,5,  0,5,    7,7,  6,7,  5,7,  4,7,  3,7,  2,7,  1,7,  0,7,
  7,0,  6,0,  5,0,  4,0,  3,0,  2,0,  1,0,  0,0,    7,2,  6,2,  5,2,  4,2,  3,2,  2,2,  1,2,  0,2,
  7,4,  6,4,  5,4,  4,4,  3,4,  2,4,  1,4,  0,4,    7,6,  6,6,  5,6,  4,6,  3,6,  2,6,  1,6,  0,6
	};
  
  
	/* Expent right 32 bits key to 48 bits */
	static unsigned char dp2_org[48] = {
		32,  1,  2,  3,  4,  5,  4,  5,   6,  7,  8,  9,  8,  9, 10, 11,  12, 13, 12, 13, 14, 15, 16, 17,  16, 17, 18, 19, 20, 21, 20, 21, 
		22, 23, 24, 25, 24, 25, 26, 27,  28, 29, 28, 29, 30, 31, 32, 1
	};
	static unsigned char dp2[48][2] = {
  3,7,  0,0,  0,1,  0,2,  0,3,  0,4,  0,3,  0,4,    0,5,  0,6,  0,7,  1,0,  0,7,  1,0,  1,1,  1,2,
  1,3,  1,4,  1,3,  1,4,  1,5,  1,6,  1,7,  2,0,    1,7,  2,0,  2,1,  2,2,  2,3,  2,4,  2,3,  2,4,
  2,5,  2,6,  2,7,  3,0,  2,7,  3,0,  3,1,  3,2,    3,3,  3,4,  3,3,  3,4,  3,5,  3,6,  3,7,  0,0
	};
	
	/* Data Final Permutation (IP) */
	static unsigned char dp3_org[64] = {
		40, 8, 48, 16, 56, 24, 64, 32,  39, 7, 47, 15, 55, 23, 63, 31,  38, 6, 46, 14, 54, 22, 62, 30,  37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,  35, 3, 43, 11, 51, 19, 59, 27,  34, 2, 42, 10, 50, 18, 58, 26,  33, 1, 41,  9, 49, 17, 57, 25
	};
	static unsigned char dp3[64][2] = {
  	4,7,  0,7,  5,7,  1,7,  6,7,  2,7,  7,7,  3,7,    4,6,  0,6,  5,6,  1,6,  6,6,  2,6,  7,6,  3,6,    
  	4,5,  0,5,  5,5,  1,5,  6,5,  2,5,  7,5,  3,5,    4,4,  0,4,  5,4,  1,4,  6,4,  2,4,  7,4,  3,4,    
  	4,3,  0,3,  5,3,  1,3,  6,3,  2,3,  7,3,  3,3,    4,2,  0,2,  5,2,  1,2,  6,2,  2,2,  7,2,  3,2,    
  	4,1,  0,1,  5,1,  1,1,  6,1,  2,1,  7,1,  3,1,    4,0,  0,0,  5,0,  1,0,  6,0,  2,0,  7,0,  3,0    
	};

	/* Data Permutation P (PP) */
	static unsigned char dp4_org[32] = {
		16,  7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10, 
		 2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25, 
	};
  static unsigned char dp4[32][2] = {
		1,7,  0,6,  2,3,  2,4,  3,4,  1,3,  3,3,  2,0,    0,0,  1,6,  2,6,  3,1,  0,4,  2,1,  3,6,  1,1,
  	0,1,  0,7,  2,7,  1,5,  3,7,  3,2,  0,2,  1,0,    2,2,  1,4,  3,5,  0,5,  2,5,  1,2,  0,3,  3,0
	};
	
	static unsigned char dp5[64][2] = {
  6,0,  6,0,  0,0,  0,5,  0,1,  0,2,  0,3,  0,4,    6,0,  6,0,  0,6,  1,3,  0,7,  1,0,  1,1,  1,2,
  6,0,  6,0,  1,4,  2,1,  1,5,  1,6,  1,7,  2,0,    6,0,  6,0,  2,2,  2,7,  2,3,  2,4,  2,5,  2,6,
  6,0,  6,0,  3,0,  3,5,  3,1,  3,2,  3,3,  3,4,    6,0,  6,0,  3,6,  4,3,  3,7,  4,0,  4,1,  4,2,
  6,0,  6,0,  4,4,  5,1,  4,5,  4,6,  4,7,  5,0,    6,0,  6,0,  5,2,  5,7,  5,3,  5,4,  5,5,  5,6,
	};

	static unsigned char dp6[6] = {
		2, 4, 5, 6, 7, 3
	};
	
	/* Substitution Box Matrix List */
	/* Table - s1 */
	static unsigned char s[8][4][16] = {
		
		/* Table - s1 */
		14, 4, 13, 1, 2, 15, 11, 8,    3, 10, 6, 12, 5, 9, 0, 7,
		 0, 15, 7, 4, 14, 2, 13, 1,   10, 6, 12, 11, 9, 5, 3, 8,
		 4, 1, 14, 8, 13, 6,  2,11,   15, 12, 9, 7, 3, 10, 5, 0,
		15, 12, 8, 2,  4, 9,  1, 7,    5, 11, 3, 14, 10, 0, 6, 13,

		/* Table - s2 */
		15, 1, 8, 14, 6, 11, 3, 4, 		9, 7, 2, 13, 12, 0, 5, 10,
		3, 13, 4, 7, 15, 2, 8, 14, 		12, 0, 1, 10, 6, 9, 11, 5,
		0, 14, 7, 11, 10, 4, 13, 1, 	5, 8, 12, 6, 9, 3, 2, 15,
		13, 8, 10, 1, 3, 15, 4, 2, 		11, 6, 7, 12, 0, 5, 14, 9,
	
		/* Table - s3 */
		10, 0, 9, 14, 6, 3, 15, 5, 		1, 13, 12, 7, 11, 4, 2, 8,
		13, 7, 0, 9, 3, 4, 6, 10, 		2, 8, 5, 14, 12, 11, 15, 1,
		13, 6, 4, 9, 8, 15, 3, 0, 		11, 1, 2, 12, 5, 10, 14, 7,
		1, 10, 13, 0, 6, 9, 8, 7, 		4, 15, 14, 3, 11, 5, 2, 12,

		/* Table - s4 */
		7, 13, 14, 3, 0, 6, 9, 10, 		1, 2, 8, 5, 11, 12, 4, 15,
		13, 8, 11, 5, 6, 15, 0, 3, 		4, 7, 2, 12, 1, 10, 14, 9,
		10, 6, 9, 0, 12, 11, 7, 13, 	15, 1, 3, 14, 5, 2, 8, 4,
		3, 15, 0, 6, 10, 1, 13, 8, 		9, 4, 5, 11, 12, 7, 2, 14,

		/* Table - s5 */
		2, 12, 4, 1, 7, 10, 11, 6, 		8, 5, 3, 15, 13, 0, 14, 9,
		14, 11, 2, 12, 4, 7, 13, 1, 	5, 0, 15, 10, 3, 9, 8, 6,
		4, 2, 1, 11, 10, 13, 7, 8, 		15, 9, 12, 5, 6, 3, 0, 14,
		11, 8, 12, 7, 1, 14, 2, 13, 	6, 15, 0, 9, 10, 4, 5, 3,

		/* Table - s6 */
		12, 1, 10, 15, 9, 2, 6, 8, 		0, 13, 3, 4, 14, 7, 5, 11,
		10, 15, 4, 2, 7, 12, 9, 5, 		6, 1, 13, 14, 0, 11, 3, 8,
		9, 14, 15, 5, 2, 8, 12, 3, 		7, 0, 4, 10, 1, 13, 11, 6,
		4, 3, 2, 12, 9, 5, 15, 10, 		11, 14, 1, 7, 6, 0, 8, 13,

		/* Table - s7 */
		4, 11, 2, 14, 15, 0, 8, 13, 	3, 12, 9, 7, 5, 10, 6, 1,
		13, 0, 11, 7, 4, 9, 1, 10, 		14, 3, 5, 12, 2, 15, 8, 6,
		1, 4, 11, 13, 12, 3, 7, 14, 	10, 15, 6, 8, 0, 5, 9, 2,
		6, 11, 13, 8, 1, 4, 10, 7, 		9, 5, 0, 15, 14, 2, 3, 12,

		/* Table - s8 */
		13, 2, 8, 4, 6, 15, 11, 1, 		10, 9, 3, 14, 5, 0, 12, 7,
		1, 15, 13, 8, 10, 3, 7, 4, 		12, 5, 6, 11, 0, 14, 9, 2,
		7, 11, 4, 1, 9, 12, 14, 2, 		0, 6, 10, 13, 15, 3, 5, 8,
		2, 1, 14, 7, 4, 10, 8, 13, 		15, 12, 9, 0, 3, 5, 6, 11 
	};
	
	int liX, liY, liZ, liCy = 0;
	unsigned char K64[8], K56[7 + 1], K48[6 + 2];
	unsigned char D64[8], L32[4 + 4], R32[4 + 4], R48[6 + 2], NR32[4 + 4], ND64[8];
	unsigned char E48[6 + 2], E32[4 + 4], NE32[4 + 4], E64[8];
	unsigned long long *llpTmp;
	unsigned char *lspTmp, *lspTmp1, lsTmp;

	/* Set K64[i], D64[i] accord viDesLen */
	switch (viDesLen){

		case 8:

			memcpy(K64, (unsigned char *)vpKey, 8);
			memcpy(D64, (unsigned char *)vpData, 8);
			break;

		case 16:

			liX = -1;
			lspTmp = (unsigned char *)vpKey;
			lspTmp1 = (unsigned char *)vpData;
			while(liX++ < 8 - 1){
				K64[liX] = (((lspTmp[2*liX] - 48) & 0xF0 ? lspTmp[2*liX] - 55 : lspTmp[2*liX] - 48) << 4) |
					(((lspTmp[2*liX + 1] - 48) & 0xF0 ? lspTmp[2*liX + 1] - 55 : lspTmp[2*liX + 1] - 48) & 0x0F);
				D64[liX] = (((lspTmp1[2*liX] - 48) & 0xF0 ? lspTmp1[2*liX] - 55 : lspTmp1[2*liX] - 48) << 4) |
					(((lspTmp1[2*liX + 1] - 48) & 0xF0 ? lspTmp1[2*liX + 1] - 55 : lspTmp1[2*liX + 1] - 48) & 0x0F);
			}
			break;

		default:
			return -1;
	}
	
	memset(L32, 0, sizeof(L32));
	memset(R32, 0, sizeof(R32));
	for(liX = -1; liX++ < 32 - 1; ){
		L32[ kp5[liX][0] ] |= 
			(((D64[ dp1[liX][0] ] & BitMask[ dp1[liX][1] ]) << dp1[liX][1]) >> kp5[liX][1]);
		R32[ kp5[liX][0] ] |= 
			(((D64[ dp1[liX + 32][0] ] & BitMask[ dp1[liX + 32][1] ]) << dp1[liX + 32][1]) >> kp5[liX][1]);
	}

	/* Init Data */
	for(liZ = -1; liZ++ < 16 - 1; ){
	
		/* Generate current 48 bits key K48[i] */
		memset(E64, 0, sizeof(E64));
		for(liX = -1, liCy = (viEncMode == 0 ? liZ : 15 - liZ); liX++ < 48 - 1; )
			E64[ liX / 6 ] |= 
				( ((K64[ kp4[liCy][liX][0] ] & BitMask[ kp4[liCy][liX][1] ]) << kp4[liCy][liX][1]) ^
 				  ((R32[ dp2[liX][0] ] & BitMask[ dp2[liX][1] ]) << dp2[liX][1]) )  >> 
 				  dp6[ liX % 6] ;
			 				  
		/* Select 48 bit to 32bit */
		for(liX = -1; liX++ < 4 - 1; ){
			liY = liX << 1;
			E64[liX] = ((s[liY][ E64[liY] >> 4][ E64[liY] & 0x0F]) << 4) | 
				(s[liY + 1][ E64[liY + 1] >> 4][ E64[liY + 1] & 0x0F]);
		}

		/* Permutation E32[i] to NE32[i] */
		for(liX = -1, memset(&E64[4], 0, 4); liX++ < 32 - 1; )
			E64[ kp5[liX + 32 ][0] ] |= 
				(((E64[ dp4[liX][0] ] & BitMask[ dp4[liX][1] ]) << dp4[liX][1] ) >> kp5[liX + 32][1] );
		
		
   	for(liX = -1, memset(&E64[0], 0, 4); liX++ < 4 - 1; )
			E64[ liX ] = E64[ liX + 4] ^ L32[liX];
   	
   	memcpy(&E64[4], R32, 4);
   	memcpy(L32, R32, 4);
   	memcpy(R32, E64, 4);
	}

	for(liX = -1, memset(D64, 0, sizeof(D64)) ; liX++ < 64 - 1; )
		D64[ kp5[liX][0] ] |= 
			(((E64[ dp3[liX][0] ] & BitMask[ dp3[liX][1] ]) << dp3[liX][1] ) >> kp5[liX][1] );
	memcpy(vpEncData, D64, 8);
	
	/* Set vpEncData accord viDesLen */
	switch (viDesLen){

		case 8:
			memcpy(vpEncData, D64, 8);
			break;

		case 16:
			liX = -1;
			while(liX++ < 8 - 1)
				sprintf(vpEncData + liX * 2, "%02X", D64[liX]);
			break;

		default:
			return -1;
	}
	
	//  /* ===================== ASC 计算方法 =========================== */
	//	int liroll, liccc;
	//	unsigned char key[64], key1[56], key2[48], key4[6 + 2], lctmp[10];
	//	unsigned char dat[64], dat1[64], dat2[48], dat3[48], dat4[4], dat41[32], dat42[32], Left[32],Right[32], encdata[64];
	//	
	//	
	//	printf("64bit key is \n");
	//	liX = -1;
	//	while(liX++ < 64 - 1){
	//	  key[liX] = (K64[ liX / 8 ] & (1 << (7 - (liX % 8))) ? 1 : 0);
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", key[liX]);
	//	}
	//  printf("\n");
	//
	//  printf("56bit key is \n");
	//	liX = -1;
	//	while(liX++ < 56 - 1){
	//	  key1[liX] = (K64[ (kp1[liX] - 1) / 8 ] & (1 << (7 - ((kp1[liX] - 1) % 8))) ? 1 : 0);
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", key1[liX]);
	//	}
	//  printf("\n");
	//  
	//  	liX = -1;
	//	while(liX++ < 64 - 1){
	//	  dat[liX] = (D64[ liX / 8 ] & (1 << (7 - (liX % 8))) ? 1 : 0);
	//	//  printf("%d ", key[liX]);
	//	}
	//	//printf("\n");
	//
	//  //printf("64bit dat is \n");
	//	liX = -1;
	//	while(liX++ < 64 - 1){
	//	  dat1[liX] = dat[ dp1_org[liX] - 1 ];
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  //printf("%d", dat1[liX]);
	//	}
	//  printf("\n");
	//  
	//	liccc = -1;
	//	liroll =0;
	//	while(liccc ++ < 16 - 1){
	//	
	//	printf("-------------------- ROLL %d ----\n", liccc);
	//		
	//	liroll = kp2[liccc], liY = -1;
	//  //printf("----------- 56bit key after left shift lix = %d\n", liX);
	//	memcpy(lctmp, key1, liroll);
	//	while(liY++ < 28 -1 - liroll)
	//		key1[liY] = key1[liY + liroll];
	//	memcpy(&key1[28 - liroll], lctmp, liroll);
	//  
	//	liY = 28 -1;
	//	memcpy(lctmp, &key1[28], liroll);
	//	while(liY++ < 56 -1 - liroll)
	//		key1[liY] = key1[liY + liroll];
	//	memcpy(&key1[56 - liroll], lctmp, liroll);
	//  
	//	liroll = -1;
	//	while(liroll++ < 56 - 1){
	//	  if((liroll % 8 == 0) && (liroll != 0)) printf(" ");
	//	  //printf("%d", key1[liroll]);
	//	}
	//  //printf("\n");
	//	
	//  //printf("48bit key is \n");
	//	liX = -1;
	//	while(liX++ < 48 - 1){
	//	  
	//	  key2[liX] = key1[ kp3_org[liX] - 1];
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  //printf("%d", key2[liX]);
	//	}
	//  printf("\n");
	//	
	//	//-------------------------
	//
	//  printf("64bit dat is \n");
	//	liX = -1;
	//	while(liX++ < 64 - 1){
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", dat1[liX]);
	//	}
	//  printf("\n");
	//
	//  printf("Rdata to 48 bit dat is \n");
	//	liX = -1;
	//	while(liX++ < 48 - 1){
	//	  dat2[liX] = dat1[ 32 + dp2_org[liX] - 1 ];
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", dat2[liX]);
	//	}
	//  printf("\n");
	//  
	//
	//  printf("56bit key is \n");
	//	liX = -1;
	//	while(liX++ < 4 - 1){
	//	  
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", key1[liX]);
	//	}
	//  printf("\n");
	//
	//  printf("48bit key is \n");
	//	liX = -1;
	//	while(liX++ < 48 - 1){
	//	  
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", key2[liX]);
	//	}
	//  printf("\n");
	//
	//  printf("Xor 48 bit key and dat is \n");
	//	liX = -1;
	//	while(liX++ < 48 - 1){
	//	  dat3[liX] = dat2[liX] ^ key2[liX];
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", dat3[liX]);
	//	}
	//  printf("\n");
	//	  
	//  printf("Select 48 bit to 32bit is \n");
	//	memset(dat4, 0, sizeof(dat4));
	//
	//	liX = 0;
	//	dat4[0] |= s[liX * 2][dat3[0] * 2 + dat3[5] * 1][dat3[1] * 8 + dat3[2] * 4 + dat3[3] * 2 + dat3[4] * 1] << 4;
	//	dat4[0] |= s[liX * 2 + 1][dat3[6] * 2 + dat3[11] * 1][dat3[7] * 8 + dat3[8] * 4 + dat3[9] * 2 + dat3[10] * 1];
	//	
	//	liX = 1;
	//	dat4[1] |= s[liX * 2][dat3[12] * 2 + dat3[17] * 1][dat3[13] * 8 + dat3[14] * 4 + dat3[15] * 2 + dat3[16] * 1] << 4;
	//	dat4[1] |= s[liX * 2 + 1][dat3[18] * 2 + dat3[23] * 1][dat3[19] * 8 + dat3[20] * 4 + dat3[21] * 2 + dat3[22] * 1];
	//  
	//	liX = 2;
	//	dat4[2] |= s[liX * 2][dat3[24] * 2 + dat3[29] * 1][dat3[25] * 8 + dat3[26] * 4 + dat3[27] * 2 + dat3[28] * 1] << 4;
	//	dat4[2] |= s[liX * 2 + 1][dat3[30] * 2 + dat3[35] * 1][dat3[31] * 8 + dat3[32] * 4 + dat3[33] * 2 + dat3[34] * 1];
	//  
	//	liX = 3;
	//	dat4[3] |= s[liX * 2][dat3[36] * 2 + dat3[41] * 1][dat3[37] * 8 + dat3[38] * 4 + dat3[39] * 2 + dat3[40] * 1] << 4;
	//	dat4[3] |= s[liX * 2 + 1][dat3[42] * 2 + dat3[47] * 1][dat3[43] * 8 + dat3[44] * 4 + dat3[45] * 2 + dat3[46] * 1];
	//	
	//	liX = -1;
	//	while(liX++ < 32 - 1){
	//	  dat41[liX] = (dat4[ liX / 8 ] & (1 << (7 - (liX % 8))) ? 1 : 0);
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", dat41[liX]);
	//	}
	//	printf("\n");
	//	
	//  printf("change 32 bit to 32bit is \n");
	//	liX = -1;
	//	while(liX++ < 32 - 1){
	//	  dat42[liX] = dat41[dp4_org[liX] - 1];
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", dat42[liX]);
	//	}
	//	printf("\n");
	//	
	//	printf("New Right32 = Xor 32 bit to Ldata is \n");
	//	liX = -1;
	//	while(liX++ < 32 - 1){
	//	  Right[liX] = dat1[liX] ^ dat42[liX];
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", Right[liX]);
	//	}
	//	printf("\n");
	//
	//	printf("New Ldata is \n");
	//	liX = -1;
	//	while(liX++ < 32 - 1){
	//		Left[liX] = dat1[liX + 32];	
	//	  if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", Left[liX]);
	//	}
	//	printf("\n");
	//	
	//	liX = -1;
	//	while(liX++ < 32 - 1){
	//		dat1[liX] = Left[liX];
	//		dat1[liX + 32] = Right[liX];
	//	}
	//	printf("\n");
	//
	//	
	//}
	//
	//	liX = -1;
	//	while(liX++ < 32 - 1){
	//		dat1[liX] = Right[liX];
	//		dat1[liX + 32] = Left[liX];
	//	}
	//
	//	liX = -1;
	//	while(liX++ < 64 - 1){
	//		encdata[liX] = dat1[dp3_org[liX] - 1];
	//	}
	//
	//	printf("Enc data is \n");
	//	liX = -1;
	//	while(liX++ < 64 - 1){
	//		if((liX % 8 == 0) && (liX != 0)) printf(" ");
	//	  printf("%d", encdata[liX]);
	//	}
	//	printf("\n");
	//
	//
	//  return 0;
}
