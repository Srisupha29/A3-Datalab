# Cache Lab (50 Points)(E)

---

The purpose of this assignment is to become more familiar with how cache works.

## Handout Instructions

---

You will need a starter package which can be fond in your GitHub repository.

Once accessed, you will find three files: `cacheSim.h`, `cacheSim.c` and `input.trace`.

Inside **`cacheSim.c`**, we have provided the skeleton code for our simple cache, which has the following properties:

<aside>
💡

- The cache has two levels
- A cache block is 16
- Each address is accessing 1 bytes of
- The L1 cache is a 64 Bytes, 2-way set associative
- The L2 cache is a 256 Bytes, 4-way set associative
- The cache is inclusive, which mean that data in L1 cache will also remain in the L2 In other word, the data in L1 is a subset of the data in L2 (This assumption simplify your design).
- The cache is using a **first-in-first-out** cache replacement policy. Note that this is simpler to implement than the LRU policy.
</aside>

Your task is to build a cache simulator using the skeleton code we provided. To help you with the task, we have provided you with the following functions

- trace captures the trace of cache accesses.
- Each line in this file will contain three items: `[0/1] [address] [data]`, where
    - `0/1` is a single number (0 means this is a read request and 1 means this is a write request),
    - `[address]` is a hexadecimal representation of the address of this particular cache access, and
    - `[data]` is either 0 or the actual data the cache access need to write to (Note that a read request does not care about what the data is and you can ignore it.
- This piece of information is specifically for the write request). Please note that you can also create your own trace to test out your code. **Testing your code is a part of this assignment, and I am going to test your implementation using traces that I collect from real programs.**
- The main() function is where the simulator handle reading the input You do not need to touch this except for the line that open the trace file (you should use your own trace file to test your cache).
- `init_DRAM()` initializes the value of Please DO NOT TOUCH this.
- `printCache()` prints the content of the cache, which you can call at any time to test your
- `readInput()` reads one line of the input This function is used in tandem with the main function and you do not have to touch this.

## Part 1: Getting Tags, SetID & Performing a Cache Lookup (20 points)

---

For the first part, you are going to write six functions.

- `unsigned int getL1SetID(uint32_t address)` returns the setID assocated with the input address for L1 cache.
- `unsigned int getL1Tag(uint32_t address)` returns the tag assocated with the input ad- dress for L1 cache.
- `unsigned int getL2SetID(uint32_t address)` returns the setID assocated with the input address for L2 cache.
- `unsigned int getL2Tag(uint32_t address)` returns the tag assocated with the input ad- dress for L2 cache.
- `int L1lookup(uint32_t address)` performs a L1 cache This function returns 1 if the input address is in the cache and 0 otherwise. Please note that this function **DOES NOT** perform any actual insertion or eviction.
- `int L2lookup(uint32_t address)` performs a L2 cache This function returns 1 if the input address is in the cache and 0 otherwise. Please note that this function **DOES NOT** perform any actual insertion or eviction.

## Part 2: A Simple FIFO-cache (20 points)

---

For this part, you are going to implement the cache insertion and eviction based on a FIFO replacement policy. The FIFO cache replacement policy choose to evict the cache block that was inserted first (literally, first-in first-out). Please note that this policy is different and simpler (and likely to be worse) than the LRU policy we covered in class.

You are going to write another function.

- `uint32_t read_fifo(uint32_t address)` processes a read request by properly going though the cache and updating the cache content based on the FIFO replacement The function should return the data you are reading.

**Be careful:** The input request can go to a 4-byte address that spans two cache blocks. Note that the compiler will usually break this particular data access into two different requests, but we are simplifying the cache here so we will assume that every request is for a 4-byte data and address can start at any byte.

## Part 3: Handling Writes (10 points)

---

After you correctly handle read requests (and assume that all write requests behave like a read request), your next task is to handle write requests. For this assignment, we assume a write-through cache, which performs a write to all cache levels (i.e., if there is a write to address addr, this new value should be updated for all the cache levels as well as in the DRAM).

Specifically, you need to finish up the following function:

- `void write(uint32_t address, uint32_t data)`, which performs a write to address by up- dating the byte at our address with data.

<aside>
💡

- **Hint 1:** When you have a write hit, please carefully check which bytes in the cache block should be updated.
- **Hint 2:** You can reuse most of the cache eviction code from part 2 in order to *both* update the data and insert/evict cache blocks at the same time.
</aside>