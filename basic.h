#ifndef BASIC_H
#define BASIC_H

#ifdef _UNICODE
#define PRIsLPTSTR "ls"
#define PRIsLPOLESTR  "ls"
#else
#define PRIsLPTSTR "s"
#define PRIsLPOLESTR  "ls"
#endif

#ifndef _In_
#define _In_
#endif
#ifndef _In_opt_
#define _In_opt_
#endif
#ifndef _In_z_
#define _In_z_
#endif
#ifndef _Inout_
#define _Inout_
#endif
#ifndef _Out_
#define _Out_
#endif
#ifndef _Out_opt_
#define _Out_opt_
#endif
#ifndef _Out_z_cap_
#define _Out_z_cap_(n)
#endif

#endif /* ifndef BASIC_H */
