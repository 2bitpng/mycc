#!/bin/bash
assert(){
  expected="$1"
  input="$2"
  ./mycc "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"
  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected ,but got $acutal"
    exit 1
  fi
}

assert 0 0
assert 42 42
assert 255 255
assert 21 '5+20-4' 
assert 0 '0+0-0-0' 
assert 41 " 12 + 34 - 5 "
assert 124 "5+100 - 1 + 20"
echo OK
