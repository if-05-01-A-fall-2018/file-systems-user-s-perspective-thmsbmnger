#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define buffer_size 256

void print_error(char* message);

int main(int argc, char const *argv[]) {
  if (argc != 3) {
    print_error("Invalid amount of arguments");
    return 1;
  }
  
  char const* src_path = argv[1];
  int src_fd; 
  if ((src_fd = open(src_path, O_RDONLY)) == -1) {
    print_error("Source file does not exist.");
    return 1;
  } 
  
  char const* dst_path = argv[2];
  int dst_fd;
  if ((dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC)) == -1) {
    print_error("Something happend???");
    return 1;
  }

  int bytes_read = 0;
  char buffer[buffer_size];
  while ((bytes_read = read(src_fd, buffer, buffer_size))) {
    write(dst_fd, buffer, bytes_read);
  }

  close(src_fd);
  close(dst_fd);
  return 0;
}

void print_error(char* message) {
  fprintf(stderr, "Error: %s\n", message);
}
