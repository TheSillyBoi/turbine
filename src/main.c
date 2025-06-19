#include "flags.h"
#include "instructions.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: turbine [file]\n");
    return EXIT_FAILURE;
  }
  FILE *fp = fopen(argv[1], "r");

  if (fp == NULL) {
    fprintf(stderr, "Failed to open file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  fseek(fp, 0L, SEEK_END);
  size_t file_size = ftell(fp);
  rewind(fp);

  uint8_t *file = malloc(file_size * sizeof(uint8_t));
  fread(file, file_size, 1, fp);

  VirtualMachine vm = init_vm();

  init_program_vm(&vm, file, file_size);
  free(file);
  file = NULL;

  debug_print_vm(&vm);
  while (!vm.halted) {
    step_vm(&vm);
    debug_print_vm(&vm);
  }

  delete_vm(&vm);
  fclose(fp);
  fp = NULL;
  return EXIT_SUCCESS;
}
