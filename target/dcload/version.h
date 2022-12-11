#ifndef __VERSION_H__
#define __VERSION_H__

// Overriden by Meson Defines

#ifndef DCLOAD_VERSION
#define DCLOAD_VERSION     "UNKOWN VERSION"
#endif

#ifndef DCLOAD_GITREPO
#define DCLOAD_GITREPO     "UNKNOWN REPO"
#endif

#ifndef DCLOAD_GITBRANCH
#define DCLOAD_GITBRANCH   "UNKNOWN BRANCH"
#endif

#ifndef DCLOAD_GITSHA
#define DCLOAD_GITSHA      "UNKNOWN SHA"
#endif

#ifndef DCLOAD_DATETIME
#define DCLOAD_DATETIME   __DATE__ " " __TIME__
#endif

#ifndef DREAMCAST_IP
#define DREAMCAST_IP       "0.0.0.0"
#endif

#endif 