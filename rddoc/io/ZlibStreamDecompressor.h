/*
 * Copyright (C) 2017, Yeolar
 */

#pragma once

#include <memory>
#include <zlib.h>
#include "rddoc/io/IOBuf.h"

namespace rdd {

extern int64_t FLAGS_zlib_decompresser_buffer_growth;
extern int64_t FLAGS_zlib_decompresser_buffer_minsize;

enum class ZlibCompressionType: int {
  NONE = 0,
  DEFLATE = 15,
  GZIP = 31
};

class ZlibStreamDecompressor {
 public:
  explicit ZlibStreamDecompressor(ZlibCompressionType type);

  ZlibStreamDecompressor() { }

  ~ZlibStreamDecompressor();

  void init(ZlibCompressionType type);

  std::unique_ptr<rdd::IOBuf> decompress(const rdd::IOBuf* in);

  int getStatus() { return status_; }

  bool hasError() { return status_ != Z_OK && status_ != Z_STREAM_END; }

  bool finished() { return status_ == Z_STREAM_END; }

 private:
  ZlibCompressionType type_{ZlibCompressionType::NONE};
  z_stream zlibStream_;
  int status_{-1};
};

}
