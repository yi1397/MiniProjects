## 명령어 정의 방법
#### 명령어가 저장된 자료구조
std::vector<std::pair<std::pair<unsigned int, unsigned int>, const char*>> commands
vector에 명령어들이 정의되어 있고 std::pair<std::pair<unsigned int, unsigned int>, const char*>에는 명령어의 순서와 길이, 명령어 이름이 저장된다.
###### 명령어의 해석방법
명령어가 저장된 unsigned int자료형의 최하위 비트부터 왼쪽순서로 해석한다. 0은 눈깜빡이기 1은 고개흔들기이다. 명령어의 길이 만큼만 해석한다.
###### 예시
commands.push_back(std::make_pair(std::make_pair(6, 3), "문이 열립니다"));

6                   → 0b110 눈 감빡이고 고개 2번 흔들기

3                   → 명령어의 정보가 몇비트인지

"문이 열립니다"     → 명령어의 이름

## 전처리기

DRAW_LANDMARKS  얼굴에 윤곽선을 표시해 줄것인지

PORT_NUM        시리얼 포트 정의

EYE_CLOSED      눈 깜빡이는 각도 -> 눈 깜빡이는것을 인식하는 것과 관련(건들 필요 거의 없음)

COMMAND_TIME    명령어가 완전히 입력되지 않았을때 입력중인 명령어를 초기화하는 시간
