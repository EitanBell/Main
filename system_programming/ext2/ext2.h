
#ifndef __EXT2__
#define __EXT2__

/*typedef struct ext2_super_block super_block_t;*/
/*typedef struct ext2_group_desc group_desc_t;*/
/*typedef struct ext2_inode ext2_inode_t;*/
/*typedef struct ext2_dir_entry dir_entry_t;*/
/*typedef struct ext2_dir_entry_2  dir_entry_2_t;*/

int ReadFileFromDevice(const char *device_path, const char *file_path);

#endif /*__EXT2__*/

