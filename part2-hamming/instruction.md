# Extended Hamming Code (16,11) (50 Points) (E)

---

In this assignment, you will implement error detection and correction using the Extended Hamming (16,11) code. In this code, 11 data bits are encoded into a 16‑bit word. The first 15 bits (starting from 1 to 15) form the standard Hamming (15,11) code (with parity bits at positions 1, 2, 4, and 8) and the special 0th bit is an overall parity bit for double-error detection.

This extra parity enables the detection (but not correction) of two-bit errors while still allowing single‑bit errors to be corrected.

## Handout Instructions

---

- Find the starting package within the repository, you will find `hamming16.c` .
- Make changes to this file, commit and push it back to your repository on GitHub Classroom.
- You are highly recommended to study how hamming code works from the following video and online simulator.
    - Video: [https://youtu.be/X8jsijhllIA?si=YNJvFX5miCej71eJ](https://youtu.be/X8jsijhllIA?si=YNJvFX5miCej71eJ)
    - Simulator: [https://harryli0088.github.io/hamming-code/](https://harryli0088.github.io/hamming-code/)
        - You can create a test case of your own and use this simulator to confirm it manually !

## **Implementation Constraints**

---

- **Operators:** Use only bitwise operators (&, |, ^, ~, <<, >>) for all bit manipulations.
- **Loops & Conditionals:** While loops and conditionals may be used in the decoding function (e.g., when computing the syndrome), aim for clear and direct bit manipulation.
- **Documentation:** Comment your code clearly, especially explaining the bit ordering, how parity bits are computed, and how the syndrome is used to correct errors.

## **Part 1: Extended Hamming Encoding**

---

**Function Prototype:**

```c
uint16_t extendedHammingEncode(uint16_t data);
```

**Requirements:**

- **Input:** The lower 11 bits of data represent the 11‑bit number (values 0–2047).
- **Output:** Return a 16‑bit code word where:
    - Bits 1–15 form a Hamming (15,11) code with parity bits at positions 1, 2, 4, and 8 (bit positions are 1‑indexed, with position 1 as the least-significant bit).
    - Bit 0 (the most significant bit) is the overall parity (the XOR of bits 1–15).

## **Part 2: Extended Hamming Decoding**

---

**Function Prototype:**

```c
uint16_t extendedHammingDecode(uint16_t code, int *errorPos);
```

**Requirements:**

- **Input:** The lower 16 bits of code contain the Extended Hamming code word.
- **Output:** Return the original 11‑bit data (stored in the lower 11 bits of the result).

**Error Detection & Correction:**

- Recompute the parity bits for positions 1, 2, 4, and 8 to form a syndrome.
- Check the overall parity (bit 0) to decide whether an error occurred.
- If the syndrome indicates a single‑bit error and the overall parity confirms it, correct that bit.
- Set the integer pointed to by `errorPos` as follows:
    - 0 if no error was detected.
    - A value from 0 to 15 if a single‑bit error was corrected.
    - A negative value (e.g., -1) if an uncorrectable (multiple‑bit) error is detected.

## **Part 3: Error Injection**

---

**Function Prototype:**

```c
uint16_t introduceError(uint16_t code, int bitPos);
```

**Requirements:**

- **Input:** A 16‑bit code word in code and an integer bitPos (0–15) indicating which bit to flip.
- **Output:** Return the new code word with the specified bit flipped.
- **Purpose:** This function is used to simulate a single‑bit error for testing your decoding function.

## Running Your Program

---

```bash
./hamming16 0x3A5       # Run without error injection.
./hamming16 0x3A5 7     # Run with error injected at bit 7.
```