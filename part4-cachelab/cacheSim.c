#include "cacheSim.h"

void init_DRAM()
{
    unsigned int i=0;
    DRAM = malloc(sizeof(char) * DRAM_SIZE);
    for(i=0;i<DRAM_SIZE/4;i++)
    {
        *((unsigned int*)DRAM+i) = i;
    }
    
    // Initialize caches timestamps/tags to 0
    int s, w;
    for(s = 0; s < 2; s++) {
        for(w = 0; w < 2; w++) {
            L1_cache[s][w].timeStamp = 0;
            L1_cache[s][w].tag = 0;
        }
    }
    for(s = 0; s < 4; s++) {
        for(w = 0; w < 4; w++) {
            L2_cache[s][w].timeStamp = 0;
            L2_cache[s][w].tag = 0;
        }
    }
}

void printCache()
{
    int i,j,k;
    printf("===== L1 Cache Content =====\n");
    for(i=0;i<2;i++)
    {
        printf("Set %d :", i);
        for(j=0;j<2;j++)
        {
            printf(" {(TAG: 0x%x)", (unsigned int)(L1_cache[i][j].tag));
            for(k=0;k<16;k++)
                printf(" 0x%x,", (unsigned int)(L1_cache[i][j].data[k]));
            printf(" |");
        }
        printf("\n");
    }
    printf("===== L2 Cache Content =====\n");
    for(i=0;i<4;i++)
    {
        printf("Set %d :", i);
        for(j=0;j<4;j++)
        {
            printf(" {(TAG: 0x%x)", (unsigned int)(L2_cache[i][j].tag));
            for(k=0;k<16;k++)
                printf(" 0x%x,", (unsigned int)(L2_cache[i][j].data[k]));
            printf(" |");
        }
        printf("\n");
    }
}

// Global counter tracking chronological block insertions to enforce FIFO logic safely
static u_int32_t fifo_counter = 1;

// Helper function to insert a block fetched from DRAM into L2 using FIFO tracking
void insert_L2(u_int32_t address) {
    unsigned int set = getL2SetID(address);
    u_int32_t tag = getL2Tag(address);
    u_int32_t block_start = address & ~0xF;
    int w;

    for (w = 0; w < 4; w++) {
        if (L2_cache[set][w].timeStamp != 0 && L2_cache[set][w].tag == tag) {
            return; // Already in L2
        }
    }
    for (w = 0; w < 4; w++) {
        if (L2_cache[set][w].timeStamp == 0) {
            L2_cache[set][w].tag = tag;
            L2_cache[set][w].timeStamp = fifo_counter++;
            int k;
            for (k = 0; k < 16; k++) {
                L2_cache[set][w].data[k] = DRAM[block_start + k];
            }
            return;
        }
    }

    //  Cache is Full: Find the block with the oldest timestampo
    int oldest_way = 0;
    u_int32_t min_time = L2_cache[set][0].timeStamp;
    for (w = 1; w < 4; w++) {
        if (L2_cache[set][w].timeStamp < min_time) {
            min_time = L2_cache[set][w].timeStamp;
            oldest_way = w;
        }
    }

    u_int32_t evicted_l2_addr = (L2_cache[set][oldest_way].tag << 6) | (set << 4);
    unsigned int l1_set = getL1SetID(evicted_l2_addr);
    u_int32_t l1_tag = getL1Tag(evicted_l2_addr);
    for (w = 0; w < 2; w++) {
        if (L1_cache[l1_set][w].timeStamp != 0 && L1_cache[l1_set][w].tag == l1_tag) {
            L1_cache[l1_set][w].timeStamp = 0; 
        }
    }

    L2_cache[set][oldest_way].tag = tag;
    L2_cache[set][oldest_way].timeStamp = fifo_counter++;
    int k;
    for (k = 0; k < 16; k++) {
        L2_cache[set][oldest_way].data[k] = DRAM[block_start + k];
    }
}

// Helper function to balance and allocate missing target lines inside L1
void insert_L1(u_int32_t address) {
    // making sure line exists inside L2 before populating L1 cache structures
    insert_L2(address);

    unsigned int set = getL1SetID(address);
    u_int32_t tag = getL1Tag(address);
    u_int32_t block_start = address & ~0xF;
    int w;

    for (w = 0; w < 2; w++) {
        if (L1_cache[set][w].timeStamp != 0 && L1_cache[set][w].tag == tag) {
            return; // already hits L1 structure
        }
    }
    for (w = 0; w < 2; w++) {
        if (L1_cache[set][w].timeStamp == 0) {
            L1_cache[set][w].tag = tag;
            L1_cache[set][w].timeStamp = fifo_counter++;
            int k;
            for (k = 0; k < 16; k++) {
                L1_cache[set][w].data[k] = DRAM[block_start + k];
            }
            return;
        }
    }

    // out based on FIFO rule
    int oldest_way = (L1_cache[set][0].timeStamp < L1_cache[set][1].timeStamp) ? 0 : 1;
    L1_cache[set][oldest_way].tag = tag;
    L1_cache[set][oldest_way].timeStamp = fifo_counter++;
    int k;
    for (k = 0; k < 16; k++) {
        L1_cache[set][oldest_way].data[k] = DRAM[block_start + k];
    }
}

// helper defined to safely pull individual byte contents checking hits at every level
unsigned char read_single_byte(u_int32_t address) {
    if (!L1lookup(address)) {
        insert_L1(address);
    }

    unsigned int set = getL1SetID(address);
    u_int32_t tag = getL1Tag(address);
    int offset = address & 0xF;
    int w;
    for (w = 0; w < 2; w++) {
        if (L1_cache[set][w].timeStamp != 0 && L1_cache[set][w].tag == tag) {
            return L1_cache[set][w].data[offset];
        }
    }
    return DRAM[address]; 
}

unsigned int getL1SetID(u_int32_t address) {
    // 16-byte offset uses 4 bits. L1 has 2 sets -> 1 index bit.
    return (address >> 4) & 0x1;
}

unsigned int getL1Tag(u_int32_t address) {
    return (address >> 5);
}

unsigned int getL2SetID(u_int32_t address) {
    // 16-byte offset uses 4 bits. L2 has 4 sets -> 2 index bits.
    return (address >> 4) & 0x3;
}

unsigned int getL2Tag(u_int32_t address) {
    return (address >> 6);
}

int L1lookup(u_int32_t address) {
    unsigned int set = getL1SetID(address);
    u_int32_t tag = getL1Tag(address);
    int w;
    for (w = 0; w < 2; w++) {
        if (L1_cache[set][w].timeStamp != 0 && L1_cache[set][w].tag == tag) {
            return 1;
        }
    }
    return 0;
}

int L2lookup(u_int32_t address) {
    unsigned int set = getL2SetID(address);
    u_int32_t tag = getL2Tag(address);
    int w;
    for (w = 0; w < 4; w++) {
        if (L2_cache[set][w].timeStamp != 0 && L2_cache[set][w].tag == tag) {
            return 1;
        }
    }
    return 0;
}

u_int32_t read_fifo(u_int32_t address) {
    u_int32_t result = 0;
    int i;

    for (i = 0; i < 4; i++) {
        u_int32_t b = read_single_byte(address + i);
        result |= (b << (i * 8));
    }
    return result;
}

void write(u_int32_t address, u_int32_t data) {
    int i, w;
    
    // processing sequential byte adjustments into backing storage
    for (i = 0; i < 4; i++) {
        u_int32_t curr_addr = address + i;
        unsigned char byte_val = (data >> (i * 8)) & 0xFF;
        
        // Write-through to basic backend DRAM immediately
        if (curr_addr < DRAM_SIZE) {
            DRAM[curr_addr] = byte_val;
        }
        
        // Updatingd L1 copy if it is a cache hit
        unsigned int l1_set = getL1SetID(curr_addr);
        u_int32_t l1_tag = getL1Tag(curr_addr);
        int l1_offset = curr_addr & 0xF;
        for (w = 0; w < 2; w++) {
            if (L1_cache[l1_set][w].timeStamp != 0 && L1_cache[l1_set][w].tag == l1_tag) {
                L1_cache[l1_set][w].data[l1_offset] = byte_val;
            }
        }
        
        // Updating L2 copy if it is a cache hit
        unsigned int l2_set = getL2SetID(curr_addr);
        u_int32_t l2_tag = getL2Tag(curr_addr);
        int l2_offset = curr_addr & 0xF;
        for (w = 0; w < 4; w++) {
            if (L2_cache[l2_set][w].timeStamp != 0 && L2_cache[l2_set][w].tag == l2_tag) {
                L2_cache[l2_set][w].data[l2_offset] = byte_val;
            }
        }
    }
}


int main()
{
    init_DRAM();
    cacheAccess buffer;
    int timeTaken=0;
    FILE *trace = fopen("input.trace","r");
    if (!trace) {
        printf("Error: Could not open input.trace!\n");
        return 1;
    }
    int L1hit = 0;
    int L2hit = 0;
    cycles = 0;
    while(!feof(trace))
    {
        if (fscanf(trace,"%d %x %x", &buffer.readWrite, &buffer.address, &buffer.data) != 3) {
            break;
        }
        printf("Processing the request for [R/W] = %d, Address = %x, data = %x\n", buffer.readWrite, buffer.address, buffer.data);

        if(L1lookup(buffer.address))// Cache hit
        {
            timeTaken = 1;
            L1hit++;
        }
        else if(L2lookup(buffer.address))// L2 Cache Hit
        {
            L2hit++;
            timeTaken = 5;
        }
        else timeTaken = 50;
        if (buffer.readWrite) write(buffer.address, buffer.data);
        else read_fifo(buffer.address);
        cycles+=timeTaken;
    }
    printCache();
    printf("Total cycles used = %ld\nL1 hits = %d, L2 hits = %d\n", cycles, L1hit, L2hit);
    fclose(trace);
    free(DRAM);
    return 0;
}