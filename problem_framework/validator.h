#ifndef VALIDATOR_H
#define VALIDATOR_H
enum validator_verdict{
    OK, FAIL
};

#include<stdio.h>
#include<stdarg.h>
#include<cstring>
#include<limits.h>
#include<stdint.h>
#include<stdlib.h>
#include<iostream>
class validator{
    int line_no=1;
    bool eof=0;
    int argc;
    char** argv;
    void vquitf(validator_verdict verdict, const char* fmt, va_list args){
        fprintf(stdout,"%d",verdict);
        fflush(stdout);
        vfprintf(stderr, fmt, args);
        fflush(stderr);
    }
    bool peek_eof(){
        char c=std::cin.peek();
        return c==(char)EOF;
    }
public:
    validator(int argc, char** argv){
        this->argc=argc;
        this->argv=argv;
        this->line_no=1;
        this->eof=false;
    }
    /// mesajul de validator trebuie scris in stderr
    /// verdictul final (un int) trebuie scris in stdout; 0 = OK, orice nenul = FAIL
    /// quitf() merge doar cu FAIL, quitf(OK) poate fi apelat doar din readEof();
    void quitf(validator_verdict verdict, const char* fmt, ...){
        if(verdict==validator_verdict::OK && !eof){
            quitf(validator_verdict::FAIL,"validator::readEof() was not called");
        }
        va_list args; va_start(args, fmt);
        vquitf(verdict, fmt, args);
        va_end(args);
        exit(0);
    } 
    /// returneaza FAIL daca conditia este adevarata cu mesajul adecvat
    void failIf(bool condition, const char* fmt, ...){
        if(condition){
            va_list args; va_start(args, fmt);
            vquitf(FAIL, fmt, args);
            va_end(args);
            exit(0);
        }
    }
    /// verifica daca a ajuns la finalul stdin
    void readEof(){
        if(peek_eof()){
            eof=1;
            quitf(validator_verdict::OK, "Ok");
        }
        char c=std::cin.get();
        if(c==' ') quitf(FAIL,"Expected EOF, found space (line %d)", line_no);
        if(c=='\n') quitf(FAIL,"Expected EOF, found EOLN (line %d)", line_no);
        if(isprint(c)) quitf(FAIL,"Expected EOF, found '%c' (line %d)", c, line_no);
        quitf(FAIL,"Expected EOF, found ascii character %d (line %d)", c, line_no);
    }
    /// citeste un caracter din charset
    char readChar(const char* charset){ /// daca charset = NULL, atunci se accepta orice caracter
        if(peek_eof()) quitf(validator_verdict::FAIL, "Expected character from charset \"%s\", found EOF (line %d)", charset, line_no);
        char c;
        std::cin.get(c);
        if(charset!=NULL && !strchr(charset, c)){
            if(c==' ') quitf(FAIL,"Expected character from charset \"%s\", found space (line %d)", line_no, charset);
            if(c=='\n') quitf(FAIL,"Expected character from charset \"%s\", found EOLN (line %d)", line_no, charset);
            if(c==EOF) quitf(FAIL,"Expected character from charset \"%s\", found EOF (line %d)", line_no, charset);
            if(isprint(c)) quitf(FAIL,"Expected character from charset \"%s\", found '%c' (line %d)", c, line_no, charset);
            quitf(FAIL,"Expected character from charset \"%s\", found ascii character %d (line %d)", c, line_no, charset);
        }
        if(c=='\n') line_no++;
        return c;
    }
    /// se citeste un spatiu
    char readSpace(){
        if(peek_eof()) quitf(validator_verdict::FAIL, "Expected space, found EOF (line %d)", line_no);
        char c;
        std::cin.get(c);
        if(c!=' '){
            if(c==' ') quitf(FAIL,"Expected space, found space (line %d)", line_no);
            if(c=='\n') quitf(FAIL,"Expected space, found EOLN (line %d)", line_no);
            if(c==EOF) quitf(FAIL,"Expected space, found EOF (line %d)", line_no);
            if(isprint(c)) quitf(FAIL,"Expected space, found '%c' (line %d)", c, line_no);
            quitf(FAIL,"Expected space, found ascii character %d (line %d)", c, line_no);
        }
        return c;
    }
    /// se citeste Eoln
    char readEoln(){
        if(peek_eof()) quitf(validator_verdict::FAIL, "Expected EOLN, found EOF (line %d)", line_no);
        char c;
        std::cin.get(c);
        if(c!='\n'){
            if(c==' ') quitf(FAIL,"Expected EOLN, found space (line %d)", line_no);
            if(c=='\n') quitf(FAIL,"Expected EOLN, found EOLN (line %d)", line_no);
            if(c==EOF) quitf(FAIL,"Expected EOLN, found EOF (line %d)", line_no);
            if(isprint(c)) quitf(FAIL,"Expected EOLN, found '%c' (line %d)", c, line_no);
            quitf(FAIL,"Expected EOLN, found ascii character %d (line %d)", c, line_no);
        }
        return c;
    }
    /// citeste un ll
    int64_t readInt(int64_t Min, int64_t Max){
        bool minus=0;
        bool has_digit=0;
        bool first=1;
        uint64_t result=0;
        std::string chars;
        while(1){
            char c=std::cin.peek();
            chars.push_back(c);
            if(first && c=='-'){
                first=0;
                minus=1;
                readChar("-");
                continue;
            }
            first=0;
            if(!isdigit(c)) break;
            has_digit=1;
            std::cin.get(c);
            uint64_t digit=c-'0';
            if(result>(ULLONG_MAX-digit)/10){
                quitf(FAIL,"Integer %s... cannot fit inside a 64-bit signed integer (line %d)", chars.c_str(), line_no);
            }
            result=result*10+digit;
        }
        if(minus && !has_digit){
            quitf(FAIL,"Expected integer, found \"-\" (line %d)", line_no);
        }
        if(!has_digit){
            char c=std::cin.peek();
            if(c==' ') quitf(FAIL,"Expected integer, found space (line %d)", line_no);
            if(c=='\n') quitf(FAIL,"Expected integer, found EOLN (line %d)", line_no);
            if(c==EOF) quitf(FAIL,"Expected integer, found EOF (line %d)", line_no);
            if(isprint(c)) quitf(FAIL,"Expected integer, found '%c' (line %d)", c, line_no);
            quitf(FAIL,"Expected integer, found ascii character %d (line %d)", c, line_no);
        }
        int64_t actual_result;
        if(minus){
            if(result>(uint64_t)LLONG_MAX+1)
                quitf(FAIL,"Number cannot fit inside a 64-bit signed integer (line %d)", line_no);
            else if(result==(uint64_t)LLONG_MAX+1)
                actual_result=LLONG_MIN;
            else 
                actual_result=-(int64_t)result;
        }
        else{
            if(result>(uint64_t)LLONG_MAX)
                quitf(FAIL,"Number cannot fit inside a 64-bit signed integer (line %d)", line_no);
            actual_result=result;
        }
        if(actual_result<Min || actual_result>Max){
            quitf(FAIL,"Number %lld is not inside the range [%lld, %lld] (line %d)", actual_result, Min, Max, line_no);
        }
        return actual_result;
    }
    // citeste un ull
    uint64_t readUnsigned(uint64_t Min, uint64_t Max){
        bool has_digit=0;
        uint64_t result=0;
        std::string chars;
        while(isdigit(std::cin.peek())){
            char c=std::cin.get();
            chars.push_back(c);
            has_digit=1;
            uint64_t digit=c-'0';
            if(result>(ULLONG_MAX-digit)/10){
                quitf(FAIL,"Unsigned integer %s... cannot fit inside a 64-bit signed integer (line %d)", chars.c_str(), line_no);
            }
            result=result*10+digit;
        }
        if(!has_digit){
            char c=std::cin.peek();
            if(c==' ') quitf(FAIL,"Expected unsigned integer, found space (line %d)", line_no);
            if(c=='\n') quitf(FAIL,"Expected unsigned integer, found EOLN (line %d)", line_no);
            if(c==EOF) quitf(FAIL,"Expected unsigned integer, found EOF (line %d)", line_no);
            if(isprint(c)) quitf(FAIL,"Expected unsigned integer, found '%c' (line %d)", c, line_no);
            quitf(FAIL,"Expected unsigned integer, found ascii character %d (line %d)", c, line_no);
        }
        if(result<Min || result>Max){
            quitf(FAIL,"Number %llu is not inside the range [%llu, %llu] (line %d)", result, Min, Max, line_no);
        }
        return result;
    }
    ~validator(){
        quitf(validator_verdict::FAIL, "neither validator::quitf() nor validator::readEof() were called");
    }
};
#endif