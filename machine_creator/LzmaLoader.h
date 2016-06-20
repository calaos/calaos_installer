#ifndef LZMALOADER_WIN_H
#define LZMALOADER_WIN_H

#include <QLibrary>

#define LZMA LzmaLoader::Instance()

typedef int lzma_ret;
#define LZMA_OK 0

typedef int lzma_reserved_enum;

typedef int lzma_action;
#define LZMA_RUN 0

#define LZMA_TELL_UNSUPPORTED_CHECK UINT32_C(0x02)
#define LZMA_CONCATENATED UINT32_C(0x08)

typedef struct {
    void *(*alloc)(void *opaque, size_t nmemb, size_t size);
    void (*free)(void *opaque, void *ptr);
    void *opaque;

} lzma_allocator;

typedef struct lzma_internal_s lzma_internal;

typedef struct {
    const uint8_t *next_in; /**< Pointer to the next input byte. */
    size_t avail_in;    /**< Number of available input bytes in next_in. */
    uint64_t total_in;  /**< Total number of bytes read by liblzma. */

    uint8_t *next_out;  /**< Pointer to the next output position. */
    size_t avail_out;   /**< Amount of free space in next_out. */
    uint64_t total_out; /**< Total number of bytes written by liblzma. */

    const lzma_allocator *allocator;

    /** Internal state is not visible to applications. */
    lzma_internal *internal;

    void *reserved_ptr1;
    void *reserved_ptr2;
    void *reserved_ptr3;
    void *reserved_ptr4;
    uint64_t reserved_int1;
    uint64_t reserved_int2;
    size_t reserved_int3;
    size_t reserved_int4;
    lzma_reserved_enum reserved_enum1;
    lzma_reserved_enum reserved_enum2;

} lzma_stream;

class LzmaLoader
{
private:
    QLibrary *lib = nullptr;
    LzmaLoader();

public:
    static LzmaLoader &Instance()
    {
        static LzmaLoader inst;
        return inst;
    }

    ~LzmaLoader();

    bool load();

    lzma_ret lzma_stream_decoder(lzma_stream *strm, uint64_t memlimit, uint32_t flags);
    lzma_ret lzma_code(lzma_stream *strm, lzma_action action);
};

#endif // LZMALOADER_WIN_H
