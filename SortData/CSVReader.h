#pragma once
#include <string>
#include <vector>


struct Book
{
	std::wstring bookID;
	std::wstring title;
	std::wstring authors;
	std::wstring average_rating;
	std::wstring isbn;
	std::wstring isbn13;
	std::wstring language_code;
	std::wstring num_pages;
	std::wstring ratings_count;
	std::wstring text_reviews_count;
	std::wstring publication_date;
	std::wstring publisher;
};

enum ColumnsID
{
	bookID,
	title,
	authors,
	average_rating,
	isbn,
	isbn13,
	language_code,
	num_pages,
	ratings_count,
	text_reviews_count,
	publication_date,
	publisher,

};



class CSVReader
{
public:

	CSVReader(const wchar_t* File);

	Book& GetBook(int i);

	std::wstring GetColumnName(int i) const;

	size_t GetCountColumns() const;

	size_t Size() const
	{
		return books.size();
	}

	auto HeadersBegin()
	{
		return columnsName.crbegin();
	}

	auto HeadersEnd()
	{
		return columnsName.crend();
	}

private:
	std::vector<Book> books;
	std::vector<std::wstring> columnsName;

};

