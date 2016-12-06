#define FILE_ID_LEN 5
// short (16 bits) 
// unsigned int 16 bits
typedef struct secretMeta{
	unsigned int  secretSize;
	unsigned char numberPieces;	// the total number of files the secret is encoded into
	unsigned char indexOfPiece;	// the index of this secret
	char fileId [FILE_ID_LEN];	// the ID of a file to tell them apart
} fileMeta;