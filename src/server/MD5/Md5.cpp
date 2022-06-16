#include <libs/md5.h>
#include <iostream>
#include <libs/BasicUtils.hpp>
std::string md5(std::string f)
{
    uint8_t final[17];
    md5_process(f, final);
    std::string output;
    for(int i = 0; i < 16; i++)
    {
        char temp[3];
        sprintf(temp, "%02x", final[i]);
        output.append(temp);
    }
    return output;
}

int md5_process(std::string f, uint8_t(&finalData)[17])
{
    uint32_t seq[] = {0x67452301, 0xefcdab89,
                      0x98badcfe, 0x10325476}; //默认为小端

    uint32_t fileLen_Byte;
    uint32_t fileLen_Bit[2];

    uint8_t complementaryBuff[65];
    uint32_t buff[16];

    fileLen_Byte = f.length();
    // printf("len:%d\n",fileLen_Byte);
    uint8_t* fileBuff = (uint8_t*)malloc(fileLen_Byte);
    // std::cout << "fileLen: " << fileLen_Byte << std::endl;
    memcpy(fileBuff, f.c_str(), fileLen_Byte);
    for(unsigned int i = 0; i < fileLen_Byte / 64; i++)
    {
        memset(buff, 0, 64);
        memcpy(&buff[0], &fileBuff[i * 64],
            64); //这段唯一用处是大小端转换，可以优化
        md5_calculate(seq, buff);
    }
    /*补位*/
    memset(buff, 0, 64);
    memset(complementaryBuff, 0, 64);
    memcpy(complementaryBuff, &fileBuff[fileLen_Byte - (fileLen_Byte % 64)],
        fileLen_Byte % 64); //注意可能的string传参问题
    complementaryBuff[fileLen_Byte % 64] = 128; //补1
    memcpy(&buff[0], &complementaryBuff[0], 64); //这样子memcpy就能大小端转换了吗？
    /*判断是不是还得填空位*/
    if((fileLen_Byte % 64) > 55)
    { // 64 - 1 - 8 = 55，但我这个还需要减1吗？
        md5_calculate(seq, buff);
        memset(buff, 0, 64);
    }

    fileLen_Bit[1] = fileLen_Byte / 0x20000000;
    fileLen_Bit[0] = (fileLen_Byte % 0x20000000) * 8;
    memcpy(&buff[14], fileLen_Bit, 8);

    //   printf("\n");
    //   for (int i = 0; i < 16; i++) {
    //     printf("%x", buff[i]);
    //   }
      //printf("\n");
      //printf("pre:%x %x %x %x\n", A, B, C, D);
    md5_calculate(seq, buff);
    // printf("trd:%x %x %x %x\n", A, B, C, D);

     //至此计算一已经完成，此时A、B、C、D已经产生了所有的值，把他交给finalData即可
     // DEBUG：输出，应该没问题
    memcpy(&finalData[0], &A, 4);  //高低位倒序赋值
    memcpy(&finalData[4], &B, 4);  //高低位倒序赋值
    memcpy(&finalData[8], &C, 4);  //高低位倒序赋值
    memcpy(&finalData[12], &D, 4); //高低位倒序赋值
    free(fileBuff);
    return 0;
}
int md5_calculate(uint32_t(&seq)[4], const uint32_t buff[])
{
    uint32_t a = A, b = B, c = C, d = D;
    /* Round 1 */
    FF(a, b, c, d, buff[0], 7, 0xd76aa478); /**/   /* 1 */
    FF(d, a, b, c, buff[1], 12, 0xe8c7b756); /**/  /* 2 */
    FF(c, d, a, b, buff[2], 17, 0x242070db); /**/  /* 3 */
    FF(b, c, d, a, buff[3], 22, 0xc1bdceee); /**/  /* 4 */
    FF(a, b, c, d, buff[4], 7, 0xf57c0faf); /**/   /* 5 */
    FF(d, a, b, c, buff[5], 12, 0x4787c62a); /**/  /* 6 */
    FF(c, d, a, b, buff[6], 17, 0xa8304613); /**/  /* 7 */
    FF(b, c, d, a, buff[7], 22, 0xfd469501); /**/  /* 8 */
    FF(a, b, c, d, buff[8], 7, 0x698098d8); /**/   /* 9 */
    FF(d, a, b, c, buff[9], 12, 0x8b44f7af); /**/  /* 10 */
    FF(c, d, a, b, buff[10], 17, 0xffff5bb1); /**/ /* 11 */
    FF(b, c, d, a, buff[11], 22, 0x895cd7be); /**/ /* 12 */
    FF(a, b, c, d, buff[12], 7, 0x6b901122); /**/  /* 13 */
    FF(d, a, b, c, buff[13], 12, 0xfd987193); /**/ /* 14 */
    FF(c, d, a, b, buff[14], 17, 0xa679438e); /**/ /* 15 */
    FF(b, c, d, a, buff[15], 22, 0x49b40821); /**/ /* 16 */

    /* Round 2 */
    GG(a, b, c, d, buff[1], 5, 0xf61e2562); /**/   /* 17 */
    GG(d, a, b, c, buff[6], 9, 0xc040b340); /**/   /* 18 */
    GG(c, d, a, b, buff[11], 14, 0x265e5a51); /**/ /* 19 */
    GG(b, c, d, a, buff[0], 20, 0xe9b6c7aa); /**/  /* 20 */
    GG(a, b, c, d, buff[5], 5, 0xd62f105d); /**/   /* 21 */
    GG(d, a, b, c, buff[10], 9, 0x02441453); /**/  /* 22 */
    GG(c, d, a, b, buff[15], 14, 0xd8a1e681); /**/ /* 23 */
    GG(b, c, d, a, buff[4], 20, 0xe7d3fbc8); /**/  /* 24 */
    GG(a, b, c, d, buff[9], 5, 0x21e1cde6); /**/   /* 25 */
    GG(d, a, b, c, buff[14], 9, 0xc33707d6); /**/  /* 26 */
    GG(c, d, a, b, buff[3], 14, 0xf4d50d87); /**/  /* 27 */
    GG(b, c, d, a, buff[8], 20, 0x455a14ed); /**/  /* 28 */
    GG(a, b, c, d, buff[13], 5, 0xa9e3e905); /**/  /* 29 */
    GG(d, a, b, c, buff[2], 9, 0xfcefa3f8); /**/   /* 30 */
    GG(c, d, a, b, buff[7], 14, 0x676f02d9); /**/  /* 31 */
    GG(b, c, d, a, buff[12], 20, 0x8d2a4c8a); /**/ /* 32 */

    /* Round 3 */
    HH(a, b, c, d, buff[5], 4, 0xfffa3942); /**/   /* 33 */
    HH(d, a, b, c, buff[8], 11, 0x8771f681); /**/  /* 34 */
    HH(c, d, a, b, buff[11], 16, 0x6d9d6122); /**/ /* 35 */
    HH(b, c, d, a, buff[14], 23, 0xfde5380c); /**/ /* 36 */
    HH(a, b, c, d, buff[1], 4, 0xa4beea44); /**/   /* 37 */
    HH(d, a, b, c, buff[4], 11, 0x4bdecfa9); /**/  /* 38 */
    HH(c, d, a, b, buff[7], 16, 0xf6bb4b60); /**/  /* 39 */
    HH(b, c, d, a, buff[10], 23, 0xbebfbc70); /**/ /* 40 */
    HH(a, b, c, d, buff[13], 4, 0x289b7ec6); /**/  /* 41 */
    HH(d, a, b, c, buff[0], 11, 0xeaa127fa); /**/  /* 42 */
    HH(c, d, a, b, buff[3], 16, 0xd4ef3085); /**/  /* 43 */
    HH(b, c, d, a, buff[6], 23, 0x04881d05); /**/  /* 44 */
    HH(a, b, c, d, buff[9], 4, 0xd9d4d039); /**/   /* 45 */
    HH(d, a, b, c, buff[12], 11, 0xe6db99e5); /**/ /* 46 */
    HH(c, d, a, b, buff[15], 16, 0x1fa27cf8); /**/ /* 47 */
    HH(b, c, d, a, buff[2], 23, 0xc4ac5665); /**/  /* 48 */

    /* Round 4 */
    II(a, b, c, d, buff[0], 6, 0xf4292244); /**/   /* 49 */
    II(d, a, b, c, buff[7], 10, 0x432aff97); /**/  /* 50 */
    II(c, d, a, b, buff[14], 15, 0xab9423a7); /**/ /* 51 */
    II(b, c, d, a, buff[5], 21, 0xfc93a039); /**/  /* 52 */
    II(a, b, c, d, buff[12], 6, 0x655b59c3); /**/  /* 53 */
    II(d, a, b, c, buff[3], 10, 0x8f0ccc92); /**/  /* 54 */
    II(c, d, a, b, buff[10], 15, 0xffeff47d); /**/ /* 55 */
    II(b, c, d, a, buff[1], 21, 0x85845dd1); /**/  /* 56 */
    II(a, b, c, d, buff[8], 6, 0x6fa87e4f); /**/   /* 57 */
    II(d, a, b, c, buff[15], 10, 0xfe2ce6e0); /**/ /* 58 */
    II(c, d, a, b, buff[6], 15, 0xa3014314); /**/  /* 59 */
    II(b, c, d, a, buff[13], 21, 0x4e0811a1); /**/ /* 60 */
    II(a, b, c, d, buff[4], 6, 0xf7537e82); /**/   /* 61 */
    II(d, a, b, c, buff[11], 10, 0xbd3af235); /**/ /* 62 */
    II(c, d, a, b, buff[2], 15, 0x2ad7d2bb); /**/  /* 63 */
    II(b, c, d, a, buff[9], 21, 0xeb86d391); /**/  /* 64 */

    A += a;
    B += b;
    C += c;
    D += d;
    return 0;
}

string md5(fstream& f)
{
    uint8_t final[17];
    md5_process(f, final);
    string output;
    for(int i = 0; i < 16; i++)
    {
        char temp[3];
        sprintf(temp, "%02x", final[i]);
        output.append(temp);
    }
    return output;
}

int md5_process(fstream& f, uint8_t(&finalData)[17])
{
    uint32_t seq[] = {0x67452301, 0xefcdab89, 0x98badcfe,
                      0x10325476}; //默认为小端

    uint32_t fileLen_Byte;
    uint32_t fileLen_Bit[2];

    uint8_t complementaryBuff[65];
    uint32_t buff[16];

    fileLen_Byte = get_file_len(f);
    // printf("len:%d\n",fileLen_Byte);
    uint8_t* fileBuff = (uint8_t*)malloc(fileLen_Byte);
    // std::cout << "fileLen: " << fileLen_Byte << std::endl;
    // memcpy(fileBuff, f.c_str(), fileLen_Byte);
    f.read((char*)fileBuff, fileLen_Byte); //
    for(unsigned int i = 0; i < fileLen_Byte / 64; i++)
    {
        memset(buff, 0, 64);
        memcpy(&buff[0], &fileBuff[i * 64],
            64); //这段唯一用处是大小端转换
        md5_calculate(seq, buff);
    }
    /*补位*/
    memset(buff, 0, 64);
    memset(complementaryBuff, 0, 64);
    memcpy(complementaryBuff, &fileBuff[fileLen_Byte - (fileLen_Byte % 64)],
        fileLen_Byte % 64); //注意可能的string传参问题
    complementaryBuff[fileLen_Byte % 64] = 128; //补1
    memcpy(&buff[0], &complementaryBuff[0],
        64); //这样子memcpy就能大小端转换了吗？
    /*判断是不是还得填空位*/
    if((fileLen_Byte % 64) > 55)
    { // 64 - 1 - 8 = 55，但我这个还需要减1吗？
        md5_calculate(seq, buff);
        memset(buff, 0, 64);
    }

    fileLen_Bit[1] = fileLen_Byte / 0x20000000;
    fileLen_Bit[0] = (fileLen_Byte % 0x20000000) * 8;
    memcpy(&buff[14], fileLen_Bit, 8);
    //   printf("\n");
    //   for (int i = 0; i < 16; i++) {
    //     printf("%x", buff[i]);
    //   }
    // printf("\n");
    // printf("pre:%x %x %x %x\n", A, B, C, D);
    md5_calculate(seq, buff);
    // printf("trd:%x %x %x %x\n", A, B, C, D);

    //至此计算一已经完成，此时A、B、C、D已经产生了所有的值，把他交给finalData即可
    // DEBUG：输出，应该没问题
    memcpy(&finalData[0], &seq, 16); //高低位倒序赋值
    free(fileBuff);
    return 0;
}

int get_file_len(fstream& f)
{
    if(f.is_open() == false)
    { // 文件打开失败
        UTILSTD::CONSOLE_LOG(true, "FILE open failed\n");
        return -1;
    }
    f.seekg(0, ios_base::end);
    int size = f.tellg();
    f.seekg(0, ios_base::beg);
    return size;
}