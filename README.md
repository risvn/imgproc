This guide provides a step-by-step approach to implementing perceptual hashing (pHash) in C, without relying on machine learning. The goal is to compute a 64-bit image fingerprint and compare it using Hamming distance.
### ** Tools and Tasks**

| Task | Tool/Method |
| :---- | :---- |
| Read image (JPG, PNG) | stb\_image.h |
| Resize to 32x32 | stb\_image\_resize.h |
| Convert to grayscale | Loop over RGB to Y |
| Compute 2D DCT | Custom dct\_2d() in C |
| Generate hash | Use median threshold |
| Compare hashes | Bitwise Hamming distance |


Perceptual hashing (pHash) generates a compact fingerprint of an image based on its structural and visual characteristics. It exhibits robustness against minor alterations such as changes in brightness, compression, or resizing.

╔══════════════════════════════════════════════════════════════════════╗
║                    IMAGE pHASH GENERATION FLOW                       ║
╚══════════════════════════════════════════════════════════════════════╝

───────────────────────────────────────────────────────────────────────
1️⃣  LOAD IMAGE
───────────────────────────────────────────────────────────────────────
➤ Use `stb_image.h` (or similar library) to load the image:
    - Any format (PNG, JPG, etc)
    - Convert to grayscale on load or manually later
    - Example:
      image loaded width of 1767px, height of 1326px, channels: 4

───────────────────────────────────────────────────────────────────────
2️⃣  RESIZE TO 32x32 GRAYSCALE
───────────────────────────────────────────────────────────────────────
➤ Resample the image to 32x32 resolution.
➤ Convert to single channel (grayscale):
    - Grayscale value = weighted average of RGB
      (e.g. 0.3*R + 0.59*G + 0.11*B)
➤ Save this for debug:
    - Example: `images/out_gray32x32.png`

───────────────────────────────────────────────────────────────────────
3️⃣  APPLY DCT-II (2D Discrete Cosine Transform)
───────────────────────────────────────────────────────────────────────
➤ Perform DCT on the 32x32 grayscale matrix:
    - Break down image into frequency components
    - Most energy is in low-frequency (top-left) region

➤ Extract top-left 8x8 DCT block:
    Example matrix:
    ┌───────────────────────────────┐
    │ 17113.37   182.30  -2363.51   │
    │ -889.65    317.98   733.70    │
    │ -4582.14  -343.59   370.11    │
    │  ...                           │
    └───────────────────────────────┘

───────────────────────────────────────────────────────────────────────
4️⃣  CALCULATE pHASH
───────────────────────────────────────────────────────────────────────
➤ Compute mean of the 63 DCT values (exclude DC term at [0][0])
    Example: Mean = -191.12

➤ For each of 64 values (or just 63 + DC if desired):
    - Set bit to 1 if DCT value > mean
    - Set bit to 0 otherwise

➤ Combine bits into `uint64_t` hash:
    Example: 0xa3b29c88ef45d012

───────────────────────────────────────────────────────────────────────
5️⃣  COMPARE HASHES
───────────────────────────────────────────────────────────────────────
➤ Compute Hamming distance between two hashes:
    - Count differing bits
    - Normalize = dist / 64
    - Similarity % = (1 - normalized) * 100

───────────────────────────────────────────────────────────────────────
✔ This method is robust to:
    - Scaling
    - Color changes
    - Minor crops or noise

✘ It is weak against:
    - Major crops (large portion missing)
    - Severe distortions or rotations
───────────────────────────────────────────────────────────────────────
for more informaion refer this blog: https://www.hackerfactor.com/blog/index.php?/archives/432-Looks-Like-It.html
### **🖼️ Step 2: Load and Resize the Image**

#### **🧩 Use stb\_image.h to Load Image**

### **🔣 Step 3: Implement Discrete Cosine Transform (DCT)**

 2D DCT function to process the 8x8 image matrix.

#### **🧮 Use a Simple DCT Function**


### **🔢 Step 4: Generate the Perceptual Hash**

1. Extract the top-left 8x8 block from the DCT result.  
2. Exclude the \[0\]\[0\] (DC) term.  
3. Compute the median of the remaining 63 values.  
4. For each of the 63 values: if the value is greater than the median, assign 1; otherwise, assign 0\.  
5. Combine these 63 bits (and one more from a consistent rule for the DC term, often 0 or based on a threshold relative to itself if it's the only value being considered) into a 64-bit hash.

### **🧪 Step 5: Compare Hashes Using Hamming Distance**


** resources:**
* 🔗 [Looks Like It – Hacker Factor Blog](http://www.hackerfactor.com/blog/index.php?/archives/432-Looks-Like-It.html)
