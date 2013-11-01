#ifndef _DEFAULTS_H
#define _DEFAULTS_H

// Below this, parfor's should execute sequentially
#ifndef PARFOR_THRESHOLD
#define PARFOR_THRESHOLD 8
#endif

// Below this, parforpar's should execute sequentially
#ifndef PARFORPAR_THRESHOLD
#define PARFORPAR_THRESHOLD 8
#endif

#ifndef BATCH_THRESHOLD
//#define BATCH_THRESHOLD USE_PARAMETER2(active_size)
#define BATCH_THRESHOLD 2 ///< Below this, batches should execute sequentially
#endif

/* #define DUMMY_WAIT 512 ///< Loop iteration to wait (to simulate operation time) */
/* #define DUMMY_ITERS 131072 ///< Number of times to execute operation */
/* #define BASE_SIZE 128 */
#endif
