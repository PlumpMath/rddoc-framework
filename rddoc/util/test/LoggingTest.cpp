/*
 * Copyright (C) 2017, Yeolar
 */

#include "rddoc/util/Logging.h"
#include <gtest/gtest.h>

using namespace rdd;
using namespace rdd::logging;

TEST(RDDLog, log) {
  RDDLOG(DEBUG) << "debug log default output to stderr";
  RDDLOG(INFO) << "info log default output to stderr";
  RDDLOG(WARN) << "warning log default output to stderr";
  RDDLOG(ERROR) << "error log default output to stderr";
}

TEST(RDDLog, plog) {
  int saved_errno = errno;
  errno = EAGAIN;
  RDDPLOG(WARN) << "EAGAIN warning log";
  errno = saved_errno;
}

TEST(RDDLog, setLevel) {
  Singleton<RDDLogger>::get()->setLevel(logging::LOG_INFO);
  RDDLOG(DEBUG) << "debug log SHOULD NOT BE SEEN";
  RDDLOG(INFO) << "info log available";
  Singleton<RDDLogger>::get()->setLevel(logging::LOG_DEBUG);
  RDDLOG(DEBUG) << "debug log available";
  RDDLOG(INFO) << "info log available";
}

TEST(RDDLog, async) {
  Singleton<RDDLogger>::get()->setLogFile("log.txt");
  RDDLOG(INFO) << "async info log to file";
}
