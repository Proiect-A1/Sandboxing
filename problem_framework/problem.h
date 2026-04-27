#ifndef PROBLEM_H
#define PROBLEM_H
#define MAX_SUBMISSION_SIZE 65536
#define CHECKER_FIRST_ARG 5
#define CHECKER_SOLUTION_ARG 4
#define CHECKER_IN_ARG 1
#define CHECKER_OUT_ARG 2
#define CHECKER_OK_ARG 3
#include<random>
#include<stdint.h>
#include<string>
#include<cassert>
#include<exception>
#include<stdio.h>
#include<stdarg.h>
#include<cstring>
#include<limits.h>
#include<stdint.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<utility>
#include<memory>

class parser{
    int line_no=1;
    std::istream* in;
public:
    enum parse_error{
        OK,
        NUM_TOO_LARGE,
        NUM_NOT_IN_RANGE,
        UNEXPECTED_SPACE,
        UNEXPECTED_EOLN,
        UNEXPECTED_EOF,
        UNEXPECTED_NONPRINT,
        OTHER
    };
    parser(){
        in=&std::cin;
    }
    parser(std::istream& in_stream){
        in=&in_stream;
    }
    int line(){
        return line_no;
    }
    char peek(){
        return in->peek();
    }
    char get(){
        char c=in->peek();
        //std::cerr<<"[parser::get()] "<<(int)c<<' '<<c<<'\n';
        if(c==(char)EOF) return c;
        if(c=='\n') line_no++;
        return in->get();
    }
    std::string readToken(bool ignore_whitespace = false){
        std::string s;
        if(ignore_whitespace){
            while(isspace(peek())) get();
        }
        while(true){
            char c=peek();
            //std::cout<<c;
            if(c==(char)EOF || isspace(c)){
                if(s.empty()){
                    s.push_back(c);
                    get();
                }
                //std::cerr<<"[parser::readToken()] "<<s<<'\n';
                return s;
            }
            s.push_back(c);
            get();
        }
        //std::cerr<<"[parser::readToken()] "<<s<<'\n';
        return s; // teoretic nenecesar
    }
    static std::pair<parse_error, uint64_t> parseUint(const std::string& s){
        //std::cerr<<"[parser::parseUint()] "<<s<<'\n';
        if(s.empty()) return std::make_pair(parse_error::OTHER, 0);
        uint64_t result=0;  
        bool first=0;
        for(char c : s){
            if(!isdigit(c)){
                if(first){
                    if(c==' ') return std::make_pair(parse_error::UNEXPECTED_SPACE, result);
                    if(c=='\n') return std::make_pair(parse_error::UNEXPECTED_EOLN, result);
                    if(c==(char)EOF) return std::make_pair(parse_error::UNEXPECTED_EOF, result);
                    if(!isprint(c)) return std::make_pair(parse_error::UNEXPECTED_NONPRINT, result);
                }
                return std::make_pair(parse_error::OTHER, result);
            }
            uint64_t digit=c-'0';
            if(result>(ULLONG_MAX-digit)/10){
                return std::make_pair(parse_error::NUM_TOO_LARGE, result);
            }
            result=result*10+digit;
        }
        return std::make_pair(parse_error::OK,result);
    }
    static std::pair<parse_error, int64_t> parseInt(const std::string& s){
        if(s.empty()) return std::make_pair(parse_error::OTHER, 0);
        bool minus=0;
        std::pair<parse_error, uint64_t> tmp;
        if(s[0]=='-') minus=1, tmp=parseUint(s.substr(1));
        else tmp=parseUint(s);
        if(tmp.first!=parse_error::OK) return std::make_pair(tmp.first, (int64_t)tmp.second);
        uint64_t result=tmp.second;
        int64_t actual_result=0;
        if(minus){
            if(result>(uint64_t)LLONG_MAX+1)
                return std::make_pair(parse_error::NUM_TOO_LARGE, (int64_t)result);
            else if(result==(uint64_t)LLONG_MAX+1)
                actual_result=LLONG_MIN;
            else 
                actual_result=-(int64_t)result;
        }
        else{
            if(result>(uint64_t)LLONG_MAX)
                return std::make_pair(parse_error::NUM_TOO_LARGE, (int64_t)result);
            actual_result=result;
        }
        return std::make_pair(parse_error::OK, actual_result);
    }
};
enum verdict_t{
    OK, WA, PE, PA, SUPER, FAIL
};
class validator{
protected:
    bool eof=0;
    parser p;
    bool peek_eof(){
        return p.peek()==(char)EOF;
    }
    char peek(){
        return p.peek();
    }
    bool require_eof;
    verdict_t translate_pe;
    verdict_t translate_wa;
public:
    validator(){
        this->eof=false;
        this->require_eof=false;
        this->translate_pe=verdict_t::FAIL;
        this->translate_wa=verdict_t::FAIL;
    }
    validator(std::istream& in){
        p=parser(in);
        this->eof=false;
    }
    virtual void vpquitf(verdict_t verdict, float points, const char* fmt, va_list args){
        if(verdict!=verdict_t::OK) /// validatoarele default pot returna doar OK sau FAIL
            verdict=verdict_t::FAIL;
        fprintf(stdout, "%.2f", points);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void vquitf(verdict_t verdict, const char* fmt, va_list args){
        if(verdict!=verdict_t::OK) /// validatoarele default pot returna doar OK sau FAIL
            verdict=verdict_t::FAIL;
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    /// mesajul de validator trebuie scris in stderr
    /// numarul de puncte (un float de regula intre 0 si 1) trebuie scris in stdout
    /// verdictul final (un int) este returnat
    /// quitf() merge doar cu FAIL, quitf(OK) poate fi apelat doar din readEof();
    virtual void pquitf(verdict_t verdict, float points, const char* fmt, ...){
        if(require_eof && verdict==verdict_t::OK && !eof){
            pquitf(verdict_t::FAIL, 0.0, "validator::readEof() was not called");
        }
        va_list args; va_start(args, fmt);
        vpquitf(verdict, points, fmt, args);
        va_end(args);
        exit(-1);
    }
    virtual void quitf(verdict_t verdict, const char* fmt, ...){
        if(require_eof && verdict==verdict_t::OK && !eof){
            quitf(verdict_t::FAIL,"validator::readEof() was not called");
        }
        va_list args; va_start(args, fmt);
        vquitf(verdict, fmt, args);
        va_end(args);
        exit(-1);
    } 
    /// returneaza FAIL daca conditia este adevarata cu mesajul adecvat
    virtual void failIf(bool condition, const char* fmt, ...){
        if(condition){
            va_list args; va_start(args, fmt);
            vquitf(translate_wa, fmt, args);
            va_end(args);
            exit(-1);
        }
    }
    int line(){
        return p.line();
    }
    /// verifica daca a ajuns la finalul stdin
    void readEof(){
        if(peek_eof()){
            eof=1;
            if(require_eof)
                quitf(verdict_t::OK, "Ok");
            else return;
        }
        char c=p.get();
        if(c==' ') quitf(translate_pe,"Expected EOF, found space (line %d)", line());
        if(c=='\n') quitf(translate_pe,"Expected EOF, found EOLN (line %d)", line());
        if(isprint(c)) quitf(translate_pe,"Expected EOF, found '%c' (line %d)", c, line());
        quitf(translate_pe,"Expected EOF, found ascii character %d (line %d)", c, line());
    }
    /// citeste un caracter din charset
    char readChar(const char* charset){ /// daca charset = NULL, atunci se accepta orice caracter
        if(peek_eof()) quitf(translate_pe, "Expected character from charset \"%s\", found EOF (line %d)", charset, line());
        char c=p.get();
        if(charset!=NULL && !strchr(charset, c)){
            if(c==' ') quitf(translate_pe,"Expected character from charset \"%s\", found space (line %d)", line(), charset);
            if(c=='\n') quitf(translate_pe,"Expected character from charset \"%s\", found EOLN (line %d)", line(), charset);
            if(c==EOF) quitf(translate_pe,"Expected character from charset \"%s\", found EOF (line %d)", line(), charset);
            if(isprint(c)) quitf(translate_pe,"Expected character from charset \"%s\", found '%c' (line %d)", c, line(), charset);
            quitf(translate_pe,"Expected character from charset \"%s\", found ascii character %d (line %d)", c, line(), charset);
        }
        return c;
    }
    /// se citeste un spatiu
    char readSpace(){
        if(peek_eof()) quitf(translate_pe, "Expected space, found EOF (line %d)", line());
        char c=p.get();
        if(c!=' '){
            if(c==' ') quitf(translate_pe,"Expected space, found space (line %d)", line());
            if(c=='\n') quitf(translate_pe,"Expected space, found EOLN (line %d)", line());
            if(c==EOF) quitf(translate_pe,"Expected space, found EOF (line %d)", line());
            if(isprint(c)) quitf(translate_pe,"Expected space, found '%c' (line %d)", c, line());
            quitf(translate_pe,"Expected space, found ascii character %d (line %d)", c, line());
        }
        return c;
    }
    /// se citeste Eoln
    char readEoln(){
        if(peek_eof()) quitf(translate_pe, "Expected EOLN, found EOF (line %d)", line());
        char c=p.get();
        if(c!='\n'){
            if(c==' ') quitf(translate_pe,"Expected EOLN, found space (line %d)", line());
            if(c=='\n') quitf(translate_pe,"Expected EOLN, found EOLN (line %d)", line());
            if(c==EOF) quitf(translate_pe,"Expected EOLN, found EOF (line %d)", line());
            if(isprint(c)) quitf(translate_pe,"Expected EOLN, found '%c' (line %d)", c, line());
            quitf(translate_pe, "found ascii character %d (line %d)", c, line());
        }
        return c;
    }

    std::string readToken(bool ignore_whitespace = false){
        return p.readToken(ignore_whitespace);
    }
    /// citeste un ll
    int64_t readInt(int64_t Min, int64_t Max, bool ignore_whitespace = false){
        std::string word=p.readToken(ignore_whitespace);
        std::pair<parser::parse_error, int64_t> token=parser::parseInt(word);
        using pe=parser::parse_error;
        switch(token.first){
            case pe::OK: break;
            case pe::NUM_TOO_LARGE: quitf(translate_pe, "Integer \"%s\" cannot fit inside a 64-bit signed integer", word.c_str(), line()); break;
            case pe::UNEXPECTED_SPACE: quitf(translate_pe,"Expected integer, found space (line %d)", line()); break;
            case pe::UNEXPECTED_EOLN: quitf(translate_pe,"Expected integer, found EOLN (line %d)", line()); break;
            case pe::UNEXPECTED_EOF: quitf(translate_pe,"Expected integer, found EOF (line %d)", line()); break;
            case pe::UNEXPECTED_NONPRINT: quitf(translate_pe,"Expected integer, found ascii character %d (line %d)", word[0], line()); break;
            default: quitf(translate_pe,"Expected integer, found \"%s\" (line %d)", word.c_str(), line()); break;
        }
        int64_t result=token.second;
        if(result<Min || result>Max){
            quitf(translate_pe,"Number %lld is not inside the range [%lld, %lld] (line %d)", result, Min, Max, line());
        }
        return result;
    }
    // citeste un ull
    uint64_t readUnsigned(uint64_t Min, uint64_t Max, bool ignore_whitespace = false){
        std::string word=p.readToken(ignore_whitespace);
        std::pair<parser::parse_error, uint64_t> token=parser::parseUint(word);
        using pe=parser::parse_error;
        switch(token.first){
            case pe::OK: break;
            case pe::NUM_TOO_LARGE: quitf(translate_pe, "Integer \"%s\" cannot fit inside a 64-bit unsigned integer", word.c_str(), line()); break;
            case pe::UNEXPECTED_SPACE: quitf(translate_pe,"Expected unsigned integer, found space (line %d)", line()); break;
            case pe::UNEXPECTED_EOLN: quitf(translate_pe,"Expected unsigned integer, found EOLN (line %d)", line()); break;
            case pe::UNEXPECTED_EOF: quitf(translate_pe,"Expected unsigned integer, found EOF (line %d)", line()); break;
            case pe::UNEXPECTED_NONPRINT: quitf(translate_pe,"Expected unsigned integer, found ascii character %d (line %d)", word[0], line()); break;
            default: quitf(translate_pe,"Expected unsigned integer, found \"%s\" (line %d)", word.c_str(), line()); break;
        
        }
        uint64_t result=token.second;
        if(result<Min || result>Max){
            quitf(translate_pe,"Number %llu is not inside the range [%llu, %llu] (line %d)", result, Min, Max, line());
        }
        return result;
    }
    ~validator(){
        if(this->require_eof)
            quitf(translate_pe, "neither validator::quitf() nor validator::readEof() were called");
    }
};
class in_validator : public validator{
public:
    in_validator(){
        this->translate_pe=verdict_t::FAIL;
        this->translate_wa=verdict_t::FAIL;
        this->require_eof=false;
        this->eof=false;
    }
    in_validator(std::istream& in){
        p=parser(in);
        this->translate_pe=verdict_t::FAIL;
        this->translate_wa=verdict_t::FAIL;
        this->require_eof=false;
        this->eof=false;
    }
public:
    virtual void vpquitf(verdict_t verdict, float points, const char* fmt, va_list args){
        if(verdict!=verdict_t::OK)
            verdict=verdict_t::FAIL;
        fprintf(stdout, "%.2f", points);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void vquitf(verdict_t verdict, const char* fmt, va_list args){
        if(verdict!=verdict_t::OK)
            verdict=verdict_t::FAIL;
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void pquitf(verdict_t verdict, float points, const char* fmt, ...){
        va_list args; va_start(args, fmt);
        vpquitf(verdict, points, fmt, args);
        va_end(args);
        exit(-1);
    }
    virtual void quitf(verdict_t verdict, const char* fmt, ...){
        va_list args; va_start(args, fmt);
        vquitf(verdict, fmt, args);
        va_end(args);
        exit(-1);
    }
};
class out_validator : public validator{
public:
    out_validator(){
        this->eof=false;
        this->translate_pe=verdict_t::PE;
        this->translate_wa=verdict_t::WA;
        this->require_eof=false;
    }
    out_validator(std::istream& in){
        p=parser(in);
        this->eof=false;
        this->translate_pe=verdict_t::PE;
        this->translate_wa=verdict_t::WA;
        this->require_eof=false;
    }
    virtual void vpquitf(verdict_t verdict, float points, const char* fmt, va_list args){
        fprintf(stdout, "%.2f", points);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void vquitf(verdict_t verdict, const char* fmt, va_list args){
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void pquitf(verdict_t verdict, float points, const char* fmt, ...){
        va_list args; va_start(args, fmt);
        vpquitf(verdict, points, fmt, args);
        va_end(args);
        exit(-1);
    }
    virtual void quitf(verdict_t verdict, const char* fmt, ...){
        va_list args; va_start(args, fmt);
        vquitf(verdict, fmt, args);
        va_end(args);
        exit(-1);
    }
};
class ok_validator : public in_validator{
public:
    ok_validator(){
        this->eof=false;
        this->translate_pe=verdict_t::FAIL;
        this->translate_wa=verdict_t::FAIL;
        this->require_eof=false;
    }
    ok_validator(std::istream& in){
        p=parser(in);
        this->eof=false;
        this->translate_pe=verdict_t::FAIL;
        this->translate_wa=verdict_t::FAIL;
        this->require_eof=false;
    }
protected:
};
class checker{
public:
    in_validator in;
    out_validator out;
    ok_validator ok;
private:
    std::ifstream in_stream;
    std::ifstream out_stream;
    std::ifstream ok_stream;
public:
    checker(int argc, char** argv){
        in_stream.open(argv[CHECKER_IN_ARG]);
        out_stream.open(argv[CHECKER_OUT_ARG]);
        ok_stream.open(argv[CHECKER_OK_ARG]);
        in=in_validator(in_stream);
        ok=ok_validator(ok_stream);
        out=out_validator(out_stream);
    }
    virtual void vpquitf(verdict_t verdict, float points, const char* fmt, va_list args){
        fprintf(stdout, "%.2f", points);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void vquitf(verdict_t verdict, const char* fmt, va_list args){
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void pquitf(verdict_t verdict, float points, const char* fmt, ...){
        va_list args; va_start(args, fmt);
        vpquitf(verdict, points, fmt, args);
        va_end(args);
        exit(-1);
    }
    virtual void quitf(verdict_t verdict, const char* fmt, ...){
        va_list args; va_start(args, fmt);
        vquitf(verdict, fmt, args);
        va_end(args);
        exit(-1);
    } 
    ~checker(){
        quitf(verdict_t::FAIL, "checker: quitf has not been called");
    }
};

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