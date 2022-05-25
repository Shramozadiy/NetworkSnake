// NetworkSnake.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Source.h"

#define STB_IMAGE_IMPLEMENTATION
#include "C:/C++/stb-master/stb_image.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    HWND hWnd;
    HDC hDC;
    HGLRC hRC;

    ReadText::getInstance();
    

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NETWORKSNAKE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow, &hWnd))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NETWORKSNAKE));

    // Активируем OpenGL
    EnableOpenGL(hWnd, &hDC, &hRC);

    int countButtons = 10;
    float betweenButtons = 0.05;
    float heightButtons = (2.0 - betweenButtons * (float)countButtons) / ((float)countButtons);
    float widthButtons = 0.6;
    vector <const char*> textOfButtons;

    textOfButtons.push_back("Создать сервер");
    textOfButtons.push_back("Подключиться");
    textOfButtons.push_back("ip4:");
    textOfButtons.push_back("Выход");

    Text::getInstance().initText("Verdana_B_alpha.png");

    for (float i = 0; i < 4; i++)
    {
        ui::Menu::getInstance().buttons.push_back(ui::Button(-widthButtons / 2,
                                                              heightButtons - i * (heightButtons + betweenButtons),
                                                              widthButtons,
                                                              heightButtons));
        ui::Menu::getInstance().buttons[i].addText(textOfButtons[i], 0.05);
    }

    ui::Menu::getInstance().enable();


    thread A = thread([]()
        {
            std::string str = getIp4Server();
            readText(str);
        });
    A.detach();

    // Инициализация Змеек
    Game::getInstance().addSnake(5, 5, 5);
    Game::getInstance().addSnake(5, 20, 10);

    // Инициализая Еды
    Game::getInstance().addFood();
    Game::getInstance().addFood();
    Game::getInstance().addFood();

    // Цикл основного сообщения:
    MSG msg;
    GetMessage(&msg, nullptr, 0, 0);
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //display(hWnd, hDC);
            glClear(GL_COLOR_BUFFER_BIT);
            glLoadIdentity();
            glPushMatrix(); // Запоминаем текущую матрицу
            scale(hWnd, Game::getInstance().data.getGameSpace().width + 20, Game::getInstance().data.getGameSpace().height + 20);

            //GameSpace::getInstance().display();
            if (isActiveServer() || isActiveClient())
                Game::getInstance().display();
            

            glPopMatrix(); // Возвращаем прошлую матрицу


            glPushMatrix();
            RECT rect;
            GetClientRect(hWnd, &rect); //  Получаем координаты клиентской области окна

            float widthWindow = (float)(rect.right - rect.left); // Ширина окна
            float heightWindow = (float)(rect.bottom - rect.top); // Высота окна
            glScalef(heightWindow / widthWindow,1,1);



            // Отрисовка счета
            if (isActiveServer() || isActiveClient())
            {
                float xText = 0;
                float yText = -0.8;
                int pointFirst = Game::getInstance().data.getGamePoints(0);
                int pointSecond = Game::getInstance().data.getGamePoints(1);
                std::string text_ = std::to_string(pointFirst) + " : " + std::to_string(pointSecond);
                float widthText = Text::getInstance().getLenText(text_.c_str());
                float heightText = 1;
                float countSimbols = strlen(text_.c_str());
                glPushMatrix();
                glTranslatef(xText, yText, 0);
                glScalef(0.1, 0.1, 0);
                glTranslatef(-widthText / 2.0, -heightText / 2, 0);
                Text::getInstance().display(text_.c_str());
                glPopMatrix();
            }


            if (ui::Menu::getInstance().isActive())
                for (auto i : ui::Menu::getInstance().buttons)
            {
                i.display();
            }
            glPopMatrix();

            SwapBuffers(hDC); // Меняем буффер
        }
    }

    // Оключаем OpenGL
    DisableOpenGL(hWnd, hDC, hRC); 

    return (int) msg.wParam;
}


//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NETWORKSNAKE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND* hWnd)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   *hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!(*hWnd))
   {
      return FALSE;
   }

   ShowWindow(*hWnd, nCmdShow);
   UpdateWindow(*hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//


int idSnake = 0;
std::thread logicThread;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CHAR:
    {
        ReadText::getInstance().set(wParam);
    }break;

    case WM_KEYDOWN:
    {
        
        if ((wParam == 'V') && (GetAsyncKeyState(VK_CONTROL)))
        {
            string fromClipboard;//в эту переменную сохраним текст из буфера обмена
            if (OpenClipboard(hWnd))//открываем буфер обмена
            {
                HANDLE hData = GetClipboardData(CF_TEXT);//извлекаем текст из буфера обмена
                char* chBuffer = (char*)GlobalLock(hData);//блокируем память
                fromClipboard = chBuffer;
                GlobalUnlock(hData);//разблокируем память
                CloseClipboard();//закрываем буфер обмена
            }

            for (auto i : fromClipboard)
            {
                ReadText::getInstance().set(i);
            }
        }


        if (isActiveServer() || isActiveClient())
            switch (wParam)
            {
            case 'W':
                Game::getInstance().setMovement(1, idSnake);
                break;
            case 'A':
                Game::getInstance().setMovement(0, idSnake);
                break;
            case 'D':
                Game::getInstance().setMovement(2, idSnake);
                break;
            case 'S':
                Game::getInstance().setMovement(3, idSnake);
                break;
            case VK_SPACE:
                closeServer();
                break;

            default:
                break;
            }
    }break;

    case WM_LBUTTONDOWN:
    {
        float xCursor = LOWORD(lParam);
        float yCursor = HIWORD(lParam);

        RECT rect;
        GetClientRect(hWnd, &rect); //  Получаем координаты клиентской области окна

        float width = (float)(rect.right - rect.left); // Ширина окна
        float height = (float)(rect.bottom - rect.top); // Высота окна

        xCursor = 2.0 * xCursor / width - 1.0;
        yCursor = -2.0 * yCursor / height + 1.0;


        if (ui::Menu::getInstance().isActive())
            for (int i = 0; i < ui::Menu::getInstance().buttons.size(); i++)
            {
                if (ui::Menu::getInstance().buttons[i].isInButton(xCursor, yCursor))
                {
                    switch (i)
                    {
                    case 0:
                        logicThread = std::thread(createServer);
                        logicThread.detach();
                        idSnake = 0;
                        ui::Menu::getInstance().disable();
                        break;
                    case 1:
                        logicThread = std::thread(connectToServer, hWnd);
                        logicThread.detach();
                        idSnake = 1;
                        ui::Menu::getInstance().disable();
                        break;
                    case 3:
                        exit(0);
                        break;
                    default:
                        break;
                    }
                }
            }
    }break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_EXIT:
                closeClient();
                closeServer();
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
       break;
    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);

        float w = rect.right - rect.left;
        float h = rect.bottom - rect.top;
        glViewport(0, 0, w, h);

    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
