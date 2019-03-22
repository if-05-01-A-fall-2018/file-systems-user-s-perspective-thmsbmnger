#define _DEFAULT_SOURCE

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <string.h>

#define header_length 54

void print_error(char* message);
void print_header(const char* file_path);
char* get_file_type(mode_t mode);
void print_access_permissions(mode_t mode);

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		print_error("Invalid amount of arguments");
		return 1;
	}

	const char* file_path = argv[1];
	struct stat file_stats;
	if (lstat(file_path, &file_stats) == -1) {
		print_error("File does not exist");
		return 1;
	}

  mode_t mode = file_stats.st_mode;

  print_header(file_path);

  printf("   %-22s: %s\n", "File type", get_file_type(mode));
  printf("   %-22s: ", "Access privileges");
  print_access_permissions(mode);
  printf("\n");
  printf("   %-22s: %ld\n", "inode-Number", file_stats.st_ino);
  printf("   %-22s: dev = %d; rdev = %d\n", "Device numbers", major(file_stats.st_rdev), minor(file_stats.st_rdev));
  printf("   %-22s: %ld\n", "Links count", file_stats.st_nlink);
  printf("   %-22s: %d\n", "UID", file_stats.st_uid);
  printf("   %-22s: %d\n", "GID", file_stats.st_gid);
  printf("   %-22s: %ld\n", "File size", file_stats.st_size);
  printf("   %-22s: %s", "Last access",ctime(&file_stats.st_atime));
  printf("   %-22s: %s", "Last modification", ctime(&file_stats.st_mtime));
  printf("   %-22s: %s", "Last inode change", ctime(&file_stats.st_ctime));
  
	return 0;
}

void print_error(char* message) {
	fprintf(stderr, "Error: %s\n", message);
}

void print_header(const char* file_path) {
  size_t len = strlen(file_path);
  for (int i = 0; i < (header_length / 2) - (len / 2) - 3; i++) {
    printf("-");
  }
  printf(" %s ", file_path);
  for (int i = 0; i < (header_length / 2) - (len / 2); i++) {
    printf("-");
  }
  printf("\n");
}

char* get_file_type(mode_t mode) {
  if (S_ISBLK(mode)) return "Block special file";
  else if (S_ISCHR(mode)) return "Character special file";
  else if (S_ISDIR(mode)) return "Directory";
  else if (S_ISFIFO(mode)) return "FIFO special file";
  else if (S_ISREG(mode)) return "Regular file";
  else if (S_ISLNK(mode)) return "Symbolic link";
  else if (S_ISSOCK(mode)) return "Socket";
  return "";
}

void print_access_permissions(mode_t mode) {
  printf( (S_ISDIR(mode)) ? "d" : "-");
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
