#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
using namespace std;
ifstream inputFile;
ofstream outputFile;
ofstream resultFile;
vector<char> inputBitData;
vector<char> outputBitData;
int inputDataNum;
int errorDataNum;
void readFile();
void writeFile(char* generator, int dwSize);
void checkError(char* codeword,char* generator, int dwSize);
void xorCal(char* word, char* div);
int main(int argc, char* argv[]){
    if(argc!=6){
        cout << "usage : ./crc_decoder input_file output_file result_file generator dataword_size\n";
        return 0;
    }//argument check
    inputFile.open(argv[1]);
    if(inputFile.is_open()==false){
        cout << "input file open error.\n";
        return 0;
    }
    outputFile.open(argv[2]);
    if(outputFile.is_open()==false){
        cout << "output file open error.\n";
        return 0;
    }
    resultFile.open(argv[3]);
    if(resultFile.is_open()==false){
        cout << "result file open error.\n";
        return 0;
    }
    //file open check
    if(atoi(argv[5])!=4&&atoi(argv[5])!=8)
    {
        cout << "dataword size must be 4 or 8.\n";
        return 0;
    }
    readFile(); // inputFile 읽기
    // for(auto k : inputBitData){
    //     cout << k;
    // }cout << "\n";
    errorDataNum=0;
    writeFile(argv[4],atoi(argv[5]));//제네레이터랑 데이터사이즈 주기
    resultFile << inputDataNum << " " << errorDataNum <<"\n";
    inputFile.close();
    outputFile.close();
    resultFile.close();
    return 0;
}

void readFile(){//input File을 char로 읽어서 bit변환 후 inputBitData에 저장
    inputBitData.clear();
    outputBitData.clear();//벡터 초기화
    char c;
    int paddingSize;
    inputFile.get(c);//패딩사이즈 읽
    paddingSize = c;
    if(paddingSize<0||paddingSize>=8){
        cout << "ERROR paddingSize is not 0<=size<8!\n";
        exit(0);
    }
    int nowp=0;
    while(inputFile.get(c)){
        for(int bit=128;bit>0;bit>>=1){
            if(nowp<paddingSize){
                nowp++;
                continue;
            }
            if(c&bit){
                inputBitData.push_back('1');
            }else{
                inputBitData.push_back('0');
            }
        }
    }
    return ;
}
void writeFile(char* generator, int dwSize){
    int genLength = strlen(generator);//제네레이터 길이. 코드워드 길이는 dwSize + genLen - 1이겠지
    int cwSize = dwSize + genLength - 1;
    int endofInBit = inputBitData.size();
    inputDataNum = endofInBit/cwSize;
    if(endofInBit%cwSize!=0){
        cout<<"InputData Size Error\n";
        exit(0);
    }
    int inputPointer=0;
    char codeword[250];
    char outputCharacter=0;
    int outputSize=0;
    while(inputPointer<endofInBit){
        for(int j=0;j<cwSize;j++){
            if(j<dwSize){
                outputCharacter<<=1;
                if(inputBitData[inputPointer]=='1'){
                    outputCharacter|=1;
                }
                outputSize++;
            }
            codeword[j] = inputBitData[inputPointer];
            inputPointer++;

        }
        codeword[cwSize] = '\0';
        checkError(codeword,generator,dwSize);
        if(outputSize==8){
            //printf("%c\n",outputCharacter);
            outputFile << outputCharacter;
            outputCharacter = 0;
            outputSize = 0;
            outputFile.flush();
        }
    }
}
void checkError(char* codeword,char* generator, int dwSize){
    char remainderword[250];
    strcpy(remainderword, codeword);
    int cdSize = strlen(codeword);
    for(int i=0;i<dwSize;i++){
        if(remainderword[i]=='0')continue;
        xorCal(remainderword+i,generator);
    }
    for(int i=0;i<cdSize;i++){
        if(remainderword[i]!='0'){
            errorDataNum ++;
            return ;
        }
    }
    return ;
}
void xorCal(char* word, char* div){//word를 div랑 xor한 결과를 word에 저장하자.
    int divLength = strlen(div);
    for(int i=0;i<divLength;i++){
        word[i] = (word[i]==div[i]) ? '0' : '1';
    }
}
