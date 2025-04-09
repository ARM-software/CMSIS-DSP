Transform buffer sizes {#transformbuffers}
======================

An empty entry in a table means 'Not Applicable'.
For instance, no temporary buffer is needed for a specific configuration.

'X' means configuration is not supported.

All returned lengths are in real numbers. The number of input samples is the transform length.


Several functions are available in the C and Python API to compute the buffer sizes at runtime.
## CFFT

CFFT of length N

### CFFT Temporary Buffer Size

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      |          |                  |          |        |
| F32                      |          |                  |          | 2N     |
| F16                      |          |                  |          | 2N     |
| Q31                      |          |                  |          | 2N     |
| Q15                      |          |                  |          | 2N     |

### CFFT Output Buffer Size

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      | 2N       | 2N               | 2N       | 2N     |
| F32                      | 2N       | 2N               | 2N       | 2N     |
| F16                      | 2N       | 2N               | 2N       | 2N     |
| Q31                      | 2N       | 2N               | 2N       | 2N     |
| Q15                      | 2N       | 2N               | 2N       | 2N     |

### CIFFT Output Buffer Size

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      | 2N       | 2N               | 2N       | 2N     |
| F32                      | 2N       | 2N               | 2N       | 2N     |
| F16                      | 2N       | 2N               | 2N       | 2N     |
| Q31                      | 2N       | 2N               | 2N       | 2N     |
| Q15                      | 2N       | 2N               | 2N       | 2N     |

## RFFT

RFFT of length N

### RFFT/RIFFT Temporary Buffer Size

Apply to both RFFT and RIFFT

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      |          |                  |          |        |
| F32                      |          |                  |          | N      |
| F16                      |          |                  |          | N      |
| Q31                      |          |                  |          | 2N     |
| Q15                      |          |                  |          | 2N     |

### RFFT Output Buffer Size

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      | N        | N                | N        | N      |
| F32                      | N        | N                | N        | N      |
| F16                      | N        | N                | N        | N      |
| Q31                      | 2N       | 2N               | N+2      | N+2    |
| Q15                      | 2N       | 2N               | N+2      | N+2    |

### RIFFT Input Buffer Size

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      | N        | N                | N        | N      |
| F32                      | N        | N                | N        | N      |
| F16                      | N        | N                | N        | N      |
| Q31                      | N+2      | N+2              | N+2      | N+2    |
| Q15                      | N+2      | N+2              | N+2      | N+2    |

## MFCC (CFFT based)

MFCC of length N

### MFCC Temporary Buffer Size 1

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      | 2N       | 2N               | 2N       | X      |
| F32                      | 2N       | 2N               | 2N       | X      |
| F16                      | 2N       | 2N               | 2N       | X      |
| Q31                      | 2N       | 2N               | 2N       | X      |
| Q15                      | 2N       | 2N               | 2N       | X      |

### MFCC Temporary Buffer Size 2

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      |          |                  |          | X      |
| F32                      |          |                  |          | X      |
| F16                      |          |                  |          | X      |
| Q31                      |          |                  |          | X      |
| Q15                      |          |                  |          | X      |

## MFCC (RFFT based)

MFCC of length N

### MFCC Temporary Buffer Size 1

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      | N        | N                | N        | N      |
| F32                      | N        | N                | N        | N      |
| F16                      | N        | N                | N        | N      |
| Q31                      | 2N       | 2N               | N+2      | N+2    |
| Q15                      | 2N       | 2N               | N+2      | N+2    |

### MFCC Temporary Buffer Size 2

| Datatype / Architecture  | SCALAR   | DSP EXTENSIONS   | HELIUM   | NEON   |
| :----------------------: | :------: | :--------------: | :------: | :----: |
| F64                      |          |                  |          |        |
| F32                      |          |                  |          | N      |
| F16                      |          |                  |          | N      |
| Q31                      |          |                  |          | 2N     |
| Q15                      |          |                  |          | 2N     |

