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

slot_found:;
    size_t mode_len = strlen(mode);
    for (size_t i = 0; i < mode_len; i++) {
        switch (mode[i]) {
            case 'r': case '+':
                ret->is_readable = 1;
                break;
            case 'a': case 'w':
                ret->is_writable = 1;
                break;
        }
    }

    ret->inner_fd = fd;
    return ret;
}

FILE *fopen(const char *filename, const char *mode) {
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
    stream->is_error = 0;
    stream->is_eof   = 0;

    if (!stream->is_writable) {
        stream->is_error = 1;
        return EOF;
    }

    if (stream->buffer_i == stream->bytes_in_buffer) {
        stream->bytes_in_buffer++;
    }
    stream->buffer[stream->buffer_i++] = character;

    if (stream->buffer_i >= FILE_BUFFER_SIZE || character == '\n') {
        if (fflush(stream)) {
            return EOF;
        }
        off_t lseek_ret = lseek(stream->inner_fd, 0, SEEK_CUR);
        if (lseek_ret == (off_t)-1) {
            stream->buffer_base = 0;
        } else {
            stream->buffer_base = lseek_ret;
        }
        if (stream->is_readable) {
            int ret = read(stream->inner_fd, stream->buffer, FILE_BUFFER_SIZE);
            switch (ret) {
                case -1:
                    stream->is_error = 1;
                    return EOF;
            }
            stream->bytes_in_buffer = ret;
        }
    }

    return character;
}

int fgetc(FILE *stream) {
    stream->is_error = 0;
    stream->is_eof   = 0;

    if (!stream->is_readable) {
        stream->is_error = 1;
        return EOF;
    }

    int ret = stream->buffer[stream->buffer_i++];

    if (stream->buffer_i == stream->bytes_in_buffer) {
        if (fflush(stream)) {
            return EOF;
        }
        off_t lseek_ret = lseek(stream->inner_fd, 0, SEEK_CUR);
        if (lseek_ret == (off_t)-1) {
            stream->buffer_base = 0;
        } else {
            stream->buffer_base = lseek_ret;
        }
        int ret = read(stream->inner_fd, stream->buffer, FILE_BUFFER_SIZE);
        switch (ret) {
            case -1:
                stream->is_error = 1;
                return EOF;
            case 0:
                stream->is_eof = 1;
                return EOF;
        }
        stream->bytes_in_buffer = ret;
    }

    return ret;
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
        lseek(stream->inner_fd, stream->buffer_base, SEEK_SET);
        int ret = write(stream->inner_fd, stream->buffer, stream->buffer_i);

        stream->buffer_i        = 0;
        stream->bytes_in_buffer = 0;

        if (ret == -1) {
            stream->is_error = 1;
            return EOF;
        }
    } else {
        lseek(stream->inner_fd, stream->buffer_base + stream->buffer_i, SEEK_SET);
        stream->buffer_i        = 0;
        stream->bytes_in_buffer = 0;
    }

    return 0;    
}

int fseek(FILE *stream, long offset, int where) {
    fflush(stream);
    off_t ret = lseek(stream->inner_fd, offset, where);
    if (ret == (off_t)-1) {
        return -1;
    }
    return 0;
}

long ftell(FILE *stream) {
    return stream->buffer_base + stream->buffer_i;
}

int fputs(const char *str, FILE *stream) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (fputc(str[i], stream) == EOF) {
            return EOF;
        }
    }
    return 0;
}

char *fgets(char *result, int count, FILE *stream) {
    char *ret = result;
    int i;
    for (i = 0; i < count - 1; i++) {
        int c = fgetc(stream);
        if (c == EOF) {
            ret = NULL;
            break;
        }
        result[i] = c;
        if (c == '\n') {
            break;
        }
    }
    result[++i] = '\0';
    return ret;
}

size_t fwrite(const void *pointer, size_t size, size_t nitems, FILE *stream) {
    size_t         pointer_i = 0;
    const uint8_t *data      = pointer;
    for (size_t i = 0; i < nitems; i++) {
        for (size_t j = 0; j < size; j++) {
            if (fputc(data[pointer_i++], stream) == EOF) {
                return i;
            }
        }
    }
    return nitems;
}

size_t fread(void *pointer, size_t size, size_t nitems, FILE *stream) {
    size_t   pointer_i = 0;
    uint8_t *data      = pointer;
    for (size_t i = 0; i < nitems; i++) {
        for (size_t j = 0; j < size; j++) {
            int c = fgetc(stream);
            if (c == EOF) {
                return i;
            }
            data[pointer_i++] = c;
        }
    }
    return nitems;
}

int feof(FILE *stream) {
    return stream->is_eof;
}

int ferror(FILE *stream) {
    return stream->is_error;
}

int fileno(FILE *stream) {
    return stream->inner_fd;
}

int putchar(int character) {
    return fputc(character, stdout);
}

int getchar(void) {
    return fgetc(stdin);
}

int puts(const char *str) {
    if (fputs(str, stdout) == EOF || fputc('\n', stdout) == EOF) {
        return EOF;
    }

    return 0;
}

int remove(const char *pathname) {
    int ret = unlink(pathname);
    return (ret && errno == EISDIR) ? rmdir(pathname) : ret;
}

// TODO: Put ourselves thru the pain and suffering of actually implementing
// scanf.
int sscanf(const char *str, const char *format, ...) {
    (void)str;
    (void)format;
    puts("sscanf is a stub");
    exit(EXIT_FAILURE);
    return EOF;
}
