//
//  main.cpp
//  Projct Gamma
//
//  Created by Wade Wilson on 3/15/17.
//  Copyright © 2017 Wade Wilson. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
#include <assert.h>
#include <cmath>
#include <fstream>

#define WWRAND (double)rand()/RAND_MAX

using namespace std;

class grid{  // not needed?
public:
    int xmin;
    int ymin;
    int xmax;
    int ymax;
};
class city{
public:
    double xpos;
    double ypos;
    
    void init();
};
class policy{
public:
    //int num_city;
    //int city;
    int start_city=0;
    double fitness;
    void mutate();
    void init(int num_city);
    void eval(vector<city> city_table, int num_city);
    void downselect();
    void repop();
    
    vector<int> solution;
};

void city::init(){
    xpos=WWRAND*100;
    ypos=WWRAND*100;
}

void policy::init(int num_city){ // creates 1 policy of random order with 0 first
    int tcity;
    for(int k=0; k<num_city;k++){
        tcity = k;
        solution.push_back(tcity);
    }
    int iterations =100000;
    for(int j=0;j<iterations;j++){
        mutate();
    }
    
}

void policy::mutate(){ // swaps 2 randoms values that arent 0
    int s1;
    int s2;
    int tempc;
    s1 = (rand() % 5)+1;//solution.size()-1)+1;
    s2 = (rand() % 5)+1; //solution.size()-1)+1;
    
    tempc = solution.at(s1);
    solution.at(s1)=solution.at(s2);
    solution.at(s2)=tempc;
    
}

vector<city> city_table(int num_city){ // creates x num of cities with random positions between 0-100
    city A;
    vector<city> cities;
    for(int c=0; c<num_city; c++){
        A.init();
        cities.push_back(A);
    }
    return cities;
}

void policy::eval(vector<city> city_table, int num_city){
    city tempc1;
    city tempc2;
    double distance;
    for(int j=0; j<num_city-1; j++){
        tempc1=city_table.at(solution.at(j));
        tempc2=city_table.at(solution.at(j+1));
        distance = sqrt(pow(tempc2.xpos-tempc1.xpos, 2) + pow(tempc2.ypos-tempc1.ypos, 2));
        fitness = fitness + distance;
    }
}

vector<policy> EA_init(int num, int num_city){ // vector of policies
    vector<policy> population;
    
    for(int i=0; i<num; i++){
        policy A;
        A.init(num_city);
        population.push_back(A);
    }
    
    return population;
}

vector<policy> EA_replicate(vector<policy> P, int popsize){
    vector<policy> population;
    population = P;
    assert(population.size() == popsize/2);
    
    while(population.size()<popsize){
        int spot = rand()%population.size();
        policy A;
        A = population.at(spot);
        A.mutate();
        population.push_back(A);
    }
    
    assert(population.size() == popsize);
    
    return population;
}

vector<policy> EA_evaluate(vector<policy> P, int pop_size, vector<city> city_table, int num_city){
    vector<policy> population;
    population = P;
    for(int initf=0; initf<population.size(); initf++){
        population.at(initf).fitness = -1;
    }
    
    for(int i=0; i<population.size(); i++){
        population.at(i).eval(city_table, num_city);
        assert(population.at(i).fitness != -1);
    }
    
    for(int testing=0; testing<population.size(); testing++){
        assert(population.at(testing).fitness != -1);
    }
    assert(population.size() == pop_size);
    return population;
}

vector<policy> EA_downselect(vector<policy> P, int pop_size){
    vector<policy> population;
    while(population.size() < pop_size / 2){
        int spot1 = rand()%P.size();
        int spot2 = rand()%P.size();
        while(spot2 == spot1){
            spot2 = rand()%P.size();
        }
        double fit1 = P.at(spot1).fitness;
        double fit2 = P.at(spot2).fitness;
        
        if(fit1<fit2){
            policy A1 = P.at(spot1);
            population.push_back(A1);
        }
        else if(fit2<=fit1){
            policy A2 = P.at(spot2);
            population.push_back(A2);
        }
    }
    
    assert(population.size() == pop_size/2);
    return population;
}



int main() {
    cout << "Main Start" << endl;
    srand(unsigned(time(NULL)));
    vector<policy> pop;
    int num_city=10;
    int pop_size = 100;
    pop = EA_init(pop_size/2, num_city);
    
    int generations = 300;
    ofstream outputFile;
    outputFile.open("LCout.txt");
    cout << "Main Loop Start"<<endl;
    for(int n = 0; n < generations; n++){
        pop = EA_replicate(pop,pop_size);
        assert(pop.size() == pop_size);
        pop = EA_evaluate(pop, pop_size, city_table(num_city), num_city);
        assert(pop.size() == pop_size);
        if(outputFile.is_open()){
            for(int i=0; i<pop_size; i++){
                outputFile << pop.at(i).fitness << "\t";
                outputFile << "\t";
            }
        }
        outputFile << "\n" << "\n"<< endl;
        pop = EA_downselect(pop,pop_size);
        assert(pop.size() == pop_size/2);
        
        
    
    }
    cout << "End Main" << endl;
    
    outputFile.close();

    return 0;
}
