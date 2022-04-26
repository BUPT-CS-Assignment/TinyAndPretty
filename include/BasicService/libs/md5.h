#include <cstdint>
#include <cstring>
#include <string>

#define F(x, y, z) ((x & y) | (~x & z))
#define G(x, y, z) ((x & z) | (y & ~z))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))
#define shift(x, n) ((x << n) | (x >> (32 - n)))

#define FF(a, b, c, d, x, s, ac) a = b + ((shift((a + F(b, c, d) + x + ac), s)))
#define GG(a, b, c, d, x, s, ac) a = b + ((shift((a + G(b, c, d) + x + ac), s)))
#define HH(a, b, c, d, x, s, ac) a = b + ((shift((a + H(b, c, d) + x + ac), s)))
#define II(a, b, c, d, x, s, ac) a = b + ((shift((a + I(b, c, d) + x + ac), s)))

#define A seq[0]
#define B seq[1]
#define C seq[2]
#define D seq[3]

int md5_process(std::string f, uint8_t (&final)[17]);
int md5_calculate(uint32_t (&seq)[4], const uint32_t buff[]);
std::string md5(std::string f);