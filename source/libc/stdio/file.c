#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

FILE *stdin  = NULL;
FILE *stdout = NULL;
FILE *stderr = NULL;

static size_t file_count = 0;
static FILE **file_list  = NULL;

FILE *fdopen(int fd, const char *mode) {
    (void)mode;
    FILE *ret = calloc(1, sizeof(FILE));
    if (ret == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < file_count; i++) {
        if (file_list[i] == NULL) {
            file_list[i] = ret;
            goto slot_found;
        }
    }

    size_t mode_len = strlen(mode);
    for (size_t i = 0; i < mode_len; i++) {
        if (mode[i] == 'a' || mode[i] == 'w') {
            ret->is_writable = 1;
            break;
        }
    }

    size_t index = file_count;
    file_count   = file_count == 0 ? 32 : file_count * 2;

    FILE **tmp = realloc(file_list, file_count * sizeof(FILE*));
    if (tmp == NULL) {
        free(ret);
        return NULL;
    }
    file_list = tmp;
    file_list[index] = ret;
    for (size_t i = index + 1; i < file_count; i++) {
        file_list[i] = NULL;
    }

slot_found:
    ret->inner_fd = fd;
    return ret;
}

FILE *fopen(const char *restrict filename, const char *restrict mode) {
    int flags;

    if (strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0) {
        flags = O_RDONLY;
    } else if (strcmp(mode, "w") == 0 || strcmp(mode, "wb") == 0) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (strcmp(mode, "a") == 0 || strcmp(mode, "ab") == 0) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    } else if (strcmp(mode, "r+") == 0 || strcmp(mode, "rb+") == 0 || strcmp(mode, "r+b") == 0) {
        flags = O_RDWR;
    } else if (strcmp(mode, "w+") == 0 || strcmp(mode, "wb+") == 0 || strcmp(mode, "w+b") == 0) {
        flags = O_RDWR | O_CREAT | O_TRUNC;
    } else if (strcmp(mode, "a+") == 0 || strcmp(mode, "ab+") == 0 || strcmp(mode, "a+b") == 0) {
        flags = O_RDWR | O_CREAT | O_APPEND;
    } else {
        errno = EINVAL;
        return NULL;
    }

    int fd = open(filename, flags);
    if (fd == -1) {
        return NULL;
    }

    return fdopen(fd, mode);
}

int fclose(FILE *stream) {
    int ret = close(stream->inner_fd);
    for (size_t i = 0; i < file_count; i++) {
        if (stream == file_list[i]) {
            file_list[i] = NULL;
        }
    }

    free(stream);
    return ret;
}

int fputc(int character, FILE *stream) {
    int ret;

    stream->is_error = 0;
    stream->is_eof   = 0;

    if (stream->buffer_i >= FILE_BUFFER_SIZE) {
        if (fflush(stream)) {
            return EOF;
        }
        ret = read(stream->inner_fd, stream->buffer, FILE_BUFFER_SIZE);
        switch (ret) {
            case -1:
                stream->is_error = 1;
                return EOF;
        }
    }

    stream->bytes_in_buffer = ret;

    if (stream->buffer_i == stream->bytes_in_buffer) {
        stream->bytes_in_buffer++;
    }
    stream->buffer[stream->buffer_i++] = character;

    return character;
}

int fgetc(FILE *stream) {
    int ret;

    stream->is_error = 0;
    stream->is_eof   = 0;

    if (stream->buffer_i == stream->bytes_in_buffer) {
        if (fflush(stream)) {
            return EOF;
        }
        ret = read(stream->inner_fd, stream->buffer, FILE_BUFFER_SIZE);
        switch (ret) {
            case -1:
                stream->is_error = 1;
                return EOF;
            case 0:
                stream->is_eof = 1;
                return EOF;
        }
    }

    stream->bytes_in_buffer = ret;
    return stream->buffer[stream->buffer_i++];
}

int fflush(FILE *stream) {
    if (stream == NULL) {
        for (size_t i = 0; i < file_count; i++) {
            if (file_list[i] != NULL) {
                fflush(file_list[i]);
            }
        }
        return 0;
    }

    stream->is_error = 0;
    stream->is_eof   = 0;

    if (stream->is_writable) {
        lseek(stream->inner_fd, -stream->bytes_in_buffer, SEEK_CUR);
        int ret = write(stream->inner_fd, stream->buffer, stream->buffer_i);

        stream->buffer_i        = 0;
        stream->bytes_in_buffer = 0;

        if (ret == -1) {
            stream->is_error = 1;
            return EOF;
        }
    } else {
        lseek(stream->inner_fd, -(stream->bytes_in_buffer - stream->buffer_i), SEEK_CUR);
        stream->buffer_i        = 0;
        stream->bytes_in_buffer = 0;
    }

    return 0;    
}
