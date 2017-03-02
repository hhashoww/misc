#ifndef TYPES_H
#define TYPES_H

typedef		unsigned char			boolean_t;
/*
typedef		unsigned char			uint8_t;
typedef		unsigned short			uint16_t;
typedef		unsigned int			uint32_t;
typedef		unsigned long long		uint64_t;
typedef		char					int8_t;
typedef		short					int16_t;
typedef		int						int32_t;
typedef		long long				int64_t;
*/
typedef		unsigned char			u8;
typedef		unsigned short			u16;
typedef		unsigned short			sum16;
typedef		unsigned int			u32;
typedef		unsigned long long		u64;

typedef		unsigned short			le16;
typedef		unsigned short			be16;
typedef		unsigned int			le32;
typedef		unsigned int			be32;
typedef		unsigned long long		le64;
typedef		unsigned long long		be64;

typedef struct {
	u8		version;			// 4 for IPv4, 6 for IPv6
	u8		address[16];		// pad zero for IPv4
} ipAddress_t;

#endif
