	#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include <guestfs.h>
#include "test-guestfs.h"

static const char * REPORT_DELIMITER = "=======================";

bool check_file_exists(const char* filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0;
}

static int
compare_keys_len(const void *p1, const void *p2)
{
    const char *key1 = *(char * const *)p1;
    const char *key2 = *(char * const *)p2;
    return strlen(key1) - strlen(key2);
}

static size_t
count_strings(char *const *argv)
{
    size_t c;

    for (c = 0; argv[c]; ++c)
        ;
    return c;
}

int mount_disks(guestfs_h* gfs)
{
    char **roots, *root, **mountpoints;

    printf("guestfs_inspect_os()...\n");
    roots = guestfs_inspect_os(gfs);
    if (roots == NULL || roots[0] == NULL) {
        printf("ERROR: no roots\n");
        return 1;
    }

    printf("iter over founded roots...\n");
    for (int j = 0; roots[j] != NULL; ++j) {
        root = roots[j];
        printf("root: %s\n", root);

        mountpoints = guestfs_inspect_get_mountpoints(gfs, root);
        if (mountpoints == NULL) {
            printf("ERROR: failed to inspect mountpoints for root: %s\n", root);
            return 1;
        }

        qsort(mountpoints, count_strings(mountpoints) / 2, 2 * sizeof(char*), compare_keys_len);
        for (int i = 0; mountpoints[i] != NULL; i += 2) {
            /* Ignore failures from this call, since bogus entries can appear in the guest's /etc/fstab.*/
            guestfs_mount (gfs, mountpoints[i+1], mountpoints[i]);
            free (mountpoints[i]);
            free (mountpoints[i+1]);
        }
        free (mountpoints);
        free (root);
    }
    free(roots);

    return 0;
}

static int 
read_file(guestfs_h* gfs, char const* remote_file_path)
{
    size_t fsize;
    char *file_content;

    printf("read: '%s'\n", remote_file_path);
    file_content = guestfs_read_file(gfs, remote_file_path, &fsize);
    if (!file_content) {
        printf("ERROR: read file='%s'\n", remote_file_path);
        return 1;
    }
    free(file_content);

    return 0;
}

static int
download_file(guestfs_h *g, const char *remote_filepath, const char *filepath)
{
    if (guestfs_download(g, remote_filepath, filepath) == -1) {
        printf("ERROR: guestfs_download()\n");
        return -1;
    }
    printf("File has been downloaded to '%s'\n", filepath);
    return 0;
}

static int
upload_file(guestfs_h *g, const char *local_filepath, const char *remote_filepath)
{
    if (guestfs_upload(g, local_filepath, remote_filepath) == -1) {
        printf("ERROR: guestfs_upload()\n");
        return -1;
    }
    printf("File has been uploaded to '%s'\n", remote_filepath);

    return 0;
}

static int
start_report(struct report *report)
{
    time_t t;

    time(&t);
    report->start = t;
}

static int
add_entry (struct report *report, const char *msg)
{
    time_t t;
    report_entry_t *entry;

    report->entries = realloc(report->entries, sizeof(report_entry_t *) * (report->nb_entries + 1));
    if (!report->entries) {
        return -1;
    }

    entry = malloc(sizeof(report_entry_t));
    if (!entry) {
        return -1;
    }

    time(&t);
    entry->time = t;
    entry->msg = strdup(msg);

    report->entries[report->nb_entries] = entry;
    report->nb_entries += 1;

    return 0;
}

static int
print_report(struct report *report)
{
    int i;
    FILE *file;
    
    file = fopen(report->path, report->overwrite ? "w" : "a");
    if (!file) {
        return -1;
    }

    fprintf(file, "%s\n\n", REPORT_DELIMITER);
    fprintf(file, "%s\n\n", report->name);

    for (i = 0; i < report->nb_entries; ++i) {
        fprintf(file, "[ %f ]: %s\n", difftime(report->entries[i]->time, report->start), report->entries[i]->msg);
    }

    fprintf(file, "\n%s\n\n", REPORT_DELIMITER);

    return 0;
}

static int
set_protocol(struct test_guestfs *test, protocol_t protocol)
{
    test->protocol = protocol;

    return 0;
}

static int
set_shared_memory(struct test_guestfs *test, int enable, int size, const char *name)
{
    test->enable_shared_memory = !!enable;
    test->shared_memory.size = size;
    test->shared_memory.name = strdup (name);
}

static int
add_drive(struct test_guestfs *test, const char *drive)
{
    test->drive = drive;
}

static int
add_read (struct test_guestfs *test, const const char *src_remote_path, const char *dest_local_path)
{
    file_t *file = malloc(sizeof(file_t));
    if (!file) {
        return -1;
    }

    file->local_path = strdup(dest_local_path);
    file->remote_path = strdup(src_remote_path);

    test->reads = realloc(test->reads, sizeof(file_t *) * (test->nb_reads + 1));
    if (!test->reads) {
        return -1;
    }

    test->reads[test->nb_reads] = file;
    test->nb_reads += 1;

    return 0;
}

static int
add_download(struct test_guestfs *test, const char *src_remote_path, const char *dest_local_path)
{
    file_t *file = malloc(sizeof(file_t));
    if (!file) {
        return -1;
    }

    file->local_path = strdup(dest_local_path);
    file->remote_path = strdup(src_remote_path);

    test->downloads = realloc(test->downloads, sizeof(file_t *) * (test->nb_downloads + 1));
    if (!test->downloads) {
        return -1;
    }

    test->downloads[test->nb_downloads] = file;
    test->nb_downloads += 1;

    return 0;
}

static int
add_upload(struct test_guestfs *test, const char *src_local_path, const char *dest_remote_path)
{
    file_t *file = malloc(sizeof(file_t));
    if (!file) {
        return -1;
    }

    file->local_path = strdup(src_local_path);
    file->remote_path = strdup(dest_remote_path);

    test->uploads = realloc(test->uploads, sizeof(file_t *) * (test->nb_uploads + 1));
    if (!test->uploads) {
        return -1;
    }

    test->uploads[test->nb_uploads] = file;
    test->nb_uploads += 1;

    return 0;

}

static int
set_report(struct test_guestfs *test, const char *path, const char *name, int enable_overwrite)
{
    test->report->path = path;
    test->report->name = name;
    test->report->overwrite = !!enable_overwrite;

    return 0;
}

static int 
run_test(struct test_guestfs *test)
{
    int i;
    char msg[256];
    guestfs_h *g;
    report_t *report;

    report = test->report;
    report->ops->start(report);

    g = guestfs_create();
    if (g == NULL) {
        printf("ERROR: guestfs_create()\n");
        return -1;
    }
    report->ops->add_entry(report, "guestfs handle has been created");

    printf("guestfs_set_verbose()...\n");
    if (guestfs_set_verbose(g, 1) == -1) {
        printf("ERROR occurred during guestfs_set_verbose()\n");
        return -1;
    }

    if (guestfs_set_direct(g, 1) == -1) {
        printf("ERROR: guestfs_set_direct()\n");
        return -1;
    }

    printf("guestfs_get_backend()...\n");
    char* default_backend = guestfs_get_backend(g);
    if (strcmp(default_backend, "direct")) {
        printf("guestfs_set_backend()...\n");
        if (guestfs_set_backend(g, "direct") == -1) {
            printf("ERROR occurred during guestfs_set_backend()\n");
            return -1;
        }
    }
    printf("backend is direct\n");

    const char* disk_image_path = test->drive;
    if (!check_file_exists(disk_image_path)) {
        printf("disk_image doesn't exist: %s\n", disk_image_path);
        return -1;
    }

    if (guestfs_add_drive_opts(g, disk_image_path,
        GUESTFS_ADD_DRIVE_OPTS_FORMAT, "raw",
        GUESTFS_ADD_DRIVE_OPTS_READONLY, 0,
        -1) == -1) {
        printf("ERROR occurred during guestfs_add_drive_opts\n");
        return -1;
    }

#ifdef GUESTFS_SHMEM
    guestfs_set_shared_memory(g, test->enable_shared_memory,
        GUESTFS_SET_SHARED_MEMORY_SIZE, test->shared_memory.size,
        GUESTFS_SET_SHARED_MEMORY_NAME, test->shared_memory.name,
        -1);
#endif /* GUESTFS_SHMEM */

    guestfs_set_recovery_proc(g, 0);

    printf("guestfs_launch()...\n");
    if (guestfs_launch(g) == -1) {
        printf("ERROR occurred during guestfs_launch()\n");
        return -1;
    }
    report->ops->add_entry(report, "guestfs has been launched");

    if (mount_disks(g)) {
        printf("ERROR occurred during mount_disks()\n");
        return -1;
    }
    report->ops->add_entry(report, "disk is mounted");

    report->ops->add_entry(report, "start reading files");
    for (i = 0; i < test->nb_reads; ++i) {
        read_file(g, test->reads[i]->remote_path);
        sprintf(msg, "read: %s", test->reads[i]->remote_path);
        report->ops->add_entry(report, msg);
    }
    report->ops->add_entry(report, "finish reading files from guest");

    report->ops->add_entry(report, "start downloading files from guest");
    for (i = 0; i < test->nb_downloads; ++i) {
        download_file(g, test->downloads[i]->remote_path, test->downloads[i]->local_path);
        sprintf(msg, "downloaded: from %s to %s", test->downloads[i]->remote_path, test->downloads[i]->local_path);
        report->ops->add_entry(report, msg);
    }
    report->ops->add_entry(report, "finish downloading files from guest");

    report->ops->add_entry(report, "start writing files to guest");
    for (i = 0; i < test->nb_uploads; ++i) {
        upload_file(g, test->uploads[i]->local_path, test->uploads[i]->remote_path);
        sprintf(msg, "uploaded: from %s to %s", test->uploads[i]->local_path, test->uploads[i]->remote_path);
        report->ops->add_entry(report, msg);
    }
    report->ops->add_entry(report, "finish writing files to guest");

    printf("guestfs_close()...\n");
    guestfs_close(g);
    report->ops->add_entry(report, "guestfs has been closed");

    test->report->ops->print(test->report);

    return 0;
}

static struct report_ops r_ops = {
    .start = start_report,
    .add_entry = add_entry,
    .print = print_report,
};

static struct test_guestfs_ops tg_ops = {
    .set_protocol = set_protocol,
    .set_shared_memory = set_shared_memory,
    .add_drive = add_drive,
    .add_read = add_read,
    .add_download = add_download,
    .add_upload = add_upload,
    .set_report = set_report,
    .run = run_test,
};

static struct report *
new_report()
{
    time_t t;
    struct report *report;

    report = malloc(sizeof(struct report));
    if (!report) {
        return NULL;
    }

    report->ops = &r_ops;
    report->path = NULL;
    report->name = NULL;
    report->entries = NULL;
    report->nb_entries = 0;
    report->overwrite = false;
}

static void
free_report(struct report *report)
{
    int i;

    for (i = 0; i < report->nb_entries; ++i) {
        free(report->entries[i]);
    }
}

static void
free_file(file_t *file)
{
    free(file->local_path);
    free(file->remote_path);
}

struct test_guestfs *
new_test_guestfs()
{
    struct test_guestfs *test;

    test = malloc(sizeof(struct test_guestfs));
    if (!test) {
        return NULL;
    }

    test->ops = &tg_ops;
    test->protocol = PROTOBUF;
    test->enable_shared_memory = false;
    test->drive = NULL;
    test->reads = NULL;
    test->nb_reads = 0;
    test->downloads = NULL;
    test->nb_downloads = 0;
    test->uploads = NULL;
    test->nb_uploads = 0;

    test->report = new_report();

    return test;
}

void
free_test_guestfs(struct test_guestfs *test)
{
    int i;

    for (i = 0; i < test->nb_reads; ++i) {
        free_file(test->reads[i]);
        free(test->reads[i]);
    }

    for (i = 0; i < test->nb_downloads; ++i) {
        free_file(test->downloads[i]);
        free(test->downloads[i]);
    }

    for (i = 0; i < test->nb_uploads; ++i) {
        free_file(test->uploads[i]);
        free(test->uploads[i]);
    }

    free_report(test->report);
}
