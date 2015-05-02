#ifndef TEST_GUESTFS_H_
#define TEST_GUESTFS_H_

#include <stdbool.h>

typedef enum { PROTOBUF, XDR } protocol_t;

typedef struct {
    char *local_path;
    char *remote_path;
} file_t;

typedef struct {
    int size;
    char *name;
} shared_memory_t;

typedef struct report_entry
{
    time_t time;
    const char *msg;
} report_entry_t;

struct report;

struct report_ops
{
    int (*start) (struct report *report);
    int (*add_entry) (struct report *report, const char *msg);
    int (*print) (struct report *report);
};

typedef struct report {
    struct report_ops *ops;

    const char *path;
    const char *name;
    bool overwrite;

    time_t start;

    report_entry_t **entries;
    int nb_entries;
} report_t;

struct test_guestfs;

struct test_guestfs_ops {
    int (*set_protocol) (struct test_guestfs *test, protocol_t protocol);
    int (*set_shared_memory) (struct test_guestfs *test, int enable, int size, const char *name);
    int (*add_drive) (struct test_guestfs *test, const char *drive);
    int (*add_read) (struct test_guestfs *test, const char *src_remote_path, const char *dest_local_path);
    int (*add_download) (struct test_guestfs *test, const char *src_remote_path, const char *dest_local_path);
    int (*add_upload) (struct test_guestfs *test, const char *src_local_path, const char *dest_remote_path);
    int (*set_report) (struct test_guestfs *test, const char *path, const char *name, int enable_overwirite);
    int (*run) (struct test_guestfs *test);
};

typedef struct test_guestfs
{
    struct test_guestfs_ops *ops;

    protocol_t protocol;

    bool enable_shared_memory;
    shared_memory_t shared_memory;

    const char *drive;

    file_t **reads;
    int nb_reads;

    file_t **uploads;
    int nb_uploads;

    file_t **downloads;
    int nb_downloads;

    report_t *report;

} test_guestfs_t;

extern struct test_guestfs * new_test_guestfs ();
extern void free_test_guestfs (struct test_guestfs *test);

#endif /* TEST_GUESTFS_H_ */
