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
#include <math.h>

#define WWRAND (double)rand()/RAND_MAX

using namespace std;


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
    s1 = (rand() % (solution.size()-1))+1;
    s2 = (rand() % (solution.size()-1))+1;
    
   
    while(s1 == s2){
        s2=rand() % (solution.size()-1)+1;
    }
    assert(s1 > 0);
    assert(s2 > 0);
    assert( s1 != s2);
    
    tempc = solution.at(s1);
    solution.at(s1)=solution.at(s2);
    solution.at(s2)=tempc;
    
}

vector<city> make_city_table(int num_city){ // creates x num of cities with random positions between 0-100
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
    fitness = 0;
    double distance;
    for(int j=0; j<num_city-1; j++){
        tempc1=city_table.at(solution.at(j));
        tempc2=city_table.at(solution.at(j+1));
        distance = sqrt(pow((tempc2.xpos-tempc1.xpos), 2) + pow((tempc2.ypos-tempc1.ypos), 2));
        fitness = fitness + distance;
    }
    //cout << city_table.at(3).xpos << "\t" << city_table.at(3).ypos << endl;
    
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

vector<policy> EA_replicate(vector<policy> P, int popsize, int maxr){
    vector<policy> population;
    population = P;
    assert(population.size() == popsize/2);
    while(population.size()<popsize){
        int spot = rand()%population.size();
        policy A;
        A = population.at(spot);
        //cout << "before mutate" << endl;
        for(int h=0; h<10; h++){
            //cout << A.solution.at(h) << "\t";
        }
        //cout << endl;
        int r = rand() % (maxr+1) +1;
        if( r== 1 || r==2){
            r=1;
        }
        else{
            r=2;
        }
        for(int i=0; i<r; i++){
            
            //cout << "after mutate" << endl;
        A.mutate();
            for(int h=0; h<10; h++){
                //cout << A.solution.at(h)<<"\t";
            }
            //cout<< endl;
        }
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

vector<policy> EA_downselect(vector<policy> P, int pop_size){ // binary tournament
    vector<policy> population;
    while(population.size() < pop_size / 2){
        int s1 = rand()%P.size();
        int s2 = rand()%P.size();
        while(s2 == s1){
            s2 = rand()%P.size();
        }
        double fit1 = P.at(s1).fitness;
        double fit2 = P.at(s2).fitness;
        
        if(fit1>fit2){
            policy A = P.at(s2);
            population.push_back(A);
        }
        else if(fit1<=fit2){
            policy B = P.at(s1);
            population.push_back(B);
        }
    }
    
    assert(population.size() == pop_size/2);
    return population;
}



void LC(int num_city, int pop_size, int generations){
    vector<policy> pop;
    pop = EA_init(pop_size/2, num_city);
    vector<city> master = make_city_table(num_city);
    
    int maxr=2;
    ofstream outputFile;
    outputFile.open("LCmainout.txt");
    for(int n = 0; n < generations; n++){
        pop = EA_replicate(pop,pop_size, maxr);
        assert(pop.size() == pop_size);
        pop = EA_evaluate(pop, pop_size, master, num_city);
        assert(pop.size() == pop_size);
        if(outputFile.is_open()){
            for(int i=0; i<pop.size(); i++){
                outputFile << pop.at(i).fitness << "\t";
            }
            
            outputFile << "\t" << "\t" <<  endl;
        }
        pop = EA_downselect(pop,pop_size);
        
        
        
        assert(pop.size() == pop_size/2);
        for(int k=0; k<pop.size(); k++){
        assert(pop.at(k).solution.at(0) == 0);
        
        }
        
    }
    cout << "End LC" << endl;
    
    outputFile.close();
    
}

void HL2(int pop_size, int generations){
    vector<policy> pop;
    int num_city=10;
    pop = EA_init(pop_size/2, num_city);
    int maxr=2;
    city A;
    vector<city> master;
    for(int c=0; c<num_city; c++){
        A.xpos=1*c;
        A.ypos=25;
        master.push_back(A);
    }
    ofstream outputFile;
    outputFile.open("LC2out.txt");
    for(int n = 0; n < generations; n++){
        pop = EA_replicate(pop,pop_size, maxr);
        assert(pop.size() == pop_size);
        pop = EA_evaluate(pop, pop_size, master, num_city);
        assert(pop.size() == pop_size);
        pop = EA_downselect(pop,pop_size);
        assert(pop.size() == pop_size/2);
        if(outputFile.is_open()){
            for(int i=0; i<pop.size(); i++){
                outputFile << pop.at(i).fitness << "\t";
            }
            
            outputFile << "\t"<<"\t" << endl;
        }
        
        
        
    }
    
    
    cout << "End HL2" << endl;
    
    outputFile.close();
    

}
void HL3(int pop_size, int generations){
    int num_city=25;
    vector<policy> pop;
    pop = EA_init(pop_size/2, num_city);
    vector<city> master = make_city_table(num_city);
    int maxr=5;
    ofstream outputFile;
    outputFile.open("LC3out.txt");
    for(int n = 0; n < generations; n++){
        pop = EA_replicate(pop,pop_size, maxr);
        assert(pop.size() == pop_size);
        pop = EA_evaluate(pop, pop_size, master, num_city);
        assert(pop.size() == pop_size);
        pop = EA_downselect(pop,pop_size);
        assert(pop.size() == pop_size/2);
        if(outputFile.is_open()){
            for(int i=0; i<pop.size(); i++){
                outputFile << pop.at(i).fitness << "\t";
            }
            
            outputFile << "\t"<<"\t" << endl;
        }
        
        
        
    }
    cout << "End HL3" << endl;
    
    outputFile.close();
    
}


void HL4(int pop_size, int generations){
    int num_city=100;
    vector<policy> pop;
    pop = EA_init(pop_size/2, num_city);
    vector<city> master = make_city_table(num_city);
    int maxr=20;
    ofstream outputFile;
    outputFile.open("LC4out.txt");
    for(int n = 0; n < generations; n++){
        pop = EA_replicate(pop,pop_size, maxr);
        assert(pop.size() == pop_size);
        pop = EA_evaluate(pop, pop_size, master, num_city);
        assert(pop.size() == pop_size);
        pop = EA_downselect(pop,pop_size);
        assert(pop.size() == pop_size/2);
        if(outputFile.is_open()){
            for(int i=0; i<pop.size(); i++){
                outputFile << pop.at(i).fitness << "\t";
            }
            
            outputFile << "\t"<<"\t" << endl;
        }
        
        
        
    }
    cout << "End HL4" << endl;
    
    outputFile.close();
    
}


int main() {
    cout << "Main Start" << endl;
    srand(unsigned(time(NULL)));
    vector<policy> pop;
    int num_city=10;
    int pop_size = 100;
    int generations = 300;
   
    LC(num_city, pop_size, generations);
    HL2(pop_size, generations);
    HL3(pop_size, generations );
    HL4(pop_size, generations);
    cout << "End Main" << endl;
    

    return 0;
}
