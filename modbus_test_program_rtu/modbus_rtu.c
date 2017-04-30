
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <modbus/modbus.h>
#include <errno.h>

int scan_mint_ai(modbus_t *ctx)
{
  uint16_t read_reg[8] = {0};
  int mint_ai_slave = 1;
  if (ctx == NULL) {
    fprintf(stderr, "Invalid modbus context\n");
    return -1;
  }
  if(modbus_set_slave(ctx, mint_ai_slave) == -1)
  printf("Didn't connect to slave %d/n", mint_ai_slave);

  if(modbus_read_input_registers(ctx, 0x00, 8, read_reg) == -1)
  {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    return -1;
  }
  printf("ANALOG INPUT SCAN\n");
  for(int i = 0; i<8; i++)
  {
    printf("CH_%d=%d ",i, read_reg[i]);
  }
  printf("\n");
  return 0;
}

int scan_mint_do(modbus_t *ctx)
{
  uint16_t read_reg[2] = {0};
  int mint_ai_slave = 3;
  if (ctx == NULL) {
    fprintf(stderr, "Invalid modbus context\n");
    return -1;
  }
  if(modbus_set_slave(ctx, mint_ai_slave) == -1)
  printf("Didn't connect to slave %d/n", mint_ai_slave);
  printf("DIGITAL OUTPUT SCAN\n");
  if(modbus_read_input_registers(ctx, 0x54, 1, read_reg) == -1)
  {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    return -1;
  }
  for (int i = 0; i < 16; i++) {
        printf("DO_%d=%d ", 15-i, (read_reg[0] & 0x8000) >> 15);
        read_reg[0] <<= 1;
    }
    printf("\n");
  return 0;
}

int scan_mint_di(modbus_t *ctx)
{
  uint8_t read_reg[16] = {0};
  int mint_ai_slave = 2;

  if (ctx == NULL) {
    fprintf(stderr, "Invalid modbus context\n");
    return -1;
  }
  if(modbus_set_slave(ctx, mint_ai_slave) == -1)
  printf("Didn't connect to slave %d/n", mint_ai_slave);

  if(modbus_read_input_bits(ctx, 0x00, 16, read_reg) == -1)
  {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
    return -1;
  }
  printf("DIGITAL INPUT SCAN\n");
  for(int i = 0; i<16; i++)
  {
    printf("DI_%d=%d ",i, read_reg[i]);
  }
  printf("\n");
  return 0;
}

int toggle_mint_do(modbus_t *ctx, int do_number, int state)
{
  int rc = 0;
  int mint_ai_slave = 3;
  if (ctx == NULL) {
    fprintf(stderr, "Invalid modbus context\n");
    return -1;
  }
  if(modbus_set_slave(ctx, mint_ai_slave) == -1)
  printf("Didn't connect to slave %d/n", mint_ai_slave);
  rc = modbus_write_bit(ctx, do_number, state);
  printf("Setting DO_%d to %d\n", do_number, state);
  if (rc == -1) {
    fprintf(stderr, "Error=%s\n", modbus_strerror(errno));
    return -1;
  }
  return 0;
}

int main(int argc, char **argv)
{
  modbus_t *ctx = NULL;
  int baud = 9600;

  printf("Lib version %s\n", LIBMODBUS_VERSION_STRING);
  ctx = modbus_new_rtu("/dev/ttyLP2", baud, 'N', 8, 1);
  if (ctx == NULL) {
    fprintf(stderr, "Unable to create the libmodbus context\n");
    return -1;
  }
  //modbus_set_debug(ctx, 1);
  if(modbus_connect(ctx) == -1)
  {
    printf("Connection failed\n");
    return -1;
  }
  printf("Connected\n");
  modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
  usleep(100);
  if(argc > 1)
  {
    toggle_mint_do(ctx, atoi(argv[1]), atoi(argv[2]));
  }
  scan_mint_ai(ctx);
  scan_mint_do(ctx);
  scan_mint_di(ctx);
  modbus_close(ctx);
  modbus_free(ctx);
  return 0;
}
