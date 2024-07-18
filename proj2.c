#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void print_binary_with_marker(int num, int bits, int marker_pos, FILE *output) {
    for (int i = bits - 1; i >= 0; i--) {
        if (i == marker_pos - 1) {
            fprintf(output, "|");
        }
        fprintf(output, "%d", (num >> i) & 1);
    }
    if (marker_pos == 0) {
        fprintf(output, "|");
    }
}


void to_binary_string(int num, int bits, char *str) {
    for (int i = bits - 1; i >= 0; i--) {
        str[bits - 1 - i] = (num & (1 << i)) ? '1' : '0';
    }
    str[bits] = '\0';
}



void print_binary_with_markerr(long long num, int bits, int marker_pos, FILE *output) {
    for (int i = bits - 1; i >= 0; i--) {
        if (i == marker_pos - 1) {
            fprintf(output, "|");
        }
        fprintf(output, "%lld", (num >> i) & 1);
    }
    if (marker_pos == 0) {
        fprintf(output, "|");
    }
}



long long to_twos_complementt(long long num, int bits) {
    if (num < 0) {
        num = (1LL << bits) + num;
    }
    return num;
}



long long from_twos_complementt(long long num, int bits) {
    if (num & (1LL << (bits - 1))) {
        num = num - (1LL << bits);
    }
    return num;
}



void to_binary_stringg(long long num, int bits, char *str) {
    for (int i = bits - 1; i >= 0; i--) {
        str[bits - 1 - i] = (num & (1LL << i)) ? '1' : '0';
    }
    str[bits] = '\0';
}




//----------------------------------------------------------------------------------------------------



// Add & Shift for both numbers positive
void add_and_shift_positive(int A, int B, FILE *output) {
    int M = B; // M initially contains B
    int n = sizeof(int) * 8; // Number of bits in an integer

    // Find the number of bits required to represent the largest number
    int bits_A = 0, bits_B = 0;
    int temp_A = A, temp_B = B;
    while (temp_A > 0) {
        bits_A++;
        temp_A >>= 1;
    }
    while (temp_B > 0) {
        bits_B++;
        temp_B >>= 1;
    }
    int max_bits = bits_A > bits_B ? bits_A : bits_B;
    max_bits *= 2; // Double the bits for the M 
    int C=A;
    int D=B;
    char binary_A[33], binary_B[33];
    to_binary_string(A, bits_A, binary_A);
    to_binary_string(B, bits_B, binary_B);

    fprintf(output, "A=%d=%s, B=%d=%s\n", C, binary_A, D, binary_B);
    fprintf(output, "~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(output, "(0, ) M=");
    print_binary_with_marker(0, max_bits / 2, max_bits / 2, output);
    print_binary_with_marker(M, max_bits / 2, 0, output);
    fprintf(output, "\n~~~~~~~~~~~~~~~~~~~~\n");

    for (int i = 0; i < max_bits / 2; i++) {
        if (M & 1) {
            fprintf(output, "(%d,+) M=", i + 1);
            int upper_bits = (M >> (max_bits / 2));
            int lower_bits = (M & ((1 << (max_bits / 2)) - 1));
            int new_upper_bits = upper_bits + A;
            M = (new_upper_bits << (max_bits / 2)) | lower_bits;
            print_binary_with_marker(M, max_bits, max_bits / 2 - i, output);
            fprintf(output, "\n");
        } else {
            fprintf(output, "(%d, ) M=", i + 1);
            print_binary_with_marker(M, max_bits, max_bits / 2 - i, output);
            fprintf(output, "\n");
        }
        // Right shift
        M >>= 1;
        fprintf(output, "(%d,>) M=", i + 1);
        print_binary_with_marker(M, max_bits, max_bits / 2 - i - 1, output);
        fprintf(output, "\n~~~~~~~~~~~~~~~~~~~~\n");
    }

    fprintf(output, "M=AxB=%d\n\n", M);
    
}



// Add & Shift for both numbers negative
void add_and_shift_negative(long long A, long long B, FILE *output) {
    int n = sizeof(long long) * 8; // Number of bits in a long long integer
    long long C = A;
    long long D = B;
    int sign = (A < 0) ^ (B < 0) ? -1 : 1; // Determine the sign of the result

    // Find the number of bits required to represent the largest number
    int bits_A = 0, bits_B = 0;
    long long temp_A = abs(C), temp_B = abs(D);
    while (temp_A > 0) {
        bits_A++;
        temp_A >>= 1;
    }
    while (temp_B > 0) {
        bits_B++;
        temp_B >>= 1;
    }
    int max_bits = bits_A > bits_B ? bits_A : bits_B;
    max_bits++; // Adjust the number of bits for the largest number

    // Convert A and B to two's complement
    A = to_twos_complementt(C, max_bits);
    B = to_twos_complementt(D, max_bits);
    long long M = B; // M initially contains B

    char binary_A[65], binary_B[65];
    to_binary_stringg(A, max_bits, binary_A);
    to_binary_stringg(B, max_bits, binary_B);

    fprintf(output, "A=%lld=%s, B=%lld=%s\n", C, binary_A, D, binary_B);
    fprintf(output, "~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(output, "(0, ) M=");
    print_binary_with_markerr(0, max_bits, max_bits, output);
    fprintf(output, "|");
    print_binary_with_markerr(M, max_bits, 0, output);
    fprintf(output, "\n~~~~~~~~~~~~~~~~~~~~\n");

    for (int i = 0; i < max_bits - 1; i++) {
        long long upper_bits = (M >> max_bits);
        long long lower_bits = (M & ((1LL << max_bits) - 1));
        if (M & 1) { // If LSB is 1
            fprintf(output, "(%d,+) M=", i + 1);
            upper_bits += A;
            M = (upper_bits << max_bits) | lower_bits;
            print_binary_with_markerr(M, max_bits * 2, max_bits, output);
            fprintf(output, "\n");
        } else {
            fprintf(output, "(%d, ) M=", i + 1);
            print_binary_with_markerr(M, max_bits * 2, max_bits, output);
            fprintf(output, "\n");
        }
        // Right shift with sign extension
        long long sign_bit = M & (1LL << (max_bits * 2 - 1));
        M >>= 1;
        if (sign_bit) {
            M |= (1LL << (max_bits * 2 - 1));
        }
        fprintf(output, "(%d,>) M=", i + 1);
        print_binary_with_markerr(M, max_bits * 2, max_bits - i - 1, output);
        fprintf(output, "\n~~~~~~~~~~~~~~~~~~~~\n");
    }

    // Handle the last step separately
    if (M & 1) { // If LSB is 1
        fprintf(output, "(%d,-) M=", max_bits);
        long long upper_bits = (M >> max_bits);
        long long lower_bits = (M & ((1LL << max_bits) - 1));
        upper_bits -= A; // Subtract A for the last step
        M = (upper_bits << max_bits) | lower_bits;
        print_binary_with_markerr(M, max_bits * 2, 0, output);
        fprintf(output, "\n");

        // Right shift with sign extension
        long long sign_bit = M & (1LL << (max_bits * 2 - 1));
        M >>= 1;
        if (sign_bit) {
            M |= (1LL << (max_bits * 2 - 1));
        }
        fprintf(output, "(%d,>) M=", max_bits);
        print_binary_with_markerr(M, max_bits * 2, 0, output);
        fprintf(output, "\n~~~~~~~~~~~~~~~~~~~~\n");
    }

    long long result = from_twos_complementt(M, max_bits * 2);
    if (sign == -1) {
        result = -result;
    }

    fprintf(output, "M=AxB=%+lld\n\n", result);
}




// Add & Shift for mixed signs with variable bit length
void add_and_shift_mixed(long long A, long long B, FILE *output) {
    int n = sizeof(long long) * 8; // Number of bits in a long long integer
    long long C = A;
    long long D = B;
    int sign = (A < 0) ^ (B < 0) ? -1 : 1; // Determine the sign of the result
    A = to_twos_complementt(A, n); // Convert A to two's complement
    B = to_twos_complementt(B, n); // Convert B to two's complement
    long long M = B; // M initially contains B
    long long neg_A = to_twos_complementt(-from_twos_complementt(A, n), n); // Calculate -A in two's complement

    // Find the number of bits required to represent the largest number
    int bits_A = 0, bits_B = 0;
    long long temp_A = abs(C), temp_B = abs(D);
    while (temp_A > 0) {
        bits_A++;
        temp_A >>= 1;
    }
    while (temp_B > 0) {
        bits_B++;
        temp_B >>= 1;
    }
    int max_bits = bits_A > bits_B ? bits_A : bits_B;
    max_bits = (max_bits + 1) * 2; // plus one is for the sign bit

    char binary_A[65], binary_B[65];
    to_binary_stringg(C, bits_A, binary_A);
    to_binary_stringg(D, bits_B, binary_B);

    fprintf(output, "A=%lld=%s, B=%lld=%s\n", C, binary_A, D, binary_B);
    fprintf(output, "~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(output, "(0, ) M=");
    print_binary_with_markerr(0, max_bits / 2, max_bits / 2, output);
    fprintf(output, "|");
    print_binary_with_markerr(M, max_bits / 2, 0, output);
    fprintf(output, "\n~~~~~~~~~~~~~~~~~~~~\n");

    for (int i = 0; i < max_bits / 2; i++) {
        long long upper_bits = (M >> (max_bits / 2));
        long long lower_bits = (M & ((1LL << (max_bits / 2)) - 1));
        if (M & 1) { // If LSB is 1
            if (lower_bits & 1) { // If LSB of lower_bits is 1, subtract A
                fprintf(output, "(%d,-) M=", i + 1);
                upper_bits += neg_A;
            } else { // Else add A
                fprintf(output, "(%d,+) M=", i + 1);
                upper_bits += A;
            }
            M = (upper_bits << (max_bits / 2)) | lower_bits;
            print_binary_with_markerr(M, max_bits, max_bits / 2, output);
            fprintf(output, "\n");
        } else {
            fprintf(output, "(%d, ) M=", i + 1);
            print_binary_with_markerr(M, max_bits, max_bits / 2, output);
            fprintf(output, "\n");
        }
        // Right shift with sign extension
        long long sign_bit = M & (1LL << (max_bits - 1));
        M >>= 1;
        if (sign_bit) {
            M |= (1LL << (max_bits - 1));
        }
        fprintf(output, "(%d,>) M=", i + 1);
        print_binary_with_markerr(M, max_bits, max_bits / 2 - 1, output);
        fprintf(output, "\n~~~~~~~~~~~~~~~~~~~~\n");
    }

    long long result = from_twos_complementt(M, max_bits);
    if (sign == -1) {
        result = -result;
    }

    fprintf(output, "M=AxB=%+lld\n\n", result);
}




//-----------------------------------------------------------------------------------------------------

// Booth's algorithm functions
void binary(int a, int b, int *anum, int *anumcp, int *bnum, int *acomp, int *bcomp, int *res, int *com) {
    int a1 = abs(a);
    int b1 = abs(b);
    int r, r2, i;
    // Convert to binary
    for (i = 0; i < 8; i++) {
        r = a1 % 2;
        a1 = a1 / 2;
        r2 = b1 % 2;
        b1 = b1 / 2;
        anum[i] = r;
        anumcp[i] = r;
        bnum[i] = r2;
        if (r2 == 0) {
            bcomp[i] = 1;
        }
        if (r == 0) {
            acomp[i] = 1;
        }
    }

// Compute two's complement for b
    int c = 0;
    for (i = 0; i < 8; i++) {
        res[i] = com[i] + bcomp[i] + c;
        if (res[i] >= 2) {
            c = 1;
        } else {
            c = 0;
        }
        res[i] = res[i] % 2;
    }
    for (i = 7; i >= 0; i--) {
        bcomp[i] = res[i];
    }

// Set bnum and bcomp for negative a
    if (a < 0) {
        c = 0;
        for (i = 7; i >= 0; i--) {
            res[i] = 0;
        }
        for (i = 0; i < 8; i++) {
            res[i] = com[i] + acomp[i] + c;
            if (res[i] >= 2) {
                c = 1;
            } else {
                c = 0;
            }
            res[i] = res[i] % 2;
        }
        for (i = 7; i >= 0; i--) {
            anum[i] = res[i];
            anumcp[i] = res[i];
        }
    }

    // Set bnum and bcomp for negative b
    if (b < 0) {
        for (i = 0; i < 8; i++) {
            int temp = bnum[i];
            bnum[i] = bcomp[i];
            bcomp[i] = temp;
        }
    }
}

void add(int num[], int *pro, int *res) {
    //c=carry
    int i, c = 0;
    for (i = 0; i < 8; i++) {
        res[i] = pro[i] + num[i] + c;
        if (res[i] >= 2) {
            c = 1;
        } else {
            c = 0;
        }
        res[i] = res[i] % 2;
    }
    for (i = 7; i >= 0; i--) {
        pro[i] = res[i];
    }
}

void arshift(int *pro, int *anumcp) {
    //temp stores the most significant bit (MSB) of pro to remain in place after the right shift.
    int temp = pro[7], temp2 = pro[0], i;
    for (i = 1; i < 8; i++) {
        //Right shift ring
        pro[i - 1] = pro[i];
    }
    pro[7] = temp;
    for (i = 1; i < 8; i++) {
        anumcp[i - 1] = anumcp[i];
    }
    anumcp[7] = temp2;
}

void print_status(int step, char action, int shift, int *pro, int *anumcp, FILE *output) {
    fprintf(output, "(%d,%c) M=", step, action);
    for (int i = 7; i >= 0; i--) {
        fprintf(output, "%d", pro[i]);
    }
    fprintf(output, "|");
    for (int i = 7; i >= 0; i--) {
        fprintf(output, "%d", anumcp[i]);
    }
    if (shift) {
        fprintf(output, "\n(%d,>) M=", step);
        for (int i = 7; i >= 0; i--) {
            fprintf(output, "%d", pro[i]);
        }
        fprintf(output, "|");
        for (int i = 7; i >= 0; i--) {
            fprintf(output, "%d", anumcp[i]);
        }
    }
    fprintf(output, "\n~~~~~~~~~~~~~~~~~~~~\n");
}

void booth_multiplication(int a, int b, FILE *output) {
    int anum[8] = {0}, anumcp[8] = {0}, bnum[8] = {0};
    int acomp[8] = {0}, bcomp[8] = {0}, pro[8] = {0}, res[8] = {0};
    int com[8] = {1, 0, 0, 0, 0, 0, 0, 0};

    binary(a, b, anum, anumcp, bnum, acomp, bcomp, res, com);

    // Find the number of bits required to represent the numbers
    int bits_A = 0, bits_B = 0;
    int temp_A = abs(a), temp_B = abs(b);
    while (temp_A > 0) {
        bits_A++;
        temp_A >>= 1;
    }
    while (temp_B > 0) {
        bits_B++;
        temp_B >>= 1;
    }
    int max_bits = bits_A > bits_B ? bits_A : bits_B;

    char binary_A[33], binary_B[33];
    to_binary_string(abs(a), max_bits, binary_A);
    to_binary_string(abs(b), max_bits, binary_B);

    fprintf(output, "A=%d=%s, B=%d=%s\n", a, binary_A, b, binary_B);

    int i, q = 0;
    for (i = 0; i < 8; i++) {
        //If the current bit (anum[i]) is equal to the previous bit (q), that is: 00 or 11
        if (anum[i] == q) {
            print_status(i, ' ', 1, pro, anumcp, output);
            arshift(pro, anumcp);
            //The previous bit (q) is updated with the current bit (anum[i]).
            q = anum[i];
            //If the current bit (anum[i]) is equal to 1 and the previous bit (q) is equal to 0, that is: 01
        } else if (anum[i] == 1 && q == 0) {
            print_status(i, '+', 0, pro, anumcp, output);
            add(bcomp, pro, res);
            print_status(i, '+', 1, pro, anumcp, output);
            arshift(pro, anumcp);
            q = anum[i];
            //If the current bit (anum[i]) is equal to 0 and the previous bit (q) is equal to 1, that is:10
        } else {
            print_status(i, '-', 0, pro, anumcp, output);
            add(bnum, pro, res);
            print_status(i, '-', 1, pro, anumcp, output);
            arshift(pro, anumcp);
            q = anum[i];
        }
    }

    fprintf(output, "M=AxB=");
    for (i = 7; i >= 0; i--) {
        fprintf(output, "%d", pro[i]);
    }
    for (i = 7; i >= 0; i--) {
        fprintf(output, "%d", anumcp[i]);
    }

    fprintf(output, "\nM=AxB= %d\n", a * b);
    fprintf(output, "\n");
}




//--------------------------------------------------------------------------------------------------------
// Main function
int main() {
    FILE *input = fopen("in.txt", "r");
    FILE *output = fopen("out.txt", "w");
    
    if (input == NULL || output == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int n;
    fscanf(input, "%d", &n);
    
    for (int i = 0; i < n; i++) {
        fprintf(output, "out-%d\n", i);
        int m, b, s, A, B;
        fscanf(input, "%d", &m);
        fscanf(input, "%d", &b);
        fscanf(input, "%d", &s);
        fscanf(input, "%d", &A);
        fscanf(input, "%d", &B);

        if (m == 0) {
            if (s == 0) {
                fprintf(output, "unsigned add & shift multiplication\n");
                add_and_shift_positive(A, B, output);
                
            } else if (s == 1) {
                fprintf(output, "signed add & shift multiplication\n");
                add_and_shift_mixed(A, B, output);
            } else if (s == 2) {
                fprintf(output, "signed add & shift multiplication\n");
                add_and_shift_negative(A, B, output);
            }
        } else if (m == 1) {
            fprintf(output, "Booth's Algorithm\n");
            booth_multiplication(A, B, output);
        }

    }

    fclose(input);
    fclose(output);

    return 0;
}
