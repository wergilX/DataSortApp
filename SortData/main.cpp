#pragma comment(lib, "comctl32.lib")

#include "windows.h"
#include "CommCtrl.h"
#include "CSVReader.h"

#include <string>
#include <array>

const int WIDTH_WINDOW = 1360;
const int HEIGHT_WINDOW = 768;
const int LIST_VIEW_ID = 1;


constexpr std::array<int, 12> HEADER_SIZES
{
	50,280,100,90,80,90,100,90,90,120,110,120 // width collums
};

int CmpDouble(double first, double second)
{
	if (first > second) { return 1; }
	else if (first < second) { return -1; }
	else { return 0; }
}



LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE globalHInstance;
HWND listView;
HWND window;



int CALLBACK CmpBooks(LPARAM lp1, LPARAM lp2, LPARAM columnIdx)
{

	if (columnIdx == 1 || columnIdx == 2 || 
		columnIdx == 6 || columnIdx == 10 ||
		columnIdx == 11)
	{
		wchar_t buf1[300], buf2[300];
		ListView_GetItemText(listView, lp1, columnIdx, buf1, _countof(buf1));
		ListView_GetItemText(listView, lp2, columnIdx, buf2, _countof(buf2));
		return  wcscmp(buf1, buf2);
	}
	else
	{
		wchar_t buf1[30], buf2[30];
		ListView_GetItemText(listView, lp1, columnIdx, buf1, _countof(buf1));
		ListView_GetItemText(listView, lp2, columnIdx, buf2, _countof(buf2));

		double d1 = wcstod(buf1, nullptr);
		double d2 = wcstod(buf2, nullptr);
		return  CmpDouble(d1, d2);
	}

}


int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR cmdLine, int cmdShow)
{
	
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;
	globalHInstance = wc.hInstance;
	wc.lpszClassName = L"Application";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WindowProcedure;


	if (!RegisterClassEx(&wc)) return EXIT_FAILURE;

	window = CreateWindow(
		wc.lpszClassName, L"Library", WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX) ^ WS_THICKFRAME,
		0, 0, WIDTH_WINDOW, HEIGHT_WINDOW,
		nullptr, nullptr, wc.hInstance, nullptr
	);

	if (window == INVALID_HANDLE_VALUE) return EXIT_FAILURE;

	ShowWindow(window, cmdShow);
	UpdateWindow(window);

	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{

		case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
		}
		return 0;

		case WM_NOTIFY:
		{
			switch (LOWORD(wParam))
			{
				case LIST_VIEW_ID:
				{

					NMLVDISPINFO* plvdi;
					auto code = ((LPNMHDR)lParam)->code;

					switch (code)
					{
						case  LVN_COLUMNCLICK:
						{
							auto pListView = (NMLISTVIEW*)lParam;
							BOOL test = ListView_SortItemsEx(listView, CmpBooks, pListView->iSubItem);
							break;
						}
					}
				}
			}
		}
		return 0;



		case WM_CREATE:
		{
			RECT rect;
			GetWindowRect(window, &rect);

			InitCommonControls();
			listView = CreateWindow(
				WC_LISTVIEW, L"ListView",
				WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
				0, 0, rect.right - 18, rect.bottom - 40,
				window, (HMENU)LIST_VIEW_ID, globalHInstance, 0
			);

			CSVReader lib(L"DataBooks.csv");

			LVCOLUMN column;
			size_t counter = 11;
			for (auto it = lib.HeadersBegin(); it != lib.HeadersEnd(); it++)
			{
				std::wstring colName{ it->cbegin(), it->cend() };

				ZeroMemory(&column, sizeof(column));
				column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				column.pszText = const_cast<wchar_t*>(colName.c_str());
				column.cx = HEADER_SIZES[counter--];
				ListView_InsertColumn(listView, 0, &column);
			}

			LVITEM row;
			for (size_t i = 0; i < lib.Size(); ++i)
			{
				ZeroMemory(&row, sizeof(row));
				row.mask = LVIF_COLFMT | LVIF_COLUMNS | LVIF_TEXT | LVIF_STATE;
				ListView_InsertItem(listView, &row);

			}

			for (int i = 0; i < lib.Size(); ++i)
			{
				ListView_SetItemText(listView, i, 0, (wchar_t*)lib.GetBook(i).bookID.c_str());
				ListView_SetItemText(listView, i, 1, (wchar_t*)lib.GetBook(i).title.c_str());
				ListView_SetItemText(listView, i, 2, (wchar_t*)lib.GetBook(i).authors.c_str());
				ListView_SetItemText(listView, i, 3, (wchar_t*)lib.GetBook(i).average_rating.c_str());
				ListView_SetItemText(listView, i, 4, (wchar_t*)lib.GetBook(i).isbn.c_str());
				ListView_SetItemText(listView, i, 5, (wchar_t*)lib.GetBook(i).isbn13.c_str());
				ListView_SetItemText(listView, i, 6, (wchar_t*)lib.GetBook(i).language_code.c_str());
				ListView_SetItemText(listView, i, 7, (wchar_t*)lib.GetBook(i).num_pages.c_str());
				ListView_SetItemText(listView, i, 8, (wchar_t*)lib.GetBook(i).ratings_count.c_str());
				ListView_SetItemText(listView, i, 9, (wchar_t*)lib.GetBook(i).text_reviews_count.c_str());
				ListView_SetItemText(listView, i, 10, (wchar_t*)lib.GetBook(i).publication_date.c_str());
				ListView_SetItemText(listView, i, 11, (wchar_t*)lib.GetBook(i).publisher.c_str());
			}



		}
		return 0;
	}

	
	return DefWindowProc(window, message, wParam, lParam);
}
