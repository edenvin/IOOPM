#include "priv_imalloc.h"


/*
 * Converts a priv_mem pointer to a style pointer.
 */
style* priv_to_style(priv_mem* mem) {
  return (style*) ((void*) ((Chunk) mem + 2) + 2);
}

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
priv_mem* style_to_priv(style* mem) {
  return (priv_mem*) ((void*) ((Chunk) mem - 2) - 2);
}

/*
 * Returns the freelist in mem.
 */
Chunk freelist(priv_mem* mem) {
  return mem->freelist;
}

/*
 * Returns the alloclist in mem.
 */
Chunk alloclist(priv_mem* mem) {
  return mem->alloclist;
}
