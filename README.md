jjy-simulator-using-raspberry-pi
==============

## 1. Abstract
JJY simulator using Raspberry Pi
### 1.1. Used pin
It makes carrier frequency (40kHz, optional 60kHz) using Hardware PWM of GPIO18(Header pin no.12, WiringPi 1).

## 2. How to build
<pre># gcc -o jjy jjy.c -lpigpio -lrt</pre>

## 3. Usage
<pre># ./jjy [-h46q]</pre>

### 3.1. Option
<pre>-h Help
-4 Carrier Frequency 40kHz (기본값)
-6 Carrier Frequency 60kHz
-q Quiet mode</pre>

### Reference
http://jjy.nict.go.jp/jjy/index-e.html



***
라즈베리파이용 JJY 시뮬레이터
==============


## 1. 개요
Raspberry Pi를 이용한 JJY simulator
### 1.1. 라즈베리 파이에서 사용하는 핀
기본적으로 GPIO18(헤더핀 번호 12, WiringPi 1번핀)을 사용하여 하드웨어 PWM을 통해 40kHz, 60kHz의 캐리어 주파수를 만들어냅니다.

## 2. 빌드
<pre># gcc -o jjy jjy.c -lpigpio -lrt</pre>

## 3. 사용법
<pre># ./jjy [-h46q]</pre>

### 3.1. 옵션
<pre>-h 도움말
-4 반송 주파수 40kHz (기본값)
-6 반송 주파수 60kHz
-q 타임 코드를 출력하지 않음</pre>

## 참고자료
http://jjy.nict.go.jp/jjy/index-e.html
