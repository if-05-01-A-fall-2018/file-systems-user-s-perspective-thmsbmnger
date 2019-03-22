#define _DEFAULT_SOURCE

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

void print_err(char* message);
char* get_file_type(mode_t mode);
void print_attrib(struct stat fstats);
void print_access(mode_t mode);

int main(int argc, char const *argv[]) {
  const char* dir_path;
  if (argc >= 2) {
    dir_path = argv[1];
  } else {
    dir_path = ".";
  }

  DIR* directory;
  if ((directory = opendir(dir_path)) == NULL) {
    print_err("Argument not a directory");
    return 1;
  }

  struct dirent* entry;
  while ((entry = readdir(directory)) != NULL) {
    char* entry_name = entry->d_name;
    if (strncmp(".", entry_name, 1) == 0) continue;
    struct stat fstats;
    if (lstat(entry_name, &fstats) == 0) {
      print_attrib(fstats);
      printf(" %s\n", entry_name);
    }
  }

  return 0;
}

void print_err(char* message) {
  fprintf(stderr, "Error: %s\n", message);
}

void print_attrib(struct stat fstats) {
  printf("%s ", get_file_type(fstats.st_mode));
  print_access(fstats.st_mode);
  printf(" %05d %05d", fstats.st_uid, fstats.st_gid);
  printf(" %09ld", fstats.st_size);
  char date_string[128];
  strftime(date_string, sizeof(date_string), "%Y-%m-%d-%H-%M", localtime(&fstats.st_mtime));
  printf(" %s", date_string);
}

char* get_file_type(mode_t mode) {
  if (S_ISBLK(mode)) return "b";
  else if (S_ISCHR(mode)) return "c";
  else if (S_ISDIR(mode)) return "d";
  else if (S_ISFIFO(mode)) return "f";
  else if (S_ISREG(mode)) return "-";
  else if (S_ISLNK(mode)) return "l";
  else if (S_ISSOCK(mode)) return "s";
  return "";
}

void print_access(mode_t mode) {
  printf( (mode & S_IRUSR) ? "r" : "-");
  printf( (mode & S_IWUSR) ? "w" : "-");
  printf( (mode & S_IXUSR) ? "x" : "-");
  printf( (mode & S_IRGRP) ? "r" : "-");
  printf( (mode & S_IWGRP) ? "w" : "-");
  printf( (mode & S_IXGRP) ? "x" : "-");
  printf( (mode & S_IROTH) ? "r" : "-");
  printf( (mode & S_IWOTH) ? "w" : "-");
  printf( (mode & S_IXOTH) ? "x" : "-");
}