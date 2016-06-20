#include "LzmaLoader.h"
#include <QtDebug>

typedef lzma_ret (*_lzma_stream_decoder)(lzma_stream *strm, uint64_t memlimit, uint32_t flags);
typedef lzma_ret (*_lzma_code)(lzma_stream *strm, lzma_action action);

#define DEFINE_FN(fnc) \
    static _##fnc func_##fnc = NULL

DEFINE_FN(lzma_stream_decoder);
DEFINE_FN(lzma_code);

LzmaLoader::LzmaLoader()
{
}

LzmaLoader::~LzmaLoader()
{
    delete lib;
}

bool LzmaLoader::load()
{
    if (!lib)
        lib = new QLibrary("liblzma");

    if (lib->isLoaded())
        return true;

    if (!lib->load())
    {
        qDebug() << lib->errorString() ;
        return false;
    }

#define RESOLVE_FN(fnc) \
    func_##fnc = (_##fnc)lib->resolve(#fnc); \
    if (!func_##fnc) qDebug() << "Unable to resolve function "#fnc;

    RESOLVE_FN(lzma_stream_decoder);
    RESOLVE_FN(lzma_code);

    return true;
}

#define IMPLEMENT_FN_RET(fnc, fmt, ...) \
    if (!func_##fnc) { qDebug() << "Function "#fnc" not resolved in library"; return 0; } \
    return func_##fnc(fmt, ##__VA_ARGS__);

#define IMPLEMENT_FN(fnc, fmt, ...) \
    if (!func_##fnc) { qDebug() << "Function "#fnc" not resolved in library"; return; } \
    func_##fnc(fmt, ##__VA_ARGS__);

lzma_ret LzmaLoader::lzma_stream_decoder(lzma_stream *strm, uint64_t memlimit, uint32_t flags)
{
    IMPLEMENT_FN_RET(lzma_stream_decoder, strm, memlimit, flags);
}

lzma_ret LzmaLoader::lzma_code(lzma_stream *strm, lzma_action action)
{
    IMPLEMENT_FN_RET(lzma_code, strm, action);
}
