#include <iostream>
#include <windows.h>
#include <vector>
#include <set>
#include <cmath>
#include <memory>

using namespace std;

const int BUTTON_0_ID = 0;
const int BUTTON_1_ID = 1;
const int BUTTON_2_ID = 2;
const int BUTTON_3_ID = 3;
const int BUTTON_4_ID = 4;
const int BUTTON_5_ID = 5;
const int BUTTON_6_ID = 6;
const int BUTTON_7_ID = 7;
const int BUTTON_8_ID = 8;
const int BUTTON_9_ID = 9;
const int BUTTON_C_ID = 10;
const int BUTTON_DIVIDE_ID = 11;
const int BUTTON_DOT_ID = 12;
const int BUTTON_EQUALS_ID = 13;
const int BUTTON_E_ID = 14;
const int BUTTON_FACTORIAL_ID = 15;
const int BUTTON_INVERSE_ID = 16;
const int BUTTON_MINUS_ID = 17;
const int BUTTON_MULTIPLE_ID = 18;
const int BUTTON_PI_ID = 19;
const int BUTTON_PLUS_ID = 20;
const int BUTTON_SQUARE_ID = 21;
const int BUTTON_SQUARE_ROOT_ID = 22;
const int BUTTON_UNARY_MINUS_ID = 23;

const int DEFAULT_CAPACITY = 100;

const vector<int> FACTORIAL = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

HWND field;
vector<string> storage;

void registerButton(HWND window, int id, string text, int gridX, int gridY);
HWND registerField(HWND window);
void pushSymbolWrapper(string symbol);
void pushOperation(string operation);
void calculateWrapper();

LRESULT wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto buttonId = LOWORD(wParam);

	switch (msg)
	{
	case WM_CREATE:
		field = registerField(window);

		registerButton(window, BUTTON_PI_ID, "p", 0, 0);
		registerButton(window, BUTTON_E_ID, "e", 1, 0);
		registerButton(window, BUTTON_C_ID, "C", 2, 0);
		registerButton(window, BUTTON_FACTORIAL_ID, "x!", 3, 0);
		registerButton(window, BUTTON_INVERSE_ID, "1/x", 0, 1);
		registerButton(window, BUTTON_SQUARE_ID, "x^2", 1, 1);
		registerButton(window, BUTTON_SQUARE_ROOT_ID, "sqrt(x)", 2, 1);
		registerButton(window, BUTTON_DIVIDE_ID, "/", 3, 1);
		registerButton(window, BUTTON_7_ID, "7", 0, 2);
		registerButton(window, BUTTON_8_ID, "8", 1, 2);
		registerButton(window, BUTTON_9_ID, "9", 2, 2);
		registerButton(window, BUTTON_MULTIPLE_ID, "*", 3, 2);
		registerButton(window, BUTTON_4_ID, "4", 0, 3);
		registerButton(window, BUTTON_5_ID, "5", 1, 3);
		registerButton(window, BUTTON_6_ID, "6", 2, 3);
		registerButton(window, BUTTON_MINUS_ID, "-", 3, 3);
		registerButton(window, BUTTON_1_ID, "1", 0, 4);
		registerButton(window, BUTTON_2_ID, "2", 1, 4);
		registerButton(window, BUTTON_3_ID, "3", 2, 4);
		registerButton(window, BUTTON_PLUS_ID, "+", 3, 4);
		registerButton(window, BUTTON_UNARY_MINUS_ID, "-", 0, 5);
		registerButton(window, BUTTON_0_ID, "0", 1, 5);
		registerButton(window, BUTTON_DOT_ID, ".", 2, 5);
		registerButton(window, BUTTON_EQUALS_ID, "=", 3, 5);
		break;
	case WM_COMMAND:
		switch (buttonId)
		{
		case BUTTON_0_ID:
			pushSymbolWrapper("0");
			break;
		case BUTTON_1_ID:
			pushSymbolWrapper("1");
			break;
		case BUTTON_2_ID:
			pushSymbolWrapper("2");
			break;
		case BUTTON_3_ID:
			pushSymbolWrapper("3");
			break;
		case BUTTON_4_ID:
			pushSymbolWrapper("4");
			break;
		case BUTTON_5_ID:
			pushSymbolWrapper("5");
			break;
		case BUTTON_6_ID:
			pushSymbolWrapper("6");
			break;
		case BUTTON_7_ID:
			pushSymbolWrapper("7");
			break;
		case BUTTON_8_ID:
			pushSymbolWrapper("8");
			break;
		case BUTTON_9_ID:
			pushSymbolWrapper("9");
			break;
		case BUTTON_E_ID:
			pushSymbolWrapper("2.72");
			break;
		case BUTTON_PI_ID:
			pushSymbolWrapper("3.14");
			break;
		case BUTTON_DOT_ID:
			pushSymbolWrapper(".");
			break;
		case BUTTON_UNARY_MINUS_ID:
			pushSymbolWrapper("-");
			break;
		case BUTTON_C_ID:
			storage.clear();
			SetWindowTextA(field, "");
			break;
		case BUTTON_DIVIDE_ID:
			pushOperation("/");
			break;
		case BUTTON_MULTIPLE_ID:
			pushOperation("*");
			break;
		case BUTTON_MINUS_ID:
			pushOperation("-");
			break;
		case BUTTON_PLUS_ID:
			pushOperation("+");
			break;
		case BUTTON_FACTORIAL_ID:
			pushOperation("!");
			break;
		case BUTTON_SQUARE_ID:
			pushOperation("s");
			break;
		case BUTTON_INVERSE_ID:
			pushOperation("i");
			break;
		case BUTTON_SQUARE_ROOT_ID:
			pushOperation("r");
			break;
		case BUTTON_EQUALS_ID:
			calculateWrapper();
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(window);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcA(window, msg, wParam, lParam);
}

int main()
{
	auto className = "Hummel009's Calculator";
	auto windowTitle = "Hummel009's Calculator";

	WNDCLASSA windowClass;
	windowClass.style = 0;
	windowClass.lpfnWndProc = wndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = nullptr;
	windowClass.hIcon = nullptr;
	windowClass.hCursor = nullptr;
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = nullptr;
	windowClass.lpszClassName = className;

	RegisterClassA(&windowClass);

	auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
	auto screenHeight = GetSystemMetrics(SM_CYSCREEN);

	auto windowWidth = 248;
	auto windowHeight = 440;

	auto windowX = max(0, (screenWidth - windowWidth) / 2);
	auto windowY = max(0, (screenHeight - windowHeight) / 2);

	CreateWindowExA(0, className, windowTitle, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, windowX, windowY, windowWidth, windowHeight, nullptr, nullptr, nullptr, nullptr);

	MSG msg;
	while (GetMessageA(&msg, nullptr, 0u, 0u) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

void calculate();
void calculateWrapper()
{
	try
	{
		calculate();
	}
	catch (exception &e)
	{
		storage.clear();
		SetWindowTextA(field, "Error!");
	}
}

void calculate()
{
	auto buffer = make_unique<char[]>(DEFAULT_CAPACITY);
	GetWindowTextA(field, buffer.get(), DEFAULT_CAPACITY);

	if (storage.size() == 2)
	{
		string op = storage[1];
		set<string, less<>> ops1 = {"+", "-", "*", "/"};
		set<string, less<>> ops2 = {"!", "s", "i", "r"};

		if (ops1.contains(op))
		{
			string str(buffer.get());
			storage.push_back(str);

			auto operand1 = stod(storage[0]);
			auto operand2 = stod(storage[2]);

			double result;
			if (op == "+")
			{
				result = operand1 + operand2;
			}
			else if (op == "-")
			{
				result = operand1 - operand2;
			}
			else if (op == "*")
			{
				result = operand1 * operand2;
			}
			else if (op == "/")
			{
				result = operand1 / operand2;
			}
			else
			{
				throw exception();
			}

			storage.clear();

			SetWindowTextA(field, to_string(result).c_str());
		}
		else if (ops2.contains(op))
		{
			auto operand = stod(storage[0]);

			double result;
			if (op == "!")
			{
				result = FACTORIAL[(int)operand];
			}
			else if (op == "s")
			{
				result = operand * operand;
			}
			else if (op == "i")
			{
				result = 1.0 / operand;
			}
			else if (op == "r")
			{
				result = sqrt(operand);
			}
			else
			{
				throw exception();
			}

			storage.clear();

			SetWindowTextA(field, to_string(result).c_str());
		}
	}
}

void pushOperation(string operation)
{
	auto buffer = make_unique<char[]>(DEFAULT_CAPACITY);
	GetWindowTextA(field, buffer.get(), DEFAULT_CAPACITY);

	if (storage.empty())
	{
		string str(buffer.get());
		storage.push_back(str);
		storage.push_back(operation);
		SetWindowTextA(field, "");
	}
	else
	{
		storage.clear();
		SetWindowTextA(field, "Error!");
	}
}

void pushSymbol(string symbol);
void pushSymbolWrapper(string symbol)
{
	auto buffer = make_unique<char[]>(DEFAULT_CAPACITY);
	GetWindowTextA(field, buffer.get(), DEFAULT_CAPACITY);
	string str(buffer.get());

	if (symbol == "3.14" || symbol == "2.72" || symbol == "-")
	{
		if (str.empty())
		{
			pushSymbol(symbol);
		}
	}
	else if (symbol == ".")
	{
		if (str.find(".") == string::npos && !str.empty())
		{
			pushSymbol(symbol);
		}
	}
	else
	{
		if (storage.empty())
		{
			pushSymbol(symbol);
		}
		else
		{
			string op = storage[1];
			set<string, less<>> ops = {"!", "s", "i", "r"};

			if (!ops.contains(op))
			{
				pushSymbol(symbol);
			}
		}
	}
}

void pushSymbol(string symbol)
{
	auto buffer = make_unique<char[]>(DEFAULT_CAPACITY);
    GetWindowTextA(field, buffer.get(), DEFAULT_CAPACITY);
	string str(buffer.get());
	if (str == "Error!")
	{
		SetWindowTextA(field, symbol.c_str());
	}
	else
	{
		SetWindowTextA(field, (str + symbol).c_str());
	}
}

HWND registerField(HWND window)
{
	return CreateWindowExA(
		WS_EX_CLIENTEDGE,
		"STATIC",
		"",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD,
		1,
		1,
		239,
		48,
		window,
		nullptr,
		nullptr,
		nullptr);
}

void registerButton(HWND window, int id, string text, int gridX, int gridY)
{
	auto buttonWidth = 60;
	auto buttonHeight = 60;

	CreateWindowExA(
		WS_EX_CLIENTEDGE,
		"BUTTON",
		text.c_str(),
		WS_TABSTOP | WS_VISIBLE | WS_CHILD,
		buttonWidth * gridX + 1,
		buttonHeight * gridY + 50,
		buttonWidth,
		buttonHeight,
		window,
		reinterpret_cast<HMENU>(id),
		nullptr,
		nullptr);
}