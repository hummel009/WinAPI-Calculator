#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUTTON_0_ID 0
#define BUTTON_1_ID 1
#define BUTTON_2_ID 2
#define BUTTON_3_ID 3
#define BUTTON_4_ID 4
#define BUTTON_5_ID 5
#define BUTTON_6_ID 6
#define BUTTON_7_ID 7
#define BUTTON_8_ID 8
#define BUTTON_9_ID 9
#define BUTTON_C_ID 10
#define BUTTON_DIVIDE_ID 11
#define BUTTON_DOT_ID 12
#define BUTTON_EQUALS_ID 13
#define BUTTON_E_ID 14
#define BUTTON_FACTORIAL_ID 15
#define BUTTON_INVERSE_ID 16
#define BUTTON_MINUS_ID 17
#define BUTTON_MULTIPLE_ID 18
#define BUTTON_PI_ID 19
#define BUTTON_PLUS_ID 20
#define BUTTON_SQUARE_ID 21
#define BUTTON_SQUARE_ROOT_ID 22
#define BUTTON_UNARY_MINUS_ID 23

#define DEFAULT_CAPACITY 100

const char *error = "Error!";

HWND field;
char *storage[3];
BOOL storage_presence[3];

const int factorial[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

void registerButton(HWND window, int id, const char *text, int gridX, int gridY);
HWND registerField(HWND window);
void pushSymbolWrapper(const char *symbol);
void pushOperation(const char *operation);
void calculateWrapper();
void initStorage();
void resetStorage();
void push(const char *str);

LRESULT wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int buttonId = LOWORD(wParam);

	switch (msg)
	{
	case WM_CREATE:
		initStorage();
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
			resetStorage();
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
		default:
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(window);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProcA(window, msg, wParam, lParam);
}

int main()
{
	const char *className = "Hummel009's Calculator";
	const char *windowTitle = "Hummel009's Calculator";

	WNDCLASSA windowClass;
	windowClass.style = 0;
	windowClass.lpfnWndProc = wndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = NULL;
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = className;

	RegisterClassA(&windowClass);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int windowWidth = 248;
	int windowHeight = 440;

	int windowX = max(0, (screenWidth - windowWidth) / 2);
	int windowY = max(0, (screenHeight - windowHeight) / 2);

	CreateWindowExA(0, className, windowTitle, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, windowX, windowY, windowWidth, windowHeight, NULL, NULL, NULL, NULL);

	MSG msg;
	while (GetMessageA(&msg, NULL, 0u, 0u) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

void calculateWrapper()
{
	char *buffer = (char *)malloc(DEFAULT_CAPACITY);
	GetWindowTextA(field, buffer, DEFAULT_CAPACITY);

	if (storage_presence[0] == TRUE && storage_presence[1] == TRUE)
	{
		char *op = (char *)malloc(DEFAULT_CAPACITY);
		strcpy_s(op, sizeof(op), storage[1]);

		if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
		{
			push(buffer);

			double operand1 = strtod(storage[0], NULL);
			double operand2 = strtod(storage[2], NULL);

			double result;
			if (strcmp(op, "+") == 0)
			{
				result = operand1 + operand2;
			}
			else if (strcmp(op, "-") == 0)
			{
				result = operand1 - operand2;
			}
			else if (strcmp(op, "*") == 0)
			{
				result = operand1 * operand2;
			}
			else if (strcmp(op, "/") == 0)
			{
				result = operand1 / operand2;
			}
			else
			{
				free(op);
				goto exception;
			}

			char *str = (char *)malloc(DEFAULT_CAPACITY);
			snprintf(str, sizeof(str), "%f", result);
			SetWindowTextA(field, str);
			free(str);
		}
		else if (strcmp(op, "!") == 0 || strcmp(op, "s") == 0 || strcmp(op, "i") == 0 || strcmp(op, "r") == 0)
		{
			double operand = strtod(storage[0], NULL);

			double result;
			if (strcmp(op, "!") == 0 && (int)operand >= 0 && (int)operand <= 12)
			{
				result = factorial[(int)operand];
			}
			else if (strcmp(op, "s") == 0)
			{
				result = operand * operand;
			}
			else if (strcmp(op, "i") == 0)
			{
				result = 1.0 / operand;
			}
			else if (strcmp(op, "r") == 0)
			{
				result = sqrt(operand);
			}
			else
			{
				free(op);
				goto exception;
			}

			char *str = (char *)malloc(DEFAULT_CAPACITY);
			snprintf(str, sizeof(str), "%f", result);
			SetWindowTextA(field, str);
			free(str);
		}
		free(op);
	}
	free(buffer);
	resetStorage();
	return;

exception:
	free(buffer);
	resetStorage();
	SetWindowTextA(field, error);
}

void pushOperation(const char *operation)
{
	char *buffer = (char *)malloc(DEFAULT_CAPACITY);
	GetWindowTextA(field, buffer, DEFAULT_CAPACITY);
	if (storage_presence[0] == FALSE)
	{
		push(buffer);
		push(operation);
		SetWindowTextA(field, "");
	}
	else
	{
		resetStorage();
		SetWindowTextA(field, error);
	}
	free(buffer);
}

void pushSymbol(const char *symbol);
void pushSymbolWrapper(const char *symbol)
{
	char *buffer = (char *)malloc(DEFAULT_CAPACITY);
	GetWindowTextA(field, buffer, DEFAULT_CAPACITY);
	if (strcmp(symbol, "3.14") == 0 || strcmp(symbol, "2.72") == 0 || strcmp(symbol, "-") == 0)
	{
		if (strnlen(buffer, sizeof(buffer)) == 0)
		{
			pushSymbol(symbol);
		}
	}
	else if (strcmp(symbol, ".") == 0)
	{
		if (strchr(buffer, '.') == NULL && strnlen(buffer, sizeof(buffer)) != 0)
		{
			pushSymbol(symbol);
		}
	}
	else
	{
		if (storage_presence[0] == FALSE)
		{
			pushSymbol(symbol);
		}
		else
		{
			const char *op = storage[1];

			if (strcmp(op, "!") != 0 && strcmp(op, "s") != 0 && strcmp(op, "i") != 0 && strcmp(op, "r") != 0)
			{
				pushSymbol(symbol);
			}
		}
	}
	free(buffer);
}

void pushSymbol(const char *symbol)
{
	char *buffer = (char *)malloc(DEFAULT_CAPACITY);
	GetWindowTextA(field, buffer, DEFAULT_CAPACITY);
	if (strcmp(buffer, error) == 0)
	{
		SetWindowTextA(field, symbol);
	}
	else
	{
		strcat_s(buffer, sizeof(buffer), symbol);
		SetWindowTextA(field, buffer);
	}
	free(buffer);
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
		NULL,
		NULL,
		NULL);
}

void registerButton(HWND window, int id, const char *text, int gridX, int gridY)
{
	int buttonWidth = 60;
	int buttonHeight = 60;

	CreateWindowExA(
		WS_EX_CLIENTEDGE,
		"BUTTON",
		text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD,
		buttonWidth * gridX + 1,
		buttonHeight * gridY + 50,
		buttonWidth,
		buttonHeight,
		window,
		(HMENU)id,
		NULL,
		NULL);
}

void push(const char *str)
{
	if (storage_presence[0] == FALSE)
	{
		strcpy_s(storage[0], sizeof(storage[0]), str);
		storage_presence[0] = TRUE;
		return;
	}
	else if (storage_presence[1] == FALSE)
	{
		strcpy_s(storage[1], sizeof(storage[1]), str);
		storage_presence[1] = TRUE;
		return;
	}
	else if (storage_presence[2] == FALSE)
	{
		strcpy_s(storage[2], sizeof(storage[2]), str);
		storage_presence[2] = TRUE;
		return;
	}
}

void initStorage()
{
	storage[0] = (char *)malloc(DEFAULT_CAPACITY);
	storage[1] = (char *)malloc(DEFAULT_CAPACITY);
	storage[2] = (char *)malloc(DEFAULT_CAPACITY);
	storage_presence[0] = FALSE;
	storage_presence[1] = FALSE;
	storage_presence[2] = FALSE;
}

void resetStorage()
{
	free(storage[0]);
	free(storage[1]);
	free(storage[2]);
	initStorage();
}