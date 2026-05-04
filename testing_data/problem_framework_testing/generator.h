#ifndef GENERATOR_H
#define GENERATOR_H
#include<random>
#include<stdint.h>
#include<string>
#include<cstring>
#include<cassert>
#include<exception>

class generator{
    class invalid_string_length_exception : public std::exception{
        const char* what() const noexcept override{
            return "[generator::nextString] len must be greater than or equal to 0";
        }
    };
    class invalid_interval_exception : public std::exception{
        const char* what() const noexcept override{
            return "[generator::nextInt] l must be less than or equal to r";
        }
    };
    class invalid_double_interval_exception : public std::exception{
        const char* what() const noexcept override{
            return "[generator::nextDouble] l must be strictly less than r";
        }
    };
    std::mt19937_64 gen;
    uint64_t splitmix64(uint64_t x){
        uint64_t z = (x += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }
public:
    generator(int argc, char** argv){
        uint64_t seed=0;
        for(int i=0;i<argc;i++){
            for(int j=0;argv[i][j];j++){
                seed+=argv[i][j];
                seed=splitmix64(seed^(seed*seed));
            }
            seed=splitmix64(seed);
        }
        gen=std::mt19937_64(seed);
    }
    int64_t nextInt(int64_t l, int64_t r){
        if(l>r) throw invalid_interval_exception();
        std::uniform_int_distribution<int64_t> rnd(l,r);
        return rnd(gen);
    }
    int64_t nextInt(int64_t n){
        return nextInt(0,n-1);
    }

    uint64_t nextUnsigned(uint64_t l, uint64_t r){
        if(l>r) throw invalid_interval_exception();
        std::uniform_int_distribution<uint64_t> rnd(l,r);
        return rnd(gen);
    }

    uint64_t nextUnsigned(uint64_t n){
        if(n==0) throw invalid_interval_exception();
        return nextUnsigned(0,n-1);
    }

    double nextDouble(double l, double r){
        if(l>=r) throw invalid_double_interval_exception();
        std::uniform_real_distribution<double> rnd(l,r);
        return rnd(gen);
    }

    double nextDouble(){
        return nextDouble(0,1);
    }
    template<class RandomIt>
    void shuffle(RandomIt begin, RandomIt end){
        int n=std::distance(begin, end);
        for(int i=0;i<n;i++){
            RandomIt u=begin+i;
            RandomIt v=begin+nextInt(i+1);
            std::swap(*u, *v);
        }
    }

    std::string nextString(char l, char r, int len){
        if(len<0) throw invalid_string_length_exception();
        std::string s;
        s.resize(len);
        for(int i=0;i<len;i++)
            s[i]=nextInt(l,r);
        return s;
    }
    std::string nextString(const char* charset, int len){
        if(len<0) throw invalid_string_length_exception();
        std::string s;
        s.resize(len);
        int charsetlen=strlen(charset);
        for(int i=0;i<len;i++)
            s[i]=charset[nextInt(charsetlen)];
        return s;
    }
};
#endif