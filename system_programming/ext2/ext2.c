
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <assert.h>			/* assert */
#include <linux/types.h>	/*__u16, __u32 */
#include <stdio.h>			/* printf */
#include <string.h>			/* memcpy */

#include "ext2.h"			/* File System Parsing API */

#define BASE_OFFSET 					(1024)
#define BLOCK_SIZE						(4096)
#define EXT2_SUPER_MAGIC				(0xEF53)
#define BLOCK_OFFSET(block, block_size)	(block * block_size)
#define EXT2_NAME_LEN 					(255)


#define	EXT2_NDIR_BLOCKS		(12)
#define	EXT2_IND_BLOCK			(EXT2_NDIR_BLOCKS)
#define	EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define	EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define	EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)
 
enum boolean
{
	FALSE = 0,
	TRUE = 1
}; 

enum status
{
	FAIL = -1,
	SUCCESS = 0
};
   
typedef struct ext2_super_block 
{
	__u32	s_inodes_count;		/* Inodes count */
	__u32	s_blocks_count;		/* Blocks count */
	__u32	s_r_blocks_count;	/* Reserved blocks count */
	__u32	s_free_blocks_count;	/* Free blocks count */
	__u32	s_free_inodes_count;	/* Free inodes count */
	__u32	s_first_data_block;	/* First Data Block */
	__u32	s_log_block_size;	/* Block size */
	__s32	s_log_frag_size;	/* Fragment size */
	__u32	s_blocks_per_group;	/* # Blocks per group */
	__u32	s_frags_per_group;	/* # Fragments per group */
	__u32	s_inodes_per_group;	/* # Inodes per group */
	__u32	s_mtime;		/* Mount time */
	__u32	s_wtime;		/* Write time */
	__u16	s_mnt_count;		/* Mount count */
	__s16	s_max_mnt_count;	/* Maximal mount count */
	__u16	s_magic;		/* Magic signature */
	__u16	s_state;		/* File system state */
	__u16	s_errors;		/* Behaviour when detecting errors */
	__u16	s_minor_rev_level; 	/* minor revision level */
	__u32	s_lastcheck;		/* time of last check */
	__u32	s_checkinterval;	/* max. time between checks */
	__u32	s_creator_os;		/* OS */
	__u32	s_rev_level;		/* Revision level */
	__u16	s_def_resuid;		/* Default uid for reserved blocks */
	__u16	s_def_resgid;		/* Default gid for reserved blocks */
	__u32	s_first_ino; 		/* First non-reserved inode */
	__u16   s_inode_size; 		/* size of inode structure */
	__u16	s_block_group_nr; 	/* block group # of this superblock */
	__u32	s_feature_compat; 	/* compatible feature set */
	__u32	s_feature_incompat; 	/* incompatible feature set */
	__u32	s_feature_ro_compat; 	/* readonly-compatible feature set */
	__u8	s_uuid[16];		/* 128-bit uuid for volume */
	char	s_volume_name[16]; 	/* volume name */
	char	s_last_mounted[64]; 	/* directory where last mounted */
	__u32	s_algorithm_usage_bitmap; /* For compression */
	__u8	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
	__u16	s_padding1;
	__u32	s_reserved[204];	/* Padding to the end of the block */
}super_block_t;

typedef struct ext2_group_desc
{
	__u32	bg_block_bitmap;	/* Blocks bitmap block */
	__u32	bg_inode_bitmap;	/* Inodes bitmap block */
	__u32	bg_inode_table;		/* Inodes table block */
	__u16	bg_free_blocks_count;	/* Free blocks count */
	__u16	bg_free_inodes_count;	/* Free inodes count */
	__u16	bg_used_dirs_count;	/* Directories count */
	__u16	bg_pad;
	__u32	bg_reserved[3];
}group_desc_t;

typedef struct ext2_inode 
{
	__u16	i_mode;		/* File mode */
	__u16	i_uid;		/* Low 16 bits of Owner Uid */
	__u32	i_size;		/* Size in bytes */
	__u32	i_atime;	/* Access time */
	__u32	i_ctime;	/* Creation time */
	__u32	i_mtime;	/* Modification time */
	__u32	i_dtime;	/* Deletion Time */
	__u16	i_gid;		/* Low 16 bits of Group Id */
	__u16	i_links_count;	/* Links count */
	__u32	i_blocks;	/* Blocks count */
	__u32	i_flags;	/* File flags */
	union 
	{
		struct 
		{
			__u32  l_i_reserved1;
		}linux1;
		struct 
		{
			__u32  h_i_translator;
		}hurd1;
		struct 
		{
			__u32  m_i_reserved1;
		}masix1;
	}osd1;				/* OS dependent 1 */
	__u32	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
	__u32	i_generation;	/* File version (for NFS) */
	__u32	i_file_acl;	/* File ACL */
	__u32	i_dir_acl;	/* Directory ACL */
	__u32	i_faddr;	/* Fragment address */
	union 
	{
		struct 
		{
			__u8	l_i_frag;	/* Fragment number */
			__u8	l_i_fsize;	/* Fragment size */
			__u16	i_pad1;
			__u16	l_i_uid_high;	/* these 2 fields    */
			__u16	l_i_gid_high;	/* were reserved2[0] */
			__u32	l_i_reserved2;
		} linux2;
		struct 
		{
			__u8	h_i_frag;	/* Fragment number */
			__u8	h_i_fsize;	/* Fragment size */
			__u16	h_i_mode_high;
			__u16	h_i_uid_high;
			__u16	h_i_gid_high;
			__u32	h_i_author;
		} hurd2;
		struct 
		{
			__u8	m_i_frag;	/* Fragment number */
			__u8	m_i_fsize;	/* Fragment size */
			__u16	m_pad1;
			__u32	m_i_reserved2[2];
		} masix2;
	} osd2;				/* OS dependent 2 */
}inode_t;


typedef struct ext2_dir_entry_2 
{
   __u32   inode;           		/* Inode number */
   __u16   rec_len;        			/* Directory entry length */
   __u8    name_len;        		/* Name length */
   __u8    file_type;
   char    name[EXT2_NAME_LEN];		/* File name */
}dir_entry_t;

static super_block_t ReadSuperBlock(int fd);
static group_desc_t ReadGroupDescriptor(int fd, unsigned int block_size);
static void ReadInode(int fd, int inode_num, unsigned int block_size,
								const group_desc_t *group_desc, inode_t *inode);
static int ReadFile(int fd, unsigned int block_size, inode_t *inode, 
						const group_desc_t *group_desc, const char *file_path);
static void ReadDir(int fd, unsigned int block_size, inode_t *inode, 
						const group_desc_t *group_desc, const char *cur_path);
static unsigned int PrintSuperBlock(super_block_t superblock, 
													  unsigned int block_size);
static void PrintGroupDescriptor(group_desc_t group_desc);

int ReadFileFromDevice(const char *device_path, const char *file_path)
{
	int result = SUCCESS;
	int fd = 0;
	unsigned int block_size = 0;
	super_block_t superblock = {0};
	group_desc_t group_desc = {0};

	inode_t inode = {0};
	
	printf("device_path: %s\n", device_path);
	printf("file_path: %s\n", file_path);
	
	fd = open(device_path, O_RDONLY);
	
	if (fd < 0)
	{
		printf("Error at open\n");
		return FAIL;
	}
	
	superblock = ReadSuperBlock(fd);
	block_size = BASE_OFFSET << superblock.s_log_block_size;
	
	group_desc = ReadGroupDescriptor(fd, block_size);
	
	PrintSuperBlock(superblock, block_size);
	PrintGroupDescriptor(group_desc);
	
	ReadInode(fd, 2, block_size, &group_desc, &inode);
	
	if ('/' == *file_path)
	{
		++file_path;
	}
	
	if (NULL == strstr(file_path, "/"))
	{
		result = ReadFile(fd, block_size, &inode, &group_desc, file_path);
	}
	else
	{
/*		ReadDir(fd, block_size, &inode, &group_desc);*/
	}
	
	if (result == FAIL)
	{
		printf("file %s not found\n", file_path);
	}
	
	return result;
}


static super_block_t ReadSuperBlock(int fd)
{
	super_block_t superblock = {0};
	
	lseek(fd, BASE_OFFSET, SEEK_SET);
	read(fd, &superblock, sizeof(superblock));
	
	return superblock;
}

static group_desc_t ReadGroupDescriptor(int fd, unsigned int block_size)
{
	group_desc_t group_desc = {0};

	lseek(fd, block_size, SEEK_SET);
	read(fd, &group_desc, sizeof(group_desc));
	
	return group_desc;
}

static void ReadInode(int fd, int inode_num, unsigned int block_size,
								const group_desc_t *group_desc, inode_t *inode)
{
	lseek(fd, BLOCK_OFFSET(group_desc->bg_inode_table, block_size) + 
									 ((inode_num - 1) * sizeof(inode_t)), SEEK_SET);
	read(fd, inode, sizeof(inode_t));
}

static int ReadFile(int fd, unsigned int block_size, inode_t *inode, 
						const group_desc_t *group_desc, const char *file_path)
{
	int result = FAIL;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned char block[BLOCK_SIZE] = {0};
	unsigned int size = 0;
	dir_entry_t *entry = NULL;
	inode_t file_inode = {0};
	
	lseek(fd, BLOCK_OFFSET(inode->i_block[0], block_size), SEEK_SET);
	read(fd, block, block_size);
	
	entry = (dir_entry_t *)block;
	
	printf("File content:\n");
	while((size < inode->i_size) && (entry->rec_len > 0))
	{
		char file_name[EXT2_NAME_LEN + 1];
		memcpy(file_name, entry->name, entry->name_len);
		file_name[entry->name_len] = '\0';
		if (0 == strcmp(file_name, file_path))
		{
			ReadInode(fd, entry->inode + 1, block_size, group_desc, &file_inode);
			printf("file_inode.i_blocks %u\n", file_inode.i_blocks);
			for (i = 0; i < file_inode.i_blocks; ++i)
			{
				int fd_runner = fd;
				lseek(fd_runner, BLOCK_OFFSET(file_inode.i_block[i], block_size), SEEK_SET);
				read(fd_runner, block, block_size);
				for (j = 0; i < BLOCK_SIZE; ++j)
				{
					printf("%c", block[j]);
				}
			}
			printf("\n");
			result = SUCCESS;
		}
		size += entry->rec_len;
		entry = (void *)((char *)entry + entry->rec_len);
	}
	printf("\n");
	
	return result;
}

static void ReadDir(int fd, unsigned int block_size, inode_t *inode, 
						const group_desc_t *group_desc, const char *cur_path)
{
	unsigned char block[BLOCK_SIZE] = {0};
	unsigned int size = 0;
	dir_entry_t *entry = NULL;
	
	lseek(fd, BLOCK_OFFSET(inode->i_block[0], block_size), SEEK_SET);
	read(fd, block, block_size);
	
	entry = (dir_entry_t *)block;
	
	printf("\nDirectory content:\n");
	while((size < inode->i_size) && (entry->rec_len > 0))
	{
		char file_name[EXT2_NAME_LEN + 1];
		memcpy(file_name, entry->name, entry->name_len);
		file_name[entry->name_len] = '\0';
		printf("%u %s\n", entry->inode, file_name);
		size += entry->rec_len;
		entry = (void *)((char *)entry + entry->rec_len);
	}
	printf("\n");
}


static unsigned int PrintSuperBlock(super_block_t superblock, 
													  unsigned int block_size)
{
	printf("Super Block:\n");
	printf("Inodes count: %u\n", superblock.s_inodes_count);
	printf("Blocks count: %u\n", superblock.s_blocks_count);
	printf("Reserved blocks count: %u\n", superblock.s_r_blocks_count);
	printf("Free inodes count: %u\n", superblock.s_free_inodes_count);
	printf("Free blocks count: %u\n", superblock.s_free_blocks_count);
	printf("Inodes per group: %u\n", superblock.s_inodes_per_group);
	printf("Blocks per group: %u\n", superblock.s_blocks_per_group);
	printf("Block size: %u\n", block_size);
	
	return block_size;
}

static void PrintGroupDescriptor(group_desc_t group_desc)
{
	printf("\n Group Descriptor:\n");
	printf("block bitmap value: %u\n", group_desc.bg_block_bitmap);
	printf("inode bitmap value: %u\n", group_desc.bg_inode_bitmap);
	printf("Inode table block: %u\n", group_desc.bg_inode_table);
	printf("Free blocks count: %u\n", group_desc.bg_free_blocks_count);
	printf("Free inodes count: %u\n", group_desc.bg_free_inodes_count);
	printf("Used directories count: %u\n", group_desc.bg_used_dirs_count);
}

