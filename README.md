─────────────────────────────────────────────────────────────
       IMAGE pHash SIMILARITY TOOL - USAGE GUIDE
─────────────────────────────────────────────────────────────

DESCRIPTION:
------------
This program computes perceptual hashes (pHash) of images in a directory,
and compares them to detect similar or duplicate images using Hamming distance.

  - Run in query mode: compare a query image to the directory images

REQUIREMENTS:
-------------
- The program is compiled as: ./bin

BASIC USAGE:
------------
1️⃣ Normal mode: compare all images in a directory

    ./bin -q <img_path> <directory_path>


✅ The program will:
- Load all regular files in the directory
- Compute phashes
- Compare every pair
- Print similar images (if similarity exceeds threshold)

─────────────────────────────────────────────────────────────

2️⃣ Query mode: compare a single image against a directory
```bash
    ./bin -q <query_image_path> <directory_path>
```
Example:

```bash
    ./bin -q query.jpg images

```
 The program will:
- Compute phash of query.jpg
- Compare it against all images in images/
- Print similar matches

─────────────────────────────────────────────────────────────

3️⃣ Planned classify mode: group similar images into folders (future feature)

    ./bin -c <directory_path>

(coming soon)

─────────────────────────────────────────────────────────────

OPTIONS:
--------
-q       Query mode. Requires an image path and a directory path.

─────────────────────────────────────────────────────────────

EXAMPLES:
---------
Compare images inside a folder:
    ./bin images

Compare a query image:
    ./bin -q myphoto.jpg images

─────────────────────────────────────────────────────────────

NOTES:
------
- Supported image formats depend on stb_image (e.g., PNG, JPEG).
- Similarity threshold is set at: 0.34 (can be adjusted in source code).
- The program prints Hamming distance, normalized similarity, and % match.

─────────────────────────────────────────────────────────────

AUTHOR:
-------
This tool was written as a perceptual image hashing project in C using:
- STB image library (load, resize, save)
- Custom DCT and pHash calculation
- Hamming distance for comparison

─────────────────────────────────────────────────────────────
                                                            last updated:jun 15 2025
