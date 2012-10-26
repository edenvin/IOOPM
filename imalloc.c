#include "imalloc.h"
#include "priv_imalloc.h"

struct style *iMalloc(chunk_size memsiz, unsigned int flags) {
  return priv_imalloc(memsiz, flags);
}
