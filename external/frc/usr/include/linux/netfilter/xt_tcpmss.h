#ifndef _XT_TCPMSS_MATCH_H
#define _XT_TCPMSS_MATCH_H

#include <linux/types.h>

struct xt_tcpmss_match_info {
    __u16 mss_min, mss_max;
    __u8 invert;
};

struct xt_tcpmss_info {
	__u16 mss;
};

#define XT_TCPMSS_CLAMP_PMTU 0xffff

#endif /*_XT_TCPMSS_MATCH_H*/
