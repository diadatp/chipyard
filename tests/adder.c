#include "mmio.h"
#include <stdio.h>

#define ADDER_BASE 0x4000

#define ADDER_OFFA 0x00
#define ADDER_OFFB 0x08
#define ADDER_OFFS 0x10

#define ADDER_BEAT 8

#define ADDER_PARL 2

int main(int argc, char *argv[])
{
  printf("Adder Tests\n");

  printf("Exhaustive test of first 64 pairs:\n");

  for (uint16_t a = 0; a < 8; a++)
  {
    for (uint16_t b = 0; b < 8; b += ADDER_PARL)
    {
      for (int i = 0; i < ADDER_PARL; i++)
      {
        uint64_t shadow_a = a + 0;
        uint64_t shadow_b = b + i;

        reg_write64(ADDER_BASE + (3 * ADDER_BEAT * i) + ADDER_OFFA, shadow_a);
        reg_write64(ADDER_BASE + (3 * ADDER_BEAT * i) + ADDER_OFFB, shadow_b);

        uint64_t res = reg_read64(ADDER_BASE + (3 * ADDER_BEAT * i) + ADDER_OFFS);

        if (res != (shadow_a + shadow_b))
        {
          printf("Hardware result %lu does not match reference value %lu\n", res, (shadow_a + shadow_b));
          return 1;
        }
        printf("Hardware result %lu is correct\n", res);
      }
    }
  }

  printf("32bit addition carry overflow test:\n");

  uint64_t shadow_a = ((1lu << 32) - 1);
  uint64_t shadow_b = 1;

  reg_write64(ADDER_BASE + ADDER_OFFA, shadow_a);
  reg_write64(ADDER_BASE + ADDER_OFFB, shadow_b);

  uint64_t res = reg_read64(ADDER_BASE + ADDER_OFFS);

  if (res != (shadow_a + shadow_b))
  {
    printf("Hardware carry overflow result %lu does not match reference value %lu\n", res, (shadow_a + shadow_b));
    return 1;
  }

  printf("Hardware carry overflow is correct\n");

  printf("Done.\n");

  return 0;
}
