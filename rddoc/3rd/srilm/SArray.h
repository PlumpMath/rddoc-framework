/*
 * SArray.h -- Maps based based on sorted arrays.
 *
 * SArray<KeyT,DataT> implements Map<KeyT,DataT> using a single, sorted
 * array of key-value pairs.  This is very space-efficient and lookups
 * take logarithmic time.  However, insertion and deletions are linear
 * in the size of the array.
 *
 * SArrayIter<KeyT,DataT> implements iteration over the entries of the
 * Map object.
 */

#pragma once

#include "Map.h"

namespace srilm {

template <class KeyT, class DataT>
class SArray;  // forward declaration
template <class KeyT, class DataT>
class SArrayIter;  // forward declaration

template <class KeyT, class DataT>
class SArrayBody {
  friend class SArray<KeyT, DataT>;
  friend class SArrayIter<KeyT, DataT>;

  unsigned deleted : 1;     /* signals deletions to iterator */
  unsigned maxEntries : 31; /* total allocated entries */

  MapEntry<KeyT, DataT> data[1]; /* sorted array of key-value pairs */
};

template <class KeyT, class DataT>
class SArray
#ifdef USE_VIRTUAL
    : public Map<KeyT, DataT>
#endif
      {
  friend class SArrayIter<KeyT, DataT>;

public:
  SArray(unsigned size = 0);
  ~SArray();

  SArray(const SArray<KeyT, DataT> &source);
  SArray<KeyT, DataT> &operator=(const SArray<KeyT, DataT> &source);

  DataT *find(KeyT key, bool &foundP) const;
  inline DataT *find(KeyT key) const {
    bool found;
    return find(key, found);
  }
  KeyT getInternalKey(KeyT key, bool &foundP) const;
  inline KeyT getInternalKey(KeyT key) const {
    bool found;
    return getInternalKey(key, found);
  }
  DataT *insert(KeyT key, bool &foundP);
  inline DataT *insert(KeyT key) {
    bool found;
    return insert(key, found);
  }
  bool remove(KeyT key, DataT *removedData = 0);
  void clear(unsigned size = 0);
  void setsize(unsigned size = 0);
  unsigned numEntries() const;

protected:
  void *body;                /* handle to the above -- this keeps
                              * the size of an empty table to
                              * one word */
  void alloc(unsigned size); /* allocate data array */
  bool locate(KeyT key, unsigned &index) const;
  /* locate key in data */
private:
  static KeyT zeroKey;
};

template <class KeyT, class DataT>
class SArrayIter {
  typedef int (*compFnType)(KeyT, KeyT);

public:
  SArrayIter(const SArray<KeyT, DataT> &sarray, int (*sort)(KeyT, KeyT) = 0);
  SArrayIter(const SArrayIter<KeyT, DataT> &iter);
  ~SArrayIter();

  void init();
  DataT *next(KeyT &key);
  bool operator()(const unsigned idx1, const unsigned idx2);
  /* callback function for sort() */

private:
  SArrayBody<KeyT, DataT> *mySArrayBody;
  /* map data being iterated over */
  unsigned current;                /* current index into data
                                    * or sortedIndex */
  unsigned numEntries;             /* number of entries */
  int (*sortFunction)(KeyT, KeyT); /* key sorting function,
                                    * or 0 for random order */
  KeyT *sortedKeys;                /* array of sorted keys */
  void sortKeys();                 /* initialize sortedKeys */
};

/*
 * Key Comparison functions
 */
template <class KeyT>
inline int SArray_compareKey(KeyT key1, KeyT key2) {
  return (key1 - key2);
}

inline int SArray_compareKey(float key1, float key2) {
  if (key1 < key2)
    return -1;
  else if (key1 > key2)
    return 1;
  else
    return 0;
}

inline int SArray_compareKey(double key1, double key2) {
  if (key1 < key2)
    return -1;
  else if (key1 > key2)
    return 1;
  else
    return 0;
}

inline int SArray_compareKey(const char *key1, const char *key2) {
  if (key1 == 0) {
    if (key2 == 0) {
      return 0;
    } else {
      return -1;
    }
  } else {
    if (key2 == 0) {
      return +1;
    } else {
      return strcmp(key1, key2);
    }
  }
}

}

#include "SArray-inl.h"

