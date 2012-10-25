#include "priv_imalloc.h"


/*
 * Converts a priv_mem pointer to a style pointer.
 */
style* priv_to_style(priv_mem* mem) {
  return (style*) mem+(2 * sizeof(void*) + 2 * sizeof(Chunk));
}

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
priv_mem* style_to_priv(style* mem) {
  return (priv_mem*) mem-(2 * sizeof(void*) + 2 * sizeof(Chunk));
}

/*
 * Returns the freelist in mem.
 */
Chunk freelist(priv_mem* mem) {
  return memory_freelist(mem->lists);
}

/*
 * Returns the alloclist in mem.
 */
Chunk alloclist(priv_mem* mem) {
  return memory_alloclist(mem->lists);
}
