#include "utilities.h"

style* priv_to_style(priv_mem* mem) {
  return (style*) mem+(2 * sizeof(void*) + 2 * sizeof(Chunk))
}

priv_mem* style_to_priv(style* mem) {
  return (priv_mem*) mem-(2 * sizeof(void*) + 2 * sizeof(Chunk));
}

Chunk freelist(priv_mem* mem) {
  return mem->freelist;
}

Chunk alloclist(priv_mem* mem) {
  return mem->alloclist;
}