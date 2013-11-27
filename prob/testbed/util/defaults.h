#ifndef _DEFAULTS_H
#define _DEFAULTS_H

// Below this, parfor's should execute sequentially
#ifndef PARFOR_THRESHOLD
#define PARFOR_THRESHOLD 1024
#endif

// Below this, parforpar's should execute sequentially
#ifndef PARFORPAR_THRESHOLD
#define PARFORPAR_THRESHOLD 1024
#endif

#ifndef BATCH_THRESHOLD
//#define BATCH_THRESHOLD USE_PARAMETER2(active_size)
#define BATCH_THRESHOLD 2 ///< Below this, batches should execute sequentially
#endif

#endif
