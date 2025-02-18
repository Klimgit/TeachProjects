#include <iostream>
#include <string>
#include <vector>

std::vector<int> prefixFunc(std::string str, std::vector<int>& arr)
{
    arr[0] = 0;
    for (int i = 1; i < str.length(); ++i)
    {
        int k = arr[i - 1];
        while (k > 0 && str[i] != str[k])
        {
            k = arr[k - 1];
        }
        if (str[i] == str[k])
        {
            k++;
        }
        arr[i] = k;
    }

    return arr;
}

int main()
{
    std::string str;
    std::cin >> str;
    std::vector<int> arr(str.length());

    std::vector<int> ans = prefixFunc(str, arr);
    for (int i = 0; i < str.length(); ++i)
    {
        std::cout << arr[i] << " ";
    }

    return 0;
}
