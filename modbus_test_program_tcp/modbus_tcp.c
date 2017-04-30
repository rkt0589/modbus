
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <modbus/modbus.h>

/* The goal of this program is to check all major functions of
   libmodbus:
   - write_coil
   - read_bits
   - write_coils
   - write_register
   - read_registers
   - write_registers
   - read_registers
*/
#define LOOP             1
#define SERVER_ID        1

int main(void)
{
    modbus_t *ctx;
    int rc;
    int nb_fail;
    int nb_loop;
    int addr;
    int nb;
    uint8_t *tab_rq_bits;
    uint8_t *tab_rp_bits;
    uint16_t *tab_rq_registers;
    uint16_t *tab_rw_rq_registers;
    uint16_t *tab_rp_registers;

    /* TCP */
    ctx = modbus_new_tcp("192.168.1.29", 1502);
    modbus_set_debug(ctx, TRUE);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    /* Allocate and initialize the different memory spaces */
    nb = 1;

    tab_rq_bits = (uint8_t *) malloc(nb * sizeof(uint8_t));
    memset(tab_rq_bits, 0, nb * sizeof(uint8_t));

    tab_rp_bits = (uint8_t *) malloc(nb * sizeof(uint8_t));
    memset(tab_rp_bits, 0, nb * sizeof(uint8_t));

    tab_rq_registers = (uint16_t *) malloc(nb * sizeof(uint16_t));
    memset(tab_rq_registers, 0, nb * sizeof(uint16_t));

    tab_rp_registers = (uint16_t *) malloc(nb * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb * sizeof(uint16_t));

    tab_rw_rq_registers = (uint16_t *) malloc(nb * sizeof(uint16_t));
    memset(tab_rw_rq_registers, 0, nb * sizeof(uint16_t));

    rc = modbus_read_registers(ctx, 0, nb, tab_rq_registers);
    if (rc == -1) {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
        return -1;
    }

    for (int i=0; i < rc; i++) {
        printf("reg[%d]=%d (0x%X)\n", i, tab_rq_registers[i], tab_rq_registers[i]);
    }

    printf("Done \n" );
    /* Free the memory */
    free(tab_rq_bits);
    free(tab_rp_bits);
    free(tab_rq_registers);
    free(tab_rp_registers);
    free(tab_rw_rq_registers);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
