/* Define hwaddr if it exists.  */

#ifndef _QB_EXEC_HWADDR__H
#define _QB_EXEC_HWADDR__H 1

#define HWADDR_BITS 64
/* hwaddr is the type of a physical address (its size can
   be different from 'target_ulong').  */

typedef uint64_t hwaddr;
#define HWADDR_MAX UINT64_MAX
#define TARGET_FMT_plx "%016" PRIx64
#define HWADDR_PRId PRId64
#define HWADDR_PRIi PRIi64
#define HWADDR_PRIo PRIo64
#define HWADDR_PRIu PRIu64
#define HWADDR_PRIx PRIx64
#define HWADDR_PRIX PRIX64

#endif
/* _QB_EXEC_HWADDR__H */
