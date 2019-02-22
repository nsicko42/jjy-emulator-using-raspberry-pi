# jjy-simulator-using-raspberry-pi

## 1. 개요
Raspberry Pi를 이용한 JJY simulator
### 1.1. 라즈베리 파이에서 사용하는 핀
기본적으로 BCM 18번(WiringPi 1번)을 사용하여 하드웨어 PWM을 통해 40kHz의 캐리어 주파수를 만들어냅니다.

## 2. 빌드
<pre># gcc -o jjy jjy.c -lpigpio -lrt</pre>

## 3. 사용법
<pre># ./jjy [-h46q]</pre>

### 3.1. 옵션
h 도움말
4 반송 주파수 40kHz
6 반송 주파수 60kHz
q 화면에 현재 시간 출력 

## 참고자료
http://jjy.nict.go.jp/jjy/index-e.html
