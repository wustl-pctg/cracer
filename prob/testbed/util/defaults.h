#ifndef _DEFAULTS_H
#define _DEFAULTS_H

#ifndef PARFOR_THRESHOLD
#define PARFOR_THRESHOLD 32
#endif

#ifndef PARFORPAR_THRESHOLD
#define PARFORPAR_THRESHOLD 32
#endif

#ifndef BATCH_THRESHOLD
#define BATCH_THRESHOLD USE_PARAMETER2(active_size)
#endif

#define DUMMY_WAIT 512 ///< Loop iteration to wait (to simulate operation time)
#define DUMMY_ITERS 131072 ///< Number of times to execute operation
#define BASE_SIZE 128
#endif
