#include<iostream>
#include<cctype>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum class TokenKind{
  TK_RESERVED, //記号
  TK_NUM,      //整数トークン
  TK_EOF,      //入力の終わりを示すトークン
};

struct Token{
  TokenKind kind;//トークンの型
  Token *next;   //次の入力トークン
  int val;       //kindがTK_NUMの場合の数値
  char *str;     //トークン文字列
};



//現在着目しているトークン
Token *token;

//エラーを報告するための関数
template<typename... Args>
void error(const char* fmt, Args... args) {
  std::fprintf(stderr, fmt, args...);
  std::fprintf(stderr, "\n");
  std::exit(1);
}


//次のトークンが期待している記号の時は一つ読み進めて
//真を返すそれ以外の場合は偽を返す
bool consume(char op){
  if(token->kind != TokenKind::TK_RESERVED || token->str[0] !=op){
    return false;
  }
  token = token->next;
  return true;
}


//次のトークンが期待している記号の時はトークンを一つ進める
//それ以外の時はエラーを報告
void expect(char op){
  if (token->kind != TokenKind::TK_RESERVED || token->str[0] != op){
    error("'%c'ではありません",op);
  }
  token = token->next;
}


//次のトークンが数値の場合は、トークンを一つ読み進めてその数値を返す
// それ以外の場合はエラーを報告する
int expect_number(){
  if(token->kind != TokenKind::TK_NUM){
    error("数ではありません");
  }
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof(){
  return token->kind == TokenKind::TK_EOF;
}

Token *new_token(TokenKind kind,Token *cur,char *str){
  Token *tok = (Token*)calloc(1,sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

//入力文字列pをトークナイズしてそれを返す関数
Token *tokenize(char *p){
  Token head;
  head.next = nullptr;
  Token *cur = &head;

  while(*p){
    if (isspace(*p)){
      p++;
      continue;
    }

    if(*p == '+' || *p == '-'){
      cur = new_token(TokenKind::TK_RESERVED,cur,p++);
      continue;
    }
    if(isdigit(*p)){
      cur = new_token(TokenKind::TK_NUM,cur,p);
      cur->val = strtol(p,&p,10);
      continue;
    }
    error("トークナイズできません");
  }
  new_token(TokenKind::TK_EOF,cur,p);
  return head.next;
}
int main(int argc, char *argv[]){
  if(argc!=2){
    std::cerr<<"引数の個数が正しくありません"<<std::endl;
    return 1;
  }

  token = tokenize(argv[1]);

  std::cout<<".intel_syntax noprefix"<<std::endl;
  std::cout<<".globl main"<<std::endl;
  std::cout<<"main:"<<std::endl;

  //式の最初は数である必要があるのでそれをチェックして出力

  std::cout<<"  mov rax, "<<expect_number()<<std::endl;

  while(!at_eof()){
    if(consume('+')){
      std::cout<<"  add rax, "<<expect_number()<<std::endl;
      continue;
    }

    expect('-');
    std::cout<<"  sub rax, "<<expect_number()<<std::endl;
  }
  std::cout<<"  ret"<<std::endl;
  return 0;
}
