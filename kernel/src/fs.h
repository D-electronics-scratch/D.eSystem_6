#define FS_BLOCK_SIZE 4096

//superblock
struct superblock {
    uint32_t magic;
    uint32_t block_size;
    uint32_t inode_count;
    uint32_t inode_table_start;
    uint32_t data_block_start;


};

//inode
struct inode {

    uint32_t size;
    uint32_t first_block;
    uint8_t type; // 0=file, 1=dir

};

//directory entry
struct dir_entry {
    uint32_t inode_nr;
    char name[32];
};