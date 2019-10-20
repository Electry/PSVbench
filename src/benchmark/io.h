#ifndef IO_H
#define IO_H

int run_io_ux0_write();
int run_io_ux0_write_4k();
int run_io_ux0_read();
int run_io_ux0_read_4k();

int run_io_uma0_write();
int run_io_uma0_write_4k();
int run_io_uma0_read();
int run_io_uma0_read_4k();

int run_io_imc0_write();
int run_io_imc0_write_4k();
int run_io_imc0_read();
int run_io_imc0_read_4k();

void do_io_prepare();
void do_io_cleanup();

#endif
