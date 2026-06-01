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

enum parser_behavior_t{
    DEFAULT,
    STRICT,
    IGNORE_WHITESPACE
};
template<unsigned buffer_size = 4096>
class parser{
protected:
    unsigned line_no=1;
    FILE* in;
    char buff[buffer_size+1];
    unsigned bpos;
    parser_behavior_t def_behaviour;
    inline void refresh_buffer(){
        if(bpos>=buffer_size){
            size_t chars_read = fread(buff, 1, buffer_size, in);
            if(chars_read < buffer_size) buff[chars_read] = EOF;
            bpos = 0;
        }
    }
public:
    class uint_too_large_exception : public std::exception{
        char msg[128]{};
    public:
        uint_too_large_exception(uint64_t num, char nxt){
            snprintf(msg, 128, "Number \"%lu%c...\" cannot fit inside a 64-bit unsigned integer", num, nxt);
        }
        const char* what() const noexcept override{
            return msg;
        }
    };
    class int_too_large_exception : public std::exception{
        char msg[128]{};
    public:
        int_too_large_exception(uint64_t num, bool sign){
            if(!sign)
                snprintf(msg, 128, "Number \"%lu\" cannot fit inside a 64-bit signed integer", num);
            else
                snprintf(msg, 128, "Number \"-%lu\" cannot fit inside a 64-bit signed integer", num);
        }
        const char* what() const noexcept override{
            return msg;
        }
    };
    parser(parser_behavior_t default_behaviour = parser_behavior_t::STRICT){
        static_assert(buffer_size>0, "buffer size must be positive");
        def_behaviour = default_behaviour;
        in=stdin;
        bzero(buff, sizeof buff);
        bpos = buffer_size;
    }
    parser(const char* path, parser_behavior_t default_behaviour = parser_behavior_t::STRICT){
        static_assert(buffer_size>0, "buffer size must be positive");
        def_behaviour = default_behaviour;
        in=fopen(path,"r");
        if(in == NULL)
            throw std::ios_base::failure("[parser] Could not open file");
        bzero(buff, sizeof buff);
        bpos = buffer_size;
    }
    inline void setDefaultBehaviour(parser_behavior_t default_behaviour){
        def_behaviour = default_behaviour;
    }
    inline parser_behavior_t getDefaultBehaviour(){
        return def_behaviour;
    }
    inline bool eof(){
        refresh_buffer();
        return buff[bpos]==EOF;
    }
    inline int line(){
        return line_no;
    }
    inline char peek(){
        refresh_buffer();
        return buff[bpos];
    }
    inline void ignore_whitespace(){
         while(isspace(peek())) get();
    }
    char get(){
        refresh_buffer();
        if(eof()) return EOF;
        char c=buff[bpos++];
        if(c=='\n') line_no++;
        return c;
    }
    std::string readToken(parser_behavior_t behavior = parser_behavior_t::DEFAULT){
        if(behavior==parser_behavior_t::DEFAULT)
            behavior=def_behaviour;
        if(behavior==parser_behavior_t::IGNORE_WHITESPACE)
            ignore_whitespace();
        
        std::string s;
        while(true){
            char c=peek();
            if(c==(char)EOF || isspace(c)){
                if(s.empty()){
                    throw c; // Expected string, found "c"
                    return s;
                }
                return s;
            }
            s.push_back(c);
            get();
        }
        return s;
    }
    uint64_t readUnsigned(parser_behavior_t behavior = parser_behavior_t::DEFAULT){
        if(behavior==parser_behavior_t::DEFAULT)
            behavior=def_behaviour;
        if(behavior==parser_behavior_t::IGNORE_WHITESPACE)
            ignore_whitespace();
        uint64_t result=0;  
        bool first=1;
        while(true){
            char c=peek();
            if(!isdigit(c)){
                if(first || !isspace(c)){
                    throw c;
                }
                return result;
            }
            first=0;
            uint64_t digit=c-'0';
            if(result>(ULLONG_MAX-digit)/10){
                throw uint_too_large_exception(result, c);
                return result;
            }
            result=(result<<1)+(result<<3)+digit;
            get();
        }
        return result;
    }
    int64_t readInt(parser_behavior_t behavior = parser_behavior_t::DEFAULT){
        if(behavior==parser_behavior_t::DEFAULT)
            behavior=def_behaviour;
        if(behavior==parser_behavior_t::IGNORE_WHITESPACE)
            ignore_whitespace();
        bool minus=0;
        if(peek()=='-') minus=1, get();
        uint64_t result;
        try{
            result=readUnsigned(parser_behavior_t::STRICT);
        }catch(uint_too_large_exception e){
            throw e;
        }catch(char c){
            throw c;
        }
        int64_t actual_result=0;
        if(minus){
            if(result>(uint64_t)LLONG_MAX+1)
                throw int_too_large_exception(result, minus);
            else if(result==(uint64_t)LLONG_MAX+1)
                actual_result=LLONG_MIN;
            else 
                actual_result=-(int64_t)result;
        }
        else{
            if(result>(uint64_t)LLONG_MAX)
                throw int_too_large_exception(result, minus);
            else
                actual_result=result;
        }
        return actual_result;
    }
};
enum verdict_t{
    OK, WA, PE, SUPER, PA, FAIL
};
class validator{
protected:
    bool eof;
    parser<4096> p;
    inline bool peek_eof(){
        return p.peek()==(char)EOF;
    }
    inline char peek(){
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
        p=parser(parser_behavior_t::STRICT);
    }
    validator(const char* path){
        this->eof=false;
        this->require_eof=false;
        this->translate_pe=verdict_t::FAIL;
        this->translate_wa=verdict_t::FAIL;
        p=parser(path, parser_behavior_t::STRICT);
    }
    virtual void vpquitf(verdict_t verdict, float points, const char* fmt, va_list args){
        if(verdict!=verdict_t::OK) /// validators must return OK or FAIL
            verdict=verdict_t::FAIL;
        fprintf(stdout, "%.2f", points);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void vquitf(verdict_t verdict, const char* fmt, va_list args){
        if(verdict!=verdict_t::OK) /// validators must return OK or FAIL
            verdict=verdict_t::FAIL;
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void pquitf(verdict_t verdict, float points, const char* fmt, ...){
        if(require_eof && verdict==verdict_t::OK && !eof){
            pquitf(verdict_t::FAIL, 0.0, "validator::readEof() was not called");
        }
        va_list args; va_start(args, fmt);
        vpquitf(verdict, points, fmt, args);
        va_end(args);
        exit(-1);
    }
protected:
    void __fail_expected_char_but_found(const char* expected, char found){
        if(found==(char)EOF) quitf(translate_pe,"Expected %s, found EOF (line %d)", expected, line());
        if(found=='\n') quitf(translate_pe,"Expected %s, found EOLN (line %d)", expected, line());
        if(found==' ') quitf(translate_pe,"Expected %s, found space (line %d)", expected, line());
        if(isprint(found)) quitf(translate_pe,"Expected %s, found character \"%c\" (line %d)", expected, found, line()); 
        quitf(translate_pe,"Expected %s, found non-printable character \"%d\" (line %d)", expected, found, line());
    }
    void __fail_expected_token_but_found(const char* expected, char found){
       if(found==(char)EOF) quitf(translate_pe,"Expected %s, found EOF (line %d)", expected, line());
        if(found=='\n') quitf(translate_pe,"Expected %s, found EOLN (line %d)", expected, line());
        if(found==' ') quitf(translate_pe,"Expected %s, found space (line %d)", expected, line());
        if(isprint(found)) quitf(translate_pe,"Expected %s, found token containing character \"%c\" (line %d)", expected, found, line()); 
        quitf(translate_pe,"Expected %s, found token containing non-printable character \"%d\" (line %d)", expected, found, line());
    }
public:
    virtual void quitf(verdict_t verdict, const char* fmt, ...){
        if(require_eof && verdict==verdict_t::OK && !eof){
            quitf(verdict_t::FAIL,"validator::readEof() was not called");
        }
        va_list args; va_start(args, fmt);
        vquitf(verdict, fmt, args);
        va_end(args);
        exit(-1);
    } 
    /// returns FAIL if condition is true
    virtual void failIf(bool condition, const char* fmt, ...){
        if(condition){
            va_list args; va_start(args, fmt);
            vquitf(translate_wa, fmt, args);
            va_end(args);
            exit(-1);
        }
    }
    inline int line(){
        return p.line();
    }
    void ignoreWhitespace(){
        p.ignore_whitespace();
    }
    void readEof(){
        if(peek_eof()){
            eof=1;
            if(require_eof)
                quitf(verdict_t::OK, "Ok");
            else return;
        }
        __fail_expected_char_but_found("EOF", p.get());
    }
    char readChar(const char* charset){ /// if charset = NULL, then all characters are accepted
        char c=p.get();
        if(charset!=NULL && !strchr(charset, c))
            __fail_expected_char_but_found(((std::string)"character from charset \""+charset+"\"").c_str(), c);
        if(charset==NULL && c==EOF) __fail_expected_char_but_found("character", c);
        return c;
    }

    char readSpace(){
        char c=p.get();
        if(c!=' ')
            __fail_expected_char_but_found("space", c);
        return c;
    }

    char readEoln(){
        char c=p.get();
        if(c!='\n')
            __fail_expected_char_but_found("EOLN", c);
        return c;
    }

    std::string readToken(parser_behavior_t behavior = parser_behavior_t::DEFAULT){
        return p.readToken(behavior);
    }

    int64_t readInt(int64_t Min, int64_t Max, parser_behavior_t behavior = parser_behavior_t::DEFAULT){
        try{
            int64_t result = p.readInt(behavior);
            if(result<Min || result>Max){
                quitf(translate_pe,"Number %lld is not inside the range [%lld, %lld] (line %d)", result, Min, Max, line());
            }
            return result;
        }catch(char c){
            __fail_expected_token_but_found("integer", c);
        }catch(parser<>::uint_too_large_exception e){
            quitf(translate_pe, e.what());
        }
        return -1;
    }
    int64_t readInt(parser_behavior_t behavior = parser_behavior_t::DEFAULT){
        return readInt(INT64_MIN, INT64_MAX, behavior);
    }
    uint64_t readUnsigned(uint64_t Min, uint64_t Max, parser_behavior_t behavior = parser_behavior_t::DEFAULT){
        try{
            uint64_t result = p.readUnsigned(behavior);
            if(result<Min || result>Max){
                quitf(translate_pe,"Number %llu is not inside the range [%llu, %llu] (line %d)", result, Min, Max, line());
            }
            return result;
        }catch(char c){
            __fail_expected_token_but_found("unsigned integer", c);
        }catch(parser<>::uint_too_large_exception e){
            quitf(translate_pe, e.what());
        }
        return -1;
    }
    uint64_t readUnsigned(parser_behavior_t behavior = parser_behavior_t::DEFAULT){
        return readUnsigned(0, UINT64_MAX, behavior);
    }
    ~validator(){
        if(this->require_eof)
            quitf(translate_pe, "neither validator::quitf() nor validator::readEof() were called");
    }
};
class in_validator : public validator{
public:
    in_validator(){
        this->eof=false;
        this->require_eof=false;
        this->translate_pe=verdict_t::FAIL;
        this->translate_wa=verdict_t::FAIL;
        p=parser(parser_behavior_t::IGNORE_WHITESPACE);
    }
    in_validator(const char* path){
        this->eof=false;
        this->require_eof=false;
        this->translate_pe=verdict_t::FAIL;
        this->translate_wa=verdict_t::FAIL;
        p=parser(path, parser_behavior_t::IGNORE_WHITESPACE);
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
        if(verdict==verdict_t::OK)
            fprintf(stdout,"1.0");
        else 
            fprintf(stdout,"0.0");
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
        this->require_eof=false;
        this->translate_pe=verdict_t::PE;
        this->translate_wa=verdict_t::WA;
        p=parser(parser_behavior_t::IGNORE_WHITESPACE);
    }
    out_validator(const char* path){
        this->eof=false;
        this->require_eof=false;
        this->translate_pe=verdict_t::PE;
        this->translate_wa=verdict_t::WA;
        p=parser(path, parser_behavior_t::IGNORE_WHITESPACE);
    }
    virtual void vpquitf(verdict_t verdict, float points, const char* fmt, va_list args){
        fprintf(stdout, "%.2f", points);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void vquitf(verdict_t verdict, const char* fmt, va_list args){
        vfprintf(stderr, fmt, args);
        if(verdict==verdict_t::OK || verdict == verdict_t::SUPER)
            fprintf(stdout,"1.0");
        else 
            fprintf(stdout,"0.0");
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
typedef in_validator ok_validator;
class checker{
public:
    in_validator in;
    out_validator out;
    ok_validator ok;
public:
    checker(int argc, char** argv){
        in=in_validator(argv[CHECKER_IN_ARG]);
        out=out_validator(argv[CHECKER_OUT_ARG]);
        ok=ok_validator(argv[CHECKER_OK_ARG]);
    }
    virtual void vpquitf(verdict_t verdict, float points, const char* fmt, va_list args){
        fprintf(stdout, "%.2f", points);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
        exit(verdict);
    }
    virtual void vquitf(verdict_t verdict, const char* fmt, va_list args){
        vfprintf(stderr, fmt, args);
        if(verdict==verdict_t::OK || verdict == verdict_t::SUPER)
            fprintf(stdout,"1.0");
        else 
            fprintf(stdout,"0.0");
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
    public:
        const char* what() const noexcept override{
            return "[generator::nextString] len must be greater than or equal to 0";
        }
    };
    class invalid_interval_exception : public std::exception{
    public:
        const char* what() const noexcept override{
            return "[generator::nextInt] l must be less than or equal to r";
        }
    };
    class invalid_double_interval_exception : public std::exception{
    public:
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