package com.github.hummel.wincalc;

import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.platform.win32.WinDef;
import com.sun.jna.platform.win32.WinUser;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class Main {
	private static final int WM_COMMAND = 0x0111;
	private static final int COLOR_WINDOW = 0x5;
	private static final int BUTTON_0_ID = 0;
	private static final int BUTTON_1_ID = 1;
	private static final int BUTTON_2_ID = 2;
	private static final int BUTTON_3_ID = 3;
	private static final int BUTTON_4_ID = 4;
	private static final int BUTTON_5_ID = 5;
	private static final int BUTTON_6_ID = 6;
	private static final int BUTTON_7_ID = 7;
	private static final int BUTTON_8_ID = 8;
	private static final int BUTTON_9_ID = 9;
	private static final int BUTTON_C_ID = 10;
	private static final int BUTTON_DIVIDE_ID = 11;
	private static final int BUTTON_DOT_ID = 12;
	private static final int BUTTON_EQUALS_ID = 13;
	private static final int BUTTON_E_ID = 14;
	private static final int BUTTON_FACTORIAL_ID = 15;
	private static final int BUTTON_INVERSE_ID = 16;
	private static final int BUTTON_MINUS_ID = 17;
	private static final int BUTTON_MULTIPLE_ID = 18;
	private static final int BUTTON_PI_ID = 19;
	private static final int BUTTON_PLUS_ID = 20;
	private static final int BUTTON_SQUARE_ID = 21;
	private static final int BUTTON_SQUARE_ROOT_ID = 22;
	private static final int BUTTON_UNARY_MINUS_ID = 23;

	private static final int DEFAULT_CAPACITY = 100;
	private static final String ERROR = "Error!";

	private static final List<String> STORAGE = new ArrayList<>();
	private static final int[] FACTORIAL = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600};

	private static WinDef.HWND field;

	public static void main(String[] args) {
		var className = "Hummel009's Calculator";
		var windowTitle = "Hummel009's Calculator";

		var windowClass = new WinUser.WNDCLASSEX();
		windowClass.cbSize = windowClass.size();
		windowClass.style = 0;
		windowClass.lpfnWndProc = new WndProc();
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = null;
		windowClass.hIcon = null;
		windowClass.hCursor = null;
		windowClass.hbrBackground = new WinDef.HBRUSH(new Pointer(COLOR_WINDOW));
		windowClass.lpszMenuName = null;
		windowClass.lpszClassName = className;
		windowClass.hIcon = null;

		ExUser32.INSTANCE.RegisterClassEx(windowClass);

		var screenWidth = ExUser32.INSTANCE.GetSystemMetrics(WinUser.SM_CXSCREEN);
		var screenHeight = ExUser32.INSTANCE.GetSystemMetrics(WinUser.SM_CYSCREEN);

		var windowWidth = 260;
		var windowHeight = 458;

		var windowX = Math.max(0, (screenWidth - windowWidth) / 2);
		var windowY = Math.max(0, (screenHeight - windowHeight) / 2);

		ExUser32.INSTANCE.CreateWindowEx(0, className, windowTitle, WinUser.WS_VISIBLE | WinUser.WS_CAPTION | WinUser.WS_SYSMENU, windowX, windowY, windowWidth, windowHeight, null, null, null, null);

		var msg = new WinUser.MSG();
		while (ExUser32.INSTANCE.GetMessage(msg, null, 0, 0) != 0) {
			ExUser32.INSTANCE.TranslateMessage(msg);
			ExUser32.INSTANCE.DispatchMessage(msg);
		}
	}

	private static class WndProc implements WinUser.WindowProc {
		@Override
		public WinDef.LRESULT callback(WinDef.HWND window, int msg, WinDef.WPARAM wParam, WinDef.LPARAM lParam) {
			switch (msg) {
				case WinUser.WM_CREATE -> {
					field = registerField(window);

					registerButton(window, BUTTON_PI_ID, "π", 0, 0);
					registerButton(window, BUTTON_E_ID, "e", 1, 0);
					registerButton(window, BUTTON_C_ID, "C", 2, 0);
					registerButton(window, BUTTON_FACTORIAL_ID, "x!", 3, 0);
					registerButton(window, BUTTON_INVERSE_ID, "1/x", 0, 1);
					registerButton(window, BUTTON_SQUARE_ID, "x^2", 1, 1);
					registerButton(window, BUTTON_SQUARE_ROOT_ID, "√x", 2, 1);
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
				}

				case WM_COMMAND -> {
					var buttonId = loword(wParam);

					switch (buttonId) {
						case BUTTON_0_ID -> pushSymbolWrapper("0");
						case BUTTON_1_ID -> pushSymbolWrapper("1");
						case BUTTON_2_ID -> pushSymbolWrapper("2");
						case BUTTON_3_ID -> pushSymbolWrapper("3");
						case BUTTON_4_ID -> pushSymbolWrapper("4");
						case BUTTON_5_ID -> pushSymbolWrapper("5");
						case BUTTON_6_ID -> pushSymbolWrapper("6");
						case BUTTON_7_ID -> pushSymbolWrapper("7");
						case BUTTON_8_ID -> pushSymbolWrapper("8");
						case BUTTON_9_ID -> pushSymbolWrapper("9");

						case BUTTON_E_ID -> pushSymbolWrapper("2.72");
						case BUTTON_PI_ID -> pushSymbolWrapper("3.14");

						case BUTTON_DOT_ID -> pushSymbolWrapper(".");
						case BUTTON_UNARY_MINUS_ID -> pushSymbolWrapper("-");

						case BUTTON_C_ID -> {
							STORAGE.clear();
							ExUser32.INSTANCE.SetWindowText(field, "");
						}

						case BUTTON_DIVIDE_ID -> pushOperation("/");
						case BUTTON_MULTIPLE_ID -> pushOperation("*");
						case BUTTON_MINUS_ID -> pushOperation("-");
						case BUTTON_PLUS_ID -> pushOperation("+");

						case BUTTON_FACTORIAL_ID -> pushOperation("!");
						case BUTTON_SQUARE_ID -> pushOperation("s");
						case BUTTON_INVERSE_ID -> pushOperation("i");
						case BUTTON_SQUARE_ROOT_ID -> pushOperation("r");

						case BUTTON_EQUALS_ID -> calculateWrapper();
					}
				}

				case WinUser.WM_CLOSE -> ExUser32.INSTANCE.DestroyWindow(window);
				case WinUser.WM_DESTROY -> ExUser32.INSTANCE.PostQuitMessage(0);
			}
			return ExUser32.INSTANCE.DefWindowProc(window, msg, wParam, lParam);
		}

		private static void calculateWrapper() {
			try {
				calculate(field);
			} catch (Exception e) {
				STORAGE.clear();
				ExUser32.INSTANCE.SetWindowText(field, ERROR);
			}
		}

		@SuppressWarnings("NumericCastThatLosesPrecision")
		private static void calculate(WinDef.HWND field) {
			var buffer = new char[DEFAULT_CAPACITY];
			ExUser32.INSTANCE.GetWindowText(field, buffer, DEFAULT_CAPACITY);

			if (STORAGE.size() == 2) {
				var operator = STORAGE.get(1);

				if (Set.of("+", "-", "*", "/").contains(operator)) {
					STORAGE.add(Native.toString(buffer));

					var operand1 = Double.parseDouble(STORAGE.get(0));
					var operand2 = Double.parseDouble(STORAGE.get(2));

					var result = switch (operator) {
						case "+" -> operand1 + operand2;
						case "-" -> operand1 - operand2;
						case "*" -> operand1 * operand2;
						case "/" -> operand1 / operand2;
						default -> throw new IllegalArgumentException("Invalid operator: " + operator);
					};

					STORAGE.clear();

					ExUser32.INSTANCE.SetWindowText(field, String.valueOf(result));
				} else if (Set.of("!", "s", "i", "r").contains(operator)) {
					var operand = Double.parseDouble(STORAGE.get(0));

					var result = switch (operator) {
						case "!" -> FACTORIAL[(int) operand];
						case "s" -> operand * operand;
						case "i" -> 1.0 / operand;
						case "r" -> Math.sqrt(operand);
						default -> throw new IllegalArgumentException("Invalid operator: " + operator);
					};

					STORAGE.clear();

					ExUser32.INSTANCE.SetWindowText(field, String.valueOf(result));
				}
			}
		}

		private static void pushOperation(String operation) {
			var buffer = new char[DEFAULT_CAPACITY];
			ExUser32.INSTANCE.GetWindowText(field, buffer, DEFAULT_CAPACITY);

			if (STORAGE.isEmpty()) {
				STORAGE.add(Native.toString(buffer));
				STORAGE.add(operation);
				ExUser32.INSTANCE.SetWindowText(field, "");
			} else {
				STORAGE.clear();
				ExUser32.INSTANCE.SetWindowText(field, ERROR);
			}
		}

		private static void pushSymbolWrapper(String symbol) {
			var buffer = new char[DEFAULT_CAPACITY];
			ExUser32.INSTANCE.GetWindowText(field, buffer, DEFAULT_CAPACITY);
			var str = Native.toString(buffer);

			switch (symbol) {
				case "3.14", "2.72", "-" -> {
					if (str.isEmpty()) {
						pushSymbol(symbol);
					}
				}
				case "." -> {
					if (!str.contains(".") && !str.isEmpty()) {
						pushSymbol(symbol);
					}
				}
				default -> {
					if (STORAGE.isEmpty()) {
						pushSymbol(symbol);
					} else {
						var operator = STORAGE.get(1);

						if (!Set.of("!", "s", "i", "r").contains(operator)) {
							pushSymbol(symbol);
						}
					}
				}
			}
		}

		private static void pushSymbol(String symbol) {
			var buffer = new char[DEFAULT_CAPACITY];
			ExUser32.INSTANCE.GetWindowText(field, buffer, DEFAULT_CAPACITY);
			var str = Native.toString(buffer);
			if (ERROR.equals(str)) {
				ExUser32.INSTANCE.SetWindowText(field, symbol);
			} else {
				ExUser32.INSTANCE.SetWindowText(field, str + symbol);
			}
		}

		private static WinDef.HWND registerField(WinDef.HWND window) {
			return ExUser32.INSTANCE.CreateWindowEx(0, "STATIC", "", WinUser.WS_TABSTOP | WinUser.WS_VISIBLE | WinUser.WS_CHILD, 1, 1, 239, 48, window, null, null, null);
		}

		private static void registerButton(WinDef.HWND window, int id, String text, int gridX, int gridY) {
			var buttonWidth = 60;
			var buttonHeight = 60;

			ExUser32.INSTANCE.CreateWindowEx(0, "BUTTON", text, WinUser.WS_TABSTOP | WinUser.WS_VISIBLE | WinUser.WS_CHILD, buttonWidth * gridX + 1, buttonHeight * gridY + 50, buttonWidth, buttonHeight, window, new WinDef.HMENU(new Pointer(id)), null, null);
		}

		private static int loword(WinDef.WPARAM wparam) {
			return (int) (wparam.longValue() & 0xFFFF);
		}
	}
}