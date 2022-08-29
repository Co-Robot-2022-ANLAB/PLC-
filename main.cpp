/*
 * 참고, 본 cpp 파일을 컴파일하기 위해선 본 소스 파일과 rpi.h 헤더 파일이 동일한 폴더 내에 존재해야 함.
 *
 * 아래의 명령줄을 이용해 컴파일할 수 있음.
 *      g++ main.cpp -lwiringPi
 * 
 * 위의 명령을 실행하면 폴더 내에 a.out 파일이 생성됨.
 * a.out 파일이 생성되면 아래의 명령줄을 이용해 실행 수 있음.
 *      sudo ./a.out
 * 
 * wiringPi 라이브러리가 시스템으로부터 GPIO 통신을 수행하므로 관리자 권한이 필요함. (sudo 실행)
 */

#include <iostream>     // 콘솔 입출력을 위한 헤더 파일.
#include <unistd.h>     // sleep 함수를 위한 헤더 파일.

#include "rpi.h"        // wiringPi 라이브러리를 이용하기 위해 작성한 헤더 파일.

int main(void) {
    rpi::init();        // wiringPi 관련 초기 환경 설정 함수.

    // 매 1초마다 사람, 로봇 영역 검사 및 안전 여부 출력.
    while (true) {
        std::cout << "Human Size: " << rpi::get_from_human() << "\n";       // 사람 영역 물체 감지 출력.
        std::cout << "Robot Size: " << rpi::get_from_robot() << "\n";       // 로봇 영역 물체 감지 출력.
        std::cout << "Not Safe: " << rpi::is_not_safe << "\n";              // 안전 여부 출력.

        sleep(1);       // 1초 대기.
    }

    return 0;
}