/*
 * Prob.h -- Probabilities and stuff
 */

#pragma once

#include <math.h>
#include <limits.h>
#include <assert.h>

namespace srilm {

#ifndef M_E
#define M_E 2.7182818284590452354
#endif
#ifndef M_LN10
#define M_LN10 2.30258509299404568402
#endif

/*
 * Types
 */
typedef float LogP;   /* A log-base-10 probability */
typedef double LogP2; /* A log-base-10 probability, double-size */
typedef double Prob;  /* a straight probability */

/*
 * Constants
 */
extern const LogP LogP_Zero; /* log(0) = -Infinity */
extern const LogP LogP_Inf;  /* log(Inf) = Infinity */
extern const LogP LogP_One;  /* log(1) = 0 */

extern const unsigned LogP_Precision;  /* number of significant decimals
                                        * in a LogP */
extern const unsigned LogP2_Precision; /* no of signif digits in LogP2 */
extern const unsigned Prob_Precision;  /* no of signif digits in Prob */
extern const Prob Prob_Epsilon;        /* probability sum considered not
                                        * significantly different from 0 */

bool parseProb(const char *string, Prob &prob);

/*
 * Convenience functions for handling LogPs
 *  Many of these are time critical, so we use inline definitions.
 */

bool parseLogP(const char *string, LogP2 &prob);

inline bool parseLogP(const char *string, LogP &prob) {
  LogP2 p2;
  if (parseLogP(string, p2)) {
    prob = (LogP)p2;
    return true;
  } else {
    return false;
  }
}

inline bool parseProbOrLogP(const char *string, Prob &prob, bool useLog) {
  if (useLog) {
    LogP logp;
    if (parseLogP(string, logp)) {
      prob = logp;
      return true;
    } else {
      return false;
    }
  } else {
    return parseProb(string, prob);
  }
}

inline Prob LogPtoProb(LogP2 prob) {
  if (prob == LogP_Zero) {
    return 0;
  } else {
    return exp(prob * M_LN10);
  }
}

inline Prob LogPtoPPL(LogP2 prob) { return exp(-prob * M_LN10); }

inline LogP ProbToLogP(Prob prob) { return (LogP)log10(prob); }

inline LogP2 MixLogP(LogP2 prob1, LogP2 prob2, double lambda) {
  return ProbToLogP(lambda * LogPtoProb(prob1) +
                    (1 - lambda) * LogPtoProb(prob2));
}

inline LogP2 AddLogP(LogP2 x, LogP2 y) {
  if (x < y) {
    LogP2 temp = x;
    x = y;
    y = temp;
  }
  if (y == LogP_Zero) {
    return x;
  } else {
    LogP2 diff = y - x;
    return x + log10(1.0 + exp(diff * M_LN10));
  }
}

inline LogP2 SubLogP(LogP2 x, LogP2 y) {
  assert(x >= y);
  if (x == y) {
    return LogP_Zero;
  } else if (y == LogP_Zero) {
    return x;
  } else {
    LogP2 diff = y - x;
    return x + log10(1.0 - exp(diff * M_LN10));
  }
}

inline LogP2 weightLogP(double weight, LogP2 prob) {
  /*
   * avoid NaN if weight == 0 && prob == -Infinity
   */
  if (weight == 0.0) {
    return 0.0;
  } else {
    return weight * prob;
  }
}

/*
 * Bytelogs and Intlogs are scaled log probabilities used in the SRI
 * DECIPHER(TM) recognizer.
 * Note: DECIPHER actually flips the sign on bytelogs, we keep them negative.
 */

typedef int Bytelog;
typedef int Intlog;

inline Bytelog ProbToBytelog(Prob prob) {
  return (int)rint(log(prob) * (10000.5 / 1024.0));
}

inline Intlog ProbToIntlog(Prob prob) {
  int intlog = (int)rint(log(prob) * 10000.5);

  /* check for int over/underflow */
  if (intlog > 0 && prob < 0.0) {
    return INT_MIN;
  } else if (intlog < 0 && prob > 0.0) {
    return INT_MAX;
  } else {
    return intlog;
  }
}

inline Bytelog LogPtoBytelog(LogP prob) {
  int bytelog = (int)rint(prob * (M_LN10 * 10000.5 / 1024.0));

  /* check for int over/underflow */
  if (bytelog > 0 && prob < 0.0) {
    return INT_MIN;
  } else if (bytelog < 0 && prob > 0.0) {
    return INT_MAX;
  } else {
    return bytelog;
  }
}

inline Intlog LogPtoIntlog(LogP prob) {
  int intlog = (int)rint(prob * (M_LN10 * 10000.5));

  /* check for int over/underflow */
  if (intlog > 0 && prob < 0.0) {
    return INT_MIN;
  } else if (intlog < 0 && prob > 0.0) {
    return INT_MAX;
  } else {
    return intlog;
  }
}

inline LogP IntlogToLogP(double prob) /* use double argument to avoid loss
                                       * of information when converting from
                                       * floating point values */
{
  return (LogP)(prob / (M_LN10 * 10000.5));
}

inline LogP BytelogToLogP(double bytelog) /* use double argument so we can
                                         * scale float values without loss of
                                         * precision */
{
  return (LogP)(bytelog * (1024.0 / 10000.5 / M_LN10));
}

const int BytelogShift = 10;

inline Bytelog IntlogToBytelog(Intlog intlog) {
  int bytelog = ((-intlog) + (1 << (BytelogShift - 1))) >> BytelogShift;

  if (bytelog > 255) {
    bytelog = 255;
  }
  return -bytelog;
}

inline Intlog BytelogToIntlog(Bytelog bytelog) {
  return bytelog << BytelogShift;
}

}

