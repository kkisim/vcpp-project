# vcpp-project

## 🎮 지뢰찾기 프로젝트 (Minesweeper)

이 프로젝트는 **Windows API**를 활용하여 개발된 고전적인 **지뢰찾기 게임**입니다. 초급부터 고급까지 다양한 난이도를 지원하며, 사용자가 직접 설정 가능한 옵션도 제공합니다.

---

## 주요 기능

- **난이도 선택**:
  - 초급: 9x9 크기, 10개의 지뢰
  - 중급: 16x16 크기, 40개의 지뢰
  - 고급: 30x16 크기, 99개의 지뢰
  - 사용자 지정: 보드 크기와 지뢰 개수를 직접 설정 가능
- **셀 상태 관리**:
  - 깃발(⚑), 물음표(?), 빈 셀 등 다양한 상태를 표시
- **승리와 패배 처리**:
  - 승리: 모든 비지뢰 셀이 열리고 지뢰에 깃발이 꽂힌 경우
  - 패배: 지뢰를 클릭할 경우 즉시 게임 종료
- **다양한 UI 요소**:
  - 남은 지뢰 수와 타이머를 화면 상단에 표시

---

## 프로젝트 구성

### 주요 파일 설명

- **`Ms_Logic.h` 및 `Ms_Logic.cpp`**:
  - 지뢰찾기 게임의 핵심 로직을 처리
  - 지뢰 배치, 셀 열기, 승리 조건 체크 등 관리
- **`Ms_Interface.h` 및 `Ms_Interface.cpp`**:
  - 화면 렌더링 및 UI 관련 코드
  - 셀, 깃발, 물음표 등의 그래픽 처리
- **`MS.cpp`**:
  - 애플리케이션의 진입점
  - 이벤트 처리 및 난이도 전환 관리
- **리소스 파일**:
  - 게임에 사용되는 비트맵 파일: `cell.bmp`, `flag.bmp`, `bomb.bmp`, `qmark.bmp` 등

### 데이터 구조

- **셀(Cell) 구조체**:
  ```cpp
  struct Cell {
      bool isMine;            // 지뢰 여부
      bool isRevealed;        // 셀이 열렸는지 여부
      bool isFlagged;         // 깃발 표시 여부
      bool isQMark = false;   // 물음표 상태 여부
      int surroundingMines;   // 주변 지뢰 개수
      bool isClicked;         // 클릭 상태 여부
  };
