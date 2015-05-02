#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "test-guestfs.h"

#ifdef _WIN32

#define DISK_IMG "C:/GuestFS/appliance/disk.img"

#else

#define DISK_IMG "disk.img"

#endif /* _WIN32 */

#define SMALL_FILES_COUNT 100
#define SHARED_MEMORY_SIZE 256
#define SHARED_MEMORY_NAME "MyShm"
#define REPORT_FILENAME "report.txt"

void
big_download_test (int enable_shm, size_t shm_size)
{
  struct test_guestfs *test;
  
  test = new_test_guestfs ();
  if (!test) {
    printf("ERROR: fail to create test\n");
    exit(EXIT_FAILURE);
  }
  
  test->protocol = XDR;
  test->ops->set_shared_memory (test, enable_shm, shm_size, SHARED_MEMORY_NAME);
  test->ops->add_drive (test, DISK_IMG);
  
  test->ops->add_download (test, "/home/novokrestdeb/subdisk.img", "subdisk.img");
  test->ops->add_download (test, "/home/novokrestdeb/subdisk.img", "subdisk.img");
  test->ops->add_download (test, "/home/novokrestdeb/subdisk.img", "subdisk.img");
  test->ops->add_download (test, "/home/novokrestdeb/video.mp4", "video.mp4");
  
  test->ops->set_report (test, REPORT_FILENAME, "BIG_DOWNLOAD_TEST", false);
  test->ops->run (test);
  free_test_guestfs (test);
}

void
small_download_test (int enable_shm, size_t shm_size)
{
  struct test_guestfs *test;
  int i = 0;
  char buf[128];
  
  test = new_test_guestfs ();
  if (!test) {
    printf("ERROR: fail to create test\n");
    exit(EXIT_FAILURE);
  }
  
  test->protocol = XDR;
  test->ops->set_shared_memory (test, enable_shm, shm_size, SHARED_MEMORY_NAME);
  test->ops->add_drive (test, DISK_IMG);
  
  for (i = 0; i < SMALL_FILES_COUNT; ++i) {
    sprintf(buf, "/home/novokrestdeb/%d", i);
    test->ops->add_read(test, buf, "");
  }
  
  test->ops->set_report (test, REPORT_FILENAME, "SMALL_DOWNLOAD_TEST", false);
  test->ops->run (test);
  free_test_guestfs (test);
}

void
big_upload_test (int enable_shm, size_t shm_size)
{
  struct test_guestfs *test;
  
  test = new_test_guestfs ();
  if (!test) {
    printf("ERROR: fail to create test\n");
    exit(EXIT_FAILURE);
  }
  
  test->protocol = XDR;
  test->ops->set_shared_memory (test, enable_shm, shm_size, SHARED_MEMORY_NAME);
  test->ops->add_drive (test, DISK_IMG);
  
  //test->ops->add_upload (test, "subdisk.img", "/home/novokrestdeb/subdisk.img");
  //test->ops->add_upload (test, "subdisk.img", "/home/novokrestdeb/subdisk.img");
  //test->ops->add_upload (test, "subdisk.img", "/home/novokrestdeb/subdisk.img");
  test->ops->add_upload (test, "video.mp4", "/home/novokrestdeb/video.mp4");
  
  test->ops->set_report (test, REPORT_FILENAME, "BIG_UPLOAD_TEST", false);
  test->ops->run (test);
  free_test_guestfs (test);
}

int
main(int args, char* argv[])
{
    big_download_test (1, 512);
    big_upload_test (0, 512);
    big_download_test (0, 512);
    
    return 0;
}
