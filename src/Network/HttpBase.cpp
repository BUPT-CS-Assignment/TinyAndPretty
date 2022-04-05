#include <Network/HttpProtocal.h>

char *nsplit(char *str, const char *token, int n);

#define CUR_MOV(offset) \
    cur += offset;      \
    if (cur >= len)     \
        break;

void StringDict::__init__(char *str, const char *token_1, const char *token_2)
{
    char *fir, *sec;
    len = strlen(str);

    size_t cur = 0;
    size_t len_1 = strlen(token_1), len_2 = strlen(token_2);

    while (true)
    {
        fir = nsplit(str + cur, token_1, len_1);
        CUR_MOV(strlen(fir) + len_1);            // std::cerr << fir << "\n";
        sec = nsplit(str + cur, token_2, len_2); // std::cerr << sec << "\n";
        item.push_back(std::make_pair(std::string(fir), std::string(sec)));
        CUR_MOV(strlen(sec) + len_2);
    }
}

std::string &StringDict::get(std::string_view str)
{
    for (auto &it : item)
    {
        if (it.first == str)
            return it.second;
    }
    throw HttpException::NON_POS;
}

void StringDict::push(std::string _fir, std::string _sec)
{
    item.push_back(std::make_pair(std::move(_fir), std::move(_sec)));
}

void StringDict::show()
{
    for (auto &it : item)
    {
        std::cout << it.first << "---" << it.second << "\n";
    }
}

std::string &StringDict::operator[](const std::string &str)
{
    try
    {
        return get(str);
    }
    catch (int e)
    {
        item.push_back(std::make_pair(str, ""));
        return (item.end() - 1)->second;
    }
}

size_t StringDict::stringize(char *buff)
{
    size_t cur = 0;

    for (auto &it : item)
    {
        strcpy(buff + cur, it.first.c_str());
        cur += it.first.length();
        strcpy(buff + cur, ": ");
        cur += 2;
        strcpy(buff + cur, it.second.c_str());
        cur += it.second.length();
        strcpy(buff + cur, "\r\n");
        cur += 2;
    }
    strcpy(buff + cur, "\r\n");
    cur += 2;
    return cur;
}

FormItem::FormItem(uint8_t *_begin, uint8_t *_end)
{
    nsplit((char *)_begin, "\r\n\r\n", 4);

    headers = std::make_unique<StringDict>((char *)_begin, ": ", "\r\n");
    std::string &disposition = headers->get("Content-Disposition");

    if (size_t t_pos = disposition.find("filename"); t_pos != disposition.npos)
    {
        filename = disposition.substr(t_pos + 9); // 9 : sizeof "filename="
        filename = filename.substr(1, filename.length() - 2); // erase ""
        disposition = disposition.substr(0, t_pos - 2); // 2 : sizeof "; "
    }
    if (size_t t_pos = disposition.find("name"); t_pos != disposition.npos)
    {
        name = disposition.substr(t_pos + 5); // 5 : sizeof "name="
        name = name.substr(1, name.length() - 2);// erase ""
        disposition = disposition.substr(0, t_pos - 2);// 2 : sizeof "; "
    }
    std::cerr << "Name : " << name << " & FileName : " << filename << "\n";
    _begin += headers->length() + 4;

    len = _end - _begin;
    std::cerr << "Item Len : " << len << std::endl;
    data = std::make_unique<uint8_t[]>(len);
    memcpy(data.get(), _begin, len);
}

std::fstream &operator<<(std::fstream &out, const FormItem &_this)
{
    out.write((char *)(_this.data.get()), _this.len);
    return out;
}
std::ofstream &operator<<(std::ofstream &out, const FormItem &_this)
{
    out.write((char *)(_this.data.get()), _this.len);
    return out;
}

void FormItem::show()
{
    for (size_t i = 0; i < len; i++)
    {
        putchar(data[i]);
    }
}

FormData::FormData(std::string &_boundary, uint8_t *body, size_t len) : boundary(std::move(_boundary))
{
    std::cerr << "String Boundary: " << boundary << " Len: " << len << "\n";
    uint8_t *fir = nullptr;

    for (size_t cur = 0; cur < len; cur++)
    {
        if (body[cur] == '-' && body[cur + 1] == '-' &&
            memcmp(body + cur + 2, boundary.c_str(), boundary.length()) == 0)
        {
            if (fir != nullptr)
                form.emplace_back(fir, body + cur - 1);
            cur += 4 + boundary.length(); // 4 : sizeof "--" "\r\n"
            fir = body + cur;
        }
    }
    std::cerr << "Finish Form Data\n";
}

FormItem &FormData::queryItem(std::string_view _name)
{
    for (auto &it : form)
    {
        if (it.name == _name)
            return it;
    }
    throw HttpException::NON_POS;
}
