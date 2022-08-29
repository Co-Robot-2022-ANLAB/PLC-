#ifndef __RPI_H     // 헤더 파일의 중복 include를 막기 위한 헤더가드.
#define __RPI_H

#include <iostream>     // 콘솔을 통한 상태 출력을 위한 헤더 파일.
#include <wiringPi.h>   // wiringPi 라이브러리 이용을 위한 헤더 파일.

#define LOG_            // 디버깅용 로그 출력용 define 상수, 이 줄을 제거하거나 주석 처리하면 [Init] 등의 로그가 출력되지 않음.

// 변수 등의 이름 충돌을 막기 위한 rpi 네임스페이스.
/*
 * 본 헤더 파일의 함수를 사용하기 위해서 rpi::{함수명}의 형태로 작성해야 함.
 * e.g.
 *      if (rpi::get_from_human) {
 *          std::cout << "사람이 감지되었습니다.\n";
 *      }
 */
namespace rpi {
    /*
     * 아래의 세 변수는 각각, 사람 영역에 사용할 포토 센서 입력 포트, 로봇 영역에 사용할 포토 센서 입력 포트, 각 영역에서의 포토 센서 개수.
     *
     * # 참고: 변수로 선언된 핀은 wPi기준 핀으로 BCM 기준 핀은 아래와 같음.
     *        Human Pins: 3, 5, 7
     *        Robot Pins: 8, 10, 11
     */
    int human_pins[3] = { 9, 21, 11 };
    int robot_pins[3] = { 10, 12, 14};
    size_t num_pins = sizeof(human_pins) / sizeof(human_pins[0]);

    // 라이브러리 환경 초기화용 함수.
    void init() {
#ifdef LOG_
        std::cout << "[Init] Init.\n";
#endif

        // 라이브러리 로딩이 비정상적으로 종료되면 프로그램 종료.
        if (wiringPiSetup() == -1)
            exit(1);

        // 사람 영역과 로봇 영역에서 입력을 받을 핀을 INPUT으로 설정.
        for (size_t i = 0; i < num_pins; i++) 
        {
            pinMode(human_pins[i], INPUT);
            pinMode(robot_pins[i], INPUT);
        }
#ifdef LOG_
        std::cout << "[Init] Done.\n";
#endif
    }

    // 매개변수 pins로부터 입력 받기.
    bool get_from_pins(int* pins, size_t count) {
#ifdef LOG_
        std::cout << "[Input] Get from pins...\n";
#endif

        // 최초 입력을 true로 설정한 후 and 연산을 통해 포토 센서 중 HIGH인 센서가 있는지 검사.
        // 왜냐하면, 포토 센서가 LOW일 때 true, HIGH일 때 false를 반환하기 때문, 즉 최종 결과가 false이면 센서 중 HIGH인 센서가 존재.
        bool res = true;
        for (size_t i = 0; i < count; i++)
            res &= digitalRead(pins[i]);

#ifdef LOG_
        std::cout << "[Input] Done.\n";
#endif

        // 상기한 이유로 마지막 결과를 반전하여 해당 영역에 물채가 감지되면 true, 감지되지 않으면 false를 반환.
        return !res;
    }

    // 사람 영역과 로봇 영역 모두를 검사하여 안전 여부 반환.
    bool is_not_safe() {
        return get_from_pins(human_pins, num_pins) && get_from_pins(robot_pins, num_pins);
    }

    // 사람 영역 검사.
    bool get_from_human() {
        return get_from_pins(human_pins, num_pins);
    }

    // 로봇 영역 검사.
    bool get_from_robot() {
        return get_from_pins(robot_pins, num_pins);
    }
};

#endif