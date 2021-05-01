#pragma once

#ifndef ARDUINO
    #include <typeinfo>
#endif

namespace blip {

/**
 * UncertainTuple хранит любые типы данных.
 * При взятии данных должны использоваться те же типы, что при создании кортежа.
 * Кортеж хранит копии объектов.
 * Кортеж неизменяем.
 */
class UncertainTuple {
public:
    const unsigned int VALUE_COUNT;

private:
    struct UncertainTupleAdapter {};

    template<class ValueType>
    struct UncertainTupleContainer : public UncertainTupleAdapter {
        ValueType value;
        UncertainTupleContainer(ValueType& _value) : value(_value) {}
    };

    UncertainTupleAdapter** adapters;

    template<class FirstType, class... OtherTypes>
    void addValues(unsigned int current, FirstType& first, OtherTypes&... other);
    void addValues(unsigned int current);

    template<class FirstType, class... OtherTypes>
    void getValues(unsigned int current, FirstType& first, OtherTypes&... other);
    void getValues(unsigned int current);

public:
    template<class... ValueTypes>
    UncertainTuple(ValueTypes... values);

    template<class... VariableTypes>
    void get(VariableTypes&... variables);

    ~UncertainTuple();
};

}

template<class... ValueTypes>
blip::UncertainTuple::UncertainTuple(ValueTypes... _values) : VALUE_COUNT(sizeof... (ValueTypes)) {
    adapters = new UncertainTupleAdapter*[VALUE_COUNT];
    addValues(0, _values...);
}

template<class... VariableTypes>
void blip::UncertainTuple::get(VariableTypes&... variables) {
    getValues(0, variables...);
}

template<class FirstType, class... OtherTypes>
void blip::UncertainTuple::addValues(unsigned int current, FirstType& first, OtherTypes&... other) {
    adapters[current] = new UncertainTupleContainer<FirstType>(first);
    addValues(current + 1, other...);
}

void blip::UncertainTuple::addValues(unsigned int current) {}

template<class FirstType, class... OtherTypes>
void blip::UncertainTuple::getValues(unsigned int current, FirstType& first, OtherTypes&... other) {
    UncertainTupleContainer<FirstType>* container = static_cast<UncertainTupleContainer<FirstType>*>(adapters[current]);
    first = container->value;
    getValues(current + 1, other...);
}

void blip::UncertainTuple::getValues(unsigned int current) {}

blip::UncertainTuple::~UncertainTuple() {
    for (unsigned int i = 0; i < VALUE_COUNT; ++i) {
        delete adapters[i];
    }
    delete adapters;
}
