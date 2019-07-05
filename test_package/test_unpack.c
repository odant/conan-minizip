/*
 * Tests for minizip Conan package
 * Unpack files
 * Dmitriy Vetutnev, Odant 2019
*/


#include <mz.h>
#include <mz_strm.h>
#include <mz_zip.h>
#include <mz_zip_rw.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#endif

#ifdef __unix__
#include <sys/types.h>
#include <sys/stat.h>
#endif


int32_t minizip_extract_entry_cb(void* handle, void* userdata, mz_zip_file* file_info, const char* path) {
    MZ_UNUSED(handle);
    MZ_UNUSED(userdata);
    MZ_UNUSED(path);

    printf("minizip_extract_entry_cb: ");
    /* Print the current entry extracting */
    printf("Extracting %s\n", file_info->filename);
    return MZ_OK;
}

int32_t minizip_extract_progress_cb(void* handle, void* userdata, mz_zip_file* file_info, int64_t position) {
    MZ_UNUSED(userdata);

    double progress = 0;
    uint8_t raw = 0;

    mz_zip_reader_get_raw(handle, &raw);

    if (raw && file_info->compressed_size > 0) {
        progress = ((double)position / file_info->compressed_size) * 100;
    } else if (!raw && file_info->uncompressed_size > 0) {
        progress = ((double)position / file_info->uncompressed_size) * 100;
    }

    printf("minizip_extract_progress_cb: ");
    /* Print the progress of the current extraction */
    printf("%s - %"PRId64" / %"PRId64" (%.02f%%)\n", file_info->filename, position, file_info->uncompressed_size, progress);
    return MZ_OK;
}

int32_t minizip_extract_overwrite_cb(void* handle, void* userdata, mz_zip_file* file_info, const char* path) {
    MZ_UNUSED(handle);
    MZ_UNUSED(userdata);
    MZ_UNUSED(file_info);

    printf("minizip_extract_overwrite_cb: Overwrite file %s", path);
    return MZ_OK;
}


int main(void) {
/* Prepare */
    int res = 0;

#ifdef _WIN32
    res = _mkdir("output_folder");
#endif
#ifdef __unix__
    res = mkdir("output_folder", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
    if (res != 0 && res != EEXIST) {
        printf("Can`t create folder 'output_folder'!\n");
        return EXIT_FAILURE;
    }

/* Unpack */
    void* reader = NULL;
    int32_t err = 0;

    mz_zip_reader_create(&reader);
    mz_zip_reader_set_entry_cb(reader, NULL, minizip_extract_entry_cb);
    mz_zip_reader_set_progress_cb(reader, NULL, minizip_extract_progress_cb);
    mz_zip_reader_set_overwrite_cb(reader, NULL, minizip_extract_overwrite_cb);

    err = mz_zip_reader_open_file(reader, "archive.zip");
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_reader_open_file()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    err = mz_zip_reader_save_all(reader, "output_folder");
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_reader_save_all()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    err = mz_zip_reader_close(reader);
    if (err != MZ_OK) {
        printf("Failed 'mz_zip_reader_close()', err: %d\n", err);
        return EXIT_FAILURE;
    }
    mz_zip_reader_delete(&reader);

    printf("Unpack OK, check content...\n");

/* Check content */
    FILE* fp = NULL;
    size_t file_size = 0;
    char* in_buffer = NULL;
    char* out_buffer = NULL;

    fp = fopen("input_folder/dir/file_a.txt", "rb");
    if (fp == NULL) {
        printf("Can`t open 'input_folder/dir/file_a.txt' for read!\n");
        return EXIT_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    in_buffer = malloc(file_size + 1);
    fread(in_buffer, sizeof(char), file_size, fp);
    in_buffer[file_size] = 0;
    fclose(fp);
    fp = fopen("output_folder/dir/file_a.txt", "rb");
    if (fp == NULL) {
        printf("Can`t open 'output_folder/dir/file_a.txt' for read!\n");
        return EXIT_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    out_buffer = malloc(file_size + 1);
    fread(out_buffer, sizeof(char), file_size, fp);
    out_buffer[file_size] = 0;
    fclose(fp);
    if (strcmp(in_buffer, out_buffer) != 0) {
        printf("'input_folder/dir/file_a.txt' not equal 'output_folder/dir/file_a.txt'\n");
        return EXIT_FAILURE;
    }
    free(in_buffer);
    free(out_buffer);

    fp = fopen("input_folder/dir/file_b.txt", "rb");
    if (fp == NULL) {
        printf("Can`t open 'input_folder/dir/file_b.txt' for read!\n");
        return EXIT_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    in_buffer = malloc(file_size + 1);
    fread(in_buffer, sizeof(char), file_size, fp);
    in_buffer[file_size] = 0;
    fclose(fp);
    fp = fopen("output_folder/dir/file_b.txt", "rb");
    if (fp == NULL) {
        printf("Can`t open 'output_folder/dir/file_b.txt' for read!\n");
        return EXIT_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    out_buffer = malloc(file_size + 1);
    fread(out_buffer, sizeof(char), file_size, fp);
    out_buffer[file_size] = 0;
    fclose(fp);
    if (strcmp(in_buffer, out_buffer) != 0) {
        printf("'input_folder/dir/file_b.txt' not equal 'output_folder/dir/file_b.txt'\n");
        return EXIT_FAILURE;
    }
    free(in_buffer);
    free(out_buffer);

    fp = fopen("input_folder/file_c.txt", "rb");
    if (fp == NULL) {
        printf("Can`t open 'input_folder/file_c.txt' for read!\n");
        return EXIT_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    in_buffer = malloc(file_size + 1);
    fread(in_buffer, sizeof(char), file_size, fp);
    in_buffer[file_size] = 0;
    fclose(fp);
    fp = fopen("output_folder/file_c.txt", "rb");
    if (fp == NULL) {
        printf("Can`t open 'output_folder/file_c.txt' for read!\n");
        return EXIT_FAILURE;
    }
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    out_buffer = malloc(file_size + 1);
    fread(out_buffer, sizeof(char), file_size, fp);
    out_buffer[file_size] = 0;
    fclose(fp);
    if (strcmp(in_buffer, out_buffer) != 0) {
        printf("'input_folder/file_c.txt' not equal 'output_folder/file_c.txt'\n");
        return EXIT_FAILURE;
    }
    free(in_buffer);
    free(out_buffer);

    printf("Content equal, OK\n");
    return EXIT_SUCCESS;
}

