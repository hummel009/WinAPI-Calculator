using System.Text;

namespace Hummel
{
	public static class Program
	{
		private const int COLOR_WINDOW = 0x5;

		private const int WM_COMMAND = 0x0111;
		private const int WM_CREATE = 0x0001;
		private const int WM_CLOSE = 0x0010;
		private const int WM_DESTROY = 0x0002;

		private const int WS_VISIBLE = 0x10000000;
		private const int WS_CAPTION = 0x00C00000;
		private const int WS_SYSMENU = 0x00080000;
		private const int WS_TABSTOP = 0x10000;
		private const int WS_CHILD = 0x40000000;
		private const int WS_EX_CLIENTEDGE = 0x00000200;

		private const int BUTTON_0_ID = 0;
		private const int BUTTON_1_ID = 1;
		private const int BUTTON_2_ID = 2;
		private const int BUTTON_3_ID = 3;
		private const int BUTTON_4_ID = 4;
		private const int BUTTON_5_ID = 5;
		private const int BUTTON_6_ID = 6;
		private const int BUTTON_7_ID = 7;
		private const int BUTTON_8_ID = 8;
		private const int BUTTON_9_ID = 9;
		private const int BUTTON_C_ID = 10;
		private const int BUTTON_DIVIDE_ID = 11;
		private const int BUTTON_DOT_ID = 12;
		private const int BUTTON_EQUALS_ID = 13;
		private const int BUTTON_E_ID = 14;
		private const int BUTTON_FACTORIAL_ID = 15;
		private const int BUTTON_INVERSE_ID = 16;
		private const int BUTTON_MINUS_ID = 17;
		private const int BUTTON_MULTIPLE_ID = 18;
		private const int BUTTON_PI_ID = 19;
		private const int BUTTON_PLUS_ID = 20;
		private const int BUTTON_SQUARE_ID = 21;
		private const int BUTTON_SQUARE_ROOT_ID = 22;
		private const int BUTTON_UNARY_MINUS_ID = 23;

		private static readonly List<string> storage = [];
		private static readonly int[] factorial = [1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600];

		private static IntPtr field;

		public static void Main()
		{
			var className = "Hummel009's Calculator";
			var windowTitle = "Hummel009's Calculator";

			var windowClass = new WNDCLASS
			{
				style = 0,
				lpfnWndProc = WndProc,
				cbClsExtra = 0,
				cbWndExtra = 0,
				hInstance = 0,
				hIcon = 0,
				hCursor = 0,
				hbrBackground = new((long) COLOR_WINDOW + 1),
				lpszMenuName = "",
				lpszClassName = className
			};

			var atom = WinAPI.RegisterClass(ref windowClass);

			var screenWidth = WinAPI.GetSystemMetrics(SystemMetric.SM_CXSCREEN);
			var screenHeight = WinAPI.GetSystemMetrics(SystemMetric.SM_CYSCREEN);

			var windowWidth = 257;
			var windowHeight = 449;

			var windowX = Math.Max(0, (screenWidth - windowWidth) / 2);
			var windowY = Math.Max(0, (screenHeight - windowHeight) / 2);

			WinAPI.CreateWindowEx(0, atom, windowTitle, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, windowX, windowY, windowWidth, windowHeight, 0, 0, 0, 0);

			var msg = new MSG();
			while (WinAPI.GetMessage(out msg, 0, 0, 0) != 0)
			{
				WinAPI.TranslateMessage(ref msg);
				WinAPI.DispatchMessage(ref msg);
			}
		}

		private static IntPtr WndProc(IntPtr window, int msg, IntPtr wParam, IntPtr lParam)
		{
			switch (msg)
			{
				case WM_CREATE:
					field = RegisterField(window);

					RegisterButton(window, BUTTON_PI_ID, "p", 0, 0);
					RegisterButton(window, BUTTON_E_ID, "e", 1, 0);
					RegisterButton(window, BUTTON_C_ID, "C", 2, 0);
					RegisterButton(window, BUTTON_FACTORIAL_ID, "x!", 3, 0);
					RegisterButton(window, BUTTON_INVERSE_ID, "1/x", 0, 1);
					RegisterButton(window, BUTTON_SQUARE_ID, "x^2", 1, 1);
					RegisterButton(window, BUTTON_SQUARE_ROOT_ID, "sqrt(x)", 2, 1);
					RegisterButton(window, BUTTON_DIVIDE_ID, "/", 3, 1);
					RegisterButton(window, BUTTON_7_ID, "7", 0, 2);
					RegisterButton(window, BUTTON_8_ID, "8", 1, 2);
					RegisterButton(window, BUTTON_9_ID, "9", 2, 2);
					RegisterButton(window, BUTTON_MULTIPLE_ID, "*", 3, 2);
					RegisterButton(window, BUTTON_4_ID, "4", 0, 3);
					RegisterButton(window, BUTTON_5_ID, "5", 1, 3);
					RegisterButton(window, BUTTON_6_ID, "6", 2, 3);
					RegisterButton(window, BUTTON_MINUS_ID, "-", 3, 3);
					RegisterButton(window, BUTTON_1_ID, "1", 0, 4);
					RegisterButton(window, BUTTON_2_ID, "2", 1, 4);
					RegisterButton(window, BUTTON_3_ID, "3", 2, 4);
					RegisterButton(window, BUTTON_PLUS_ID, "+", 3, 4);
					RegisterButton(window, BUTTON_UNARY_MINUS_ID, "-", 0, 5);
					RegisterButton(window, BUTTON_0_ID, "0", 1, 5);
					RegisterButton(window, BUTTON_DOT_ID, ",", 2, 5);
					RegisterButton(window, BUTTON_EQUALS_ID, "=", 3, 5);
					break;
				case WM_COMMAND:
					var buttonId = Loword(wParam);

					switch (buttonId)
					{
						case BUTTON_0_ID:
							PushSymbolWrapper("0");
							break;
						case BUTTON_1_ID:
							PushSymbolWrapper("1");
							break;
						case BUTTON_2_ID:
							PushSymbolWrapper("2");
							break;
						case BUTTON_3_ID:
							PushSymbolWrapper("3");
							break;
						case BUTTON_4_ID:
							PushSymbolWrapper("4");
							break;
						case BUTTON_5_ID:
							PushSymbolWrapper("5");
							break;
						case BUTTON_6_ID:
							PushSymbolWrapper("6");
							break;
						case BUTTON_7_ID:
							PushSymbolWrapper("7");
							break;
						case BUTTON_8_ID:
							PushSymbolWrapper("8");
							break;
						case BUTTON_9_ID:
							PushSymbolWrapper("9");
							break;
						case BUTTON_E_ID:
							PushSymbolWrapper("2.72");
							break;
						case BUTTON_PI_ID:
							PushSymbolWrapper("3.14");
							break;
						case BUTTON_DOT_ID:
							PushSymbolWrapper(",");
							break;
						case BUTTON_UNARY_MINUS_ID:
							PushSymbolWrapper("-");
							break;
						case BUTTON_C_ID:
							storage!.Clear();
							WinAPI.SetWindowText(field, "");
							break;
						case BUTTON_DIVIDE_ID:
							PushOperation("/");
							break;
						case BUTTON_MULTIPLE_ID:
							PushOperation("*");
							break;
						case BUTTON_MINUS_ID:
							PushOperation("-");
							break;
						case BUTTON_PLUS_ID:
							PushOperation("+");
							break;
						case BUTTON_FACTORIAL_ID:
							PushOperation("!");
							break;
						case BUTTON_SQUARE_ID:
							PushOperation("s");
							break;
						case BUTTON_INVERSE_ID:
							PushOperation("i");
							break;
						case BUTTON_SQUARE_ROOT_ID:
							PushOperation("r");
							break;
						case BUTTON_EQUALS_ID:
							CalculateWrapper();
							break;
					}
					break;
				case WM_CLOSE:
					WinAPI.DestroyWindow(window);
					break;
				case WM_DESTROY:
					WinAPI.PostQuitMessage(0);
					break;
			}
			return WinAPI.DefWindowProc(window, msg, wParam, lParam);
		}

		private static void CalculateWrapper()
		{
			try
			{
				Calculate(field);
			}
			catch (Exception)
			{
				storage!.Clear();
				WinAPI.SetWindowText(field, "Error!");
			}
		}

		private static void Calculate(IntPtr field)
		{
			StringBuilder buffer = new(WinAPI.GetWindowTextLength(field) + 1);
			WinAPI.GetWindowText(field, buffer, buffer.Capacity);
			var str = buffer.ToString();

			if (storage!.Count == 2)
			{
				var op = storage[1];

				if (new HashSet<string> { "+", "-", "*", "/" }.Contains(op))
				{
					storage.Add(str);

					var operand1 = double.Parse(storage[0]);
					var operand2 = double.Parse(storage[2]);

					var result = op switch
					{
						"+" => operand1 + operand2,
						"-" => operand1 - operand2,
						"*" => operand1 * operand2,
						"/" => operand1 / operand2,
						_ => throw new Exception("Invalid operator: " + op)
					};

					storage.Clear();

					WinAPI.SetWindowText(field, result.ToString());
				}
				else if (new HashSet<string> { "!", "s", "i", "r" }.Contains(op))
				{
					var operand = double.Parse(storage[0]);

					var result = op switch
					{
						"!" => factorial[(int)operand],
						"s" => operand * operand,
						"i" => 1.0 / operand,
						"r" => Math.Sqrt(operand),
						_ => throw new Exception("Invalid operator: " + op)
					};

					storage.Clear();

					WinAPI.SetWindowText(field, result.ToString());
				}
			}
		}

		private static void PushOperation(string operation)
		{
			StringBuilder buffer = new(WinAPI.GetWindowTextLength(field) + 1);
			WinAPI.GetWindowText(field, buffer, buffer.Capacity);
			var str = buffer.ToString();

			if (storage!.Count == 0)
			{
				storage.Add(str);
				storage.Add(operation);
				WinAPI.SetWindowText(field, "");
			}
			else
			{
				storage.Clear();
				WinAPI.SetWindowText(field, "Error!");
			}
		}

		private static void PushSymbolWrapper(string symbol)
		{
			StringBuilder buffer = new(WinAPI.GetWindowTextLength(field) + 1);
			WinAPI.GetWindowText(field, buffer, buffer.Capacity);
			var str = buffer.ToString();

			switch (symbol)
			{
				case "3.14":
				case "2.72":
				case "-":
					if (string.IsNullOrEmpty(str))
					{
						PushSymbol(symbol);
					}
					break;

				case ".":
					if (!str.Contains('.') && !string.IsNullOrEmpty(str))
					{
						PushSymbol(symbol);
					}
					break;
				default:
					if (storage!.Count == 0)
					{
						PushSymbol(symbol);
					}
					else
					{
						var op = storage[1];

						if (!new HashSet<string> { "!", "s", "i", "r" }.Contains(op))
						{
							PushSymbol(symbol);
						}
					}
					break;
			}
		}

		private static void PushSymbol(string symbol)
		{
			StringBuilder buffer = new(WinAPI.GetWindowTextLength(field) + 1);
			WinAPI.GetWindowText(field, buffer, buffer.Capacity);
			var str = buffer.ToString();
			if ("Error!" == str)
			{
				WinAPI.SetWindowText(field, symbol);
			}
			else
			{
				WinAPI.SetWindowText(field, str + symbol);
			}
		}

		private static IntPtr RegisterField(IntPtr window)
		{
			return WinAPI.CreateWindowEx(WS_EX_CLIENTEDGE, "STATIC", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 1, 1, 239, 48, window, 0, 0, 0);
		}

		private static void RegisterButton(IntPtr window, int id, string text, int gridX, int gridY)
		{
			var buttonWidth = 60;
			var buttonHeight = 60;

			WinAPI.CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", text, WS_TABSTOP | WS_VISIBLE | WS_CHILD, buttonWidth * gridX + 1, buttonHeight * gridY + 50, buttonWidth, buttonHeight, window, new((long)id), 0, 0);
		}

		private static int Loword(IntPtr wparam)
		{
			return (int)(wparam.ToInt64() & 0xFFFF);
		}
	}
}