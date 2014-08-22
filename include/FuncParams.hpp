#ifndef _FuncParams_hpp_
#define _FuncParams_hpp_

// http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer

#include <tuple>

template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };

template <typename ...Args>
struct FuncParams
{
    public:
        std::tuple<Args...> params;
        double (*func)(Args...);

        double delayed_dispatch()
        {
            return callFunc(typename gens<sizeof...(Args)>::type());
        }

        template<int ...S>
        double callFunc(seq<S...>)
        {
            return func(std::get<S>(params) ...);
        }
};

#endif /* _FuncParams_hpp_ */
