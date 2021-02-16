//Gabriel Pivoto Ambrsósio - GEC - 1478
//Nathan Ribeiro da Cunha Azevedo - GEC - 1765

#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

// OPERAÇÕES TIPO R
// formato das operções 
//  OP   RD,  RS1, RS2
// XXX XXXXX XXXX XXXX
// 3 BITS PARA OPCODE
// 5 BITS PARA REGISTRADOR DESTINO
// 4 BITS PARA CADA REGISTRADOR FONTE

struct cache{
    int tag;
    bool valid;
    int data;
};

class virtualMachine{

    private:
        int opCode = 0;
        int rd = 0;
        int rs1 = 0; 
        int rs2 = 0;
        string mnemonicos[7] = {"SUM", "DIF", "MU", "DV", "LAND", "LOR", "LNAND"};
        string registers[10] = {"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"};
        int rValues[10];

    public:

        cache MemoriaCache[10];

        virtualMachine(){
            for(int i = 0; i < 10; i++)
                rValues[i] = i;
        }

        
        void initCache(){

            for(int i = 0; i < 5; i++){
                MemoriaCache[i].data = 0;
                MemoriaCache[i].tag = -1;
                MemoriaCache[i].valid = false;
            }
        }
        

    
        void showRegisters(){
            cout << endl;
            for(int i = 0; i < 10; i ++)
                cout << this->registers[i] << " = " << this->rValues[i] << endl;
            cout << endl;
        }

        void decimalToBinary(int *bin, int number){
            
            for (int i = 0; number > 0; i++){
                bin[i] = number%2;
                number = number/2;
            }
        }

         int fetchCache(int *instructions,int pc){

            int bin[] = {0,0,0,0};
            decimalToBinary(bin,pc);
            int tag =bin[2];
            int l = getIndex(bin);
            cout << "Valor da TAG: " << tag << endl;
            if(!(MemoriaCache[l].valid && MemoriaCache[l].tag == tag)){

                cout << "Cache Miss!" << endl;
                int pos = 0;
                for(int i = 0; i < 10; i++){
                    MemoriaCache[i].valid = true;
                    MemoriaCache[i].tag = tag;
                    MemoriaCache[i].data = instructions[pc + pos++];
                    
                }
            }
            else
                cout << "Cache Hit!" << endl;

            return MemoriaCache[l].data;
        }

        int getIndex(int *bin){
            int aux = 1;
            int index = 0;
            for (int i = 1; i >= 0; i--)
                if(bin[i] == 1){
                    index += pow(2,aux);
                    aux--;
                }
                else aux--;
            return index;
        }

        int getOpCode(int *bin){
           int aux = 2;
           opCode = 0;
            for( int i = 15; i >= 13; i--)
                if(bin[i] == 1){
                    opCode += pow(2,aux);
                    aux--;
                }
                else aux--;
            return opCode;
        }
        int getRd(int *bin){
            int aux = 4;
            rd = 0;
            for( int i = 12; i >= 8; i--){
                if(bin[i] == 1){
                    rd += pow(2,aux);
                    aux--;
                }
                else aux--;
            }
            return rd;
        }
        
        int getRs1(int *bin){
            int aux = 3;
            rs1 = 0;
            for( int i = 7; i >= 4; i--)
                if(bin[i] == 1){
                    rs1 += pow(2,aux);
                    aux--;
                }
                else aux--;
            return rs1;
        }

        int getRs2(int *bin){
            int aux = 3;
            rs2 = 0;
            for( int i = 3; i >= 0; i--)
                if(bin[i] == 1){
                    rs2 += pow(2,aux);
                    aux--;
                }
                else aux--;
            return  rs2;
        }

        void execute(int inst){

            int op = 0;
            int r1 = 0;
            int r2 = 0;
            int r3 = 0;


            int bin[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

            decimalToBinary(bin,inst);
            op = getOpCode(bin);
            r1 = getRd(bin);
            r2 = getRs1(bin);
            r3 = getRs2(bin);

            if(rd > 9 || rs1 > 9 || rs2 > 9 ){
                cout << "Registradores inválidos!" << endl;
                cout << "Nenhuma operação foi realizada!" << endl;
                return;
            }

            if(opCode > 6)
                cout << "Nenhuma operação foi realizada!" << endl;

            switch(opCode){
                case 0:
                    cout << "Operação realizada: " << mnemonicos[op] << " " << registers[r1] << ", " << registers[r2] << ", " << registers[r3] << endl;
                    rValues[r1] = rValues[r2] + rValues[r3];
                    break;
                case 1:
                    cout << "Operação realizada: " << mnemonicos[op] << " " << registers[r1] << ", " << registers[r2] << ", " << registers[r3] << endl;
                    rValues[r1] = rValues[r2] - rValues[r3];
                    break;
                case 2:
                    cout << "Operação realizada: " << mnemonicos[op] << " " << registers[r1] << ", " << registers[r2] << ", " << registers[r3] << endl;
                    rValues[r1] = rValues[r2] * rValues[r3];
                    break;
                case 3:
                    cout << "Operação realizada: " << mnemonicos[op] << " " << registers[r1] << ", " << registers[r2] << ", " << registers[r3] << endl;
                    rValues[r1] = rValues[r2] / rValues[r3];
                    break;
                case 4:
                    cout << "Operação realizada: " << mnemonicos[op] << " " << registers[r1] << ", " << registers[r2] << ", " << registers[r3] << endl;
                    rValues[r1] = rValues[r2] & rValues[r3];
                    break;
                case 5:
                    cout << "Operação realizada: " << mnemonicos[op] << " " << registers[r1] << ", " << registers[r2] << ", " << registers[r3] << endl;
                    rValues[r1] = rValues[r2] | rValues[r3];
                    break;
                case 6:
                    cout << "Operação realizada: " << mnemonicos[op] << " " << registers[r1] << ", " << registers[r2] << ", " << registers[r3] << " -> valor em complemento 2" << endl;
                    rValues[r1] = ~(rValues[r2] & rValues[r3]);
                    break;
                default:
                    cout << "OP CODE não encontrado!" << endl;
                    break;
            }

        }
};

//OPERAÇÕES REALIZADAS:

// DIF  $t8,  $t7, $t5 -> $t8 = $t7 - $t5
// 001 01000 0111 0101 -> 10357 em decimal

// SUM  $t9,  $t8, $t1 -> $t9 = $t8 + $t1
// 000 01001 1000 0001 -> 2433 em decimal

//  MU  $t3,  $t9, $t4 -> $t3 = $t9 * $
// 010 00011 1001 0100 -> 17300 em decimal

//  DV  $t0,  $t3, $t6 -> $t0 = $t3 / $t6
// 011 00000 0011 0110 -> 24630 em decimal

// LAND  $t9,  $t3, $t7 -> $t9 = $t3 & $t7
// 100  01001 0011 0111 -> 35127 em decimal

// LOR  $t0,  $t5, $t9 -> $t0 = $t5 | $t9
// 101 00000 0101 1001 -> 41049 em decimal

//LNAND $t3,  $t3, $t7 -> $t3 = ~($t3 & $t7)
// 110 00011 0011 0111 -> 49975 em decimal


int main(){

    virtualMachine vm;
    vm.initCache();

    int operacoes[] = {10357,2433,17300,24630,35127,41049,49975};
    int pc = sizeof(operacoes)/sizeof(operacoes[0]);

    for(int i = 0; i < pc; i++){

        cout << "Registradores antes da operação: " << endl;
        vm.showRegisters();
        cout << "PC = " << i << " -> ";
        int IR = vm.fetchCache(operacoes,i);
        cout << endl;
        vm.execute(IR);
        //cout << vm.MemoriaCache[i].data << endl;
        cout << "Registradores depois da operação: " << endl;
        vm.showRegisters();
    }
    cout << endl;
    return 0;
}