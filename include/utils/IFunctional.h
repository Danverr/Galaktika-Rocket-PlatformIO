#pragma once

namespace blip {

template< typename ReturnValue, typename ... Args >
class IFunctional {
public:
    virtual ReturnValue operator() (Args... args) = 0;
    virtual ~IFunctional() {}
};

template< typename ReturnValue, typename ... Args >
class FunctionFunctional : public IFunctional< ReturnValue, Args... > {
private:
    typedef ReturnValue (*functionType)(Args...);
    functionType functionPtr;

public:
    FunctionFunctional(functionType _functionPtr) : functionPtr(_functionPtr) {}

    ReturnValue operator() (Args... args) {
        return (*functionPtr)(args...);
    }
};

template< typename T, typename ReturnValue, typename ... Args >
class FunctorFunctional : public IFunctional< ReturnValue, Args... > {
private:
    T* objectPtr;

public:
    FunctorFunctional(T* _objectPtr) : objectPtr(_objectPtr) {}

    ReturnValue operator() (Args... args) {
        return (*objectPtr)(args...);
    }
};

template< typename T, typename ReturnValue, typename ... Args >
class ClassMemberFunctional : public IFunctional< ReturnValue, Args... > {
private:
    typedef ReturnValue (T::*functionType)(Args...);
    functionType functionPtr;
    T* objectPtr;

public:
    ClassMemberFunctional(functionType _functionPtr, T* _objectPtr)
        : objectPtr(_objectPtr), functionPtr(_functionPtr) {}

    ReturnValue operator() (Args... args) {
        return (objectPtr->*functionPtr)(args...);
    }
};

template< typename ReturnValue, typename ... Args >
IFunctional<ReturnValue, Args...>* toFunctional(ReturnValue (*function)(Args...)) {
    return new FunctionFunctional<ReturnValue, Args...>(function);
}

template< typename T, typename ReturnValue, typename ... Args >
IFunctional<ReturnValue, Args...>* toFunctional(T* ptr) {
    return new FunctorFunctional<T, ReturnValue, Args...>(ptr);
}

template< typename T, typename ReturnValue, typename ... Args >
IFunctional<ReturnValue, Args...>* toFunctional(ReturnValue (T::* function)(Args...), T* ptr) {
    return new ClassMemberFunctional<T, ReturnValue, Args...>(function, ptr);
}

}
