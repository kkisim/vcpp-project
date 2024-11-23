﻿// MS.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MS.h"



#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



const Ms_level* currentLevel = &Easy;           // 현재 난이도 (기본: Easy)
static int timerCount = 0;                      // 타이머 값
Ms_Logic logic(Easy.getWidth(), Easy.getHeight(), Easy.getMines()); // 초기 로직 객체


// 창 크기 계산 및 UI 재배치
void ResizeWindow(HWND hWnd, const Ms_level& level);
void AdjustUIPositions(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MS));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
// 창 크기 조정 함수
void ResizeWindow(HWND hWnd, const Ms_level& level)
{
    int boardWidth = level.getWidth() * CELL_WIDTH;
    int boardHeight = level.getHeight() * CELL_HEIGHT;
    int clientWidth = boardWidth;
    int clientHeight = boardHeight + 100; // UI 포함 높이

    RECT rect = { 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

    SetWindowPos(hWnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
    AdjustUIPositions(hWnd);
}

// UI 위치 재배치
void AdjustUIPositions(HWND hWnd)
{
    RECT rect;
    GetClientRect(hWnd, &rect);

    int timerWidth = 40;
    int timerHeight = 30;
    int width = rect.right;

    HWND hResetButton = GetDlgItem(hWnd, ID_BUTTON_RESET);
    HWND hTimer = GetDlgItem(hWnd, ID_TIMER);

    if (hResetButton) {
        MoveWindow(hResetButton, 10, 10, 80, 30, TRUE);
    }

    if (hTimer) {
        int timerX = (width -50) - (timerWidth -50);
        MoveWindow(hTimer, timerX, 10, timerWidth, timerHeight, TRUE);
    }
}

/**
 * @brief 지뢰 이미지를 지정 위치에 렌더링합니다.
 * @param hdc 현재 HDC
 * @param hBitmap 지뢰 비트맵 핸들
 * @param x 셀의 X 좌표 (픽셀 단위)
 * @param y 셀의 Y 좌표 (픽셀 단위)
 */



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    // 창 크기 설정
    ResizeWindow(hWnd, *currentLevel);

    // 초기 지뢰 배치 및 숫자 계산
    logic.InitializeBoard();
    logic.PlaceMines(-1, -1, 0); // 랜덤 위치에 지뢰 배치
    logic.CalculateSurroundingMines();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
//
//   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//       CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);
//
//
//   if (!hWnd)
//   {
//      return FALSE;
//   }
//   // 비트맵 로드
//   hMineBitmap = (HBITMAP)LoadImage(hInstance, L"resources/mine.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//   hFlagBitmap = (HBITMAP)LoadImage(hInstance, L"resources/flag.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//
//   // 창 크기 조정 (난이도에 따라)
//   ResizeWindow(hWnd, *currentLevel);
//
//   ShowWindow(hWnd, nCmdShow);
//   UpdateWindow(hWnd);
//
//   return TRUE;
//}
// 창 크기 조정 함수
//void ResizeWindow(HWND hWnd, const Ms_level& level)
//{
//    // 보드 크기 계산
//    int boardWidth = level.getWidth() * CELL_WIDTH;
//    int boardHeight = level.getHeight() * CELL_HEIGHT;
//
//    // 창 크기 조정
//    RECT rect = { 0, 0, boardWidth, boardHeight };
//    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
//
//    SetWindowPos(hWnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
//}
//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        LoadResources(hInst);  // 리소스 로드
        InitializeInterface(hWnd);
        break;

    case WM_SIZE:
        AdjustUIPositions(hWnd); // 창 크기 변경 시 UI 재배치
        break;


    case WM_LBUTTONDOWN: {
        int x = LOWORD(lParam) / CELL_WIDTH;
        int y = (HIWORD(lParam) - 100) / CELL_HEIGHT;

        // 좌표 범위 확인
        if (y >= 0 && x >= 0 && x < logic.GetBoard()[0].size() && y < logic.GetBoard().size()) {
            const Cell& clickedCell = logic.GetBoard()[y][x];

            // 숫자나 깃발이 있는 셀은 클릭 불가
            if (clickedCell.isFlagged || (clickedCell.isRevealed && clickedCell.surroundingMines > 0)) {
                break;
            }

            logic.SetCellClicked(x, y, true); // 셀을 눌린 상태로 설정

            // 클릭된 셀만 갱신
            RECT cellRect = {
                x * CELL_WIDTH,
                y * CELL_HEIGHT + 100, // 보드 시작 Y 좌표 보정
                (x + 1) * CELL_WIDTH,
                (y + 1) * CELL_HEIGHT + 100
            };
            InvalidateRect(hWnd, &cellRect, TRUE); // 클릭 효과 즉시 반영
        }
        break;
    }


    case WM_LBUTTONUP: {
        int x = LOWORD(lParam) / CELL_WIDTH; // 클릭한 셀의 X 좌표
        int y = (HIWORD(lParam) - 100) / CELL_HEIGHT; // 클릭한 셀의 Y 좌표 (UI 영역 보정)

        // 보드 범위 내인지 확인
        if (y >= 0 && x >= 0 && x < logic.GetBoard()[0].size() && y < logic.GetBoard().size()) {
            logic.SetCellClicked(x, y, false); // 클릭 상태 해제

            // 셀 열기 및 드러난 셀들 갱신
            auto revealedCells = logic.RevealCell(x, y); // 탐색된 셀 좌표 반환
            for (const auto& cell : revealedCells) {
                RECT cellRect = {
                    cell.first * CELL_WIDTH,
                    cell.second * CELL_HEIGHT + 100, // UI 영역 보정
                    (cell.first + 1) * CELL_WIDTH,
                    (cell.second + 1) * CELL_HEIGHT + 100
                };
                InvalidateRect(hWnd, &cellRect, TRUE); // 탐색된 각 셀 갱신
            }

            // 게임 오버 확인
            if (logic.IsGameOver()) {
                // 모든 지뢰 드러내기
                for (int row = 0; row < logic.GetBoard().size(); ++row) {
                    for (int col = 0; col < logic.GetBoard()[0].size(); ++col) {
                        if (logic.GetBoard()[row][col].isMine) {
                            RECT cellRect = {
                                col * CELL_WIDTH,
                                row * CELL_HEIGHT + 100,
                                (col + 1) * CELL_WIDTH,
                                (row + 1) * CELL_HEIGHT + 100
                            };
                            InvalidateRect(hWnd, &cellRect, TRUE); // 지뢰를 화면에 갱신
                        }
                    }
                }

                // 게임 오버 메시지 표시
                if (MessageBox(hWnd, L"Game Over!", L"Game Over", MB_ICONERROR | MB_OK) == IDOK) {
                    SendMessage(hWnd, WM_COMMAND, ID_BUTTON_RESET, 0); // 리셋 버튼 기능 실행
                }
            }
            // 게임 승리 확인
            else if (logic.IsGameWon()) {
                // 게임 승리 메시지 표시
                if (MessageBox(hWnd, L"Congratulations!", L"Victory", MB_ICONINFORMATION | MB_OK) == IDOK) {
                    SendMessage(hWnd, WM_COMMAND, ID_BUTTON_RESET, 0); // 리셋 버튼 기능 실행
                }
            }
        }
        break;
    }





    case WM_RBUTTONDOWN: { // 오른쪽 클릭 이벤트
        int x = LOWORD(lParam) / CELL_WIDTH;
        int y = (HIWORD(lParam) - 100) / CELL_HEIGHT;

        if (y >= 0 && x >= 0 && x < logic.GetBoard()[0].size() && y < logic.GetBoard().size()) {
            logic.ToggleFlag(x, y);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_32771: // Easy 난이도
            currentLevel = &Easy;
            timerCount = 0;
            logic = Ms_Logic(Easy.getWidth(), Easy.getHeight(), Easy.getMines(), false); // DFS 사용
            
            logic.InitializeBoard(); // 보드 초기화
            logic.PlaceMines(-1, -1, 0); // 지뢰 배치 (처음 클릭 위치가 없으므로 -1, -1 전달)
            logic.CalculateSurroundingMines(); // 주변 지뢰 수 계산
            ResizeWindow(hWnd, *currentLevel);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case ID_32772: // Normal 난이도
            currentLevel = &Normal;
            timerCount = 0;
            logic = Ms_Logic(Normal.getWidth(), Normal.getHeight(), Normal.getMines(), true); // BFS 사용
            logic.InitializeBoard(); // 보드 초기화
            logic.PlaceMines(-1, -1, 0); // 지뢰 배치 (처음 클릭 위치가 없으므로 -1, -1 전달)
            logic.CalculateSurroundingMines(); // 주변 지뢰 수 계산
            ResizeWindow(hWnd, *currentLevel);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case ID_32773: // Hard 난이도
            currentLevel = &Hard;
            timerCount = 0;
            logic = Ms_Logic(Hard.getWidth(), Hard.getHeight(), Hard.getMines(), true); // BFS 사용
            logic.InitializeBoard(); // 보드 초기화
            logic.PlaceMines(-1, -1, 0); // 지뢰 배치 (처음 클릭 위치가 없으므로 -1, -1 전달)
            logic.CalculateSurroundingMines(); // 주변 지뢰 수 계산
            ResizeWindow(hWnd, *currentLevel);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case ID_BUTTON_RESET:
            timerCount = 0;                        // 타이머 초기화
            logic.InitializeBoard();               // 보드 초기화
            logic.PlaceMines(-1, -1, 0);           // 지뢰 배치 (처음 클릭 위치 없음)
            logic.CalculateSurroundingMines();     // 주변 지뢰 수 계산

            // 게임 상태 초기화
            logic.ResetGameState();                // 게임 오버, 승리 상태 초기화

            InvalidateRect(hWnd, NULL, TRUE);      // 화면 갱신
            break;

        }
        break;


    case WM_TIMER:
        if (wParam == ID_TIMER) {
            timerCount++;
            HWND hTimer = GetDlgItem(hWnd, ID_TIMER);
            if (hTimer) {
                WCHAR timerText[20];
                swprintf_s(timerText, L"Time: %d", timerCount);
                SetWindowTextW(hTimer, timerText);
            }
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 백 버퍼 생성
        HDC memDC = CreateCompatibleDC(hdc);
        RECT rect;
        GetClientRect(hWnd, &rect);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

        // 배경 초기화
        FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

     

        // 남은 지뢰 표시
        RECT minesRect = { 0, 50, 120, 80 }; // 리셋 버튼 아래
        WCHAR mineCountText[20];
        swprintf_s(mineCountText, L"Mines: %d", logic.GetRemainingMines());
        FillRect(memDC, &minesRect, (HBRUSH)(COLOR_WINDOW + 1));
        DrawText(memDC, mineCountText, -1, &minesRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        // 보드 렌더링
        const std::vector<std::vector<Cell>>& board = logic.GetBoard();
        for (int y = 0; y < board.size(); ++y) {
            for (int x = 0; x < board[y].size(); ++x) {
                int startX = x * CELL_WIDTH;
                int startY = y * CELL_HEIGHT + 100;

                if (board[y][x].isClicked) {
                    DrawCell(memDC, true, startX, startY);
                }
                else if (board[y][x].isRevealed) {
                    if (board[y][x].isMine) {
                        DrawMine(memDC, hMineBitmap, startX, startY);
                    }
                    else {
                        DrawCell(memDC, true, startX, startY);
                        if (board[y][x].surroundingMines > 0) {
                            DrawNumberBitmap(memDC, board[y][x].surroundingMines, startX, startY);
                        }
                    }
                }
                else {
                    if (board[y][x].isFlagged) {
                        DrawFlag(memDC, hFlagBitmap, startX, startY);
                    }
                    else {
                        DrawCell(memDC, false, startX, startY);
                    }
                }
            }
        }

        // 백 버퍼에서 화면으로 출력
        BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

        // 리소스 해제
        SelectObject(memDC, oldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER); // 타이머 제거
     
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}



// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
