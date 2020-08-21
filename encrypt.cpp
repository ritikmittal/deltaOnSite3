#include <bits/stdc++.h>
using namespace std;

uint8_t Rcon[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

uint8_t S_box[][16] = {
        { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
        { 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
        { 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
        { 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
        { 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
        { 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
        { 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
        { 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
        { 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
        { 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
        { 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
        { 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
        { 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
        { 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
        { 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
        { 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
};


uint8_t multiply(uint8_t x, uint8_t y){
    uint16_t mod = 0x011b;
    uint16_t temp = y,ans = 0;
    for(int i=0;i<8;i++){
        if((1<<i) & x){
            ans = ans ^ (temp << i);
        }
    }
    for(int i=15;i>=8;i--){
        if(ans & (1<<i)){
            ans ^= (mod << (i - 8));
        }
    }
    return (uint8_t(ans));
}

void AddRoundKey(uint8_t * current_state, uint8_t word[][4], int n){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            current_state[i*4 + j] ^= word[n+i][j];
        }
    }
};
void SubstituteByte(uint8_t *current_state){
    for(uint8_t i=0;i<16;i++){
        uint8_t j = current_state[i];
        current_state[i] = S_box[j>>4][j&0x0f];
    }
};

void ShiftRows(uint8_t *current_state){
    int k = 1;
    uint8_t temp[16];
    for(int i=0;i<16;i++)
        temp[i] = current_state[i];
    for(int i=0;i<4;i++){
        current_state[k] = temp[(k+4)%16];
        k+=4;
    }
    k=2;
    for(int i=0;i<4;i++){
        current_state[k] = temp[(k+8)%16];
        k+=4;
    }
    k=3;
    for(int i=0;i<4;i++){
        current_state[k] = temp[(k+12)%16];
        k+=4;
    }
};

void MixingColumns(uint8_t *current_state){
    uint8_t mat[4][4] = {
            {0x2, 0x3, 0x1, 0x1},
            {0x1, 0x2, 0x3, 0x1},
            {0x1, 0x1, 0x2, 0x3},
            {0x3, 0x1, 0x1, 0x2}
    };
    for(int i=0;i<16;i+=4){
        uint8_t temp[4] = {0};
        for(int j=0;j<4;j++){
            for(int m=0;m<4;m++){
                temp[j] ^= multiply(mat[j][m],current_state[i+m]);
            }
        }
        for(int j=0;j<4;j++)
            current_state[i+j] = temp[j];
    }
};

void KeyExpension(uint8_t key[16], uint8_t word[44][4]){
    uint8_t temp[4];
    int index = 0;
    for(;index<4;index++){
        for(int j=0;j<4;j++){
            word[index][j] = key[4*index + j];
        }
    }
    for(;index<44;index++){
        for(int i=0;i<4;i++){
            temp[i] = word[index - 1][i];
        }
        if(index%4 == 0){
            uint8_t temp2[4];
            for(int i=0;i<4;i++){
                temp2[i] = temp[i];
            }
            for(int i=0;i<4;i++){
                temp[i] = temp2[(i+1)%4];
            }
            for(int i=0;i<4;i++){
                int j = temp[i];
                temp[i] = S_box[j>>4][j&0x0f];
            }
            temp[0] = temp[0] ^ Rcon[(index/4)-1];
        }
        for(int i=0;i<4;i++)
            word[index][i] = word[index-4][i] ^ temp[i];
    }

}


void cipher(uint8_t input[16], uint8_t output[16],uint8_t word[44][4]){
    uint8_t current_state[16];
    for(int i=0;i<16;i++){
        current_state[i] = input[i];
    }
    AddRoundKey(current_state, word,0);
    for(int i=0;i<9;i++){
        SubstituteByte(current_state);
        ShiftRows(current_state);
        MixingColumns(current_state);
        AddRoundKey(current_state, word, 4*(i+1));
    }
    SubstituteByte(current_state);
    ShiftRows(current_state);
    AddRoundKey(current_state, word, 40);
    for(int i=0;i<16;i++)
        output[i] = current_state[i];
};

string base64Encoder(uint8_t input_str[], int len_str)
{
    char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int len_out = (len_str*4)/3 + 4;
    string res_str(len_out,'\0');
    int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp;
    int i, j, k = 0;
    for (i = 0; i < len_str; i += 3){
        val = 0, count = 0, no_of_bits = 0;
        for (j = i; j < len_str && j <= i + 2; j++){
            val = val << 8;
            val = val | input_str[j];
            count++;
        }
        no_of_bits = count * 8;
        padding = no_of_bits % 3;
        while (no_of_bits != 0){
            if (no_of_bits >= 6){
                temp = no_of_bits - 6;
                index = (val >> temp) & 63;
                no_of_bits -= 6;
            } else{
                temp = 6 - no_of_bits;
                index = (val << temp) & 63;
                no_of_bits = 0;
            }
            res_str[k++] = char_set[index];
        }
    }
    for (i = 1; i <= padding; i++){
        res_str[k++] = '=';
    }
    res_str[k] = '\0';
    return res_str;
}

int main(){
    uint8_t aes_input[16],aes_output[16], key[16];
    string str,k;
    cout<<"Enter the key: ";
    getline(cin,k);
    if(k.length() != 16) {
        cout << "not a valid key of 16 byte" << endl;
        return 0;
    }
    for(int i=0;i<16;i++){
        key[i] = k[i];
    }
    uint8_t word[44][4];
    KeyExpension(key,word);
    cout<<"enter the string"<<endl;
    getline(cin,k);
    int l = str.length() + 1;
    int len_out = (int)ceil(l/16.0)*16;
    uint8_t aes_out[len_out];
    for(int i=0;i<l;i+=16){
        int j = 0;
        if((l-i)%16 == 0) {
            for (; j < 16; j++) {
                aes_input[j] = (uint8_t) str[i + j];
            }
        } else {
            for (; (i+j) < l; j++) {
                aes_input[j] = (uint8_t) str[i + j];
            }
            for(;j<16;j++){
                aes_input[j] = 0;
            }
        }
        cipher(aes_input,aes_output, word);
        for(int j=0;j<16;j++){
            aes_out[i+j] = aes_output[j];
        }
    }
    cout<<base64Encoder(aes_out,len_out)<<endl;
    return 0;
}
