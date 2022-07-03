

#ifndef __GLOBAL_CONFIGS_HPP
#define __GLOBAL_CONFIGS_HPP 1000

#define F_CPU			16000000UL


#define FF_FS_READONLY	0
#define FF_FS_MINIMIZE	0
#define FF_USE_STRFUNC	1
#define FF_USE_FIND		0
#define FF_USE_MKFS		0
#define FF_USE_FASTSEEK	0
#define FF_USE_EXPAND	0
#define FF_USE_CHMOD	0
#define FF_USE_LABEL	0
#define FF_USE_FORWARD	0
#define FF_FS_RPATH		0
#define FF_VOLUMES		1
#define FF_STR_VOLUME_ID	0
#define FF_VOLUME_STRS		"RAM","NAND","CF","SD","SD2","USB","USB2","USB3"
#define FF_MULTI_PARTITION	0
#define FF_MIN_SS		512
#define FF_MAX_SS		512
#define FF_USE_TRIM		0
#define FF_FS_NOFSINFO	0
#define FF_FS_TINY		1
#define FF_FS_EXFAT		0
#define FF_FS_NORTC		0
#define FF_NORTC_MON	1
#define FF_NORTC_MDAY	1
#define FF_NORTC_YEAR	2017
#define FF_FS_LOCK		0
#define FF_FS_REENTRANT	0
#define FF_FS_TIMEOUT	1000
#define FF_SYNC_t		HANDLE
#define FF_CODE_PAGE	860
#define FF_USE_LFN		0
#define FF_MAX_LFN		255
#define FF_LFN_UNICODE	0
#define FF_LFN_BUF		255
#define FF_SFN_BUF		12
#define FF_STRF_ENCODE	3

#endif
