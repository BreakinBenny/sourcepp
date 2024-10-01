#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	VPKPP_PACK_FILE_TYPE_UNKNOWN,
	VPKPP_PACK_FILE_TYPE_BSP,
	VPKPP_PACK_FILE_TYPE_FPX,
	VPKPP_PACK_FILE_TYPE_GCF,
	VPKPP_PACK_FILE_TYPE_GMA,
	VPKPP_PACK_FILE_TYPE_PAK,
	VPKPP_PACK_FILE_TYPE_PCK,
	VPKPP_PACK_FILE_TYPE_VPK,
	VPKPP_PACK_FILE_TYPE_VPK_VTMB,
	VPKPP_PACK_FILE_TYPE_WAD3,
	VPKPP_PACK_FILE_TYPE_ZIP,
} vpkpp_pack_file_type_e;

#ifdef __cplusplus
} // extern "C"
#endif
