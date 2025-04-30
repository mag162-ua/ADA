// MARTÍN AZNAR GARCÍA 51797315G

#include <iostream>
#include <cmath>

using namespace std;

int pasos=0;

// coste: \Theta( n^2 )
unsigned long pow2_1(unsigned n){ //Recursión
    pasos+=1;
    unsigned long resultado = 1;

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            pasos+=1;
            if(j==n-1){
                resultado*=2;
            }
        }
    }
    return resultado;
}

// coste: \Theta( n )
unsigned long pow2_2(unsigned n){ //Iterativo

    unsigned long resultado = 1;

    while (n > 0) {
        pasos+=1;
        resultado *= 2;
        n--;
        
    }
    pasos+=1;
    return resultado;
}

// coste: \Theta( log2(n) )
unsigned long pow2_3(unsigned n){
    unsigned long resultado = 1;
    unsigned long base = 2;

    while (n > 0) {
        //cout<<"n: "<<n<<" r: "<<resultado<<" base: "<<base<<endl;
        pasos+=1;
        if (n % 2 == 1) { 
            resultado *= base;
            pasos+=1;
            
        }
        base *= base; 
        n /= 2;
    }
    pasos+=1;
    return resultado;
}

int main(){

    cout << "# Comparación diferentes complejidades temporales."<<endl
        << "# Pasos de programa, calculo 2^n"<< endl
        << "# ------------------------------------------------"<<endl
        << "#  n \t ( n^2 ) \t ( n ) \t ( log2(n) )"<<endl
        << "#================================================="
        << endl;

    //grafica de 10 a 20 valores

    for(int exp=1;exp<=20;exp++){

        //int n=2*exp;
        cout<<"   "<<exp<<"\t";
        
        pasos=0;
        int p1=pow2_1(exp);
        cout<<"   "<<pasos<<"\t";
        if(pasos<10000){
            cout<<"\t";
        }
        //cout<<pasos<<endl;

        if(!(pow(2,exp)==p1)){
            cout<<"Error, valor obtenido incorrecto, pow2_1, "<<endl;
            return -1;
        }

        pasos=0;
        int p2=pow2_2(exp);
        cout<<"   "<<pasos<<"\t";

        if(!(pow(2,exp)==p2)){
            cout<<"Error, valor obtenido incorrecto, pow2_2"<<endl;
            return -1;
        }

        pasos=0;
        int p3=pow2_3(exp);
        //cout<<pasos<<endl;
        cout<<"\t"<<pasos<<"\t";

        if(!(pow(2,exp)==p3)){
            cout<<"Error, valor obtenido incorrecto, pow2_3"<<endl;
            return -1;
        }
        
        cout<<endl;
    }

    return 0;
}