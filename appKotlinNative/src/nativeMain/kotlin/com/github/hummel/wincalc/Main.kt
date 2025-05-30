package com.github.hummel.wincalc

import kotlinx.cinterop.*
import platform.windows.*
import kotlin.math.max
import kotlin.math.sqrt

private const val BUTTON_0_ID: Int = 0
private const val BUTTON_1_ID: Int = 1
private const val BUTTON_2_ID: Int = 2
private const val BUTTON_3_ID: Int = 3
private const val BUTTON_4_ID: Int = 4
private const val BUTTON_5_ID: Int = 5
private const val BUTTON_6_ID: Int = 6
private const val BUTTON_7_ID: Int = 7
private const val BUTTON_8_ID: Int = 8
private const val BUTTON_9_ID: Int = 9
private const val BUTTON_C_ID: Int = 10
private const val BUTTON_DIVIDE_ID: Int = 11
private const val BUTTON_DOT_ID: Int = 12
private const val BUTTON_EQUALS_ID: Int = 13
private const val BUTTON_E_ID: Int = 14
private const val BUTTON_FACTORIAL_ID: Int = 15
private const val BUTTON_INVERSE_ID: Int = 16
private const val BUTTON_MINUS_ID: Int = 17
private const val BUTTON_MULTIPLE_ID: Int = 18
private const val BUTTON_PI_ID: Int = 19
private const val BUTTON_PLUS_ID: Int = 20
private const val BUTTON_SQUARE_ID: Int = 21
private const val BUTTON_SQUARE_ROOT_ID: Int = 22
private const val BUTTON_UNARY_MINUS_ID: Int = 23

private const val DEFAULT_CAPACITY: Int = 100
private const val ERROR: String = "Error!"

private val storage: MutableList<String> = ArrayList()
private val factorial: Array<Int> = arrayOf(1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600)

private lateinit var field: HWND

fun main() {
	memScoped {
		val className = "Hummel009's Calculator"
		val windowTitle = "Hummel009's Calculator"

		val windowClass = alloc<WNDCLASSW>()
		windowClass.style = 0u
		windowClass.lpfnWndProc = staticCFunction(::wndProc)
		windowClass.cbClsExtra = 0
		windowClass.cbWndExtra = 0
		windowClass.hInstance = null
		windowClass.hIcon = null
		windowClass.hCursor = null
		windowClass.hbrBackground = (COLOR_WINDOW + 1).toLong().toCPointer()
		windowClass.lpszMenuName = null
		windowClass.lpszClassName = className.wcstr.ptr

		RegisterClassW(windowClass.ptr)

		val screenWidth = GetSystemMetrics(SM_CXSCREEN)
		val screenHeight = GetSystemMetrics(SM_CYSCREEN)

		val windowWidth = 257
		val windowHeight = 449

		val windowX = max(0, (screenWidth - windowWidth) / 2)
		val windowY = max(0, (screenHeight - windowHeight) / 2)

		CreateWindowExW(
			0u,
			className,
			windowTitle,
			(WS_VISIBLE or WS_CAPTION or WS_SYSMENU).toUInt(),
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			null,
			null,
			null,
			null
		)

		val msg = alloc<MSG>()
		while (GetMessageW(msg.ptr, null, 0u, 0u) != 0) {
			TranslateMessage(msg.ptr)
			DispatchMessageW(msg.ptr)
		}
	}
}

private fun wndProc(window: HWND?, msg: UINT, wParam: WPARAM, lParam: LPARAM): LRESULT {
	when (msg.toInt()) {
		WM_CREATE -> {
			field = registerField(window)

			registerButton(window, BUTTON_PI_ID, "π", 0, 0)
			registerButton(window, BUTTON_E_ID, "e", 1, 0)
			registerButton(window, BUTTON_C_ID, "C", 2, 0)
			registerButton(window, BUTTON_FACTORIAL_ID, "x!", 3, 0)
			registerButton(window, BUTTON_INVERSE_ID, "1/x", 0, 1)
			registerButton(window, BUTTON_SQUARE_ID, "x^2", 1, 1)
			registerButton(window, BUTTON_SQUARE_ROOT_ID, "√x", 2, 1)
			registerButton(window, BUTTON_DIVIDE_ID, "/", 3, 1)
			registerButton(window, BUTTON_7_ID, "7", 0, 2)
			registerButton(window, BUTTON_8_ID, "8", 1, 2)
			registerButton(window, BUTTON_9_ID, "9", 2, 2)
			registerButton(window, BUTTON_MULTIPLE_ID, "*", 3, 2)
			registerButton(window, BUTTON_4_ID, "4", 0, 3)
			registerButton(window, BUTTON_5_ID, "5", 1, 3)
			registerButton(window, BUTTON_6_ID, "6", 2, 3)
			registerButton(window, BUTTON_MINUS_ID, "-", 3, 3)
			registerButton(window, BUTTON_1_ID, "1", 0, 4)
			registerButton(window, BUTTON_2_ID, "2", 1, 4)
			registerButton(window, BUTTON_3_ID, "3", 2, 4)
			registerButton(window, BUTTON_PLUS_ID, "+", 3, 4)
			registerButton(window, BUTTON_UNARY_MINUS_ID, "-", 0, 5)
			registerButton(window, BUTTON_0_ID, "0", 1, 5)
			registerButton(window, BUTTON_DOT_ID, ".", 2, 5)
			registerButton(window, BUTTON_EQUALS_ID, "=", 3, 5)
		}

		WM_COMMAND -> {
			val buttonId = wParam.loword()

			when (buttonId) {
				BUTTON_0_ID -> pushSymbolWrapper("0")
				BUTTON_1_ID -> pushSymbolWrapper("1")
				BUTTON_2_ID -> pushSymbolWrapper("2")
				BUTTON_3_ID -> pushSymbolWrapper("3")
				BUTTON_4_ID -> pushSymbolWrapper("4")
				BUTTON_5_ID -> pushSymbolWrapper("5")
				BUTTON_6_ID -> pushSymbolWrapper("6")
				BUTTON_7_ID -> pushSymbolWrapper("7")
				BUTTON_8_ID -> pushSymbolWrapper("8")
				BUTTON_9_ID -> pushSymbolWrapper("9")

				BUTTON_E_ID -> pushSymbolWrapper("2.72")
				BUTTON_PI_ID -> pushSymbolWrapper("3.14")

				BUTTON_DOT_ID -> pushSymbolWrapper(".")
				BUTTON_UNARY_MINUS_ID -> pushSymbolWrapper("-")

				BUTTON_C_ID -> {
					storage.clear()
					SetWindowTextW(field, "")
				}

				BUTTON_DIVIDE_ID -> pushOperation("/")
				BUTTON_MULTIPLE_ID -> pushOperation("*")
				BUTTON_MINUS_ID -> pushOperation("-")
				BUTTON_PLUS_ID -> pushOperation("+")

				BUTTON_FACTORIAL_ID -> pushOperation("!")
				BUTTON_SQUARE_ID -> pushOperation("s")
				BUTTON_INVERSE_ID -> pushOperation("i")
				BUTTON_SQUARE_ROOT_ID -> pushOperation("r")

				BUTTON_EQUALS_ID -> calculateWrapper()
			}
		}

		WM_CLOSE -> DestroyWindow(window)
		WM_DESTROY -> PostQuitMessage(0)
	}
	return DefWindowProcW(window, msg, wParam, lParam)
}

private fun calculateWrapper() {
	try {
		calculate(field)
	} catch (_: Exception) {
		storage.clear()
		SetWindowTextW(field, ERROR)
	}
}

private fun calculate(field: HWND) {
	memScoped {
		val buffer = allocArray<WCHARVar>(DEFAULT_CAPACITY)
		GetWindowTextW(field, buffer.reinterpret(), DEFAULT_CAPACITY)

		if (storage.size == 2) {
			val operator = storage[1]

			if (operator in setOf("+", "-", "*", "/")) {
				storage.add(buffer.toKString())

				val operand1 = storage[0].toDouble()
				val operand2 = storage[2].toDouble()

				val result = when (operator) {
					"+" -> operand1 + operand2
					"-" -> operand1 - operand2
					"*" -> operand1 * operand2
					"/" -> operand1 / operand2
					else -> throw IllegalArgumentException("Invalid operator: $operator")
				}

				storage.clear()

				SetWindowTextW(field, "$result")
			} else if (operator in setOf("!", "s", "i", "r")) {
				val operand = storage[0].toDouble()

				val result = when (operator) {
					"!" -> factorial[operand.toInt()]
					"s" -> operand * operand
					"i" -> 1.0 / operand
					"r" -> sqrt(operand)
					else -> throw IllegalArgumentException("Invalid operator: $operator")
				}

				storage.clear()

				SetWindowTextW(field, "$result")
			}
		}
	}
}

private fun pushOperation(operation: String) {
	memScoped {
		val buffer = allocArray<WCHARVar>(DEFAULT_CAPACITY)
		GetWindowTextW(field, buffer.reinterpret(), DEFAULT_CAPACITY)

		if (storage.isEmpty()) {
			storage.add(buffer.toKString())
			storage.add(operation)
			SetWindowTextW(field, "")
		} else {
			storage.clear()
			SetWindowTextW(field, ERROR)
		}
	}
}

private fun pushSymbolWrapper(symbol: String) {
	memScoped {
		val buffer = allocArray<WCHARVar>(DEFAULT_CAPACITY)
		GetWindowTextW(field, buffer.reinterpret(), DEFAULT_CAPACITY)
		val str = buffer.toKString()

		when (symbol) {
			"3.14", "2.72", "-" -> {
				if (str.isEmpty()) {
					pushSymbol(symbol)
				}
			}

			"." -> {
				if (!str.contains(".") && str.isNotEmpty()) {
					pushSymbol(symbol)
				}
			}

			else -> {
				if (storage.isEmpty()) {
					pushSymbol(symbol)
				} else {
					val operator = storage[1]

					if (operator !in setOf("!", "s", "i", "r")) {
						pushSymbol(symbol)
					}
				}
			}
		}
	}
}


private fun pushSymbol(symbol: String) {
	memScoped {
		val buffer = allocArray<WCHARVar>(DEFAULT_CAPACITY)
		GetWindowTextW(field, buffer.reinterpret(), DEFAULT_CAPACITY)
		val str = buffer.toKString()
		if (str == ERROR) {
			SetWindowTextW(field, symbol)
		} else {
			SetWindowTextW(field, str + symbol)
		}
	}
}

private fun registerField(window: HWND?): HWND {
	return CreateWindowExW(
		WS_EX_CLIENTEDGE.toUInt(),
		"STATIC",
		"",
		(WS_TABSTOP or WS_VISIBLE or WS_CHILD).toUInt(),
		1,
		1,
		239,
		48,
		window,
		null,
		null,
		null
	)!!
}

private fun registerButton(window: HWND?, id: Int, text: String, gridX: Int, gridY: Int) {
	val buttonWidth = 60
	val buttonHeight = 60

	CreateWindowExW(
		WS_EX_CLIENTEDGE.toUInt(),
		"BUTTON",
		text,
		(WS_TABSTOP or WS_VISIBLE or WS_CHILD).toUInt(),
		buttonWidth * gridX + 1,
		buttonHeight * gridY + 50,
		buttonWidth,
		buttonHeight,
		window,
		id.toLong().toCPointer(),
		null,
		null
	)
}

private fun WPARAM.loword(): Int = (this and 0xFFFFu).toInt()