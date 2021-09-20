// MALDICION_DE_LA_DIMENSIONALIDAD.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include <queue>
#include <algorithm>

int GenRamdon() {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 1000);

    return distrib(gen);
}
struct Point
{
    std::vector<int> p;
    Point(int tam) {
        for (int i = 0; i < tam; i++)
        {
            p.push_back(GenRamdon());
        }
    }

    void Print() {
        for (int i = 0; i < p.size(); i++)
        {
            std::cout << p[i] << "  ";
        }
        std::cout << std::endl;
    }
};
float DistanciaEuclidiana(Point p1, Point p2) {
    /*Con esta Funcion tenemos la DistanciaEuclidiana para cualquier tama;o*/
    int tam = p1.p.size();
    float distancia = 0;
    for (int i = 0; i < tam; i++)
    {
        distancia += pow((p2.p[i] - p1.p[i]), 2);
    }
    return sqrt(distancia);
}
//Aqui guardamos todos los datos calculados

void CalculoMejorado(int k, int t) {
    //Primero Creamos un Punto de t dimensiones
    Point p1(t);
    //Aqui guardaremos las distancias ordenadas de menor a mayor
    std::priority_queue<float, std::vector<float>, std::greater<float>> d;
    float max = 0, min;
    //Luego hallaremos la Distancia del punto p1 de los nuevos k-1 puntos
    float d_aux;
    for (int i = 0; i < k - 1; i++)
    {
        d_aux = DistanciaEuclidiana(p1, Point(t));
        d.push(d_aux);
        if (d_aux > max)
        {
            //Aqui tendremos la maxima distancia
            max = d_aux;
        }
    }
    //Aqui tendremos la minima distancia
    min = d.top();
    std::vector<int> E(11);
    float razon = max - min;

    //PARA HACER EL TRUNCAMIENTO EN RELACION DE 1 A 0 LO HAGO DE ESTA MANERA
    for (int i = 0; i < k - 1; i++)
    {
        switch (int(trunc(((d.top() - min) / razon) * 10)))
        {
        case 0: E[0]++; break;
        case 1: E[1]++; break;
        case 2: E[2]++; break;
        case 3: E[3]++; break;
        case 4: E[4]++; break;
        case 5: E[5]++; break;
        case 6: E[6]++; break;
        case 7: E[7]++; break;
        case 8: E[8]++; break;
        case 9: E[9]++; break;
        case 10: E[10]++; break;
        }
        d.pop();
    }

    for (int i = 0; i < E.size(); i++)
    {
        std::cout << i << "->" << E[i] << std::endl;
    }
}
int main()
{
    std::cout << "Para dimension = " << 2 << std::endl;
    CalculoMejorado(20000, 2);
    std::cout << "Para dimension = " << 5 << std::endl;
    CalculoMejorado(20000, 5);
    std::cout << "Para dimension = " << 10 << std::endl;
    CalculoMejorado(20000, 10);
    std::cout << "Para dimension = " << 15 << std::endl;
    CalculoMejorado(20000, 15);
    std::cout << "Para dimension = " << 20 << std::endl;
    CalculoMejorado(20000, 20);
    std::cout << "Para dimension = " << 25 << std::endl;
    CalculoMejorado(20000, 25);
}
