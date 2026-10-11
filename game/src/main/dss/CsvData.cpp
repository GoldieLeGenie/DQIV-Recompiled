#include "main/dss/CsvData.hpp"

ARM void CsvRow::unkfunc_020571c4()
{
    for (int i = 0; i < 8; i++) {
        column_[i] = -1;
    }
}

ARM void CsvRow::unkfunc_020571e4(int index, int offset)
{
    column_[index] = offset;
}

ARM short CsvRow::unkfunc_020571f0(int index)
{
    return column_[index];
}

ARM CsvData::CsvData() : text_(0)
{
}

ARM CsvData::~CsvData()
{
}

ARM void CsvData::unkfunc_02057234(const char* filename, int a)
{
    data_.setup(filename, a, 1);
    text_ = (char*)data_.getAddr();
    size_ = data_.getSize();
}

ARM void CsvData::unkfunc_0205726c(const char* filename, int a)
{
    unkfunc_02057234(filename, a);
    row_ = (CsvRow*)unkfunc_0207f834(&data_0211a60c, 0x2800, -0x20);
    for (int i = 0; i < 0x280; i++) {
        row_[i].unkfunc_020571c4();
    }
    int pos = 0;
    int row = 0;
    int column = 0;
    row_[row].unkfunc_020571e4(column, pos);
    char* text = text_;
    do {
        if (text[pos] == ',') {
            text[pos] = 0;
            pos++;
            column++;
            row_[row].unkfunc_020571e4(column, pos);
        } else if (text[pos] == '\r' && text[pos + 1] == '\n') {
            text[pos] = 0;
            text[pos + 1] = 0;
            pos += 2;
            row++;
            column = 0;
            row_[row].unkfunc_020571e4(column, pos);
        } else {
            pos++;
        }
    } while (pos < size_);
    rowCount_ = row;
}

ARM char* CsvData::unkfunc_02057368(int row, int column)
{
    if (rowCount_ <= row) {
        return 0;
    }
    if (row_[row].unkfunc_020571f0(column) == -1) {
        return 0;
    }
    return text_ + row_[row].unkfunc_020571f0(column);
}
