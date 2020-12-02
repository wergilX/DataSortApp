#include "CSVReader.h"
#include "windows.h"

#include <exception>
#include <iostream>


CSVReader::CSVReader(const wchar_t* fileName)
{
	std::wstring data;
	char buffer[1024];
	auto bytesReaded = 0ul;


	auto file = CreateFile(fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (file == INVALID_HANDLE_VALUE)
	{
		std::string msg{ "Failed to open file. Code: " };
		msg += std::to_string(GetLastError());
		throw std::exception{ msg.c_str() };
	}

	while (ReadFile(file, buffer, sizeof(buffer), &bytesReaded, nullptr))
	{
		if (bytesReaded <= 0)
		{
			break;
		}
		std::string temp{ buffer, bytesReaded };
		data += std::wstring{ temp.cbegin(), temp.cend() };
	}

	CloseHandle(file);

	bool firstLine = true;
	auto fromData = data.cbegin();
	auto toData = std::find(data.cbegin(), data.cend(), '\n');
	while (fromData != toData)
	{
		std::wstring line(&*fromData, toData - fromData);

		// Sort line to collum

		std::vector <std::wstring> sortLine;
		auto fromLine = line.cbegin();
		auto toLine = std::find(line.cbegin(), line.cend(), ',');
		while (fromLine != toLine)
		{
			std::wstring colum(&*fromLine, toLine - fromLine);

			if (firstLine) { columnsName.push_back(colum); }
			else { sortLine.push_back(colum); }
			
			if (toLine == line.cend()) break;

			fromLine = toLine + 1;
			toLine = std::find(fromLine, line.cend(), ',');
		}

		if (firstLine == false) {
			// fill structure
			Book book = {
				sortLine[bookID],
				sortLine[title],
				sortLine[authors],
				sortLine[average_rating],
				sortLine[isbn],
				sortLine[isbn13],
				sortLine[language_code],
				sortLine[num_pages],
				sortLine[ratings_count],
				sortLine[text_reviews_count],
				sortLine[publication_date],
				sortLine[publisher]
			};

			books.push_back(book);
		}

		firstLine = false;

		if (toData == data.cend()) break;

		fromData = toData + 1;
		toData = std::find(fromData, data.cend(), '\n');
	}
}

Book& CSVReader::GetBook(int i)
{
	return books[i];
}

std::wstring CSVReader::GetColumnName(int i) const
{
	return { columnsName[i].begin(), columnsName[i].end() };
}

size_t CSVReader::GetCountColumns() const
{
	return columnsName.size();

}