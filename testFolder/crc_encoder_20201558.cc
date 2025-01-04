#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
using namespace std;
ifstream inputFile;
ofstream outputFile;
vector<char> inputBitData;
vector<char> outputBitData;
void readFile();//파일 읽기
void checkGenerator(char* gn);//generator 체크해서 앞 0 없애기
void parseDataWord(char* gn, int dwSize);//데이터워드 사이즈대로 파싱해서 코드워드 만드는 함수에 넘겨주기
void makeCodeWord(char* dataword, char* generator);//코드워드로 만들기(데이터워드부분 push)
void putRemainder(char* codeword, char* generator, int dataSize);//generator로 나눈 값이 0이되게하는 나머지 push
void xorCal(char* word, char* div);//xor 연산  
void writeFile();
int main(int argc, char* argv[]){
    if(argc!=5){
        cout << "usage : ./crc_encoder input_file output_file generator dataword_size\n";
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
    //file open check
    if(atoi(argv[4])!=4&&atoi(argv[4])!=8)
    {
        cout << "dataword size must be 4 or 8.\n";
        return 0;
    }
    readFile(); // inputFile 읽기
    checkGenerator(argv[3]);//혹시 첫비트가 1이 아닌경우를 체크해서 보정해주기
    parseDataWord(argv[3],atoi(argv[4]));//dataword를 파싱해서, 다른 함수에 넘겨줌
    // for(auto k : outputBitData){
    //     cout << k;
    // }cout <<'\n';
        // for(int i=0;i<8;i++){
    //     cout << outputBitData[i] ;
    // }cout << '\n';
    writeFile();
    inputFile.close();
    outputFile.close();
    return 0;
}

void readFile(){//input File을 char로 읽어서 bit변환 후 inputBitData에 저장
    inputBitData.clear();
    outputBitData.clear();//벡터 초기화
    char c;
    while(inputFile.get(c)){
        for(int bit=128;bit>0;bit>>=1){
            if(c&bit){
                inputBitData.push_back('1');
            }else{
                inputBitData.push_back('0');
            }
        }
    }
    return ;
}

void checkGenerator(char* gn){
    char tmpgn[300];
    strcpy(tmpgn, gn);
    int gnLength = strlen(tmpgn);
    int start = 0;
    for(int i=0;i<gnLength;i++){
        if(tmpgn[i]=='1'){
            break;
        }
        start = i+1;
    }
    strcpy(gn,tmpgn+start);
    return ;
}
 
void parseDataWord(char* gn, int dwSize){//dwSize로 잘라서 generator사용해서 코드워드 만들기
    int inputMax = inputBitData.size();
    //cout << gnLenght << '\n'; // debug
    char tmpDataword[20];//최대 크기 8이지만 여유있게 설정. 어차피 local tmp data라서 ㄱㅊ을듯
    for(int start = 0;start<inputMax;start+=dwSize){
        for(int i=0;i<dwSize;i++){
            if(start+i>=inputMax){
                printf("inputBitData Size ERROR\n");
                exit(0);
            }
            tmpDataword[i] = inputBitData[start+i];
        }
        tmpDataword[dwSize] = '\0';
        //tmpDataWord에 이제 매 반복문마다 데이터워드가 들어있음. 사이즈만큼 잘라서
        //이걸 이제 generator사용해서 codeword만들어서 다시 넣으면 됨.
        makeCodeWord(tmpDataword, gn);
    }
}
void makeCodeWord(char* dataword, char* generator){
    //cout << datwaord <<' '<< strlen(dataword) << '\n'; // debug
    int dataLength = strlen(dataword);
    char tmpCodeword[250];//dataword 4 or 8 , generator max 200
    strcpy(tmpCodeword,dataword);
    int genLength = strlen(generator);
    for(int i = 1; i < genLength; i++){
        strcat(tmpCodeword,"0");
    }
    //일단 뒤에 generator - 1 길이만큼 0 붙여둠
    //이제 generator로 나눠서 나머지를 찾으면 될듯
    //cout << tmpCodeword ;//debug
    for(int i=0;i<dataLength;i++){
        outputBitData.push_back(dataword[i]);
    }
    putRemainder(tmpCodeword, generator, dataLength);
}
void putRemainder(char* codeword, char* generator, int dataSize){
    int genLength = strlen(generator);
    int codeLength = dataSize + genLength - 1 ;
    char remainderword[250]; // generator max 200 dataword 4 or 8
    strcpy(remainderword, codeword);
    for(int i=0;i<dataSize;i++){
        if(remainderword[i]=='0')continue;
        xorCal(remainderword+i,generator);
    }
    //cout <<" "<<remainderword << '\n';//debug
    for(int i=0;i<codeLength;i++){
        if(i<dataSize&&remainderword[i]=='1'){
            cout << "divide Error ?!\n";
            exit(0);
        }
        if(i>=dataSize){
            outputBitData.push_back(remainderword[i]);
        }
    }
}
void xorCal(char* word, char* div){//word를 div랑 xor한 결과를 word에 저장하자.
    int divLength = strlen(div);
    for(int i=0;i<divLength;i++){
        word[i] = (word[i]==div[i]) ? '0' : '1';
    }
}
void writeFile(){
    int outputSize = outputBitData.size();
    int paddingSize = (8 - (outputSize%8))%8;
    //padding에서 첫 8비트는 숫자. 뒤의 n비트는 숫자가 의미하는만큼의 '0'
    char outputCharacter;
    //printf("%d\n",outputCharacter);
    //cout << "[ "<< outputCharacter <<"]\n";//debug
    outputCharacter = char(paddingSize);
    //printf("%d\n",outputCharacter);
    outputFile << outputCharacter;
    //printf("%x\n",outputCharacter);
    int outputPointer=0;
    outputCharacter = 0;
    // for(int i=0;i<8;i++){
    //     cout << outputBitData[i] ;
    // }cout << '\n';
    for(int i=paddingSize;i<8;i++){
        outputCharacter <<= 1;
        if(outputBitData[outputPointer]=='1'){
            outputCharacter |= 1;
        }
        outputPointer++;
    }//패딩 + Alpha로 첫비트 작성
    outputFile << outputCharacter;
    //printf("padding is %d\n",paddingSize);
    int endofOutBit = outputBitData.size();
    while(outputPointer<endofOutBit){
        outputCharacter = 0;
        if((endofOutBit-outputPointer)%8!=0){
            cout << "data lenght Error \n";
            exit(0);
        }
        for(int i=0;i<8;i++){
            outputCharacter <<= 1;
            if(outputBitData[outputPointer]=='1'){
                outputCharacter |= 1;
            }
            outputPointer++;
        }
        outputFile<<outputCharacter;
        //printf("%d %d %d\n",outputPointer,endofOutBit,outputCharacter);
    }
}
