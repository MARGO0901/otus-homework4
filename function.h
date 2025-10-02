#include <iomanip>
#include <iostream>
#include <list>
#include <stdint.h>
#include <tuple>
#include <type_traits>
#include <vector>

constexpr bool verbose = false;
#define LOG                                                                    \
  if (verbose)                                                                 \
  std::cout

/**
    \brief Преобразовывает целочисленное значения в строку в виде ip-адреса

    Проверяя размер входного параметра, выбирает нужную ветку для печати ip адреса

    \tparam T целочисленный тип 
*/
template <typename T>
typename std::enable_if_t<std::is_integral<T>::value, void> 
print_ip(T value) {
    std::size_t size = sizeof(T);
    if (size == 1) {
        LOG << "uint8_t = ";
        std::cout << static_cast<int>(value) << std::endl;
    } else if (size == 2) {
        LOG << "uint16_t ";
        std::cout << std::fixed << std::setprecision(1) << static_cast<float>(value)
                << std::endl;
    } else if (size == 4 || size == 8) {
        std::size_t n = size * 8;
        while (n) {
        if (n != size * 8)
            std::cout << ".";
        n -= 8;
        std::cout << ((value >> n) & 0xFF);
        }
        std::cout << std::endl;
    } else {
        throw "Unabled type!";
    }
}

/**
    \brief Выводит на печать строку

    Шаблонная функция для типа T, являющегося std::string, выводит на экран переданную в параметре строку

    \tparam T  std::string
    \param container константная ссылка на std::string
*/
template <typename T>
typename std::enable_if_t<std::is_same<T, std::string>::value, void>
print_ip(const T &container) {
    LOG << "String " << std::endl;
    for (auto value : container)
        std::cout << value;
    std::cout << std::endl;
}

/**
    \brief Определяет, является ли тип Т типом std::vector
 */
template <typename T> struct is_vector : std::false_type {};

template <typename... Args>
struct is_vector<std::vector<Args...>> : std::true_type {};

/**
    \brief Определяет, является ли тип Т типом std::list
*/

template <typename T> struct is_list : std::false_type {};

template <typename... Args>
struct is_list<std::list<Args...>> : std::true_type {};

/**
    \brief Выводит на печать контейнера в виде ip-адреса

    Шаблонная функция для вывода элемента контейнера со вставкой точки '.' перед элементом, если его индекс не равен нулю

    \tparam T std::vector или std::list
    \param container константная ссылка на std::vector или std::string
*/
template <typename T>
typename std::enable_if_t<is_vector<T>::value || is_list<T>::value, void>
print_ip(const T &container) {
    if constexpr (is_vector<T>::value) {
        LOG << "Vector" << std::endl;
        for (auto i = 0; i < container.size(); ++i) {
        if (i != 0)
            std::cout << ".";
        std::cout << container[i];
        }
        std::cout << std::endl;
    }
    if constexpr (is_list<T>::value) {
        LOG << "List" << std::endl;
        bool first = true;
        for (const auto &elem : container) {
        if (!first)
            std::cout << ".";
        std::cout << elem;
        first = false;
        }
        std::cout << std::endl;
    }
}

//-------------------------------
/**
    \brief Определяет, является ли Т типом std::tuple
*/
template <typename T> struct is_tuple : std::false_type {};

template <typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

/**
    \brief Выводит элемент с условным разделителем

    Шаблонная функция для вывода элемента со вставкой точки '.' перед элементом, если его индекс `I` больше нуля.

    \param I Индекс текущего элемента
    \tparam T Тип элемента для вывода
    \param x Константная ссылка на элемент, который нужно вывести
*/
template <std::size_t I, typename T>
void printElem(const T &x) {
    if constexpr (I > 0)
        std::cout << '.';
    std::cout << x;
}

/**
    \brief Выводит на печать элементы кортежа с разделителем

    Функция принимает кортеж 'tp' и последоваетльность символов 'Is...', затем выводит каждый элемент с помощью функции 'printElem'

    \tparam TupleT тип std::tuple
    \param Is... набор индексов элементов для обхода кортежа
    \param tp константная ссылка на кортеж, элементы которого нужно вывести
    \param std::index_sequence<Is...> набор индексов, используемых для доступа к элементам кортежа
*/
template <typename TupleT, std::size_t... Is>
void printTupleManual(const TupleT &tp, std::index_sequence<Is...>) {
    (printElem<Is>(std::get<Is>(tp)), ...);
}

// проверка однотипности типов кортежа
/**
    \brief Проверяет однотипность списка типа Т

    Проверяет, являются ли все типы в списке 'Args...' типом 'Т'. В случае равенства возвращает true

    \param T тип нулевого элемента кортежа, с которым сравниваются все остальные
    \param Args Остальные типы для сравнения
*/
template <typename T, typename... Args>
struct all_type_is_same {
    static constexpr bool value = (std::is_same_v<T, Args> && ...);
};

/**
    \brief Проверяет, содержит ли кортеж только один тип во всех элементах
 
    Шаблонная структура, которая определяет, состоит ли весь кортеж
    `std::tuple<T, Args...>` из элементов одного типа `T`. Использует
    `all_type_is_same` для проверки.

    \tparam T Тип первого элемента кортежа
    \tparam Args Остальные типы кортежа
 */
template <typename T, typename... Args> 
struct tuple_has_only_one_type;

template <typename T, typename... Args>
struct tuple_has_only_one_type<std::tuple<T, Args...>> {
    static constexpr bool value = all_type_is_same<T, Args...>::value;
};

/**
    \brief Проверяет, являются ли элементы кортежа элементами одного типа, и выводит на печать элементы, разделяя ий точкой (.)

    Если элементы кортежа 'container' не являются элементами одного типа, то в compile-time выбрасывается исключение "Not same type!"
    В противном случае происходит печать элементов кортежа

    \tparam T std::tuple
    \param container константная ссылка на контейнер типа std::tuple
*/
template <typename T>
typename std::enable_if_t<is_tuple<T>::value, void>
print_ip(const T &container) {
    LOG << "Tuple " << std::endl;

    using container_type = std::remove_cv_t<std::remove_reference_t<decltype(container)>>;
    static_assert(tuple_has_only_one_type<container_type>::value,
                    "Not same type!");

    using tupleType = std::remove_cv_t<std::remove_const_t<T>>;
    printTupleManual(container,
                    std::make_index_sequence<std::tuple_size_v<tupleType>>{});
}
